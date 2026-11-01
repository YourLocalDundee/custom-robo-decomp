# The GC/2.6 scheduler: what the community already has (Phase A)

Written Oct 31, 2026 (docs/scheduler_plan.md section 4, branch sched-a). Reading and web research only: no build,
no compile, no Dolphin. The one local action was a read-only string scan of
`decomp/build/compilers/GC/2.6/mwcceppc.exe` and a look at a few bytes of it (section 4 below).

## 1. Verdict

**Phase C mostly collapses.** The scheduler of mwcceppc has already been decompiled and modelled by others, and
the GC/2.6 binary we build with is the same build another project already instruments:

- rayanht/mwcc is a *matching* decompilation of mwcceppc GC/1.0-1.3 with `src/backend/Scheduler.c` and one
  `MachineSimulation<cpu>.c` per processor (601, 603, 603e, 604, 750, 7400, 821). Its pick function is the
  recovered priority, and it is not a critical-path list scheduler: a candidate wins first by **urgency**
  (its deadline, max height - height, has been reached), then by the **number of successors this issue would make
  ready** ("release count"), then by **height**, then (first pass only, virtual registers) by a per-opcode
  **rank**, and otherwise the earlier instruction in program order keeps the slot. The release count ranked above
  height is exactly the component our 144 policies (step 84) did not have, and it is a candidate explanation of the
  step 84 observation that the mask statement's `lis` goes above the increment's although the increment's chain
  is taller.
- inspiredrobot/mwcc (mirror JackPriceBurns/mwcc) `SCHEDULER.md` (docs folder) gives the same algorithm with the
  machine-model tables for GC/1.2.5 (issue width 2, one unit per class, per-opcode latency / occupancy, branches
  serialize) and reports a Python simulator that reproduced a 54-instruction Melee block 54/54.
- zcanann/mwcc-rs reimplements the compiler in Rust with a scheduler built on that model and reports byte-exact
  canaries for GC/1.3 through **2.6 and 2.7**; its code names the 2.x differences (a second integer unit
  "post-1.2.5"; the final pass orders every memory access; zero-latency releases issue in the same cycle in the
  first pass).
- cadmic/mwcc-debugger already supports **GC/2.6** (its version check passes on our binary: the string
  `Metrowerks` sits at the same address, 0x58D224) and dumps the PCode between backend passes, including **after
  the first scheduling pass, before / after register allocation and after the final scheduling pass**, plus the
  allocator's priority list. It runs the compiler under retrowin32 + gdb on Linux / macOS, so for us (Windows
  only) it is a map of addresses and structures, not a tool to run as is.

So C3 (find the scheduler by coverage diff) is replaced by addresses (section 4), C4 (read the scheduler) by
reading a decompiled `Scheduler.c` and checking the GC/2.6 differences, and C8 (fit a model) should be
unnecessary. What remains is real but smaller: confirm the 2.6 machine model that `-proc gekko` selects (the 2.6
driver has a case the 1.x driver does not), its latency / unit table, the 2.x dependence differences, and the
level-0 specifics; then the replay (C6) and the application (C7) as planned. Nothing found shows another level-0
game matched this way; the algorithm itself does not depend on the optimisation level (level 0 only changes what
PCode reaches the scheduler).

## 2. Sources

