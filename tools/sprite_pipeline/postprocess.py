"""
Post-processing for generated sprites.
Handles: resize to 32x36, background removal, palette enforcement,
grayscale conversion, and transparency cleanup.
"""

import numpy as np
from PIL import Image


# Ingnomia sprite cell dimensions
CELL_W = 32
CELL_H = 36


def remove_background(img: Image.Image, threshold: int = 20) -> Image.Image:
    """Remove near-uniform background and replace with transparency."""
    img = img.convert("RGBA")
    data = np.array(img)

    # Sample corners to estimate background color
    h, w = data.shape[:2]
    corners = [
        data[0, 0, :3],
        data[0, w - 1, :3],
        data[h - 1, 0, :3],
        data[h - 1, w - 1, :3],
    ]
    bg_color = np.median(corners, axis=0).astype(np.uint8)

    # Create mask: pixels close to background color become transparent
    diff = np.abs(data[:, :, :3].astype(int) - bg_color.astype(int))
    mask = np.all(diff < threshold, axis=2)
    data[mask, 3] = 0

    return Image.fromarray(data)


def resize_to_cell(img: Image.Image, width: int = CELL_W, height: int = CELL_H) -> Image.Image:
    """Resize image to target cell size, preserving aspect ratio and centering."""
    img = img.convert("RGBA")

    # Find the bounding box of non-transparent content
    bbox = img.getbbox()
    if bbox is None:
        return Image.new("RGBA", (width, height), (0, 0, 0, 0))

    content = img.crop(bbox)
    cw, ch = content.size

    # Scale to fit within cell, preserving aspect ratio
    scale = min(width / cw, height / ch)
    new_w = max(1, int(cw * scale))
    new_h = max(1, int(ch * scale))

    # Use NEAREST for pixel art to keep crisp edges
    content = content.resize((new_w, new_h), Image.NEAREST)

    # Center on transparent canvas
    result = Image.new("RGBA", (width, height), (0, 0, 0, 0))
    x_off = (width - new_w) // 2
    y_off = height - new_h  # Bottom-align (sprites sit on ground)
    result.paste(content, (x_off, y_off), content)

    return result


def quantize_to_pixel_art(img: Image.Image, pixel_size: int = 1) -> Image.Image:
    """Enforce pixel grid alignment by downscaling then upscaling with NEAREST."""
    if pixel_size <= 1:
        return img

    img = img.convert("RGBA")
    w, h = img.size
    small_w = max(1, w // pixel_size)
    small_h = max(1, h // pixel_size)

    small = img.resize((small_w, small_h), Image.NEAREST)
    return small.resize((w, h), Image.NEAREST)


def convert_to_grayscale(img: Image.Image) -> Image.Image:
    """Convert to grayscale while preserving alpha channel.
    Used for sprites that will be material-tinted at runtime."""
    img = img.convert("RGBA")
    data = np.array(img)

    # Luminance conversion (ITU-R BT.601)
    gray = (0.299 * data[:, :, 0] + 0.587 * data[:, :, 1] + 0.114 * data[:, :, 2]).astype(np.uint8)

    data[:, :, 0] = gray
    data[:, :, 1] = gray
    data[:, :, 2] = gray
    # Alpha unchanged

    return Image.fromarray(data)


def enforce_palette(img: Image.Image, max_colors: int = 32) -> Image.Image:
    """Reduce color count to enforce a pixel art palette.
    Preserves alpha channel."""
    img = img.convert("RGBA")
    data = np.array(img)
    alpha = data[:, :, 3].copy()

    # Quantize RGB channels
    rgb = Image.fromarray(data[:, :, :3])
    rgb_quantized = rgb.quantize(colors=max_colors, method=Image.Quantize.MEDIANCUT)
    rgb_back = rgb_quantized.convert("RGB")

    # Recombine with original alpha
    result = np.array(rgb_back)
    final = np.dstack([result, alpha])

    return Image.fromarray(final)


def cleanup_alpha(img: Image.Image, threshold: int = 128) -> Image.Image:
    """Make alpha channel binary — fully opaque or fully transparent.
    Prevents semi-transparent artifacts from AI generation."""
    img = img.convert("RGBA")
    data = np.array(img)
    data[:, :, 3] = np.where(data[:, :, 3] >= threshold, 255, 0).astype(np.uint8)
    return Image.fromarray(data)


def process_sprite(
    img: Image.Image,
    grayscale: bool = False,
    max_colors: int = 32,
    bg_threshold: int = 20,
    alpha_threshold: int = 128,
) -> Image.Image:
    """Full post-processing pipeline for a single generated sprite."""
    # 1. Remove AI-generated background
    img = remove_background(img, threshold=bg_threshold)

    # 2. Resize to game cell
    img = resize_to_cell(img)

    # 3. Clean up alpha
    img = cleanup_alpha(img, threshold=alpha_threshold)

    # 4. Reduce colors
    img = enforce_palette(img, max_colors=max_colors)

    # 5. Convert to grayscale if this sprite will be material-tinted
    if grayscale:
        img = convert_to_grayscale(img)

    return img


if __name__ == "__main__":
    import sys

    if len(sys.argv) < 2:
        print("Usage: python postprocess.py <input.png> [--grayscale] [--colors N]")
        sys.exit(1)

    input_path = sys.argv[1]
    grayscale = "--grayscale" in sys.argv
    max_colors = 32
    for i, arg in enumerate(sys.argv):
        if arg == "--colors" and i + 1 < len(sys.argv):
            max_colors = int(sys.argv[i + 1])

    img = Image.open(input_path)
    result = process_sprite(img, grayscale=grayscale, max_colors=max_colors)

    out_path = input_path.rsplit(".", 1)[0] + "_processed.png"
    result.save(out_path)
    print(f"Saved: {out_path} ({result.size[0]}x{result.size[1]})")
