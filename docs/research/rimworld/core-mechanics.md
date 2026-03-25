# RimWorld Game Mechanics Research - Design Reference for Ingnomia

This document contains thorough research from the RimWorld wiki on seven core game systems, intended as a reference for building equivalent systems in Ingnomia.

---

## 1. CHARACTER TRAITS

Traits are permanent personality modifiers assigned at character creation. Each pawn gets 0-3 traits. Traits fall into two categories: **standalone traits** (independent) and **spectrum traits** (mutually exclusive tiers on a scale).

### Spectrum Traits

Each spectrum has 2-4 tiers. A pawn can only have one trait from each spectrum.

#### Drug Desire Spectrum
| Trait | Effects | Description |
|-------|---------|-------------|
| Chemical Fascination | Strong drug need; mood penalties without drugs (-4/-8/-12); mood bonuses with (+3/+6); binges more likely | "utterly fascinated with chemical sources of enjoyment" |
| Chemical Interest | Moderate drug need; mood effects (-2/-4/-6 without, +1/+3 with) | "unusual interest in chemical sources of enjoyment" |
| Teetotaler | Cannot use recreational drugs; refuses drug policies | "abhors the idea of gaining pleasure from chemicals" |

#### Industriousness Spectrum
| Trait | Effect |
|-------|--------|
| Industrious | +35% global work speed |
| Hard Worker | +20% global work speed |
| Lazy | -20% global work speed |
| Slothful | -35% global work speed |

#### Speed Spectrum
| Trait | Effect |
|-------|--------|
| Jogger | +0.4 c/s movement speed |
| Fast Walker | +0.2 c/s movement speed |
| Slowpoke | -0.2 c/s movement speed |

#### Base Mood Spectrum
| Trait | Permanent Mood |
|-------|---------------|
| Sanguine | +12 |
| Optimist | +6 |
| Pessimist | -6 |
| Depressive | -12 |

#### Nerves Spectrum
| Trait | Mental Break Threshold Modifier |
|-------|-------------------------------|
| Iron-Willed | -18% |
| Steadfast | -9% |
| Nervous | +8% |
| Volatile | +15% |

#### Neurotic Spectrum
| Trait | Work Speed | Mental Break Threshold |
|-------|-----------|----------------------|
| Neurotic | +20% | +8% |
| Very Neurotic | +40% | +14% |

#### Shooting Accuracy Spectrum
| Trait | Effect |
|-------|--------|
| Careful Shooter | +25% aiming time, +5 accuracy |
| Trigger-Happy | -50% aiming time, -5 accuracy |

#### Beauty Spectrum
| Trait | Beauty Stat | Opinion from Others |
|-------|------------|-------------------|
| Beautiful | +2 | +40 |
| Pretty | +1 | +20 |
| Ugly | -1 | -20 |
| Staggeringly Ugly | -2 | -40 |

#### Psychic Sensitivity Spectrum
| Trait | Modifier |
|-------|---------|
| Psychically Hypersensitive | +80% |
| Psychically Sensitive | +40% |
| Psychically Dull | -50% |
| Psychically Deaf | -100% |

#### Immunity Spectrum
| Trait | Effect |
|-------|--------|
| Super-Immune | +30% immunity gain speed |
| Sickly | Catches random diseases more often |

### Standalone Traits

#### Lifestyle
| Trait | Effects |
|-------|---------|
| Night Owl | +16 mood at night, -10 mood during daytime; negates darkness penalty |
| Undergrounder | +3/+4 mood indoors/underground, -3 outdoors; negates outdoors need |
| Nudist | +20 mood when naked, -3 per clothing item worn |
| Masochist | Pain gives mood bonuses instead of penalties (+5/+10/+15/+20) |
| Body Modder | Mood bonuses for artificial parts/xenogenes |
| Body Purist | Mood penalties for artificial enhancements (-10 to -35 scaling) |
| Gourmand | +4 cooking skill, +50% hunger rate, food binge-prone |
| Ascetic | Prefers simple bedrooms (+5 mood for awful room); unaffected by meal quality; -5 mood for impressive bedroom |
| Greedy | Requires impressive bedroom, -8 mood without one |
| Jealous | -8 mood if not having the most impressive bedroom in the colony |
| Pyromaniac | +2 with incendiary weapons; cannot be assigned to firefighting; random fire-starting sprees every ~50 days |

#### Social/Personality
| Trait | Effects |
|-------|---------|
| Bloodlust | Ignores corpse/death mood penalties; +8-12 mood from witnessing deaths |
| Cannibal | +15/+20 mood from eating human flesh (cooked/raw) |
| Psychopath | Negates all social/empathy penalties; prevents animal bonding |
| Kind | Can perform "kind words" interaction (+5 opinion, +5 mood); never insults |
| Abrasive | 2.3x more likely to insult others |
| Annoying Voice | -25 opinion from others |
| Creepy Breathing | -25 opinion from others |
| Misandrist | -25 opinion of men |
| Misogynist | -25 opinion of women |

#### Sexuality
| Trait | Effect |
|-------|--------|
| Asexual | Cannot form romantic relationships |
| Gay | Romantic relationships only with same gender |
| Bisexual | Romantic relationships with any gender |

