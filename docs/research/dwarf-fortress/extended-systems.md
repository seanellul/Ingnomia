# Dwarf Fortress Mechanics Research -- Reference for Ingnomia

Research compiled from the DF wiki (dwarffortresswiki.org). Each section covers mechanical rules and design philosophy for adaptation into Ingnomia.

---

## 1. Creatures & Animals

### Creature Definition System
DF defines creatures using a **token-based system** in raw text files. Each creature has:
- **Body plan**: Defines physical structure (limbs, organs, body parts). Body plans are templates (HUMANOID, QUADRUPED, INSECT, etc.) that can be customized per creature.
- **Castes**: Variants within a species (e.g., male/female, worker/soldier for ant-men with 4 castes). Each caste can have different sizes, abilities, and roles.
- **Creature tokens**: Tags that define behavior and capabilities:
  - `[FREQUENCY:X]` (0-100): spawn likelihood
  - `[CLUSTER_NUMBER:X]`: group size when spawning
  - `[POPULATION_NUMBER:X]`: max visiting creatures
  - `[PET]` / `[PET_EXOTIC]`: domesticable
  - `[TRAINABLE]`, `[TRAINABLE_WAR]`, `[TRAINABLE_HUNTING]`: training capability
  - `[GRAZER:<value>]`: requires pasture grazing
  - `[LAYS_EGGS]`: egg production
  - `[MILKABLE]`: can be milked

### Size System
- Measured in cm3 (roughly equals grams for flesh creatures)
- Range: 500 (rabbit) to 25,000,000 (dragon)
- Size varies by individual height/broadness
- Size affects: butchering yield, combat effectiveness, grazing requirements, training difficulty

### Creature Categories
| Category | Examples | Role |
|---|---|---|
| Civilized races | Dwarves, humans, elves, goblins, kobolds | Form societies, trade, wage war |
| Domestic animals | Cows, chickens, dogs, cats, sheep | Livestock, products, companions |
| Wild animals | Deer, bears, eagles | Hunting, danger, training |
| Vermin | Rats, flies, cave spiders | Nuisance, contamination |
| Megabeasts | Dragons, hydras, rocs, bronze colossi | Major threats |
| Semi-megabeasts | Cyclops, ettin, giant, minotaur | Medium threats |
| Procedural | Forgotten beasts, titans | Unique per-world threats |
| Night creatures | Vampires, werebeasts, necromancers | Infiltration/curse threats |
| Underground | Cave creatures per cavern layer | Cavern ecosystem |

### Domestic Animal Products

**Milkable animals** (all on 17-day cooldown):
Alpaca, camel (one-humped & two-humped), cow, donkey, goat, horse, kangaroo, llama, pig, purring maggot, reindeer, sheep, tapir, water buffalo, yak.
- Milk -> cheese at kitchen, or used in cooking. Cannot be consumed raw.
- Requires: farmer's workshop, empty bucket, milking labor enabled.

**Egg-laying animals** (domestic): Chickens, turkeys, peafowl, blue peafowl, geese, ducks, and various others with `LAYS_EGGS` token. Also exotic: dragons, giant eagles, kobolds.
- Require nest boxes. Fertilized eggs hatch after exactly 3 months.
- Unfertilized eggs collected as food. Cooking yields prepared meals.
- Clutch sizes vary by species.

**Shearable animals** (wool): Sheep, llamas, alpacas. (Trolls only by goblins.)
- Sheared at farmer's workshop every 300 days.
- Produces yarn -> spun into cloth at loom -> crafted into goods.

**Butchering products** (all slaughtered animals yield some subset):
Meat, bones, skull, fat (-> tallow at kitchen), hide (-> leather at tannery), horns, hooves, hair, cartilage, ivory/teeth (species-dependent). Larger creatures yield more products.

