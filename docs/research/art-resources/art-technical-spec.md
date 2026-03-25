# Ingnomia Art Technical Specification

## Art Style

Ingnomia uses an **isometric/dimetric projection** (~30-degree) with chunky, voxel-like pixel art inherited from Gnomoria. Objects appear as small 3D blocks rendered as 2D sprites. The palette is earthy and muted -- many sprites are drawn in grayscale and tinted at runtime by material color. All sprites use transparent backgrounds within cells.

## Technical Specs

- **Standard cell size**: 32x36 pixels (32 wide, 36 tall -- extra 4px for isometric depth)
- **Total base sprites in DB**: 2,759
  - 2,743 at 32x36
  - 10 at 32x32
  - 6 at 1x1
- **Format**: PNG spritesheets with transparent backgrounds
- **Sprite addressing**: Each sprite has a `SourceRectangle` ("X Y W H") defining its coordinates within a tilesheet
- **Loader**: `SpriteFactory` (`src/gfx/spritefactory.cpp`) loads tilesheets and extracts individual sprites via `extractPixmap()`

## Tilesheets

26 tilesheet files containing all game sprites:

| Tilesheet | Dimensions | Sprites | Contents |
|-----------|-----------|---------|----------|
| `seasonalgrass.png` | 416x1116 | 390 | Grass variants across 4 seasons |
| `default.png` | 1024x1152 | 314 | General items, tools, crafted goods, resources |
| `seasonalslopes.png` | 320x864 | 288 | Slopes with seasonal grass |
| `terrain.png` | 1024x1152 | 254 | Walls, floors, ramps, stairs, ore veins, water |
| `workshops.png` | 416x540 | 198 | Workshop buildings and components |
| `gnomes.png` | 768x540 | 183 | Gnome body parts (composited at runtime) |
| `plants.png` | 1024x1152 | 150 | Crops, bushes, trees in growth stages |
| `multitrees.png` | 640x576 | 144 | Large multi-tile tree sprites |
| `seasonaldetails.png` | 288x504 | 118 | Seasonal detail overlays |
| `animals.png` | 512x576 | 103 | Domestic/wild animals with facing variants |
| `mushroom_biome_slopes.png` | 320x360 | 100 | Underground mushroom biome slopes |
| `windmill.png` | 256x324 | 72 | Windmill animation frames |
| `furniture.png` | 512x576 | 65 | Beds, tables, chairs, doors, containers |
| `weapons-armour-UI-large.png` | 320x252 | 64 | Large UI weapon/armor display |
| `traps_mechanism.png` | 576x432 | 52 | Mechanisms, traps, axles, gears, levers |
| `multicreatures.png` | 544x576 | 44 | Large multi-tile creatures |
| `goblin.png` | 480x288 | 38 | Goblin sprites |
| `mushrooms.png` | 1056x108 | 38 | Mushroom plant varieties |
| `mobs.png` | 512x432 | 36 | Monsters/enemies with facing variants |
| `weapons_armour.png` | 384x288 | 28 | Worn/held weapon and armor sprites |
| `food_drink_ingredients.png` | 384x144 | 27 | Food items and ingredients |
| `mushroom_biome_zygs.png` | 256x576 | 16 | Underground zyg creatures |
| `automatons.png` | 256x108 | 15 | Automaton mechanical creatures |
| `mushroom_biome_grass.png` | 256x288 | 14 | Mushroom biome grass |
| `magic.png` | 224x72 | 8 | Magic effects |
| `windmill-32x36.png` | 384x468 | 0 | On disk but 0 DB entries (alternate render) |

**Total: 2,759 base sprites** (2,743 at 32x36, 10 at 32x32, 6 at 1x1)

## Sprite System Features

### Material Tinting

Grayscale sprites are tinted at runtime via `tintPixmap()` using the material's `Color` value from the database. One grayscale sprite can represent many different material appearances (e.g., a single "wall" sprite becomes stone, wood, copper, etc.).

### Seasonal Variants

The `Sprites_Seasons` table maps sprites to season-specific alternatives. Terrain, grass, slopes, and detail overlays all change appearance per season (spring, summer, autumn, winter).

