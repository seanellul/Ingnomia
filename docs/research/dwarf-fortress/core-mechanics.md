# Dwarf Fortress Deep Systems Research -- Game Design Reference for Ingnomia

Research compiled from the Dwarf Fortress Wiki (dwarffortresswiki.org), covering the seven core simulation systems that make DF legendary. Each section is structured as a design reference with mechanical details suitable for adaptation.

---

## 1. PERSONALITY / TRAITS SYSTEM

DF's personality system has three layers: **Facets** (innate temperament), **Values/Beliefs** (what the dwarf cares about), and **Goals** (life aspirations). Together they create unique individuals who react differently to the same circumstances.

### 1.1 Personality Facets (Innate Temperament)

Each facet is a 0-100 scale. Values 40-60 are "neutral" and produce no visible description. Extreme values generate descriptive text in the dwarf's profile. There are approximately 50 facets. Key ones grouped by function:

**Emotional Disposition:**
| Facet | High (75-100) | Low (0-25) | Mechanical Effect |
|-------|---------------|------------|-------------------|
| LOVE_PROPENSITY | Easily develops positive feelings | Never falls in love | Required for romance; conflicts with ROMANCE value |
| HATE_PROPENSITY | Easily develops hatred | Never feels hatred | Drives negative sentiment formation |
| CHEER_PROPENSITY | Often feels joy | Never cheerful | Low values block positive thoughts from drinking/working |
| DEPRESSION_PROPENSITY | Frequently depressed | Never discouraged | High increases melancholy insanity risk |
| ANGER_PROPENSITY | Constant internal rage | Never angry | High increases tantrum/berserk risk |
| ANXIETY_PROPENSITY | Nervous wreck | Incredibly calm | High increases obliviousness/stark-raving-mad risk |
| LUST_PROPENSITY | Constantly lustful | Never lustful | Influences romantic/sexual behavior |
| STRESS_VULNERABILITY | Helpless under stress | Impervious to stress | 50% catatonia chance at highest; core stress resistance stat |
| EMOTIONALLY_OBSESSIVE | Forms lifelong attachments | No emotional attachment | Low prevents emotional responses to deaths |
| SWAYED_BY_EMOTIONS | Buffeted by others' emotions | Never moved by others | Affects Consoler skill learning |

**Social Behavior:**
| Facet | High | Low | Mechanical Effect |
|-------|------|-----|-------------------|
| GREGARIOUSNESS | Treasures company | Prefers solitude | Affects socialization need; Conversationalist skill |
| FRIENDLINESS | Bold flatterer | Quarreler | Always/never learns Flatterer; contributes to grudges |
| POLITENESS | Refined etiquette | Vulgar | Contributes to grudges |
| DISCORD | Revels in chaos | Wants harmony | High: can't learn Consoler/Pacifier; Low: can't learn Intimidator |
| BASHFUL | Crippling shyness | Shameless | High: can't learn Conversationalist |
| ASSERTIVENESS | Aggressive communicator | Never speaks up | Affects Intimidator/Persuader skill learning |
| TRUST | Naturally trusting | Sees others as conniving | Affects social trust |
| HUMOR | Finds everything funny | Utterly humorless | Always/never learns Comedian; contributes to grudges |

