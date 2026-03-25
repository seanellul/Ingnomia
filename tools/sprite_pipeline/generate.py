"""
Sprite generator for Ingnomia using Stable Diffusion on Apple Silicon.

Uses the 'All-In-One-Pixel-Model' (SD 1.5 fine-tune for pixel art) via
HuggingFace diffusers with MPS backend. Generates sprites from text prompts,
then post-processes them to match Ingnomia's 32x36 isometric sprite format.

Usage:
    # Generate all sprites from manifest
    python generate.py

    # Generate specific sprite by ID
    python generate.py --id terrain_stone_wall

    # Generate with custom model
    python generate.py --model "Onodofthenorth/SD_PixelArt_SpriteSheet_Generator"

    # Quick test with fewer steps
    python generate.py --steps 15 --id item_sword
"""

import argparse
import json
import os
import sys
import time
from pathlib import Path

import torch
from diffusers import StableDiffusionPipeline, DPMSolverMultistepScheduler
from PIL import Image

from postprocess import process_sprite


SCRIPT_DIR = Path(__file__).parent
MANIFEST_PATH = SCRIPT_DIR / "manifest.json"
OUTPUT_DIR = SCRIPT_DIR / "output" / "individual"


def load_manifest(path: str = None) -> dict:
    """Load the sprite generation manifest."""
    p = Path(path) if path else MANIFEST_PATH
    with open(p) as f:
        return json.load(f)


def create_pipeline(model_id: str, device: str = None) -> StableDiffusionPipeline:
    """Load the SD pipeline with optimal settings for Apple Silicon."""
    if device is None:
        if torch.backends.mps.is_available():
            device = "mps"
        elif torch.cuda.is_available():
            device = "cuda"
        else:
            device = "cpu"

    print(f"Loading model: {model_id}")
    print(f"Device: {device}")

    pipe = StableDiffusionPipeline.from_pretrained(
        model_id,
        # MPS has float16 issues that produce black images — use float32
        torch_dtype=torch.float32,
        safety_checker=None,
        requires_safety_checker=False,
    )

    # Use DPM++ 2M Karras for better quality at fewer steps
    pipe.scheduler = DPMSolverMultistepScheduler.from_config(
        pipe.scheduler.config,
        use_karras_sigmas=True,
    )

    pipe = pipe.to(device)

    # Memory optimizations for 16GB Mac
    pipe.enable_attention_slicing()
    if hasattr(pipe, "enable_vae_slicing"):
        pipe.enable_vae_slicing()

    return pipe


def generate_sprite(
    pipe: StableDiffusionPipeline,
    prompt: str,
    negative_prompt: str = "",
    num_inference_steps: int = 30,
    guidance_scale: float = 7.5,
    seed: int = None,
    size: int = 512,
) -> Image.Image:
    """Generate a single sprite image."""
    generator = None
    if seed is not None:
        generator = torch.Generator(device=pipe.device.type)
        generator.manual_seed(seed)

    result = pipe(
        prompt=prompt,
        negative_prompt=negative_prompt,
        num_inference_steps=num_inference_steps,
        guidance_scale=guidance_scale,
        generator=generator,
        height=size,
        width=size,
    )

    return result.images[0]


def generate_rotations(
    pipe: StableDiffusionPipeline,
    base_prompt: str,
    negative_prompt: str = "",
    num_inference_steps: int = 30,
    guidance_scale: float = 7.5,
    seed: int = None,
) -> dict[str, Image.Image]:
    """Generate 4 rotation variants (FR, FL, BL, BR) for isometric sprites."""
    directions = {
        "FR": "front-right facing",
        "FL": "front-left facing",
        "BL": "back-left facing, seen from behind",
        "BR": "back-right facing, seen from behind",
    }

    results = {}
    for suffix, direction in directions.items():
        prompt = base_prompt.replace("isometric", f"isometric {direction}")
        img = generate_sprite(
            pipe, prompt, negative_prompt,
            num_inference_steps=num_inference_steps,
            guidance_scale=guidance_scale,
            seed=seed,
        )
        results[suffix] = img
        if seed is not None:
            seed += 1

    return results


