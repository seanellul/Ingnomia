# Gnomoria Game Mechanics Research

This document captures comprehensive research on Gnomoria's mechanics to inform Ingnomia development.

---

## 1. Core Gameplay Loop

Gnomoria is a sandbox village management game where you lead a small group of gnomes to thrive into a bustling kingdom. The core loop is:

1. **Start small** -- Begin with ~7 gnomes on a procedurally generated map
2. **Gather & Build** -- Mine stone/ore, chop trees, farm crops, build workshops
3. **Craft & Process** -- Raw materials flow through crafting chains (ore -> bars -> weapons/armor)
4. **Grow Kingdom Worth** -- Combined value of stockpiles + buildings + Great Hall
5. **Attract Gnomads** -- Higher worth = more gnomes arrive each season (population growth)
6. **Trade** -- Host ambassadors, send merchants to trade with other kingdoms
7. **Defend** -- Higher worth also attracts stronger enemies (goblins, ogres, mants)
8. **Dig Deeper** -- Better ores (iron, gold, platinum) at deeper Z-levels, but also underground enemies

**What made it fun/addictive:**
- The "just one more thing" building loop (always another workshop to build, another level to dig)
- Watching your colony grow from a handful of gnomes to a bustling kingdom
- The tension between growing wealth (which attracts gnomads) and growing danger (which attracts enemies)
- The satisfying crafting chains -- turning raw materials into finished goods
- The sandbox freedom -- no prescribed path, play peaceful or militaristic
- Designing fortress defenses, traps, and kill corridors

**What the game lacked:**
- No real endgame -- once self-sufficient, challenge evaporates
- No narrative hooks or goals beyond survival
- Limited gnome personality/social systems

---

## 2. Gnome Management

### Attributes (5 stats)

| Attribute | Raised By | Affects |
|-----------|-----------|---------|
| **Fitness** | Mining, Smelting, Fighting | Weapon damage, wake time, health, combat endurance |
| **Nimbleness** | Horticulture, Farming, Woodcutting, Hauling | Movement speed, build/deconstruct speed, dodge |
| **Focus** | Carpentry, Combat training, Ranged weapons | Ranged accuracy, armor pierce chance, critical hits |
| **Curiosity** | Tinkering, Cooking, Smelting/Forging, Building | Rate of all skill learning, mechanism building |
| **Charm** | Feeding patients, Farming animals | Animal care, patient care tasks |

### Skills (Complete List)

Skills are organized into professions. The individual skills are:

- **Mining category:** Miner, Mason, Stonecarver
- **Wood category:** Woodcutter, Carpenter, Woodcarver
- **Metal category:** Smelter, Blacksmith, Metalworker, Weaponsmith, Armorer
- **Gem category:** Gemcutter, Jeweler
- **Cloth/Leather:** Weaver, Tailor, Leatherworker, Bonecarver
- **Agriculture:** Farmer, Gardener (Horticulture), Rancher, Butcher
- **Food/Drink:** Chef, Brewer
- **Engineering:** Tinkerer, Machinist, Engineer
- **Medical:** Medic, Caretaker
- **General:** Builder, Hauler, Mechanic
- **Military:** Fighting skills (leveled through combat/training)
- **Not assigned by default:** Pottery, Bonecarving, Prospecting

### Predefined Professions

Miner, Woodcutter, Farmer, Rancher, Builder, Tailor, Engineer, Jeweler, Carpenter, Brewer, Cook, Mason, Hauler, Blacksmith, Armorsmith, Weaponsmith, Archer, Footman, Monk

### Gnome Needs

- **Food** -- Better quality food keeps gnomes satisfied longer. Hierarchy from raw to cooked.
- **Drink** -- Water/milk are average quality; wines and beers are better.
- **Sleep** -- Gnomes sleeping in personal rooms need less sleep than dormitories. Room value affects sleep quality. More valuable room = longer between sleep.
- **Morale** -- Gnomes slowly forget troubles and become happier over time. Civilian gnomes become upset near corpses and combat, refusing tasks until mood recovers.
- **Skill leveling** -- Leveling up contributes to gnome bliss.

