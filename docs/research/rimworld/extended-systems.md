# RimWorld Mechanics Reference for Ingnomia

Comprehensive research document covering 8 major game systems from RimWorld, intended as a design reference for building equivalent content in Ingnomia.

---

## 1. ANIMALS

### Training System

Animals have trainability categories that determine what they can learn:
- **None**: Cannot be trained at all
- **Simple**: Can learn Guard only
- **Intermediate**: Can learn Guard + Attack
- **Advanced**: Can learn Guard + Attack + Rescue + Haul

**Training Levels:**

| Training | Intelligence | Steps | Effect |
|----------|-------------|-------|--------|
| Guard | Simple | 3 | Animal follows master, attacks nearby aggressors. Master gets +5 mood |
| Attack | Intermediate | 5 | Animal can be commanded to attack distant enemies |
| Rescue | Advanced | 2 | Animal rescues downed master and colonists within 75-tile radius |
| Haul | Advanced | 7 | Animal hauls items like a colonist (capacity based on body size) |

**Training Mechanics:**
- 6 in-game hour cooldown between training attempts on the same animal
- Skills decay over time; decay speed depends on wildness percentage
- Higher handler skill = fewer training attempts needed
- Bonded animals are 5x easier for their master to train
- Farm animals don't lose tameness regardless of wildness

### Bonding System

Bonds form through:
- Wound treatment: 0.4% chance per treatment
- Successful taming: 1% chance
- Successful training: 0.7% chance
- Proximity (within 12 tiles): 0.1% chance per 2,500 ticks

Bonded animals give master +5 mood. Unbonded assigned animals give no bonus. Unassigned bonded animals cause -3 mood.

### Manhunter Mechanics

- **Revenge chance on harm**: Base percentage varies by species, 3x higher for melee attacks vs ranged
- **Manhunter packs**: Scaria-infected animals arrive as a threat event, last 24-54 hours
- **Psychic wave (mad animals)**: Drives all local wildlife manhunter simultaneously

### Animal Products

**Milk Producers:**

| Animal | Amount | Interval | Per Day |
|--------|--------|----------|---------|
| Cow | 14 | 1 day | 14 |
| Elk | 11 | 1 day | 11 |
| Yak | 11 | 1 day | 11 |
| Dromedary | 18 | 2 days | 9 |
| Caribou | 10 | 2 days | 5 |
| Goat | 12 | 3 days | 4 |

- Each milk unit = 0.05 nutrition
- Milking takes 400 ticks (6.67 seconds)
- Production halved when hungry, 1/3 when urgently hungry, stops when starving

**Wool Producers:**

| Animal | Wool Type | Amount | Interval |
|--------|-----------|--------|----------|
| Megasloth | Megasloth wool | 200 | 20 days |
| Mastodon* | Mastodon wool | 200 | 20 days |
| Bison | Bison wool | 120 | 15 days |
| Muffalo | Muffalo wool | 120 | 15 days |
| Muskox* | Muskox wool | 120 | 15 days |
| Alpaca | Alpaca wool | 45 | 10 days |
| Sheep | Sheep wool | 45 | 10 days |

*Odyssey DLC. Shearing takes 1,700 ticks (28.33 seconds).

**Egg Layers:**

| Animal | Eggs/Clutch | Interval | Nutrition/Egg | Fertilizable |
|--------|-------------|----------|---------------|--------------|
| Chicken | 1 | 1 day | 0.25 | Yes |
| Duck | 1 | 1 day | 0.25 | Yes |
| Goose | 1 | 2 days | 0.5 | Yes |
| Turkey | 1 | 1.33 days | 0.5 | No |
| Cassowary | 1 | 3.33 days | 0.5 | No |
| Emu | 1 | 3.33 days | 0.5 | No |
| Ostrich | 1 | 3.33 days | 0.6 | No |
| Cobra | 1.5 | 10 days | 0.25 | No |
| Iguana | 1.5 | 5.66 days | 0.25 | No |
| Tortoise | 2 | 6.66 days | 0.25 | No |

- Unfertilized eggs rot in 15 days unless refrigerated
- Fertilized eggs don't rot but need 0-50C temperature range
- Eggs substitute for 5 meat in cooking recipes

### Nuzzling Animals
Only certain species nuzzle colonists: cats, guinea pigs, huskies, labradors, monkeys, Yorkshire terriers.

---

## 2. ENEMIES/THREATS

### Mechanoid Types

| Type | Combat Power | Armor (S/B/H) | Health Scale | Weapons | Speed |
|------|-------------|----------------|--------------|---------|-------|
| Pikeman | 110 | 40/20/200% | 0.85 | Needle gun | 2.5 c/s |
| Termite | 110 | 56/22/200% | 2.16 | Thump cannon | 2.1 c/s |
| Scyther | 150 | 40/20/200% | 1.32 | Blade melee | 4.7 c/s |
| Lancer | 190 | 40/20/200% | 0.72 | Charge lance | 4.7 c/s |
| Centipede | 400 | 72/22/200% | 4.32 | Varies (3 variants) | 1.9 c/s |

**Mechanoid Characteristics:**
- No pain, mood, food, or temperature needs
- Immune to fire and toxic buildup
- 200% heat armor across all types
- Vulnerable to EMP (but adapt after 2,200 ticks, becoming temporarily immune)
- Die immediately when downed or at 0% Manipulation
- Never retreat

**Biotech DLC Mechanoids (Player-controllable via Mechanitor):**
- **Combat**: Militor (1 bandwidth), Scorcher (1), Legionary (2), Tesseron (3), Centurion (5), Diabolus (5), War Queen (5)
- **Labor**: Agrihand, Cleansweeper, Constructoid, Fabricor, Lifter, Paramedic, Tunneler (3)
- Require bandwidth from Mechanitor to control
- Consume power, produce pollution when recharging