| URL | What it is | What it says about the scheduler / schedule-only diffs | How it applies to us (GC/2.6, level 0) |
|---|---|---|---|
| https://github.com/rayanht/mwcc | Matching decompilation of mwcceppc.exe GC/1.0, 1.1, 1.1p1, 1.2.5, 1.2.5n, 1.3 (progress on decomp.dev) | `src/backend/Scheduler.c`: `Scheduler_Schedule(force)` picks the machine model from `copts.processorModel` and schedules every block with more than 2 instructions (`flags & 3` skipped unless `force`, `flags & 8` = done); `schedule_block` builds the DAG walking the block from the last instruction to the first (def / use lists per GPR / FPR / CR / SPR, per-object memory lists, pointer operations against all memory, barriers on flags 0x420 and the model's `checkLate`), height = latency path to the block end, deadline = max height - height, then a cycle loop issuing up to `count` instructions per cycle through `select_ready_coloring_node` (urgency, release count, height, opcode rank, program order). `MachineSimulation750.c` and the others: the per-processor unit / latency tables and the `check` / `issue` / `advance` functions | The algorithm to replay. GC/1.x, not 2.6: the 2.6 bodies must be compared (addresses in section 4); the structure and the pick rule are expected to carry over |
| https://github.com/inspiredrobot/mwcc (same docs at https://github.com/JackPriceBurns/mwcc) | Partial decompilation and notes on GC/1.2.5 (the Melee compiler) | `SCHEDULER.md` (docs folder): the model with its 1.2.5 addresses (driver 0x4ccae0, per-block 0x4ccbf0, DAG builder 0x4ccf10, pick 0x4ccdc0), the default model's table (issue width 2; one unit per class; `addi/mr/li/rlwinm` latency 1, `lwz/lfs/stfs` 2, `fadds/fmuls` 3, `fdivs` 18; branches serialize; `lmw/stmw` latency grows with the register count), the pick rule as a 4-level strict-win tie-break, the opcode rank (`mr` 0, `stfs/cmp/fcmp` 1, `addi/fadds` 2, `lfs/lwz` 3, `li/lis` 4), the global max height never reset between blocks (deadlines shift), and **`-proc gekko` in 1.2.5 falls to the default 603 model** (no case for its CPU value). `PASS_PIPELINE.md` (docs folder): the backend order: initial code, first scheduling, forward peephole, register colouring, prologue / epilogue, merge, peephole, **final scheduling** | The rank table and the pick rule are the first things to check on 2.6. The max-height point disagrees with rayanht's decompilation (which resets it per block); settle it on 2.6 |
| https://github.com/zcanann/mwcc-rs | Rust reimplementation of mwcceppc aiming at byte-exact objects; an oracle loop against the real compiler for GC/1.3, 1.3.2, 2.0, 2.5, **2.6**, 2.7 | `crates/pipeline/mwcc-pcode-to-machine-code/src/schedule.rs`: the list scheduler "following the model recovered from GC/1.2.5", with 2.x switches: a second integer unit (post-1.2.5 builds), struct frame objects with independent fields, every memory access ordered in the final (physical) pass, zero-latency releases issuing in the same cycle in the first pass but not for loads in the final pass; `MWCC_SCHED_TRACE` prints every pick. `harness/sched_dataset/`: ground-truth orders from GC/2.6 -O4,p for small shapes ("Linearization is a function of the block DAG, independent of source statement order for independent statements"; long-latency operations first; pointer stores keep their order) | A tested 2.x model to compare against; its canaries include `schedule_off` and `_o0` cases. Mostly -O4 work; the level-0 PCode differs, the scheduler should not |
| https://github.com/cadmic/mwcc-debugger | Python + gdb script that runs mwcceppc under retrowin32 and dumps internal state; supports GC/1.1 and **GC/2.6** | For GC/2.6: codegen 0x433492-0x4340B1, PCode block list 0x5EA748, opcode table 0x5C0FA8 (471 opcodes), pass breakpoints `after-scheduling` 0x433E0C (first pass), `before-regalloc` 0x433EA6, `after-regalloc` 0x433EAB, `after-prologue-epilogue` 0x433F8C, `after-peephole` 0x434006, final `after-scheduling` 0x434063; readers for the PCode, block and operand structures. README: "Scheduling ... is done both before and after register allocation"; the allocator is Chaitin-style, its priority list dumped | Our binary passes its GC/2.6 check. The layouts and addresses are what a Windows hook (C5) needs; its dumps give the pre-schedule PCode directly, better than our no-`schedule` compile |
| https://github.com/Savestate2A03/mwcc_debug | A replacement `lmgr326b.dll` for GC/1.2.5n that patches the compiler from `DllMain` to turn on its dormant debug listing (`pcdump.txt`) | `#pragma debug_listing` exists but the backend listing functions (`pclistblocks`) are compiled as empty stubs in the shipped compilers; the IR optimizer's logging is gated by a flag forced to 0 at init; the operand formatter is intact and can be called from injected code | **The Windows-native way to instrument the compiler**: it loads `lmgr326b.dll` from its own folder (our GC/2.6 folder has one), so a proxy DLL in an analysis copy of the folder can hook the scheduler with no debugger. Never in the build's folder |
| https://github.com/RootCubed/mwcceppc-decomp | Decompilation of mwcceppc for Wii 1.1 (CodeWarrior 9.2 era) | No scheduler source yet (only `CMachine.h`) | Not directly useful; a later compiler line |
| https://github.com/MarkMcCaskey/decomp-scripts | Scripts and `mwcc-compiler-notes.md` on the GC compiler's pipeline and allocator; PE inspection; headless Ghidra | Allocator-focused; addresses and layouts for live observation | Background for C1 tooling |
| https://github.com/KakarottoCake/decomp-triage | Triage tools for MWCC GC/1.2.5 matching (Super Mario Sunshine) | Classifies failures (frame-only, regperm, structural); a catalogue of "levers" | Not scheduler-specific |
| https://github.com/Batenkaitos-dev/mh2-decomp (`mwcc-matching.md` in its docs folder) | A project's matching handbook (another MW compiler line) | "Placement" recipes for schedule-only diffs (store order, a load before / after a branch, raw address vs named symbol); first scheduling pass, colouring on that order, a second pass with registers fixed; `#pragma schedule off` turns both passes off | The community practice: source tricks per symptom, no model. The same place our steps 84-85 reached |
| https://github.com/Limero/thug2 (`19-gxtev-color-sched.md` in its docs/parkings folder) | A GC project's write-up of three GX functions differing only by schedule (mwcc 1.2.5n) | "PERMANENTLY PARKED - pure scheduler-ordering coin-flip", after every source form, flag, compiler version and decomp-permuter runs; the file split around them, the three kept as asm | How projects without a model handle it: asm and a file split. The permuter (Phase B) is not expected to break the wall |
| https://www.nxp.com/docs/en/user-guide/EPPCUG.pdf | Freescale "Embedded PowerPC Targeting Manual" (2005), Target Settings Reference | The Processor and Instruction Scheduling settings (quotes in section 3); nothing on the priority | Only the documented surface |