**Notable limitation:** No personality system, no social relationships, no marriage/children, no individual preferences or goals. This was the #1 feature request from players.

---

## 3. Crafting/Workshops

### Workshop Build Order
Crude Workbench (starter) -> Stonecutter -> Stonemason -> Sawmill -> Carpenter -> everything else

### Complete Workshop List

| Workshop | Inputs | Outputs |
|----------|--------|---------|
| **Crude Workbench** | Logs | Planks, chairs, workbenches, chisels (starter only) |
| **Stonecutter** | Raw Stone | Stone Blocks |
| **Stonemason** | Stone Blocks | Stone furniture, doors, tables, chairs, chisels, sawblades, knife blades, hearths, molds, furnaces, troughs |
| **Sawmill** | Logs | Planks |
| **Carpenter** | Planks | Bed frames, doors, tables, chairs, crates, barrels, torches, workbenches, looms, weapon components (hilts, hafts, stocks) |
| **Woodcarver** | Wood | Trinkets, statues |
| **Smelter** | Ore, Metal slivers, Worn armor | Metal Bars, Alloy bars |
| **Forge/Blacksmith** | Metal Bars | Pickaxes, felling axes, cutting wheels, files, hammers, alarm bells |
| **Weaponsmith** | Metal Bars + weapon components | Swords, axes, hammers, claymores, battle axes, warhammers, shields |
| **Armorsmith** | Metal Bars | Helmets, breastplates, pauldrons, greaves, gauntlets, boots |
| **Metalsmith** | Metal Bars | Trinkets, statues |
| **Kitchen** | Grain, fruit, meat, eggs | Bread, sausages, sandwiches, omelets |
| **Distillery** | Fruit, grain | Wine (strawberry, apple, grape), beer |
| **Loom** | Cotton, Wool | Bolts of cloth |
| **Tailor Shop** | Cloth | Bags, sacks, bandages, string, padding, mattresses, ammo pouches |
| **Leatherworker** | Hides | Leather armor (all pieces), straps, panels, quivers |
| **Bonecarver** | Bones, skulls | Bone needles, statuettes, skull helmets, bone shirts |
| **Jeweler** | Metal bars + cut gems | Rings, necklaces, gemmed versions |
| **Gemcutter** | Raw gems | Cut gems |
| **Prospector** | Soil, Raw Stone | Metal slivers, silica |
| **Furnace** | Logs | Coal |
| **Kiln** | Clay | Brick blocks, ceramic tiles, clay statuettes |
| **Engineering Shop** | Various | Traps, crossbows, mechanisms, automatons, prosthetics |
| **Tinker Bench** | (research only) | Unlocks new mechanisms/mechanical parts |

### Material Hierarchy
- **Metals:** Copper -> Tin -> Bronze (alloy) -> Iron -> Steel (alloy) -> Gold, Silver, Platinum (for value/jewelry)
- **Leather:** Non-bear animal -> Ogre -> Two-headed Ogre
- **Stone:** Various types with different values

### Key Crafting Mechanic
- Workshops auto-chain: ordering a complex item will cascade orders to dependent workshops
- Each item left on workshop floor adds 5% production time penalty (must haul to stockpiles)
- Gnomes sometimes use wrong materials for sub-components (major bug/complaint)

---

## 4. Military/Combat

### Squad Organization
- Squads of up to 5 gnomes each
- Three configuration layers: **Uniforms** (equipment), **Positions** (individual perks), **Formations** (squad-wide bonuses)
- Can enroll every gnome in a squad as a militia