**Work/Character Traits:**
| Facet | High | Low | Mechanical Effect |
|-------|------|-----|-------------------|
| GREED | Obsessed with wealth | No interest in material goods | Affects acquisition need |
| VIOLENT | Starts fights | Flees all confrontation | Affects combat avoidance; creates grudges when divergent |
| PERSEVERANCE | Unbelievably stubborn | Drops tasks at first difficulty | Conflicts with PERSEVERANCE value |
| ORDERLINESS | Obsessed with order | Oblivious to neatness | High: stores clothes properly; Low: leaves clothes scattered |
| PERFECTIONIST | Obsessed with details | Sloppy and careless | Contributes to grudges |
| SINGLEMINDED | Focused on one thing | Complete scatterbrain | Affects task focus |
| CURIOSITY | Implacably curious | Incurious | Contributes to grudges |
| BRAVERY | Utterly fearless | Complete coward | Controls stress accumulation speed in combat |
| CONFIDENCE | Blind overconfidence | No confidence | Affects ability assessment |
| VANITY | Wrapped up in appearance | Doesn't care | Contributes to grudges |
| AMBITION | Relentless drive | No ambition | Contributes to grudges |
| VENGEFUL | Never forgets grievances | No sense of vengeance | Contributes to grudges |
| PRIDE | Delusions of self-importance | Convinced of worthlessness | Affects arguments/teaching |
| CRUELTY | Deliberately cruel | Merciful and compassionate | High: can't learn Consoler |
| ALTRUISM | Fulfilled by helping | Helping is an imposition | High: happy thoughts from healing; Low: unhappy thoughts |
| IMMODERATION | Ruled by cravings | Never overindulges | Conflicts with SELF_CONTROL value; alcohol dependency |
| WASTEFULNESS | Careless with resources | Cuts corners | Affects resource efficiency |
| GRATITUDE | Returns favors | Never reciprocates | Contributes to grudges |
| DUTIFULNESS | Profound sense of duty | Hates obligations | Conflicts with LAW, LOYALTY, INDEPENDENCE values |
| CLOSEMINDED | Never changes mind | Goes with prevailing view | Affects debate flexibility |
| TOLERANT | Unbothered by differences | Cannot tolerate differences | Contributes to grudges |
| HOPEFUL | Developed optimism | Lives without hope | Contributes to grudges |
| EXCITEMENT_SEEKING | Seeks danger | Avoids stress | Conflicts with TRANQUILITY value |
| IMAGINATION | Prefers fantasy | Interested only in facts | Contributes to grudges |
| ABSTRACT_INCLINED | Philosophical | Only practical | Contributes to grudges |
| ART_INCLINED | Absorbed in art/nature | Unmoved by beauty | Conflicts with ARTWORK and NATURE values |
| ACTIVITY_LEVEL | Frenetic energy | Languid pace | Affects pace of activity |
| PRIVACY | Paranoid about sharing | Shares everything | Conflicts with STOICISM value |
| IMMODESTY | Extravagant presentation | Austere lifestyle | Affects clothing preferences; contributes to grudges |
| THOUGHTLESSNESS | Acts without deliberation | Overthinks everything | Contributes to grudges |
| DISDAIN_ADVICE | Relies on own counsel | Can't decide without input | Influences decision-making |

**Key Design Insight -- Grudge Formation:** Many facets are tagged as "contributes to grudges." When two dwarves have significantly divergent values in these facets, they form grudges instead of friendships during socialization. This creates emergent social dynamics where some dwarves are naturally incompatible.

### 1.2 Personality Values (Beliefs)

34 values on a -50 to +50 scale. These represent what a dwarf believes is important. Key ones with gameplay effects:

| Value | Gameplay Effect |
|-------|-----------------|
| LAW | "Absolute believer in rule of law" vs. "finds laws abhorrent" |
| LOYALTY | Highest regard for loyalty vs. disgusted by it |
| FAMILY | Family is most important vs. loathsome |
| FRIENDSHIP | Key to ideal life vs. disgusting; affects Conversationalist learning |
| POWER | Acquisition of power is ideal vs. abhorrent; affects Intimidator learning |
| TRUTH | Truth is inviolable vs. lies without compunction; affects Flatterer/Liar learning |
| ELOQUENCE | Artful speech is highest ideal vs. wasteful deception; affects Persuader learning |
| TRADITION | Maintenance of tradition highest ideal; generates thoughts from Champion interaction |
| ARTWORK | Creation/appreciation of art highest ideal vs. would destroy it |
| STOICISM | Any show of emotion is offensive vs. concealment is betrayal; affects Consoler learning |
| TRANQUILITY | Tranquility highest ideal; affects Intimidator learning |
| HARMONY | World in harmony vs. chaos is truest expression; affects Intimidator learning |
| MERRIMENT | Good parties vs. worthless merrymaking; affects Comedian learning |
| NATURE | Nature > civilization; unhappy/happy thoughts from animal slaughter, tree felling |
| PEACE | War is repellent vs. perpetual warfare; affects Pacifier learning |
| CRAFTSMANSHIP | Celebrates talented artisans vs. would desecrate masterworks |
| MARTIAL_PROWESS | Martial prowess defines good character |
| HARD_WORK | Hard work is key to good life |
| ROMANCE | Romance is highest ideal vs. repellent |
| COMMERCE | Commerce is a high ideal |
| KNOWLEDGE | Quest for knowledge is highest value |
| SELF_CONTROL | Self-mastery and denial of impulses |
| INDEPENDENCE | Freedom is non-negotiable |

**Value-Facet Conflicts:** When a dwarf's values contradict their facets (e.g., high SELF_CONTROL value but high IMMODERATION facet), internal psychological tension generates specific thoughts. This creates richly conflicted characters.

### 1.3 Personality Goals (Life Aspirations)

Each dwarf has one or more life goals. Completing a goal generates a strong happy thought.

