# Ingnomia Feature Reference Library

Cross-references the [development roadmap](../updates/development_roadmap.md) with design research from RimWorld, Dwarf Fortress, and Gnomoria. Each feature lists what the reference games do, what Ingnomia should adopt, and what would make its implementation stand out.

**Source research**: `docs/research/rimworld/`, `docs/research/dwarf-fortress/`, `docs/research/gnomoria/`
**Performance constraints**: `docs/updates/parallelization_plan.md` (Laws 1-5, scaling audit)

---

## Milestone 0: Stabilize & Modernize

*No research reference — driven by profiling data and codebase audit.*

See `docs/updates/parallelization_plan.md` for the full Phase 0 audit, performance laws, and scaling bottleneck analysis.

---

## Milestone 1: Core QoL — "Make What Exists Feel Good"

### 1.1 — Event & Notification Log

| Game | Approach |
|------|----------|
| **RimWorld** | Thoughts system generates textual feedback for every event with mood value + duration + stack limit. Categorized into Major Threats, Bad Events, Good Events, Incidental. "Trapped underground" thought exists explicitly (-9 mood). Mental break announcements at 35%/20%/5% thresholds. |
| **Dwarf Fortress** | Text-based announcements with flavorful specific phrases per mood type. Combat reports track body-part-specific injuries. Tantrum spirals cascade visibly. Stress thresholds at +10K/+25K/+50K generate announcements. |
| **Gnomoria** | **No feedback on failures** — the #1 community complaint. No indication when orders fail, gnomes are stuck, rooms don't function, or gnomes die from starvation. Civilians charge into combat with no warning. |

**Ingnomia design:**
- 4 severity tiers: Info / Warning / Danger / Critical
- Scrollable event log panel (RimWorld-style) + on-screen toast notifications for critical events
- DF-style flavorful text for mood events and combat ("Urist has been taken by a strange mood!")
- **Fix Gnomoria's fatal flaw**: explicit failure notifications — "Cannot reach destination," "Workshop stalled: missing materials," "Gnome is trapped"
- "Gnome is trapped" detection using existing `RegionMap::checkConnectedRegions`

**Performance note:** Event log is append-only, O(1) per event. No scaling concern.

---

### 1.2 — Stockpile UX Overhaul

| Game | Approach |
|------|----------|
| **RimWorld** | Zone-based stockpiles with priority levels. Bills can target specific stockpiles for output. Colony wealth drives Expectations mood modifier (+30 to +0). |
| **Dwarf Fortress** | Stockpile links to workshops (input/output). Bookkeeper noble tracks inventory accuracy. Wealth thresholds trigger threats. |
| **Gnomoria** | Had filtering system (praised). But: stockpile system caused performance lag, wrong-material bug, no production control, cumbersome material selection. |

**Ingnomia design:**
- Keep Gnomoria's filtering + add RimWorld-style priority levels (Low/Normal/Preferred/Critical)
- **Auto-accept**: new item types default to accepted by relevant stockpiles
- **"For trade" designation**: mark stockpiles whose contents are earmarked for merchants (DF-inspired)
- **Stockpile limits**: max count per item type to prevent overproduction
- **Copy/paste settings** between stockpiles (QoL none of the three had)
- **DF-style stockpile links** to workshops for material flow control, fixing the "wrong material" bug

**Performance note:** `StockpileManager::finishJob()` is currently O(stockpiles) — needs reverse jobID→stockpileID index (see parallelization plan Tier 1).

---

### 1.3 — Workshop Production Limits

| Game | Approach |
|------|----------|
| **RimWorld** | Bill system: "Do X times," "Do until you have X," "Do forever." Bills have ingredient restrictions and target stockpiles. |
| **Dwarf Fortress** | Manager noble automates conditional work orders ("make 10 helmets if we have fewer than 5"). |
| **Gnomoria** | Workshop auto-chaining (universally praised). But: no "craft until N" mode, wrong-material bug, broken priority system, no failure feedback. |

**Ingnomia design:**
- Three bill modes: "Craft X times" / "Craft until N in stockpile" / "Craft forever"
- **Keep Gnomoria's auto-chaining** — cascading orders to dependent workshops
- Material restriction filters per bill (preventing wrong-material bug)
- Priority field per bill (1-4) so urgent orders jump the queue
- Pause/resume on individual bills
- **No Manager noble required** — DF's approach adds friction without fun here

