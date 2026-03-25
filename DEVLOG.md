# Ingnomia Development Log

Every change to the codebase must be logged here. This is the master record of all development — it becomes the release changelog.

**Format**: Newest entries at the top. Use the template below for each entry.

---

## [2026-03-25] Milestone 2.0 — Character Traits & Backstories

**Milestone**: 2.0 — Gnome Depth
**Files changed**: `content/db/ingnomia.db.sql`, `src/game/creature.h`, `src/game/creature.cpp`, `src/game/gnomefactory.h`, `src/game/gnomefactory.cpp`, `src/gui/aggregatorpopulation.h`, `src/gui/aggregatorpopulation.cpp`, `src/gui/ui/ui_sidepanels.cpp`

### Changes

- **12 personality traits** added on a -50 to +50 scale: Bravery, Sociability, Industriousness, Appetite, Temper, Creativity, Greed, Curiosity, Empathy, Stubbornness, Optimism, Nerve. Generated with a bell curve centered on 0 — most gnomes are average in most traits, distinctive in 2-4.
- **40 backstories** (15 childhood + 25 adulthood): each gnome gets a random childhood + adulthood pair. Backstories provide skill modifiers (±1-3 levels) and trait biases that nudge personality. All DB-defined for moddability.
- **Personality tab** in the Population panel: shows backstory (Youth/Before labels with tooltip for full text) and notable traits (|value| > 25) as colored progress bars with tooltips.
- **Backward-compatible**: old saves load without crash — gnomes simply have empty traits/backstories.

### Technical Details
- Traits stored as `QVariantMap m_traits` in `Creature` class, serialized/deserialized alongside existing attributes/skills
- `GnomeFactory::createGnome()` now calls `generateTraits()` and `assignBackstory()` after skill assignment
- Backstory skill modifiers and trait biases use pipe-delimited format in DB (`"Mining:2|Masonry:1"`) parsed at gnome creation
- Trait descriptions only appear for extreme values (|value| > 25), fetched from DB `Traits` table via `DB::selectRow()`
- This is data-only — traits don't affect gameplay yet. Mood (2.1) and Social (2.0b) will wire them up.

---

## [2026-03-25] Milestone 1.1 — Event & Notification Log

**Milestone**: 1.1 — Event & Notification Log
**Files changed**: `src/base/logger.h`, `src/base/logger.cpp`, `src/gui/imguibridge.h`, `src/gui/ui/ui_gamehud.cpp`, `src/gui/ui/ui_sidepanels.h`, `src/gui/ui/ui_sidepanels.cpp`, `src/gui/mainwindow.cpp`, `src/game/eventmanager.cpp`, `src/game/gnome.cpp`

### Changes
- **Extended LogType enum** — Added INFO, MIGRATION, DEATH, DANGER types alongside existing DEBUG/JOB/CRAFT/COMBAT/WARNING
- **Log timestamps** — Each log message now includes formatted game time (Year/Season/Day/Hour:Minute)
- **Log cap at 1000 entries** — Prevents unbounded memory growth
- **Toast notification system** — Critical events (WARNING, DANGER, COMBAT, DEATH, MIGRATION) appear as fading overlays in the top-right corner, color-coded by severity, auto-dismiss after ~5 game-minutes
- **Event Log side panel** — New "Log" button in toolbar opens a full-screen scrollable event log with filter checkboxes (Info, Warning, Combat, Death, Jobs, Debug). Entries color-coded: red=death, orange=danger, yellow=warning, pink=combat, green=migration, blue=info, grey=jobs
- **Migration event logging** — EventManager now logs migration/trader events to the event log
- **Death type logging** — Gnome deaths use LogType::DEATH for distinct color and filtering

### Technical Details
- Toast generation checks `Global::logger().messages().size()` against `bridge.lastLogCount` to detect new entries
- Event log renders newest-first with `ImGui::BeginChild` scroll region
- SidePanel::EventLog added to enum and dispatched in `mainwindow.cpp` switch

---

## [2026-03-25] Milestone 0.4 — Game Loop Parallelization