| Goal | Trigger |
|------|---------|
| START_A_FAMILY | Giving birth / fathering a child |
| CREATE_A_GREAT_WORK_OF_ART | Creating artifact or masterpiece |
| CRAFT_A_MASTERWORK | Creating artifact or masterpiece |
| MASTER_A_SKILL | Reaching Legendary skill level |
| ATTAIN_RANK_IN_SOCIETY | Becoming a baron or similar |
| FALL_IN_LOVE | (undocumented trigger) |
| BECOME_A_LEGENDARY_WARRIOR | (undocumented trigger) |
| BRING_PEACE_TO_THE_WORLD | (undocumented trigger) |
| RULE_THE_WORLD | (undocumented trigger) |
| IMMORTALITY | Leads to necromancy; aging immunity |
| MAKE_A_GREAT_DISCOVERY | (undocumented trigger) |
| SEE_THE_GREAT_NATURAL_SITES | (undocumented trigger) |
| BATHE_WORLD_IN_CHAOS | (undocumented trigger) |

### Design Takeaways for Ingnomia
- Three-layer personality (facets + values + goals) creates enormous combinatorial variety
- Facets drive *how* a gnome reacts; values drive *what* they care about; goals drive *what they aspire to*
- Grudge formation from personality divergence creates emergent social conflict
- Internal value-facet conflicts create psychologically complex characters
- Personality determines which social skills can be learned, creating role specialization

---

## 2. THOUGHTS AND EMOTIONS SYSTEM

### 2.1 Core Architecture

The system tracks **thoughts** (events that happened) which generate **emotions** (feeling responses). Emotions accumulate into a **stress** score. Different personalities generate different emotions from the same thought.

### 2.2 Thought Categories

14 major categories of thoughts:
1. **Accommodations** -- sleeping conditions, bedroom quality
2. **Nourishment** -- food/drink quality, variety, hunger
3. **Amenities** -- furniture, cleanliness
4. **Relations** -- social interactions, deaths
5. **Job satisfaction** -- work quality, skill use
6. **Clothing** -- wear conditions, quality
7. **Wedlock** -- marriage, family events
8. **Noble pretensions** -- title/rank effects
9. **Justice** -- punishment, confinement
10. **Injury and death** -- combat trauma, witnessing death
11. **Military duty** -- combat exposure
12. **Tantrums** -- destructive behavior consequences
13. **Weather** -- rain/sun exposure
14. **Memories** -- reliving past experiences

### 2.3 Emotion System

180 defined emotions (130 implemented). Each has a **divider value** from -8 to 8:
- **Negative divider** = stress-reducing (positive emotions)
- **Positive divider** = stress-increasing (negative emotions)

**Strongest Positive Emotions (divider -8):** Acceptance, Arousal, Contentment, Enjoyment, Enthusiasm, Expectancy, Fondness, Interest, Nostalgia, Patience, Satisfaction, Sympathy, Wonder, Admiration

**Strongest Negative Emotions (divider 8):** Alienation, Annoyance, Anxiety, Boredom, Contempt, Confusion, Disappointment, Disgust, Disillusionment, Dislike, Displeasure, Exasperation, Frustration, Grouchiness, Grumpiness, Irritation, Nervousness, Resentment, Righteous Indignation, Suspicion, Uneasiness, Worry

**Special High-Stress Emotions:**
- **Rage** -- "Enraged at all enemies!" (combat trigger)
- **Terror** -- Creature flees from enemies
- **Angst/Misery** -- Creature lies immobilized
- **Agony, Fear, Panic, Wrath, Horror** -- Severe incapacitation

### 2.4 Stress System

Single numerical value ranging from **-1,000,000 to +1,000,000**.

**Threshold Breakpoints:**
| Threshold | Effect |
|-----------|--------|
| -1,000,000 | Maximum happiness (floor) |
| 0 | Neutral |
| +10,000 | Initial stress symptoms; haggard appearance |
| +25,000 | Emotional breakdowns begin (tantrum/depression/obliviousness) |
| +50,000 | Severe consequences; insanity risk |
| +1,000,000 | Maximum stress (ceiling) |

**Personality Modifiers on Stress:**
- **Bravery** -- Controls accumulation speed (high = slower)
- **Stress Vulnerability** -- Determines breaking point (low = higher tolerance)
- **Anxiety Propensity** -- Controls dissipation rate
- **Depression Propensity** -- Affects recovery difficulty

### 2.5 Memory System

Dwarves form **long-term memories** from significant experiences. These can be revisited, generating new emotion instances. Key findings from community research:
- 89% of dwarves formed blissful memories from 3x3 engraved bedrooms
- 70% formed positive memories from large dining halls
- 93% formed happy memories from military training
- 62% achieved satisfying work-related memories

