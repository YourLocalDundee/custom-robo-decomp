# Game-code drafts: m2c, compile, compare (batch 12)

Milestone 5 (game code to matching C, about 1,750 functions) needs a first C version of every function. This batch makes
one for all of them, checks each against the original, and says where to start. The drafts take about 6.5 minutes, parallel
(two m2c passes and the repair loop, plan.md step 59; 2 minutes in the first version, step 57); after them the batch promotes
the linkable matches into `decomp/src/game` and, when that folder changed, runs batch 9 again, which adds that build's time
(with `-Sample` there is no promotion):

    pwsh tools\batches\batch12_drafts.ps1 [-Sample 50] [-Jobs 8]      # Windows; or: python -I tools\draft_batch.py [--only NAME ...]
    tools/batches/batch12_drafts.sh [--sample 50] [--jobs 8]          # Linux / macOS; or: python3 -I tools/draft_batch.py [--only NAME ...]

It is in `run_all.ps1` (after batch 11, before the checksums). The draft step needs batch 9's output (`decomp/build/GXCE01/asm`
and `obj`, the compilers; all read only: the drafts write nothing in `decomp/build` and never run ninja; the promotion
and the batch 9 rerun after it do) and `tools/m2c` (`tools/setup.ps1` / `tools/setup.sh` clone it at the commit in `tools/pins.txt`;
plain Python, nothing to install).

## What it does, per game function

The game code is `0x800055E0-0x80131C08` of `decomp/config/GXCE01/splits.txt` (1,751 functions), i.e. everything before
the first library object: dtk's automatic objects there plus the game objects linked from C (`game/`, see "Promoted to
decomp/src/game" below). The batch reads it from every object of dtk's split in that range (`decomp/build/GXCE01/config.json`),
so the drafts do not depend on how the game code is split.

1. **Asm**: the function is cut out of dtk's asm of the object that holds it (names are already ours). A `jumptable_*` it uses is
   copied next to it with `.L_` targets (and a label where nothing branches), because m2c wants the table beside the code.