#### Combat
| Trait | Effects |
|-------|---------|
| Nimble | +15 melee dodge chance; x10% trap spring chance |
| Brawler | +4 melee skill, -10 shooting; -10 mood if equipped with ranged weapon |
| Tough | x50% incoming damage |
| Wimp | Pain shock threshold at 30% pain |
| Delicate | x115% incoming damage |

#### Mental/Learning
| Trait | Effects |
|-------|---------|
| Too Smart | +75% learning speed; +12% mental break threshold |
| Fast Learner | +75% global learning factor |
| Slow Learner | -75% global learning factor |
| Quick Sleeper | +50% rest rate |
| Great Memory | x50% skill loss rate |
| Tortured Artist | -8 permanent mood; 50% chance of inspiration after mental breaks |

#### Anomaly DLC (Creepjoiner-exclusive)
| Trait | Effects |
|-------|---------|
| Perfect Memory | Skills never decay |
| Occultist | +100% study efficiency, +50% entity study rate |
| Joyous | +20 opinion from others, +3 mood to nearby pawns |
| Body Mastery | Food, sleep, and comfort needs completely disabled |
| Disturbing | Only strange chats; -2 mood to nearby pawns |
| Void Fascination | +6 mood with captured entities; entity-liberation mental breaks |

---

## 2. MOOD / THOUGHTS SYSTEM

### Core Mechanics

Mood is a 0-100% bar. The current mood moves toward a "Mood Target" (sum of all active thoughts + base mood).

**Movement rates:**
- Mood increases at max +12 per in-game hour
- Mood decreases at max -8 per in-game hour
- Mood changes pause during sleep/unconsciousness

### Base Mood by Difficulty
| Difficulty | Base Mood |
|-----------|-----------|
| Peaceful | 42 |
| Community Builder | 42 |
| Adventure Story | 37 |
| Strive to Survive | 32 |
| Blood and Dust | 27 |
| Losing is Fun | 22 |

### Mental Break Thresholds
| Threshold | Mood Level | Break Severity |
|-----------|-----------|---------------|
| Minor | 35% | Sad wander, food binge, insulting spree |
| Major | 20% | Tantrum, daze, corpse obsession |
| Extreme | 5% | Berserk, catatonic, murderous rage |

These thresholds are modified by traits (e.g., Iron-Willed -18%, Volatile +15%).

### High Mood Effects
- High mood can trigger **inspirations** (temporary creative/combat bonuses)
- Prisoners with high mood are easier to recruit and less likely to escape

### Complete Thought Categories and Values

#### Death Thoughts
| Thought | Mood | Duration | Stacks |
|---------|------|----------|--------|
| Justified execution | -2 | 6 days | 5 |
| Someone was euthanized | -3 | 6 days | 5 |
| Someone was executed | -5 | 6 days | 5 |
| Someone organ-harvested | -6 | 6 days | 5 |
| Colonist died | -3 | 6 days | 5 |
| Witnessed ally's death | -5 | 1.5 days | 5 |
| Witnessed family member's death | -7 | 1 day | 1 |
| My son/daughter died | -20 | 30 days | 10 |
| My husband/wife died | -20 | 30 days | 10 |
| My lover died | -16 | 30 days | 10 |
| My brother/sister died | -14 | 30 days | 10 |
| Bonded animal died | -8 | 20 days | 5 |
| My friend died | -10 | 20 days | 5 |
| My rival died | +10 | 10 days | 5 |

#### Eating Thoughts
| Thought | Mood | In-Game Text |
|---------|------|-------------|
| Ate lavish meal | +12 | "nourished my body and my soul" |
| Ate fine meal | +5 | "really tasty" |
| Ate awful meal | -4 | "disgusting, tasteless meal" |
| Ate raw food | -7 | |
| Ate kibble | -12 | "like I don't get to be human" |
| Ate corpse | -12 | "horrible" |
| Raw cannibalism (non-cannibal) | -20 | "like an animal... nightmare" |
| Raw cannibalism (cannibal) | +20 | "so... succulent" |
| Cooked cannibalism (non-cannibal) | -15 | "horrible" |
| Cooked cannibalism (cannibal) | +15 | "So pleasurable" |
| Ate insect meat | -6 | "most disgusting thing" |
| Ate rotten food | -10 | "disgusting, putrid" |
| Ate without table | -3 | |

#### Drug Thoughts
| Thought | Mood |
|---------|------|
| Inebriated (Warm) | +8 |
| Inebriated (Tipsy) | +12 |
| Inebriated (Drunk) | +18 |
| Inebriated (Hammered) | +24 |
| Hungover (Pounding) | -10 |
| Hungover (Strong) | -5 |
| Hungover (Slight) | -2 |
| High on smokeleaf | +13 |
| High on flake | +35 |
| High on yayo | +35 |
| High on go-juice | +5 |