### 2.6 Major Stress Sources and Reducers

**Top Stress Reducers:**
- Quality bedrooms (engraved)
- Large dining halls
- Military training/sparring
- Meaningful skilled work
- Socializing
- Prayer/worship
- Satisfying needs

**Top Stress Increasers:**
- Witnessing death
- Corpse/remains exposure
- Miasma (rotting smell)
- Rain exposure (dwarves hate rain)
- Trauma
- Talking to grudges (700+ stress/day!)
- Unmet needs

### Design Takeaways for Ingnomia
- Thought -> Emotion -> Stress pipeline creates nuanced mental states
- Same event produces different emotions based on personality
- Long-term memories mean one traumatic event can haunt a gnome for years
- Stress is a single number but personality determines sensitivity
- The 14 thought categories provide a good taxonomy for our own system
- Social grudges are one of the most powerful stressors -- emergent drama

---

## 3. STRANGE MOODS / TANTRUMS / INSANITY

### 3.1 Strange Moods -- Artifact Creation

**Trigger Conditions:**
- No currently active mood in the fortress
- Maximum artifact limit not reached
- At least 20 eligible dwarves present
- Internal counter set to 1000, decrements every 100 frames
- When counter reaches 0: 1/500 chance per tick (~2.4% daily likelihood)

**Artifact Limit:** Lower of (total items created / 100) or (revealed subterranean tiles / 2304)

**Dwarf Selection -- Weighted Lottery:**
| Weight | Professions |
|--------|-------------|
| 21 tickets | Armorsmith, Bone Carver, Clothier, Craftsdwarf, Jeweler, Glassmaker, Leatherworker, Metalcrafter, Weaponsmith, Weaver, Woodcrafter |
| 11 tickets | Bowyer, Carpenter, Mason, Woodworker |
| 6 tickets | Engraver, Mechanic, Miner, Tanner, all others |

Military dwarves on-duty (except Recruits) cannot enter moods.

**The Five Mood Types:**

| Type | Announcement | Happiness Req | Material Hints | XP Gained | Special |
|------|-------------|---------------|----------------|-----------|---------|
| **Fey** | "taken by a fey mood!" | Any | Clear demands | ~20,000 | Standard artifact |
| **Secretive** | "withdraws from society..." | Any | Cryptic sketches | ~20,000 | Same as Fey but hints are pictures |
| **Possessed** | "has been possessed!" | Any | Cryptic speech | **None** | No skill gain -- unique drawback |
| **Fell** | "looses a roaring laughter!" | Unhappy only | N/A | ~20,000 | **Murders nearest dwarf** for materials |
| **Macabre** | "begins to stalk and brood..." | Unhappy only | Requires bones/skulls | ~20,000 | Needs corpse materials, no murder |

**Material Demands:**
- 1-3 base items + up to 7 decoration items
- First item depends on primary skill (stoneworkers want boulders, woodworkers want logs, etc.)
- Decorations randomly selected from: wood, metal, gems, stone, bones, leather, cloth, glass
- Metalsmith artifacts don't require fuel (unique exception)
- Hints cycle with 2-second timing; 6 seconds = 3 of that material needed

**Success:** Artifact created, very strong happy thought, ~20,000 XP (except Possessed), partial insanity immunity

**Failure (42 days without materials):** Dwarf goes insane (see 3.3). Timer: exactly 50,000 ticks = 41.66 days.

### 3.2 Tantrums and Tantrum Spirals

**Trigger:** Stress exceeds +25,000 AND dwarf has high ANGER_PROPENSITY. (Anxiety-prone dwarves become oblivious instead; depression-prone become depressed.)

**Duration:** Several days per episode. Prone dwarves snap approximately once per month if stress persists.

**Tantrum Behaviors:**
- Cancel all current jobs
- Throw items around
- Start fist fights (can severely injure others)
- Topple and destroy buildings
- Harm pets
- Lose all needs during tantrum

**Cascade Mechanics (Tantrum Spiral):**
1. Tantruming dwarf injures or kills another dwarf
2. Witnesses gain "witnessed death" stress
3. Friends/family of victim gain grief stress
4. Punishment of tantruming dwarf may kill them, causing more grief
5. Newly stressed dwarves may tantrum themselves
6. Cycle repeats, potentially destroying the fortress

**Prevention:**
- Focus on removing negative thoughts, not adding positive ones
- Quality bedrooms (3x3 engraved)
- Large dining halls
- Minimize corpse/miasma/rain exposure
- Containment: lock severely stressed dwarves in rooms with food/drink for months

