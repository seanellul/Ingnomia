#!/bin/bash
# Ingnomia Sprite Generation Pipeline
# Run from project root: ./tools/sprite_pipeline/run.sh
#
# First-time setup:
#   pip3 install -r tools/sprite_pipeline/requirements.txt
#
# The first run will download the model (~4GB for SD 1.5 pixel art checkpoint).
# Subsequent runs use the cached model from ~/.cache/huggingface/

set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== Ingnomia Sprite Pipeline ==="
echo ""

case "${1:-all}" in
    all)
        echo "Generating all sprites from manifest..."
        python3 generate.py "${@:2}"
        ;;
    test)
        echo "Quick test: generating a single sprite (15 steps)..."
        python3 generate.py --steps 15 --id item_sword "${@:2}"
        ;;
    terrain)
        echo "Generating terrain sprites..."
        python3 generate.py --id terrain_stone_wall terrain_stone_floor terrain_dirt_floor terrain_grass terrain_water "${@:2}"
        ;;
    furniture)
        echo "Generating furniture sprites..."
        python3 generate.py --id item_wooden_chair item_wooden_table item_wooden_bed item_chest "${@:2}"
        ;;
    pack)
        echo "Packing existing sprites into tilesheets..."
        python3 pack.py output/individual output/tilesheets
        ;;
    postprocess)
        echo "Post-processing a single image..."
        if [ -z "$2" ]; then
            echo "Usage: $0 postprocess <input.png> [--grayscale]"
            exit 1
        fi
        python3 postprocess.py "${@:2}"
        ;;
    *)
        echo "Usage: $0 {all|test|terrain|furniture|pack|postprocess}"
        echo ""
        echo "  all         Generate all sprites from manifest"
        echo "  test        Quick test with one sprite (fast)"
        echo "  terrain     Generate terrain sprites only"
        echo "  furniture   Generate furniture sprites only"
        echo "  pack        Pack existing sprites into tilesheets"
        echo "  postprocess Post-process a single image"
        echo ""
        echo "Extra args are passed through to generate.py:"
        echo "  $0 all --steps 20 --seed 123"
        echo "  $0 all --model 'Onodofthenorth/SD_PixelArt_SpriteSheet_Generator'"
        ;;
esac
