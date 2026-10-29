# Game-code drafts: m2c, compile, compare (batch 12)

Milestone 5 (game code to matching C, about 1,750 functions) needs a first C version of every function. This batch makes
one for all of them, checks each against the original, and says where to start. It runs on its own in about 2 minutes:

    pwsh tools\batches\batch12_drafts.ps1 [-Sample 50] [-Jobs 8]      # or: python -I tools\draft_batch.py [--only NAME ...]

It is in `run_all.ps1` (after batch 11, before the checksums). It needs batch 9's output (`decomp/build/GXCE01/asm` and
`obj`, the compilers; all read only, nothing in `decomp/build` is written and ninja is never run) and `tools/m2c`
(`tools/setup.ps1` clones it at the commit in `tools/pins.txt`; plain Python, nothing to install).

## What it does, per game function

The game code is the one automatic object `0x800055E0-0x80131C08` of `decomp/config/GXCE01/splits.txt` (1,751 functions;
see plan.md Milestone 1), i.e. everything that is not in a library object.

1. **Asm**: the function is cut out of dtk's `auto_01_800055E0_text.s` (names are already ours). A `jumptable_*` it uses is
   copied next to it with `.L_` targets (and a label where nothing branches), because m2c wants the table beside the code.
2. **Draft**: m2c (`-t ppc-mwcc-c --valid-syntax --deterministic-vars`) with a context: every HSD header of Melee's
   baselib + `symbols/cr_types.h` + an `extern` for each global of `symbols/cr_globals.txt`, preprocessed once by the game
   compiler's own `-E` (`decomp/drafts/ctx_pp.c`, defines as `ParseHeaders.java`: `M2CTX`, no `__MWERKS__`).
3. **Compile**: `GC/2.6` with `cflags_cr` + the `game` set of `decomp/configure.py` (read from that file, so the flags cannot
   drift): `-opt level=0,peephole,schedule,speed -fp_contract on`, see `docs/compiler.md`. Draft + `ctx.h` (the same headers).
4. **Compare** with the function in `decomp/build/GXCE01/obj/auto_01_800055E0_text.o` (copied at the start), word by
   word. Fields a relocation fills in (call targets, `@ha` / `@l` halves, small-data offsets) are masked on both sides
   by relocation type; the rest must be equal. match % = equal words / the longer of the two. **This is the same test as
   the compiler probe, not objdiff**: it does not check that a relocation points to the same symbol, so "100% MATCH" means
   "code identical modulo relocations". Confirm with objdiff once a function is moved into `decomp/src`.

## Output (all in `decomp/drafts/`, gitignored: decompiled output is never committed)

| Path | What |
|---|---|
| `c/<name>.c` | the draft (m2c's text behind `#include "ctx.h"`) |
| `asm/<name>.s`, `obj/<name>.o` | its input and its compiled object |
| `log/<name>.txt` | m2c or compiler messages of the failures |
| `draft_report.txt` | one line per function: address, name, bytes, status (`m2c failed`, `compile failed`, `N%`, `100% MATCH`) and the first error; totals; match % histogram; the most common errors |
| `ctx_pp.c`, `ctx.h`, `orig.o` | m2c context, compile header, copy of the original object |

`decomp/reports/draft_summary.txt` is the committed part: the same totals, histogram and error counts, no code.

## First full run (m2c c632946, GC/2.6)

1,751 functions: m2c ok 1,749 (2 failed: `fn_800A0E64`, `fn_800A63C0` use `bctr` through a table m2c cannot read), compiles
891, **100% MATCH 153** (mostly 4 to 240 byte functions: stubs, getters, small float and bit functions), 541 below 10%.
The compile failures (858) are m2c's usual draft problems under the compiler's strict C: `illegal type` (pointer
arithmetic on `void *` / `M2C_FIELD` forms), `illegal implicit conversion` (pointer / int mixes), `undefined identifier`
(stack arrays like `sp64` that m2c names but does not declare), `pointer/array required`. The low percentages are mostly
the same story: with unknown global types the compiler picks small-data (`sda21`) access where the original loads the
address with `lis`/`addi`, and m2c's temporaries change register allocation. A draft is a start, not a match.

## Using it

- Start with the 100% matches: they can go into `decomp/src` as they are (rename, add to `CR_MATCHING`; the file split of
  the game code is still open, plan.md Milestone 1).
- Then the 60-99% ones: usually one wrong type or a missing cast. Fix the type in `symbols/cr_types.h` / `cr_globals.txt`
  (a proven fix helps every draft that touches it) and rerun; `--only NAME` reruns single functions in seconds.
- The report is the work list; the error histogram says which m2c patterns to fix generally (e.g. a post-pass on drafts).

## Determinism

Functions are processed in address order, m2c runs with `--deterministic-vars`, the reports contain no times or paths, and
the output only depends on the pinned m2c, the asm / object of batch 9, the headers and the types files. Two consecutive
full runs gave byte-identical `draft_report.txt` and `draft_summary.txt`. It changes legitimately whenever a name, a type or
`cr_globals.txt` changes, so it is not in `tools/checksums.ps1` (which would then fail after every naming step); the
summary is committed instead so a diff shows progress.
