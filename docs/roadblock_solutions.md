# The seven roadblocks: methods compared, the choice, the fallback

Written Oct 31, 2026 (research only: reading this repo and public sources; no build, no compile, no Ghidra, no
Dolphin). For each roadblock of plan.md's "Roadblock map and the queue after the seven" bullet: what the decomp
community and the tools offer, compared against this project's constraints (Windows only; every tool pinned and
hash-checked, nothing prebuilt shipped; main.dol byte-identical to retail SHA-1; the build stays shiftable), then the
choice, the fallback chain, the done criterion and the first three steps. Claims are marked as in
[README.md](README.md) ("How claims are marked"): **Verified** (seen in the source or doc named), **Inferred**
(follows from what was read, not checked), **Unknown**.

State this page starts from (Verified: `decomp/reports/progress_report.txt`, `decomp/reports/splits_report.txt`,
plan.md "Resume here", git log of main 798493c): 661 of 949 units from C, 22.20% of main.dol's code, 12.49% of its
data; HSD 22 of 30 objects; 229 automatic objects with 1,517 functions; batch 12: 1,706 of 1,751 drafts compile, 483
at 100%, 213 at 80-99%; step 87 (merged): the GC/2.6 scheduler replay reproduces 57 of 63 corpus blocks through both
passes (docs/sched_model.md); step 88 (branch hsd3, not merged): HSD mtx.c from C.

## Summary

