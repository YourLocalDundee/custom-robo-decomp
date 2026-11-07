# Hand-draft helpers (Nov 3, 2026)

Scratch tools of the main session's hand drafting (docs/drafts.md, the Nov 3 sections). Run from the repo root.

- `hd.py OUTDIR FUNC DRAFT.c [-w lo hi]`: compile a hand draft like draft_batch.py and compare with the original
  (OUTDIR = a draft_batch --out folder that drafted FUNC, e.g. decomp/build/draftscan); percent + diff, `-w` a
  side-by-side window.
- `hdvar.py OUTDIR FUNC SPEC.py`: variants; SPEC has `BASE` (text with `{SLOT}`s) and `V = {name: {SLOT: text}}`;
  outputs land in `hdvar_out/` next to the script. Read every line: do not sort the output as text ("100%" sorts
  below "9x%").
- `mtxconv.py FUNC SRC.c OUT.py`: m2c's stack struct (f32 triples, `f32 spX[4]` + 32 pad bytes) to Vec / Mtx
  locals, writes an hdvar spec with the four declaration orders.
- `scores.py OUTDIR`: the match % of every compiled draft in OUTDIR/obj (the near-miss work list).
- `cc1.py SRC.c`: compile a small test file with the game flags and objdump it (env OPT=level=..., VER=GC/x.y).
- `perm/run_queue2.sh`: the next permuter queue (decomp-permuter via tools/sched/permuter_setup.py, 8 workers,
  MINUTES each, then the HSD near misses through hsd_permute.sh). Sources are the wrapped drafts in `perm/`.
  Apply a score-0 output: check it with hd.py, then put the function in tools/hand_drafts with a header comment.
- `wip/`: the unfinished hand drafts (fn_800057B8 99%, fn_8000860C 93%, fn_8006EEF8 74%,
  cr_BattleRobo_InitFromLoadout ~50%, fn_80080C7C 83%).
- `relcheck.py OUTDIR FUNC DRAFT.c`: the relocation symbol at every instruction of our compile against the
  original .s (hd.py masks relocated fields: a draft that reorders stores can match word for word with the symbols
  swapped). Our literal constants (`@729`) are accepted when they map one to one onto the original's pooled ones.
- `hd.py ... -r`: our object's disassembly with relocations.
- `permdiff.py FUNC [OUTPUT]`: what decomp-permuter changed (statement diff of base.c and the best output).
- `structscore.py [OUTDIR]`: three scores per draft (exact, noreg = register fields masked, ops = opcodes only),
  OUTDIR/structscore.txt sorted by size; finds the functions whose structure is right and only registers / operands
  differ (Nov 4: 334 unmatched functions, 243 KB, at ops >= 85%; 214, 136 KB, at noreg >= 85%).
- `hsdparts.py FILE.dat ...`: per root joint of a standard HSD .dat (models_dat/): joints, DObjs, PObj kinds
  (plain / shape animation / envelope), hidden and moved joints (docs/formats/model_entry.md, "Part files").
