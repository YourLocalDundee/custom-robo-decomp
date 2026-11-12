# Battle action file drafts (the 0x80329B60 pool)

The battle robo code (at least fn_8003FE98 .. fn_8005098C: the part updaters, cr_BattleRobo_UpdateAction and its 23
action handlers) is one source file whose .bss is the object dtk calls `lbl_80329B60` (0xA84 bytes). That object is
the file's **static variables**: a 0xA0C-byte block, then ten Vecs at +0xA0C .. +0xA78 (the view vectors). MWCC
addresses a file's statics from one base register (the pool start in r30 / r31, loaded at the function entry), and
copies between two of them load all three words before storing (a struct copy within one object goes in pairs, so
`lbl_80329B60` as an extern array can never give this code). MWCC lays the statics out **in the order of their first
reference in the file**, not in declaration order (tested with tools/sched/scratch/hand/cc1.py).

So these functions match only inside a file object that defines the pool. A draft here is the function alone;
`pool.inc` declares the statics (`cr_s_9B60_<offset>`) and `cr_Pool9B60_Order`, a scoring-only function that
references them in address order first so that every static gets the original's offset.

    python -I tools/file_drafts/battle_9B60/score.py fn_80045724

compiles pool.inc + the draft and runs hd.py and relcheck.py (which maps our `...bss.0` to `lbl_80329B60`).
When every function of the file matches, the file object is the drafts in address order with the statics declared
once (and no cr_Pool9B60_Order: the real first references then have to give the same layout; check it then).