### 3.3 Insanity (Terminal Mental States)

Four types, all **permanent and fatal** with no cure:

| Type | Trigger Personality | Behavior | Death Cause |
|------|-------------------|----------|-------------|
| **Stark Raving Mad** | High anxiety | Strips naked, runs around babbling | Dehydration/starvation |
| **Melancholy** | High depression | Seeks cliffs/water/magma to die; may kill carried infants | Suicide |
| **Berserk** | High anger | Attacks all creatures indiscriminately; becomes enemy of civilization | Killed by others or starvation |
| **Catatonic** | High stress vulnerability OR low anger+anxiety+depression (all <40) | Stares into space, does nothing | Starvation |

**Insanity Causes:**
- Failed strange mood (materials not provided within 42 days)
- Extreme stress accumulation over time
- Sleep deprivation (6+ months)
- Trapped diplomats/merchants (25+ days)

### Design Takeaways for Ingnomia
- Strange moods are DF's most iconic feature -- artifact creation through obsession
- The fell mood (murdering a colleague for materials) is peak dark humor
- Tantrum spirals are emergent disasters from cascading emotional states
- Insanity type depends on personality -- same cause, different manifestation
- The 42-day timer on moods creates tension and urgency
- Possessed moods giving no XP creates a risk/reward dynamic

---

## 4. COMBAT / ANATOMY / WOUND SYSTEM

### 4.1 Tissue Layer Model

Bodies are composed of layered tissues, from outside in:
1. **Skin** -- Outermost protection; cut/torn = light bleeding, light scarring
2. **Fat** -- Under skin; damage = worse bleeding, severe scarring
3. **Muscle** -- Functional tissue; damage impairs limb use
4. **Bone** -- Structural; fractures = severe pain, immobility
5. **Organs** -- Internal; damage effects vary by organ

### 4.2 Wound Types and Pain

| Wound Type | Pain Multiplier | Description |
|------------|----------------|-------------|
| Bruise | 1x base pain | Blunt damage, internal |
| Partial damage | 2x base pain | Tissue partially compromised |
| Break/Fracture | 3x base pain | Structural integrity lost |

**Pain Values by Tissue:**
- Most tissue: 5 pain per unit
- Bone: 50 pain per unit
- Example: Gut punch = 20 pain + nausea; Broken bone = 15 + (50 x 3) = 165 pain
- **150+ total pain = unconsciousness**
- Roughly: 10 body parts bruised through 3 layers, or 4 body parts broken through 3 layers = collapse

### 4.3 Organ-Specific Effects

| Organ | Damage Effect |
|-------|---------------|
| **Brain** | Structural damage = instant death; bruise = survivable |
| **Heart** | Structural damage = fatal bleeding; bruise = no effect |
| **Lungs** | One damaged = impaired breathing; both = near-fatal |
| **Spine (lower)** | Paralyzes legs |
| **Spine (upper/mid)** | Disables all limbs + organs, causes suffocation |
| **Guts** | Pain, nausea, vomiting, dehydration; may spill externally |
| **Throat** | Structural damage = major artery opened (fatal bleeding) |
| **Eyes** | Vision loss, reduced perception |
| **Pancreas/Liver** | No mechanical effects (flavor only) |

### 4.4 Injury Severity Classification

| Level | Description |
|-------|-------------|
| NONE | No wounds |
| MINOR | Damage without functional consequences (may include heavy bleeding) |
| INHIBITED | Muscular/structural/functional damage, partial loss |
| FUNCTION_LOSS | Important function completely lost, structure intact |
| BROKEN | Total structural integrity or muscular ability lost |
| MISSING | Part completely gone |

### 4.5 Bleeding Mechanics
- Skin cuts/tears: light bleeding, light scarring
- Fat damage: worse bleeding, severe scarring
- Pierced heart/throat: massive fatal hemorrhaging
- Bleeding is the primary cause of combat death

### 4.6 Nerve and Functional Damage
- **Motor nerve damage** to a limb: permanent inability to use that limb
- **Sensory nerve damage**: eliminates pain in affected area
- **Spinal nerve damage**: disconnects ALL nervous function below the damaged point
- Non-spinal nerve damage doesn't benefit from healing rate adjustments

### 4.7 Permanent Injuries and Their Consequences
- **Missing arms**: Cannot haul, equip armor, operate screw pumps
- **One leg lost**: Cannot walk; can use crutches for slow mobility
- **Both legs/lower body paralysis**: No crutch benefit; stationary roles only
- **Blindness**: Cannot see; unafraid of animals; severe work quality reduction
- **Both arms lost**: Cannot perform almost any task