**Milestone**: 0.4 — Parallelization
**Files changed**: `src/game/game.cpp`, `src/base/pathfinder.h`, `src/base/pathfinder.cpp`, `src/game/creaturemanager.cpp`, `src/game/gnomemanager.cpp`, `src/base/regionmap.cpp`

### Changes

**Creature position spatial index:**
- `creaturesAtPosition()`, `animalsAtPosition()`, `monstersAtPosition()` changed from O(creatures) linear scan to O(1) lookup via existing `World::m_creaturePositions` index. The index was already maintained on every creature move but **never used** by the lookup functions.
- `gnomesAtPosition()` similarly refactored to use the world position index + `m_gnomesByID` hash instead of scanning all gnome lists.

**Phase 1 — Non-blocking pathfinding:**
- Split `findPaths()` into `dispatchPaths()` (launches async workers, non-blocking) and `collectPaths()` (waits for results).
- Game loop now collects previous tick's path results at the START of each tick, and dispatches new requests at the END. Path workers run in parallel with the next tick's game logic.
- Added `m_activeTasks` vector to PathFinder to track outstanding futures between ticks.

**Phase 2 — Parallel natural world:**
- `processGrass()`, `processWater()`, and `processPlants()` now run concurrently via `std::async`. They touch independent tile fields (vegetation vs fluid vs plant map).
- Water moved from end-of-tick to parallel block at start-of-tick.

**Phase 3 — Parallel passive systems:**
- Rooms, mechanisms, fluids, neighbors, and sound now run in a background `std::async` task while the main thread handles events and item history.
- Main thread pipeline: creatures → gnomes → jobs → stockpiles → farming → workshops → events
- Background pipeline: rooms → mechanisms → fluids → neighbors → sound

**Bug fix:**
- Fixed `RegionMap::checkSplitFlood()` — allocated `m_dimX * m_dimX` instead of `m_dimX * m_dimY` for visited array. Would cause out-of-bounds on non-square maps.

### Technical Details
- PathFinder: `dispatchPaths()` at line 109, `collectPaths()` at line 168, `findPaths()` preserved as legacy wrapper
- Game loop: `collectPaths` → parallel(grass, water, plants) → creatures → gnomes → jobs → stockpiles → farming → workshops → parallel(rooms, mechanisms, fluids, neighbors, sound) + events → `dispatchPaths`
- RegionMap fix: `regionmap.cpp:615` — `m_dimX * m_dimX` → `m_dimX * m_dimY`

---

## [2026-03-24] Milestone 0.3 — Performance: Algorithmic Bottlenecks

**Milestone**: 0.3 — Performance Bottlenecks
**Files changed**: `src/game/inventory.h`, `src/game/inventory.cpp`, `src/game/job.h`, `src/game/jobmanager.cpp`, `src/game/creaturemanager.cpp`, `src/game/farm.cpp`, `src/game/grove.cpp`, `src/base/pathfinder.cpp`, `src/game/workshopmanager.h`, `src/game/workshopmanager.cpp`

### Changes

**Tier 1 — Critical (20+ gnome scaling):**
- **P1: JobManager availability cache** — Job item availability is now cached per inventory generation. Previously 50 gnomes × 200 jobs = 10,000+ octree queries/tick. Now rechecks only when inventory actually changes.
- **P2: Inventory::itemCount() cache** — Added generation-based cache for item counts. Cache is invalidated on any item state change (create, destroy, pickup, putdown, setInJob, setConstructed, setUsedBy). Repeated calls with same (itemSID, materialSID) return cached result in O(1).

**Tier 2 — High (50+ gnome scaling):**
- **P3: Farm/Grove tick throttle** — Farms and groves now only check fields every 10 ticks instead of every tick. A 400-tile farm goes from 400 octree queries/tick to 40/tick.
- **P4: PathFinder cancel stale requests** — Implemented `cancelRequest()` which was a TODO stub. Pending path requests for dead/removed creatures can now be cancelled instead of wasting thread time.
- **P5: CreatureManager time-slicing** — Re-enabled the commented-out time budget (2ms). Wild animals now time-slice across ticks instead of all ticking every frame. `m_startIndex` properly carries over between ticks.