**Performance note:** `Workshop::checkItemsAvailable()` triggers O(ingredients × materials × octree) per craft job check. `Inventory::itemCount()` is an uncached linear scan — needs a count cache (see parallelization plan Tier 1).

---

### 1.4 — HUD & UI Improvements

| Game | Approach |
|------|----------|
| **RimWorld** | Colony wealth visible, raid formula transparent, mood bar with break thresholds, passion indicators on skills, full thought breakdown per pawn. Clean minimal HUD with info on hover/click. |
| **Dwarf Fortress** | Bookkeeper provides stock counts. 14 thought categories. Focus mechanic (+/-50% skill bonus) visible. Detailed combat reports. |
| **Gnomoria** | UI was its weakest point. No tutorial, no tooltips, terrible learning curve, no failure feedback, clunky priority system, over-use of sub-menus. |

**Ingnomia design:**
- **Stock counters**: resource bar showing food/drink/wood/stone/metals/finished goods (RimWorld-style, no Bookkeeper noble required)
- **Battle recap**: post-combat summary — who fought, injuries (DF-level body-part detail, simplified), kills, deaths, equipment damage
- **Job counter**: active/pending/stalled jobs with breakdown by type, stalled jobs shown prominently with reasons
- **Tooltips everywhere** — the single most requested basic UI improvement from Gnomoria feedback
- **Kingdom Worth display**: current KW with breakdown (stockpiles, buildings, Great Hall multiplier)
- Minimal by default, expandable stats panel for detail (RimWorld approach)

---

## Milestone 2: Gnome Depth — "Make Gnomes Feel Alive"

### 2.1 — Happiness/Mood System

| Game | Approach |
|------|----------|
| **RimWorld** | Mood 0-100% bar. Thoughts have value + duration + stack limit. Room impressiveness -4 to +8. Food quality -12 to +12. Mental breaks at 35%/20%/5%. Catharsis +40 after break. Expectations tied to wealth. |
| **Dwarf Fortress** | Three-layer: Thoughts → Emotions (180 defined) → Stress (-1M to +1M). Same event produces different emotions based on personality (50 facets, 34 values). Focus system +/-50% skill bonus from needs satisfaction. Room quality 8 tiers. Long-term memories. Social grudges ~700+ stress/day. Tantrum spirals. |
| **Gnomoria** | Minimal. Food/drink quality affects satisfaction duration. Room quality affects sleep. Morale slowly recovers. No personality, no social relationships, no individual preferences — #1 player complaint. |

**Ingnomia design:**
- **RimWorld thought stacks** (value + duration + stack limit) for accessibility and moddability
- **DF personality modulation**, simplified: 10-15 core traits on a scale that modify which thoughts fire and their intensity (high Sociability = bigger social boosts/penalties)
- **DF focus mechanic**: mood affects work speed +/-30% — makes mood economically meaningful, not just a break timer
- **RimWorld 3-tier mental breaks** (minor/major/extreme) with **DF personality-determined break type** (angry gnomes tantrum, anxious gnomes freeze, depressed gnomes wander)
- **RimWorld catharsis** (+40 after break) to prevent instant cascade spirals
- **Room impressiveness**: DF value-threshold table (8 tiers) generating RimWorld-style mood thoughts
- **Migration**: tied to both Kingdom Worth (Gnomoria) AND colony happiness. Unhappy = fewer gnomads; happy = bonus immigrants

**Performance note:** Mood calculation is O(active_thoughts) per gnome — negligible. The social interaction check needs spatial indexing (see CreatureManager audit).

---

### 2.2 — Designated Movement Zones

| Game | Approach |
|------|----------|
| **RimWorld** | Named painted areas, per-colonist assignment. Simple click-drag. Animal pens with markers. |
| **Dwarf Fortress** | Burrows restrict movement. Civilian alerts retreat to designated burrow. Traffic designations (high/normal/low/restricted) as soft avoidance. |
| **Gnomoria** | Guard areas for military patrols. Pastures for animals. No civilian restriction. Gnomes wander into danger constantly. |

**Ingnomia design:**
- **RimWorld area painting** as base (simple click-drag)
- **Zone types** layered on top:
  - Allowed Area (per-gnome, RimWorld-style)
  - Restricted Area (DF traffic — all gnomes avoid unless no alternative)
  - Military Only / Civilian Only
- **Lockdown button**: instantly confines all civilians to pre-designated safe burrow (DF civilian alert). Military operates freely.
- Per-gnome assignment from gnome info panel (not from the zone — scales better)