### Animal Training
- Training levels: Wild -> Semi-wild -> Trained (quality levels) -> Tame
- **War animals**: Stronger, used as bodyguards. Dogs, bears, big cats, elephants.
- **Hunting animals**: Faster, stealthier, accompany hunters.
- 40+ trainable species. Size correlates with combat effectiveness.
- Civilization-level knowledge system makes training easier over time.
- Higher training quality = longer before animal reverts to wild.

### Design Philosophy
The creature system's power comes from its **composability** -- body plans, tokens, castes, and materials combine to create enormous variety from a relatively small set of rules. For Ingnomia, this suggests a data-driven creature definition system rather than hard-coded creature types.

---

## 2. Enemies & Megabeasts

### Threat Taxonomy

**Megabeasts** (4 fixed types):
| Beast | Key Ability | Notes |
|---|---|---|
| Dragon | Fire breath, flight | Size 25,000,000. Breathes fire, destroys buildings |
| Hydra | Multiple heads | Regenerates, extremely tough |
| Roc | Flight, massive size | Aerial threat |
| Bronze Colossus | Metal body, indestructible | Immune to most damage |

**Semi-megabeasts** (4 types): Cyclops, Ettin, Giant, Minotaur -- medium-tier threats, often lair in caves.

**Forgotten Beasts** (procedurally generated, underground):
- Exactly one per underground region. Spawn in cavern layers.
- **Body forms**: blob, worm, slug, quadruped, humanoid, insect, spider, amorphous
- **Materials**: flesh, ash, steam, rock, gem, metal (copper through steel), water, fire, glass, grime
- **Special attacks**: venomous stings, poisoned blood, explosive dust clouds, fire breath, web-shooting, noxious secretions, blood-sucking, deadly dust
- All are building destroyers, trap-immune, stun-immune
- Steel-bodied forgotten beasts are fortress-ending threats
- **Trigger**: 50,000 created wealth + discovered cavern

**Titans** (procedurally generated, surface):
- Same generation system as forgotten beasts but spawn on the surface
- Biome-specific titles (marsh titan, savanna titan)
- Can block bridge operation by standing on them
- Inhabit shrines in adventure mode

**Werebeasts**:
- ~64 mammal types + ~9 reptile types (werewolf, werebear, weregiraffe, etc.)
- Transform on 13 specific full moon dates per year (3-4 day duration)
- During transformation: complete health restoration, legendary combat skills, pain/fatigue/drowning immunity, building destroyer level 2, trap-immune
- Halved weapon damage BUT one random metal weakness (10x damage)
- Infection via bite (must penetrate skin/fat/muscle)
- Can also be cursed by toppling temple statues or angering deities
- **Key design tension**: infected dwarves become threats from within

**Vampires**:
- Created during world gen when deities curse temple desecrators
- Arrive as migrants with **false identities** -- hidden name and kill list
- Never among starting 7, children, or caravan members
- Abilities: 2x strength/agility/toughness, enhanced speed, no food/water/sleep needs, immortal, pain-immune, sterile
- Feed on sleeping creatures' blood. Victims drained dry.
- Detection: never eat/drink/sleep, excessive skills, many group associations, corpses drained of blood
- **Key design tension**: infiltration mystery, social deduction gameplay
- Can be weaponized: isolated vampires serve as eternal administrators

**Necromancers**:
- Historical figures obsessed with mortality who gain secrets from death-sphere deities
- Read artifact slabs containing "secrets of life and death" to gain powers
- Abilities: immortal (still breathe, unlike vampires), raise corpses within ~15 tiles via line-of-sight gesture
- Fortress dwarves can become necromancers by reading slabs/books in libraries
- **Sieges**: Massive disorganized zombie hordes. Each killed defender can be reanimated.
- **Necromancer towers** in world gen: require 50+ undead followers, spread evil regions
- World gen setting "Number of Secret Types" controls prevalence

### Siege System

**Trigger requirements**: 80+ population (except undead attacks, which can come anytime)
- Cap: 120 soldiers + 40 monsters per siege