**Tier 3 — Medium (done: P6, deferred: P7-P8):**
- **P6: WorkshopManager hash lookup** — `workshop(id)` changed from O(workshops) linear scan to O(1) hash lookup via `m_workshopsByID`. Hash maintained on add/remove.
- **P7: StockpileManager reverse job map** — Deferred (more invasive, less critical at current scale)
- **P8: Pasture animal indexing** — Deferred (more invasive, less critical at current scale)

### Technical Details
- Inventory generation counter: `m_itemCountGeneration` incremented by `invalidateItemCounts()`, called from: `addObject`, `destroyObject`, `pickUpItem`, `putDownItem`, `setInJob`, `setConstructed`, `setIsUsedBy`
- Job cache: `m_availabilityCacheGeneration` + `m_cachedAvailability` per Job, compared against `Inventory::itemCountGeneration()`
- Farm/grove throttle: `tick % 10 != 0` guard at top of `onTick()`
- CreatureManager: uncommented `if (m_startIndex >= m_creatures.size())` reset and `if (timer.elapsed() > 2) break` budget

---

## [2026-03-24] Milestone 0.2 — Critical Bug Fixes (Batch 2)

**Milestone**: 0.2 — Critical Bug Fixes
**Files changed**: `src/base/io.cpp`, `src/base/gamestate.h`, `src/base/gamestate.cpp`, `src/game/worldgenerator.cpp`, `src/game/jobmanager.cpp`

### Changes
- **Fixed save corruption with same kingdom name** — Save folders now use a unique identifier (`<KingdomName>_<timestamp>`) instead of just the kingdom name. Two games named "The Life Kingdom" no longer share the same save folder. Legacy saves without `saveFolderName` fall back to the old behavior for backward compatibility.
- **Fixed rotated workshop crafting** — Work position offsets from the DB were not being rotated to match the workshop's rotation. A workshop rotated 90° had its gnome work positions calculated as if unrotated, causing gnomes to pathfind to the wrong tile and fail to craft. Now applies the same rotation transform to work offsets as is applied to component tiles and I/O positions.
- **Investigated disappearing floors** — Audited all `FT_NOFLOOR` assignments in `world.cpp` and `worldconstructions.cpp`. The deconstruct logic correctly gates floor removal behind `isFloor` flag. Previous fixes in v0.7.0 addressed known cases. Marked as likely-fixed pending gameplay testing.

### Technical Details
- Save path: `GameState::saveFolderName` stored in save file, generated via `QDateTime::currentSecsSinceEpoch()` during world generation
- Rotated workshops: `workPositionWalkable()` in `jobmanager.cpp` now applies rotation cases 1-3 to offset before computing `testPos`
- Floor investigation: `worldconstructions.cpp:1192` correctly uses `isFloor` guard; wall deconstruct at 1208 doesn't touch floor type

---

## [2026-03-24] Milestone 0.2 — Critical Bug Fixes (Batch 1)

**Milestone**: 0.2 — Critical Bug Fixes
**Files changed**: `src/game/gnome.cpp`, `src/game/gnome.h`, `src/game/gnomeactions.cpp`, `src/gui/ui/ui_sidepanels.cpp`, `src/game/jobmanager.cpp`, `src/base/logger.h`

### Changes
- **Fixed negative thirst/hunger crash** — Needs values now clamped between -100 and 150 in `evalNeeds()`. Previously needs could decrease unboundedly, causing UI crashes when displaying negative values. Progress bars in gnome info panel now also clamped to 0-1 range via `qBound`.
- **Added trapped gnome detection** — Every in-game hour, each gnome checks if it can reach any stockpile using `RegionMap::checkConnectedRegions()`. If trapped, sets "Trapped" thought bubble and logs a WARNING. Automatically clears when gnome can reach stockpiles again.
- **Improved job cancellation** — Cancelling a worked job now sets both `canceled` AND `aborted` flags, ensuring the gnome detects the cancel on the very next tick. Previously only `setCanceled()` was called, which some behavior tree states didn't check. Also added stale job sprite cleanup when no job is found at a cancelled position.
- **Improved dead gnome cleanup** — `Gnome::die()` now also clears room ownership (not just workshop assignments). Added death log message via WARNING log type.
- **Added WARNING log type** — New `LogType::WARNING` for trapped gnomes, deaths, and other player-visible alerts.