### Insectoid Types

| Type | Combat Power | Armor (S/B/H) | Health Scale | DPS | Speed |
|------|-------------|----------------|--------------|-----|-------|
| Megascarab | 40 | 72/18/0% | 0.4 | 2.5 | 3.75 c/s |
| Spelopede | 75 | 18/18/0% | 1.7 | 3.5 | 3.65 c/s |
| Megaspider | 150 | 27/18/0% | 2.5 | 4.62 | 3.6 c/s |
| Hive Queen* | 500 | 22/27/0% | 9.8 | 11.54 | 3.4 c/s |

*Odyssey DLC

**Insectoid Characteristics:**
- 100% toxic resistance and vacuum resistance
- Produce insect jelly from hives
- Stay within 10-tile radius of hive when dormant
- Rush up to 100 tiles when triggered (any insectoid killed/downed)
- Mine through walls (sapper behavior)
- No ranged attacks

**Infestation Mechanics:**
- Spawn under overhead mountain roofs within 30 tiles of structures
- Require temperature above -17C (reduced chance below -8C)
- Hives reproduce more insects over time
- "Too Deep" event spawns insects from deep drill operations

### Raid Types

| Type | Description |
|------|-------------|
| Assault | Direct frontal attack |
| Center Drop | Drop pods onto colonists |
| Scatter Drop | Dispersed pod deployment |
| Smart | Tactical pathfinding avoiding defenses |
| Sappers | Mine through walls to bypass defenses |
| Breachers | Destroy walls indiscriminately |
| Siege | Establish fortified position with mortars |

**Raid Point Formula:**
```
Raid Points = (Wealth Points + Pawn Points) x Difficulty x Starting Factor x Adaptation Factor
```

**Wealth Points:**
- Colony Wealth = Items + Creatures + (Buildings x 0.5)
- 0-14,000 wealth: 0 points
- 14,000-400,000: ~1 point per 167 wealth
- 400,000-1,000,000: Up to 4,200 points
- 1,000,000+: Capped at 4,200

**Pawn Points:**
- 15-140 points per colonist (scales with wealth)
- Slaves: 75% multiplier
- Cryptosleep: 30%
- Children: 0-100% based on age
- Animals: 8% of combat power

**Difficulty Multipliers:**
- Peaceful: 0.10
- Community Builder: 0.30
- Adventure Story: 0.60
- Strive to Survive: 1.00
- Blood and Dust: 1.55
- Losing is Fun: 2.20

**Starting Factor:** 0.7 at day 10, linearly to 1.0 by day 40

**Point Caps:** Min 35, Max 10,000
- 10,000 points = ~250 Tribals, ~100 Pirates, ~66 Scythers, or ~25 Centipedes

**Raider Equipment Progression:**
- Early: Autopistols, crude melee, some bolt-action rifles and pump shotguns
- Late: Frag grenades, sniper rifles, incendiary launchers, machine pistols, rare assault rifles/LMGs

**Human Raider Retreat:** Flee when sufficient casualties/downed or damage threshold reached
**Mechanoid Raids:** Fight to destruction, never retreat

### Weather/Environmental Threats

| Event | Effect | Duration | Cooldown |
|-------|--------|----------|----------|
| Toxic Fallout | 40%/day toxic buildup outdoors, plant death, -5 mood | 2.5-10.5 days | 90 days |
| Volcanic Winter | -7C temp, 30% light reduction, halved wildlife | 7.5-40 days | 140 days |
| Heat Wave | +17C temperature | 1.5-3.5 days | 30 days |
| Cold Snap | -20C temperature | 1.5-3.5 days | 30 days |
| Solar Flare | All powered devices offline | 0.15-0.5 days | - |
| Eclipse | Solar generators offline, outdoor crops stop | 0.75-1.25 days | 15 days |
| Flashstorm | Localized lightning strikes causing fires | 0.075-0.1 days | 15 days |
| Blight | Plant disease spreading within 4-tile radius | Until cut | - |
| Psychic Drone | Gender-specific mood debuff (-12 to -40) | 0.75-1.75 days | - |
| Short Circuit (Zzztt) | Power conduit explosion/fire, battery discharge | Instant | - |

### Other Threat Events

- **Manhunter pack**: Scaria-infected animals, 24-54 hours
- **Mad animal**: Single animal goes manhunter
- **Crashed ship part**: Defoliator (kills plants in 100-tile radius) or Psychic Droner (mood debuff)
- **Mech cluster** (Royalty): Fortified mechanoid position with turrets and condition causers
- **Beavers**: Alphabeaver herd eats all trees

### Positive Events

- **Aurora**: +14 mood to outdoor non-blind pawns, 0.125-0.35 days
- **Psychic soothe**: +16 mood (gender-specific), 1.5-3 days
- **Ambrosia sprout**: Mood-boosting fruit bushes appear
- **Animals join**: Tamed animals arrive
- **Wanderer joins**: New colonist arrives
- **Cargo pods**: Resource drop
- **Man in Black**: Emergency rescuer (1-year cooldown)
- **Meteorite**: Resource crash (~1-year cooldown)

---

## 3. BIOMES

### Temperate Biomes

**Temperate Forest**
- Temp: -25C to 35C | Growing: year-round to 20/60 days
- Disease: 50-day MTB | Forageability: 100%
- Plants: Oak, poplar, berry bush, grass, dandelions, wild healroot
- Animals: Deer, horse, cougar, lynx, bear, wolf, bison, muffalo
- Movement difficulty: 1

