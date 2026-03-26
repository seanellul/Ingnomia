# RFC: Gnome AI & Job System Redesign

> **Status**: Draft — needs review, stress testing, and iteration
> **Goal**: Scale gnome colonies from ~50 to 500+ without degrading tick performance
> **Output**: This document will be saved to `docs/design/gnome_ai_redesign.md` for collaborative review

---

## 1. Problem Statement

Benchmarks show gnome tick cost scales non-linearly:
- 7 gnomes: 25μs/gnome (1ms total)
- 50 gnomes: 26μs/gnome (2.3ms total)
- 100 gnomes: **43μs/gnome** (8ms total) — non-linear jump
- 200 gnomes: 30μs/gnome (7.3ms total) — budget-capped, not all gnomes tick

At 100+ gnomes, the per-gnome cost nearly doubles. The game becomes unresponsive at 200+ because the 5ms GnomeManager budget means many gnomes skip their tick entirely.

**Target**: 500 gnomes at <15ms total tick time (~30μs/gnome).

## 2. Current Architecture

### 2.1 The Tick Pipeline (game.cpp)
```
collectPaths()          ← gather async pathfinding results from last tick
grass/water/plants      ← parallel (std::async)
creatureManager.onTick  ← sequential, 2-5ms adaptive budget
gnomeManager.onTick     ← sequential, 5ms hard budget
jobManager.onTick       ← process returned/completed jobs
stockpileManager.onTick
farmingManager.onTick
workshopManager.onTick
passive systems         ← parallel (rooms, mechanisms, fluids, neighbors, sound)
events.onTick
dispatchPaths()         ← launch async pathfinding for next tick
```

### 2.2 How a Gnome Gets a Job (Pull Model)
```
Gnome BT ticks → "am I idle?" → YES →
  gnomeactions.cpp:574: jm->getJob(m_skillPriorities, m_id, m_position)
    → for each skill in priority order:
      → for each job type matching that skill:
        → for each job of that type (sorted by priority):
          → check: isWorked? isCanceled?
          → check: requiredItemsAvail?
          → check: requiredToolExists?
          → check: isReachable (region check)?
          → if all pass → CLAIM IT
    → if nothing found → m_jobCooldown = 100 ticks (~10 seconds idle)
```

**Cost**: O(skills × jobTypes × jobs) per idle gnome per tick. With 15 skills and hundreds of jobs, this is thousands of checks per gnome.

### 2.3 Pathfinding (Already Parallelized)
- Non-blocking dispatch/collect split across ticks
- Requests with shared goals are folded together
- Runs on async threads via `std::future`
- This is NOT the bottleneck — it's well-optimized

### 2.4 Social Interactions
- `processSocialInteractions()`: O(n²) nested loop over all gnome pairs
- Runs every 600 ticks (~1 minute in-game)
- At 200 gnomes = 19,900 pair checks

### 2.5 Time Budgets
- GnomeManager: 5ms hard cap. Processes gnomes sequentially from `m_startIndex`, wraps around.
- CreatureManager: adaptive 2-5ms based on creature count.
- Neither has per-entity budgets — early gnomes in the list get ticked more often than later ones.

## 3. Proposed Design

### 3.1 Push Model: Jobs Find Gnomes

**Current**: Every idle gnome scans every job every tick.
**Proposed**: When a job is created, it's pushed to eligible gnomes.

#### How It Works

**Job Manager maintains a skill→gnome registry:**
```
m_gnomesBySkill: QMultiHash<QString, unsigned int>
  "Mining" → [gnome_42, gnome_17, gnome_88]
  "Masonry" → [gnome_42, gnome_55]
  ...
```

**When `addJob()` is called:**
1. Get the job's required skill
2. Look up `m_gnomesBySkill[skill]` → list of eligible gnomes
3. For each eligible gnome, add job ID to their `m_pendingJobs` queue
4. Queue is priority-sorted — high priority jobs go to front

**When a gnome is idle:**
1. Check `m_pendingJobs` — O(1) pop from front
2. Validate: is the job still available? Reachable? Items available?
3. If valid → claim and start working
4. If invalid → pop next, repeat
5. If queue empty → enter "looking for work" state (limited fallback scan)

#### Edge Cases & Failure Modes