| # | Roadblock | Chosen method | Fallback (in order) | Done when | Depends on | Effort | Blocked by |
|---|---|---|---|---|---|---|---|
| 1 | GC/2.6 scheduler wall | Finish the replay against the compiler's own decisions (in-process hook on Windows), then the **inverse search**: from the original's order, the first-pass input orders that produce it, then the C that gives one of them | (a) learn the open rules from hook logs (C8); (b) permuter scored by the replay on the handler head; (c) the residual functions as `asm` functions inside their C file; (d) leave the run as dtk's extracted object (today's state) | replay 100% of corpus blocks (63 of 63 and every hook-dumped block of fn_8006EEF8); cr_CM_DUMMY_008 100%; batch 12 shows the handler family flip; fn_8006EEF8 100% and its run linked; batch 9 retail + static shift PASS, batch 8 0 FAIL, batch 11 PASS | the hook (sched-c) | 6-11 days | the owner's yes to one new pinned native tool (hook build); see "What to decide" |
| 2 | Typed game data at scale | Types from the reader code (base-register scan, the readers' field offsets, Ghidra's auto-create-structure as a proposal only), symbol splits in `symbols/` with a `why:` line, then `gen_data_c.py` units; string tables wait for their file | (a) an opaque `u8[]`/`u32[]` definition of the preceding object, only to reach an 8-aligned start; (b) leave the object in dtk | every step 86 blocker is a unit in `gen_data_c.py` UNITS (batch 9 PASS) or has a one-line reason; data from C above 12.49% in progress_report.txt; batch 8 0 FAIL; batch 11 PASS | nothing (string tables: roadblock 5/6) | 3-5 days | nothing |
| 3 | HSD remainder | Per function, the first source that compiles equal: wowjinxy/KAR C (same revision) -> doldecomp/kar asm transcribed -> Melee C (now complete) adapted -> by hand; edges proven by naming the neighbour from KAR's per-file asm; the 8 near-miss files through roadblock 1's replay | (a) the remaining functions as `asm` functions in the C file; (b) the file stays unlinked (CR_NOT_MATCHING with the reason) | 30 of 30 HSD objects linked (progress_report.txt), every `GALE01:sysdolphin` edge line in splits_report.txt proven; batch 9, batch 8, batch 11 | roadblock 1 for the 8 near-miss files | 4-8 days (+ roadblock 1) | nothing for the asm-only functions and the edges |
| 4 | m2c draft limits | Generator post-passes for the three typing patterns plus type fixes (the repo's rule: never edit a draft), then a diff-class triage of the 80-99% drafts (structural / register-only / schedule-only / frame) with the replay as the schedule test; hand matches on the structural class | (a) the permuter on the register-only class; (b) Ghidra's typed C as a second draft for the 45 failures; (c) hand C per function | 45 compile failures -> 0 or each with a reason in draft_report.txt; every 80-99% draft labelled with its class; 100% count above 483 | roadblock 1 for the schedule-only class; roadblock 2's types help | 4-6 days | nothing |
| 5 | Splitting the 229 automatic objects | Extend gen_dtk_splits.py's evidence: the per-file pool rule (in use), the data-pool base rule of step 86, the 8-aligned section padding, duplicated string literals (one file never holds two equal literals with `-str reuse`), the .bss/.sbss reverse order; every split with its proof in splits_report.txt | (a) splits only where two independent rules agree; (b) leave automatic (byte-identical, shiftable through dtk's relocations) | the 229 count falls with each new split proven; batch 9 retail + static shift PASS; batch 11 PASS | nothing | 4-7 days | nothing |
| 6 | One-function objects into source files | dtk-template's model: one `Object` per source file in configure.py, the file linked only when complete; promote_drafts.py's file objects grown run by run, data defined in the file; an unmatched function stays as an `asm` function inside the file only if a test proves it keeps the file shiftable | (a) the run stays dtk's extracted object until it matches (dtk-template's NonMatching); (b) split the file around the function (only where no file-pool data is touched) | the first multi-function run with its data (fn_8006EEF8..fn_8006FDD0) linked as one C file; batch 9 retail + static shift, batch 11 | roadblock 1 (closed runs), roadblock 5 (boundaries) | 2-4 days per pattern, then linear | roadblock 1 |
| 7 | Fresh-clone run and script consolidation | Keep `from_scratch.ps1` -> `setup.ps1` + `run_all.ps1` with configure.py + ninja inside; one shared pinned-download function; the short check (batch 9 + 8 + checksums) as the local "CI"; drop or freeze what the Windows-only rule makes dead | (a) a self-hosted Windows runner running the short check (owner's machine, the ISO never leaves it); (b) no CI, the short check by hand at every merge (today) | `pwsh tools\from_scratch.ps1` PASS from an empty folder on Windows with every checksum matching; one download routine; the docs name one entry point | every other roadblock (run last) | 2-3 days | the owner's decisions on the `.sh` twins and CI |
| 8 | More split evidence; the chunks with many unknowns (Nov 1, 2026) | Two new measurable split rules: the float-pool restart (a constant repeated in .sdata2 means a new file) and assert file names not yet used as anchors. Then the particle family named by compiling Melee / wowjinxy C, the unknown game chunks named from typed data, community addresses and a runtime trace | (a) one rule plus a C match; (b) leave the range automatic | section 8's done criterion | nothing | 3-6 days | nothing |

## 1. The GC/2.6 instruction-scheduler wall

**Where it stands (Verified: docs/sched_model.md, plan.md step 87).** `-proc gekko` selects the 750 model; the GC/1.x
pick rule of rayanht/mwcc holds in 2.6 with three 2.6 changes; the replay (`tools/sched/replay.py`) reproduces 41 of
43 first-pass-only blocks and 57 of 63 blocks through both passes. cr_CM_DUMMY_008: the compiler's own 38 words are
reproduced from our C; the original's 38 words are a fixed point of the final pass, but no order of our seven
statements gives it (best 24 of 38 over 3,780 orders): **the source shape is missing, not the scheduler model**.
fn_8006EEF8's vertex blocks still disagree inside the first pass. A worker is on branch sched-c (Verified: `git
branch`), presumably the hook package (Inferred from sched_model.md section 9).

**What the final (post-peephole) pass means for a replay (Verified: sched_model.md section 4; sched_community.md
section 4).** The backend runs: first scheduling on virtual registers -> peephole forward -> register colouring ->
prologue / epilogue -> peephole -> final scheduling on physical registers. Measured consequences: only the blocks that
received the epilogue are rescheduled in the final pass; frame code keeps its order; the `addi` fold runs after
colouring. So (a) a replay must model both passes and the colouring between them, and its input is the first pass's
PCode, which the no-`schedule` compile only approximates (`#pragma scheduling off` turns off both passes:
sched_community.md, mh2-decomp `mwcc-matching.md`); (b) register numbers in the original are the product of the first
pass's order plus colouring, so an "inverse" search must work on the first-pass order, then let colouring and the
final pass run forward; (c) the hook must dump the block at both pass inputs (cadmic's GC/2.6 breakpoints exist for
both: after-scheduling 0x433E0C and the final one 0x434063, before / after regalloc 0x433EA6 / 0x433EAB; Verified in
sched_community.md from https://github.com/cadmic/mwcc-debugger).

**Register-allocation-only diffs at matching time (how projects handle them).** objdiff counts a function as matched
only when its instructions are equal (with relocation targets optionally relaxed: our `-c functionRelocDiffs=none`,
Verified in docs/decomp.md "HSD (sysdolphin) notes"); a register-only diff stays a non-match everywhere. The community
levers: the permuter, which its README calls "generally best towards the end, when mostly regalloc changes remain"
(Verified: https://github.com/simonlindholm/decomp-permuter README FAQ); declaration-order and inline tricks
(decomp-triage's `decl-order` lever family "only recolours registers", Verified:
https://github.com/KakarottoCake/decomp-triage); and Melee's `#ifdef MUST_MATCH` source variants (47 such blocks in
tools/melee 24e685d `src`, Verified by grep; e.g. axdriver.c's `sqrtf_store` with a volatile slot to pin stack
pairs). Its regperm bucket is small in Sunshine (20 of 10,115 functions, Verified: decomp-triage README). For us
step 84 showed allocation follows scheduling and step 85 that saved registers go by use count (Verified:
docs/drafts.md "Eleventh generator", "Step 85"), so most "register" diffs of the drafts are schedule or source-shape
diffs; the replay plus a colouring model is the tool that separates the two (Inferred).

| Method | What it needs | Evidence it worked, and where | Cost | Risk | Fits our constraints? |
|---|---|---|---|---|---|
| Replay of the scheduler (in progress) + inverse search | the hook's PCode for ground truth; replay.py; a constraint search over the DAG | Verified for us: 57 / 63 blocks, cr_CM_DUMMY_008's compiler output exact (sched_model.md). Elsewhere: a Python simulator reproduced a 54-instruction Melee block 54/54 (inspiredrobot/mwcc `SCHEDULER.md`, cited in sched_community.md); nobody has published an inverse search (Unknown) | 2-4 days to 100%, 2-4 days inverse + generator rule | the source may contain statements the compiler deletes (dead code, folded pointer locals), so the inverse must search over PCode, not over our statements | yes: Python, Windows, nothing shipped |
| Hook the compiler: proxy `lmgr326b.dll` | a 32-bit DLL built from our source; the compiler copy in an analysis folder | Verified for GC/1.2.5n: https://github.com/Savestate2A03/mwcc_debug (DllMain patches the compiler, writes `pcdump.txt` with nine backend passes per function); its build needs "MSVC x86 cl.exe with a usable Windows SDK" | 1-3 days | a new native toolchain to pin; GC/2.6 addresses differ (cadmic's map covers them) | yes if the toolchain is pinned and hash-checked (MSVC Build Tools is not a hashable zip; a pinned zip toolchain such as Zig's `x86-windows` target is: Inferred, not tried) |
| Hook the compiler: retrowin32 + gdb (cadmic) | Linux or macOS, Rust, gdb | Verified: https://github.com/cadmic/mwcc-debugger ("Installation (Linux or macOS)"; GC/2.6 supported) | n/a | n/a | **no** (Windows only) - its addresses are reused |
| Hook the compiler: Frida / x32dbg | a pinned Frida wheel (pip hash) or x32dbg zip; a script at 0x507D80 / 0x507F50 | Unknown for mwcc; Frida injecting into a 32-bit process from 64-bit Python on Windows is Inferred | 1-2 days | slow per pick (x32dbg logging); WoW64 injection detail | yes (pinned download) |
| decomp-permuter | Python, pycparser, a compile script, a PPC disassembler for its scorer | README: supports PowerPC; "permuter@home ... does not work on native Windows (but WSL does work)" (Verified, README); on schedule-only diffs other projects parked the functions after permuter runs (thug2, Verified in sched_community.md) | 1 day to adapt + unattended hours | finds nothing for a pure schedule question (expected); scorer counts registers that colouring decides | partly: the local mode needs a PowerShell compile script and a scorer adapter (Inferred); nothing else |
| Empirical fitting (step 84's 144 policies) | micro-programs | Verified failure without the right features: at most 23 of 38 words (drafts.md "Eleventh generator" item 4) | spent | only as C8: fit on hook-logged features | yes |
| Community practice: asm and split the file around it | the function as an extracted object or `asm` function | Verified: thug2's "PERMANENTLY PARKED" GX functions, file split, three kept as asm (sched_community.md) | hours per function | does not scale to ~300 handlers; the file's data pool (step 86) forbids a split that cuts its data (see roadblock 6) | yes, as the last fallback |
| mwcc-rs as an oracle | Rust; its oracle needs `wibo` and objdump | Verified: its parity doc (https://github.com/zcanann/mwcc-rs, its docs folder's `parity-status.md`, checkpoint 2026-09-09) still lists scheduling differences and "Exact matching remains **0/4,515 functions**" on one panel; README lists the scheduler as not yet matched | n/a | it is less exact than our replay on our corpus (Inferred) | hypotheses only, not an oracle |

**Choice.** Finish the replay with the hook as ground truth, then the inverse search. Why: it is the only route whose
cost does not grow with the number of functions, it already reproduces 90% of blocks, and step 87 moved the question
from "which priority" to "which source": the inverse search answers that directly, and every handler shares one head
(drafts.md "Eleventh generator" item 2). The hook is chosen over the no-schedule compile because the first pass works
on virtual registers and the pass-1 input of fn_8006EEF8's vertex blocks cannot be reconstructed from outside
(sched_model.md section 6).

**Fallback chain.** (1) C8: log every pick with its fields over the corpus and fit the open rule (scheduler_plan.md
section 6). (2) For the handler head: the permuter with a scorer that calls replay.py on the PCode-level candidate
(cheap, since the replay is instant). (3) The residual functions as `asm` functions inside their C file, tested
first on one run (roadblock 6). (4) Today's state: the run stays dtk's extracted object.

**Done criterion.** replay.py agrees on 100% of corpus blocks (currently 57 of 63) and on every block the hook dumps
for fn_8006EEF8; cr_CM_DUMMY_008 at 100% in batch 12, then the generator rule applied to the head (batch 12: the
~220 handler drafts at 78-85% move); fn_8006EEF8 at 100% and the run fn_8006EEF8..fn_8006FDD0 linked; batch 9
retail SHA-1 and static shift PASS, batch 8 0 FAIL, batch 11 PASS.

**First three steps.**
1. Merge sched-c's hook (or, if it is not the hook, build it): dump the PCode at `schedule_block` and every pick at
   `select_ready_node` 0x507F50 for hd/base.c, fr/v13.c and the 58 corpus files; feed replay.py from the dump instead of
   the no-schedule compile (sched_model.md section 9).
2. Close the six disagreeing blocks (fn_8006EEF8's q segment and vertex segments, m_r1, m_r7) until
   `tools/sched/corpus_report.txt` shows 63 of 63.
3. Write the inverse search in tools/sched (new script): from the original's 38 words of cr_CM_DUMMY_008, enumerate
   the first-pass input orders the replay maps onto them (DAG constraints: which node had to be ready when), allowing
   deleted statements (a dead store, the folded pointer local of step 85 rule 2); then spell one as C and add it to
   draft_batch.py's head rule; run batch 12.

## 2. Typed game data at scale

**Where it stands (Verified: docs/decomp.md "Game data objects").** 14 data-only units. Blocked: the 4-aligned starts
(cr_g_StageSelector, cr_g_Customize, cr_g_FileSlots, cr_g_ScrCmdNames, cr_g_PartOrderBody) need the untyped object
before them; dtk objects that run on into unnamed data (cr_g_CamModeFuncs + 14 floats, cr_g_CharaMotionSets, the SEQ
tables, cr_g_CamPtrs, cr_g_ScrCmdNames / cr_g_ScrCmdFuncs); cr_g_BodyData (34 x 0x78, today `u8[4080]`);
cr_g_PartOrderLists; the string tables (cr_g_SeqNames, cr_g_MotionNames, cr_g_CamModeNames), which need the file's
own string pool.

**What the tools offer (Verified unless marked).**
- dtk's analyser "attempts to identify the type and size of data objects by analyzing usage" and detects "string
  literals, wide string literals, and string tables" (https://github.com/encounter/decomp-toolkit README; config keys
  `detect_objects`, `detect_strings` in tools/refs/dtk-template `config/GAMEID/config.example.yml`). Its sizes are a
  first guess; step 86 shows where they are wrong (cr_g_Pads as three objects, cr_g_FileDescs 0x14 bytes too long).
- symbols.txt carries `size:`, `type:object`, `align:`, `scope:`, `noreloc` (data that looks like pointers) and
  `data:` kinds including `string_table`, `sjis`, `sjis_table`, `float` (https://github.com/encounter/dtk-template
  its docs folder's `symbols.md`): a symbol split is one line per new symbol, and a Shift-JIS name table can be declared as such
  for the disassembly.
- objdiff compares "functions and data" (https://github.com/encounter/objdiff README); batch 9 already requires 100%
  data per C object (Verified: docs/REPRODUCE.md "Verify this checkout").
- m2c `--globals=all` "includes initializers for unreferenced symbols. This can be used to convert data/rodata files
  without decompiling any functions"; its struct-field inference fills `unk_` fields from the code that uses them
  (tools/m2c README, c632946).
- Ghidra's decompiler has "Auto Create Structure" / "Auto Fill in Structure" on a pointer variable (Ghidra help;
  Inferred from the Ghidra 12.1.4 install, not opened in this research).
- String pools: with `-str reuse,readonly` a literal of 8 bytes or less is its own .sdata2 object and longer ones go
  to .rodata in the unit's pool (Verified: docs/drafts.md "Step 83"); a `char *` table's strings must be contiguous and
  used by nothing else to come with a data unit (docs/decomp.md). Melee's guidance on names: "Some strings embedded in
  the binary provide some indication about what some things were named" (https://github.com/doldecomp/melee
  `.github/CONTRIBUTING.md`).

| Method | What it needs | Evidence | Cost | Risk | Fits? |
|---|---|---|---|---|---|
| Types from reader code (base-register scan of step 79, field offsets read by the functions, name_evidence leads) | our Ghidra export, `symbols/cr_types.h`, gen_data_c.py's byte check | Verified here: 14 units built this way (step 86); the gen_data_c check refuses a wrong type | 1 day per family | a type that fits the bytes but not the meaning (padding vs fields); the byte check catches only layout | yes |
| Ghidra auto-create-structure on the readers | the Ghidra project | Inferred (standard feature) | hours | proposes field sizes from one function's use only | yes, as a proposal generator |
| dtk `detect_objects` re-run / its sizes | dtk | Verified as a first guess only (step 86 corrections) | none | wrong sizes where data runs on | yes |
| m2c `--globals=all` initialisers | m2c | Verified feature (README); gen_data_c.py already writes initialisers from bytes and types | none | adds nothing over gen_data_c.py | not needed |
| Symbol split with evidence (SYM_DROP / SYM_TRIM, new `lbl_` with size) | the reader that indexes each part; `symbols/` `why:` line | Verified: cr_g_Pads, cr_g_BattleMotionSets (step 86) | hours per object | a split no code proves; batch 8 cross-checks names, not sizes | yes |
| Opaque bytes for the preceding object (`u8 lbl_X[N]`) | gen_data_c.py support for an untyped array | Inferred (the compiler places any 1-aligned array; the C's next object starts where the bytes end) | hours | data "from C" without meaning; owner decision | yes |
| Strings with their file | roadblock 5/6 (the file's functions in C) | Verified rule: the pool belongs to the file (docs/decomp.md) | with the file | none extra | yes |

**Choice.** Types from the reader code, with symbol splits proven by the code that indexes each part, and Ghidra's
auto-create-structure only to propose a record layout (cr_g_BodyData's 0x78 record) that the byte check then accepts
or refuses. String tables are not forced into data units: they come with their file (roadblock 5/6), except a table
whose strings are contiguous and used by nothing else.

**Fallback chain.** (1) For a 4-aligned start whose preceding object resists typing: define that object as opaque
bytes so the unit starts 8-aligned (needs the owner's yes; it raises "data from C" without types). (2) Leave the object
in dtk with its reason in the unit list.

**Done criterion.** Every blocker of the step 86 list is either a unit in gen_data_c.py's UNITS (batch 9: retail
SHA-1, 100% data, static shift PASS) or has a one-line reason in docs/decomp.md; progress_report.txt's data from C is
above 12.49%; batch 8 0 FAIL; batch 11 PASS; batch 12's 100% count does not fall (the new types reach the drafts).

**First three steps.**
1. cr_g_BodyData: read its readers' offsets (Ghidra export via `tools/find.py`), write the 0x78 record in
   `symbols/cr_types.h`, regenerate with `python -I tools/gen_data_c.py --all`, run batch 9.
2. cr_g_CamModeFuncs and cr_g_CharaMotionSets: find the instruction that reads the 14 floats / the tail, add the new
   symbols to `symbols/` with `why:` lines and the SYM_TRIM size, then the units; batch 9 + batch 8.
3. The 4-aligned five: list the object before each and its readers; type what the readers prove; for the rest put the
   opaque-bytes question to the owner.

## 3. The remaining HSD (sysdolphin) library

**Where it stands (Verified: progress_report.txt; configure.py CR_NOT_MATCHING; splits_report.txt).** 22 of 30
objects linked (42.57% of HSD code); 8 files with register / schedule near-misses (lobj, texp, pobj, texpdag, jobj, cobj,
displayfunc, mtx; step 88 on hsd3 has mtx.c from C, not merged); edges not proven: shadow.c start (neighbour
HSD_Alloc), archive.c end (fn_801A5634), debug.c start (HSD_SaveContext), synth.c end (fn_8017933C), axdriver.c end
(fn_8016FF44), devcom.c start (fn_80170EAC), particle.c start (fn_80179C14); debugconsole_main.c, generator.c, psdisp.c,
psdisptev.c: "none of its functions is (uniquely) named in ours".

**Which projects have the same HSD or close.**

| Source | Revision vs ours | What it gives | Evidence |
|---|---|---|---|
| wowjinxy/KAR (tools/refs/kar_wowjinxy cb94b15) | same revision (KAR PAL build's) | C for every HSD file, equal for many functions; a complete GKYE01 symbol map | Verified: docs/decomp.md "HSD (sysdolphin) notes" |
| doldecomp/kar (tools/refs/kar 3ed51a1) | same revision | exact asm per file: `asm/sysdolphin/` has aobj, axdriver, bytecode, class, cobj, controller, debug, displayfunc, fobj, fog, gobj*, hash, initialize, jobj, lobj, memory, mtx, objalloc, particle, pobj, random, robj, rumble, shadow, spline, tev, texp, texpdag, tobj, util, video `.s` = file boundaries of this revision | Verified: `ls tools/refs/kar/asm/sysdolphin` |
| doldecomp/melee | older revision | **the whole game is now C**: "Super Smash Bros. Melee is 100.00% decompiled", "100.00% fully linked" at commit 598987c, so every Melee HSD file (shadow, archive, debug, devcom, synth, axdriver, particle, psdisp, generator ...) has matching C for its revision | Verified: https://decomp.dev/doldecomp/melee (Oct 31, 2026); our pin tools/melee is 24e685d (tools/pins.txt) |
| TeamOrre/xd-decomp (tools/refs/xd 4989794) | Genius Sonority's C++ fork (`game/maximum/GSAPI/baselib/*.cpp`: 35 files incl. archive, debug, shadow_base) | layout proof only; its HSD build differs (HSD_SetupPEMode 0x4E0 vs 0x174) | Verified: tools/match_lib_hsd.py docstring; `tools/refs/xd/config/GXXE01/splits.txt` |
| Pokemon Colosseum, other HSD games | Unknown (no public decomp found in this search) | - | Unknown |
| HSDLib (Ploaj) | file formats only | struct layouts of data files | Verified: docs/compiler.md "HSD (sysdolphin, HSD audio, particle)" |

**Porting conventions (Verified: docs/decomp.md "HSD (sysdolphin) notes", step 61 iterations 4-9).** Our repo already
has the full recipe: kar headers stay and `include/cr_kar` overrides bridge differences; wowjinxy functions ported per
function with a provenance comment; assert line numbers as literals under `MUST_MATCH`; `static` (not inline) helpers
compiled where defined so their pool constants land in order; dead-stripped functions kept when main.dol's .data has
their strings; main.dol's function order in the file; `int` not `s32` where the original used int. Melee uses the same
`MUST_MATCH` switch for source-variant matching (47 blocks in tools/melee src, Verified by grep).

| Method | Needs | Evidence | Cost | Risk | Fits? |
|---|---|---|---|---|---|
| wowjinxy C per function | the port recipe above | Verified: 22 objects linked this way | hours per function | 88-99% on some (texpdag, bytecode, video callbacks) | yes |
| doldecomp/kar asm transcribed into C | the `.s` of the same revision | Verified: step 88's displayfunc billboards "from the asm (mkVBillBoardMtx equal)" (git log hsd3) | 0.5-1 day per hard function | none beyond effort | yes |
| Melee C (complete) adapted | the revision deltas (one more `*Info` method, inlined accessors, larger functions) | Verified: decomp.dev 100%; docs/decomp.md lists the deltas | hours-days per file | older revision: assert lines, sizes differ | yes |
| Naming the edge neighbours from KAR's per-file asm | masked-byte equality against kar's `.s` (rule "masked instructions equal one C function" of step 61 iteration 9) | Verified rule in docs/decomp.md; the boundaries are in kar's file list | hours | a neighbour that is a header inline's copy (allowed by gen_dtk_splits.py rule) | yes |
| The 8 near-miss files by the replay | roadblock 1 | Inferred: step 84 says allocation follows scheduling, so "register allocation" near-misses are schedule questions (scheduler_plan.md section 1) | with roadblock 1 | some are true colouring questions (declaration order) | yes |
| `asm` functions inside the file | mwcc inline asm (our SDK files already contain 32 such files, Verified by grep of decomp/src) | Verified as compilable in our build (library flags) | hours | at HSD flags the compiler must not touch asm bodies (Inferred) | yes as fallback |

**Choice.** Per function, the first source that compiles equal, in the order wowjinxy/KAR -> kar asm transcribed ->
Melee's (now complete) C adapted -> by hand; edges proven by naming each neighbour by masked-byte equality against
doldecomp/kar's per-file asm; the 8 near-miss files wait for the replay. Why: the same-revision sources come first
because they carry this revision's sizes and asserts; Melee's completion means no HSD function lacks some C, which
shrinks "no reference" (41 functions in docs/compiler.md "What remains unknown") to "older revision to adapt".

**Fallback chain.** (1) A function that resists: an `asm` function in the C file, so the rest links (needs the
roadblock 6 test first). (2) The file stays in CR_NOT_MATCHING with its reason (today).

**Done criterion.** progress_report.txt: sysdolphin 30 of 30 objects from C; splits_report.txt: no `GALE01:sysdolphin`
"not proven" line for shadow, archive, debug, synth, axdriver, devcom, particle; batch 9 retail SHA-1 and static
shift PASS, batch 8 0 FAIL, batch 11 PASS.

**First three steps.**
1. Merge hsd3 (step 88) after its checks; re-read splits_report.txt.
2. For each edge neighbour (fn_801A5634, fn_8017933C, fn_8016FF44, fn_80170EAC, fn_80179C14): compare its masked
   bytes with the first / last function of the matching doldecomp/kar `.s` file; name it in `symbols/symbols.txt` "HSD
   object edges" with a `why:` line; batch 8 then batch 9.
3. Bump the Melee pin from 24e685d to the 100% commit (598987c or later) in tools/pins.txt and re-run the step 61
   source hunt (`tools/match_lib_hsd.py`) over the newly-C files (archive, shadow, debug, devcom, synth, axdriver,
   particle family) to list per function the closest C.

## 4. The m2c draft pipeline's limits

**Where it stands (Verified: docs/drafts.md "Step 85" and the run table).** 45 drafts do not compile (m2c typing:
`**x` chains, struct / float to pointer casts, `void *` arithmetic); 213 drafts at 80-99%; ~220 of them are the
handler family behind roadblock 1; ~198 have the gap-register pattern of step 85 rule 2. The repo's rule: never edit a
draft, every fix goes into the generator or the types.

| Method | Needs | Evidence | Cost | Risk | Fits? |
|---|---|---|---|---|---|
| Generator post-passes for the three m2c typing patterns + type fixes | draft_batch.py rules; `symbols/cr_types.h` | Verified here: fold_temp_compound, store_literals, script_counter_ptr (steps 84-85) each moved many drafts | 0.5 day per pattern | a rule that compiles but mistypes; batch 12's full run shows regressions | yes |
| m2c `--valid-syntax` | m2c flag | Verified: "likely to directly compile without edits ... go directly from assembly to the permuter" (tools/m2c README) | none | output less readable (M2C macros); changes every draft | yes, for the 45 only |
| Diff-class triage (frame-only / register-only / slot / structural) | our compare + the replay | Verified elsewhere: decomp-triage's buckets on Sunshine (frame-only ~600, regperm 20, structural ~350; it recommends structural first) | 1 day to build on draft_batch.py's compare | none | yes (its tools are stdlib Python; we would write our own on our compare) |
| decomp-permuter on register-only | see roadblock 1 | Verified: "best towards the end, when mostly regalloc changes remain" (README) | 1 day adapt, unattended runs | random "nonsensical" changes (README) to clean by hand | partly (Windows adaptation) |
| Ghidra's typed C as a second draft | our ExportDecompiledC.java output | Inferred: Ghidra's C has our types but not compilable without a cleanup pass | 1-2 days for a cleanup | two generators to maintain | yes |
| decomp.me scratches | the web site; uploading game asm | Unknown for GC/2.6 at `-opt level=0` (the site's API refused this research's fetch) | per function | public upload of game code; owner decision | doubtful (Windows fine, publication question) |

**How big projects run bulk matching (Verified unless marked).** Melee: per-function work by many contributors through
decomp.me links and objdiff, maintainers merging (https://github.com/doldecomp/melee README and CONTRIBUTING);
progress tracked by objdiff reports on decomp.dev (whose main index now says "adding new projects are disabled",
https://decomp.dev/projects, Oct 31, 2026); the permuter as a complement, not a replacement (permuter README). No
project found that runs a whole-game m2c-compile-compare loop like batch 12 (Unknown elsewhere).

**Choice.** Generator post-passes plus type fixes for the 45, then a diff-class triage of the 213 with the replay as
the schedule test, hand matches on the structural class. Why: it is the method that already moved hundreds of drafts
at once (steps 80-85) and keeps the no-hand-edit rule; the triage keeps work away from the schedule-only class until
roadblock 1 lands.

**Fallback chain.** (1) The permuter on the register-only class (adapted, unattended). (2) Ghidra's C for a failure
m2c cannot type. (3) Hand C per function in a scratch harness (steps 83-85 method).

**Done criterion.** draft_report.txt: compile failures 45 -> 0, or each with its reason; every 80-99% draft carries a
class; batch 12 100% above 483 with no draft down; batch 9 PASS after promotion.

**First three steps.**
1. Group the 45 failures by compiler error in `decomp/drafts/draft_report.txt` and write one post-pass per pattern
   in tools/draft_batch.py (`void *` arithmetic -> `(u8 *)` cast; `**x` chains -> typed pointer locals; float / struct
   to pointer -> the field's type from cr_types.h); `--only NAME` on each, then a full batch 12.
2. Add the class column to the compare (register-only = equal with registers masked; schedule-only = equal as a
   multiset per block; frame-only; structural) and print the counts in draft_summary.txt.
3. Work the structural class from the top of the size-weighted list; send the schedule-only list to roadblock 1.

## 5. Splitting the 229 automatic dtk objects

**Where it stands (Verified: splits_report.txt).** 229 unproven code ranges, 1,517 functions; game code splits so far
come from promote_drafts.py's file runs (per-file pool rule) and from C files.

**Evidence available (Verified unless marked).**
- dtk resolves the link order by "a topological sort" of the split objects (decomp-toolkit README) and accepts an
  `order:` attribute for ambiguous cases (dtk-template its docs folder's `splits.md`): a wrong split that breaks the order shows at
  configure time.
- Per-file pools: constants and strings of a file are laid out in link order (gen_dtk_splits.py's per-file pool rule,
  docs/drafts.md "File objects").
- The data-pool base: a file addresses its own data from one base, other files' data by symbol (step 86: 62 functions
  address 0x803B8C00.. from one base, 280 others name cr_g_Script) - the 62 are one file (docs/decomp.md).
- Every section of a compiled object is 8-aligned (`-align powerpc`, step 86): zero padding to an 8-byte boundary in
  .data / .rodata / .sdata2 marks a possible file end (a file can also end 8-aligned without padding).
- .bss / .sbss are laid out in reverse declaration order, .data / .sdata / .rodata / .sdata2 in declaration order
  (step 86): an order inversion across two objects marks two files.
- With `-str reuse` identical literals are merged within a file; the same string twice in .rodata means two files
  (Inferred: standard mwcc behaviour, not measured here).
- Reference decomps: no other game shares Noise's game code (Inferred), so file lists help only for libraries.

| Method | Needs | Evidence | Cost | Risk | Fits? |
|---|---|---|---|---|---|
| Per-file pool rule (in use) | promote_drafts.py | Verified: runs in the step 85 table | none | runs grow to 1,739 functions where a pool entry is shared widely (the big run) | yes |
| Data-pool base rule | reloc_audit.py glue spans | Verified: 89 spans, the 62-function span | 1 day to turn into a split rule | a base shared by two files (Unknown whether it happens) | yes |
| Padding / alignment rule | main.dol bytes | Verified rule for compiled objects | 1 day | false boundaries at naturally 8-aligned ends: needs a second rule | yes |
| Duplicate-literal rule | the .rodata string list | Inferred | 0.5 day | a literal written differently in two places | yes |
| dtk's own analysis / auto split | dtk | Verified: these 229 are what it left | none | none | yes (already used) |
| Splits by hand from reading the code | Ghidra | Inferred | days | unproven boundaries | only with a rule as proof |

**Risks for a shiftable build (Verified: docs/decomp.md "Game data objects").** A wrong code split still links
byte-identical (order is kept); the risk is a split that cuts a file's data pool: inside a glue span the unit can only
move with its file, and shift_test.py refuses insertion points there. Each new split must keep batch 9's static shift
PASS and batch 11 PASS.

**Choice.** Extend gen_dtk_splits.py with the data-pool base rule, the padding rule and the duplicate-literal rule,
and accept a game split only when two independent rules agree (or one rule plus a matching C file). Why: every rule is
measurable on main.dol alone, needs no new tool, and the two-rule acceptance mirrors how the library splits were proven
(several references per edge in splits_report.txt).

**Fallback chain.** (1) One rule plus a reader-evidence `why:` line, flagged as weaker in the report. (2) Leave the
range automatic (byte-identical and shiftable through dtk's relocations).

**Done criterion.** splits_report.txt "unproven code ranges" below 229, each new split with its rules listed; batch 9
retail SHA-1 and static shift PASS; batch 11 PASS; promote_report.txt's runs recomputed (smaller runs where a boundary
cuts the big one).

**First three steps.**
1. Turn reloc_audit.py's glue spans into a "functions addressing one base" list; check each list is contiguous in .text.
2. Scan .data / .rodata / .sdata2 for 8-alignment zero padding and .rodata for duplicate strings; print both next to
   the pool rule's boundaries in splits_report.txt.
3. Accept the boundaries where two rules agree; batch 9, batch 11.

## 6. One-function objects into source files, data with its file

**How dtk-template projects structure it (Verified).** One `Object(...)` per source file in configure.py with a
status (Melee's README asks for new files "marked `Equivalent`"; `metadata.complete` marks a unit linked in objdiff:
https://github.com/encounter/objdiff README); "For files that are not yet complete, the build uses the corresponding
machine code extracted from the user's original ... executable" (https://github.com/doldecomp/melee/wiki/FAQ). So a
file is linked from C only when every function matches; a partial file is compiled for objdiff and the extracted
object is linked. Link order comes from dtk's topological sort with `order:` hints (dtk-template its docs folder's `splits.md`).
Melee reached 100% linked this way (decomp.dev), with source tricks under `MUST_MATCH` rather than asm parking.

**Our constraint (Verified: docs/decomp.md "Game data objects").** A file's data must live in the file with its code:
compiled, the pool comes back as base + offsets. So splitting a file around an unmatched function (the thug2 practice)
works only when that function touches no file-pool data.

| Method | Needs | Evidence | Cost | Risk | Fits? |
|---|---|---|---|---|---|
| File objects run by run (promote_drafts.py, in use) | every function of the run at 100% | Verified: 4 file objects (docs/drafts.md "File objects") | per run | blocked by roadblock 1 | yes |
| The run stays extracted until complete (dtk-template NonMatching) | nothing | Verified: dtk-template / Melee practice | none | none: today's state | yes |
| `asm` function inside the C file for the residual function | mwcc inline asm with relocations to the file's data | Verified that inline asm compiles and links in our build (32 SDK / MSL files in decomp/src); Unknown at game flags and against a file's anonymous pool labels (an asm body cannot name `@123`; it must address a named object + offset) | 1 day for the test | if the pool base has no name the asm cannot reach it shiftably | to prove |
| Split the file around the function | a boundary inside the run | Verified: thug2 practice (sched_community.md) | hours | cuts the data pool (forbidden where the function uses it) | only without pool use |
| `.s` file units for the residual | an assembler step | Inferred: dtk-template's `comment:0` note warns that `mwld` crashes on assembled files with only label symbols (dtk-template its docs folder's `splits.md`) | days | toolchain change | no |

**Choice.** Keep the dtk-template model: the run's file object is linked only when complete, its data defined in the
file; prove once whether an `asm` function inside the C file keeps the file byte-identical and shiftable, and use that
for residuals only if the test passes. Why: it is the standard model, needs no new tool, and the data-pool rule makes
any split around a function risky.

**Fallback chain.** (1) The run stays as dtk's extracted object until the last function matches. (2) A split around
the function only where it uses no file-pool data.

**Done criterion.** fn_8006EEF8..fn_8006FDD0 (7 functions, the first run with its own data) linked as one C file with
its data; promote_report.txt shows it as a FILE OBJECT; batch 9 retail SHA-1 and static shift PASS (the shift test pads
at the file's start), batch 11 PASS. Then the same per closed run.

**First three steps.**
1. The asm test on a scratch copy: the 7-function run with fn_8006EEF8 as an `asm` function from the dtk asm,
   compiled with the game flags; compare bytes and relocations with main.elf (tools/draft_batch.py's compare,
   `dtk elf disasm`); record whether the pool can be reached by a named symbol.
2. Write the outcome into docs/drafts.md "File objects" (rule or "not usable").
3. When roadblock 1 closes fn_8006EEF8: `python -I tools/promote_drafts.py --check --try-run fn_8006EEF8 fn_8006FDD0`,
   then batch 12 + batch 9 + batch 11.

## 7. Reproducibility and script consolidation (fresh clone, Windows)

**What comparable projects do (Verified).** dtk-template: `python configure.py` then `ninja`; tools fetched by
`tools/download_tool.py` with a required `--tag` and **no hash check** ("never computes or compares a checksum",
read in https://raw.githubusercontent.com/encounter/dtk-template/main/tools/download_tool.py); the DOL hash in
`config.yml` (`hash:`) and `build.sha1` from `dtk shasum`. CI: a private build container repo holding the game files
(dtk-template its docs folder's `github_actions.md`: the build repository "should be **private**"). Melee on Windows: "highly
recommended to use native tooling", Python and ninja only (Melee README).

**Ours (Verified: docs/REPRODUCE.md, tools/pins.txt).** `pwsh tools\from_scratch.ps1 -Iso ...` runs setup (every
download pinned by commit or SHA-256 in tools/pins.txt, stricter than the template), run_all (batches 1-12) and the
checksum compare; PASS in 41 minutes on Oct 31, 2026. The build itself is the template's configure.py + ninja,
patched (tools/patches). The short check (batch 9 + batch 8 + checksums) is the local equivalent of CI.

**Redundancy found (Verified by reading the files).**
- Two copies of the pinned-download routine: tools/setup.ps1 (lines 42-70) and tools/setup_decomp.ps1 (lines 29-31),
  same SHA-256 check against tools/pins.txt.
- A `.sh` twin for every `.ps1` (2,651 lines of scripts in tools/*.ps1, tools/*.sh, tools/batches): REPRODUCE.md
  says the Linux / macOS notes are "not maintained at the moment", and the project rule is Windows only.
- tools/boot_check.ps1 (warning-dialog check) and tools/boot_test.ps1 (screenshot) both start Dolphin on a build;
  batch 11 uses one of them (Inferred: one Dolphin launcher with two modes would do).

| Method | Needs | Evidence | Cost | Risk | Fits? |
|---|---|---|---|---|---|
| Keep from_scratch.ps1 + run_all.ps1, one download routine | a shared module | Verified: PASS Oct 31, 2026 | 1 day | none | yes |
| dtk-template-only (configure.py + ninja for everything) | moving the naming / Ghidra batches into ninja | Inferred: Ghidra and naming are not build steps | weeks | loses the batch structure | no |
| Public CI (GitHub-hosted) | game files in a private container | Verified: dtk-template's method | days | the ISO leaves the owner's machine; Linux container | **no** (Windows only, game files) |
| Self-hosted Windows runner | the owner's desktop as runner | Inferred | 1 day | the machine is shared with the batches | owner decision |
| Short check by hand at each merge (today) | nothing | Verified: docs/workflows.md section 14 | none | human step | yes |

**Choice.** Keep the one entry point (from_scratch.ps1) with configure.py + ninja inside batch 9; merge the two
download routines; decide the `.sh` twins and CI with the owner; run the fresh-clone test last, after the other
roadblocks change the scripts.

**Fallback chain.** (1) A self-hosted Windows runner running the short check. (2) No CI: the short check at every
merge (today).

**Done criterion.** From an empty folder on Windows: `pwsh tools\from_scratch.ps1 -Iso ...` PASS with every checksum
of docs/expected_checksums.txt matching; one download routine; REPRODUCE.md names one entry point; check_docs 0
problems.

**First three steps.**
1. Move the pinned-download function into one module dot-sourced by setup.ps1 and setup_decomp.ps1.
2. With the owner's answer on the `.sh` twins: delete them or mark them frozen in REPRODUCE.md and docs/tools.md.
3. Fresh clone into a new folder and run from_scratch.ps1; record the time and table in REPRODUCE.md.

## 8. More ways to split, and the chunks with many unknowns (research, Nov 1, 2026)

Written after steps 97-99 (main 51fb1e7). Measured on main.dol with throwaway scripts unless marked; the numbers can be
recomputed from `decomp/config/GXCE01/symbols.txt` and main.dol.

**What did not work (Verified, step 98).** wowjinxy/KAR's C files cannot prove file ranges: its memory.c defines
shadow.c's functions, its tev.c state.c's, its texpdag.c texp.c's. A generator rule that trusted them was tried and
reverted. doldecomp/kar's asm files are only as good as kar's own splits: its random.s starts with HSD_ArchiveFree,
and text_804452A0.s is an automatic split. So kar's asm counts as evidence only at a file's first and last function.

**New split evidence.**

- **Float-pool restart (Verified, measured).** mwcc pools a file's floating-point constants in .sdata2 once per value
  (Inferred: standard mwcc behaviour, which the per-file pool rule already relies on). So when a value reappears,
  another file has started somewhere after its previous copy. Measured: 1,707 4- or 8-byte .sdata2 objects; 0.0f
  appears 78 times, 1.0f 62, 0.5f 32, -1.0f 26, 2.0f 24. Cutting greedily at the first repeat gives at least **99**
  pools, against 60 .sdata2 pieces in splits.txt today. Each repeat proves a boundary inside an interval (between the
  two copies), not at a point. The padding rule (section 5) or a pool user's code edge then pins it. The code that
  loads each pool entry follows its pool, which turns the .sdata2 intervals into .text intervals.
- **Assert and file-name strings not yet anchors (Verified: strings in main.dol).** main.dol holds 53 `.c` / `.h` names.
  Besides the HSD files these are game and middleware files: dvd.c, dvdfs.c, dvdsche.c, background.c,
  seq_movie.c, sprite.c, vi.c, pslist.c, psinterpret.c, generator.c, psdisp.c. The G3 notes (symbols/game_real.txt,
  220 lines) place the functions that pass them. A run of functions between two G3 notes of the same file is that
  file's. dvdsche.c is a concrete lead for devcom.c's end: wowjinxy's devcom.c ends with kar_dvdsche_* functions, and
  main.dol has a separate dvdsche.c name. "memory.c" is a lead for memory.c's real start (_HSD_Mem*).
- **Byte-identical functions (Verified, measured; weak).** 10 groups of exactly equal functions (unmasked, at least
  0x10 bytes): two copies of one header inline, or two files' identical statics, which would be two files. But one
  group is 124 copies of a script handler that come from one generated file, so this needs a second rule. With
  relocations masked there are 81 groups, mostly handler families: no use as split evidence.
- **Call-graph locality (Inferred).** A function called only from neighbours inside one range is probably a `static`
  of that file. A function called from many distant ranges is probably global API. Weak on its own, useful as the
  second rule.
- **Runtime trace (Inferred).** Dolphin's debugger can log the functions that run per scene (title, customize, battle,
  story) using a symbol map exported from symbols.txt. Functions that only ever run in one scene group by feature.
  This helps naming more than splitting. Desktop only (no cross-system testing).
- **Ghidra BSim (Verified as a tool; Inferred for us).** It compares decompiler feature vectors and ignores constants
  and register names. Ghidra documents it as working across compilers and architectures; small functions match with
  low confidence. Possible use: match HSD / particle functions against Melee's and KAR's binaries when C does not
  compile equal. No other game shares Noise's game code (the DS Custom Robo Arena is ARM and another codebase), so it
  does not help the game code.
- **dtk / ppcdis (Verified: their docs).** Neither documents a boundary heuristic beyond relocations, .ctors / extab
  pairing and user configuration. Map files and asserts remain the community's sources.

**The chunks with many unknowns, and the best way to each.**

| Chunk | Unknowns | Best method (measured where possible) | Then |
|---|---|---|---|
| Particle family 0x80179C14-0x80186490 (particle, generator, psdisp, psdisptev, psappsrt, pslist; 68 functions, 1 named) | file edges, names | Compile Melee 598987cf's and wowjinxy's particle-family C with our flags and compare (as step 98): **15 of 68 exact** (e.g. psClearPointJObj, psGetNewIDNum, psSetupTevCommon, psAttachGeneratorAppSRT, _psListDelete; 7 named only kar_particle_* in wowjinxy) and 13 more at 90% or above. Name the exact ones (batch 8 size check), then the edges fall to the existing rules | port per function, as HSD |
| HSD edges left (axdriver end, devcom start, particle end, random start) | 1 small function each | devcom / axdriver: fn_80170EAC calls synth's counter, KAR has no such function, so read its caller (0x80098EDC). random: archive.c's tail by its .data / .bss order (docs/decomp.md step 98) | name or prove by data order |
| shadow.c (5 of 16 not equal) | 4 near misses + fn_801A4998 | Melee shadow.c is compiled in branch hsd-shadow; take wowjinxy's version per function where it scores higher (StartRender 0.915) | objdiff per function |
| The 232 unproven code ranges (dtk automatic objects) | boundaries | Float-pool restart + padding (two rules agree), G3 runs, the per-file pool rule | section 5's done criterion |
| Typed data left (step 90 list) | layouts | Base-register scan (in use); community layouts: robo part records at 0x801C6C64 with stride 0x134 (seiken's Dolphin Memory Engine map), movement stats as reduced / unmodified / boosted triples that leg values 1 / 100 / 200 pick and interpolate, panzer_maya's v2.py parts reader, DesiacX's CRBR-Parts-Expansion | gen_data_c.py units |
| Unnamed game functions | names | Typed globals (reader rule); community code addresses as anchors: 0x80079DFC / 0x80079ED0 draw the damage / combo counters, 0x800466A4 sets the tag invulnerability frames, frame counter 0x8044F2A2, P1-in-egg 0x803872B2, song id 0x803B7682, stage id 0x803756A2 (agrees with symbols/unverified.txt); the scene trace | batch 8 |

**Choice.** First the particle family's exact names: they are measured, cheap and unblock edges. Then the float-pool
restart as a reported rule in splits_report.txt, accepted only where it agrees with padding or a G3 run. Community
addresses are leads to be verified in the code before any name.

**Done criterion.** particle family: the exact matches named (batch 8 0 FAIL) and at least one of its objects split;
splits_report.txt prints the pool-restart intervals; the 232 unproven code ranges fall; batch 9 retail SHA-1 + static shift PASS,
batch 11 PASS.

**First three steps.**
1. Name the 15 exact particle-family functions, each with a `why:` line (compiled C equal); batch 8, batch 9. **Done, step 100**: 12 named (3 have only decomp placeholder names, one name is taken), psSetupTev added, psdisptev.c split (docs/decomp.md "Particle family names (step 100)").
2. Add the float-pool restart intervals to gen_dtk_splits.py's report (report only), next to the padding candidates. **Done, step 101**: 48 of 51 testable intervals agree with proven edges, 3 false positives (SDK code), 47 new (docs/decomp.md "Float-pool restart intervals (step 101)").
3. Read 0x80098EDC (fn_80170EAC's caller) and the archive.c tail's data order, to close two HSD edges. **Done, step 102**: fn_80170EAC has no reference to place it; archive.c runs to 0x801A5AE8 by its string pool, closing it needs the fn_801A5708 port (0.808, docs/decomp.md "The two HSD edges by caller and data order (step 102)").

## What to decide

1. **A native toolchain for the compiler hook (roadblock 1).** The proxy DLL needs a 32-bit C compiler: MSVC Build
   Tools (what mwcc_debug uses; installer, not a hashable zip) or a pinned zip toolchain (Zig, clang); or Frida from a
   pinned wheel instead of a DLL. Which one may enter tools/pins.txt?
2. **`asm` functions in final C files (roadblocks 1, 3, 6).** Does a C file with one function written as inline asm
   count as "from C" for the super goal, or only as an interim state?
3. **Opaque data definitions (roadblock 2).** May the object before a 4-aligned start be defined as untyped bytes to
   let the typed unit link?
4. **The `.sh` twins and the Linux notes (roadblock 7).** Delete, freeze, or keep maintaining?
5. **CI (roadblock 7).** A self-hosted runner on the desktop (the ISO stays local), or the short check by hand?
6. **Public tools that publish code (roadblock 4).** May game asm be uploaded to decomp.me scratches?
7. **The Melee pin (roadblock 3).** Move tools/pins.txt's Melee commit from 24e685d to the 100% commit?

## Sources

Repo files (all read on Oct 31, 2026, main 798493c): plan.md ("Fresh session: start here", "Resume here"),
[scheduler_plan.md](scheduler_plan.md), [sched_community.md](sched_community.md), [sched_model.md](sched_model.md),
[decomp.md](decomp.md), [drafts.md](drafts.md), [compiler.md](compiler.md), [REPRODUCE.md](REPRODUCE.md),
[tools.md](tools.md), decomp/configure.py (CR_NOT_MATCHING), decomp/reports/progress_report.txt,
decomp/reports/splits_report.txt, tools/pins.txt, tools/setup.ps1, tools/setup_decomp.ps1, tools/match_lib_hsd.py,
tools/m2c/README.md (c632946), tools/melee (24e685d) `src`, tools/refs/kar `asm/sysdolphin`, tools/refs/xd
`config/GXXE01`, tools/refs/kar_wowjinxy `config/GKYE01`, tools/refs/dtk-template (d366876) `docs/` and
`config/GAMEID/config.example.yml`.

Web (fetched Oct 31, 2026):
- https://github.com/simonlindholm/decomp-permuter (README: PowerPC support, Windows note, FAQ on regalloc)
- https://github.com/encounter/dtk-template (its docs folder's `getting_started.md`, its docs folder's `splits.md`, its docs folder's `symbols.md`,
  its docs folder's `github_actions.md`) and https://raw.githubusercontent.com/encounter/dtk-template/main/tools/download_tool.py
- https://github.com/encounter/decomp-toolkit (README: analysis, link order)
- https://github.com/encounter/objdiff (README: data diff, objdiff.json units)
- https://github.com/doldecomp/melee (README), https://raw.githubusercontent.com/doldecomp/melee/master/.github/CONTRIBUTING.md,
  https://github.com/doldecomp/melee/wiki/FAQ
- https://decomp.dev/doldecomp/melee (100.00% decompiled, fully linked, commit 598987c), https://decomp.dev/projects
- https://github.com/doldecomp/kar (make-based build, no progress figure shown)
- https://github.com/zcanann/mwcc-rs and its its docs folder's `parity-status.md` (checkpoint 2026-09-09)
- https://github.com/cadmic/mwcc-debugger (README: Linux / macOS, GC/2.6)
- https://github.com/Savestate2A03/mwcc_debug (README: proxy lmgr326b.dll, MSVC build)
- https://github.com/KakarottoCake/decomp-triage (README: buckets, levers)
- Through docs/sched_community.md (not re-fetched): https://github.com/rayanht/mwcc, https://github.com/inspiredrobot/mwcc,
  https://github.com/Batenkaitos-dev/mh2-decomp, https://github.com/Limero/thug2
- Fetched Nov 1, 2026 (section 8): https://raw.githubusercontent.com/encounter/dtk-template/main/docs/splits.md,
  https://raw.githubusercontent.com/encounter/decomp-toolkit/main/docs/other_approaches.md, https://github.com/SeekyCt/ppcdis,
  https://ghidra.re/ghidra_docs/GhidraClass/BSim/BSimTutorial_Intro.html,
  https://www.pentestpartners.com/security-blog/fuzzy-matching-with-ghidra-bsim-a-guide/; community notes the owner passed on
  (Discord, Nov 1, 2026: seiken's Dolphin Memory Engine map, DesiacX's codes, https://github.com/DesiacX/CRBR-Parts-Expansion,
  https://gitlab.com/panzer_maya/robo/-/blob/master/v2.py).
- Not reachable: decomp.me's site and API (HTTP 403), decomp.dev/doldecomp/kar (404).