**Temperate Swamp**
- Temp: -25C to 35C | Growing: year-round to 20/60 days
- Disease: 1.5/year | Forageability: 75%
- Plants: Maple, willow, cypress, berry bush, chokevine
- Animals: Similar to temperate forest + duck
- Movement difficulty: 4 (6 in winter). Marshy terrain, requires bridges

### Tropical Biomes

**Tropical Rainforest**
- Temp: 0C to 35C | Growing: year-round to 40/60 days
- Disease: 1.7/year | Forageability: 100%
- Plants: Bamboo, cecropia, palm, teak, alocasia, giant rafflesia
- Animals: Elephant, panther, monkey, cobra, capybara, tortoise
- Movement difficulty: 2. Sleeping sickness endemic

**Tropical Swamp**
- Temp: 0C to 35C | Growing: year-round to 40/60 days
- Disease: 2.0/year (highest) | Forageability: 75%
- Plants: Same as rainforest + maple, cypress, chokevine
- Animals: Rainforest species + alligator, hippo
- Movement difficulty: 4

### Hot/Arid Biomes

**Arid Shrubland**
- Temp: 0C to 35C | Growing: year-round to 40/60 days
- Disease: 0.9/year | Forageability: 50%
- Plants: Agave, pincushion cactus, saguaro cactus, drago tree
- Animals: Bison, cougar, elephant, gazelle, ostrich, donkey
- Movement difficulty: 1

**Desert**
- Temp: -25C to 35C | Growing: year-round to 10/60 days
- Disease: 0.7/year | Forageability: 25%
- Plants: Agave, pincushion cactus, saguaro, drago tree
- Animals: Ostrich, gazelle, iguana, dromedary, fennec fox
- Movement difficulty: 1. Soft sand patches prevent building

**Extreme Desert**
- Temp: -5C to 35C | Growing: year-round to 30/60 days
- Disease: 0.7/year | Forageability: 0%
- Plants: Agave, saguaro cactus, grass only
- Animals: Minimal - dromedary, iguana, fennec fox, donkey
- Movement difficulty: 1. Never grazable

### Cold Biomes

**Boreal Forest**
- Temp: -35C to 25C | Growing: 10/60 to 30/60 days
- Disease: 1.0/year | Forageability: 75%
- Plants: Pine, birch, poplar, moss, berry bush, wild healroot
- Animals: Arctic fox, wolf, elk, caribou, grizzly bear, lynx
- Movement difficulty: 1 (3 in winter)

**Cold Bog**
- Temp: -35C to 25C | Growing: 10/60 to 30/60 days
- Disease: 1.3/year | Forageability: 50%
- Plants: Maple, willow, moss, astragalus, berry bush, chokevine
- Animals: Arctic species + polar bear, duck
- Movement difficulty: 4 (6 in winter)

**Tundra**
- Temp: -50C to 25C | Growing: 20/60 days to never
- Disease: 0.8/year | Forageability: 50%
- Plants: Birch, pine, moss, grass, astragalus, wild healroot
- Animals: Caribou, megasloth, muffalo, polar bear
- Movement difficulty: 1 (3 in winter). Animal migrations 1-2x/year

**Ice Sheet**
- Temp: -70C to 10C | Growing: Never
- Disease: 0.7/year | Forageability: 0%
- Plants: None | Animals: Arctic fox, polar bear, megasloth, penguin
- Movement difficulty: 1.5 (3.5 in winter). Indoor farming only

**Sea Ice**
- Temp: -60C to 10C | Growing: Never
- Disease: 0.7/year | Forageability: 0%
- Plants: None | Animals: Arctic fox, polar bear, penguin, walrus
- Movement difficulty: 1.5 (3.5 in winter). No geothermal vents, no mining

### Odyssey DLC Biomes

**Grassland**: Open plains, excellent farming/ranching, 100% forageability, disease 1.2/year
**Glowforest**: Sulfur geysers, luminescent fungi, darkened skies, disease 1.2/year
**Scarlands**: Toxic ruins, mechanoid presence, scaria-infected animals, 25% forageability
**Glacial Plain**: Glacier-scraped terrain, frozen ruins, 0% forageability
**Lava Field**: Volcanic wastes, periodic eruptions, mineral deposits, 50% forageability

---

## 4. BACKSTORIES

### System Overview

- Every pawn gets a **childhood backstory** (always)
- Pawns generated at age 20+ also get an **adulthood backstory**
- ~36 childhood backstories, ~85 adult backstories
- 25% chance of getting a "solidBio" (player-created character)

### Effects

Backstories modify three things:
1. **Skill modifiers**: -5 to +9 in any of 12 skills
2. **Work type restrictions**: Enable or disable specific work categories
3. **Forced traits**: Some backstories force specific personality traits

### Skill Categories
Shooting, Melee, Social, Intellectual, Medicine, Cooking, Plants, Mining, Construction, Crafting, Artistic, Animals

### Work Types That Can Be Disabled
ManualDumb, ManualSkilled, Violent, Social, Intellectual, Cooking, PlantWork, Mining, Animals, Caring, Artistic, Cleaning, Hauling, Crafting

### Backstory Categories
- **Imperial Common**: Servants, cooks, craftspeople, priests
- **Imperial Fighter**: Soldiers, medics, engineers, snipers, officers
- **Imperial Royal**: Guards, warmasters, regents, spymasters
- **Offworld Glitterworld**: Scientists, surgeons, novelists, architects
- **Offworld Medieval**: Farm oafs, lords, blacksmiths, sailors
- **Offworld Nonspecific**: Corporate workers, miners, builders, farmers
- **Outsider**: Police, teachers, nurses, bartenders, artists
- **Pirate**: Convicts, torturers, defectors