---

### 2.3 — Medical System Improvements

| Game | Approach |
|------|----------|
| **RimWorld** | Doctors come TO patients. Treatment quality from skill + medicine type + lighting + cleanliness. Disease: severity/day vs immunity/day race. Surgery system. 11 health capacities. No built-in triage. |
| **Dwarf Fortress** | Five specializations pipeline: Diagnose → Surgery → Suture → Bone Set → Dress Wound. "Recover Wounded" labor for transport. Hospital zones need supplies (thread, cloth, splints, soap). Infection from unwashed wounds. Layered tissue model. |
| **Gnomoria** | Basic: Medic + Caretaker skills, hospital rooms, bandages from Tailor. No disease, no infection, no triage, no specializations. Players wanted medics to seek out injured gnomes. |

**Ingnomia design:**
- **Medics come to patients** (RimWorld) + **"Recover Wounded" hauler job** (DF) for incapacitated gnomes
- **3-tier auto-triage** (neither RimWorld nor DF does this well):
  - Critical (bleeding out) → Serious (breaks, infection) → Minor (bruises)
  - Medics auto-assess priority — strict improvement over both games
- **2 roles** (simplified DF): Medic (diagnosis + treatment) and Caretaker (transport + feeding bedridden)
- **Supply chain**: bandages (cloth), splints (wood), herbal medicine (farming) — connects medical to crafting economy
- **3 starter diseases**: Infection (untreated wounds), Plague (seasonal), Food Poisoning (low cooking skill)
- **RimWorld body-part HP** system, simplified: head, torso, arms ×2, legs ×2, internal organs

---

### 2.4 — Food & Farming QoL

