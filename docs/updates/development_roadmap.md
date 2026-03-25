# Ingnomia Development Roadmap

Built from community feedback (4,693 suggestions), developer discussions (2,567 dev messages), bug reports (6,066 messages, 407 bug threads), version history (60 releases, v0.2.3→v0.8.10), and codebase analysis.

**Total Discord data analyzed: 15,893 messages across 4 channels.**

## Guiding Principles

1. **Fix foundations before adding features** — performance and stability unlock everything else
2. **Each milestone should produce a playable, noticeably improved game**
3. **Leverage what's already built** — needs system, skills, mechanisms, pathfinding parallelism all exist in some form
4. **Community's top 10 most-requested features drive the order**
5. **Systems must talk to each other** — personality drives mood, mood drives work speed, work speed drives production, production drives Kingdom Worth, KW drives threats

**Design reference:** Each milestone's detailed design (competitor analysis, exact mechanics, performance notes) is in `docs/research/feature_reference_library.md`. Research sources are in `docs/research/` (RimWorld, Dwarf Fortress, Gnomoria wikis + community feedback).

---

## Milestone 0: Stabilize & Modernize (Foundation)

*Goal: A clean, buildable, performant base to develop on.*

### 0.1 — Build System & Platform
- [x] Verify clean CMake build on macOS (OpenGL 4.1 shaders ported, ImGui UI migrated)
- [ ] Verify Linux build still works
- [ ] Document build steps for all 3 platforms in a single BUILD.md

### 0.1b — Loading Performance (DONE)
- [x] **98% faster loading** — batch DB queries, parallel tile processing, bulk GPU upload
- [x] Renderer init path optimized (sprite factory, tile data upload)
- Note: This addresses the loading path, not tick-time bottlenecks (those are 0.3/0.4)

### 0.1c — Selection Preview & Escape Key (DONE)
- [x] **Ghost/preview rendering fixed** — selection shader was using wrong program for `tile` uniform (`m_axleShader` instead of `m_selectionShader`). Previews now show for all tools (mine, build, workshop, etc.)
- [x] **Escape key priority chain** — clears active tool first, then side panels, then pause menu (was going straight to pause menu)
- [x] **Connected `signalPropagateKeyEsc`** — was emitted but never wired to `Selection::clear()`. Escape now properly clears selection on game thread

### 0.2 — Critical Bug Fixes
From 6,066 bug report messages (407 bug threads, 310 potentially still open). These are the **recurring crash/stability patterns** that must be fixed first:

| Bug Pattern | Reports | Date Range | Status |
|-------------|---------|------------|--------|
| **Floors disappearing / construction holes** | 7 | 2018–2021 | [~] Investigated — deconstruct logic looks correct, v0.7.0 fixed known cases. Needs gameplay testing |
| **Gnomes get stuck/trapped** | 7 | 2018–2021 | [x] **FIXED** — trapped detection + WARNING log |
| **UI crash on scroll/click in workshops** | 4 | 2018–2020 | [x] **FIXED** — old Noesis UI removed, ImGui handles this |
| **Save corruption / overwrite** | 3 | 2018–2020 | [x] **FIXED** — save folders now use unique `<name>_<timestamp>` IDs |
| **Job cancellation not working** | 3 | 2019–2020 | [x] **FIXED** — cancel now sets aborted flag + cleans up stale sprites |
| **Negative thirst → crash** | 2 | 2019 | [x] **FIXED** — needs clamped, UI progress bars bounded |
| **Rotated workshop bugs** | Recurring | 2018–2021 | [x] **FIXED** — work position offsets now rotated to match workshop rotation |
| **Game speed → lag/freeze** | Recurring | 2019–2020 | [ ] Partially addressed — trapped gnome detection prevents cascading pathfinding |

**Bug report stats**: 68% had dev response, 24% confirmed fixed, 310 potentially still open.
Full details: `docs/bug-reports/bug_report_summary.md`

### 0.3 — Performance: Algorithmic Bottlenecks

Profiling shows the game is timer-bound (1ms work, 49ms sleeping) currently, but has scaling time bombs. These must be fixed BEFORE parallelization.

**Tier 1 — Critical (will hit at 20+ gnomes):**

