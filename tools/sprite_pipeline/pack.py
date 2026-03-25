"""
Tilesheet packer for Ingnomia sprites.
Takes individual 32x36 sprite PNGs and packs them into tilesheet images
matching Ingnomia's tilesheet format.
"""

import json
import math
import os
from pathlib import Path

from PIL import Image


CELL_W = 32
CELL_H = 36
# Max tilesheet width in cells (sprites per row)
DEFAULT_COLS = 16


def pack_tilesheet(
    sprite_paths: list[str],
    output_path: str,
    cols: int = DEFAULT_COLS,
) -> dict:
    """Pack individual sprite PNGs into a single tilesheet.

    Returns a dict mapping sprite filename -> SourceRectangle string ("X Y W H")
    for database entry.
    """
    if not sprite_paths:
        return {}

    rows = math.ceil(len(sprite_paths) / cols)
    sheet_w = cols * CELL_W
    sheet_h = rows * CELL_H

    sheet = Image.new("RGBA", (sheet_w, sheet_h), (0, 0, 0, 0))
    source_rects = {}

    for i, path in enumerate(sprite_paths):
        col = i % cols
        row = i // cols
        x = col * CELL_W
        y = row * CELL_H

        sprite = Image.open(path).convert("RGBA")

        # Ensure correct size
        if sprite.size != (CELL_W, CELL_H):
            sprite = sprite.resize((CELL_W, CELL_H), Image.NEAREST)

        sheet.paste(sprite, (x, y), sprite)

        name = Path(path).stem
        source_rects[name] = f"{x} {y} {CELL_W} {CELL_H}"

    sheet.save(output_path)
    return source_rects


def pack_by_category(
    sprite_dir: str,
    output_dir: str,
    cols: int = DEFAULT_COLS,
) -> dict:
    """Pack sprites grouped by category subdirectory into separate tilesheets.

    Expected structure:
        sprite_dir/
            terrain/
                terrain_stone_wall_0.png
                terrain_stone_wall_1.png
            furniture/
                item_wooden_chair_FR.png
            ...

    Returns a dict of {tilesheet_name: {sprite_name: source_rectangle}}.
    """
    sprite_dir = Path(sprite_dir)
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    all_rects = {}

    # Collect by category
    categories = {}
    for item in sorted(sprite_dir.iterdir()):
        if item.is_dir():
            pngs = sorted(item.glob("*.png"))
            if pngs:
                categories[item.name] = [str(p) for p in pngs]

    # Also collect any loose PNGs in the root
    loose = sorted(sprite_dir.glob("*.png"))
    if loose:
        categories["misc"] = [str(p) for p in loose]

    for category, paths in categories.items():
        sheet_path = str(output_dir / f"{category}.png")
        rects = pack_tilesheet(paths, sheet_path, cols=cols)
        all_rects[category] = rects
        print(f"Packed {len(paths)} sprites -> {sheet_path}")

    # Write source rectangle mapping
    mapping_path = str(output_dir / "source_rectangles.json")
    with open(mapping_path, "w") as f:
        json.dump(all_rects, f, indent=2)
    print(f"Source rectangles -> {mapping_path}")

    return all_rects


if __name__ == "__main__":
    import sys

    if len(sys.argv) < 3:
        print("Usage: python pack.py <sprite_dir> <output_dir> [cols]")
        print("  sprite_dir: directory with category subdirs of 32x36 PNGs")
        print("  output_dir: where to write tilesheet PNGs")
        sys.exit(1)

    sprite_dir = sys.argv[1]
    output_dir = sys.argv[2]
    cols = int(sys.argv[3]) if len(sys.argv) > 3 else DEFAULT_COLS

    pack_by_category(sprite_dir, output_dir, cols=cols)