**Q: What happens when NO gnome has the required skill?**
- Job stays in the global job list, unclaimed
- The job board UI shows it as "No eligible workers"
- If a gnome later enables that skill, the registry updates and the job gets pushed to them
- Periodic sweep (every 60 ticks): check for jobs with zero eligible gnomes → warn the player

**Q: What if a gnome's pending queue grows huge?**
- Cap at ~20 pending jobs per gnome. Beyond that, new pushes are dropped (job stays in global pool for fallback scan)
- When a gnome finishes a job, it has room → next sweep refills from global pool

**Q: What about jobs created before any gnomes exist (e.g., loading a save)?**
- On save load: run a one-time "distribute all unclaimed jobs" pass
- On gnome skill change: re-scan global pool for matching unclaimed jobs

**Q: What if a job is canceled while in a gnome's pending queue?**
- Job has a `canceled` flag. When the gnome pops it, it checks and skips.
- No need to actively remove from all queues — lazy invalidation is sufficient.

**Q: Multiple gnomes eligible for the same job — who gets it?**
- First gnome to pop and claim it wins. Other gnomes see `isWorked=true` and skip.
- The push distributes to ALL eligible gnomes, but only one can claim.
- Closest gnome will likely claim first (processes earlier in tick order, or has fewer pending jobs).

### 3.2 Gnome States: Idle / Thinking / Acting

**Current**: Every gnome runs the full BT tick every frame, regardless of what they're doing.
**Proposed**: Gnomes have explicit states that determine tick cost.

```
┌──────┐   pending job    ┌──────────┐   validated    ┌─────────────────┐
│ IDLE │ ───────────────→ │ THINKING │ ─────────────→ │ WAITING_FOR_PATH│
└──────┘                  └──────────┘                └─────────────────┘
   ↑                         │  ↑                           │
   │  no jobs / cooldown     │  │ invalid job,              │ path arrived
   │←────────────────────────┘  │ try next                  │
   │                            │←──────────────────────────┘ (no path)
   │                                                        │
   │         job done / aborted                             ↓
   │←──────────────────────────────────────────────── ┌────────┐
                                                      │ ACTING │
                                                      └────────┘
```

**IDLE** (very cheap: ~1μs)
- Check pending job queue
- If empty: increment idle counter, maybe wander
- If has job: transition to THINKING

**THINKING** (moderate: ~5-20μs, budgeted)
- Validate job: reachable? items available? tool available?
- Has a per-gnome μs budget (~50μs). If validation takes too long, defer to next tick.
- If valid: request pathfinding, transition to WAITING_FOR_PATH
- If invalid: discard, try next pending job

**WAITING_FOR_PATH** (nearly free: ~1μs)
- Check if pathfinding result has arrived (via dispatch/collect system)
- If path found: transition to ACTING
- If no path / unreachable: discard job, back to IDLE

**ACTING** (cheap per step: ~5-10μs)
- Execute one step of the current BT action (move one tile, mine one block, etc.)
- Always completes the current step — no interruption mid-step
- After step: check for interruption flag (high-priority job arrived)

#### Interruption Protocol

When a priority ≥ 8 job arrives in a gnome's pending queue while ACTING:
1. Set `m_interruptFlag = true` on the gnome
2. At end of current action step, gnome checks flag
3. If set: abort current job (mark as unworked, return to job pool), transition to THINKING for the new high-priority job
4. The aborted job goes back to the global pool and gets re-pushed to eligible gnomes

**Q: What if the gnome is carrying items for the current job?**
- Drop items at current position (or return to stockpile if nearby)
- The interrupted job's item requirements are released (set `inJob = 0`)

**Q: What if mid-action the target tile becomes unreachable (wall built, floor removed)?**
- The BT action node already handles this — returns FAILURE, gnome's job gets aborted
- With the new state machine: transitions to IDLE, picks up next pending job