### Squad Archetypes
- **Soldiers** -- Melee combat
- **Snipers** -- Ranged units
- **Tanks** -- Shield-focused defenders
- **Bruisers/Crushers** -- Warhammers, stun-and-execute
- **Rogues** -- Leather armor, dual-wielding, Sneaky perk
- **Scouts** -- Torches, pistols, enhanced movement/vision
- **Commanders** -- Squad damage bonus aura

### Weapons

**1-Handed Melee:**
| Weapon | Damage | Special |
|--------|--------|---------|
| Sword | Slash | Causes bleeding, Parry, defend every 4 sec |
| Axe | 100 slash | Cuts off limbs, defend every 6 sec |
| Hammer | 125 blunt | Knockdown/knockback |
| Shield | 100 blunt | Knockback/daze, blocks attacks |

**2-Handed Melee:**
| Weapon | Damage | Special |
|--------|--------|---------|
| Battle Axe | 200 slash | Half swing speed |
| Warhammer | 250 blunt | Knockdown |
| Tower Shield | 200 blunt | 4x block chance vs 1-handed |
| Claymore | Slash + Blunt | Split damage type |

**Ranged:**
| Weapon | Range | Damage | Speed |
|--------|-------|--------|-------|
| Pistol | 0-5 | Pierce | 6 sec |
| Crossbow | 2-16 | 300 pierce | Medium |
| Blunderbuss | 2-8 | 600 pierce | 12 sec |

### Armor
- **Metal armor:** Great vs slash, poor vs blunt
- **Leather armor:** Great vs blunt, decent vs pierce, poor vs slash
- Metal progression: Copper -> Bronze -> Steel
- Leather progression: Animal -> Ogre -> Two-Headed Ogre

### Position Perks
- **Way of the Gnome** -- Massive damage/dodge buff when unarmored/unarmed. Multiplier: 1 + (Fighting Skill / 25)
- **Disarm** -- +50% chance to hit arms/hands
- **Guard** -- Boosts shield blocking
- **Taunt** -- 50% chance to redirect enemies within 5 tiles
- **Commander** -- Allies get +10% damage to commander's target
- **Sneaky** -- +20% damage, enemies only see adjacent gnomes
- **Knock Em Down / Finish Him** -- Stun then execute combo

### Formation Perks
- **The Best Defense** -- +2% melee damage per member (max 10%)
- **Shield Wall** -- +2% blocking per shield-wielder
- **Firing Squad** -- +2% ranged damage per member (max 10%)
- **Fire At Will** -- +8.3% ranged attack speed per melee member

### Training
- Setting soldiers to train 8+ hours/day levels skills
- Spectators gain reduced skill bonuses vs active sparring
- Way of the Gnome makes early unarmed training very efficient

### Combat Stats
- **Fitness** -- Damage output
- **Nimbleness** -- Dodge and speed
- **Focus** -- Accuracy
- **Dodge** -- Max once per 1.5 sec (except Way of the Gnome)

---

## 5. Building/Construction

### Terrain Manipulation
- **Mining/Digging** -- Extract resources, create stairs/ramps for vertical access
- **Wall/Floor modification** -- Change or remove terrain
- **Hole filling** -- Seal excavations
- Fully destructible environment -- everything can be broken and rebuilt

### Room Types
- **Personal Quarters** -- Individual sleeping/eating; better rest than dormitories
- **Dormitories** -- Communal beds
- **Dining Halls** -- Food consumption
- **Hospitals** -- Medical treatment
- **Great Hall** -- Designated hall whose value counts DOUBLE for Kingdom Worth calculations

Room value increases through expensive materials and decorative items. Higher room value = better sleep quality.

### Mechanisms System

Five kinds of mechanisms:
1. **Linkages** -- Axles, gearboxes (transmit power)
2. **Switches** -- Levers, pressure plates (control devices)
3. **Devices** -- Mechanical walls (20 power), hatches (10 power)
4. **Power Sources** -- Handcranks, windmills, steam engines
5. **Self-powered traps** -- Don't need external power

