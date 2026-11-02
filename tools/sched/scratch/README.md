# Scheduler-wall scratch harnesses (saved Nov 2, 2026)

The working files the step 84 / 85 workers built in their session scratchpads, saved here so the next session
(docs/scheduler_plan.md) can reuse them. Scripts were written with absolute paths to the worktrees of that session
(D:\emulators\gameTesting-hsd2, D:\emulators\gameTesting-hsd) and to a scratchpad folder that no longer exists:
fix the paths at the top of each script before running. Object files (.o, .o.s) and the large output folders were
not copied; the scripts regenerate them.

| Folder | From | What |
|---|---|---|
| hd/ | step 84 (handler head) | pr.py: compile one C file with the game flags, dtk disasm, word-aligned score, side-by-side diff; runall.py / runall_v.py: sweep a folder of .c; sweep_orders.py: the 120 statement orders x counter-pointer positions (sw_cnt, sw_none); sched_sim.py + fit.py: the 144 list-scheduler policies fitted against the corpus; m/*.c: 62 micro-programs on the scheduler's dependence model; base.c, v0.c, so.c, so2.c: the pure handler head shapes (cr_CM_DUMMY_008); dis.py: disasm helper |
| hm/ | step 83 (fn_8006EEF8, first attempt, 73%) | hm.py: harness; g1..g9.c, h*.c, k*.c, m*.c, p*.c: the shapes; dprobe.c: register-allocation micro-tests; g2.c: the best of that round |
| fr/ | step 85 (fn_8006EEF8 72.6%, frame exact; fold_temp_compound, store_literals) | cc.py / sc.py / md.py: compile, score, micro-tests; ppsweep*.py: gap-pointer placement sweeps; v13.c: the best fn_8006EEF8 C (frame and registers exact); fn_80080A94*.c / fn_80080B80*.c with .diff: the two near-misses that became generator rules; ctx*.h: the draft context headers used; draft_report_before.txt / draft_summary_before.txt: the step 84 baseline |
| ar/ | step 102 (archive.c's loader fn_801A5708, 0.808) | archive_v1.c: decomp/src archive.c plus fn_801A5708 and HSD_ArchiveFree (equal); arch_cmp.py FILE: compile with the HSD flags and score every function against main.dol; arch_diff.py FILE FUNCTION: instruction diff (relocated fields masked). Both find the repository from their own path |
| sh/ | steps 103-104 (shadow.c) | sh_diff.py REPO FILE FUNCTION [-q]: compile FILE with shadow.c's flags (include/kar_shadow), score and diff one function against main.dol; sh_data.py REPO FILE: its .data / .sdata / .sdata2 / .bss against main.dol's shadow.c ranges |
| base/ | step 82 | draft_report_baseline.txt and draft_summary.txt: the step 81 per-function baseline |

hd/pr.py finds the repository from its own path (fixed for step 87); the other scripts still carry the old paths.

How a score is computed (pr.py): compile with `mwcceppc.exe` GC/2.6 and configure.py's game flags, disassemble the
object with dtk, align the words with the original's (from decomp/build/GXCE01/asm), count equal words by
position; a second count masks the register fields.