Searches that found nothing more: "mwcc instruction scheduling priority", "metrowerks scheduler latency table",
the CodeWarrior manuals for a description of the priority (none goes beyond "optimized for the specific
processor"). Ninji's decompilation of the Mac-hosted CodeWarrior compiler (git.wuffs.org/MWCC), cited by two of
the projects above as the reference for the backend, returned 404 on Oct 31, 2026; not needed now that rayanht's
Windows GC decompilation has the same file. decomp.me and the big GC decomps (Melee, Twilight Princess, Pikmin 2)
were not read further once the compiler projects above turned up: they build at -O4,p and handle schedule-only
diffs with the same per-symptom tricks.

## 3. Documented options and pragmas

From the Freescale "Embedded PowerPC Targeting Manual" (EPPCUG.pdf, Target Settings Reference, pages 152-153):

> "Selecting a particular target processor has these results: Instruction scheduling -- If the Instruction
> Scheduling checkbox (also in the EPPC Processor panel) is selected, the processor selection helps determine how
> scheduling optimizations are made."

> "If the Instruction Scheduling checkbox is checked, scheduling of instructions is optimized for the specific
> processor you are targeting (determined by which processor is selected in the Processor list box.)"

> "Check the Peephole Optimization checkbox to have the compiler perform peephole optimizations. Peephole
> optimizations are small local optimizations that can reduce several instructions into one target instruction,
> eliminate some compare instructions, and improve branch sequences. This checkbox corresponds to #pragma
> peephole."

The pragma list of the same manual includes `peephole` and `scheduling`. The compiler's own help strings (read
from the GC/2.6 binary): `-opt` "schedule instructions"; `-proc` "specify processor for scheduling and inline
assembler"; `-O4` "same as '-opt level=4, peephole, schedule'"; `-O0` "suppress all optimizations; default";
`-opt display|dump` "display list of active optimizations"; an obsolete form "opt level=1, peep, schedule=601".
The processor names it knows include 601, 603, 603e, 604, 604e, 750, 7400, `gekko||gecko`, 821 and generic.

## 4. The binary scan (GC/2.6 mwcceppc.exe, SHA-1 1110a7a9924a09a608967efdf79ecf09900344e7)

Printable runs of 4+ characters matching sched / dump / dag / show / verbose / debug / peephole / pragma /
processor names. What looks like an option or a switch:

- **The pass-dump titles, in pipeline order**: `INITIAL CODE`, `BEFORE SCHEDULING`, `AFTER INSTRUCTION
  SCHEDULING`, `AFTER PEEPHOLE FORWARD`, `AFTER REGISTER COLORING`, `AFTER VALUE NUMBERING (POST COLORING)`,
  `AFTER GENERATING EPILOGUE, PROLOGUE`, `AFTER MERGING EPILOGUE, PROLOGUE`, `AFTER PEEPHOLE OPTIMIZATION`, `FINAL
  CODE AFTER INSTRUCTION SCHEDULING`, `FINAL CODE` (file offsets 0x1a3efc-0x1a4024). Two scheduling passes: the
  first before register colouring (as step 84 measured), a final one after the peephole.