#### Social Thoughts
| Thought | Mood | Duration | Stacks |
|---------|------|----------|--------|
| Insulted | -5 | 2 days | 10 |
| Kind words | +5 | 2 days | 10 |
| Rebuffed by someone | -5 | 3 days | 5 |
| Had to rebuff someone | -3 | 3 days | 5 |
| Broken up with | -15 | 25 days | 2 |
| Cheated on | -20 | 25 days | 2 |
| Divorced | -20 | 30 days | 2 |
| Proposal rejected | -18 | 25 days | 5 |
| Got some lovin' | +8 | 3 days | 10 |
| Loved one sold | -10 | 120 days | 10 |
| Forced to take drugs | -20 | 6 days | 10 |

#### Gathering/Event Thoughts
| Thought | Mood | Duration | Stacks |
|---------|------|----------|--------|
| Got married | +40 | 30 days | 1 |
| Attended wedding | +20 | 10 days | 10 |
| Attended party | +8 | 10 days | 10 |
| Attended concert | +8 | 10 days | 10 |
| Terrible speech | -12 | 8 days | 10 |
| Uninspiring speech | -4 | 8 days | 10 |
| Encouraging speech | +4 | 8 days | 10 |
| Inspirational speech | +8 | 8 days | 10 |

#### Need-Based Thoughts

**Hunger:**
| Level | Mood |
|-------|------|
| Hungry | -6 |
| Ravenously hungry | -12 |
| Malnourished | -20 |
| Badly malnourished | -26 |
| Starving | -32 |
| Advanced starvation | -38 |
| Starving (extreme) | -44 |

**Exhaustion:**
| Level | Mood |
|-------|------|
| Drowsy | -6 |
| Tired | -12 |
| Exhausted | -18 |

**Recreation:**
| Level | Mood |
|-------|------|
| Recreation-starved | -20 |
| Recreation-deprived | -10 |
| Recreation unfulfilled | -5 |
| Recreation satisfied | +5 |
| Recreation fully satisfied | +10 |

**Comfort:**
| Level | Mood |
|-------|------|
| Uncomfortable | -3 |
| Comfortable | +4 |
| Very comfortable | +6 |
| Extremely comfortable | +8 |
| Luxuriantly comfortable | +10 |

**Beauty/Environment:**
| Level | Mood |
|-------|------|
| Hideous environment | -15 |
| Ugly environment | -10 |
| Unsightly environment | -5 |
| Pretty environment | +5 |
| Beautiful environment | +10 |
| Gorgeous environment | +15 |

**Space:**
| Level | Mood |
|-------|------|
| Confined interior | -10 |
| Cramped interior | -5 |
| Spacious interior | +5 |
| Entombed underground | -9 |
| Trapped underground | -7 |
| Cabin fever | -5 |
| Trapped indoors | -3 |
| Stuck indoors | -1 |

#### Temperature Thoughts
| Thought | Mood | Condition |
|---------|------|-----------|
| Chilly | -4 | 1-10C below comfort |
| Cold | -8 | 11-20C below comfort |
| Numbing cold | -12 | 21+C below comfort |
| Absolutely freezing | -16 | Extreme cold |
| Sweaty | -4 | 1-10C above comfort |
| Hot | -8 | 11-20C above comfort |
| Sweltering hot | -12 | 21+C above comfort |
| Blistering hot | -16 | Extreme heat |

#### Room Impressiveness Thoughts
Rooms generate thoughts based on their "impressiveness" stat. Pattern for private rooms (bedroom):
| Level | Mood |
|-------|------|
| Awful | -4 |
| Decent | +2 |
| Slightly impressive | +3 |
| Somewhat impressive | +4 |
| Very impressive | +5 |
| Extremely impressive | +6 |
| Unbelievably impressive | +7 |
| Wondrously impressive | +8 |

Shared rooms (barracks) have lower values (-7 to +4). Dining rooms and rec rooms follow a +2 to +8 scale.

#### Pain Thoughts (without Masochist)
| Level | Mood |
|-------|------|
| Minor pain | -5 |
| Serious pain | -10 |
| Intense pain | -15 |
| Mind-shattering pain | -20 |

#### Pain Thoughts (with Masochist)
| Level | Mood |
|-------|------|
| Little pain | +5 |
| Moderate pain | +10 |
| Acute pain | +15 |
| Exceptional pain | +20 |

#### Clothing Thoughts
| Thought | Mood |
|---------|------|
| Ratty apparel | -3 |
| Tattered apparel | -5 |
| Wearing wrong gender apparel | -5 |
| Tainted apparel (1 piece) | -5 |
| Tainted apparel (2 pieces) | -8 |
| Tainted apparel (3 pieces) | -11 |
| Tainted apparel (4+ pieces) | -14 |
| Naked (non-nudist) | -6 |
| Naked (nudist) | +20 |

#### Miscellaneous
| Thought | Mood | Duration |
|---------|------|----------|
| Initial optimism | +10 | 8 days |
| Initial hope | +5 | 8 days |
| Catharsis (after mental break) | +40 | 3 days |
| Nuzzled by animal | +4 | 1 day |
| Slept outside | -4 | 1 day |
| Slept on ground | -4 | 1 day |
| Disturbed sleep | -1 | 1 day |
| My organ harvested | -30 | 8 days |
| Was imprisoned | -10 | 6 days |
| Rescued | +18 | 30 days |
| Freed from slaver | +15 | 20 days |
| Beautiful aurora | +14 | |
| I butchered humanlike | -6 | 6 days |
| Observed corpse | -4 | 0.5 days |
| Observed rotting corpse | -6 | 0.5 days |
| Soaking wet | -3 | 0.1 days |
| Darkness | -5 | |
| Sick | -5 | |
| Minor passion for work | +8 | |
| Burning passion for work | +14 | |