**Siege types by race**:
| Attacker | Trigger | Behavior | Equipment |
|---|---|---|---|
| Goblins | Automatic (no war needed) | Blind charge with mounts | Mixed, trolls, beak dogs |
| Elves | Trade wooden goods, deforest | Massive mounted numbers | Wooden weapons (weak) |
| Humans | Kill diplomats, destroy wagons | Establish camps, patient tactics | Metal gear |
| Undead/Necromancer | Any time | Shamble from all map edges | Reanimation chains |
| Dwarves | Raid their sites, refuse artifact demands | Steel-equipped martial trances | Best equipment |
| Bandits | Year one possible | Small, poorly equipped | Basic |

**Escalation mechanics**:
- Wealth thresholds trigger increasing threats
- Megabeasts require: 100,000 total wealth + 10,000 exported wealth + 80 population
- Forgotten beasts: 50,000 created wealth + discovered cavern
- Diplomats who visit report trap layouts to future siegers (humans become trap-immune)
- Winter freezes moats, removing water defenses
- Necromancer sieges create exponential threats via reanimation

### Design Philosophy
DF's threat system works because of **variety and escalation**. Early threats (wildlife, bandits) teach basics. Mid-game introduces sieges that test defenses. Late-game brings procedurally-generated existential threats that can't be fully predicted. The social threats (vampires, werebeasts within your own population) add paranoia. For Ingnomia, the key lesson is layered threat types that test different defensive strategies.

---

## 3. World Generation & History

### Geography Generation Pipeline
1. **Seed values**: elevation, rainfall, temperature, drainage, volcanism, wildness on a grid
2. **Fractal fill**: expand seed values fractally across the map
3. **Peak placement**: mountains and altitude adjustments
4. **Rejection testing**: worlds failing criteria (inadequate mountains, biome distribution) are rejected and regenerated. 86th rejection triggers Easter egg message.
5. **Vegetation & biomes**: determined by elevation + rainfall + temperature
6. **Erosion & rivers**: mountain edge detection, river flow simulation, lake placement, elevation smoothing
7. **Final adjustments**: rain shadows, temperature corrections for elevation/forests, salinity for oceans, geological layers

### World Size Options
| Size | Dimensions | Forgotten Beasts |
|---|---|---|
| Pocket | 17x17 | 12 |
| Smaller | 33x33 | 27 |
| Small | 65x65 | 75 |
| Medium | 129x129 | 243 |
| Large | 257x257 | 867 |

Each region tile = 16x16 local blocks of 48x48 tiles each. Maximum world size comparable to the state of Minnesota.

### History Simulation
After geography, DF runs a **"giant zero-player strategy game"** with thousands of agents:

**Duration options**: Very Short (5 years), Short (125), Medium (250), Long (550), Very Long (1,050 years). Minimum possible: 2 years.

**What happens during history simulation**:
- Civilizations rise, establish sites, wage wars, fall
- Historical figures are born, live, achieve things, die
- Artifacts are created and change hands
- Megabeasts rampage across populated areas
- Necromancers are expelled, build towers
- Vampires infiltrate societies
- Wars reshape territory
- Roads are built between settlements

**Key parameters**:
- Number of Civilizations (3-160 depending on world size)
- Maximum Sites (towns, hamlets, retreats, towers)
- Number of Beasts (megabeasts, semi-megabeasts, titans)
- Natural Savagery (aggressive wildlife density)
- Mineral Occurrence (sparse to frequent)

**World ages**: Age of Myth -> Age of Legends -> Age of Heroes (as megabeasts are killed off)

### Legends System
A dedicated game mode for exploring world history:
- All historical figures tracked: named creatures, megabeasts, demons, deities
- Sites: towns, towers, fortresses, retreats, caves
- Organizations: civilizations, governments, religions
- Events: battles, duels, rampages, foundings, diplomatic incidents
- Interactive historical map showing territorial shifts over time
- Player actions in Fortress/Adventure mode become permanent history
- Information can be hidden, requiring adventurers to discover it via conversation, reading, examining artifacts