### Representative Adult Backstories

| Name | Category | Skills | Disabled Work | Forced Trait |
|------|----------|--------|---------------|-------------|
| Recon Sniper | Imperial Fighter | Shooting +9 | - | Jogger |
| Royal Cook | Imperial Common | Cooking +7, Plants +2 | - | Gourmand |
| Menagerie Keeper | Imperial Common | Animals +8 | - | Beautiful |
| Glitterworld Empath | Offworld Glitter | Social +8, Medicine +2 | Violent | anti-Psychopath |
| Planetary Miner | Offworld | Mining +8 | - | Undergrounder |
| Novelist | Offworld Glitter | Artistic +8, Construction -5, Mining -5 | ManualDumb, Social | - |
| Medieval Lord | Offworld Medieval | Social +7, Melee +5, Shooting +5 | ManualDumb, ManualSkilled | - |
| Medieval Farm Oaf | Offworld Medieval | Plants +8, Mining +3 | Intellectual | - |
| Torturer | Pirate | Melee +6 | Social, Caring | Psychopath |
| Charity Worker | Offworld Glitter | Social +6, Mining -3 | Violent | anti-Psychopath |
| Warmonger | Imperial Fighter | Shooting +7, Melee +4, Social -3 | Caring | Bloodlust |
| Sculptor | Offworld Glitter | Artistic +9, Mining +4, Plants -4 | ManualDumb, Crafting, Cooking | - |
| Ascetic Priest | Outsider | Plants +4, Medicine +2, Cooking +2 | Violent, Social | Ascetic |
| Spymaster | Imperial Royal | Social +4, Shooting +3, Melee +3 | - | Psychopath |
| Corporate Fixer | Offworld | Social +4, Shooting +3, Melee +3 | - | Psychopath |
| Loyal Janissary | Imperial Fighter | Shooting +7, Melee +4 | - | Industrious |
| Ballet Dancer | Imperial Common | Social +3, Melee +3 | - | Nimble |
| Mathematician | Outsider | Intellectual +8, Shooting -3 | ManualDumb | - |
| Assembler | Offworld | Crafting +7 | - | - |
| Crop Farmer | Offworld | Plants +8 | - | - |
| Nurse | Outsider | Social +4, Medicine +5 | Violent | - |
| Bodyguard | Outsider | Shooting +4, Melee +4 | Social | - |
| Construction Engineer | Outsider | Construction +8, Plants -3 | Intellectual, Cooking | - |

---

## 5. FACTIONS

### Human Factions

| Faction | Tech Level | Default Goodwill | Permanent Enemy | Can Trade |
|---------|-----------|-----------------|-----------------|-----------|
| Gentle Tribe | Neolithic | -50 to +50 | No | Yes |
| Fierce Tribe | Neolithic | -100 to -80 | No (hostile start) | No |
| Savage Tribe | Neolithic | -100 (locked) | Yes | No |
| Cannibal Tribe* | Neolithic | -100 (locked) | Yes | No |
| Nudist Tribe* | Neolithic | -50 to +50 | No | Yes |
| Civil Outlander | Industrial | -50 to +50 | No | Yes |
| Rough Outlander | Industrial | -100 to -80 | No (hostile start) | No |
| Pirate Gang | Industrial+ | -100 (locked) | Yes | No |
| Cannibal Pirates* | Industrial+ | -100 (locked) | Yes | No |
| Empire** | Ultratech | Varies | No | Yes (requires title) |

*Ideology DLC, **Royalty DLC

### Non-Human Factions

| Faction | Behavior | Permanently Hostile |
|---------|----------|-------------------|
| Mechanoids | Autonomous robots, no retreat | Yes |
| Insectoids | Underground hive builders | Yes |
| Ancients | Found in cryptosleep caskets | Usually hostile |

### Goodwill System

**Thresholds:**
- Hostile: Goodwill < -75
- Neutral: -75 to +75
- Ally: Goodwill > +75
- Range: -100 to +100
- Relations slowly drift toward faction's default range

**Positive Actions:**
- Rescue fallen members: +16
- Destroy mutual enemy bases: +20
- Return prisoners: +12 to +15
- Fulfill trade requests: +12
- Destroy raider outposts: +8
- Trade: +1 per 500 silver
- Gifts: +1 per 40-160 silver value

**Negative Actions:**
- Arrest visiting member: -75 (instant hostile)
- Strip downed pawn: -40
- Remove organs/limbs: -20
- Settle near faction base: Continuous drain
- Hostile psycast on member: -30 to -50

### Faction Interactions
- **Trade caravans**: Cost 15 goodwill to summon, arrive in 2 days
- **Military aid**: Allied factions send troops
- **Raids**: Hostile factions attack with strength scaling to wealth
- **Visitors**: Neutral/friendly factions visit temporarily
- **Peace talks**: Negotiation event with 5 outcome variants
- **Faction bases**: Can assault and destroy; 24-hour loot window; destroying all bases eliminates faction

---

## 6. DISEASES/CONDITIONS

### Fatal Diseases

| Disease | Severity/Day | Immunity/Day | Treatment Effect | Notes |
|---------|-------------|-------------|-----------------|-------|
| Infection | +0.84 | +0.644 | -0.53 severity | From wounds/burns/frostbite |
| Plague | +0.666 | +0.522 | -0.362 severity | Extremely painful (+20-85% pain) |
| Malaria | +0.370 | +0.314 | -0.232 severity | Tropical biomes, impairs blood filtration |
| Flu | +0.249 | +0.239 | -0.077 severity | Mild, elderly at risk |
| Sleeping Sickness | +0.12 | +0.11 | -0.07 severity | Rainforests/swamps only, very slow |
| Lung Rot | +0.300 | N/A | -1.000 severity | From rot stink exposure |
| Organ Decay | +0.017-0.033 | N/A | Organ replacement only | Targets heart/lungs/kidneys |

