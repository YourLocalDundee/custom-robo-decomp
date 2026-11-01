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

## 1. Sources (reference only: tools/downloads, gitignored, never run)

| Repository | Commit (shallow clone, Oct 31, 2026) | What was used |
|---|---|---|
| https://github.com/rayanht/mwcc | 92f893fbcbb23d9833be5fc24ac9e069090f8729 | `src/backend/Scheduler.c` (the 1.x algorithm), `MachineSimulation750.c` / `603.c` (the 1.x unit simulations and tables) |
| https://github.com/inspiredrobot/mwcc | ef08e865561446c072f457eaf89bde9030cccb30 | `docs/SCHEDULER.md` (the GC/1.2.5 model, the rank table), `docs/PASS_PIPELINE.md` |
| https://github.com/zcanann/mwcc-rs | c00fecefe48b73be47af50f447822697bd8a1683 | `crates/pipeline/mwcc-pcode-to-machine-code/src/schedule.rs` (its 2.x switches, compared in section 5) |
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

1. **The final pass reschedules only the blocks that received the epilogue.** A block scheduled in the first pass
   keeps flag 8 and the final pass skips it; leaf blocks without frame code come out exactly in first-pass order (the
   final pass run on them breaks 21 of 63 blocks), and the entry block of fn_8006EEF8 keeps its prologue in front of
   the first-pass body (rescheduling it moves 40 of its 105 words). Measured: "final pass on every block" 36 / 63,
   "on every block with frame code" 57 / 63 (61 / 63 fixed points), "on the epilogue blocks" 57 / 63 (62 / 63).
   What clears the flag of the epilogue block is not read yet.
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

## 9. Next

The hook (sched_community.md section 5 item 3): a proxy lmgr326b.dll in an analysis copy of the GC/2.6 folder that
breaks at `schedule_block` 0x507D80 and the pick 0x507F50 and prints, per block, the PCode in (opcode, operands,
flags at +0x14, alias at +0x18) and each pick's candidates (height +0x16, deadline +0x14, release count), using
cadmic's layouts. It answers: the pass-1 input of fn_8006EEF8's vertex blocks (and the q-segment pick of section 6),
which instructions carry flags 0x80 / 0x100 / 0x1000000 / 0x20000000, what clears flag 8 of the epilogue block, and
the alias sets of pointer accesses. With the hook's input the replay is tested without the no-schedule compile and
without the register mapping. The functions named in the Ghidra project: Scheduler_Schedule 0x507C70,
schedule_block_body 0x507D80, init_register_owner_lists 0x508550, build_sched_dependencies 0x508090,
sched_register_deps 0x508390, add_dependency 0x508480, sched_store_deps 0x5082E0, select_ready_node 0x507F50,
sched_operand_conflict 0x507BF0, alias_may_alias 0x511CE0, sched_once_init (the alias set-up) 0x511F20, m750_*
0x57A240-0x57A830.