### 4.8 Combat Resolution

**Attack Targeting:** Specific body parts targeted, not generic HP. Accuracy based on "ease and squareness" from positioning. Unconscious/immobilized targets = perfectly accurate hits.

**Three Damage Types:**
| Type | Properties | Material Ranking (best to worst) |
|------|-----------|----------------------------------|
| **Slashing/Edged** | Opens arteries, removes limbs | Adamantine > Steel > Iron > Bronze > Copper > Silver |
| **Piercing** | Lung damage = asphyxiation | (same as edged) |
| **Blunt** | Creates fractures; uses mass | Silver > Copper > Bronze > Iron > Steel > Adamantine |

Note: Blunt weapon effectiveness is *reversed* from edged -- heavy soft metals are better for bludgeoning.

**Defense Mechanics:**
- **Armor**: Deflects based on coverage and material properties (yield/fracture values)
- **Dodging**: Skill-based; character physically moves away
- **Parrying/Blocking**: Weapons and shields deflect strikes

**Skill Impact:** Grand Master weapon user can reliably land Tricky strikes; Novice cannot.

### 4.9 Healing and Scarring
- Healed major injuries remain listed as "Minor" or "Inhibited" with permanent scarring
- Scar types: Jagged, Dent, Straight with severity modifiers (Tiny through Massive)
- Missing limbs, nerve damage, blindness = **never heal naturally**
- Higher recuperation attribute = faster healing

### Design Takeaways for Ingnomia
- Body part targeting instead of HP pool is the key differentiator
- Layered tissue model (skin/fat/muscle/bone/organ) creates realistic wound progression
- Pain as a separate track from health (unconsciousness at 150+) adds tactical depth
- Organ-specific effects (brain = death, spine = paralysis) create memorable moments
- Permanent injuries create long-term consequences and stories
- The blunt vs. edged material reversal is an elegant design detail
- Scar system provides lasting narrative from combat history

---

## 5. MEDICAL SYSTEM

### 5.1 Hospital Setup
Hospitals are designated zones requiring:
- Beds (for patients)
- Tables (for surgery)
- Traction benches (for bone setting)
- Containers (for supply storage)
- A **Chief Medical Dwarf** noble appointment enables fortress-wide health monitoring

### 5.2 Five Medical Specializations

| Role | Function | What They Treat |
|------|----------|-----------------|
| **Diagnostician** | Determines what procedures are needed | Must diagnose before ANY treatment |
| **Surgeon** | Repairs internal organs, excises necrotic tissue | Serious muscle/bone/organ injuries |
| **Suturer** | Stops serious bleeding | Open wounds with active bleeding |
| **Bone Doctor** | Sets fractures | Broken bones; uses thread, cloth, splints, casts, traction benches |
| **Wound Dresser** | Finalizes closed wounds | Bandaging for final healing stage |

### 5.3 Treatment Pipeline

```
Injury -> Transport to Hospital -> Diagnosis -> Appropriate Treatment(s) -> Recovery
```

**Critical Rule:** Diagnosis MUST occur first. Without it, no treatment can begin. A fortress without a diagnostician will have untreated patients.

### 5.4 Required Supplies

| Supply | Use |
|--------|-----|
| Thread | Suturing wounds |
| Cloth | Bandaging, casts |
| Splints | Bone setting |
| Crutches | Mobility for leg injuries |
| Gypsum plaster | Casts for fractures |
| Buckets | Water for cleaning |
| Soap | Infection prevention |

Supplies stored in hospital containers with adjustable quantity limits.

### 5.5 Infection System
- Unwashed wounds can develop infections
- Stagnant water increases infection risk
- Cleaning without soap can cause infection
- Infected dwarves experience constant internal bleeding
- Healthy dwarves may outpace infection through blood regeneration
- A well nearby reduces infection risk

### 5.6 Recovery
- "Recover Wounded" labor allows designated dwarves to transport injured to hospital
- Low-priority task means immobile patients risk starvation
- Dedicated orderlies needed for feeding bedridden patients

### Design Takeaways for Ingnomia
- Diagnosis-first pipeline is elegant -- creates bottleneck on a single critical skill
- Five distinct medical roles encourage specialization
- Supply chain (thread, cloth, soap, splints) connects medical system to crafting/economy
- Infection as a complication adds stakes to wound treatment timing
- The orderly/transport problem creates emergent logistics challenges

---

## 6. SOCIAL RELATIONSHIPS

### 6.1 Relationship Formation