### Non-Fatal Diseases

| Disease | Effect | Cure |
|---------|--------|------|
| Gut Worms | 2x hunger, +20% pain | 300% cumulative tend quality |
| Muscle Parasites | -30% manipulation/movement, +20% pain | 300% cumulative tend quality |
| Fibrous Mechanites | Pain + tiredness, BUT +50% manipulation/movement/blood pumping | Fades in 15-30 days |
| Sensory Mechanites | Pain + tiredness, BUT +50% sight/hearing/talking, +30% manipulation | Fades in 15-30 days |
| Scaria | Drives animals manhunter, lethal in 5 days | Surgery (3 medicine + skill 8) |

### Chronic Conditions

| Condition | Effect | Treatment |
|-----------|--------|-----------|
| Bad back | Movement reduction | Bionic spine |
| Cataracts | Vision impairment | Bionic eyes |
| Dementia | Cognitive decline | Healer mech serum/luciferium |
| Alzheimer's | Cognitive decline | Healer mech serum/luciferium |
| Frail | General weakness | Biosculpter/healer serum |
| Artery blockage | Cardiovascular restriction | Bionic heart |
| Carcinoma | Malignant tumor | Bionic organ/healer serum |
| Hearing loss | Auditory damage | Bionic ears |

### Environmental Conditions

| Condition | Cause | Effect |
|-----------|-------|--------|
| Heatstroke | High temperature | Consciousness reduction, pain |
| Hypothermia | Cold exposure | Progressive severity, movement penalty |
| Malnutrition | Inadequate food | Resolves with nutrition |
| Toxic buildup | Toxic fallout/waste | Permanent damage at 40% severity |
| Food poisoning | Dirty kitchen/poor cook | Vomiting, mood penalty |

### Immunity System

- Each disease has base immunity gain speed
- Factors affecting immunity: food saturation, bed rest (hospital bed + vitals monitor best), age (decreases after 54), blood filtration, Super-immune trait (+30%), Immunoenhancer implant
- Penoxycyline drug prevents malaria, plague, and sleeping sickness
- Diseases are NOT contagious (except Scaria)
- Multiple colonists affected in single event simulates outbreaks

### Treatment Hierarchy
1. Natural healing (minor injuries only)
2. Medical treatment (stabilizes, slows progression)
3. Organ transplants
4. Bionics/artificial parts
5. Healer mech serum (cures almost anything)
6. Luciferium (powerful but permanently addictive)
7. Biosculpter pod (Ideology DLC)

### Disease Frequency by Difficulty
- Peaceful: 3x multiplier
- Community Builder: 2.5x
- Adventure Story: 1.3x
- Strive to Survive: 1.0x
- Blood and Dust: 0.95x
- Losing is Fun: 0.9x

---

## 7. APPAREL/WEAPONS/MATERIALS

### Quality System

| Quality | Accuracy (Ranged) | Melee Damage | Armor Factor | Insulation | Market Value | Deterioration |
|---------|-------------------|-------------|-------------|------------|-------------|---------------|
| Awful | 0.80x | 0.90x | 0.60x | 0.80x | 0.50x | 2.00x |
| Poor | 0.90x | 1.00x | 0.80x | 0.90x | 0.75x | 1.50x |
| Normal | 1.00x | 1.00x | 1.00x | 1.00x | 1.00x | 1.00x |
| Good | 1.10x | 1.00x | 1.15x | 1.10x | 1.25x | 0.80x |
| Excellent | 1.20x | 1.00x | - | - | - | - |
| Masterwork | 1.35x | 1.25x | 1.45x | 1.50x | 2.50x | 0.30x |
| Legendary | 1.50x | 1.50x | 1.80x | 1.80x | 5.00x | 0.10x |

**Quality Determination:**
- Based on crafter skill level (standard distribution centered on skill)
- Skill 6 = Normal quality on average
- Inspired Creativity: +2 quality levels
- Production Specialist (Ideology): +1 quality level

### Melee Weapons

| Weapon | Tech | Damage Type | Base DMG | Cooldown | DPS | Armor Pen |
|--------|------|-------------|----------|----------|-----|-----------|
| Club | Neolithic | Blunt | 6.31 | 2.0s | 3.16 | 18.83% |
| Knife | Neolithic | Sharp | 7.02 | 1.63s | 4.31 | 17.06% |
| Ikwa | Neolithic | Sharp | 6.70 | 2.0s | 3.35 | 19.63% |
| Spear | Neolithic | Sharp | 7.88 | 2.6s | 3.03 | 41.5% |
| Gladius | Medieval | Sharp | 7.06 | 2.0s | 3.53 | 21.08% |
| Longsword | Medieval | Sharp | 7.90 | 2.45s | 3.22 | 28.58% |
| Mace | Medieval | Blunt | 6.95 | 2.0s | 3.48 | 21.08% |
| Monosword | Ultra | Sharp | 10.3 | 1.9s | 5.42 | 64.8% |
| Zeushammer | Ultra | Blunt+EMP | 9.41 | 2.75s | 3.42 | 38.33% |
| Persona Monosword | Ultra | Sharp | 14.53 | 1.6s | 9.08 | 72% |
| Persona Zeushammer | Ultra | Blunt+EMP | 13.17 | 2.05s | 6.42 | 40% |