### Design Philosophy
World generation is DF's most iconic feature. The key insight is that **history creates context** -- your fortress isn't in a vacuum, it exists in a world with thousands of years of accumulated events. Artifacts have provenance, enemies have motivations rooted in history, and your actions become part of the legend. For Ingnomia, even a simplified version of pre-generated history (a few hundred years, major events, civilization relationships) would add enormous depth. The "zero-player strategy game" approach is elegant -- simple AI rules produce emergent complexity.

---

## 4. Biomes & Regions

### Biome Generation Factors
Biomes are determined by a hierarchy of values:
1. **Elevation**: 0-99 = ocean, 300-400 = mountain
2. **Drainage + Rainfall**: base biome type
3. **Salinity**: 0 = freshwater, 66+ = saltwater
4. **Temperature**: 85+ = tropical, -5 or below = frozen

### Surface Biome Types

**Wetlands**: Swamps and marshes (freshwater/saltwater, temperate/tropical), mangrove swamps

**Forests**: Taiga, temperate coniferous, temperate broadleaf, tropical coniferous, tropical dry broadleaf, tropical moist broadleaf

**Plains**: Grasslands, savannas, shrublands (temperate or tropical variants)

**Deserts**: Badlands, rocky wastelands, sand deserts

**Water Bodies**: Oceans (arctic/temperate/tropical), lakes, rivers, murky pools (freshwater/saltwater/brackish)

**Cold**: Glaciers, tundra

**Mountains**: Extensive ore deposits, no soil

### Alignment & Savagery System
Every biome has two additional modifiers:
- **Alignment**: Good / Neutral / Evil
  - Good biomes have unicorns, benign creatures
  - Evil biomes have undead reanimation, evil fog, thralling
- **Savagery**: Peaceful / Neutral / Savage
  - Savage biomes have giant versions of animals, aggressive wildlife

This creates a 3x3 matrix (9 combinations) overlaid on every biome type, massively multiplying variety.

### Underground Layers (Caverns)
Three distinct cavern layers, each spanning multiple z-levels:

**Cavern Layer 1** (shallowest):
- Relatively safer fauna (giant cave spiders, blind cave bears)
- Underground trees: tower-caps, fungiwood
- Underground plants: plump helmets, cave wheat, pig tail, quarry bush
- Underground grass: cave moss, floor fungus
- Water pools provide mud for farming

**Cavern Layer 2**:
- More dangerous creatures
- Similar vegetation but harder to access safely

**Cavern Layer 3** (deepest):
- Most dangerous creatures (cave dragons, voracious cave crawlers)
- Connects to the magma sea below
- Blood thorns (dangerous plant)

**Below Layer 3**: The magma sea -- infinite magma for forges, extremely dangerous

**Key rule**: Caverns without water pools have only muddy floor fungus, no trees or plants except blood thorns.

### Aquifers
Subterranean water-bearing rock layers:

**Light aquifers** (~95%): Slow water production, can be penetrated with patience. ~4 water units/month.

**Heavy aquifers** (~5%): Near-instant flooding, effectively blocks excavation. Must use special techniques (cave-in, pumps, freezing).

- Geographic distribution: low elevations near rivers/oceans. Mountains rarely have them.
- Provide infinite freshwater (advantage) but block access to minerals below (disadvantage)

### Design Philosophy
The biome system's brilliance is in **layered modifiers** -- base biome type x alignment x savagery creates 100+ distinct environments from relatively few variables. The underground cavern layers add vertical exploration with escalating risk/reward. For Ingnomia, the alignment/savagery overlay system is particularly worth adapting -- it multiplies content variety cheaply.

---

## 5. Needs & Preferences

### The Needs System (v0.42+)
Replaced simple "on break" mechanic. Unmet needs cause stress, bad thoughts, and damage creature focus.

### Focus Mechanic
Focus = percentage of met vs unmet need values.
- Very Focused (140%+): +50% skill bonus
- Focused: moderate bonus
- Badly Distracted (60% or below): -50% skill penalty

