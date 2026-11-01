# The GC/2.6 scheduler wall: the plan to break it

Written Oct 31, 2026 at the end of the step 77-86 session, for the next session. This is the FIRST task of that
session, ahead of everything else in plan.md. It is written to be followed one step after the next, one worker at a
time.

## 1. What the wall is

Custom Robo's game code was compiled with CodeWarrior GC/2.6 (`decomp/build/compilers/GC/2.6/mwcceppc.exe`) at
`-opt level=0,peephole,schedule,speed -use_lmw_stmw on -str reuse,readonly` (docs/compiler.md). At that level the
compiler still runs its instruction SCHEDULER inside each basic block, and the scheduler's priority is the one thing
no C spelling of ours controls. Everything else about level 0 is now known and written down:

- Register allocation runs AFTER scheduling (step 84): the register numbers of the original carry no information
  about the source order; only the instruction ORDER does.
- Callee-saved registers are assigned by USE COUNT from r31 down, declaration order breaks ties; a local only
  written, or used only in removed code, is appended after the live locals and never makes a gap (step 85).
- A saved-but-unused register between two live locals is a pointer local to a stack object, assigned (not
  initialised) and used through members only; the peephole folds its uses onto r1 and drops the addi, the save
  stays (step 85). About 198 unmatched game functions show this gap; the recipe is a hand match, not a rule.
- The scheduler's dependence model, measured (step 84): constant-offset accesses to one symbol at different offsets
  are independent; a variable-indexed access conflicts with the whole symbol; a pointer access is a full barrier;
  different symbols are free; prologue / epilogue stack operations bound everything; the issue order of independent
  statements is structural (source permutations give the same listing).
- What does NOT change the order: 25 flag sets, 15 processor models (603 / generic / 5xx / 8xx give 10 of 38 words,
  750 / 7400 / gekko the 18), 44 pragmas, compilers GC/2.0, 2.5, 2.6, 2.7 (identical), 3.0a3-3.0a5 (worse);
  `#pragma scheduling off` with the original's statement order gives 1 word of 38 (the unscheduled allocation puts
  r3 everywhere). 360 statement-order x pointer-position files, inline / macro / pointer / volatile / register
  shapes: best 18 of 38 words by position (21 with registers masked). 144 list-scheduler policies (top-down,
  bottom-up, critical path, latencies, widths, tie-breaks) fit at most 23 of 38 on the head and about 55% on the
  micro-programs. Details: docs/drafts.md "Ninth generator" item 1, "Tenth generator" item 5, "Eleventh generator"
  items 1-5, "Step 85" (fn_8006EEF8 recipe).

What sits behind the wall (as of main 6e7cc51 / step 85):

| Blocked | Size | State |
|---|---|---|
| The script-command handler head: cr_CM_*, cr_S_*, cr_SYS_*, cr_C_ACT_*, cr_CHAR_* ... | ~300 functions, ~220 drafts at 78-85% | the biggest single block of the game code (about 17% of its functions); every handler begins with the same 38-word head (cr_CM_DUMMY_008 is the pure case) written by hand each time (the unused saved register sits at varying declaration positions across 246 handlers: r30 in 182, r29 in 23, r27 in 15, ...), so no macro reproduces it; one matching source SHAPE applies to all of them |
| fn_8006EEF8 (debug grid drawer, 0xBCC) | 755 words, 548 equal (72.6%) | frame and registers exact (tools/sched/scratch/fr/v13.c, hm/g2.c); the remaining difference is the schedule inside the vertex blocks after each GXBegin; its match closes the file run fn_8006EEF8..fn_8006FDD0 (7 functions, the first file object with its own data) |
| The gap-register class | ~198 functions | recipe known (step 85 rule 2), placement is a schedule question too |
| HSD near-misses: lobj SetupSpecularInit / SetupInit, texp SetReg / TExp2TevDesc, pobj PObjLoad / drawShapeAnim, texpdag CalcDistance, jobj resolveIKJoint1 / 2 | 8 files, 22 -> 30 objects | "register allocation" per step 78: with allocation after scheduling, these are schedule questions |

## 2. Goal and the done criterion