| # | Issue | Root Cause | Fix | Status |
|---|-------|-----------|-----|--------|
| P1 | JobManager::getJob() | Every idle gnome re-evaluates every job's item availability from scratch. 50 gnomes × 200 jobs = 10k+ octree queries/tick | Dirty-flagged availability cache | [x] **DONE** |
| P2 | Inventory::itemCount() | Linear scan with no count cache, called constantly by workshops and farms | Add count cache, invalidate on item add/remove | [x] **DONE** |

**Tier 2 — High (will hit at 50+ gnomes or large farms):**

| # | Issue | Root Cause | Fix | Status |
|---|-------|-----------|-----|--------|
| P3 | Farm/Grove tick | Ticks every tile every tick — 400-tile farm = 400 octree queries/tick, unthrottled | Throttle to hourly/daily, use active-set pattern like grass | [x] **DONE** — throttled to every 10 ticks |
| P4 | PathFinder thread creation | Creates fresh OS thread per search group — no pool, no cancellation for dead requests | Use thread pool, cancel stale requests | [x] **DONE** — cancelRequest() implemented |
| P5 | CreatureManager time-slicing | Commented-out budget guard means all wild animals tick every frame | Re-enable time budget with proper carry-over | [x] **DONE** — 2ms budget re-enabled |

**Tier 3 — Medium (death by a thousand cuts):**

| # | Issue | Root Cause | Fix | Status |
|---|-------|-----------|-----|--------|
| P6 | WorkshopManager::workshop(id) | O(workshops) linear scan | Replace QList with QHash lookup | [x] **DONE** |
| P7 | StockpileManager::finishJob() | Scans all stockpiles per finished job | Add reverse jobID→stockpileID map | [ ] Deferred |
| P8 | Pasture::onTick() | O(pastures × all_animals) — hidden quadratic | Index animals by pasture, only check assigned | [ ] Deferred |

**Already well-designed:** grass, water, and plant processing use active-set patterns.
**Parallelization (0.4)** comes AFTER fixing these algorithmic issues.

### 0.4 — Performance: Game Loop Parallelization (DONE)
*(See `docs/updates/parallelization_plan.md` for full technical plan)*

- [x] **Creature position spatial index** — O(creatures) → O(1) lookups via existing world position map
- [x] **Phase 1**: Non-blocking pathfinding — dispatch at end of tick, collect at start of next
- [x] **Phase 2**: Parallel natural world — grass/water/plants run concurrently via std::async
- [x] **Phase 3**: Parallel passive systems — rooms/mechanisms/fluids/neighbors/sound alongside event processing
- [x] **Bug fix**: RegionMap checkSplitFlood dimension bug (m_dimX * m_dimX → m_dimX * m_dimY)

**Game loop is now multi-threaded.** Three parallel phases overlap independent work across CPU cores.

### 0.5 — Dead Gnome Cleanup (DONE)
ext3h flagged: "all references to the dead Gnome still left in the world need to be cleaned up. Ownerships, carried items, jobs, etc." Workshop assignments were specifically broken on death.

- [x] Audit all systems that hold gnome references (Jobs, Workshops, Inventory, Military, Rooms)
- [x] `Gnome::die()` now cleans up: current job, workshop assignments, room ownership, drops inventory+equipment
- [x] `GnomeManager` removes from military on death
- [x] Death logged via `LogType::WARNING`

---

## Milestone 1: Core QoL — "Make What Exists Feel Good"

*Goal: Address the most-requested quality-of-life features. No new game systems — polish existing ones.*

**Design reference:** See `docs/research/feature_reference_library.md` §1.1–1.4 for full designs with competitor analysis.

### 1.1 — Event & Notification Log (#11, #19)
The most universally wanted feature. "Find cow dead, want to know why."

- [ ] Create `EventLog` system — structured log of game events (deaths, combat, traps, migration)
- [ ] "Gnome is trapped" detection using existing region IDs (`RegionMap::checkConnectedRegions`)
- [ ] HUD notification popup for critical events (death, trapped, combat)
- [ ] Scrollable event log panel in UI

### 1.2 — Stockpile UX Overhaul (#5, #57, #58, #59)
Biggest source of frustration across all suggestions.

- [ ] Auto-accept new item types when stockpile filter is set to "all" of a category
- [ ] "For trade" stockpile designation — mark stockpiles, filter in trade window
- [ ] Stockpile limit: count items across materials ("max 200 statuettes" regardless of wood type)
- [ ] Copy/paste stockpile settings between stockpiles (clarify UX)
- [ ] Drag-and-drop stockpile priority ordering