This creates a direct economic incentive to keep dwarves happy -- distracted dwarves work at half speed.

### Need Categories

**Physical Needs**:
- Drink alcohol (influenced by immoderation personality trait)
- Eat good meals (preferred foods, valuable meals)
- Take leisure time

**Social Needs**:
- Spend time with people (gregariousness)
- Be with family and friends
- Make romance

**Mental/Creative Needs**:
- Practice a craft (masonry, carpentry, jewelry, smithing, etc.)
- Create art or perform
- Learn / gain skills
- Hear eloquent speech or poetry

**Spiritual/Martial Needs**:
- Pray / meditate (varies by religiousness trait)
- Practice martial arts
- Wander / hunt / gather

**Behavioral Needs**:
- Fight / cause trouble
- Help others
- Think abstractly
- Admire art / engravings

### Satisfaction Through Locations

**Taverns**: Socializing, drinking, eating, hearing performances, telling stories. But risk: excessive drinking, bar fights, foreign spy infiltration. Require: meeting area zone, drink containers, optional tavern keeper and performers.

**Temples**: Prayer, meditation, communion with deity. "Serious enraptured stress decrease." Some dwarves need deity-specific temples, not just generic ones. Tiers: Shrine (<2000 value), Temple (2000+), Temple Complex (10,000+ with high priesthood). Require: 25+ tile dance floor, musical instruments.

**Libraries**: Reading, scholarly discussion, knowledge creation. Scholars discuss topics regardless of physical distance within the zone. Visiting scholars bring books and may petition for residency. Libraries influence world history (5 book picks per year per library). Require: bookcases, writing materials, tables, chairs.

### Room Quality System
Room value = total value of floor/walls + all furniture/constructions inside.

**Quality tiers**:
| Level | Value | Name |
|---|---|---|
| 1 | 1 | Meager Quarters |
| 2 | 100 | Modest Quarters |
| 3 | 250 | Decent Quarters |
| 4 | 500 | Fine Quarters |
| 5 | 1,000 | Great Quarters |
| 6 | 1,500 | Grand Quarters |
| 7 | 2,500 | Royal Quarters |
| 8 | 10,000 | Royal Bedroom |

**Value boosters**: Smoothed/engraved walls and floors, artifact furniture, gem-encrusted items, weapon traps (10 components + mechanism in one tile).

**Penalties**: Overlapping rooms reduce value by 75%. Unenclosed rooms get partial penalty on tile values.

**13 room types**: Bedroom, dormitory, dining hall, office, barracks, tomb, jail, meeting hall, zoo, memorial hall, sculpture garden, museum + more.

### Design Philosophy
The needs system creates a **virtuous cycle** where happy dwarves are productive dwarves. The location types (tavern, temple, library) create infrastructure goals beyond basic survival. Room quality scales smoothly from functional to luxurious. For Ingnomia, the key insight is that needs should be **personality-driven** (not every dwarf has the same needs) and satisfiable through building specific facility types.

---

## 6. Artifact System

### Strange Moods (Artifact Creation)

**Trigger conditions**: No active mood + artifact limit not reached + at least 20 eligible dwarves. Then ~2.4% daily chance.

**Five mood types**:
| Mood | Requirement | Behavior | Outcome |
|---|---|---|---|
| Fey | Any | Clearly states material demands | Artifact + legendary skill + happy thought |
| Secretive | Any | Sketches pictures instead of stating demands | Artifact + legendary skill |
| Possessed | Any | Cryptic requests | Artifact but NO skill gain |
| Macabre | Unhappy dwarf | Requires bones/skulls/vermin remains | Artifact (bone/skull themed) |
| Fell | Unhappy dwarf | Murders nearest dwarf for materials | Artifact from victim's remains |

**Material demands**: Fetched in order. Dwarves demand their profession's primary material first. Metalworkers prefer metals matching preferences. 20 moodable skills determine which workshop is claimed.