- **Pragmas** (the pragma name table): `debug_listing` (next to `use_lmw_stmw`, `optimizewithasm`, `profile`),
  `dumpir`, `simple_prepdump` / `line_prepdump` / `fullpath_prepdump`, `scheduling`, `peephole`,
  `optimization_level`, `global_optimizer`, `optimize_for_size`, `processor`. IR-side texts: `Dumping function %s
  after %s`, `Dumps for pass=%d`, `Starting function %s`, the IRO pass names. Source file names left in
  assertions: `Scheduler.c`, `COptimizer.c`, `IrOptimizer.c`, `LoopOptimization.c`.
- **Command line**: `-opt display|dump` (lists the active optimizations: "- schedule for %s", "- no instruction
  scheduling"), `-v|verbose`, `--plugin-debug`, `--parser-debug`; the `-show` family belongs to the disassembler
  (object headers, DWARF, symbol tables).
- Per the mwcc_debug project, `debug_listing` prints nothing useful in the shipped GC compilers (the backend
  listing functions are empty stubs); untested here because running the compiler was out of scope. A one-minute
  check for the next session: compile a micro-program with `#pragma debug_listing on` and `-opt dump` and look
  for a new file in the working directory.

The driver, read from the bytes (the call sites just before cadmic's breakpoints): `call 0x507C70` with argument
0 returns to 0x433E0C (first pass) and with argument 1 to 0x434063 (final pass), so **0x507C70 is
`Scheduler_Schedule(force)` in GC/2.6** and the per-block call in its block loop goes to **0x507D70
(`schedule_block`)**. Its switch on the processor byte [0x5EB096] stores the machine model in [0x5E1858]: 10 ->
0x5D1C48 (a case the 1.x driver does not have), 7 or the AltiVec flag [0x5EB0A4] -> 0x5D2C70, 5 -> 0x5D4498, 3
and 6 -> 0x5D5028, 4 -> 0x5D5B50, 1 -> 0x5D6678, 9 -> 0x5D71A0, 2 and anything else -> 0x5D3970. It then calls
0x511F20 once and walks the blocks (instruction count at +0x28, flags at +0x2A: more than 2 instructions, `& 3`
skipped unless forced, `& 8` set when done). Which value `-proc gekko` sets in 2.6 (the new case 10 is the likely
one) is the first thing C1 checks; step 84 measured gekko = 750 = 7400 in order, 603 / generic different.

## 5. What the next session should do differently

1. **Skip C3 and most of C4.** Start from rayanht/mwcc `Scheduler.c` and `MachineSimulation*.c` and inspiredrobot
   `SCHEDULER.md` (docs folder); in Ghidra, compare 0x507C70 / 0x507D70 and their callees with that source (the pick, the
   DAG builder, the model `-proc gekko` selects, its opcode table). Write sched_model.md (new, in docs) as "the 1.x
   algorithm plus the 2.6 differences found".
2. **Test the pick rule on the known failures before anything else**: a small Python replay of the 1.x algorithm
   (750 and default models) over the step 84 micro-programs and the cr_CM_DUMMY_008 head, with the
   no-`schedule` compile as the pre-schedule order. If the release count explains the mask `lis` above the
   increment's, C6 is close.
3. **Take the pre-schedule PCode from the compiler itself rather than from the no-`schedule` compile**: the first
   pass works on virtual registers and the rank tie-break depends on them. On Windows: a proxy `lmgr326b.dll` (the
   mwcc_debug approach) in an analysis copy of the GC/2.6 folder that hooks 0x507D70 and the pick and prints each
   block before / after and each pick's candidates (height, deadline, release count, rank), using cadmic's GC/2.6
   structure layouts. That is C5 without x32dbg, and it is C8's oracle if one is still needed.
4. **Model both passes**: the final pass after the peephole reorders again (physical registers, every memory access
   ordered); a replay of the first pass alone will not reach 100%.
5. **Phase B (permuter) drops in priority**: other projects ran it on schedule-only diffs and parked them (thug2);
   run it only when a worker is idle.
6. On disagreements, compare with mwcc-rs: its 2.x switches are hypotheses already tested on canaries.