A **replay** of the compiler's scheduler in Python (`tools/sched/replay.py`) that, given the pre-schedule instruction
sequence of a block, produces the compiler's scheduled order on 100% of a corpus of compiler runs. Then, with the
replay: the source shape (or the generator rule) that gives cr_CM_DUMMY_008 at 100%; batch 12 shows the handler
family at 100%; fn_8006EEF8 at 100% and its run closed. Pass criteria stay the project's: batch 9 retail SHA-1 and
static shift PASS, batch 8 0 FAIL, checksums, batch 11 boot.

If the priority function turns out to be unrecoverable from the binary, the fallback (section 6, C8) still gets a
replay: instrument the compiler and learn the rule from its own decisions, which is far stronger than fitting from
the outside.

## 3. The four routes and the decision

1. **decomp-permuter** (simonlindholm/decomp-permuter): random source mutations scored against the target bytes.
   Cheap; searches source shapes; two workers showed no shape in a very wide space reproduces the order, so expect
   it to confirm the wall rather than break it. Run it in the background anyway.
2. **Reverse-engineer the scheduler in mwcceppc.exe** (Ghidra, coverage diff, hooks): reads the real rule once;
   its cost does not grow with the number of functions; the only route that scales. **This is the main route.**
3. **Empirical fit at scale**: what step 84 did with 144 policies, with thousands of micro-programs; helps only if
   the model family contains the real rule. Used as the fallback with ground truth from inside the compiler (C8).
4. **Other mwcc 2.x decomps** (Melee, Kirby Air Ride, ...): how they matched the same shapes. One hour; most of
   their code is at higher optimisation levels, so the tricks may not transfer.

Order: 4 for one hour, 1 started in the background, 2 as the main work, 3 as the fallback inside 2.

## 4. Phase A: community check (about 1 hour)

Search GitHub, decomp.me and the decomp wikis for: "mwcceppc" + "scheduler" / "scheduling" / "reverse",
"CodeWarrior PowerPC scheduler", "mwcc -opt schedule", "mwcc instruction scheduling priority", "metrowerks
scheduler latency table", repositories named mwcc-*, and the Melee (doldecomp/melee), Kirby Air Ride
(doldecomp/kar), Twilight Princess, Pikmin 2 repos for functions compiled at `-O0,p` with `schedule`. Also check
whether the compiler has undocumented dump switches: `strings` the binary (Phase C1 does this anyway) for words like
"dump", "sched", "dag", "-show", "ir". Deliverable: a short section in docs/drafts.md with links and the verdict.
Anything found collapses Phase C.