**Failure**: If materials not provided within 50,000 ticks (~42 days), dwarf goes insane (berserk, melancholy, or catatonic) and eventually dies. Timer resets after each item delivered.

**Success**: Dwarf gains 20,000 experience (legendary), strong happy thought, partial immunity to future insanity. Artifact is permanent.

### Artifact Properties
- **3x quality bonus** to weapon accuracy and armor deflection (vs 2x for masterwork)
- **Indestructible**: Cannot be damaged by temperature, building destroyers, or fire
- Cannot be dumped, melted, or further decorated
- **Artifact cages** permanently contain any vermin
- **Artifact doors/hatches** create indestructible, instantly-lockable gates
- Artifact furniture dramatically boosts room quality for noble requirements

### Artifact Naming
- 1% chance to be named after the creator (if happy during Fey mood)
- Warriors can name equipment used to slay megabeasts
- All artifacts receive generated names in the game's constructed languages

### World-Level Artifact Dynamics
- Non-player dwarves create artifacts during world gen
- Civilizations track artifact ownership (keepers, entity gifts, family heirlooms)
- Lost artifacts trigger quester/agent dispatches
- **Wars can be declared over artifact claims**
- Stealing artifacts triggers diplomatic consequences and potential invasion

### Design Philosophy
The artifact system is brilliant because it **transforms a mundane crafting system into narrative moments**. The moods create tension (will you have the right materials?), the artifacts create permanent landmarks in your fortress history, and world-level artifact dynamics create diplomatic gameplay. For Ingnomia, the mood system is very adaptable -- it requires only: mood triggers, material demands, skill-based workshop selection, and a failure state.

---

## 7. Noble & Administrative System

### The Hierarchy

**Utility nobles** (player-appointed, functional):
- Manager: automates work orders
- Broker: handles trade negotiations
- Bookkeeper: tracks inventory accuracy
- Chief Medical Dwarf: oversees hospital

**Elected nobles**:
- Expedition Leader: initial leader (first 2 years)
- Mayor: elected by population vote after year 2

**Aristocratic nobles** (appointed by higher authority):
- Baron (-> Count -> Duke): escalating authority and demands
- Monarch: apex of hierarchy

**Military positions**:
- Militia Commander, Militia Captains
- Captain of the Guard / Sheriff
- General

### Appointment Mechanics
- Utility nobles: directly appointed/replaced by player
- Mayor: population vote
- Baron+: requires higher-ranking noble authorization
- Militia Commander must return from missions before Captain appointments

### Mandate System
Nobles issue **mandates** (fortress-wide orders):
- **Production mandates**: Craft specific items within ~6 months. Caravan goods don't count.
- **Export bans**: Temporarily restrict trading certain item types for ~6 months.

Mandate capacity scales with rank:
| Rank | Simultaneous Mandates |
|---|---|
| Baron/Mayor | 1 |
| Count | 2 |
| Duke | 3 |
| Monarch | 5 |

**Violation consequences**: Failing production mandates -> unhappy noble + dwarves sentenced to punishment. Trading banned items -> crime charges for haulers (even if items were traded before ban took effect, creating unfair situations).

### Room Requirements
Noble room demands scale dramatically:
- Baron: Level 4 Decent Quarters with modest furnishings
- Duke: Level 7 Grand Bedrooms with opulent throne rooms
- Jealous nobles become upset if "lesser" dwarves have superior rooms

### Design Philosophy
The noble system creates **beneficial tension** -- nobles provide morale bonuses and administrative functions, but their demands consume resources and create restrictions. The mandate system forces players to sometimes produce unwanted items or face punishment. For Ingnomia, the key elements to adapt are: escalating room requirements (drives base-building), mandates (create production pressure), and the utility noble roles (provide actual gameplay benefits that justify the overhead).

---

## 8. Industry, Economy & Trade

### Trade Caravan System