**Q: What about needs (hunger, thirst, sleep)?**
- `evalNeeds()` runs every tick regardless of state (it's cheap)
- If a critical need triggers, it overrides the current state → gnome seeks food/drink/bed
- This is already how it works — needs take priority over jobs

### 3.3 Social Interactions: Proximity-Based

**Current**: O(n²) all-pairs check.
**Proposed**: Spatial hash, only check nearby gnomes.

- Divide world into 20×20 cells
- Maintain `QHash<cellID, QList<gnomeID>>` updated when gnomes move
- In `processSocialInteractions`: iterate cells, only check pairs within same/adjacent cells
- At 200 gnomes on 200×200: ~0.5 gnomes per cell average → effectively O(n)

**Q: What about gnomes that are always far apart?**
- They never interact. This is realistic — gnomes working in different parts of the colony don't socialize.

### 3.4 Role System & Skill Caps

**Current**: Gnomes can have all ~15 skills active simultaneously.
**Proposed**: Max 3-5 active skills per gnome, organized by roles.

**Default Roles** (templates, customizable):
- **Laborer**: Hauling, Mining, Woodcutting (3 skills)
- **Crafter**: Masonry, Carpentry, Cooking (3 skills)
- **Farmer**: Farming, Animal Husbandry, Herbalism (3 skills)
- **Scholar**: Medicine, Machining, Engineering (3 skills)
- **Soldier**: Melee, Ranged, Defense (3 skills — these are already separate from work skills)

Players can reassign skills freely but are capped at the max.

**Impact on job search**: Instead of scanning 15 skills × N job types, scan 3-4 skills × N job types — immediate 4x speedup for the fallback scan path.

**Q: What if the player wants a generalist gnome?**
- **Decision: Soft cap with diminishing returns.** No hard limit on active skills, but each skill beyond 3-4 reduces XP gain and work speed (e.g., -10% per extra skill). Generalists are viable but noticeably slower than specialists. The UI shows the penalty clearly so players understand the tradeoff.

**Q: What if no gnome has a skill for an important job?**
- **Decision: UI warning + job stays.** Toast notification: "No gnome can do [Mining] — assign one!" Job stays in pool, highlighted differently in the job board. Player must actively assign a gnome to that skill. No auto-assignment — player agency is preserved.

### 3.5 Spatial Job Index (Fallback Search)

When a gnome's pending queue is empty and it needs to do a fallback scan:

- Jobs indexed in a grid (same 20×20 cells as social hash)
- Search order: own cell → adjacent cells → 2-ring → 3-ring → ...
- Stop when a valid job is found or search radius exceeds threshold
- **Priority override**: Jobs with priority ≥ 8 are in a separate global "urgent" list, always checked first regardless of distance

**Q: Does this mean far-away low-priority jobs never get done?**
- Yes, until a gnome wanders closer or finishes nearby work
- This is actually good gameplay — players should build stockpiles and workshops near work sites
- If the player really wants a distant job done, they can raise its priority

## 4. Open Questions (Need Research)

1. **Thread safety for push model**: When `addJob()` runs on the game thread, it modifies gnome pending queues. If gnome BT ticks are ever parallelized, this needs synchronization. Should we use a lock-free queue? Or keep gnome ticks sequential and only parallelize the push distribution?

2. **Optimal pending queue size**: How many jobs should a gnome buffer? Too few = frequent fallback scans. Too many = stale jobs piling up. Need to benchmark different cap values.

3. **Job cooldown redesign**: Currently 100 ticks (~10s) after failing to find work. With push model, this should be much shorter (10-20 ticks) since the gnome isn't doing an expensive scan anymore.

4. **Behavior tree compatibility**: The current BT XML drives gnome decisions. The state machine proposal wraps around the BT, not replaces it. Need to verify BT halt/resume works correctly for THINKING→WAITING transitions.

5. **Save/load compatibility**: Gnome state, pending queues, and the skill→gnome index need to be either serialized or reconstructed on load.

6. **Automaton interaction**: Automatons use `CanWork` too. Does the push model apply to them? They have different job patterns.

7. **Priority gaming**: If players set everything to priority 9, the system degrades back to "scan everything." Need a UI constraint — limited number of high-priority slots?

## 5. Benchmark Targets

| Metric | Current (200 gnomes) | Target |
|--------|---------------------|--------|
| Total tick time | 7.3ms | <5ms |
| μs per gnome | 30 | <15 |
| Job search time (idle gnome) | ~100μs (full scan) | <5μs (queue pop) |
| Social interaction time | O(n²) every 600 ticks | O(n) |
| Max gnomes before degradation | ~100 | 500+ |

## 6. Implementation Phases

**Phase A**: Push model + pending queue (can be tested independently)
**Phase B**: Gnome state machine (requires Phase A)
**Phase C**: Social spatial hash (independent, can parallel with A/B)
**Phase D**: Role system / skill caps (independent, UI work)
**Phase E**: Spatial job index (requires Phase A for fallback path)

Each phase should be benchmarked before and after with the existing test infrastructure.
