# Isometric Art Resource Search

Research into free/open isometric art assets, generation tools, and pipelines compatible with Ingnomia's 32x36 dimetric sprite system.

**Target specs**: 32x36 pixel cells, ~30-degree isometric/dimetric projection, voxel-like pixel art, earthy muted palette, grayscale-with-tinting support, PNG spritesheets.

---

## Free Isometric Tilesets

### itch.io Collections

**Free Isometric + Pixel Art Assets**
- URL: https://itch.io/game-assets/free/tag-isometric/tag-pixel-art
- License: Varies per asset (check individual pages)
- Notable packs:
  - **32x32 Pixel Isometric Tiles** by scrabling (https://scrabling.itch.io/pixel-isometric-tiles) -- basic terrain tiles, close to target size but 32x32 not 32x36
  - **443 Isometric Town and Roof Tiles** -- large set of building/roof tiles
  - **1,008 Isometric Floor Tiles** -- extensive floor variety
  - **360 Isometric Overworld Tiles** -- outdoor terrain
  - **395 Isometric Object Tiles** -- props and items
  - **Simple Cozy Farm Asset Pack** -- 32x32 isometric farm assets
  - **Isometric Space Colony** by monogon -- hundreds of pixelart sprites for space colony games
  - **[Mega Pack] Admurins Freebies** -- large mixed isometric pack
  - **2.5D FREE Tiles: Infernus Tileset** -- stylized isometric terrain
- Style compatibility: Most are 32x32 or 64x64 base. Would need resizing/redrawing to match Ingnomia's 32x36 cells. Art styles vary widely -- few match the chunky Gnomoria voxel look.

**Free Isometric Tilesets (general)**
- URL: https://itch.io/game-assets/free/tag-isometric/tag-tileset
- Large browseable collection, regularly updated with new submissions.

### OpenGameArt.org

**Isometric Tiles**
- URL: https://opengameart.org/content/isometric-tiles
- License: Varies (CC0, CC-BY, CC-BY-SA, GPL common)
- Definitive collection of community-submitted isometric tiles across all styles.
- Style compatibility: Mixed. Some pixel art sets could serve as reference or starting points.

**PixVoxel Colorful Isometric Wargame Sprites**
- URL: https://opengameart.org/content/pixvoxel-colorful-isometric-wargame-sprites
- License: Check page (likely CC-BY or CC-BY-SA)
- Created using voxel-to-sprite pipeline. Demonstrates the voxel-to-isometric workflow that could produce Gnomoria-compatible art.

### GameDev Market

**Isometric Pixel Art Tileset**
- URL: https://www.gamedevmarket.net/asset/isometric-pixel-art-tileset
- License: Commercial (paid)
- Professional quality isometric pixel art. Not free, but useful as style reference.

### Natural RPG Isometric Tileset

- URL: https://raptor-reece.itch.io/isometric-tiles-free
- License: Free to use, credit optional
- Beginner-friendly pack for practicing isometric map creation.

### Key Compatibility Notes

Most free isometric tilesets use **32x32**, **64x64**, or **16x16** cell sizes. Ingnomia's **32x36** cells (with 4px extra height for isometric depth) are uncommon. Any external assets would need:
1. Resizing or redrawing to fit the 32x36 grid
2. Style adjustment to match the chunky voxel-block aesthetic
3. Grayscale conversion if they need to support material tinting

---

## AI Sprite Generators

### PixelLab
- URL: https://www.pixellab.ai/
- Type: AI generator specifically for pixel art game assets
- Key feature: Can generate 4 or 8 directional views of characters from a single direction, which maps well to Ingnomia's FR/FL/BL/BR rotation system
- Output sizes: Configurable, including 32x32 and 64x64
- Workflow: Generate base sprite, then manually adjust to 32x36 and match art style
- Cost: Free tier available, paid for higher volume
- Assessment: Best current option for generating character/creature sprites with directional variants. Would need post-processing in Aseprite to match Ingnomia's exact style.

### Sprite AI
- URL: https://www.sprite-ai.art/
- Type: AI sprite generator with built-in editor
- Output sizes: 16x16, 32x32, 64x64, 128x128
- Features: Built-in editor to fix stray pixels and adjust colors after generation
- Assessment: Useful for rapid prototyping. Generated sprites would need style matching.

### Chroma Studio AI Sprite Generator
- URL: https://www.chromastudio.ai/generator/ai-sprite-generator
- Type: Free AI sprite generator
- Assessment: General purpose, less specialized for isometric than PixelLab.

### Easy-Peasy AI Isometric Art Generator
- URL: https://easy-peasy.ai/ai-image-generator/isometric-art
- Type: Specifically targets isometric art generation
- Assessment: More suited for illustration than game sprites. Output would need significant rework.

### Recommended AI Workflow

The most practical approach for Ingnomia:
1. Generate base concepts with PixelLab (set to isometric, 32x32)
2. Import into Aseprite
3. Resize canvas to 32x36 (add 4px height)
4. Manually adjust to match voxel-block style
5. Convert to grayscale if material tinting is needed
6. Export and add to tilesheet

---

## Voxel-to-Sprite Tools

These tools create 2D isometric sprites from 3D voxel models, which is the most natural pipeline for matching Ingnomia's voxel-like art style.

### MagicaVoxel
- URL: https://ephtracy.github.io/
- License: Free (closed source)
- Platform: Windows, macOS
- Key features for Ingnomia:
  - Built-in **ISO export** -- renders voxel model as isometric pixel sprite PNG
  - Built-in **2D export** -- renders as flat 2D sprite PNG
  - Customizable camera angle and rendering settings
  - Animation support via multi-frame .vox files
- Pipeline: Model in MagicaVoxel -> ISO export -> crop to 32x36 -> add to tilesheet
- Assessment: **Best tool for creating new Ingnomia sprites from scratch.** The voxel modeling approach naturally produces the chunky block aesthetic. Artists can model items/creatures in 3D and export matching isometric views.

### IsoVoxel
- URL: https://github.com/tommyettinger/IsoVoxel
- License: Open source
- Platform: Windows, macOS (via Mono), Linux (via Mono)
- Features:
  - Processes MagicaVoxel .vox files into isometric pixel art
  - Generates **44 images** per model from various perspectives
  - 8 standard isometric renders + 8 top-down + diagonal variants
  - Supports animated .vox files (up to 24 frames)
  - Automatic slope rendering
- Pipeline: Model in MagicaVoxel -> process with IsoVoxel -> select best angle -> crop to 32x36
- Assessment: **Excellent for batch-generating rotation variants.** The 8-direction output maps naturally to Ingnomia's FR/FL/BL/BR rotation system (select 4 of the 8).

### SpriteStack
- URL: https://spritestack.io/ / https://rezoner.itch.io/spritestack
- License: Commercial (paid, ~$15)
- Platform: Windows, macOS, Linux
- Features:
  - Voxel editor with hand-crafted retro renderer
  - Sprite stacking technique (layered horizontal slices)
  - Exports spritesheets with color and normal maps
  - Keyframe animation support
  - Can import MagicaVoxel .vox files
  - Customizable retro-style rendering
- Assessment: Good for artists who want an all-in-one voxel-to-sprite workflow. The retro renderer can produce output close to Gnomoria's style.

### PixZels
- URL: https://pixel-salvaje.itch.io/pixzels
- License: Free (browser-based, early development)
- Features:
  - Dedicated voxel engine optimized for pixel art output
  - Start from 1-3 orthographic reference images
  - Uses 6-sided colored voxels for pixel-accurate rendering
  - Export in 8 or 16 directions with one click
  - Power-of-2 sprite output sizes
- Assessment: Promising but very early (v0.001). Worth watching for future development. The multi-direction export is particularly useful for Ingnomia's rotation system.

### Voxel2Pixel
- URL: https://github.com/BenMcLean/Voxel2Pixel
- License: Open source
- Features:
  - Graphics library for rendering voxel models as pixel art
  - `SpriteMaker.Iso8` generates 8 sprites from 8 compass directions using alternating isometric and 3/4 perspectives
- Assessment: Programmatic approach, good for automated pipelines.

### vox2png
- URL: https://github.com/jemisa/vox2png
- License: Open source
- Features: Converts MagicaVoxel .vox files into layered sprite PNGs
- Assessment: Simple conversion tool, useful as part of a larger pipeline.

### Recommended Voxel Pipeline for Ingnomia

1. **Model** objects in MagicaVoxel at appropriate scale (e.g., 16x16x20 voxels for a 32x36 sprite)
2. **Export** using MagicaVoxel ISO mode or process through IsoVoxel for multi-angle output
3. **Select** the 4 rotation variants (FR/FL/BL/BR) from the generated angles
4. **Post-process** in Aseprite: crop to 32x36, adjust colors, convert to grayscale if needed
5. **Pack** into tilesheets and add DB entries

---

## 3D Model Converters

### Drububu Voxelizer
- URL: https://drububu.com/miscellaneous/voxelizer/
- Type: Online tool
- Features: Converts 3D models or images to voxel format
- Assessment: Useful for converting existing 3D models to voxels, which can then be run through the voxel-to-sprite pipeline above.

---

## Aseprite Isometric Extensions

Aseprite (https://www.aseprite.org/) is the primary pixel art editor. These extensions add isometric-specific features:

### Easymetric
- Author: Oroshibu
- Features:
  - Colored and textured rendering modes
  - Automatic outlines
  - Per-layer geometry for complex models
  - Dithering and interpolation tools
  - Animation support
  - .obj export
- Assessment: **Most feature-rich isometric Aseprite plugin.** Particularly useful for creating complex isometric objects with proper perspective.

### Pixthic
- URL: https://community.aseprite.org/t/pixthic-aseprite-for-isometric-grid-drawing/25071
- Features: Generates customizable isometric grids inside Aseprite (width and height adjustable)
- Assessment: Essential for ensuring sprites align to the correct isometric grid. Set grid to 32x36 for Ingnomia work.

### IsoBox (Isometric Box Generator)
- URL: https://darkwark.itch.io/isobox-for-aseprite
- Source: https://github.com/darkwark/isobox-for-aseprite
- Features:
  - Generate isometric boxes of various sizes
  - Custom colors per side
  - Highlight and stroke color options
  - 2px and 3px corner styles
- Assessment: Very useful for quickly creating basic isometric block shapes that match Ingnomia's voxel style. Good starting point for furniture, containers, and building blocks.

### Isometric Guidelines Plugin
- Author: Rokugatsua
- URL: https://rokugatsua.hashnode.dev/isometric-guidelines-plugin-for-aseprite
- Features: Automatically creates guideline layers for 2:1 isometric projection
- Assessment: Helpful for maintaining consistent projection across sprites.

---

## Colony Sim Specific Assets

### Isometric Space Colony (monogon)
- URL: https://itch.io/game-assets (search "isometric space colony monogon")
- Hundreds of pixelart sprites for space colony games
- Style: Clean pixel art, more sci-fi than Ingnomia's fantasy/medieval setting

### Colony-Sim / RTS Tilesets
- Various packs on itch.io tagged for colony-sim and RTS genres
- Most use different projections or cell sizes than Ingnomia
- Best used as reference for what types of sprites a colony sim needs

---

## Summary and Recommendations

### For Immediate Use (with modification)
1. Browse **itch.io free isometric assets** for placeholder sprites during development
2. Check **OpenGameArt.org** for CC0/CC-BY licensed sprites that can be adapted

### For New Original Art Production
1. **MagicaVoxel + IsoVoxel pipeline** is the best match for Ingnomia's voxel aesthetic
2. Use **Aseprite with Easymetric/Pixthic/IsoBox** for hand-drawn sprites and post-processing
3. **PixelLab AI** for generating creature/character concepts with rotation variants

### For Long-Term Art Replacement
The Gnomoria sprites cannot be redistributed. A complete replacement art pack should:
- Use the **MagicaVoxel pipeline** for consistency across all 2,759 sprites
- Create a standardized voxel model library that can regenerate sprites at different sizes
- Maintain grayscale variants for all material-tinted sprites
- Cover all 26 tilesheets systematically, prioritizing terrain (254 sprites) and default items (314 sprites) first