#### Expectations System
Colony wealth determines an "expectations" level that modifies mood:
| Expectation Level | Mood |
|-------------------|------|
| Extremely low | +30 |
| Very low | +24 |
| Low | +18 |
| Moderate | +12 |
| High | +6 |
| Sky-high | 0 |

---

## 3. MENTAL BREAKS

### Minor Breaks (Below 35% mood, mean time 10 days)

| Break | Duration | What Happens |
|-------|----------|-------------|
| Food Binge | 4-18 hours | Pawn consumes available food regardless of restrictions. Requires 10+ nutrition in stockpile. |
| Sad Wander | 16-24 hours | Pawn wanders aimlessly, doing nothing. |
| Hide in Room | 16-24 hours | Pawn locks themselves in their bedroom. |
| Insulting Spree | 10-18 hours | Pawn insults nearby colonists randomly. Requires 2+ other colonists. |
| Targeted Insulting Spree | 10-18 hours | Pawn follows and repeatedly insults one specific colonist (up to -33 mood penalty on target). |
| Dark Visions (Anomaly) | 12-18 hours | Pawn wanders spouting nonsense every 20-60 seconds. Grants +30 void catharsis on recovery. |

### Major Breaks (Below 20% mood, mean time 3 days)

| Break | Duration | What Happens |
|-------|----------|-------------|
| Social Drug Binge | 16-24 hours | Pawn binges on social drugs (beer, ambrosia, smokeleaf, psychite tea). |
| Psychotic Wandering (Daze) | 28-36 hours | Pawn wanders aimlessly, ignores food restrictions. Prisoners can stage prison breaks. |
| Tantrum | 3.2-4.8 hours | Pawn punches colony structures randomly, possibly destroying them. |
| Targeted Tantrum | 16-24 hours | Pawn focuses destructive rage on a single structure or item stack. Dangerous if targeting explosives. |
| Bedroom Tantrum | 16-24 hours | Pawn hides in bedroom and punches structures within. |
| Sadistic Rage | 12-30 hours | Pawn periodically melee attacks prisoners. |
| Corpse Obsession | 20-28 hours | Pawn digs up a corpse and drops it in a high-traffic area. |
| Insane Ramblings (Anomaly) | 16-24 hours | Pawn wanders shouting nonsense every 10-15 seconds. |

### Extreme Breaks (Below 5% mood, mean time 0.7 days)

| Break | Duration | What Happens |
|-------|----------|-------------|
| Berserk | 16-24 hours | Pawn attacks all nearby living creatures indiscriminately. Only stopped by being subdued. |
| Fire Starting Spree | 2.4-4 hours | Pyromaniac-only; sets flammable materials ablaze. Also triggers randomly every ~50 days regardless of mood. |
| Hard Drug Binge | 16-24 hours | Pawn binges on hard drugs (flake, yayo, go-juice, wake-up). Risk of overdose and addiction. |
| Catatonic Breakdown | 40h - 5 days | Pawn becomes completely incapacitated and immobile. Least dangerous but longest duration. |
| Jailbreaker | 24 hours | Pawn moves toward a prisoner and triggers a prison break. |
| Slaughterer | 3.2-4.8 hours | Pawn slaughters tamed animals at random. |
| Murderous Rage | 40 hours | Pawn selects a random colonist/prisoner and attacks until death. |
| Run Wild | Indefinite | Pawn acts like an animal. Lasts until tamed by another colonist (requires Animals skill 7+). |
| Given Up and Leaving | Until escape | Pawn leaves the colony entirely. Only appears when colony has 8+ colonists. |

### Special Breaks (Non-mood-triggered)

| Break | Trigger | What Happens |
|-------|---------|-------------|
| Confusion | Alzheimer's, dementia, anesthesia | Pawn wanders confused for 1.4-4.8 hours. |
| Social Fighting | Insults/slights between pawns | Brief fistfight (7 seconds to minutes). Small chance of permanent injury. |
| Fleeing in Panic | 50% of non-colonist group killed | Pawns flee the map. |
| Manhunter | Events, failed taming, bonded master death | Animal attacks all humans/mechanoids. |

### Key Mechanics
- **Catharsis:** Most breaks grant +40 catharsis mood for 2.5 days afterward, preventing cascading breakdowns
- **Interruption:** Breaks can be stopped by arrest, melee incapacitation, or psycasts
- **Recovery:** Most breaks end via time passage, arrest, being downed, or sleeping

---

## 4. EVENTS / INCIDENTS

### Major Threats