**Power transmission:** Mechanical items transmit power adjacently on same Z-level. Axles and gearboxes extend range. Vertical axle + gearbox combo transmits between Z-levels.

### Common Trap Designs
- Lever-controlled doors (mechanical wall + lever + windmills)
- Pressure plate doors (mechanical wall + pressure plate + windmills)
- Pitfall traps (hatch + pressure plate + mechanical wall over a hole)
- Crushing traps (floors above mechanical walls)
- One-way hatches for controlled entry
- Kill corridors forcing enemies through gauntlets

### Defensive Building
- Stone block walls prevent goblin tunneling
- Sniper perches with 1-block-high walls for firing angles
- Moats (water mechanics were buggy)

---

## 6. Animals

### Livestock Types

| Animal | Product | Available at KW | Notes |
|--------|---------|-----------------|-------|
| **Yak** | Milk | >0 (starter) | Eats ~1.4 wheat straw/day |
| **Alpaca** | Wool | >25,000 | Eats wheat straw |
| **Emu** | Eggs | >50,000 | Eats seeds |

### Animal Husbandry
- Animals require a **pasture** (designated area)
- Requires a gnome with **Rancher** skill
- **Breeding:** Male + female in same pasture. Each night, each male impregnates one available female. Gestation = 12 days. One male can keep 12 females continuously pregnant.
- **Butchering:** Yields bone, skull, hide, and meat
- Animals purchased from merchants at appropriate Kingdom Worth thresholds

### Wild Animals
- Bears, wolves, and other creatures exist on the map
- Some can be hostile

---

## 7. Enemies/Threats

### Surface Invaders

| Enemy | Trigger | Behavior |
|-------|---------|----------|
| **Goblins** | Kingdom Worth (end of year 1) | Scale with KW; start unarmored, gain equipment as KW rises. Weak to slash damage. Iron/steel goblins are very tough. |
| **Goblin Tunnelers** | After raiders fail to enter | Dig tunnels into your fortress if raiders can't find entry. Can be bribed with weapons/armor/crafts. |
| **Ogres** | Kingdom Worth (with goblins) | Single and two-headed variants. No weapons but very tough. |
| **Mants** | Food/drink stores (not KW) | Scouts spawn when food/drink stockpiles are large. |

### Underground Enemies

| Enemy | Behavior |
|-------|----------|
| **Beetles** | Create cocoons that spawn more beetles (population explosion if unchecked, causes lag) |
| **Skeletons** | Underground spawners |
| **Zombies** | Underground spawners |
| **Golems** | Multiple types, underground |
| **Spiders** | Underground |

### Threat Escalation
- Day 2+: Underground creatures spawn in dark places
- End of Year 1: Goblins begin surface invasions
- Continuous scaling: Higher KW = more numerous and better-equipped enemies
- If goblins can't get in, tunnelers come next time
- Great Hall value counts DOUBLE for invasion calculations
- Mants are independent -- triggered by food/drink stockpile size, not KW

### Spawning Mechanics
- Surface invaders appear at map edges and seek fortress entry
- Underground spawners spawn in dark areas (torches prevent spawning)
- Beetle cocoons create exponential growth if left alone

---

## 8. Tech/Progression

There is **no formal tech tree**. Progression is organic and tied to:

1. **Kingdom Worth thresholds** -- Higher KW unlocks:
   - More gnomad immigrants per season
   - Access to better merchant goods (iron, silver, gold, platinum ores)
   - Harder enemy invasions
   - New livestock types from merchants (alpaca at ~25K, emu at ~50K)

2. **Mining depth** -- Deeper Z-levels contain better ores:
   - Surface: Stone, clay
   - Mid-depth: Copper, tin, coal
   - Deep: Iron (Z -37 to -50), gold, silver, platinum, gems

3. **Tinker Bench research** -- Unlocks new mechanism types and mechanical parts (the closest thing to a "tech tree")

