# Pooled drafts

Functions that match only when their file's data is **defined** in the same translation unit.

GC/2.6 addresses the globals a file defines through one base register per function (`lis r3, sym@ha; addi r31, r3,
sym@l`, then `N(r31)`): the file's pooled data. m2c prints that base as a local (`base_lbl_80329B60 = lbl_80329B60;`)
with byte offsets, and a draft compiled with only `extern` declarations cannot reproduce two things the pool gives
the original: the alias classes (two objects of the pool never overlap, so the scheduler moves loads above stores)
and struct copies between named objects. 92 open drafts have such a base.

A file here defines the objects (a pad up to the first one used, then each object with its type), references them
in address order in a static `pool_order()` (the compiler lays the pool out in the order of first reference, which
in the real file comes from the earlier functions), and then holds the function. It is scored like any draft:

    python -I tools/sched/scratch/hand/hd.py decomp/drafts FUNC tools/pooled_drafts/FUNC.c

It is **not** a hand draft: its relocations name this object's own `.bss`, not the game's symbol, so
`relcheck.py` rejects it, `draft_batch.py` does not read this folder, and nothing here is linked. These files are
the evidence, and the starting point for the file objects (`decomp/src/game/<first>_<last>.c`) that will carry the
data and all functions of a file together. See [docs/matching_research.md](../../docs/matching_research.md) section 2.

| Function | Words | What the pool changed |
|---|---|---|
| `fn_800456B8` | 27 of 27 | two 12-byte struct copies between three vectors of the file's `.bss` (was 70% as a pointer draft) |