**Findings (Oct 31, 2026, docs/sched_community.md): Phase C mostly collapses.** The scheduler is already decompiled: rayanht/mwcc (a matching decompilation of mwcceppc GC/1.0-1.3) has `Scheduler.c` and the per-processor `MachineSimulation*.c`; the pick is not critical-path first but urgency (deadline reached), then the number of successors the issue makes ready, then height, then (first pass) an opcode rank, then program order; inspiredrobot/mwcc `SCHEDULER.md` (its docs folder) has the tables and a simulator that reproduced a Melee block, and zcanann/mwcc-rs reimplements it with the 2.x differences, tested against GC/2.6. cadmic/mwcc-debugger already maps our exact GC/2.6 binary (PCode structures, pass boundaries); from its call sites, GC/2.6 `Scheduler_Schedule(force)` is at 0x507C70 (called with 0 before register colouring, with 1 for the final pass after the peephole) and `schedule_block` at 0x507D70. Next: skip C3, read C4 against that source, replay the 1.x pick rule on the step 84 micro-programs first, hook on Windows with a proxy lmgr326b.dll in an analysis copy of the compiler folder (Savestate2A03/mwcc_debug's approach), and model both passes.

**Phase B/C progress (Oct 31, 2026, step 87, branch sched-b, docs/sched_model.md):** C1, C2, C4 and C6 done as far as the outside allows. `-proc gekko` selects the 750 model in GC/2.6 (processor byte 4; its table equals rayanht's 1.x 750 table); the pick (0x507F50) is the 1.x rule unchanged; the 2.6 differences are a live second-integer-unit operand check (0x507BF0, dead in 1.x), the 2.x alias analysis (disjoint ranges of one object independent, a pointer local worst-case), CR anti dependences at latency 0, the 0x80 in-order list and an extra branch-edge latency (0 for the 750). Around it: the final pass reschedules only the epilogue block, frame code keeps its order, the `addi` fold runs after colouring. `tools/sched/replay.py` over the corpus of `tools/sched/corpus.py` (58 of 59 files compile): 41 of 43 first-pass-only blocks exact, 57 of 63 blocks through both passes, 62 of 63 compiler listings a fixed point of the final pass (tools/sched/corpus_report.txt). It reproduces the compiler's cr_CM_DUMMY_008 from hd/base.c exactly; the original is a fixed point of the final pass but no statement order of base.c gives it (24 of 38): the source is what is missing. fn_8006EEF8's vertex blocks still disagree in the first pass (one pick traced in sched_model.md section 6). Next: the hook (C5), then C7.

**Step 89 (Oct 31, 2026, branch sched-c, docs/sched_model.md sections 10-13):** C5 done: a replacement lmgr326b.dll in the analysis copy of the compiler folder logs the compiler's scheduler input, DAG and picks, and the replay matches it on 118 of 118 scheduled blocks of the corpus (both passes). What was missing in step 87 was in the input: a call ends a PCode block, the final pass reschedules every block changed after the first pass (flag 8 cleared by any insertion / deletion), pointer locals carry set aliases. C7 for cr_CM_DUMMY_008: the original is reachable from base.c's instructions after the first pass but not as base.c's first pass (best 25 of 38): the source had an instruction deleted later. Next: shapes with a deletable copy, scored with the inverse search.

## 5. Phase B: the permuter in the background (about 2 hours to set up, then unattended)

- Clone simonlindholm/decomp-permuter into tools/downloads (untrusted download: its own empty directory, hash
  noted, run with `python -I` from outside it; nothing of ours copied into it). It is Python; its compile step is a
  script we write (`tools/sched/permuter/compile.ps1` wrapping mwcceppc with the game flags from configure.py) and
  its target is the original function's asm, produced with dtk (`decomp/build/GXCE01/asm/...` or
  `tools/dtk/dtk.exe elf disasm`) for cr_CM_DUMMY_008 and fn_8006EEF8.
- Base C: the best shapes so far (tools/sched/scratch/hd/base.c, v0.c; fr/v13.c).
- Let it run for hours; check the best score against our word-aligned harness (hd/pr.py). A 100% is a gift; a
  plateau is the expected result and is recorded.

## 6. Phase C: reverse-engineering the scheduler (the main work)

### C1. Setup and triage (half a day)

- The binary: `decomp/build/compilers/GC/2.6/mwcceppc.exe` (also 2.7, identical output: use whichever has the
  cleaner analysis). It is a 32-bit Windows PE. Keep a copy under tools/sched/bin/ for analysis; never modify the
  one the build uses.
- Tools, Windows only, 32-bit builds where the tool runs inside the process: Ghidra 12.1.4
  (tools/ghidra_12.1.4_PUBLIC, headless scripts in tools/scripts show how we drive it), DynamoRIO (drrun -t drcov)
  or Frida (frida-tools, 32-bit python or the `frida` CLI with `-f`), x32dbg (the 32-bit half of x64dbg), Python
  3.13 (tools/pyenv). New downloads go under tools/downloads with their hash recorded in setup.ps1's table, like the
  other tools; until then document the manual install in docs/INSTALL.md.
- Ghidra: a new project (tools/sched/ghidra/), import the exe, auto-analysis with the Windows x86 profile, then:
  every string (option words `schedule`, `peephole`, `level`, `speed`, `space`; processor names `603`, `604`,
  `750`, `7400`, `gekko`, `generic`; error texts), the data tables near those names (the processor latency /
  issue tables; step 84 showed the processor changes the schedule, so the scheduler reads them), and every function
  referencing the `schedule` option variable. Bookmark and name what is found (`opt_schedule_flag`,
  `proc_latency_table_750`, ...). Expect Metrowerks' own x86 compiler output: unusual calling conventions on
  internal functions; check a few known imports (fopen, printf) first to calibrate.
- Also note: the compiler may schedule twice (before and after register allocation; step 84 says allocation is
  after scheduling, so look for a post-allocation pass too) and the peephole runs after and moves things (step 80's
  fold). Keep the three apart.

### C2. The corpus (half a day; much of it exists)

Reuse tools/sched/scratch: hd/m/*.c (62 micro-programs), hd/sw_cnt and sw_none (the 360 statement-order files),
hd/base.c / v0.c (the pure handler head), hm/g2.c and fr/v13.c (fn_8006EEF8), the harness hd/pr.py (compile with
the game flags, dtk disasm, word-aligned score, side-by-side diff) and hd/sched_sim.py / fit.py (the 144-policy
fit). Build `tools/sched/corpus.py`: for each .c, compile twice with the game flags, once WITH `schedule` and once
WITHOUT (`-opt level=0,peephole,speed`), keep both listings; the without-schedule listing is the PRE-SCHEDULE order
(modulo allocation, so compare with registers masked). Extend the corpus with the 1,751 drafts (batch 12 compiles
them anyway; add a `--no-schedule` second compile to draft_batch.py or run it from corpus.py): thousands of real
blocks with their pre- and post-schedule orders. This corpus is the test for the replay.

### C3. Locate the scheduler by coverage diff (one day)

- Run the compiler on ONE micro-program under the coverage tracer twice: identical arguments except `schedule`
  present / absent. drcov writes a per-module basic-block list; convert to offsets within mwcceppc.exe and
  subtract: blocks that execute only with `schedule` are the scheduler (and whatever only it calls). Repeat with
  `processor 603` vs `750` (same source, schedule on): the blocks that differ touch the latency tables. Repeat with
  a 1-instruction function vs the 38-word head: separates set-up from the inner loops.
- Import the block lists into Ghidra (a small Ghidra script: colour / bookmark the addresses) and read the
  functions that own them. Deliverable: the list of scheduler functions with addresses and a one-line guess of
  each one's role, in docs/drafts.md. **Stop here and report before reading further (the main session checks that
  the cluster is plausible: a DAG builder, a priority pass, a pick loop, latency lookups).**

### C4. Read the scheduler (two to four days)

Expected shape, to be confirmed against the code: per basic block (1) build a dependence DAG over the instruction
list (register def / use, memory dependences with the rules measured in step 84: those rules are the test of
having found the right function), (2) compute a priority per node (height = longest latency path to the block end
is the classic; the compiler's has a component the 144 policies missed: step 84 saw the mask statement's `lis`
hoisted above the increment's although the increment's chain is taller), (3) a list scheduler: a ready set, pick
by priority, tie-break (original index? node id? operand class?), issue it, advance the cycle, repeat. Also:
per-processor latency and issue-width tables; special handling of loads vs stores vs branches vs the write-gather
pipe (fn_8006EEF8's stores); anything the `speed` / `space` keywords switch.

Name every function and structure in Ghidra (`sched_build_dag`, `sched_node_priority`, `sched_pick`,
`SchedNode.height`, ...), export the decompiled C of the cluster to tools/sched/ghidra_export/, and write the
recovered algorithm as pseudocode in `sched_model.md` (new, in docs): the DAG rules, the priority formula, the tie-break
order, the latency tables as numbers.

### C5. Confirm by hooking before trusting the read (one day)

Hook the pick point (x32dbg conditional breakpoint with a log string, or a Frida script attached with
`frida -f mwcceppc.exe -- <args>`): at each pick, log the ready candidates with their priority fields and the chosen
one, for the compile of cr_CM_DUMMY_008 (hd/base.c). Compare the logged priorities with the formula from C4 and the
logged choice with the formula's choice. Fix the read until the log and the formula agree on every pick of that
block, then on five random micro-programs.

### C6. The replay (one to two days)

`tools/sched/replay.py`: input = a block's pre-schedule instruction list (from the no-schedule listing, or from the
hook's dump of the DAG input), the processor, the flags; output = the scheduled order. Run it over the whole corpus
(C2): required agreement 100% of blocks (registers masked). Every disagreement is a missing rule: go back to C4 / C5
for that block. Keep `tools/sched/corpus_report.txt` with the agreement per file.

### C7. Apply (one to two days)

With a replay that is instant, the source-shape search becomes cheap:
- Forward: generate shapes (statement orders, temporaries, pointer locals) -> their pre-schedule order (the
  compiler's no-schedule compile, or a model of it: at level 0 the pre-schedule order is close to the statement
  order expanded) -> replay -> compare with the original; thousands per minute instead of one compile per second.
- Inverse (better): the replay is deterministic, so from the original's scheduled order derive the set of
  pre-schedule orders that map to it (constraint solving over the DAG: which nodes had to be ready when), then find
  the C whose statement order gives one of them. For the handler head that is the exact statement form the author
  wrote; turn it into a draft_batch rule (script head rewrite) and run batch 12. For fn_8006EEF8 the same per
  vertex block, then the file run closes (batch 12, batch 9, batch 11).
- Then the HSD near-misses of section 1 with the same tool.

### C8. Fallback if the priority cannot be read from the code

Use the hook as the oracle: log tens of thousands of pick decisions over the corpus (candidates with their fields,
the chosen one) and fit a model with the fields the compiler itself uses (height, successor count, latency, node
index, operand class, ...): a decision tree or a linear ranking. Ground truth from inside the compiler makes this
route 3 with the right features; done when the replay built from it hits 100% on the corpus.

## 7. Phase D: the functions behind the wall

In this order, each verified with batch 9 (retail, static shift PASS), batch 8, checksums, and batch 11 when a file
object or run changes: (1) cr_CM_DUMMY_008 to 100%, then the generator rule and a full batch 12 (expect most of the
~220 handlers to flip); (2) fn_8006EEF8 (closes the first data-owning run); (3) the gap-register functions that
block the runs of docs/drafts.md "Step 85" (fn_80080FD8, run 4; then runs 2 and 3); (4) the 8 HSD near-miss files.
Docs: plan.md step 87+ per phase, docs/drafts.md sections, docs/decomp.md status, sched_model.md (new, in docs).

## 8. How to run this in packages

One worker at a time, in a git worktree (docs/REPRODUCE.md "A second checkout"), branch per package, plain commit
messages (CONTRIBUTING.md), Windows only, CRLF docs, check_docs 0 problems; the main checkout
merges and verifies (docs/workflows.md section 14). Suggested packages, each with a hand-back report:

| Package | Phase | Budget | Report back |
|---|---|---|---|
| W1 | A + B | 3 h | links and verdict; permuter running with its best score |
| W2 | C1 + C2 + C3 | 2 days | the scheduler cluster (addresses, roles), the corpus size; STOP for the check |
| W3 | C4 + C5 | 4 days | sched_model.md (new, in docs), the hook log agreeing on cr_CM_DUMMY_008 |
| W4 | C6 | 2 days | replay.py at 100% on the corpus (or the fallback C8 started) |
| W5 | C7 + D(1) | 2 days | cr_CM_DUMMY_008 at 100%, the rule, batch 12 numbers |
| W6 | D(2)-(4) | as needed | runs closed, HSD files linked |

Every worker reads this file, docs/drafts.md "Eleventh generator" and "Step 85", and tools/sched/scratch/README.md
first. If a package fails its goal, it writes exactly what it tried and measured (as steps 84 / 85 did) and stops;
the next package is re-planned from that record, not restarted blind.

## 9. Risks

- Stripped binary, Metrowerks' own x86 code generation, possible custom calling conventions: coverage diff and the
  hook compensate for the missing names.
- Two scheduling passes or a scheduler interleaved with allocation: find both; the peephole after them.
- The corpus must be compiled with exactly the game flags and GC/2.6; `-opt level=0` is unusual (most decomps use
  -O4,p), so community knowledge may not apply.
- 32-bit tooling on Windows 11 is fine; keep the analysis copy of the compiler separate from the build's.
- Analysing a compiler we own to reproduce its output for a build we own is interoperability work; no distribution
  of the compiler or its code.