### 1.3 — Workshop Production Limits (#48)
`craft-to` mode was added in v0.4.0 ("jobs suspend when number reached") and workshop assignment was added in v0.6.1. But the community still wanted "craft until N **in stockpile**" which is different from "craft N total."

- [ ] Add "craft until N in stockpile" mode — check Inventory count before starting repeat, not just count crafted
- [x] ~~Assign gnomes to workshops~~ — done in v0.6.1

### 1.4 — HUD & UI Improvements (#1, #2, #12, #14)
- [ ] Stock counters on main HUD (food/drink/materials count) — like Gnomoria
- [ ] Battle recap screen after combat (8⭐ — injuries, deaths, equipment summary)
- [ ] Gnome job counter next to name in population list
- [ ] Z-level keyboard shortcut (not just scroll wheel)

---

## Milestone 2: Gnome Depth — "Make Gnomes Feel Alive"

*Goal: Complete the gnome behavior systems that are partially implemented. This is the highest-impact milestone — personality and social systems are the #1 missing feature per community feedback.*

**Design reference:** See `docs/research/feature_reference_library.md` §2.0–2.4 for full designs with competitor analysis.

### 2.0 — Character Traits & Backstories (NEW)
Foundation for all personality-driven systems. Must be implemented BEFORE mood (2.1) since traits modulate mood responses.

- [ ] Add 12-15 trait scales to `Creature` (Bravery, Sociability, Industriousness, Appetite, Temper, Creativity, Greed, Curiosity, Empathy, Stubbornness, Optimism, Nerve)
- [ ] Trait generation at gnome creation (bell curve, extreme values get descriptions)
- [ ] DB table for backstories (childhood + adulthood) with skill/trait modifiers
- [ ] 15-20 childhood + 20-30 adulthood backstories as initial content
- [ ] Backstories reference NeighborManager kingdoms for world connectivity
- [ ] Gnome info panel shows traits + backstory
- [ ] Traits visible as plain-language descriptions (not raw numbers)

### 2.0b — Social System (NEW)
Relationships between gnomes — the engine for emergent stories. Depends on traits (2.0) for compatibility calculations.

- [ ] Opinion score per gnome pair (-100 to +100) stored in `GnomeManager` or new `SocialManager`
- [ ] Social interactions during idle time in shared rooms (Chat +1, Deep Talk +8, Argument -8, Insult -12, Compliment +5)
- [ ] Trait compatibility drives interaction outcomes (similar traits → friendship, divergent → rivalry)
- [ ] Relationship labels at thresholds (Rival, Disliked, Neutral, Friendly, Close Friend)
- [ ] Social mood thoughts: Close Friend nearby +3, Rival nearby -2, Friend died -8, Insulted -3
- [ ] Dining hall as social hub (bonus interaction chances when eating together)

### 2.1 — Happiness/Mood System (#17, #89, #91)
The needs map exists (`creature.h:m_needs`) with Hunger/Thirst/Sleep. Automatons explicitly remove "Happiness" — so the slot exists but is unused. Depends on traits (2.0) and social (2.0b) for full effect.