def run_manifest(
    manifest: dict,
    pipe: StableDiffusionPipeline,
    output_dir: Path,
    sprite_ids: list[str] = None,
    steps_override: int = None,
    seed: int = 42,
):
    """Generate all sprites defined in the manifest."""
    settings = manifest["global_settings"]
    sprites = manifest["sprites"]

    if sprite_ids:
        sprites = [s for s in sprites if s["id"] in sprite_ids]
        if not sprites:
            print(f"No sprites found matching IDs: {sprite_ids}")
            return

    steps = steps_override or settings.get("inference_steps", 30)
    guidance = settings.get("guidance_scale", 7.5)

    total_images = sum(
        s.get("variants", 1) * (4 if s.get("rotations") else 1)
        for s in sprites
    )
    print(f"\nGenerating {total_images} images for {len(sprites)} sprite definitions")
    print(f"Steps: {steps}, Guidance: {guidance}\n")

    generated = 0
    start_time = time.time()

    for sprite_def in sprites:
        sid = sprite_def["id"]
        category = sprite_def.get("category", "misc")
        prompt = sprite_def["prompt"]
        neg = sprite_def.get("negative_prompt", "")
        grayscale = sprite_def.get("grayscale", False)
        variants = sprite_def.get("variants", 1)
        rotations = sprite_def.get("rotations", False)

        cat_dir = output_dir / category
        cat_dir.mkdir(parents=True, exist_ok=True)

        print(f"[{sid}] generating...", end=" ", flush=True)

        for v in range(variants):
            variant_seed = seed + generated

            if rotations:
                rot_images = generate_rotations(
                    pipe, prompt, neg,
                    num_inference_steps=steps,
                    guidance_scale=guidance,
                    seed=variant_seed,
                )
                for rot_name, raw_img in rot_images.items():
                    processed = process_sprite(raw_img, grayscale=grayscale)
                    suffix = f"_v{v}_{rot_name}" if variants > 1 else f"_{rot_name}"
                    out_path = cat_dir / f"{sid}{suffix}.png"
                    processed.save(out_path)
                    generated += 1

                    # Also save raw for comparison
                    raw_dir = output_dir / "_raw" / category
                    raw_dir.mkdir(parents=True, exist_ok=True)
                    raw_img.save(raw_dir / f"{sid}{suffix}_raw.png")
            else:
                raw_img = generate_sprite(
                    pipe, prompt, neg,
                    num_inference_steps=steps,
                    guidance_scale=guidance,
                    seed=variant_seed,
                )
                processed = process_sprite(raw_img, grayscale=grayscale)
                suffix = f"_v{v}" if variants > 1 else ""
                out_path = cat_dir / f"{sid}{suffix}.png"
                processed.save(out_path)
                generated += 1

                # Save raw
                raw_dir = output_dir / "_raw" / category
                raw_dir.mkdir(parents=True, exist_ok=True)
                raw_img.save(raw_dir / f"{sid}{suffix}_raw.png")

        elapsed = time.time() - start_time
        rate = generated / elapsed if elapsed > 0 else 0
        print(f"done ({generated}/{total_images}, {rate:.1f} img/s)")

    elapsed = time.time() - start_time
    print(f"\nComplete: {generated} images in {elapsed:.1f}s")
    print(f"Output: {output_dir}")


def main():
    parser = argparse.ArgumentParser(description="Generate Ingnomia sprites")
    parser.add_argument("--manifest", type=str, default=None, help="Path to manifest JSON")
    parser.add_argument("--model", type=str, default=None, help="HuggingFace model ID override")
    parser.add_argument("--id", type=str, nargs="+", help="Generate only these sprite IDs")
    parser.add_argument("--steps", type=int, default=None, help="Override inference steps")
    parser.add_argument("--seed", type=int, default=42, help="Random seed")
    parser.add_argument("--output", type=str, default=None, help="Output directory")
    parser.add_argument("--device", type=str, default=None, help="Device (mps/cuda/cpu)")
    args = parser.parse_args()

    manifest = load_manifest(args.manifest)

    model_id = args.model or manifest["global_settings"]["model"]
    output_dir = Path(args.output) if args.output else OUTPUT_DIR

    pipe = create_pipeline(model_id, device=args.device)
    run_manifest(
        manifest, pipe, output_dir,
        sprite_ids=args.id,
        steps_override=args.steps,
        seed=args.seed,
    )

    # Auto-pack into tilesheets
    from pack import pack_by_category
    tilesheet_dir = SCRIPT_DIR / "output" / "tilesheets"
    print("\nPacking into tilesheets...")
    pack_by_category(str(output_dir), str(tilesheet_dir))


if __name__ == "__main__":
    main()
