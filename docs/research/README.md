# Ingnomia Research Documents

Game design research compiled from wiki pages, community discussions, and web resources. These documents serve as reference material for building Ingnomia's game systems.

---

## RimWorld

### [rimworld/core-mechanics.md](rimworld/core-mechanics.md)
Core game systems research from the RimWorld wiki. Covers **73 character traits** across 10 spectrum categories, the complete **mood/thoughts system** with 150+ individual thoughts and exact values, **mental breaks** across three severity tiers, **events/incidents** (threats, weather, good/bad events), the **social system** (relationships, romance, marriage, opinion modifiers), the **medical/health system** (11 health capacities, body part hierarchy, diseases, artificial parts), and the **12-skill system** with passion mechanics and decay rules.

### [rimworld/extended-systems.md](rimworld/extended-systems.md)
Extended systems including **animals** (training, bonding, manhunter mechanics), **backstories** (36 childhood + 85 adult backstories with skill/work/trait effects), **17+ biomes** with temperature/disease/forageability profiles, **factions** (tribes, outlanders, pirates, mechanoids with full goodwill system), **diseases and health conditions**, **weapons** (ranged/melee formulas, quality tiers, material modifiers), **mechanoids** (hostile and allied types with stats), and **insectoids** (infestation mechanics).

---

## Dwarf Fortress

### [dwarf-fortress/core-mechanics.md](dwarf-fortress/core-mechanics.md)
Deep systems research from the DF wiki. Covers the **three-layer personality system** (50 facets, 34 values, life goals), the **thoughts and emotions system** (180 defined emotions, stress score from -1M to +1M), **strange moods and insanity** (5 mood types, artifact creation, tantrum spirals, 4 permanent insanity types), **combat/anatomy/wounds** (layered tissue model, organ-specific effects, bleeding mechanics, 3 damage types), the **medical system** (5 specializations, diagnosis pipeline, infection), **social relationships** (interaction counters, compatibility, grudges, marriage/children), and **needs/room quality** (30 personality-driven needs, focus system, room value thresholds).

### [dwarf-fortress/extended-systems.md](dwarf-fortress/extended-systems.md)
Extended DF systems including **creature definitions** (token-based system, body plans, castes, size system, domestic animal products), **enemies and megabeasts** (4 megabeasts, procedural forgotten beasts/titans, werebeasts, vampires, necromancers, siege system), **world generation and history** (geography pipeline, history simulation, legends system), **biomes and regions** (surface biomes, alignment/savagery overlay, 3 underground cavern layers, aquifers), **artifact system** (strange moods, artifact properties, world-level dynamics), **noble/administrative system** (hierarchy, mandates, room requirements), and **industry/economy/trade** (seasonal caravans, civilization preferences, trade agreements).

---

## Gnomoria

### [gnomoria/wiki-mechanics.md](gnomoria/wiki-mechanics.md)
Comprehensive Gnomoria mechanics research. Covers the **core gameplay loop** (build-craft-grow cycle, Kingdom Worth progression), **gnome management** (5 attributes, complete skill list, professions, needs), **crafting/workshops** (20+ workshops with full input/output tables, material hierarchy), **military/combat** (squad organization, weapon stats, armor types, position/formation perks), **building/construction** (terrain manipulation, room types, mechanism system, trap designs), **animals** (livestock types, breeding mechanics), **enemies/threats** (surface invaders, underground creatures, threat escalation), **tech/progression** (organic progression through KW thresholds, mining depth, tinker research), **missing features and bugs** (33 specific issues), and **UI/UX analysis**.

### [gnomoria/community-feedback.md](gnomoria/community-feedback.md)
Player perspectives from Reddit, Steam discussions, and review sites. Documents **what players loved** (accessibility, crafting depth, difficulty scaling, creative freedom), **what frustrated them** (abandonment, game-breaking bugs, performance, steep learning curve, shallow content, AI problems), **feature wishlists** (personality/social systems, combat improvements, endgame content, multiplayer, mod support), **competitive comparison** (vs Dwarf Fortress and RimWorld), and **community expectations for a successor**.

---

## Art Resources

### [art-resources/art-technical-spec.md](art-resources/art-technical-spec.md)
Complete technical specification of Ingnomia's sprite/art system. Documents the **32x36 isometric cell format**, all **26 tilesheets** with dimensions and sprite counts (2,759 total base sprites), the **sprite system features** (material tinting, seasonal variants, rotation variants, random variants, animation frames, composite sprites, creature compositing), the **SpriteFactory pipeline**, how to **add new sprites**, current **content gaps**, and the **license situation** (Gnomoria sprites not redistributable).

### [art-resources/isometric-art-search.md](art-resources/isometric-art-search.md)
Targeted search for **isometric-compatible** art resources matching Ingnomia's specific 32x36 dimetric projection. Covers free isometric tilesets from **itch.io and OpenGameArt**, **voxel-to-sprite pipeline tools** (MagicaVoxel, IsoVoxel, SpriteStack, PixZels), **AI sprite generators** (PixelLab, God Mode AI), **3D model converters** (IsoTool, IsoSpriteGenerator), and **Aseprite isometric extensions** (Pixthic, IsoBox Generator).
