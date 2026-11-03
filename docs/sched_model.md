# The GC/2.6 instruction scheduler: the model and its replay

Written Oct 31, 2026 (plan.md step 87, branch sched-b; packages W2 / W3 of [scheduler_plan.md](scheduler_plan.md) as
changed by [sched_community.md](sched_community.md) section 5). What it is: the scheduler of mwcceppc GC/2.6 (the
compiler of the game, `-opt level=0,peephole,schedule,speed -proc gekko`) written down as "the GC/1.x algorithm of
rayanht/mwcc plus the GC/2.6 differences found", a Python replay of it (`tools/sched/replay.py`) and the replay
measured against the compiler's own output (`tools/sched/corpus.py`, `tools/sched/corpus_report.txt`).

**Result in one paragraph.** `-proc gekko` selects the **750 model** in GC/2.6 (not the 603 default that GC/1.2.5
falls back to). The 1.x pick rule is unchanged in 2.6 (read in the binary). Three 2.6 changes decide most orders: a
**second-integer-unit operand check** (dead code in 1.x, live in 2.6), the **2.x alias analysis** (ranges of one
object are independent; a pointer local has the worst-case alias) and **CR anti dependences at latency 0**. With them
and three facts about the passes around the scheduler (the final pass reschedules only the epilogue blocks; frame code
keeps its order; the `addi` fold runs after colouring), the replay reproduces **41 of 43** first-pass-only blocks of
the corpus exactly (95%) and **57 of 63** blocks through both passes (90%), against 0 of 43 for the unscheduled order;
the compiler's own output is a fixed point of the replayed final pass in 62 of 63 blocks. For the targets: the replay
reproduces **the compiler's 38 words of cr_CM_DUMMY_008 from our best C exactly**, and the original's 38 words are a
fixed point of the final pass, but no statement order of that C gives the original (best 24 of 38, 3,780 orders):
what is missing is the source, not the scheduler. fn_8006EEF8's vertex blocks still disagree inside the first pass
(section 6).