2. **Draft**: m2c (`-t ppc-mwcc-c --valid-syntax --deterministic-vars --stack-structs`) with a context: every HSD header of
   Melee's baselib + `symbols/cr_types.h` + an `extern` for each global of `symbols/cr_globals.txt` + **every `lbl_` data
   object of `decomp/config/GXCE01/symbols.txt` with its real size and section** (`data_decls`: dtk's element type, the
   size as the array length; an object of a small section stays at most 8 bytes, one of a large section is never smaller
   than 9 or an array of unknown size, because the compiler decides small vs large data by the size alone with
   `-sdata 8 -sdata2 8`: without a size it took every unknown global for small data, one `sda21` load where the original
   has `lis`/`addi`) + **the prototypes of the game functions from their own drafts** (`ctx_protos.h`: a first m2c pass
   over all functions without the data declarations gives each function's signature and the pointer types m2c infers
   for 4-byte objects; the second pass, the one kept, then sees the callee's real parameters), preprocessed once by the
   game compiler's own `-E` (`decomp/drafts/ctx_pp.c`, defines as `ParseHeaders.java`: `M2CTX`, no `__MWERKS__`).
   `--only` / `--sample` reuse the `ctx_protos.h` of the last full run (a full run always rewrites it).
3. **Post-process** (all of it in `draft_batch.py`, so a rerun reproduces it; no draft is edited by hand): a stack array m2c
   names but does not declare is declared from its `--stack-structs` template (length up to the next field); `sp8 = arg0;`
   with `sp8` used nowhere else is dropped (the compiler's own store of a parameter to its stack home at level 0; the draft
   must not write it) and so is `var = saved_reg_rN;` (a read of a register nothing wrote); `*(void *)ADDR = x` gets the
   value's type. The draft defines the function with its own signature: the context's prototype of the function itself is
   renamed away (`#define NAME NAME__proto` around `#include "ctx.h"`).
4. **Compile**: `GC/2.6` with `cflags_cr` + the `game` set of `decomp/configure.py` (read from that file, so the flags cannot
   drift): `-opt level=0,peephole,schedule,speed -fp_contract on`, see `docs/compiler.md`. Draft + `ctx.h` (the same headers).
   **Repair loop**: the compiler's first message is placed in the draft (its printed window of the line, matched without
   spaces, with `M2C_FIELD` expanded when the compiler expanded it) and one mechanical fix is applied, then it compiles
   again, until it compiles, no rule applies, or 300 fixes: `illegal implicit conversion from A to B` casts the expression
   to B (for a call's argument, the last one not cast yet); `illegal type` / `illegal operand` on `void *` arithmetic turns
   the `void *` locals and parameters of that line into `u8 *`; `pointer/array required` on `*(x + 8)` loads through the
   type of the cast in front of it (else `s32 *`); `call of non-function` on `*(table + i * 4)(args)` calls through an
   unprototyped function pointer. The report's note says how many fixes a draft took.
5. **Compare** with the function in dtk's split object that holds it (`decomp/build/GXCE01/obj/...`, copied at the start
   to `decomp/drafts/orig/`), word by word. Fields a relocation fills in (call targets, `@ha` / `@l` halves, small-data offsets) are masked on both sides
   by relocation type. **100% MATCH** = the same number of words and every word equal (the only meaning `promote_drafts.py`
   relies on); below that, match % = equal words after an alignment (difflib's matching blocks over the masked words) /
   the longer of the two, at most 99, so one inserted or dropped instruction costs one word, not the rest of the function
   (the first version compared position by position, which scored 541 near-exact drafts 0-9%). **This is the same test as
   the compiler probe, not objdiff**: it does not check that a relocation points to the same symbol, so "100% MATCH" means
   "code identical modulo relocations". Confirm with objdiff once a function is moved into `decomp/src`.

## Output (all in `decomp/drafts/`, gitignored: decompiled output is never committed)

| Path | What |
|---|---|
| `c/<name>.c` | the draft (m2c's text behind `#include "ctx.h"`) |
| `asm/<name>.s`, `obj/<name>.o` | its input and its compiled object |
| `log/<name>.txt` | m2c or compiler messages of the failures |
| `draft_report.txt` | one line per function: address, name, bytes, status (`m2c failed`, `compile failed`, `N%`, `100% MATCH`) and a note (the first error, `M2C_ERROR`, `N fixes`); totals incl. bytes matched; match % histogram; the most common errors |
| `ctx_pp.c`, `ctx.h`, `ctx_protos.h`, `orig/` | m2c context, compile header, the game functions' prototypes from the first pass, copies of the original objects |

`decomp/reports/draft_summary.txt` is the committed part: the same totals, histogram and error counts, no code.

## First full run (m2c c632946, GC/2.6)

1,751 functions: m2c ok 1,749 (2 failed: `fn_800A0E64`, `fn_800A63C0` use `bctr` through a table m2c cannot read), compiles
891, **100% MATCH 153** (mostly 4 to 240 byte functions: stubs, getters, small float and bit functions), 541 below 10%.
The compile failures (858) are m2c's usual draft problems under the compiler's strict C: `illegal type` (pointer
arithmetic on `void *` / `M2C_FIELD` forms), `illegal implicit conversion` (pointer / int mixes), `undefined identifier`
(stack arrays like `sp64` that m2c names but does not declare), `pointer/array required`. The low percentages are mostly
the same story: with unknown global types the compiler picks small-data (`sda21`) access where the original loads the
address with `lis`/`addi`, and m2c's temporaries change register allocation. A draft is a start, not a match.

## Second generator (plan.md step 59, Oct 29, 2026)

The three generator changes above (sized data declarations, the drafts' own prototypes, the post-processing and repair
loop) and the aligned score, same m2c and compiler: **1,446 compile (891 before), 236 match 100% (153), 279,816 of the
1,230,376 game-code bytes equal after alignment**; histogram of the 1,446 compiled: 0-9% 57, 10-19% 106, 20-29% 128,
30-39% 281, 40-49% 178, 50-59% 156, 60-69% 93, 70-79% 92, 80-89% 81, 90-99% 38, 100% 236. The 303 compile failures left
(`draft_summary.txt`): `pointer/array required` on `**x` / `***x` chains and `x[i]` of integers (m2c's own typing, 68),
`illegal explicit conversion` from a struct or a float to a pointer (63), conversions the loop cannot place (57), the
rest are a struct used by value, prototypes of the library (`PSMTXRotRad` with 3 args: m2c's guess vs the header) and
m2c's own errors. Below 100%, the typical gaps now are m2c's temporaries (one more store / load pair), a wrong width
(`u8` where the original loads a `u16`) and the few functions compiled with optimisation (register pointer walks with
`lfsu`: cr_Mtx_Store): the next step is types in `cr_types.h` / `cr_globals.txt` (the drafts' context sizes every `lbl_`
already; naming and typing one fixes every draft that touches it).

## Third generator (plan.md step 60, Oct 30, 2026)

Round 2 on the compile failures, same m2c and compiler. (1) **The context declared none of the 111 library functions
the game code calls** (655 call sites): to the compiler they were implicit `int f()` (float arguments promoted to
double, `PSMTXRotRad` "3 vs 4 args" was its own guess from the first call of a draft). Now `sdk_protos` takes the
prototype of every undeclared `bl` target from the decomp's pinned SDK headers (`decomp/include/dolsdk2004`, read only;
only a prototype whose types the context knows: 20, `ctx_sdk.h`) and the unnamed library `fn_` the game calls (90)
get theirs from their own first-pass draft like the game functions (`ctx_protos.h`); the context is compiled once as
a check. (2) **Repair rules** added: `**x` / `***x` chains and `x[i]` on integers load through `(T **)` / `(T *)`;
casts to function-pointer types (`long (**)(...)`) are allowed; `*M2C_FIELD(x, void **, N) = v` stores through
`v`'s type; a conversion to an anonymous `struct *` casts to `void *`; a struct wanted by value from a pointer
derefs (`*(GXColor *) &sp38`); an explicit conversion from a struct by value reads the scalar at the offset
(`*(u16 *) ((u8 *) base + off)`) or takes a stack struct's address; a call's argument is chosen by the callee's
parameter of the reported type (prototypes parsed from the context; a variadic call's extra arguments are never
cast, which was the `float` to `char *` class); `call of non-function` also for `*obj(args)` and `table[i](args)`;
`illegal operand` casts a pointer used as the right operand of `+` / an index / compared with `(u32)` to an
integer and `(void *) (addr)` in a comparison to `u32`; `type mismatch` swaps the cast; a call with fewer / more
arguments than the prototype is padded with zeros / cut (compiles, cannot match); `unksp` locals and m2c's `s24`
width are declared; a definition with a function-pointer parameter is recognised (the stack arrays of such drafts
were not declared before). **Before -> after**: compile 1,446 -> 1,653 of 1,751 (failures 303 -> 96), 100% MATCH
236 -> 235, bytes equal after alignment 279,816 -> 354,216; histogram 0-9% 57 -> 77, 10-19% 106 -> 135, 20-29%
128 -> 159, 30-39% 281 -> 313, 40-49% 178 -> 193, 50-59% 156 -> 184, 60-69% 93 -> 117, 70-79% 92 -> 111, 80-89%
81 -> 88, 90-99% 38 -> 41. The one match lost (fn_80021A40, kept in decomp/src/game) calls a library function whose
draft signature takes an argument the caller never sets (`M2C_ERROR`); the one gained, fn_800215F8, passes a
pointer to `DVDClose`. The 96 failures left: `pointer/array required` where the operand is an array element or a
struct field (25), explicit conversions m2c itself writes (`M2C_BITWISE(f32, ptr)`, 19), `illegal operand` on
struct-typed globals (`*(cr_g_AreaBoxes + i) & 1`, 19), `not an lvalue`, misplaced `case` / `default` labels (m2c).
The 5 matched drafts with an .sdata2 constant of their own cannot be promoted by the sole-user rule: every one of
their constants is shared (lbl_804502A0 18 objects, lbl_80450AB0 13, lbl_80451290 63, lbl_80451850 7), so a
per-function object cannot own the piece; they need the real file edges (several functions in one object).

## Fourth generator (plan.md step 62, Oct 30, 2026)

Round 3, on the drafts at 80-99% first. Method: every near match diffed instruction by instruction (the original
function of `decomp/drafts/orig/` and the draft's object, relocation fields masked, disassembled with batch 9's
binutils), the differences sorted into classes, one generator fix per class, measured on the 364 drafts at 80-100%
(`--only`, no draft lost a match at any step) and then on a sample. Same m2c and compiler. The classes and fixes (all
in `tools/draft_batch.py`; no draft is edited, no type file changed):
1. **Data model of the globals** (the largest class): a 4-byte global of `.bss` such as `cr_g_SeqCurrent` (`cr_globals.txt`
   declared it with its 4-byte type) was small data to the compiler, the original reads it with `lis`/`addi`. Every
   `cr_globals.txt` declaration now carries its section, and so does every `lbl_` object whose size disagrees with its
   section (`CR_SECTION`: `__declspec(section ".data" / ".sdata" / ".sdata2" / ".rodata")`, which sets the data model of
   an extern in GC/2.6, tested; only when a draft is compiled (`-DCR_DRAFT_CC`), nothing for m2c). Those `lbl_` objects
   were cut to `u8[8]` or declared `u8 name[]` before, which also hid their type (a float of `.rodata` read as bytes); they
   now keep dtk's type and size. `promote_drafts.py` writes the same `__declspec` into a promoted file.
2. **Compound assignments**: m2c writes `x->f = x->f + 1` / `(u8) (f + 0x20)`; at level 0 the compiler then loads a global
   pointer twice or keeps the sum in another register. `X = X op Y` becomes `X op= Y` when X has no call or `++`/`--` and
   Y is one operand (a cast only when it is X's own field type).
3. **Loops**: m2c's `loop_N: if (c) { ...; goto loop_N; }` becomes `while (c) { ... }` when nothing else jumps there (the
   compiler emits a while / for loop with its test at the bottom).
4. **Casts to a value's own type in a comparison** (`(u8) lbl != 0`): one more `clrlwi.` than the original's `cmplwi`;
   dropped only in comparisons (elsewhere the same cast moves registers both ways: cr_C_ACT_TURN_POINT).
5. **Jump-table calls**: `(*(table + i * 4))(a, table)`: m2c passes the table's address (still in r4) as an extra argument;
   it goes (applied in the repair rule that writes the function-pointer call).
6. **Byte / halfword stores of 0x80-0xFF / 0x8000-0xFFFF** through `s8 *` / `s16 *` fields are unsigned (`li r4,255`, not -1).
7. **Compile failures**: `*M2C_FIELD(x, u32 *, 0)` (a field holding a pointer) reads through `u32 **`; `*x[2]` on an
   array declared `u32[]` casts the element to a pointer (the old rule wrapped the base in casts up to 300 times); m2c's
   `--(x)` (a double negation) becomes `- -(x)`; the prototype-aware argument cast compares `HSD_JObj *` with the
   compiler's `struct HSD_JObj *` (it cast the wrong argument: the `(struct HSD_JObj *) <float>` class).
Also: `--out DIR` now compiles against that folder's context (it used `decomp/drafts/ctx.h` before).
**Before -> after** (full run): compile 1,653 -> 1,679 of 1,751 (failures 96 -> 70 + 2 m2c), **100% MATCH 235 -> 293**,
bytes equal after alignment 354,216 -> 393,544; histogram 0-9% 77 -> 76, 10-19% 135 -> 116, 20-29% 159 -> 154, 30-39%
313 -> 310, 40-49% 193 -> 144, 50-59% 184 -> 159, 60-69% 117 -> 138, 70-79% 111 -> 145, 80-89% 88 -> 90, 90-99% 41 -> 54.
45 of the 129 drafts at 80-99% now match; 762 drafts score higher, 66 lower (none of them a match before; mostly an
instruction-scheduling difference that the old declarations happened to share, e.g. fn_8000DE08 77 -> 72%). **Promoted
286 + 3 kept = 289 game objects** (0x3F8C bytes = 1.32% of the game code; 388 of 732 units from C, 6.69% of all code;
main.dol retail SHA-1, every game object 100% code + data). Left out: 6 with an .sdata2 constant of their own (they need
real file edges) and cr_Chara_SetCurrentPointers (struct by value). Left for the next round: at 80-99%, pointer globals
of mixed use (`lbl_8044F728` is a struct pointer; pass 1 gives it `u8 *` because the drafts disagree, so `*p` is a byte
where the original loads a halfword: the fn_800AB468 family at 96%; declaring it `void *` to m2c alone did not help, m2c
still prints `*p`), m2c's `ptr += 0x38` on a `u32 *` (bytes as elements, fn_8009760C), a callee prototype from its own
draft that returns s32 where the caller tests a byte (fn_800F60A8), callee-saved registers of dead locals (fn_800F1860);
failures: `illegal operand` (23: pointer / integer comparisons, struct-typed globals in arithmetic
`*(cr_g_AreaBoxes + i)`), conversions from struct values (`cr_g_FileDescs[i] + n`, `motion_set + i`, 24), `*p += x`
through `void *` (10), `pointer/array required` (8). Target 3 of the step (SDK prototypes for AIRegisterDMACallback /
atan2 / acos / HSD_JObjSetPtclTargetCallback) not done.

## Promoted to decomp/src/game (plan.md step 58)

After the drafts, batch 12 runs `python -I tools/promote_drafts.py` (method: its docstring). Every 100% draft that can be
linked as it is becomes `decomp/src/game/<function>.c`, which IS committed (matched C is the decomp itself; the drafts
are not): its object is exactly the function with no data of its own (no constants, no strings), no padding follows it,
every relocation goes to the same address as the original's (the compare above masks them), and the promoted file
compiles to the same bytes and relocations as the draft. The file is the draft's text behind the committed header
`decomp/include/game/cr_game.h` (basic types, NULL, M2C_UNK / M2C_FIELD) plus the declarations it uses from the drafts'
context, generated with m2c's pycparser (prototypes, enums of the constants it names, typedefs; a struct only used
through a pointer is declared incomplete). A function nothing in main.dol references gets `#pragma force_active on`,
because mwld strips unreferenced C functions. `tools/gen_dtk_splits.py` makes each file an object (range from the
functions it defines), `decomp/configure.py` links every game/ object from C, and batch 9 checks retail SHA-1 and
objdiff 100%; when decomp/src/game changed, batch 12 runs batch 9 again. **These are per-function objects, not the game's
original files** (those are not known); merging functions into real files later only changes the ranges.
`decomp/reports/promote_report.txt` lists what was promoted and every 100% draft left out with the reason. First run
(Oct 29, 2026): **152 of the 153 promoted** (3,800 bytes, 0.31% of the game code); left out fn_800694C4 (an .sdata2
constant of its own: the shared int-to-float constant, needs that data piece split first). After step 59: **230 of the
236 promoted (9,308 bytes) + 2 kept = 232 objects, 0.77% of the game code**; left out 5 with an .sdata2 constant of their
own and cr_Chara_SetCurrentPointers (its generated declarations leave a struct incomplete that the draft uses by value);
the 2 kept (fn_800A0804, fn_800F60A8) matched in the first version and still link, their new drafts are at 85 / 94%.
Files carry a marker line: only marked files are rewritten; remove it before editing one by hand.

## Using it

- The 100% matches are linked by batch 12 by itself (above); the ones left out are listed in promote_report.txt.
- Then the 60-99% ones: usually one wrong type or a missing cast. Fix the type in `symbols/cr_types.h` / `cr_globals.txt`
  (a proven fix helps every draft that touches it) and rerun; `--only NAME` reruns single functions in seconds (it reuses
  `ctx_protos.h`; delete it to redo the first pass, about 100 s). Never edit a draft: every fix goes into the generator.
- The report is the work list; the error histogram says which m2c patterns to fix generally (e.g. a post-pass on drafts).

## Determinism

Functions are processed in address order, m2c runs with `--deterministic-vars`, the reports contain no times or paths, and
the output only depends on the pinned m2c, the asm / object of batch 9, the headers, the types files and
`decomp/config/GXCE01/symbols.txt` (the data sizes; a full run also rebuilds `ctx_protos.h` from the first pass). Two consecutive
full runs gave byte-identical `draft_report.txt` and `draft_summary.txt`. It changes legitimately whenever a name, a type or
`cr_globals.txt` changes, so it is not in `tools/checksums.ps1` / `checksums.sh` (which would then fail after every naming step); the
summary is committed instead so a diff shows progress.