Dwarves form relationships through **idle chatting** when standing on the same or adjacent tiles. Both must be idle (not actively working). Each chat increments a hidden interaction counter.

### 6.2 Relationship Progression

| Interaction Rank | Result |
|-----------------|--------|
| ~15 | Friendship or grudge forms (based on compatibility) |
| ~40 | Compatible friends become lovers (range 31-42 observed) |
| ~50 | Lovers can marry |

### 6.3 Compatibility Factors

**Positive compatibility (friendship):**
- Similar likes (e.g., both like elephants)
- Same skills (e.g., two miners)
- Similar personality facets

**Negative compatibility (grudges):**
- Vastly different personalities
- Divergent values in grudge-contributing facets (see Section 1)

**Romance requirements:**
- Age gap: max(10, min(age1, age2) / 2)
- Not closely related (cousins OK, siblings not)
- Not extremely low LOVE_PROPENSITY, GREGARIOUSNESS, or LUST_PROPENSITY
- Orientation-compatible
- Both adult and unattached

### 6.4 Relationship Types

**Kin:** Spouse, lover, child, parent, grandparent, sibling, aunt/uncle, niece/nephew, cousin

**Non-kin Personal:** Kindred spirit, close friend, friend, long-term acquaintance, passing acquaintance, grudge

**Professional:** Apprentice, master, former master/apprentice

**Spiritual:** Deity, object of worship

**Animal:** Pet, bonded animal

### 6.5 Marriage Mechanics
- Married dwarves share a bed (only one needed)
- Marriage generates very strong positive thoughts
- Spouse death generates strong negative thoughts
- Widowed dwarves may remarry eventually
- If one spouse is expelled, entire family follows
- Similar professions increase marriage likelihood (especially miners)
- Several years of socialization may produce only 1-2 couples

### 6.6 Children
- Opposite-sex married couples: wives may become pregnant from spouse interaction
- Not every interaction results in pregnancy
- ~One baby per year maximum
- Miscarriage possible from starvation, dehydration, or injury

### 6.7 Grudge Effects
- Talking to grudges generates ~700+ stress per day
- Social skills (Conversationalist, Intimidator, etc.) play NO role in relationships
- Grudges are purely personality-driven

### Design Takeaways for Ingnomia
- Simple interaction counter + compatibility check creates emergent social networks
- The grudge mechanic is powerful -- forced proximity with incompatible gnomes causes chronic stress
- Marriage as a progression (friend -> lover -> spouse) feels natural
- Family loyalty (expelled = family follows) creates interesting political dynamics
- The fact that social *skills* don't affect relationships is notable -- it's purely personality

---

## 7. NEEDS AND ROOM QUALITY

### 7.1 The Needs System

Needs replaced DF's older "on break" mechanic. Each dwarf has needs determined by their personality. Unmet needs cause stress and unfocus; met needs provide focus bonuses.

### 7.2 Focus Mechanic

Focus = percentage of met vs. unmet need values. 1:1 ratio = 100% (neutral).

| Focus Level | Threshold | Effect |
|-------------|-----------|--------|
| Very Focused | 140%+ | Up to +50% skill bonus |
| Focused | 120-139% | Moderate skill bonus |
| Neutral | ~100% | Normal performance |
| Distracted | 80-99% | Moderate skill penalty |
| Unfocused | 61-79% | Significant skill penalty |
| Badly Distracted | 60% or below | Up to -50% skill penalty |

### 7.3 Complete Needs List (30 Needs)