**Seasonal arrivals**:
| Season | Civilization | Notes |
|---|---|---|
| Spring | Elves | Refuse wood-derived products; wooden weapons (weak traders) |
| Summer | Humans | Moderate guards; large-sized clothing (useless for dwarves) |
| Autumn | Dwarves | Always arrive unless extinct; heavily guarded; metal goods |

- Caravans depart 25-33 days after arriving regardless of trade success
- Merchants go insane if prevented from leaving for ~2 months
- Require wagon-navigable paths to trade depots
- Will flee if encountering corpses or hostile wildlife
- **Caravans don't arrive during active sieges**

### Trade Value Mechanics
- Broker skill affects visible prices and negotiation
- Novice+ Appraisal skill reveals item values
- Judge of Intent skill shows merchant mood (ecstatic -> unwilling)
- Dwarven traders initially demand ~100% profit margin
- Happier merchants = lower profit margin demands
- Broker gains 50 skill points per transaction attempt

### Trade Agreements
- Outpost liaisons arrive with dwarven caravan to negotiate
- Players can request specific imports for next year
- Can offer production contracts at premium prices
- Agreements appear in Civilization menu

### Civilization Preferences
- **Dwarves**: Accept all goods. Bring metal bars, weapons, armor, food, books.
- **Elves**: Refuse anything wood-derived, clear/crystal glass, or decorated with those materials. Offering wood products "greatly offends" the merchant. Bring exotic animals, cloth, crafts.
- **Humans**: Accept diverse goods. Bring food, cloth, leather goods, some metals.

### Tribute System
Successful raid missions convert regular caravans to tribute caravans that drop items at depot without negotiation. Contents unpredictable -- from mundane clothing to exotic animals.

### Diplomatic Consequences of Trade
- Destroying caravans damages relations through escalating categories: "never seen again" -> "suffered hardships" -> "seized goods" -> "missing goods"
- Repeated destruction triggers sieges
- Trade agreements build goodwill over time

### The Fortress Economy Loop
1. **Production**: Create valuable goods (crafts, furniture, weapons, prepared food)
2. **Export**: Trade surplus for needed materials (metals, exotic animals, seeds)
3. **Import requests**: Order specific goods for next year via liaison
4. **Wealth accumulation**: Total wealth attracts both caravans and enemies
5. **Tension**: High wealth brings better trade but also megabeast attacks

### Design Philosophy
DF's economy creates a **risk-reward feedback loop** -- wealth production improves quality of life and trade options but simultaneously attracts threats. The civilization-specific trade restrictions (elves refusing wood) create interesting constraints. The seasonal caravan schedule creates planning horizons. For Ingnomia, the core elements to adapt are: seasonal caravans with civilization-specific goods/restrictions, a broker skill system, trade agreements for planning, and the wealth-attracts-threats feedback loop.

---

## Summary: Top Design Principles from DF

1. **Data-driven definitions**: Creatures, materials, items all defined by composable tokens, not hard-coded classes. Maximizes variety from minimal code.

2. **Layered modifiers**: Biome type x alignment x savagery; creature type x material x special ability. Multiplication beats addition for content variety.

3. **Escalating risk-reward**: Wealth and population unlock better tools/trade but attract proportionally greater threats. Players choose their difficulty curve.

4. **Procedural generation for replayability**: Forgotten beasts, titans, world history, artifact names -- no two games are identical in the details that matter most.

5. **Social simulation creates emergent stories**: Dwarf needs, noble demands, vampire infiltration, werebeast infection -- the best DF stories come from systems interacting unexpectedly.

6. **History provides context**: Pre-generated world history means your fortress exists in a living world with provenance, grudges, and ongoing events.

7. **Tension through competing demands**: Nobles want luxury but consume resources. Trade brings goods but attracts enemies. Caverns offer resources but unleash forgotten beasts. Every benefit has a cost.

8. **Vertical depth**: Underground layers (3 caverns + magma sea) create literal and figurative depth. Each layer is more dangerous and more rewarding.