### Technical Details
- `evalNeeds()` in `gnome.cpp`: `qMax(-100.f, qMin(150.f, oldVal + decay))` prevents unbounded decrease
- Trapped check uses `ticksPerDay / 24` as hourly interval since `ticksPerHour` doesn't exist as a Util field
- Job cancel: `setAborted(true)` alongside `setCanceled()` hits the check at `gnome.cpp:648`
- Room cleanup iterates `g->rm()->allRooms()` — required adding `#include "../game/roommanager.h"`

---

## [2026-03-24] Selection Preview & Escape Key Fix

**Milestone**: 0.1c — UI Fixes
**Files changed**: `src/gui/mainwindowrenderer.cpp`, `src/gui/imguibridge.cpp`, `src/game/gamemanager.cpp`

### Changes
- **Fixed ghost/preview rendering for all tools** — Selection shader was using `m_axleShader->uniformLocation("tile")` instead of `m_selectionShader->uniformLocation("tile")`. This caused all tool previews (mine, build, workshop, etc.) to render at the wrong position. Previews now follow the cursor correctly.
- **Escape key now clears active tools first** — Previously, pressing Escape while a tool was active would skip straight to the pause menu. Now follows priority: active tool → side panel → pause menu.
- **Connected `signalPropagateKeyEsc` to `Selection::clear()`** — This signal was emitted but never wired to anything. Escape now properly clears the selection state on the game thread.

### Technical Details
- `paintSelection()` in `mainwindowrenderer.cpp:662` was copy-pasted from `paintAxles()` but the shader reference was never updated
- `onKeyEsc()` in `imguibridge.cpp` now checks `currentToolbar` and `currentBuildCategory` before falling through to panel/menu logic
- Added `connect(m_eventConnector, &EventConnector::signalPropagateKeyEsc, Global::sel, &Selection::clear)` in `gamemanager.cpp`

---

## [2026-03-24] Loading Performance — 98% Faster

**Milestone**: 0.1b — Loading Performance
**Files changed**: Renderer init path, sprite factory, DB queries

### Changes
- **98% reduction in loading time** — Batch DB queries, parallel tile processing, and bulk GPU upload
- Sprite factory initialization optimized
- Tile data upload to GPU now uses bulk operations instead of per-tile uploads

---

## [2026-03-24] ImGui UI Migration & Loading Screen

**Milestone**: 0.1 — Build System & Platform
**Files changed**: Multiple (full UI migration)

### Changes
- **Migrated entire UI from Noesis/XAML to Dear ImGui** — Removed Noesis dependency entirely
- **Added loading screen** with progress indicator
- **Added MCP test command server** for automated testing via `--test-mode`
- **ImGui theme** applied for consistent look

---

## [2026-03-24] macOS Port

**Milestone**: 0.1 — Build System & Platform
**Files changed**: Shaders, CMakeLists.txt, renderer

### Changes
- **Downgraded OpenGL from 4.3 to 4.1** for macOS compatibility
- **Replaced SSBOs with Texture Buffer Objects** (TBOs)
- **Fixed all shaders** for GLSL 4.1 compatibility
- Clean CMake build on macOS with Homebrew Qt5 and OpenAL

---

## [2026-03-24] Project Analysis & Roadmap

**Milestone**: Planning
**Files changed**: `docs/` directory

### Changes
- **Analyzed 15,893 Discord messages** across 4 channels (#suggestions, #bug-reports, #dev-discussion, #updates)
- **Created development roadmap** with 6 milestones from community feedback, dev knowledge, and codebase analysis (`docs/updates/development_roadmap.md`)
- **Extracted 100 feature requests** from 4,693 community suggestions (`docs/suggestions/feature_requests.md`)
- **Catalogued 407 bug threads** with 310 potentially still open (`docs/bug-reports/bug_report_summary.md`)
- **Built Discord reply queue** — 66 draft replies mapped to original messages, ready for batch send when milestone 0+1 are complete (`docs/discord_reply_queue.json`)
- **Documented complete version history** from v0.2.3 to v0.8.10 (`docs/changelogs/version_history.md`)
- **Created parallelization plan** for game loop threading (`docs/updates/parallelization_plan.md`)