| Need | Satisfaction Action | Related Personality |
|------|-------------------|-------------------|
| **Socialization** | Socialize with others | GREGARIOUSNESS |
| **Alcohol** | Drink alcohol | IMMODERATION, SELF_CONTROL |
| **Prayer/Meditation** | Pray/meditate in temple | Religion-dependent |
| **Occupation** | Perform fortress tasks | HARD_WORK, ACTIVITY_LEVEL |
| **Creativity** | Create artistic works | ARTWORK |
| **Excitement** | Dangerous/novel activities | EXCITEMENT_SEEKING |
| **Learning** | Gain skill ranks; read | KNOWLEDGE, CURIOUS |
| **Family** | Socialize with family; have children | FAMILY |
| **Friendship** | Socialize with friends | FRIENDSHIP |
| **Eloquence** | Hear/recite poetry | ELOQUENCE |
| **Tradition** | Deliver justice; perform rituals | TRADITION |
| **Introspection** | Read; compose poetry | INTROSPECTION |
| **Merriment** | Witness performances | MERRIMENT, HUMOR |
| **Craftsmanship** | Craft/improve items | CRAFTSMANSHIP |
| **Martial Arts** | Gain combat skills; spar | MARTIAL_PROWESS |
| **Skill Practice** | Use relevant skills | SKILL |
| **Leisure** | Rest idly | LEISURE_TIME |
| **Romance** | Interact with spouse/partner | ROMANCE, LOVE_PROPENSITY |
| **Nature** | See animals | NATURE |
| **Great Beasts** | Encounter large creatures | EXCITEMENT_SEEKING, NATURE |
| **Acquisition** | Acquire trade goods/trinkets | GREED, COMMERCE |
| **Fine Dining** | Eat preferred/valuable meals | IMMODERATION |
| **Combat** | Engage in fighting | VIOLENT |
| **Trouble-Making** | Fight or argue | DISCORD |
| **Argumentation** | Argue with others | FRIENDLINESS (low) |
| **Extravagance** | Wear quality items; create masterworks | IMMODESTY |
| **Wandering** | Complete hunting/fishing/gathering | NATURE, ACTIVITY_LEVEL |
| **Helping** | Assist patients/prisoners | ALTRUISM, SACRIFICE |
| **Abstract Thinking** | Read/write; compose; design | ABSTRACT_INCLINED |
| **Art Appreciation** | View quality furniture/engravings | ARTWORK |

**Need weights:** 1, 2, 5, or 10 -- higher personality alignment = higher weight = more impact on focus.

**Satisfaction:** When any need is met, its value refreshes to maximum (400) regardless of previous value.

### 7.4 Room Quality System

**Room Value Thresholds:**

| Value | Bedroom | Dining Room | Office | Tomb |
|-------|---------|-------------|--------|------|
| 1 | Meager Quarters | Meager Dining Room | Meager Office | Grave |
| 100 | Modest Quarters | Modest Dining Room | Modest Office | Servant's Burial Chamber |
| 250 | Quarters | Dining Room | Office | Burial Chamber |
| 500 | Decent Quarters | Decent Dining Room | Decent Office | Tomb |
| 1,000 | Fine Quarters | Fine Dining Room | Splendid Office | Fine Tomb |
| 1,500 | Great Bedroom | Great Dining Room | Throne Room | Mausoleum |
| 2,500 | Grand Bedroom | Grand Dining Room | Opulent Throne Room | Grand Mausoleum |
| 10,000 | Royal Bedroom | Royal Dining Room | Royal Throne Room | Royal Mausoleum |

**Value Calculation:**
- Base = total value of floor tiles + wall tiles
- Plus value of all furniture and constructions in the room
- Smoothed/engraved surfaces add value
- Valuable stone (magnetite, obsidian) increases base
- Artifact furniture massively boosts value
- **Overlap penalty: 75% reduction** when room floor areas overlap
- Walls can be shared between rooms without penalty

**Happiness Effects:**
- Owning a bedroom generates happy thoughts when sleeping
- No bedroom generates unhappy thoughts
- Higher quality = stronger positive thoughts
- Nobles require specific quality levels (demanding rooms)

### Design Takeaways for Ingnomia
- 30 personality-driven needs create unique per-gnome requirements
- Focus system (+/-50% skill) makes need satisfaction economically meaningful
- Not all gnomes need the same things -- a warrior doesn't need art appreciation
- Room value as a single number with clear thresholds is clean design
- The overlap penalty discourages mega-rooms and encourages individual spaces
- Noble room demands create tension between satisfying nobles and efficient layout

---

## CROSS-SYSTEM INTERACTIONS (The Magic)

The true genius of DF is how these systems interact:

1. **Personality -> Needs -> Stress -> Tantrum -> Death -> Grief -> More Stress** (the tantrum spiral)
2. **Personality -> Compatibility -> Grudges -> Forced Social Stress -> Breakdown**
3. **Personality -> Strange Mood -> Material Demand -> Success/Failure -> Artifact/Insanity**
4. **Combat -> Injury -> Medical Pipeline -> Recovery/Permanent Disability -> Reduced Capability -> Unhappy Thoughts**
5. **Room Quality -> Thoughts -> Stress Reduction -> Focus -> Productivity**
6. **Relationships -> Spouse Death -> Grief -> Stress -> Tantrum -> More Deaths**

These feedback loops create DF's emergent storytelling. A single death can cascade through the social network, stress system, and need system to bring down an entire fortress -- or a well-designed colony can hum along with happy, focused dwarves creating masterworks.

**For Ingnomia, the priority should be implementing these interaction loops, not just the individual systems.** The individual systems are relatively simple (single numbers, threshold checks, weighted lotteries). The emergent complexity comes from connecting them.