| Event | Details |
|-------|---------|
| **Enemy Raid** | Raiders assault colony. Multiple arrival methods (walk in, drop pods, tunneling, siege). Scales with colony wealth. |
| **Infestation** | Insectoid hives spawn underground. Requires overhead mountain roof, nearby structures, temp > -17C. |
| **Manhunter Pack** | Scaria-infested animals hunt colonists. 40% more raid points than normal. Lasts 24-54 hours. |
| **Psychic Wave** | Drives local wildlife insane. |
| **Crashed Ship Part (Defoliator)** | Kills plants in expanding circle up to 100 tiles. Must be destroyed. |
| **Crashed Ship Part (Psychic)** | Projects psychic drone affecting one gender's mood. Must be destroyed. |
| **Mech Cluster** (Royalty) | Complex defensive structures with walls, turrets, power cells. Spawns dormant until triggered. |

### Extreme Weather

| Event | Duration | Cooldown | Effects |
|-------|----------|----------|---------|
| Flashstorm | 0.075-0.1 days | 15 days | Lightning strikes every 5-13 seconds in 45-60 tile radius |
| Toxic Fallout | 2.5-10.5 days | 90 days | Toxic buildup outdoors; kills unroofed plants |
| Volcanic Winter | 7.5-40 days | 140 days | -7C temperature, -30% light, halves wildlife |
| Eclipse | 0.75-1.25 days | - | Solar generators stop; crops stop growing outdoors |
| Heat Wave | 1.5-3.5 days | 30 days | +17C temperature; fire risk |
| Cold Snap | 1.5-3.5 days | 30 days | -20C temperature; hypothermia risk |
| Solar Flare | 0.15-0.5 days | - | All electrical devices disabled |

### Bad Events

| Event | Details |
|-------|---------|
| **Beavers** | Tree-eating alphabeavers arrive. 30-day cooldown. |
| **Blight** | Plant disease spreading within 3 tiles. Affects crops < 15-day growth. |
| **Mad Animal** | Single animal becomes manhunter, attacks nearest human. |
| **Psychic Drone** | Negative mood (-12 to -40) for one gender. 0.75-1.75 days, 15-day cooldown. |
| **Short Circuit (Zzztt)** | Power conduit fault; fire or explosion centered on batteries. 8-day cooldown. |

### Good Events

| Event | Details |
|-------|---------|
| **Ambrosia Sprout** | Mood-boosting fruit grove appears. Forest/shrubland biomes only. |
| **Animals Join** | Tamed animals arrive from off-map. |
| **Animal Self-Tamed** | Random wild animal becomes tame. |
| **Aurora** | 0.125-0.35 days; +14 mood boost to outdoor pawns. |
| **Cargo Pods** | Resources fall from sky. |
| **Herd Migration** | Large animal groups pass through. |
| **Party** | Stacking +0.8 mood (max +8). Requires party spot. |
| **Psychic Soothe** | 1.5-3.0 days; +16 mood to one gender. 15-day cooldown. |
| **Wanderer Joins** | Random pawn tries joining. 24-hour acceptance window. |
| **Wild Person** | Unaffiliated human wanders in; requires Animals skill 7+ to tame. |
| **Transport Pod Crash** | Downed occupant arrives; can join colony or require rescue. |
| **Trader (Orbital)** | Trade ship in orbit. Requires powered comms console. |
| **Trader (Land)** | Faction caravan visits for trade. |
| **Visitor(s)** | Friendly faction members stop by. |

### Incidental Events

**Social:** New Lovers, Breakup, Rejected Proposal, Marriage, Affair, Divorce (all affect mood)

**Colony:** Man in Black (60-day cooldown - a lone rescuer arrives when colony is nearly wiped), Prison Break, Roof Collapse

**Faction:** Allied faction improves relations (when < -75), Enemy becomes neutral, Hostile faction (worsens relations when > 75), Leader Died, New Leader

### World Map Events

| Event | Details |
|-------|---------|
| Bandit Camp | Quest: destroy ruffians for 2000-3000 silver + 8 goodwill |
| Caravan Ambushed | Enemy/manhunter attacks traveling caravan |
| Caravan Meeting | Random trader encounter |
| Demand | Hostile faction demands items/slaves; refusal triggers assault |
| Peace Talks | Negotiate: outcomes range +100 to -50 relations. 15-day expiry. |

---

## 5. SOCIAL SYSTEM

### Relationship Types

**Family Relations (permanent base opinions):**
| Relation | Base Opinion | Killed Penalty |
|----------|-------------|---------------|
| Parent/Child | +30 | -80 |
| Sibling | +20 | -80 |
| Grandparent/child | +15 | -15 |
| Uncle/Aunt, Cousin | +10 | -15 |
| Half-sibling | +15 | -15 |

**Romantic Relations:**
| Relation | Base Opinion | Notes |
|----------|-------------|-------|
| Lover | +35 | Active romantic relationship |
| Fiance/Fiancee | +30 | Engaged |
| Spouse | +30 | Married |
| Ex-Lover | -15 | Additional -50 for jilted party |
| Ex-Spouse | -15 | Additional -70 for jilted party |

**Non-Intimate Relations (opinion-based thresholds):**
| Relation | Opinion Range |
|----------|--------------|
| Rival | -100 to -20 |
| Acquaintance | -20 to +20 |
| Friend | +20 to +100 |