**Step 89 (Oct 31, 2026, branch sched-c): the hook.** A replacement lmgr326b.dll in the analysis copy of the
compiler folder logs the compiler's own scheduler input, DAG and every pick (section 10). Against it **the replay is
exact: on all 59 corpus files, 118 of 118 scheduled blocks (both passes) get the compiler's DAG from the logged PCode
and the compiler's pick sequence from the logged DAG** (tools/sched/corpus_report.txt, last lines). The disagreements
left in the listing-based measurement (79 of 89 blocks through both passes after this step's fixes) are all in the
input, not in the scheduler: the no-schedule listing is not the first pass's PCode (section 11). The hook settled the
q-vector pick (a call ends a PCode block, so heights and deadlines are per call segment), the flag meanings, the alias
sets of pointer locals, and what clears flag 8 (an instruction inserted into or deleted from a block after the first
pass). Inverse search for cr_CM_DUMMY_008 (section 12): base.c's PCode admits a first-pass output order that the
final pass turns into the original's 38 words exactly, but no input order of that PCode makes the first pass produce
it (best 25 of 38): the original's first-pass input holds instructions base.c's does not.

## 1. Sources (reference only: tools/downloads, gitignored, never run)

| Repository | Commit (shallow clone, Oct 31, 2026) | What was used |
|---|---|---|
| https://github.com/rayanht/mwcc | 92f893fbcbb23d9833be5fc24ac9e069090f8729 | `src/backend/Scheduler.c` (the 1.x algorithm), `MachineSimulation750.c` / `603.c` (the 1.x unit simulations and tables) |
| https://github.com/inspiredrobot/mwcc | ef08e865561446c072f457eaf89bde9030cccb30 | its docs folder: SCHEDULER.md (the GC/1.2.5 model, the rank table) and PASS_PIPELINE.md |
| https://github.com/zcanann/mwcc-rs | c00fecefe48b73be47af50f447822697bd8a1683 | `crates/pipeline/mwcc-pcode-to-machine-code/src/schedule.rs` (its 2.x switches, compared in section 5; hypotheses only: its own parity status reports no exactly matching functions, so it is not a tested oracle) |
| https://github.com/Savestate2A03/mwcc_debug | c18a6d7b3f0b2ad9a7865bfdde0e73260f56273d | the approach of the hook (a replacement lmgr326b.dll that answers the licence calls and patches the compiler from DllMain); none of its code is used |
| https://github.com/cadmic/mwcc-debugger | bad9cea2423bed957188c930086f9dabe669d30c | the GC/2.6 addresses: opcode table 0x5C0FA8 (471 records of 18 bytes), PCode block list 0x5EA748, the pass breakpoints |

The GC/2.6 binary read here is an analysis copy, `tools/sched/bin/mwcceppc.exe` (gitignored, SHA-1
1110a7a9924a09a608967efdf79ecf09900344e7, the same as the build's), in a Ghidra project of its own
(`tools/sched/ghidra/`, gitignored; `tools/sched/ghidra_scripts/SchedExport.java` names the functions below and
exports their decompiled C to `tools/sched/ghidra_export/`, gitignored: the compiler's code is not committed). The
replay reads the opcode and model tables from the build's compiler at run time (read only); no table of the compiler
is copied into the repository beyond the numbers quoted in this file.

## 2. Which model `-proc gekko` selects

The command line stores the processor as a word at 0x583B48 (its option table: 750 = 12, 7400 = 21, gekko = 22,
generic = 20, 7450 = 24; the `-opt display` names at 0x5A23A0 use the same numbers), copied to copts 0x5EB094. A
switch at 0x42DB1A (jump table 0x5A6660) turns it into the scheduler's processor byte 0x5EB096: 601 -> 1, 603 -> 2,
603e / 8240 / 8260 -> 5, 604 -> 3, 604e -> 6, **740 / 750 / gekko -> 4**, 7400 -> 7, 7450 -> 10, anything else -> 8.
`Scheduler_Schedule` (0x507C70) then picks the model: 10 -> 0x5D1C48 (the new 7450 model: width 6, an extra latency
of 4 on edges into a branch), 7 or the AltiVec flag -> 0x5D2C70, 5 -> 0x5D4498, 3 / 6 -> 0x5D5028, **4 -> 0x5D5B50
(750)**, 1 -> 0x5D6678, 9 -> 0x5D71A0, else -> 0x5D3970 (603). So gekko is the 750 model; the corpus agrees (750:
41 / 43 first-pass blocks, 603: 5 / 43).

The 2.6 MachineInfo is 1.x's with one more word: {width, omit register anti latency, extra branch-edge latency,
getLatency, reset, check, issue, advance, serialize, a hook (0x507BA0, returns 0)}. For the 750: width **2**, omit
**1**, extra **0**; its opcode records (6 bytes at 0x5D5B78 + 6 x opcode: unit, latency, three stage cycles,
serialize) are **identical to rayanht's 1.x 750 table** for every opcode 1.x has (only the AltiVec rows and the new
opcodes differ). The rows that matter at level 0:

| Opcodes | Unit | Latency | Stage cycles | Serialize | Pick rank (opcode table byte 9) |
|---|---|---|---|---|---|
| b, bl, bc, blr, bt, bf, bctrl | 0 BPU | 0 | 0 0 0 | yes | 0 |
| addi, addis, add, subf, rlwinm (slwi, clrlwi ...), xoris, extsh, srawi, neg | 2 IU1 or IU2 | 1 | 1 0 0 | | 2 |
| li, lis | 2 IU1 or IU2 | 1 | 1 0 0 | | 4 |
| mr | 2 IU1 or IU2 | 1 | 1 0 0 | | 0 |
| cmpi, cmp, cmpli | 2 IU1 or IU2 | 3 | 1 0 0 | | 1 |
| mulli / mullw / divw | 1 IU1 only | 3 / 5 / 19 | 3 / 5 / 19 | | 2 |
| lwz, lha, lhz, lbz, lfs, lfd, psq_l, lmw | 3 LSU | 2 | 1 1 0 | | 3 |
| stw, sth, stb, stfs, stfd, psq_st, stmw | 3 LSU | 2 | 1 1 0 | | 1 (stwu 2) |
| fadds, fsubs, fmuls, fmadds, fneg, frsp, fctiwz, fcmpu | 5 FPU | 3 | 1 1 1 | | 2 (fcmpu 1) |
| fmr | 5 FPU | 3 | 1 1 1 | | 0 |
| fdivs | 5 FPU | 17 | 17 0 0 | | 2 |
| mflr / mtlr, mtctr / crxor | 8 SRU | 1 / 2 / 1 | 1 / 2 / 1 | yes | 4 / 1 / 2 |

getLatency adds 2 for instruction flag 0x20000000 unless flags & 9, and operand count - 2 for lmw / stmw.

## 3. The algorithm (1.x plus the 2.6 differences)

Per block (`schedule_block` 0x507D80 in 2.6; `Scheduler_Schedule` calls it for every block with more than 2
instructions, skipping blocks with flags & 3 unless forced and blocks already flagged 8, then sets flag 8):

```
init: empty per-register lists (later defs, later uses) for the five register classes, empty memory lists,
      max_height = 0                                         # reset per block (0x508550), not global
for each instruction I from the LAST to the first (node N, N.height = N.latency = getLatency(I)):
    registers (0x508390), each operand of I in order, GPR r2 / r13 and r0-as-base skipped:
        use of R:  edge N -> each later def of R, latency L_anti; add N to R's uses
        def of R:  edge N -> each later use of R, latency N.latency (RAW)
                   edge N -> each later def of R, latency L_anti; add N to R's defs
        L_anti = 0 when the model's omit flag is set and the class is not SPR (GC/2.6: GPR, FPR, CR, VR;
                 1.x: GPR, FPR, VR only), else N.latency
    memory (2.x, through the alias of each instruction, 0x511CE0):
        load:  edge N -> each later store whose alias may alias I's, latency N.latency; add N to loads
        store: edge N -> each later load and store that may alias, latency N.latency; add N to stores
    in-order list (instruction flag 0x80): edge N -> every later 0x80 instruction, latency 0
    barrier (flag 0x1000000, flag 0x100, or the model's serialize byte: branches, bl, mflr / mtlr / mtctr):
        edge N -> every later instruction, latency 0; add N to barriers
    edge N -> every barrier already seen, latency 0
    no successor and the block ends in a branch: edge N -> the branch, latency 0
    an edge with latency into a branch gets + the model's extra (750: 0)
    each edge: target.npred += 1; N.height = max(N.height, edge latency + target.height)
    max_height = max(max_height, N.height)
deadline(N) = max_height - N.height
cycle = 0; model.reset()
while nodes left:
    issued = 0
    while issued < width:
        scan the unissued nodes in program order; ready = npred == 0 and earliest <= cycle and model.check(I)
        best = the first ready one; a later ready candidate C replaces best only when
            C due (deadline <= cycle) and best not due, or, both due or both not due:
            more successors with npred == 1 (the release count), else greater height,
            else (first pass only, virtual registers) a smaller opcode rank
        none ready: break
        issue best: each successor S: S.npred -= 1, S.earliest = max(S.earliest, cycle + edge latency)
                    (a 0-latency successor can issue in the same cycle, both passes)
        model.issue(best); issued += 1
    model.advance(); cycle += 1
```

The 750 machine (check / issue / advance, 0x57A680 / 0x57A5F0 / 0x57A260): nine stages (BPU, IU1, IU2, LSU1, LSU2,
FPU1, FPU2, FPU3, SRU), a ring of 6 instructions in flight retiring at most 2 a cycle in order, an instruction enters
its unit's first stage with the record's first stage count and moves LSU1 -> LSU2, FPU1 -> FPU2 -> FPU3 with the next
counts. check: no free ring slot -> no; a simple integer op (unit 2) takes IU1 when free, else IU2; both busy -> no;
both free -> yes; **one busy (GC/2.6, `sched_operand_conflict` 0x507BF0, which returns 0 unconditionally in 1.x):
no when the instruction reads or writes the GPR that is the first operand defined by the instruction in the other
integer unit, or by an instruction that completed in IU1 or IU2 during the last cycle**; any other unit: no when its
first stage is busy; finally a store (flag 4) waits while LSU2 holds a store.

## 4. The two passes and what runs between them

The pipeline (sched_community.md section 4): initial code -> first scheduling (virtual registers, `Scheduler_
Schedule(0)`) -> peephole forward -> register colouring -> prologue / epilogue -> peephole -> final scheduling
(`Scheduler_Schedule(1)`, physical registers). Measured with the replay:

1. **The final pass reschedules only the blocks changed after the first pass** (step 89, from the hook's block
   flags before and after each pass): the first pass sets flag 8 on every block it schedules, and the PCode insert
   and delete functions clear it (`and word [block+0x2A], 0xFFF7` at 0x4DCF60 / 0x4DCFA2 next to the count increment
   and at 0x4DCFE9 next to the decrement; the scheduler's own append, 0x4DCFF0, leaves it). So a block is
   rescheduled when colouring, the prologue / epilogue code or the peephole inserted or deleted an instruction in it:
   in a leaf function the body is merged with the prologue block (flags 5) and gets the frame code; in fn_8006EEF8
   (calls: the prologue stays a block of its own) the vertex blocks lose an instruction to the peephole (48 -> 47, 101
   -> 99) and are rescheduled. Step 87's "only the epilogue blocks" was the visible part of this rule. **A call ends
   a PCode block** (fn_8006EEF8 is 50 blocks), so heights and deadlines are per call segment.
2. **Frame code keeps its order** (the stfd / psq_st pairs, stmw, lmw, the LR save and restore; the 0x80 list is the
   likely carrier) and its loads / stores have no alias, so they are ordered against every memory access (the step
   84 "prologue / epilogue bound everything").
3. **The `addi` fold runs after colouring.** A pointer local set by `addi P, B, c` and used as `d(P)` is folded into
   `c+d(B)` use by use only while B's physical register still holds B; where colouring gave B's register to a value
   loaded through it, the later uses keep P (m_r7: `lwz r3, 0x14(r3)` folded, `stw r0, 0x0(r31)` not). This is the
   mechanism behind step 85's gap register. The replay takes the decision from the target (`fold_to_target`).

## 5. Every GC/2.6 difference found

| Point | GC/1.x (rayanht / inspiredrobot) | GC/2.6 (address) | Effect on the corpus (blocks exact, both passes) |
|---|---|---|---|
| Model for `-proc gekko` | 1.2.5: no case, the 603 default | the 750 model (processor byte 4) | 603: 13 / 63 |
| Second integer unit | 750: IU1 or IU2, no further check | operand check against the other unit and last cycle's integer completions (0x507BF0 live) | without it: 31 / 63 |
| Memory dependences | per object; same object always ordered; pointer ops against "aliasable" objects | per alias (0x511CE0): object, range (object + offset + size: disjoint ranges independent), set; instructions without one get the worst-case set | one object per symbol: 39 / 63 |
| A pointer local's accesses | object of the pointer | worst-case alias (no provenance through a callee-saved local) | provenance kept: 56 / 63 |
| CR anti / output latency | producer latency | 0 (every class but SPR) | no case in the corpus |
| Barrier flags | 0x420 + serialize | 0x1000000, 0x100 + serialize | (same instructions here) |
| In-order list | none | flag 0x80 instructions chained, latency 0 | frame code not in order: 56 / 63 |
| Edges into a branch | latency | + model extra (750: 0, 7450: 4) | none for 750 |
| Model record | MachineInfo of 8 words | one more word (the extra latency) and a hook | none |
| max height | global (inspiredrobot) or per block (rayanht) | per block (0x508550 sets it to 0) | kept across blocks: same 57 / 63 |
| Pick rule | urgency, release count, height, rank, program order | identical (0x507F50) | without the rank: 46 / 63 |
| Same-cycle release | 0-latency successors issue in the same cycle | identical; mwcc-rs' "final pass: loads wait" is not in the 2.6 loop | waiting a cycle: 52 / 63 |

mwcc-rs' other 2.x switches, compared: "final pass orders every memory access" is true only for instructions without
an alias (frame code, peephole products); for every access it gives 51 / 63. Its "second integer unit" is right, but
the operand check is what makes the 750 schedule.

## 6. The corpus and the measurement

`tools/sched/corpus.py` compiles 59 files (56 micro-programs of tools/sched/scratch/hd/m, hd/base.c, hd/v0.c,
fr/v13.c) with GC/2.6 and configure.py's game flags twice, with and without `schedule`; **58 compile** (hd/m/mixed.c
declares a local after a statement: "expression syntax error", a deliberate test of step 84). The no-schedule listing
is the first pass's input order (registers renamed: each definition of r0, r3-r12, f0-f13 is a new virtual register,
the callee-saved locals keep theirs). `tools/sched/replay.py --corpus` measures three things per block of 3+
instructions, words compared with register numbers masked; `tools/sched/corpus_report.txt` has the per-file numbers
and one row per switch:

| Measure | Default model | Unscheduled order |
|---|---|---|
| p1: first pass alone, blocks the final pass does not touch | **41 of 43 blocks exact**, 824 of 1,179 words by position | 0 of 43, 278 words |
| p2: both passes (first pass, the compiler's physical registers mapped on by masked text, frame code, final pass) | **57 of 63 blocks exact**, 1,261 of 1,670 words | |
| fix: the final pass run on the compiler's scheduled listing | 62 of 63 blocks unchanged | |

What disagrees: fn_8006EEF8 (fr_v13: 2 of 4 first-pass blocks, the two blocks of 285 and 304 instructions: of their 16
call-separated segments 6 match, among them every segment before the first PSMTXMultVec; the q-vector segment and the
vertex segments after each GXBegin do not), m_r1 and m_r7 (the pointer-local programs: the final pass sees other
physical registers than the compiler's because the first pass order differs), m_o5 (the unscheduled compile drops a
load the scheduled one keeps: not a scheduler question). The first diverging pick in fn_8006EEF8 (the q segment): the
compiler issues the conversion's low-word `stw` before `lfs lbl_80450BB4`, the replay the `lfs` (release count 1 vs
0: the `lfd` of the conversion needs both stores); the compiler behaves as if the `lfs`'s store had a second
predecessor or the `stw` released the `lfd`. Not resolved from the outside (max height shifts of 1-50: worse, 207-228
of 598 words instead of 243): the next package's hook decides it.