**Melee Material Modifiers (vs Steel baseline):**
- **Plasteel**: 0.8x cooldown, 1.1x sharp damage, 2.8x HP
- **Uranium**: 1.1x cooldown, 1.5x blunt/1.1x sharp damage
- **Jade**: 1.3x cooldown, 1.5x blunt/1.0x sharp damage
- **Gold**: No cooldown penalty, 0.75x sharp damage
- **Wood**: 0.9x blunt, 0.4x sharp damage
- **Stone**: 1.3x cooldown, reduced sharp (clubs only)

### Ranged Weapons

**Neolithic:**

| Weapon | DMG | AP | Range | Accuracy (T/S/M/L) | DPS | Stopping |
|--------|-----|----|----|---------------------|-----|----------|
| Short Bow | 11 | 16 | 22.9 | 75/65/45/25% | 3.67 | 0.5 |
| Recurve Bow | 14 | 21 | 25.9 | 70/78/65/35% | 4.52 | 1.0 |
| Greatbow | 17 | 15 | 29.9 | 65/85/75/50% | 4.86 | 1.5 |
| Pila | 25 | 10 | 18.9 | 80/71/50/32% | 3.85 | 2.5 |

**Industrial:**

| Weapon | DMG | AP | Range | Accuracy (T/S/M/L) | DPS | Stopping |
|--------|-----|----|----|---------------------|-----|----------|
| Autopistol | 10 | 15 | 25.9 | 80/70/40/30% | 7.69 | 0.5 |
| Revolver | 12 | 18 | 25.9 | 80/75/55/40% | 6.32 | 1.0 |
| Machine Pistol | 6 | 9 | 19.9 | 90/65/35/15% | 11.02 | 0.5 |
| Heavy SMG | 12 | 18 | 22.9 | 85/65/35/20% | 12.34 | 0.5 |
| Pump Shotgun | 18 | 14 | 15.9 | 80/87/77/64% | 8.37 | 3.0 |
| Chain Shotgun | 18 | 14 | 12.9 | 57/64/55/45% | 18.73 | 3.0 |
| Bolt-Action Rifle | 18 | 27 | 36.9 | 65/80/90/80% | 5.63 | 1.5 |
| Assault Rifle | 11 | 16 | 30.9 | 60/70/65/55% | 10.88 | 0.5 |
| LMG | 12 | 16 | 25.9 | 40/48/35/26% | 18.08 | 1.0 |
| Sniper Rifle | 25 | 38 | 44.9 | 50/70/88/90% | 5.0 | 1.5 |
| Minigun | 10 | 15 | 30.9 | 20/25/25/18% | 41.67 | 0.5 |

**Spacer:**

| Weapon | DMG | AP | Range | Accuracy (T/S/M/L) | DPS | Stopping |
|--------|-----|----|----|---------------------|-----|----------|
| Charge Rifle | 16 | 35 | 27.9 | 55/64/55/45% | 14.12 | 0.5 |
| Charge Lance | 30 | 45 | 32.9 | 65/85/85/75% | 6.82 | 1.5 |

**Explosives/Grenades:**

| Weapon | DMG | Range | Blast Radius | DPS |
|--------|-----|-------|-------------|-----|
| Frag Grenades | 50 | 12.9 | 1.9 | 12.0 |
| EMP Grenades | 50 | 12.9 | 3.5 | 12.0 |
| Molotov Cocktails | 10 | 12.9 | 1.1 | 2.4 |
| Doomsday Rocket | 50 | 36 | 7.8 | 5.56 |
| Triple Rocket | 50 | 36 | 3.9 | 15.52 |

**Ranged Weapon Mechanics:**
- Firing cycle: Warmup (aiming) -> Shot/Burst -> Cooldown
- Accuracy = Shooting Accuracy x Weapon Accuracy x Weather x Smoke x Cover
- Stopping power 1 = stagger humans (83% slow for 95 ticks); 4 = stagger elephants
- Fog reduces accuracy to 50%; smoke adds 30% penalty

### Armor

| Armor | Sharp | Blunt | Heat | Move Penalty | Value |
|-------|-------|-------|------|-------------|-------|
| Simple Helmet | 50% | 50% | 50% | - | 56 |
| Flak Helmet | 70% | 70% | 70% | - | 220 |
| Flak Vest | 100% | 36% | 27% | -0.12 c/s | 225 |
| Flak Pants | 55% | 8% | 10% | -0.12 c/s | 225 |
| Plate Armor | 90% | 90% | 90% | -0.8 c/s | 300 |
| Recon Helmet | 92% | 40% | 46% | - | 525 |
| Recon Armor | 92% | 40% | 46% | - | 1540 |
| Marine Helmet | 106% | 45% | 54% | - | 635 |
| Marine Armor | 106% | 45% | 54% | -0.25 c/s | 2035 |
| Cataphract Helmet | 120% | 50% | 60% | - | 745 |
| Cataphract Armor | 120% | 50% | 60% | -0.5 c/s | 3120 |
| Locust Armor | 87% | 35% | 41% | - | 2230 |
| Grenadier Armor | 101% | 40% | 49% | -0.25 c/s | 2305 |
| Phoenix Armor | 115% | 45% | 75% | -0.5 c/s | 3480 |

**Armor Damage Calculation:**
- Armor Penetration subtracted from Armor Rating
- Random roll 0-100 compared to result:
  - Below 50% of armor: Complete deflection
  - 50-100% of armor: Damage halved (sharp converted to blunt)
  - Above armor: Full damage
- Each armor point below 100% = 0.75% net damage reduction
- Each armor point above 100% = 0.25% net damage reduction