### Social Interactions

| Interaction | Opinion Change | Stacks | Notes |
|-------------|---------------|--------|-------|
| Chitchat | +0.66 | 10 max | Most common; nullified by Psychopath trait |
| Deep talk | +15 | 10 max | Meaningful conversation; nullified by Psychopath |
| Insulted | -15 | 10 max | Triggers -5 mood; can start social fights |
| Slighted | -5 | 10 max | Small chance to trigger social fights |
| Kind words | +5 | 10 max | Kind trait only |

### Opinion Modifiers (Actions)

| Modifier | Opinion | Stacks | Decay |
|----------|---------|--------|-------|
| Botched surgery | -20 | 5 | x0.9 |
| Rescued me | +15 | 3 | x0.9 |
| Recruited me | +20 | 1 | - |
| Crashed together | +25 | 1 | - |
| Sold loved one | -10 | 2 | - |
| Sold bonded animal | -10 | 2 | - |
| Killed a child | -20 | 5 | x0.9 |
| Harmed me | -15 | 1 | 10 days |
| Cathartic fight | +38 | 5 | 20 days |
| Angering fight | -22 | 5 | 20 days |

### Romance Modifiers

| Modifier | Opinion | Stacks |
|----------|---------|--------|
| Rebuffed me | -10 | 5 |
| Failed to romance me | -15 | 5 |
| Broke up with me | -50 | 1 |
| Affair | -70 | 1 |
| Divorced me | -70 | 1 |
| Rejected my proposal | -30 | 1 |
| Honeymoon phase | +40 | 1 |
| Got some lovin' | +10 | 3 |

### Beauty System
Each level of beauty above or below 0 changes opinion by +/-20, capped at +/-40.

### Romance Mechanics

**Requirements:**
- Target must have positive opinion of romancer
- Not closely related
- Compatible sexual orientation
- Available every 12 in-game days for pawns age 16+

**Success Factors:**
1. Target's opinion of romancer
2. Relative age factor (heavily weighted for young pawns 16-17)
3. Beauty multipliers: Ugly = x30%, Pretty = x230%
4. Existing relationship status

**Age Difference Penalties:**
- Applied when male is 10+ years older or female is 3+ years older
- Penalty accumulates quadratically

### Lovin' Mechanics
- Duration: 6 minutes to 1 hour 6 minutes (250-2750 ticks)
- Requires lovers/fiances/spouses sharing a double sleeping spot
- Provides +8 mood thought ("Got some lovin'"), stacks 3x
- Frequency affected by: pain, consciousness, age, beauty, opinion of partner

### Marriage Progression
Lovers -> Proposal -> Fiances (+30 opinion) -> Marriage ceremony at Marriage spot -> Spouses (+30 opinion)

### Social Fights
- Triggered by insults/slights
- Duration: minimum 7 seconds until one is downed
- Both get minor injuries; small chance of permanent injury
- No weapons used (fists only)
- 50/50 chance of cathartic (+38 opinion, 20 days) or angering (-22 opinion, 20 days) outcome

---

## 6. MEDICAL / HEALTH SYSTEM

### Health Capacities (11 systems)

| Capacity | Body Parts | Fatal if Lost | Notes |
|----------|-----------|--------------|-------|
| Consciousness | Brain | Yes | Below 30% = unconscious; 0% = death |
| Sight | Eyes (2) | No | -25% per eye lost |
| Hearing | Ears (2) | No | Reduced by damage/loss |
| Moving | Legs, Feet, Spine | No | Below 15% = incapacitated |
| Manipulation | Arms, Hands, Spine | No | Each arm = 50%; each finger ~8% |
| Talking | Jaw, Neck | No | Speech capacity |
| Breathing | Lungs (2) | Yes | Fatal if both lost |
| Blood Filtration | Kidneys (2), Liver | Yes | Fatal if all lost |
| Blood Pumping | Heart | Yes | Fatal if lost |
| Digestion | Stomach | Yes | Fatal if lost |
| Eating | Jaw | No | Reduced by jaw damage |

### Human Body Part Hierarchy

**HEAD (25 HP)**
- Skull (25 HP) -> Brain (10 HP) - always scars
- Eyes (10 HP each, x2)
- Ears (12 HP each, x2)
- Nose (10 HP) - cosmetic only
- Jaw (20 HP) -> Tongue (10 HP)

**TORSO (40 HP)**
- Neck (25 HP) - lethal if destroyed
- Spine (25 HP)
- Ribcage (30 HP), Sternum (20 HP)
- Pelvis (25 HP)
- Stomach (20 HP)
- Liver (20 HP) - lethal if lost
- Heart (15 HP) - lethal if lost
- Lungs (15 HP each, x2) - lethal if both lost
- Kidneys (15 HP each, x2) - lethal if both lost
- Shoulders (30 HP each, x2) -> Clavicle (25 HP) -> Arm (30 HP) -> Humerus (25 HP), Radius (20 HP) -> Hand (20 HP) -> 5 fingers (8 HP each)

**LEGS (30 HP each, x2)**
- Femur (25 HP), Tibia (25 HP)
- Foot (25 HP) -> 5 toes (8 HP each, -4% Moving per toe)