## 7. The targets

- **cr_CM_DUMMY_008.** From hd/base.c's unscheduled order the replay gives the compiler's own 38 words exactly (both
  passes, registers included once mapped). The original's 38 words are a fixed point of the replayed final pass.
  Against the original: 24 of 38 by position (the compiler on the same C: 22). **Step 84's observation is explained,
  but not by the release count**: in the original the increment's `lis` reuses r3, the register of the history
  index's base (colouring), so in the final pass it has to wait for the load that still reads r3 (anti dependence),
  while the mask statement's `lis` has a register of its own (r5) and goes first. The order is set by the first pass's order through colouring, so
  the question becomes the source that gives the original's first-pass order. A forward search over the seven
  statements of base.c (the pc load, the counter pointer, the history store with its `lha` in three places, the PC
  advance, the increment, the mask, the counter increment; 3,780 orders) reaches 24 of 38 at best: base.c's statements
  in another order are not the original.
- **fn_8006EEF8.** The original is a fixed point of the final pass (755 words, both frame blocks). Replay from v13.c
  vs the original: 358 of 755 by position (the compiler on v13.c: 605): the vertex-block disagreement of section 6.

## 8. What was tried and failed (numbers from tools/sched/corpus_report.txt and the session)

| Tried | Result |
|---|---|
| rayanht's 1.x algorithm as is (750, offset alias, final pass on every block) | p1 27 / 62 blocks (before the per-block split of section 4), p2 22 / 63 |
| + the GC/2.6 integer-unit check and the store rule | p1 47 / 62, p2 29 / 63 (the store rule alone: 2 of the 20) |
| + final pass only on blocks with frame code | p2 50 / 63, fix 59 / 63 |
| + register mapping by a consistent virtual -> physical matching (was k-th to k-th) | p2 53 / 63 |
| + frame code in order, final pass on the epilogue block, the post-colouring fold, pointer locals worst-case | p2 57 / 63, fix 62 / 63 |
| the 603 model | p1 5 / 43, p2 13 / 63 |
| max height kept across blocks, shifted by 1-50 | no gain; fn_8006EEF8 worse (207-228 of 598) |
| the fold on virtual registers in the first pass | p2 54 / 63 |
| every memory access ordered in the final pass | p2 51 / 63 |

## 9. The functions named in the Ghidra project

Scheduler_Schedule 0x507C70, schedule_block_body 0x507D80, init_register_owner_lists 0x508550,
build_sched_dependencies 0x508090, sched_register_deps 0x508390, add_dependency 0x508480, sched_store_deps
0x5082E0, select_ready_node 0x507F50, sched_operand_conflict 0x507BF0, alias_may_alias 0x511CE0, sched_once_init (the
alias set-up) 0x511F20, m750_* 0x57A240-0x57A830.

## 10. The hook (step 89)