### Apparel Layers
1. **Skin**: Undergarments (button-down shirt, pants)
2. **Middle**: Vests, mid-layer armor (flak vest)
3. **Outer**: Visually displayed layer (duster, jacket)
4. **Shell**: Heavy armor (marine armor, cataphract)
5. **Belt**: Utility items (shield belt)
6. **Eyes**: Blindfolds only

**Apparel Durability:**
- Combat: 1 HP per 4 damage directed at layer
- Daily wear: 40% chance for 0.4 HP loss per day
- 20-50% HP: -3 mood ("Ratty apparel")
- Below 20%: -5 mood ("Tattered apparel")

**Tainted Apparel** (worn during death):
- 1 item: -5 mood, 2: -8, 3: -11, 4+: -14
- Market value decreased 90%; traders reject

**Nudity:**
- Males need leg coverage; females need legs + chest
- Missing coverage: -6 mood
- Nudist trait: +20 mood naked

### Materials

**Fabrics:**

| Textile | Value | Sharp | Blunt | Heat | Cold Ins | Heat Ins |
|---------|-------|-------|-------|------|----------|----------|
| Cloth | 1.5 | 0.36x | 0x | 0.18x | +18C | +18C |
| Devilstrand | 5.5 | 1.4x | 0.36x | 3.0x | +20C | +24C |
| Synthread | 4.0 | 0.94x | 0.26x | 0.9x | +22C | +22C |
| Hyperweave | 9.0 | 2.0x | 0.54x | 2.88x | +26C | +26C |
| Alpaca wool | 3.8 | 0.36x | 0x | 1.1x | +30C | +16C |
| Muffalo wool | 2.7 | 0.36x | 0x | 1.1x | +28C | +12C |
| Megasloth wool | 2.7 | 0.8x | 0x | 1.1x | +34C | +12C |
| Sheep wool | 2.7 | 0.36x | 0x | 1.1x | +26C | +10C |
| Bison wool | 2.7 | 0.36x | 0x | 1.1x | +26C | +12C |

**Notable Leathers:**

| Leather | Value | Sharp | Cold Ins | Heat Ins | Special |
|---------|-------|-------|----------|----------|---------|
| Thrumbofur | 14 | 2.08x | +34C | +22C | 8x beauty, 2x HP |
| Rhinoceros | 4.2 | 1.29x | +14C | +14C | 1.5x HP |
| Heavy fur | 3.3 | 1.24x | +30C | +14C | 1.5x HP |
| Bearskin | 3.4 | 1.12x | +20C | +20C | 1.3x HP |
| Elephant | 3.0 | 1.12x | +14C | +12C | 1.5x HP |
| Chinchilla fur | 6.5 | 0.67x | +30C | +16C | 3.6x beauty |
| Guinea pig fur | 5.0 | 0.67x | +38C | +18C | Best cold insulation |
| Human leather | 4.2 | 0.64x | +12C | +12C | Mood penalties |
| Plainleather | 2.1 | 0.81x | +16C | +16C | Baseline leather |
| Lightleather | 1.9 | 0.54x | +12C | +12C | Weakest |
| Patchleather | 1.5 | 0.45x | +9C | +9C | Worst overall |

**Metals (for weapons/structures):**
- **Steel**: Baseline (1.0x all stats)
- **Plasteel**: 1.14x sharp armor, 2.8x HP, best weapon material
- **Uranium**: 1.08x sharp armor, 1.5x blunt melee damage
- **Gold**: 4x beauty, 0.72x sharp, ceremonial use
- **Silver**: Lower protection, moderate beauty
- **Jade**: 0.45x blunt armor, beautiful (stony category)

**Stone Types** (walls/floors only):
- Granite, Limestone, Marble, Sandstone, Slate
- Cannot be used for apparel
- Stone structures provide no sharp armor

---

## 8. IDEOLOGY/CULTURE

### System Structure

Players select 1-4 **memes** (core beliefs) that define the ideoligion. Memes unlock/constrain **precepts** (specific rules), **rituals**, **roles**, and **buildings**.

### Memes by Impact

**High Impact:**
- Animal Personhood: Animals have equal rights to humans
- Blindsight: Only the blind perceive true reality
- Cannibal: Must consume human flesh
- Nudism: Clothing is wrong
- Darkness: Light is abhorrent
- Tunneler: Underground living is ideal
- Tree Connection: Trees are sacred
- Transhumanist: Body modification is essential
- Inhuman: Rejects human nature

**Medium Impact:**
- Raider: Enrichment through raiding
- Rancher: Animals are central to life
- Proselytizer: Must spread the faith
- Flesh Purity: Body must remain unmodified
- High Life: Drug use is spiritual
- Nature Primacy: Nature above all
- Ritualist: Ceremonies are essential
- Bloodfeeding: Blood consumption

**Low Impact:**
- Collectivist / Individualist
- Supremacist / Loyalist / Guilty

### Precept Categories (19 mandatory)

| Precept | Range | Mood Effects |
|---------|-------|-------------|
| Cannibalism | Abhorrent to Required | -20 to +6 mood |
| Corpses | Ugly to Don't Care | Mood penalty or none |
| Drug Use | Prohibited to Essential | -20 to +mood from use |
| Body Modification | Abhorrent to Approved | -10 to +13 per mod |
| Slavery | Disapproved to Honorable | Social penalties to bonuses |
| Execution | Abhorrent to Required | -mood to +mood |
| Raiding | Disapproved to Required | -mood to +mood |
| Charity | Despised to Essential | Various |
| Mining | Prohibited to Required | Various |
| Tree Cutting | Prohibited to Required | Various |
| Physical Love | Restricted to Free | Various |
| Marriage | Partner count, naming | Various |
| Diversity of Thought | Bigotry to Exalted | -25 to +50 opinion |
| Eating Nutrient Paste | Don't Mind to Disgusting | 0 to -mood |
| Blindness | Horrible to Sublime | -mood to +mood |
| Comfort/Pain | Ignored to Idealized | Various |

