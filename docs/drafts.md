# Game-code drafts: m2c, compile, compare (batch 12)

Milestone 5 (game code to matching C, about 1,750 functions) needs a first C version of every function. This batch makes
one for all of them, checks each against the original, and says where to start. The drafts take about 21 minutes, parallel
(two m2c passes, the repair loop and the variants of every draft below 100%, plan.md step 69; 6.5 minutes after step 59,
2 minutes in the first version, step 57); after them the batch promotes
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

## Fifth generator (plan.md step 63, Oct 30, 2026)

Round 4 worked on the compile failures and the 4 library prototypes left over from step 62. As before, every fix is in
the generator and no draft is edited. Fixes were measured with `--only` on the 72 failures in a separate `--out` folder
and then with a full run.
1. **Library prototypes the context lacked** (`sdk_protos`): `AIRegisterDMACallback` was skipped because its type
   `AIDCallback` was unknown. A one-line typedef from the same SDK header is now taken along when the context knows
   its types, and is written first in `ctx_sdk.h`. `atan2` / `acos` come from MSL's `math.h` (Mario Party 4's MSL_C in
   decomp/include, read only). Before, their pass-1 drafts had typed them `void f(void)`.
   `HSD_JObjSetPtclTargetCallback` (KAR's name, no header has it) gets `void (*)()` from `LIB_PROTOS`. Evidence: its
   body `stw r3, lbl_8044FFE0; blr` stores its argument, and 0x80197A00 loads that word, tests it for 0 and calls it
   (`mtctr r12; bctrl`). Effect: fn_80075CA8 74 -> 81%, fn_800E52D0 29 -> 46%, fn_8001C5AC 21 -> 24%.
2. **`illegal operand` in comparisons**:
   - an integer field compared with a pointer (`M2C_FIELD(p, u32 *, 4) == arg0`): the pointer is cast to `(u32)`, which
     is the same `cmplw`;
   - `(void *) M2C_FIELD(...) == x` with an integer x: the field is cast to `(u32)`;
   - `M2C_FIELD(p, u8 **, n) == (u32) x`: the field is cast to `(u32)`;
   - `(u8 *) lbl == arg` with an integer arg: the address is cast to `(u32)`.
   23 -> 11.
3. **A struct read by value where the code reads a scalar** (`illegal implicit conversion from 'struct cr_FileDesc'`):
   m2c steps a struct pointer in bytes (`*(cr_g_FileDescs[a] + ((e * 0x10) & 0xFFFF0) + 8)`). The read becomes
   `*(T *) ((u8 *) base + off)`, with T the type the compiler names, as step 60 already did for the explicit casts.
   Implicit conversions 12 -> 6.

**Before -> after** (full run): compile 1,679 -> 1,697 of 1,751 (failures 70 -> 52 + 2 m2c), 100% MATCH 293 -> 293,
bytes equal after alignment 393,544 -> 399,064. Histogram: 0-9% 76 -> 76, 10-19% 116 -> 117, 20-29% 154 -> 159,
30-39% 310 -> 311, 40-49% 144 -> 148, 50-59% 159 -> 160, 60-69% 138 -> 142, 70-79% 145 -> 145, 80-89% 90 -> 92,
90-99% 54 -> 54. No match was lost; the 18 newly compiled drafts score 14-87% (one at 80-99%). Promoted: 287 + 3 kept =
**290 game objects**. cr_Chara_SetCurrentPointers now links: the promoted file defines the structs that its globals
point to when the compiler needs their fields (promote_drafts.py). No file object was possible yet (next section).
Not done in this round (bound): the 146 drafts at 80-99% (144 before) (classes listed under the Fourth generator, still open). Of the
52 failures, the ones left are explicit conversions from `float *` / `void (*)()` (12), `illegal operand` on
struct-typed globals in arithmetic (`*(cr_g_AreaBoxes + off) & 1`) and on struct values plus offsets (11), `illegal
type` (10) and `pointer/array required` (8).

## Sixth generator (plan.md step 64, Oct 30, 2026)

Round 5 worked on the drafts at 80-99% first (each one diffed instruction by instruction against the original, the
differences sorted into classes), then on the failures. Every fix is in `tools/draft_batch.py` (one in
`symbols/cr_types.h`, one in the game flags of `decomp/configure.py`); no draft is edited. "Changed" = drafts whose
text the rule changes in the final run (counted by running each rule off), "matched" = drafts it brought to 100%.
1. **Game flags: `-use_lmw_stmw on`** (configure.py, tools/patches/dtk_template_configure.patch). All 614 game
   functions that save five or more registers use stmw / lmw (three or four: single stw), none calls `_savegpr_N`,
   which GC/2.6 does at level 0 without the option. No draft with stmw in the original had ever matched. With the
   register order (6) it matched fn_800772C0, fn_800773D0, fn_800774A4, fn_80077578.
2. **Library prototypes without m2c's `arg_spN`** (`signature`): m2c reads the stack slots `_savegpr_N` addresses
   through r11 as a stack argument; the 17 prototypes that had one made every caller pass
   `M2C_ERROR(unset register)`, which the repair loop turned into `li r3,0`. 8 near matches had that load; 7 match
   now (fn_800055E8, fn_80021A40, cr_SEQ_TITLE_FrameC with rule 3, fn_80075BBC, fn_80075C14, fn_80079080,
   fn_800BCC60; fn_800EAC48 86%).
3. **HSD assert as a conditional expression** (`assert_ternary`): `if (c) { } else { __assert(...); }` ->
   `(c) ? (void) 0 : __assert(...);` (Melee's HSD_ASSERT; `beq; b` instead of `bne`). Changed 46, matched
   fn_80075120, fn_80094394 and the callers of rule 2.
4. **Mixed-use pointer globals** (`data_decls` MIXED, `mixed_externs`): a 4-byte object the first-pass drafts type as
   pointers to different things (44 of them, e.g. lbl_8044F728) is no longer declared `u8 *` for everyone; each
   draft declares it with its own type (m2c's own extern; `void *` becomes `u8 *`, the section is kept). Changed 263,
   matched the fn_800AB468 family (10 functions, `lhz` where `u8 *` gave `lbz`).
5. **m2c's byte steps** (`byte_steps`, `element_terms`): m2c prints `p + n` with n in bytes whatever p points to. For a
   pointer / array of larger elements: element steps when n is a constant multiple (`p + 8` -> `p + 2`), the terms
   divided when every term is (`p + ((x * 0xC) + 4)` -> `p + x * 3 + 1` on a u32 *, not for dtk's `lbl_` arrays),
   else `(T *) ((u8 *) p + n)`. Changed 562, matched fn_8001F654, fn_8006FAC4, fn_8009760C, fn_80096F54,
   fn_80097530, fn_80097588, fn_80099538, fn_800BAF3C.
6. **Register order** (`register_order`): the locals m2c names after the original's register are declared from r31
   (f31) down in the slots they had; the compiler hands out callee-saved registers in declaration order. Changed 862.
   A variant (below), because the allocator's first key is not always the declaration (fn_8001F558).
7. **Unsigned same-type copies** (`same_type_copies`): `M2C_FIELD(a, u16 *, n) = (u16) M2C_FIELD(b, u16 *, m);` drops the
   cast (another register at level 0). Changed 131, matched cr_Motion_LoadInfo (with rule 5).
8. **cr_Sin16 / cr_Cos16 return f32** (symbols/cr_types.h, with evidence: every path ends in `lfs f1` / `fneg`, callers
   store with stfs / use fmuls with no frsp): 19 drafts call them; matched fn_800A54A4, fn_800A5638.
9. **`cror eq, lt, eq`** (asm input): dtk's spelling of `x <= y` after fcmpo was unknown to m2c's FcmpoCrorPattern
   (numeric only), so the bne after it became `==`. The asm m2c reads uses `cror 2, 0, 2` now: 79 functions.
10. **Callee return widths** (`narrow_returns`): a first-pass caller declares an unknown callee itself (`u8
    fn_80099538(M2C_UNK); /* extern */`); when every caller agrees on u8 / s8 / u16 / s16 and the callee's own draft
    says s32 / u32, the prototype takes the narrower type: 25 prototypes; matched fn_800F60A8, fn_80125C2C.
11. **Repairs**: explicit conversion from `T *` to `T` of `(f32) (&a[i])` -> the element; a float array element used as
    a pointer read as u32; `void **p` for `*p += x` (relocation loops) declared `u8 **`; `->` no longer ends an
    operand in byte_steps. Failures 52 -> 47.
12. **`-1U` against a halfword / byte** (`narrow_minus_one`): `(u16) x != -1U` / `M2C_FIELD(.., u16 *, n) != -1U`
    -> `0xFFFF` (0xFF). Changed 82, matched fn_80130848.
13. **Parameters**: `var_r3_0 = arg0;` as a first statement (the parameter's own register) is the parameter
    (`param_regs`, 7; fn_8008E598 18% -> 75%); `var_r23_8 = arg0;` in a function that calls (`param_copies`,
    24): the parameter kept in a callee-saved register (matched fn_80095700); `sp8 = arg1;` (`param_homes`, a variant,
    72: the parameter's stack home in fn_800683B4, a real local in fn_8005A668).
14. **Signed fields** (`signed_fields`): `(s8) M2C_FIELD(x, u8 *, n)` -> `M2C_FIELD(x, s8 *, n)` (s16 alike): one
    register for load and extension. Changed 186, matched fn_80008E80.
15. **Access widths in m2c's output** (`M2C_WIDTH`, a run-time patch in the generated m2c_run.py; the pinned m2c is
    unchanged): m2c prints a load it cannot place in a field as `*p` / `*(p + x)` whatever p's C type points to (a
    lwz through a `u8[]` became a byte load). When p's element size is known and differs, or p is `void *` / an
    integer, m2c's own `M2C_FIELD(p, T *, 0)` with the access's type is printed instead (address constants stay as
    they were). Changed 383; matched fn_80079108, fn_80077290 (with 16).
16. **VARIANTS** (`one`): a draft that does not match and scores 40% or more is compiled again without
    register_order, without param_copies, without both, with param_homes, with `((void *) 0)` for NULL in comparisons
    (`void_null`: `li r0,0; cmplw` where MSL's 0L gives cmplwi; 313 drafts have a NULL comparison), and without
    register_order with void_null; the best score is kept (ties: the full rules). Deterministic; batch 12 takes about
    8 minutes.
**Before -> after** (full runs, same m2c, GC/2.6 with the new flag): compile 1,697 -> 1,702 of 1,751 (failures
52 -> 47, + 2 m2c), **100% MATCH 293 -> 350** (57 new, none lost), bytes equal after alignment 399,064 -> 428,472;
histogram 0-9% 76 -> 67, 10-19% 117 -> 93, 20-29% 159 -> 152, 30-39% 311 -> 294, 40-49% 148 -> 136, 50-59% 160 -> 159,
60-69% 142 -> 138, 70-79% 145 -> 156, 80-89% 92 -> 114, 90-99% 54 -> 43. **Promoted 340 + 2 kept = 342 game objects**
(was 290), 0x67AC bytes = 2.16% of the game code (was 1.33%); 441 of 809 units from C, 7.26% of all code (was 6.69%);
batch 9 main.dol retail SHA-1, every game object 100% code + data in objdiff.
Left for the next round: the `li r5,2` family of the SEQ handlers (cr_SEQ_ENDING_FrameA & co., two stores whose
constant the original keeps in r5: not found), m2c merging a temporary into a later variable (fn_8001D05C family,
the sum before cr_Sqrt), ArrayAccess widths (m2c's ArrayAccess keeps the element type, not the access's:
fn_8001B3F0), extra callee-saved registers of m2c temporaries (the `stmw r25` vs `r26` class), switch layouts
(fn_8001F558); failures: pointer/array required 11, illegal operand 10, explicit / implicit conversions 7 + 7,
illegal type 7.

## Seventh generator (plan.md step 69, Oct 30, 2026)

Round 6 started from a baseline run of the step 64 generator on the current build (batch 9 first: the copied build
still had step 63's asm with `cr_Sqrt`, which cost two matches): 350 at 100%, 1,702 compile, 428,472 bytes equal.
Method as in round 5: the near matches diffed instruction by instruction (scratch tools, not committed: an objdump
side-by-side of the draft's and the original's function, and a classifier that sorts every draft's differing blocks
by their mnemonics, so the classes are ranked by how many drafts they touch), one generator fix per class, measured
with `--only` on the drafts at 40-100% (no match lost at any step) and then with full runs. Every fix is in
`tools/draft_batch.py` (one in `tools/promote_drafts.py`, three field types in `symbols/cr_types.h`); no draft is
edited. "Changed" = drafts whose text the rule changes (counted over the final m2c output, default and --reg-vars),
"matched" = 100% drafts whose kept form needs it (the report's note now names the winning variant, e.g. `[reg-vars
+float_literals]`).
1. **m2c --reg-vars drafts** (`REG_VARS`, `regvar_cleanup`): at level 0 the compiler gives every C local its own
   callee-saved register from r31 / f31 down and never shares one, so two m2c temporaries in the same callee-saved
   register were one variable (fn_80017668's saved interrupt state, `temp_r31_23` / `temp_r31_76`), and m2c's folding
   of a value into its single use hides the variable the original kept (fn_8001D05C family: `f31 = x*x + y*y + z*z;
   f31 = sqrt(f31);`). Every draft below 100% is drafted again with m2c's own `--reg-vars` for r14-r31 / f14-f31 (the
   epilogue restores it prints as `M2C_ERROR(psq_l ...)` statements are dropped) and tried with the same rules and
   variants. 34 matches need it (fn_80017668, fn_8001D05C / D104 / D1AC, fn_8000DA5C / DB94 / DCCC, fn_800F67FC,
   fn_80104AB0 ...).
2. **Volatile temporaries back into their use** (`inline_volatile_temps`): a temporary named after r0, r3-r12 or
   f0-f13, assigned once and used once, was an expression; as a C local it took a callee-saved register the
   original never saves (the `stmw r25` vs `r26` / extra `stw r30` class). The expression goes into its use: in place
   when the statements between only update other locals; with those statements moved after the use when they update
   locals it reads (the scheduler had moved a loop's `addi` above the store: fn_8008C3B4); past any simple statements
   when it reads only locals; an address of named objects (`&lbl_803BE358[lbl_8044F770]`) past statements that write
   only other named objects (two named objects never overlap); a pointer or narrow integer keeps its type as a cast
   (fn_80005D38's `u16 *`). Changed 694 (702 of the --reg-vars drafts); a variant leaves it out (it loses as often as it wins: 250 of the kept drafts
   below 100% are better without it).
3. **Parameter aliases** (`param_aliases`): `var_r31 = arg0;` among the first statements with neither assigned again
   (the --reg-vars drafts name the parameter after its copy everywhere) is the parameter itself; not in leaf
   functions; a variant leaves it out. Changed 120 (all --reg-vars drafts).
4. **Float constants as literals** (`float_literals`, a variant; `pool_floats` reads the values of dtk's
   data:float / data:double objects of .sdata2 from main.dol): three stores of `0.0f` load f0, f1, f0 with the second
   load moved up, three stores of an extern float load f0 three times (fn_80079C70, fn_8001CA84). The literal is the
   compiler's own pool constant, masked by the compare. 16 matches need it; such a draft owns its .sdata2 constant,
   so it is linked only with its whole file run (promote_report.txt "left out": all 33 are drafts with data of their own).
5. **Table calls**: `temp_r3 = table + i * 4; *temp_r3(temp_r3, x);` drops the slot's own address from the arguments
   (`table_call_self`, it was only in r3; changed 27); the variant `table_call_noargs` calls through the slot with
   no arguments (m2c passed whatever the argument registers held: fn_8005C4D0's r4; 15 matches); the repair rule
   for `call of non-function` takes `M2C_FIELD(table + i * 4, u32 *, off)(args)` (rule 8's width form of a function
   pointer) as a call through that slot.
6. **Callers' parameter signedness** (`narrow_params`, pass 1): when most callers that type an argument (their
   first-pass `/* extern */` declarations; constants say s32 or M2C_UNK) pass a byte / halfword of the other
   signedness than the callee's own draft, the prototype in ctx_protos.h takes the callers' type (fn_80098104's
   `s8 arg1` gave every caller an `extsb` after its `clrlwi`): 6 parameters; with rule 7 matched fn_8005C4D0.
7. **Small expression rules**: `(u8) (s32) (f)` -> `(u8) (f)` (`narrow_int_casts`: m2c's two casts of a float
   converted to a byte add a `clrlwi`; changed 25); `= -1U;` -> `= -1;` (`minus_one_stores`: `li r0,-1` stored
   through a byte; changed 29; fn_80111E54 97%); `(u8) a[3] == 0` on a u8 array drops its cast (drop_same_casts
   for indexed arrays: cr_SEQ_MODE_SELECTOR_Leave).
8. **Access widths of words in byte arrays** (`M2C_WIDTH`): m2c's int-or-pointer loads (its type for a `lwz` it
   cannot tell from a pointer) from dtk's untyped `u8[]` objects are printed with their width,
   `M2C_FIELD(lbl_8024E120 + i * 0x33520, u32 *, 0)`, and so is its array spelling `lbl_801F10C0[(x * 4) & 0xFC]`
   (fn_8001B3F0, the round-5 ArrayAccess item) and a struct pointer stepped in bytes (`*(chara->motion_set + i * 2)`
   -> `M2C_FIELD(chara->motion_set, u16 *, i * 2)`: cr_C_ACT_MOT, the `lhzx` the repair rule had read as u32).
   Matched fn_8000768C, fn_800BC9A4, fn_8010D2D0, fn_800B77AC, fn_8001B3F0.
9. **More variants** (each one toggle; a second round adds every single toggle to the best variant of a draft at
   80-99%, for two rules a draft needs together): `drop_same_field_casts` (`(s32) M2C_FIELD(x, s32 *, n)` anywhere),
   `unsigned_fields` (`(u16) M2C_FIELD(x, s16 *, n)` -> a u16 field: cr_C_ACT_TURN_POINT; as a rule it lost
   cr_C_ACT_MOVE), `redundant_returns` (`if (c) { ...; return; } return;` -> one return: fn_8008BA44's two blr),
   `widen_masked_bytes` (a `u8` local only ever assigned `(u8) (E)` becomes a u32 of `(E) & 0xFF`: fn_8005C628 with
   table_call_noargs), `invert_early_returns` (`if (c) { return; } rest` at the function's end -> `if (!c) { rest }`),
   `chain_assign` (`A = v; B = v;` -> `B = A = v;`); no match needs the last two yet. The variants and --reg-vars now run for every compiled draft
   below 100% (VARIANT_MIN 0: 3 more matches below 40%); the draft step takes about 21 minutes (1,284 s; was 445 s).
10. **Field signedness from the loads** (symbols/cr_types.h): cr_Chara.id (+193C), target_id (+1996) and
    countdown_1990 (+1990) are u16: every load of them in the game code is `lhz` (42, 3, 2), none `lha`.
11. **promote_drafts.py**: the other names of an anonymous struct (`typedef struct { ... } Vec, Vec3, *VecPtr,
    Point3d;`) are emitted as typedefs of its first name; as copies they were distinct structs and
    cr_Chara_SetCurrentPointers' regenerated file stored a `Vec3 *` in a `Point3d *` (round 5's open item: it links
    again from its current draft); and when one level of struct definitions is not enough, a third try ("deep") also
    defines our own `cr_` structs that a defined struct's members point to (cr_C_ACT_TURN_POINT reads
    `cr_g_CurrentChara->motion_set->turn_r`).
**Before -> after** (full runs; m2c, compiler, flags unchanged): compile 1,702 -> 1,703 of 1,751 (failures 47 -> 46 + 2 m2c: pointer/array required 11,
illegal operand 9, explicit / implicit conversions 6 + 8, illegal type 7), **100% MATCH 350 ->
439**, bytes equal after alignment 428,472 -> 482,760; histogram 0-9% 67 -> 48, 10-19% 93 -> 72, 20-29% 152 -> 106, 30-39% 294 -> 127, 40-49% 136 -> 275, 50-59% 159 -> 144, 60-69% 138 -> 143, 70-79% 156 -> 155, 80-89% 114 -> 134, 90-99% 43 -> 60. **Linked: 407 game objects** (405 promoted, 1 file object, 1 kept; was 342), the
first FILE OBJECT among them: `game/fn_8005C4D0_fn_8005C4D0.c`, fn_8005C4D0 with its own int-to-float constant
(.sdata2 0x804509A0-0x804509A8), which no other function uses (round 5's prediction); 0xABF4 bytes = 3.58% of the game
code; 565 of 894 units from C, 16.72% of all code. Batch 9: main.dol retail SHA-1, every object 100%, static shift PASS.
Not found / left for the next round:
- the script-command head (`cr_S_BGM_STOP`, `cr_CHAR_MOVE` and about 35 more at 78-85%): the original saves r30 and
  never uses it, evaluates the log index `lbl_803BE358[2]` before the store's base and keeps the counter's address in
  r4; no spelling found (struct form, index arithmetic, temporaries inlined or not, an unused local: 78-82%); the
  common dispatch tail (`cr_g_ScrCmdFuncs[*script]()` / error flags from .sdata2 words) points to a macro or inline
  function of the script system;
- the `li r5,2` SEQ handlers (cr_SEQ_ENDING_FrameA & co., 6): **an inline function with a parameter reproduces them
  exactly** (`static inline void f(s32 v) { seq.x40 = v; seq.x24 = 1; }`; tested by hand): the source had a helper
  that sets the sequence state; it needs a name with evidence before a rule can write it;
- HSD inlines: fn_80095D7C matches as `if (!(jobj && HSD_JObjMtxIsDirty(jobj))) return; HSD_JObjSetupMatrixSub(jobj);`
  with this revision's MtxIsDirty (its assert at jobj.h line 605, Melee's 564) as an inline function, not as m2c's
  expansion: 12 drafts carry that assert; a rule that folds known HSD inline bodies back into calls would take them;
- epilogue order (`lwz r0` before `lwz r31` after a final store: fn_800A0804, 96 originals): the compiler gives the
  other order for every spelling tried (a label, do / while, an inline, a block, an unused local);
- `-inline_volatile_temps` wins in 250 drafts that do not match: the expression form is not always the source's.

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

## File objects (plan.md step 63)

A 100% draft with data of its own (an .sdata2 constant or a string literal) cannot be linked as a one-function object.
That data is part of its source file's pool, and the file's other functions use the same pool. `tools/promote_drafts.py`
therefore looks for the draft's **file run**. Every source file has its own constant pool and string literals, laid out
in link order (the per-file pool rule of `tools/gen_dtk_splits.py`). So every function that uses one of the draft's
pool entries is in its file, and so is every function in between; this is repeated until nothing new is added. When all
functions of the run are 100% drafts, they become one generated file `decomp/src/game/<first>_<last>.c`: the
declarations, the functions' own prototypes, then the drafts in address order. The file must compile to the drafts'
.text one after another. `gen_dtk_splits.game_file_data` then places its data: every relocation into its own .sdata2 /
strings must give the same piece start in main.elf, the bytes must be main.dol's, and nothing outside the run may use
the piece. The piece then goes into splits.txt with the object, and the run's one-function files are removed.
`decomp/reports/promote_report.txt` lists every run under "file runs": its range, how many of its functions match,
and why it is not built yet, plus the span of each draft's direct pool users. `python -I tools/promote_drafts.py
--check --try-run FIRST LAST` builds any run of 100% drafts as a file without writing anything (tested on
fn_80112848..fn_80112940, four functions: the file compiles to the four drafts' bytes in a row).
Status after step 63: **no run is complete**. The 6 matched drafts with a constant: fn_80008F8C's direct users are
127 functions, 54 at 100% (0x80008ED8-0x8001AA98); fn_800694C4 46, 8 at 100%; fn_800858B0 27, 1; cr_C_ACT_MOVE_LENG
and cr_C_ACT_MOVE 458, 24 (lbl_80451290 alone has 52 users over 0x80099EAC-0x800D9C88); fn_80112A0C 27, 10. Closed
over every pool entry, the runs grow further, because a string (0x801B6620) is also used from a data table and some
.sdata2 entries are used across very distant code (const globals or very large files). These runs become available as
the drafts around them match; the rule never links a group it cannot prove.
Step 64 (owned data only): a C file brings only the data its compiler generates itself (in practice the int-to-float
constants 0x4330000080000000 / 0x4330000000000000, one copy per file; jump tables and literals would count too); every
other pool entry is an extern `lbl_` of the drafts and stays in its asm object. So `Pools.file_run` now closes over the
users of the entries the drafts OWN (their compiled objects' data, mapped to main.dol through main.elf's relocations at
the same sites), not over every entry they read. The "constant placed with its first user, the others reference it"
alternative does not work: the constant is the compiler's local `@N` symbol, which the asm users cannot name, and
writing the conversion by hand around an extern would be a fake match. Result after step 64: still **no file run is
complete**, because each conversion constant is shared by the whole source file: fn_80008F8C 127 functions of
0x80008ED8-0x8001AA98 (55 at 100%; before: "not a file" through the string 0x801B6620, which is not owned), closed
129 (56); fn_800683B4 / fn_800694C4 38 (8); fn_800858B0 27 (1); fn_80112A0C 27 (10), closed 34 (15);
fn_800A54A4 / fn_800A5638 / cr_C_ACT_MOVE_LENG / cr_C_ACT_MOVE 458 (40), closed 1,340 (242: some entries are shared
across files, e.g. 0x80451290 by 52 functions). A function whose conversion constants only it uses links as a
one-function file object as soon as it matches (10 such functions, e.g. fn_8001EB48, fn_80022830; nearest fn_8005C4D0
at 85%).

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