`tools/sched/hook/schedhook.c` builds a replacement lmgr326b.dll for the **analysis copy** of the compiler folder,
`tools/sched/bin/` (gitignored; the build's decomp/build/compilers is never touched). It answers the compiler's three
licence calls (ordinals 189-191: lp_checkin, lp_checkout, lp_errstring) as the shipped stub does, and when the
environment variable SCHEDLOG names a file and the binary is the GC/2.6 build (the string `Metrowerks` at 0x58D224)
it patches a jump at three entry points from DllMain and writes:

| Line | Hooked at | What |
|---|---|---|
| `S` / `B` / `A` | Scheduler_Schedule 0x507C70 | the pass argument, every block (index +0x1C, count +0x28, flags +0x2A) before and after the pass |
| `K` / `I` | schedule_block_body 0x507D80 | the block and each PCode instruction: opcode +0x20, flags +0x14, source line +0x1C, the alias +0x18 (type +0x2C, object +0x10, offset +0x14, size +0x18, index +0x28, a set's member bits from +0x24), the operands (12 bytes each from +0x24) |
| `N` | the first pick of a block | the DAG: per node the instruction, latency +0x10, height +0x16, deadline +0x14, predecessor count +0x18, earliest cycle +0x12, every edge with its latency |
| `P` | select_ready_node 0x507F50 | each pick: the cycle, every ready candidate (height, deadline, release count, the model's check) and the one chosen |

Built with Microsoft Visual Studio 18 Community, MSVC 14.51.36231 (cl 19.51.36260 for x86) and the Windows SDK
10.0.26100.0 (`pwsh tools/sched/hook/build.ps1` finds them with vswhere): **analysis-only, not pinned** (setup.ps1 does
not install a native toolchain; the owner decides whether to pin one). Tools: `python -I tools/sched/hooklog.py
FILE.c` (one compile through the analysis copy with the game flags, the log in tools/sched/corpus), `--all` (every
corpus file), `python -I tools/sched/hookcheck.py LOG ...` (the replay against the log: DAG and picks);
`replay.py --corpus` adds the hook totals to the corpus report.

## 11. What the hook settled

- **The replay is exact against the compiler**: 118 of 118 scheduled blocks of the 59 corpus logs, both passes, DAG
  and pick sequence. The fixes it took: a register operand that is both used and defined counts as a def only
  (0x508390 takes `flags & 2`: a call's argument registers, `stwu`'s r1); lmw / stmw latency + PCode operand count - 2
  where the PCode lists every saved register (stmw r14: 20 operands, latency 20); memory edges through the logged 2.x
  aliases (type 0 object, 1 range, 2 set with member bits; `alias_may_alias` 0x511CE0 reimplemented in
  hookcheck.Alias): loads (flags 0x20002) wait for later aliasing stores, stores (0x40004) for later aliasing loads
  and stores; the barrier flags 0x1000000 / 0x100 and the in-order flag 0x80. The pick and the 750 machine needed no
  change.
- **(a) fn_8006EEF8 and the q-vector pick.** A call ends a PCode block, so the q segment (16 instructions before
  PSMTXMultVec) is a block of its own: max height 10, and the heights decide between the `stw` and the `lfs`. Step
  87's listing replay treated the whole 285-instruction run as one block, so its deadlines were wrong. With blocks
  split at calls (replay.py `SPLIT_CALLS`) fn_8006EEF8's listing replay goes from 2 of 4 to 24 of 30 first-pass
  blocks; the vertex blocks still differ in the listing replay because they are rescheduled by the final pass after
  the peephole deleted an instruction (section 4 item 1), which the no-schedule listing does not show.
- **(b) Flags** (counted over every logged instruction): 0x1 branch, 0x2 load, 0x4 store, 0x8 call, 0x10 a copy (mr),
  0x20 an access through a pointer (set alias), 0x40 seen on float loads / stores of constants and stack vectors (not
  decoded), **0x80 the in-order list: volatile accesses (the write-gather pipe's stb / stfs) and the frame saves /
  restores**, **0x100 a barrier: the frame's psq_l / psq_st**, 0x2000 / 0x4000000 on add, 0x4000 / 0x80000000 on
  simple integer ops, 0x400000 / 0x8000000 on addi, **0x1000000 a barrier: calls**, 0x2000000 calls, branches and
  stwu, **0x20000000 calls and branches** (getLatency's +2 is masked by flags & 9, so it never applies), 0x40000000
  float arithmetic.
- **(c) Pointer-local aliases.** An access through a pointer local gets a set alias (type 2) whose members are the
  objects whose address is taken or that are global, not every object: in m_o7 the epilogue's set does not hold the
  local stored at 8(r1), so the frame code does not wait for it. The listing replay's "pointer local = worst case" is
  right for globals and too strong for private stack objects.
- **(d) What clears flag 8**: section 4 item 1.

The listing-based corpus report (79 of 89 blocks through both passes, 63 of 69 first-pass-only, after this step's
fixes: calls end blocks, the "changed" final-pass rule, the stmw latency) keeps its value as the measure of what a
no-schedule compile can tell; its remaining misses are input differences: the vertex blocks of fn_8006EEF8 (the
deleted instruction), m_r1 / m_r7 (pointer-local aliases and colouring), m_o5 (a load the unscheduled compile drops).

## 12. The inverse search for cr_CM_DUMMY_008 (scheduler_plan.md C7)

`tools/sched/scratch/hd/inv/inverse.py` works on the hook's PCode of a C file: it anneals over first-pass input orders
(a definition before its uses; accesses of different statements that alias keep their order), replays the first
pass, colours the result with the original's registers (one consistent virtual -> physical map; the folded counter
pointer's uses rewritten onto its base), replays the final pass with the logged prologue and epilogue, and counts the
original's words. Self-test: base.c's own order gives the compiler's 38 words exactly.

| Search | Result (words of 38 equal to the original, registers included) |
|---|---|
| base.c's 7 statements in every order (1,260; 18 keep the aliasing order) | 25 |
| any instruction-level input order of base.c's 32 PCode instructions (annealing, 6-8 restarts of 4,000-6,000 steps; with and without the aliasing-order rule; the counter pointer or the PC load given an object alias) | 24-25 |
| the first pass's OUTPUT order searched directly (DIRECT) | **38**: an order exists that the final pass turns into the original |
| input orders scored against that one output order (PTARGET) | 8 of 32 positions |
| other pointer-local shapes: a history-element pointer (i_hist.c), a pointer to history_index (i_idx.c), a state pointer (i_state.c) | 28 / 0 / 0 even as DIRECT (the compiler on them: 12, 4, 2 of 38 by position) |

So the original's words are consistent with base.c's instruction semantics after the first pass, but not as a first
pass of base.c's PCode: the original's first-pass input had instructions that are gone from the final code (deleted
after the first pass, which also makes the final pass reschedule the block), for example a copy the colouring
coalesced. No shape matched, so no draft_batch rule was written and batches 12 / 9 / 8 were not run. fn_8006EEF8 was
not searched (its vertex blocks are final-pass blocks for the same reason).

## 14. Step 91: the deleted instruction, the near-miss labels

The hook also logs, between the first and the final scheduling, every PCode insertion (`+A` after, `+B` before:
0x4DCF30 / 0x4DCF70) and deletion (`-D`: 0x4DCFB0) with the instruction. For base.c the only real deletion is the
folded counter pointer's `addi r31, rX, 0xc` (the other `-D` lines are the body moving into the prologue block); for
fn_8006EEF8 the vertex blocks lose redundant `clrlwi rD, r29, 24` (the u8 alpha widened before its `stb`), a
duplicate `lis 0xcc01` and the gap pointer's `addi r30, r1, ...`.

**Synthetic instructions** (inverse.py `SYNTH`): one extra instruction placed anywhere in base.c's first-pass input
(18 statement orders that keep the aliasing order x every position), removed from the first pass's output before
colouring, both passes replayed against the original:

| Extra instruction | Placements | Best of 38 |
|---|---|---|
| a duplicate of an existing instruction (dead), a `mr` from a register, a dead `li` | (all candidates: 36,111 evaluations) | below 38 |
| a `mr` INTO an existing register, before its definition | 1,092 placements | **38** |
| an in-place `rlwinm` / `extsh` on a register right after its definition (the PC local r30 after `pc = cr_g_ScriptPC`, or an address temporary) | 156 placements | **38** |

So the original's first-pass input had one more integer instruction on a definition chain (most simply on the PC
local, statements in the order counter pointer, PC, history, increment, mask, advance, count) that was deleted before
the final pass. The C spellings tried give the same PCode as base.c (the front end folds them): `(s16 *)` casts,
`+ 0`, `&pc[0]`, through `u32`, `& 0xFFFFFFFF`, `| 0`, `pc = pc;`, `pc += 0;`, a parameter, dead assignments
(which stay: 39 words). A compiled sweep of 256 shapes (tools/sched/corpus/shp, the eight statement orders of the synthetic solutions x no
extra / a second `pc = cr_g_ScriptPC;` / `pc = 0;` / `pc = (s16 *) cmd_cnt;` / a second counter-pointer assignment /
`cmd_cnt = 0;` at three places x both declaration orders, scored with hd/pr.py) gives at best 18 of 38 by position
(the step 84 ceiling); every extra statement stays in the code (39-40 words): the level-0 compiler does not delete
dead local writes.

**Labels**: tools/sched/label_drafts.py, docs/drafts.md "Step 91":

| Label | Drafts | of them handlers (cr_*_ script commands) |
|---|---|---|
| structural (other instructions in at least one block) | 156 (96 + 60 with the frame too) | 22 |
| schedule-only (every block has the original's instructions, in another order) | 36 (28 + 8 with the frame too) | 17 |
| register-only (the body is the original once register numbers are masked) | 19 (17 + 2 with the frame too) | 0 |
| frame (only the prologue / epilogue differ) | 2 | 0 |

## 15. Step 92: the deleted instruction's class, the float-temporary rule

**cr_CM_DUMMY_008.** `tools/sched/shape_eval.py` scores a C shape by its hook compile (the deleted instructions),
its word score (scratch/hd/pr.py) and the inverse search over its own first-pass PCode (`inverse.py` now removes every
instruction the hook saw deleted between the passes before colouring; `NOMEM` drops the aliasing-order rule):

| Shape (tools/sched/corpus/s92, gitignored) | Words | By position | First-pass PCode | Deleted between the passes | Inverse best |
|---|---|---|---|---|---|
| B1 / B2 / B5: the history store as `(s16) *pc` (also `(s16) (s32)`, `(s16) (u32)`) | 38 | 12 | 33 | the counter pointer's addi **and an `extsh` after the `lha`** | **29** |
| B3 `(s32) (s16) (u16) *pc`, B4 `*(u16 *) pc`, B6 `(u8)` index, B7 `(u8)` mask store, B8 / B9 `(s16)` / `(u16)` depth | 39-41 | 3-7 | 33-35 | the addi only (the conversion stays) | 0 |
| A1 / A2 / A4: inline functions taking the PC or the counter pointer as a parameter | 38 | 18 | 32 | the addi only (the front end substitutes the argument: no copy) | 24 |
| A3 inline returning `*p`, A5 comma expression, A6 ternary, A7 / A8 inline returning the pointer | 40-45 | 1-3 | 9-33 | the addi or nothing | 0-20 |

- **The `(s16) *pc` store is the first real construct of class B**: the front end emits `extsh` after the `lha` and
  the passes delete it. With it the inverse ceiling rises from 25 to 29 of 38 (instruction-level annealing, with or
  without the aliasing-order rule), and the compiled score from 18 to **20 of 38 by position** (statement order PC,
  history, increment, counter pointer, mask, count, advance; tools/sched/corpus/s92p: the 210 legal statement orders x
  2 declaration orders; s92q: 180 combinations with the increment / mask / advance spelled `+= 1`, `++`, `x = x + 1`,
  with casts, `x = x & 0x3F`, `cr_g_ScriptPC = cr_g_ScriptPC + 2`, `++; ++`: best 20).
- **What is still missing**: `inverse.py SYNTH` on the `(s16)` shape finds 38 of 38 with one more deletable
  integer instruction: an in-place operation on the increment statement's `lis cr_g_Script@ha` (378 placements, all
  on that register: r48 in the log). No C spelling found: the increment / mask through inline helpers taking
  `&cr_g_Script.history_index` (tools/sched/corpus/s92s, 50 shapes) fold away like A1-A4 (best 19-20).
- **The `(s16)` store in the handler drafts** (tools/sched/corpus/s92r, the 21 handler drafts at 80-99% whose
  head the pattern finds): by position 16 lower, 4 equal, 1 higher; aligned words 19 lower, 1 equal, 1 higher. Not
  made a rule.

**fn_8006EEF8**: not worked on in this step (blocks 32, 36, 40, 43 of the call-split listing; the deleted
`clrlwi` of the u8 alpha is the known class B instance there).

**The near misses, grouped** (tools/sched/group_near.py over decomp/reports/sched_labels.txt): of the 36
schedule-only drafts, 20 differ only in the script-command head block (13 without a frame difference), 7 in a float
block, 9 elsewhere; the 19 register-only drafts differ in no block once registers are masked (colouring: locals' use
counts, step 85 rule 1). `tools/sched/permute_stmts.py NAME` permutes a flat draft's statements (locals defined before
use) and compiles each order: **fn_8001CCD4, fn_8001CD64, fn_8001CDF4 (36 of 36) and fn_8001D254 (63 of 63) match
when the float temporaries come before the stores**; fn_80076404 reaches 45 of 51 (300 orders). The generator variant
`hoist_float_temps` (tools/draft_batch.py, docs/drafts.md "Step 92") writes that order; `draft_batch.py --only` on the
six float drafts: the four at 100%.

## 16. Next

1. Run batch 12 in this worktree (the decomp/drafts folder of the worktree was emptied by the stopped runs) and watch
   it progress (decomp/drafts/c fills; the first-pass ctx_protos.h is written within minutes); then batch 9 (retail
   SHA-1, static shift PASS) and batch 8 (0 FAIL); report the drafts flipped by `hoist_float_temps` (expected: the
   four above and any other flat float draft).
2. The handler head: find a C construct that adds one deletable integer instruction on the increment statement's
   address (`lis cr_g_Script@ha` of `history_index += 1`), on top of the `(s16) *pc` store; score candidates with
   `python -I tools/sched/shape_eval.py DIR` (sources under tools/sched/corpus) and `inverse.py LOG 3 6 5000`.
3. fn_8006EEF8's four vertex blocks with the same method; the other schedule-only groups with permute_stmts.py.

## 17. Step 93: the issue slot of a deleted instruction; the head's address

**The mechanism, shown on fn_8001F6BC.** A deleted instruction changes the code through the first pass's issue
slots, not only through heights. In the call block of fn_8001F6BC (`cr_g_Pads[arg0].unk_92 = arg1;
PADControlMotor(arg0, 1);`) the 750 model issues two integer instructions a cycle. With `u16 arg1` the first pass
issues `mulli` and `lis` in cycle 0, the `addi` is not ready in cycle 1, and the argument moves `mr r3` / `li r4`
fill the gap, above the store. With `s32 arg1` the front end adds `clrlwi rX, arg1, 16` before the `sth`. It is ready
in cycle 0 and has the store as successor, so it takes the second slot. `lis` moves to cycle 1, and the
`addi` / `add` / `sth` chain comes before the arguments: the original's order, 39 of 39 words. The hook log shows
the `rlwinm` deleted between the passes. Generator variant `widen_narrow_params` (docs/drafts.md "Step 93").

**What this says about the synthetic search.** inverse.py SYNTH tried leaf instructions (`dup`, `mr from`, `li`):
they have no successor, so their height is that of a leaf and they never win an early slot. A real deleted
conversion feeds one consumer. Mode `REDIRECT` (step 93) inserts a conversion `r904 = op(u)` feeding one consumer of
`u` and gives the consumer `u` back before colouring. On the head (B1, the first 6 statement orders, 438
evaluations) it reaches 26 of 38, below `INPLACE`. `OPS=` chooses the opcodes of `INPLACE` / `REDIRECT`. `INPLACE`
with `OPS=addi,mr,addis,ori` gives 38 of 38 for every opcode, again only on r48. The working positions (19-30) lie
after the `addi r49, r48`: the synthetic instruction does not feed that `addi`. It reads and redefines r48 after its
last use, like a second assignment to a variable held in the `lis` register.

**C tried for the head's address** (tools/sched/corpus/s93*, gitignored): 22 spellings of the increment's address,
cr_g_Script declared as an array (`->`, `[0]`), defined in the unit (plain, static, initialised): all give B1's
first-pass PCode (29 inverse, 12 by position). A literal address `(cr_ScriptState *) 0x803BE358` emits `lis` + a
load with the low half in the displacement (no `addi`). Local pointer / value variables (`s = &cr_g_Script;
s->history_index += 1;`, reassigned for the mask, `i = cr_g_Script.history_index; ... = i + 1`): the copies stay
(39-43 words, 1-6 by position). The reference compiler (rayanht/mwcc, Operands.c `Operands_Normalize`,
PCodeUtilities.c `PCodeUtilities_EmitAddress`) builds a global's address as `lis` then `addi` into a new register
and copies only after the `addi` (a `mr` for an offset-0 address forced into a register).

## 18. Next

1. The head: a construct that redefines the increment's `lis` register after the `addi` and is deleted (a variable
   that the front end keeps in that register, assigned twice), scored with shape_eval.py.
2. fn_8006EEF8's vertex blocks. Done in step 94 as far as types go: the colour helper's alpha parameter declared
   `const u16` (tools/sched/scratch/fr/v14.c) raises v13.c from 485 to 531 of 755 by position (539 -> 601 in sequence;
   v13.c scores 485 with the current headers, 548 in step 85). Every other combination is lower or equal: alpha local
   u8 / s8 / u16 / s16 / s32 / int x parameter u8 / u16 / u32 / s32 / int (35), the r / g / b parameters u8 / u16 / u32
   (27, constants: no effect), the index s16 / int / long x the count u16 / s32 / u32 / s16 / int (20). What is left in
   each vertex block: the original issues the int-to-float constant's `lfd` early and ours late.
3. The 12 schedule-only drafts outside the head (group_near.py): a conversion or copy that takes an issue slot.

## 19. Step 110 (Nov 2, 2026): wider C sweeps of the head; the permuter set up

Main session, on main. Every result is by position against the original's 38 words (scratch/hd/pr.py).

- **20,000 random head shapes** (`tools/sched/scratch/hd/gen110.py OUT N SEED`, standalone sources with their own
  struct so the field types can vary). Axes: history_index u32 / s32 / int / unsigned long; cmd_count u32 / s32;
  cmd_history s32 / u32 / int; ScriptDepth s32 / u32 / int; 5 spellings of the history store, 10 of the increment,
  7 of the mask (`&= 0x3F`, `%= 64`, casts, shifts), 3 of the counter pointer, 5 of the count, 4 of the advance; the
  180 legal statement orders; both declaration orders. **Best 20 of 38** (3 shapes, the step 92 order). Field types
  and spellings change nothing beyond that.
- **Dead stores at level 0 are not deleted** (`dead110.py`, 924 shapes: 20 dead-local statements at every position
  of 6 orders, and in-expression assignments `v = x += 1`, `(s = &S)->...`). Every one that emits code adds words;
  none is removed between the passes. So the deletable instruction of section 18 is not a dead store. It stays in
  the two classes the hook has shown deleted: a copy coalesced at colouring and a redundant extension.
- **decomp-permuter** (scheduler_plan.md phase B): cloned to tools/downloads/decomp-permuter_dl (commit 8556c81),
  binutils extracted to tools/downloads/binutils_ppc, a venv in tools/downloads/permuter_venv (toml, Levenshtein).
  `tools/sched/permuter/cr_CM_DUMMY_008/` has base.c (the step 92 shape, standalone), target.o (the original's 38
  words assembled with powerpc-eabi-as, relocations kept), compile.sh (GC/2.6 with the game flags) and settings.toml,
  plus run_permuter.py (it runs the clone from outside its folder). Not run: the session's permission check refused
  to execute the downloaded code without the owner's approval. To run it:
  `tools\downloads\permuter_venv\Scripts\python.exe -I tools\sched\permuter\run_permuter.py tools\sched\permuter\cr_CM_DUMMY_008 -j 6`
  (from the repository root; the permuter folder holds absolute paths of this machine and is not committed).
- The hook (tools/sched/hook/build.ps1) and the corpus (tools/sched/corpus.py) were rebuilt after the worktrees were
  removed; `shape_eval.py` works again (B1 shape: 38 words, 18 by position, deleted: the counter pointer's addi and
  the extsh).

Next for the head: the permuter run once allowed. Then a coalesced copy whose source register is the increment's
`lis` (a copy into a variable that colouring merges with the `lis` result), checked with shape_eval.py.

## 20. Step 110, later the same day: the head solved (cr_CM_DUMMY_008 38 of 38)

With the owner's approval the permuter ran (`tools/sched/permuter_setup.py FUNCTION SOURCE.c --run JOBS` does the whole setup now; by hand: section 19, plus a `cpp` on PATH: tools/downloads/cpp_shim/cpp.exe, a
copy of LLVM's clang-cpp.exe; run_permuter.py needs the `__main__` guard on Windows). In about 40,000 iterations (12
minutes) it went from score 840 to 300. Its best source (tools/sched/scratch/hd/perm110_best.c) drops the counter
pointer: `(*(&cr_g_Script.cmd_count[cr_g_ScriptDepth]))++`. Every body instruction is then the original's, registers
included. Only the save and restore of r30 were missing (36 of 38 words).

The r30 save: a local whose only uses are in dead code, `u32 *cmd_cnt; ... if (0) { (*cmd_cnt)++; }` (also
`cmd_cnt = cmd_cnt;` or `while (0) { ... }`). Its uses count for the callee-saved order, so it takes r30 and keeps the
save, and the front end removes its code. `if (0) { cmd_cnt = 0; }` and `(void) cmd_cnt;` are not enough.

The source, 38 of 38 with the project header (tools/sched/scratch/hd/head110.c):

```c
void cr_CM_DUMMY_008(void) {
    s16 *pc;
    u32 *cmd_cnt;

    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) *pc;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth]++;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
}
```

`(s16)` is needed: m2c's `(s32) M2C_FIELD(pc, s16 *, 0)` gives 13 of 38. So what sections 14-18 called a deleted
instruction is a different first-pass shape: the counter written through no local at all. The scheduler model was
right; the source shape was never in the searched space.

The generator rule `script_head_dead` (tools/draft_batch.py, on by default; the step 80 pointer form stays a variant)
writes this head for every handler, with whatever PC advance it has (`+= 2`, `+= 4`, ...). `--only` on 9 handlers: 5
at 100% (cr_CM_DUMMY_008, cr_CM_SHAKE_STOP, cr_S_SE_STOP, cr_S_SE_PLAY, cr_S_BGM_STOP), 96-97% for two more. The full batch 12: **609 -> 777 drafts at 100%** (+168), 707 promoted, 948 of 1,300 units from C (27.14% of the code).