- [ ] Thought stack system: each thought has value + duration + stack limit (RimWorld model)
- [ ] Mood 0-100 bar calculated from sum of active thoughts + trait-based base mood
- [ ] Traits modulate thought intensity (Compassionate gnome: death = -8; Cold gnome: death = -2)
- [ ] Mood affects work speed ±30% (DF focus mechanic — makes mood economically meaningful)
- [ ] 3-tier mental breaks at 35%/20%/5% thresholds, type determined by personality traits
- [ ] Catharsis +40 after break to prevent cascade spirals
- [ ] Room impressiveness generates mood thoughts (8 tiers)
- [ ] Happiness affects migration rate and kingdom worth
- [ ] "Dead friends" mood penalty (#91 — 3⭐) — now powered by social system
- [ ] Gnome needs panel in UI showing all current thoughts/mood

### 2.2 — Designated Movement Zones (#30)
"No pass designation" was added in v0.4.7 with a checkbox to let gnomes ignore it (v0.4.8). But the community wanted a fuller zone system.

- [x] ~~Basic no-pass zone~~ — done in v0.4.7
- [ ] Zone types: civilian allowed, military only, forbidden (expand beyond simple no-pass)
- [ ] Per-gnome zone permissions (beyond the single "ignore no pass" checkbox)
- [ ] Emergency lockdown button — confine civilians to safe area, military operates freely
- [ ] Emergency override when gnome is trapped

### 2.3 — Medical System Improvements (#31, #37)
Anatomy/wound system exists (`anatomy.cpp`). Hospital/bandage code exists.

- [ ] Medics bring bandages TO injured gnomes (not other way around)
- [ ] 3-tier auto-triage: Critical (bleeding) → Serious (breaks, infection) → Minor (bruises)
- [ ] 2 roles: Medic (diagnosis + treatment) and Caretaker (transport + feeding)
- [ ] Supply chain: bandages (cloth), splints (wood), herbal medicine (farming)
- [ ] 3 starter diseases: Infection (untreated wounds), Plague (seasonal), Food Poisoning (low cooking skill)
- [ ] Wire blindness flags (HALFBLIND/BLIND already in enums but not connected to eye damage)

### 2.4 — Food & Farming QoL (#28, #39, #41, #46)
- [ ] Food exclusion list — named food policies with per-gnome assignment
- [ ] "Preserve Seeds" toggle: prevent kitchens from using last N seeds of any crop
- [ ] Groves include existing trees (fix Gnomoria's #1 grove complaint)
- [ ] Food variety bonus: 3+ different foods in 3 days = +3 mood; same food repeatedly = -2 mood
- [ ] Drink hierarchy: water < milk < beer < wine, better drinks = bigger mood boost
- [ ] Simple meal chain: raw (-mood) → simple cooked (neutral) → quality meal (2+ ingredients, +mood)
- [ ] "Paint" mode for bulk farm/grove/pasture crop changes

---

## Milestone 3: Combat & World — "Make the World Dangerous"

*Goal: Flesh out combat, enemies, and world dynamics.*

**Design reference:** See `docs/research/feature_reference_library.md` §3.1–3.3 for full designs with competitor analysis.

### 3.1 — Combat UI & Feedback (#12, #13)
- [ ] Combat overview screen (gnome stats left, enemies right)
- [ ] Battle recap after fights end
- [ ] Combat notifications in event log

### 3.2 — Enemy Diversity (#33, #34)
- [ ] Different enemy types: ranged, armored, trap-disarming, flying (over walls)
- [ ] Monsters that dig upward if player avoids digging deep
- [ ] Enemy spawn scaling with kingdom worth

### 3.3 — World Dynamics (#67, #68, #70, #71)
- [ ] Difficulty presets for game creation (easy/normal/hard)
- [ ] Improved water physics (fix premature evaporation from Gnomoria)
- [ ] Day/night effects on work speed and enemy behavior

---

## Milestone 4: Automation & Late Game

*Goal: Give players systems to manage a large, established fortress.*

**Design reference:** See `docs/research/feature_reference_library.md` §4.1–4.3 for full designs with competitor analysis.

### 4.1 — Event-Triggered Mechanisms (#93)
Mechanism system exists (`mechanismmanager.cpp`). Controls for mechanisms were added in v0.8.6. Need to extend with event-based triggers.

- [x] ~~Basic mechanism controls~~ — done in v0.8.6
- [ ] Connect mechanisms to game events (raid → close doors, alarm → activate traps)
- [ ] Configurable triggers via UI

### 4.2 — Automaton Progression (#56)
Automatons were added in v0.7.0 with fuel system. Controls added in v0.8.6. Currently single-tier.

- [x] ~~Basic automatons with fuel~~ — done in v0.7.0
- [ ] Tier system: basic (hauling only) → advanced (crafting)
- [ ] Prevent golem cheese (automatons shouldn't fully replace gnomes)

### 4.3 — Magic & Religion (#85, #88) — STRETCH GOAL
Full expansion-level feature. No visual or mechanical foundations exist. Deferred until Milestones 1-3 are solid. Full design documented in `docs/research/feature_reference_library.md` §4.3 for when the time comes.

- [ ] Deity system with domains (earth, war, craft, harvest, death)
- [ ] Temple tiers using room-value thresholds
- [ ] Prayer as personality-driven need
- [ ] Mana from temples + deep underground crystals
- [ ] Spells as crafted items at Scriptorium workshop

---

## Milestone 5: Modding & Community

*Goal: Let the community extend the game.*

**Design reference:** See `docs/research/feature_reference_library.md` §5.1–5.2 for full designs with competitor analysis.

### 5.1 — Modding API (#95)
Currently mods can only override SQLite DB entries.

- [ ] Document current modding capabilities thoroughly
- [ ] Expose behavior trees as loadable XML/JSON (#96 — BehaviorTree.CPP was suggested)
- [ ] Mod manager UI in game
- [ ] Mod load order and conflict detection

### 5.2 — Translation System Overhaul
.roest acknowledged it needs "a complete overhaul." ext3h suggested using Qt linguist tooling or XLIFF.

- [ ] Proper i18n framework (string extraction, XLIFF or similar)
- [ ] Community translation contribution pipeline

---

---

## Bug Report Overview

From the #bug-reports channel (6,066 messages, 2018–2022):

| Category | Total Reports | Key Themes |
|----------|--------------|------------|
| UI / Display | 166 | Crashes on scroll/click, missing info, display glitches |
| Save / Load | 139 | Corruption, overwrite by same name, OneDrive conflicts |
| Building / Construction | 129 | Floors disappearing, rotated buildings broken, scaffolding issues |
| Jobs / Tasks | 95 | Jobs stuck, cancel not working, gnomes ignoring available jobs |
| World / Terrain | 86 | Water behavior, trees blocking, light level issues |
| Stockpile / Storage | 82 | Filter confusion, hauling to deleted stockpiles, item not found |
| Crafting / Workshop | 80 | Rotated workshops, repeat jobs breaking, ingredient detection |
| Crash / Freeze | 72 | Workshop scroll, negative thirst, large map generation |
| Farming / Animals | 71 | Grove issues, animal breeding overflow, harvest timing |
| Pathfinding | 46 | Stuck gnomes, trapped by floor removal, idle despite available jobs |
| Performance | 42 | High speed lag, large stockpile slowdown, enemy spawn spikes |
| Combat / Military | 28 | Squad assignment, equipment not dropped on death |

**Cross-reference with feature requests**: Many bugs and features overlap. For example:
- "Gnomes get stuck" (bug) → "Gnome is trapped notification" (feature #19) → "Designated movement zones" (feature #30)
- "Job cancellation broken" (bug) → "Production limits on workshops" (feature #48)
- "Stockpile filter confusion" (bug) → "Stockpile UX overhaul" (feature #5)
- "Equipment not dropped on death" (bug) → "Dead gnome cleanup" (Milestone 0.5)

Fixing the bugs often naturally leads into implementing the requested features.

Full bug details: `docs/bug-reports/bug_report_summary.md`
Raw data: `docs/bug-reports/bug_reports_raw.json`

---

## What NOT to Build

These were discussed and explicitly rejected or deprioritized by the dev team:

| Suggestion | Why Not |
|-----------|---------|
| Adventurer mode | .roest: "not planned, even if I'd say I do it at some point, that point will be far in the future" |
| Multiple bases | .lld: "double the stuff running, and for what benefit?" |
| Microtransactions | Running joke in Discord |
| Walking on impossible tiles | .roest: "walking on impossible tiles is a bad workaround" |
| MySQL database | Overkill — SQLite is appropriate for a single-player game |

---

## Priority Summary

```
 NOW   Milestone 0: Fix foundations (build, perf, cleanup)
  |    Milestone 1: Core QoL (notifications, stockpiles, workshops, HUD)
  |    Milestone 2: Gnome depth (traits, social, mood, zones, medical, farming)
  |    Milestone 3: Combat & world (enemies, difficulty, world dynamics)
  |    Milestone 4: Late game (automation, automaton tiers)
  v    Milestone 5: Modding & community (API, translations)
LATER  Stretch: Magic & Religion (expansion scope)
```

Each milestone builds on the previous. Milestone 0 is purely technical — it makes everything after it faster and more stable. Milestone 1 produces the most visible player-facing improvements for the least new code. Milestone 2 is the highest-impact gameplay milestone — traits + social + mood transform gnomes from "drones" into characters. Milestone 3 makes the world threatening. Milestones 4-5 are endgame and extensibility. Magic & Religion is deferred as expansion scope.

**Dependency chain within Milestone 2:**
```
2.0 Traits & Backstories (foundation — no dependencies)
  └─→ 2.0b Social System (needs traits for compatibility)
       └─→ 2.1 Mood System (needs traits for modulation + social for thoughts)
2.2 Movement Zones (independent)
2.3 Medical (independent, but mood thoughts integrate with 2.1)
2.4 Food QoL (independent, but food mood thoughts integrate with 2.1)
```