### Pain System

| Pain Level | Threshold | Mood Effect (Normal) | Mood Effect (Masochist) |
|------------|-----------|---------------------|------------------------|
| Minor | 1% | -5 | +5 |
| Serious | 15% | -10 | +10 |
| Intense | 40% | -15 | +15 |
| Mind-shattering | 80% | -20 | +20 |

**Consciousness formula:** Pain above 10% reduces consciousness by approximately `(Pain - 0.1) / 2.25`

**Pain reduction:**
- Painstopper implant: x0%
- Go-juice: x10%
- Yayo: x50%
- Beer (drunk): x50%
- Luciferium: x80%
- Smokeleaf: -20% offset

### Healing
Base rate: 8 damage per day. Treatment quality affects scarring risk and infection likelihood.

### Diseases

#### Fatal Diseases
| Disease | Severity/day | Immunity/day | Treatment/day | Time to Kill (untreated) |
|---------|-------------|-------------|--------------|------------------------|
| Infection | +0.84 | +0.644 | -0.53 | ~1.25 days |
| Flu | +0.249 | +0.239 | -0.077 | Slow |
| Malaria | +0.370 | +0.314 | -0.232 | Moderate |
| Plague | +0.666 | +0.522 | -0.362 | ~1.5 days |
| Sleeping Sickness | +0.12 | +0.11 | -0.07 | Very slow |
| Lung Rot | +0.300 | N/A | -1.000 | 6-8 days to resolve |
| Scaria | Fatal in 5 days | N/A | Surgery (Medicine + Skill 8) | 5 days |
| Organ Decay | +0.017-0.033 | N/A | Organ replacement only | 2-40 days |

#### Non-Fatal Diseases
| Disease | Effects | Treatment |
|---------|---------|-----------|
| Gut Worms | 2x hunger rate, +20% pain | Cumulative medicine (300% total tend quality) |
| Muscle Parasites | -30% manipulation/movement, tiredness | Cumulative medicine (300% total tend quality) |
| Fibrous Mechanites | +50% manipulation/moving/blood pumping, but painful/tiring | Self-resolving (15-30 days) |
| Sensory Mechanites | +50% sight/hearing/talking, +30% manipulation | Self-resolving (15-30 days) |

#### Chronic/Age-Related Conditions
| Condition | Min Age | Effects | Treatment |
|-----------|---------|---------|-----------|
| Alzheimer's | 33.6 | Mental confusion | Healer serum, Luciferium |
| Asthma | 16 | Reduced breathing | Lung transplant, Biosculpter |
| Bad Back | 40 | Reduced moving/manipulation | Bionic spine, Biosculpter |
| Carcinoma | 50 | Cancer; fatal if untreated | Bionics, Healer serum |
| Cataracts | 40 | Reduced sight | Bionic eye, Biosculpter |
| Dementia | 48 | Reduced consciousness | Healer serum, Luciferium |
| Frail | 50 | Reduced everything | Healer serum, Biosculpter |
| Artery Blockage | 68 | Reduced blood pumping | Heart transplant, Biosculpter |
| Hearing Loss | 40 | Reduced hearing | Bionic ear, Biosculpter |

### Artificial Body Parts

#### By Tech Level

**Improvised (Wood):**
- Wooden hand: 60% efficiency
- Peg leg: 60% efficiency
- Wooden foot: 80% efficiency

**Prosthetic (Steel + Components):**
- Prosthetic arm: 50% efficiency
- Prosthetic leg: 85% efficiency
- Prosthetic heart: 80% efficiency
- Denture: 80% efficiency
- Cochlear implant: 65% efficiency

**Bionic (Plasteel + Advanced Components):**
- Bionic arm: 125% efficiency
- Bionic leg: 125% efficiency
- Bionic eye: 125% efficiency
- Bionic ear: 125% efficiency
- Bionic heart: 125% efficiency, prevents heart attacks
- Bionic jaw: 125% efficiency
- Bionic stomach: 125% efficiency, x50% food poisoning
- Bionic spine: 100% efficiency, prevents bad back

**Archotech (Ultra-rare, cannot be crafted):**
- Archotech arm: 150% efficiency
- Archotech leg: 150% efficiency
- Archotech eye: 150% efficiency

#### Brain Implants
| Implant | Effects |
|---------|---------|
| Joywire | +30 mood, -20% consciousness (non-removable) |
| Painstopper | x0% pain |
| Circadian assistant | -20% sleep fall rate |
| Circadian half-cycler | Never need sleep, -15% consciousness |
| Learning assistant | +20% global learning factor |
| Neurocalculator | +20% research speed |
| Psychic harmonizer | Mood affects nearby pawns (+/- mood aura) |
| Psychic reader | +50% negotiation, x110% trade price |