4. **Crafting chain progression:**
   - Crude Workbench -> Proper workshops
   - Copper tools -> Bronze -> Iron -> Steel
   - Raw food -> Cooked meals
   - Raw materials -> Refined goods -> Finished products

5. **Trading progression:**
   - Host ambassadors from discovered kingdoms
   - Different kingdom types offer different goods (Mining colonies sell metals cheap, buy food expensive)
   - Higher KW unlocks higher-tier merchant goods

---

## 9. What Was Missing / Player Complaints

### Critical Bugs
1. **Container Bug** -- Wheelbarrows/sacks lose attributes during stockpiling, permanently bugging gnomes
2. **Sleep-Carrying Bug** -- Gnomes falling asleep while carrying items become permanently stuck
3. **Item Ghost Bug** -- Items appear on ground but become uninteractable
4. **Merchant Trade Bug** -- Traded items don't always materialize properly
5. **Wrong Material Bug** -- Gnomes use incorrect materials for sub-components of complex items

### Performance Issues
6. **Late-game lag** -- Game becomes unplayably slow after ~10 years
7. **Beetle population lag** -- Unchecked beetles cause severe performance issues
8. **Stockpile system lag** -- New stockpile system caused performance degradation
9. **Crashes** -- Average 10 crashes per 5-hour session reported by some players
10. **Underground rendering lag** -- Toggle transparency (E key) causes lag underground

### Design Complaints
11. **No tutorial / No tooltips** -- Game refuses to teach players anything
12. **No feedback on failures** -- When something goes wrong, NO indication of what or why
13. **Priority system broken** -- Gnomes won't prioritize urgent tasks correctly
14. **No emergency priority** -- Can't force gnomes to do life-or-death tasks
15. **Mid/endgame content gap** -- Once self-sufficient, no remaining challenge
16. **Enemy scaling too aggressive for new players** -- Monsters end civilizations too early on "easy"
17. **No corpse burial** -- Can't bury fallen gnomes
18. **Iron too deep** -- Requires mining to Z -37 to -50
19. **Pasture dependency** -- Forced reliance on pastures with no alternatives
20. **Clunky underground visibility** -- Hard to see fortress construction underground

### Missing Features Players Wanted Most
21. **Gnome personality/social systems** -- Marriage, children, individual goals, preferences
22. **Education system** -- Schools for child gnomes
23. **Alchemy/magic system** -- Potions, antidotes, stat boosts
24. **Better water physics** -- Rivers, pressure, irrigation, aquifers
25. **Lava** -- Lava forges, magma-based power
26. **More complex combat** -- Beyond tank-rush strategies
27. **Automated defenses** -- Stationary arrow dispensers
28. **Gnome leadership** -- Elect a king/leader with kingdom-wide skill bonuses
29. **Animations** -- Gnomes working (swinging axes at trees, etc.)
30. **Cheese from yak milk** -- (and generally more food processing variety)
31. **Better save system** -- Quick save, rotating autosaves, unlimited manual saves
32. **More underground incentives** -- Unique items, magical resources, reasons to dig beyond metals
33. **Mod support** -- Official Steam Workshop integration

### Development Abandonment
- Developer (RoboB0b) disappeared, deleted Reddit/Twitter/forum accounts
- Cited expecting a baby, took a break, never returned
- Game shipped 1.0 but was missing ~half of the published roadmap features
- 84% positive Steam reviews overall, but many late reviews are negative about abandonment
- Community split between "it's abandoned" vs "it was released as 1.0"

---

## 10. UI/UX

### Interface Organization
- **Isometric view** with 4 cardinal direction rotation
- **Z-level navigation** with +/- or W/S keys (confusing -- players thought these should change layers, not camera)
- **E key** toggles grey-undiscovered-tiles transparency
- Multiple zoom levels
- Population/Professions tabs for gnome management
- Workshop interface for crafting orders
- Designation tools for mining, farming, stockpiles, rooms, pastures