### Rotation Variants

The `Sprites_Rotations` table provides four isometric viewing directions:
- **FR** -- Front-Right
- **FL** -- Front-Left
- **BL** -- Back-Left
- **BR** -- Back-Right

### Random Variants

The `Sprites_Random` table assigns weighted random alternatives. Used for natural variation in things like rock textures, grass patches, and other terrain where visual monotony would be noticeable.

### Animation Frames

The `Sprites_Frames` table defines frame sequences for animated objects (windmill blades, water, etc.).

### Composite Sprites

The `Sprites_Combine` table layers multiple sprites to form complex objects. For example, a bed is composed of a frame sprite plus a sheets sprite, each potentially tinted by different materials.

### Material-Based Sprite Selection

- `Sprites_ByMaterials` -- Select entirely different sprites based on specific material
- `Sprites_ByMaterialTypes` -- Select sprites based on material type/category

### Creature Compositing

Gnomes and other humanoid creatures are built from layered body parts composited at runtime:
- Base body
- Head
- Hair (with color variants)
- Clothing layers
- Armor layers
- Held weapons/tools

Each part is a separate sprite from `gnomes.png`, assembled based on the creature's equipment and appearance attributes.

## Sprite Class Hierarchy

Defined in `src/gfx/sprite.h`:

- **SpritePixmap** -- Single sprite (base unit)
- **SpriteSeasons** -- Seasonal variant wrapper (holds up to 4 SpritePixmaps)
- **SpriteRotations** -- Rotation variant wrapper (FR/FL/BL/BR)
- **SpriteFrames** -- Animation frame wrapper (sequence of sprites)

## How to Add New Sprites

### Pipeline

1. **Draw** the sprite at 32x36 pixels in matching isometric/dimetric projection (~30-degree). If the sprite will be material-tinted, draw it in grayscale.
2. **Place** the sprite in an existing or new tilesheet PNG file in `content/tilesheet/`.
3. **Add a `BaseSprites` DB row**: Assign an ID, set `SourceRectangle` to `"X Y 32 36"` (position within the tilesheet), and specify the tilesheet filename.
4. **Reference from the `Sprites` table** with entries in sub-tables as needed:
   - `Sprites_Seasons` for seasonal variants
   - `Sprites_Rotations` for directional variants
   - `Sprites_Random` for weighted random alternatives
   - `Sprites_Frames` for animation
   - `Sprites_Combine` for composite layering
   - `Sprites_ByMaterials` / `Sprites_ByMaterialTypes` for material-specific selection
5. **SpriteFactory** automatically loads the sprite on init via `extractPixmap()`.

### Style Guidelines

- Match the existing chunky, voxel-like aesthetic
- Use the 2:1 isometric ratio (2 pixels horizontal per 1 pixel vertical for edges)
- Keep lighting consistent: light source from top-left
- Use muted, earthy tones for base colors; grayscale if the sprite will be material-tinted
- Transparent background required

## Content Gaps

Areas where new sprites are needed:

- **New biomes**: Desert terrain, snow/ice terrain, tropical terrain, volcanic/magma terrain tiles
- **New creatures**: More enemy types, wildlife, fantasy creatures
- **New items**: More weapons, armor, tools, crafted goods
- **UI elements**: Mood icons, status icons, thought bubbles, notification icons
- **Effects**: Weather particles, magic effects, combat animations

## License Situation

Current tilesheets are sourced from Gnomoria with personal permission from the original developer. **These sprites are NOT redistributable** -- they cannot be included in the open source repository or distributed with builds.

New art must use one of these licenses to be compatible with the AGPL v3 codebase:
- **Original work** (created by or commissioned for Ingnomia)
- **CC0** (public domain)
- **CC-BY** (attribution required)
- **CC-BY-SA** (attribution + share-alike)
- **Commissioned with full rights transfer**

The `content/tilesheet/` directory is required at runtime but not checked into the repository. Users must supply their own tilesheets from an existing Gnomoria installation or from a replacement art pack.