#### Specialty Implants
| Implant | Body Part | Effects |
|---------|-----------|---------|
| Drill arm | Hand | +70% deep drilling, +160% mining speed, -8% moving |
| Field hand | Hand | +160% plant work speed, -8% moving |
| Power claw | Hand | 22 damage scratch attack, -8% moving |
| Elbow blade | Arm | 18 damage cut attack |
| Hand/Venom talon | Hand | 15/11 damage scratch/toxic attack |
| Knee spike | Leg | 22 damage stab attack |
| Toughskin gland | Torso | +10% blunt/heat, +35% sharp armor |
| Armorskin gland | Torso | +20% blunt/heat, +52% sharp armor, -1 beauty |
| Stoneskin gland | Torso | +30% blunt/heat, +70% sharp armor, -2 beauty |
| Love enhancer | Torso | Lovin' mood bonus from +8 to +12 |
| Coagulator | Torso | x50% bleed rate |
| Healing enhancer | Torso | x150% natural healing |
| Death acidifier | Torso | Destroys gear on death (non-removable) |

### Consciousness and Death

**Fatal conditions (capacity reaches 0%):** Consciousness, Breathing, Blood Filtration, Blood Pumping, Digestion

**Consciousness formula:**
`E(C) = E(CS) x (1 - Clamp((Pain - 0.1) x 4/9, 0, 0.4)) x (1 - 0.2 x dE(BP)) x (1 - 0.2 x dE(Br)) x (1 - 0.1 x dE(BF))`

Where CS = brain capacity, BP = blood pumping, Br = breathing, BF = blood filtration.

Below 30% consciousness = unconscious. 0% = death.

---

## 7. SKILLS SYSTEM

### The 12 Core Skills

| Skill | What It Affects | Key Thresholds |
|-------|----------------|---------------|
| **Shooting** | Ranged weapon accuracy (calculated per tile distance) | Accuracy improves per level |
| **Melee** | Hit chance and dodge probability in melee combat | Higher = better chance to hit/dodge |
| **Construction** | Building speed, repair speed, roofing, flooring | +15% speed per level |
| **Mining** | Extraction speed and resource yield percentage | Higher yield at higher levels |
| **Cooking** | Meal preparation and butchering quality | Skill 8 for all recipes; 9 eliminates food poisoning |
| **Plants** | Sowing, harvesting, tree cutting, foraging in caravans | Higher = faster + better yield |
| **Animals** | Taming chance, training chance, hunting stealth, animal gathering | Skill 7+ needed to tame wild people |
| **Crafting** | Weapons, armor, component quality | Higher = better quality output |
| **Artistic** | Sculpture creation quality | Higher = more beautiful/valuable art |
| **Medical** | Surgery success rate, treatment quality, operation speed | Critical for surgery outcomes |
| **Social** | Negotiation, recruitment, trade prices, faction relations | Affects trade pricing |
| **Intellectual** | Research speed, hacking, drug synthesis, entity study | Faster research at higher levels |

### Skill Levels
- Range: 0 to 20
- Level 0: "Barely heard of it"
- Level 20: "Legendary Master"
- Higher levels require exponentially more XP
- "Skill training is more effective the lower the skill level"

### Passion System
| Passion | XP Multiplier | Mood Bonus (when doing passioned work) |
|---------|--------------|---------------------------------------|
| None | 35% | 0 |
| Interested (1 flame) | 100% | +8 |
| Burning (2 flames) | 150% | +14 |

### Experience Gain Formula
```
Real XP = Global Learning Factor x Passion Multiplier x Base XP
```

Global Learning Factor modified by traits:
- Fast Learner / Too Smart: +75%
- Slow Learner: -75%

### Skill Decay
- Below level 10: No decay
- Above level 10: Continuous XP drain, accelerating with higher levels
- A pawn must fall 1000 XP below zero before actually losing a level (prevents rapid fluctuation)
- Great Memory trait: x50% skill loss rate
- Perfect Memory trait (Anomaly): Skills never decay

### Work Type Restrictions
Some traits prevent pawns from doing certain work types entirely:
- Pyromaniac: Cannot do firefighting
- Psychopath: Some social work restricted
- Various backstories disable specific skill categories entirely (shown as "incapable of" in character info)

---

## Summary: Design Priorities for Ingnomia

Based on this research, the systems that would add the most depth to Ingnomia's colony simulation with the least implementation complexity:

1. **Traits** - Relatively simple stat modifiers that create unique characters. Start with spectrum traits (mood, work speed, movement) and a few standouts (Pyromaniac, Psychopath, Bloodlust).

2. **Mood/Thoughts** - A running tally of positive/negative modifiers. The key insight: thoughts have a value, duration, and stack limit. Start with needs-based thoughts (hunger, rest, comfort) and room impressiveness.

3. **Mental Breaks** - Three tiers triggered at mood thresholds. Even implementing just Sad Wander, Tantrum, and Berserk would add significant gameplay tension.

4. **Skills with Passion** - The passion system (+8/+14 mood for doing work you love) elegantly ties skills to mood. Skill decay above level 10 prevents easy maxing.

5. **Social System** - Opinion-based with simple accumulation from interactions. The +0.66 chitchat / -15 insult interaction system is lightweight but creates emergent relationships.

6. **Events** - Categorized into threats, good, and bad. Scale with colony wealth. Cooldown timers prevent spam.

7. **Health** - Body part hierarchy with HP values and capacity connections. The pain-to-consciousness formula creates interesting medical gameplay.