### What Worked
- Once learned, the interface was functional
- Workshop auto-chaining (ordering complex items cascades to dependent workshops)
- Stockpile filtering system for organizing goods
- The isometric view with rotation gave good spatial awareness

### What Didn't Work
- **Terrible learning curve** -- No tutorial, no tooltips, nothing teaching the interface
- **Underground visibility** -- Very hard to see fortress construction underground
- **No failure feedback** -- When orders aren't being executed, no indication of why
- **Clunky multi-level navigation** -- Hard to understand what's happening at different Z-levels
- **Priority system UI** -- Difficult to get gnomes to do what you want when you want it
- **Dated interface** -- Felt clunky and slow compared to peers
- **No quick-access hotkeys** -- Lots of menu diving required

### Key Takeaway for Ingnomia
The UI was Gnomoria's weakest point after performance. The game was extremely deep mechanically but actively hostile to new players. The #1 improvement opportunity is making the same depth of systems accessible through a modern, intuitive interface with proper tutorials, tooltips, and feedback systems.

---

## Summary: What Ingnomia Should Keep vs Improve

### Keep (Core Strengths)
- Sandbox freedom with no prescribed path
- Deep crafting chains with material variety
- Kingdom Worth as the central progression driver (attracts both gnomads and enemies)
- The satisfying build-craft-grow loop
- Military squad customization with perks and formations
- Mechanism/trap system
- Trading with different kingdom types

### Improve (Critical Gaps)
- **Performance** -- Must handle late-game without lag
- **UI/UX** -- Modern interface with tutorials, tooltips, and failure feedback
- **Gnome depth** -- Personalities, relationships, social systems, children
- **Endgame content** -- Deeper threats, goals, narrative hooks
- **Combat variety** -- More tactical options beyond tank-rush
- **Underground incentives** -- More reasons to dig deep (magic, unique resources, lava)
- **Water/fluid physics** -- Proper rivers, irrigation, aquifers
- **Save system** -- Multiple saves, autosave
- **Mod support** -- First-class modding capabilities
- **Bug elimination** -- The container/sleep/ghost-item bugs were devastating to player trust

---

## Sources

- [Gnomoria Wiki (Fandom)](https://gnomoria.fandom.com/wiki/Gnomoria_Wiki)
- [Gnomoria Basics - Beginner's Guide (Steam)](https://steamcommunity.com/sharedfiles/filedetails/?id=247712396)
- [Unofficial Military Guide (Steam)](https://steamcommunity.com/sharedfiles/filedetails/?id=523097405)
- [Unofficial Production Guide (Steam)](https://steamcommunity.com/sharedfiles/filedetails/?id=535310713)
- [Professions Guide (Steam)](https://steamcommunity.com/sharedfiles/filedetails/?id=305077332)
- [Gnomoria on Steam](https://store.steampowered.com/app/224500/Gnomoria/)
- [Your biggest problem with Gnomoria (Steam discussion)](https://steamcommunity.com/app/224500/discussions/0/353916584666963658/)
- [Features you would like to see (Steam discussion)](https://steamcommunity.com/app/224500/discussions/0/810923021656460898/)
- [Ingnomia Feature Request List (Steam)](https://steamcommunity.com/app/709240/discussions/0/1709564547929033404/)
- [Features compared to Gnomoria and DF (Steam)](https://steamcommunity.com/app/709240/discussions/0/1474222595293550558/)
- [Gnome Attributes discussion (Steam)](https://steamcommunity.com/app/224500/discussions/0/810920513884245750)
- [Is this game abandoned? (Steam)](https://steamcommunity.com/app/224500/discussions/0/2119355556488520774/?ctp=2)
- [Hostile Creatures Wiki](https://gnomoria.fandom.com/wiki/Hostile_Creatures)
- [Workshops Wiki](https://gnomoria.fandom.com/wiki/Workshops)
- [Mechanicals Wiki](https://gnomoria.fandom.com/wiki/Mechanicals)