| Game | Approach |
|------|----------|
| **RimWorld** | Named food policies with checkboxes per food type, per-pawn assignment. No seeds as items. Growing zones with single crop. No grove system. |
| **Dwarf Fortress** | Kitchen toggles to prevent cooking specific plants (preserving seeds). Farm plots per season. Individual food preferences. Food variety matters. |
| **Gnomoria** | Farms + Groves. But groves did NOT include existing trees (#1 grove complaint). Seeds separate from food with no protection. No food exclusion. Drink quality hierarchy (water < milk < beer < wine). |

**Ingnomia design:**
- **Food exclusion list** (RimWorld model): named food policies with toggleable categories, assignable per-gnome
- **"Preserve Seeds" toggle**: prevents kitchens/breweries from using the last N seeds of any crop (default: 5) — solves the classic "cooked all my seeds" problem
- **Groves include existing trees**: fix Gnomoria's #1 grove complaint. Trees in grove protected from general "chop" designation.
- **Food variety bonus**: 3+ different foods in 3 days = +3 mood; same food repeatedly = -2 mood (simplified DF variety mechanic)
- **Drink hierarchy** from Gnomoria: water < milk < beer < wine, better drinks = bigger mood boost
- Simple meal chain: raw (-mood) → simple cooked (neutral) → quality meal (2+ ingredients, +mood)

**Performance note:** `Farm::onTick()` calls `getClosestItem()` per jobless tile every tick — needs throttling to `hourChanged` (see parallelization plan Tier 2).

---

## Milestone 3: Combat & World — "Make the World Dangerous"

### 3.1 — Combat UI & Feedback

| Game | Approach |
|------|----------|
| **RimWorld** | Combat log tracks every hit/miss/deflection per pawn with accuracy calculation. Health tab shows wound-by-wound detail. Post-raid summary (stolen, downed, killed). Difficulty multipliers transparent. |
| **Dwarf Fortress** | Extremely detailed text combat logs ("The goblin strikes the dwarf in the upper left arm, tearing the muscle and fracturing the bone"). No consolidated battle recap. Information scattered across individual logs. |
| **Gnomoria** | Very poor combat feedback. No combat log, no battle recap, no injury/death notifications. |

**Ingnomia design:**
- **3-layer combat feedback**: (1) toast notifications per event, (2) scrollable combat log with per-blow DF-level detail in RimWorld-readable format, (3) post-battle recap dialog
- **Combat overview panel**: all squads, targets, health, engagement state
- Color-coded severity: green (enemy killed), yellow (gnome wounded), red (gnome downed/killed)
- Every combat outcome must be visible and explainable — direct fix for Gnomoria's #1 complaint

---

### 3.2 — Enemy Diversity

| Game | Approach |
|------|----------|
| **RimWorld** | Ranged enemies (autopistol → charge lance). Armored mechanoids (72% sharp armor). Sappers mine through walls. Drop pods bypass perimeters. Scaling: `Raid = (Wealth + Pawn Points) × Difficulty × Adaptation`. 7 raid types. Mechanoids immune to fire/toxic/mood. |
| **Dwarf Fortress** | Procedural forgotten beasts (unique body + material + attack per world). Megabeasts (dragon, hydra, roc, bronze colossus). Trap-immune bosses. Necromancer reanimation chains. Siege system with civilization-specific tactics. 3 underground cavern layers. Werebeasts, vampires. Wealth thresholds trigger threat tiers. |
| **Gnomoria** | Goblins scale with KW (equipment tiers). Goblin tunnelers dig in. Mants triggered by food stockpile. Underground: beetles (exponential cocoon growth), skeletons, zombies, golems, spiders. Dark areas spawn enemies (torches prevent). No ranged/flying/trap-disarming enemies. |

**Ingnomia design:**
- **Ranged enemies**: goblin archers scaling with KW, forces beyond melee kill corridors
- **Armored enemies**: equipment tiers (leather → bronze → iron → steel) + natural-armor variants (armored beetles, golems)
- **Trap-disarming enemies**: "goblin sapper" class that detects/disables traps, forcing military engagement
- **Flying enemies**: harpies/giant bats ignore walls, force ranged response or roofed fortifications
- **Upward-digging monsters**: deep creatures dig through floors, forces floor reinforcement
- **Scaling**: `Raid Strength = (KW + Population) × Difficulty × Time Factor`. DF-style thresholds for specific threats (flying > 50K KW, armored squads > 100K). Great Hall double-counting preserved.
- **2-3 procedural bosses per world** (DF forgotten beast model): unique body type + material + special attack, generated at world creation

---

### 3.3 — World Dynamics

| Game | Approach |
|------|----------|
| **RimWorld** | 6 difficulty presets (0.10x–2.20x). Weather events (toxic fallout, volcanic winter, heat wave, cold snap, solar flare). Day/night affects solar/crops/schedules. 15+ biomes. Seasonal temperature shifts. No water physics. |
| **Dwarf Fortress** | Multi-step world generation pipeline. Biomes from elevation + drainage + rainfall + temperature × alignment × savagery (3×3 matrix). 3 underground cavern layers. Full water physics (pressure, freezing, aquifers). Seasonal effects (frozen moats, caravan schedules). History simulation. |
| **Gnomoria** | No difficulty presets (KW-only curve). No water physics (buggy). No day/night effects. No weather. Z-level ore distribution. Darkness mechanic (torches prevent spawning). Players explicitly wanted water physics, lava, and difficulty settings. |

**Ingnomia design:**
- **4-5 difficulty presets** (Peaceful/Normal/Hard/Brutal/Custom) as multiplier tuples affecting raid strength, spawn frequency, equipment tier, immigration, resources. Custom mode exposes individual parameters (DF-style).
- **Pressure-based water** (DF-inspired, 7 levels). Rivers flow across surface. Water freezes in winter (removing moat defenses). Optional aquifers.
- **Day/night**: mood bonus for outdoor daytime, certain enemies stronger at night, visibility range for ranged combat, darkness mechanic preserved
- **Seasonal weather**: temperature affects crops/water/comfort. Events: storms, heat waves, cold snaps. Atmosphere, not domination.
- **Preserve Gnomoria's Z-level ore distribution** + add 2-3 DF-inspired cavern layers with distinct ecosystems

---

## Milestone 4: Automation & Late Game

### 4.1 — Event-Triggered Mechanisms

| Game | Approach |
|------|----------|
| **RimWorld** | No mechanism/engineering system. Turrets auto-fire. Storyteller AI triggers events. No player-programmable logic. |
| **Dwarf Fortress** | Physical triggers only (levers pulled manually by dwarves, pressure plates). No event-based triggers. Diplomats report trap layouts to future siegers. |
| **Gnomoria** | Five-category mechanism system (linkages, switches, devices, power, traps). Tinker Bench research gates. All triggers are physical/spatial — no connection to abstract game events. |

**Ingnomia design:**
- Preserve Gnomoria's physical mechanism system + Tinker Bench research
- Add **Alarm Bell** device: wired to mechanisms, activated by game events (raid detected, gnome death, stockpile threshold)
- Add **Condition Plate**: non-physical trigger evaluating game state per tick ("food < 50", "enemies > 0", "nighttime")
- **Mechanism chains** with 1-tick-per-link signal propagation
- Gate event-triggers behind late-game Tinker Bench research

**Industry-defining opportunity:** A **visual logic system** — craftable AND/OR/NOT/DELAY gates connecting events to mechanisms. "IF (raid AND night) THEN (close gates, activate flood, ring alarm) AFTER 30s." Each gate is a physical object gnomes craft and place. Factorio's circuit network meets DF's lever system. Entirely novel in the genre.

---

### 4.2 — Automaton Progression

| Game | Approach |
|------|----------|
| **RimWorld (Biotech)** | Mechanitor system: bandwidth-gated mechs from 1-bandwidth haulers to 5-bandwidth war machines. Labor mechs: Agrihand, Constructoid, Fabricor, Lifter, Paramedic, Tunneler. Pollution cost. No mood/food needs. |
| **Dwarf Fortress** | No player-built automatons. Bronze Colossus as reference for indestructible metal construct. Token-based creature definition system enables composable entity definitions. |
| **Gnomoria** | Automatons craftable at Engineering Shop. "Golem cheese" was a community complaint. Tinker Bench gates access. No tier system. |

**Ingnomia design:**
- **Tier 1 — Clockwork** (copper/bronze): 1 labor type, slow, fragile, requires wind-up. Mid-game.
- **Tier 2 — Steam** (iron/steel + coal): 2-3 labor types or light combat, self-powered, needs maintenance. Late-mid-game.
- **Tier 3 — Arcane** (mana-infused + rare materials): any labor or capable combat, requires mana upkeep from temples. Ties to magic system (4.3).
- **Anti-cheese**: max 1 per 10 gnomes, degrades over time, vulnerable to a specific damage type, never exceeds skilled gnome output

**Industry-defining opportunity:** **Socially integrated automatons.** Gnomes have opinions about them — some find them helpful (+mood), others unsettling ("replaced by machine" stress). Tinkerer gnomes bond with their creations, name them, grieve their destruction. Transforms automatons from optimization mechanic into emergent narrative.

---

### 4.3 — Magic & Religion

| Game | Approach |
|------|----------|
| **RimWorld (Ideology)** | Belief memes + precepts. Rituals provide mood + development points. Roles (Leader, Moral Guide). Affects mood, work restrictions, faction relations. No magic (sci-fi setting). Royalty DLC: psycasts gated behind royal titles. |
| **Dwarf Fortress** | Temples as need-satisfying locations. Shrine/Temple/Temple Complex tiers. Prayer/meditation as personality-driven need. Necromancers gain powers from death-sphere deities. Vampires cursed by deities. No player-controlled magic — magic is NPC/world-driven. |
| **Gnomoria** | No religion. Community requested alchemy/magic/potions. Developer had magic planned but never implemented. Players wanted "underground incentives — unique items, magical resources." |

**Ingnomia design:**

*Religion layer:*
- Gnomes arrive with deity affiliations (generated at world creation). Multiple deities with domains (earth, war, craft, harvest, death).
- Temple tier system (Shrine → Temple → Grand Temple) using room-value thresholds (DF model)
- Prayer satisfies personality-driven need (high religiousness = needs prayer; secular gnomes do not)
- **High Priest** role enables divine favor accumulation — colony-wide resource from prayer, rituals, temple quality, deity mandates

*Magic layer:*
- Mana generated by temples and deep-underground mana crystals (incentivizes deep mining)
- Spells as **crafted items** (scrolls/enchantments) at a Scriptorium workshop, not direct-cast
- Spell categories tied to deity domains: Earth (wall reinforcement), War (combat buffs), Craft (quality boost), Harvest (crop acceleration), Death (raise skeleton defenders — disturbing to non-death-cult gnomes)
- Divine favor as gating currency: casting consumes favor, creating tension between offensive magic and temple upkeep

**Industry-defining opportunity:** **Magic emergent from social simulation.**
1. Mana scales with *genuinely faithful* gnomes — faith fluctuates based on whether prayers are "answered" (colony prospers = faith rises; disaster despite devotion = faith erodes). Colony in crisis loses magic precisely when needed most.
2. **Theological schisms**: conflicting deity precepts create internal religious conflict → gnome grudges → stress cascades. Player must choose unity (authoritarian, efficient) or pluralism (risky, but access to multiple spell domains).
3. **Moral cost**: powerful spells (raising undead) cause negative thoughts in non-aligned gnomes. Heavy death magic wins battles but triggers mood spirals. Makes magic a genuine strategic tradeoff.

---

## Milestone 5: Modding & Community

### 5.1 — Modding API

| Game | Approach |
|------|----------|
| **RimWorld** | XML Defs for all content + C# Harmony patching. Steam Workshop (20,000+ mods). Mod manager, load order, conflict detection, XML inheritance, PatchOperation system. 10+ year relevance driven by mods. |
| **Dwarf Fortress** | Token-based raw text files for creatures, materials, items, reactions. Enormously powerful but cryptic, undocumented, no load-order management. Behavior hard-coded. |
| **Gnomoria** | Minimal. ~3 mods on Nexus. "Feels like an afterthought" vs RimWorld's "designed for modding." Restricted to difficulty tweaks and cosmetics. |

**Ingnomia design:**
- **Data layer** (XML/JSON): all content definitions loadable from external files with schema validation. Support inheritance/patching (RimWorld PatchOperation model).
- **Behavior layer** (XML behavior trees): expose existing BT system as AI modding interface. Library of built-in nodes (MoveTo, PickUp, UseWorkshop, Attack, Flee) that modders compose.
- **Mod manager**: in-game browser, load order, conflict detection, enable/disable without restart, per-mod settings panel.

**Industry-defining opportunity:** A **visual behavior tree editor** shipped as a first-party tool. Drag-and-drop nodes to define AI behaviors, craft chains, event responses. Outputs standard XML. Lowers modding barrier from "must write code" to "must understand flowcharts." Enables "AI personality packs" as a new mod category. Doubles as a debugging tool. Add **hot-reloading** of XML during gameplay for rapid iteration — no competitor supports this.

---

### 5.2 — Translation System Overhaul

| Game | Approach |
|------|----------|
| **RimWorld** | Externalized XML strings by language folder. Named parameter interpolation. Gender-aware grammar. 20+ community translations via Workshop. |
| **Dwarf Fortress** | Community-driven text files. Procedural names use constructed languages. Translation notoriously difficult due to embedded text and dynamic sentence generation. |
| **Gnomoria** | English-only. No translation system. |

**Ingnomia design:**
- Externalize all strings to keyed JSON/XML files
- Named parameter interpolation (`"{gnome_name} finished crafting {item_name}"`) for word-order flexibility
- CLDR plural rules for grammatical gender/pluralization
- Community translation packs as a mod type (using 5.1 mod manager)
- SQLite string table mapping internal IDs to localized display names

**Industry-defining opportunity:** **In-game translation editor** — bilingual players enable "translator mode," hover any string to see its ID, click to edit. Submissions go to community review pipeline. Converts players into translators without external tools. Plus **grammar-aware procedural text** templates with case/gender/number slots, producing natural text in any language.

---

## Design Philosophy: What Makes Ingnomia "Industry-Defining"

The research reveals a clear pattern across all three reference games:

| Game | Strength | Weakness |
|------|----------|----------|
| **RimWorld** | Accessibility, UI clarity, moddability | Systems are broad but shallow. Mood is a single bar. Combat is simple. No Z-levels. |
| **Dwarf Fortress** | Unmatched depth and emergent narrative | Impenetrable UI, undocumented systems, no visual feedback, performance issues |
| **Gnomoria** | Correct genre (3D colony sim with Z-levels), solid crafting/mechanisms | Abandoned, poor UI, no personality/social systems, minimal feedback, performance collapse |

**Ingnomia's positioning**: DF's depth with RimWorld's accessibility, in Gnomoria's 3D format.

The consistent differentiators across all milestones are:

1. **Systems that talk to each other.** Religion affects mood affects work speed affects production affects Kingdom Worth affects enemy scaling affects combat affects mood. No system should be an island.
2. **Failure is visible and explainable.** Every complaint about Gnomoria's UI reduces to "I couldn't tell what was happening." Every system must surface its state.
3. **Personality creates narrative.** DF proved that personality-driven emergent stories are the genre's real product. RimWorld proved they can be accessible. Ingnomia should make every gnome's personality visible in how they react to the same events differently.
4. **Modding is a multiplier.** RimWorld's 10-year relevance comes from mods. Ship the visual BT editor and the XML content system, and the community builds the long tail.