### Rituals

Special ceremonies that provide mood bonuses and development points.

**Anytime Rituals (1-2 dev points):** Dance parties, funerals
**Dated Rituals (0-7 dev points):** Award ceremonies, recruitment events
**Specialized Rituals:** Scarification, gladiator duels, blinding, cannibal feasts

Quality outcomes depend on meeting requirements. Higher quality = better mood bonuses.

### Roles

**Mandatory:** Leader, Moral Guide
**Specialists (pick 2):** Shooting, Melee, Animals, Plants, Mining, Medical, Research, Production

Specialists gain bonuses in their field but lose access to diverse work types.

### Development Points & Reformation

**Earning Points:**
- Rituals: 1-7 points
- Conversions: 1 point per person
- Relic acquisition: 5 points

**Reformation Cost:** 10 points initially, +2 per reform (max 20)
- Each reform: Add/remove 1 meme OR change structure, plus unlimited other changes

### Conversion Mechanics

- Colonists have **certainty** percentage in their faith
- Conversion attempts reduce certainty
- At 0% certainty, ideology changes
- Proselytizing precept levels: Occasional, Sometimes, Frequent

### Styles (Visual)

0-3 visual styles per ideoligion: Hindu, Christian, Islamic, Buddhist, Morbid, Totemic, Spikecore, Rustic, Animalist, Techist, Horaxian

Matching surroundings: +2 to +5 mood. Mismatching: -2 to -4 mood.

### Faction Goodwill Effects

- Same ideology: +10 goodwill
- Compatible memes: +10 per match (stacking)
- Supremacist/Raider memes: -50 with all factions
- Loyalist: -10 with all
- Guilty: +10 with all

---

## BONUS: CHARACTER TRAITS

### Spectrum Traits

| Positive | Negative | Effect |
|----------|----------|--------|
| Sanguine (+12 mood) | Depressive (-12 mood) | Base mood offset |
| Optimist (+6 mood) | Pessimist (-6 mood) | Base mood offset |
| Iron-Willed (-18% break threshold) | Volatile (+15% break threshold) | Mental break resistance |
| Steadfast (-9% break threshold) | Nervous (+8% break threshold) | Mental break resistance |
| Industrious (+35% work speed) | Slothful (-35% work speed) | Work speed |
| Hard Worker (+20% work speed) | Lazy (-20% work speed) | Work speed |
| Jogger (+0.4 c/s) | Slowpoke (-0.2 c/s) | Move speed |
| Fast Walker (+0.2 c/s) | - | Move speed |
| Beautiful (+2 beauty) | Staggeringly Ugly (-2 beauty) | Social opinion |
| Pretty (+1 beauty) | Ugly (-1 beauty) | Social opinion |
| Super-Immune (+30% immunity) | Sickly (random disease MTB 30 days) | Disease resistance |
| Careful Shooter (+5 acc, +25% aim time) | Trigger-Happy (-5 acc, -50% aim time) | Shooting style |
| Fast Learner (+75% learning) | Slow Learner (-75% learning) | Skill gain |
| Psychically Hypersensitive (+80%) | Psychically Deaf (-100%) | Psychic sensitivity |

### Standalone Traits

| Trait | Key Effects |
|-------|-------------|
| Tough | 50% incoming damage |
| Wimp | -50% pain shock threshold |
| Delicate | 115% incoming damage |
| Nimble | +15 melee dodge, 10% trap spring chance |
| Brawler | +4 melee, -10 shooting, +4 melee hit, -10 mood with ranged |
| Bloodlust | No corpse/butcher penalties, +mood from killing/gore |
| Psychopath | No empathy penalties, no social bonuses, can't bond animals |
| Cannibal | No cannibalism penalties, +15/+20 mood human meat |
| Kind | "Kind Words" interaction (+5 mood, +15 opinion), won't insult |
| Abrasive | 2.3x likely to insult |
| Pyromaniac | +mood near fire, incapable of firefighting, fire binges |
| Gourmand | +4 cooking, +50% hunger rate, food binges |
| Ascetic | +mood for bad rooms, -mood for nice rooms, unaffected by meal quality |
| Greedy | -8 mood without impressive bedroom |
| Jealous | -8 mood if not best bedroom |
| Night Owl | +16 mood at night, -10 during day |
| Undergrounder | +mood indoors, -mood outdoors, no outdoors need |
| Nudist | +20 mood naked, -3 per clothing item |
| Masochist | +5 to +20 mood from pain |
| Body Modder | +4 to +13 mood per bionic part |
| Body Purist | -10 to -35 mood per bionic part |
| Recluse | +12 mood (1 person) to -8 mood (16+ people) |
| Quick Sleeper | +50% rest rate |
| Great Memory | 50% skill loss rate |
| Tortured Artist | 50% creativity inspiration after mental break, -8 permanent mood |
| Very Neurotic | +40% work speed, +14% break threshold |
| Neurotic | +20% work speed, +8% break threshold |
| Too Smart | +75% learning, +12% break threshold |
| Chemical Fascination | Strong drug need, auto-takes drugs |
| Chemical Interest | Moderate drug need |
| Teetotaler | Won't take drugs, -20 mood if forced |
| Asexual | No romantic relationships |
| Gay | Same-gender only |
| Bisexual | Either gender |
| Misandrist | -25 opinion of men |
| Misogynist | -25 opinion of women |
| Annoying Voice | -25 opinion from others |
| Creepy Breathing | -25 opinion from others |
