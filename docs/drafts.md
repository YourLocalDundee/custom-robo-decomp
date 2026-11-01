# Game-code drafts: m2c, compile, compare (batch 12)

Milestone 5 (game code to matching C, about 1,750 functions) needs a first C version of every function. This batch makes
one for all of them, checks each against the original, and says where to start. The drafts take about 26 minutes (1,552 s on Oct 31, 2026), parallel
(two m2c passes, the repair loop and the variants of every draft below 100%, plan.md steps 69 and 77; 6.5 minutes after step 59,
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
   variants (step 69 late: register_order also takes its names, `var_r29` without m2c's `_N` suffix: 7 more, e.g.
   fn_80111EEC, fn_80068CFC, fn_80104C54). 44 matches need it (fn_80017668, fn_8001D05C / D104 / D1AC, fn_8000DA5C / DB94 / DCCC, fn_800F67FC,
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
   table_call_noargs; or only ever loaded from a byte field: fn_80077648 / 768 / 888), `invert_early_returns` (`if (c) { return; } rest` at the function's end -> `if (!c) { rest }`),
   `chain_assign` (`A = v; B = v;` -> `B = A = v;`); no match needs the last two yet. The variants and --reg-vars now run for every compiled draft
   below 100% (VARIANT_MIN 0: 3 more matches below 40%); the draft step takes about 26 minutes (1,574 s; was 445 s).
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
451** (101 new, none lost), bytes equal after alignment 428,472 -> 490,044; histogram 0-9% 67 -> 46, 10-19% 93 -> 73, 20-29%
152 -> 105, 30-39% 294 -> 121, 40-49% 136 -> 268, 50-59% 159 -> 127, 60-69% 138 -> 151, 70-79% 156 -> 157, 80-89% 114 -> 135,
90-99% 43 -> 69. **Linked: 420 game objects** (416 promoted, 3 file objects, 1 kept; was 342); the first three FILE
OBJECTS: `game/fn_8005C4D0_fn_8005C4D0.c`, fn_8005C4D0 with its own int-to-float constant (.sdata2
0x804509A0-0x804509A8), which no other function uses (round 5's prediction), and `game/fn_800AA130_fn_800AA130.c`,
fn_800AA130 (a vowel test, a switch) with its own jump table (.data 0x8022B130-0x8022B204) and
`game/fn_80111EEC_fn_80111EEC.c` with its own conversion constant (.sdata2 0x80451888-0x80451890):
`gen_dtk_splits.game_file_data` now takes a data section with relocations when every one is an ADDR32 into the file's
own code (main.elf must relocate each word to that address; the words are left out of the byte compare), and
`tools/shift_test.py` puts no insertion point into a piece of a C file with `#pragma force_active` (reassembled from
dtk's asm, the unreferenced function lost its force_active and mwld stripped it: the control link was not retail),
and the drafts read such an object's own jump table (dtk names it `"@16"`, entries `.rel fn, .L_x`) as
`jumptable_<address>`, so the draft of a file object with a switch stays 100%. 0xB880 bytes = 3.84% of the game
code; 578 of 913 units from C, 16.90% of all code, data from C 3.78%. Batch 9:
main.dol retail SHA-1, every object 100% code + data, static shift PASS; checksums written.
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

## Eighth generator (plan.md step 77, Oct 31, 2026)

Round 7 (branch drafts2), aimed at types first: a typed global or struct field fixes every draft that touches it.
Baseline: the step 69 generator on the step 76 build gives 451 at 100%, 1,703 compile, 490,720 bytes equal. Method as
in round 6 (scratch tools: an objdump side-by-side of draft and original, a classifier of the differing blocks, and a
"measure" script that applies a candidate rule to every current draft below 100%, compiles and scores it, so a rule
is ranked by the drafts it raises / lowers before a 30-minute full run). Every fix is in `tools/draft_batch.py` or in
the types files; no draft is edited. Found by compiling hand-made variants of the near matches first, then written
as rules:
1. **Types: objects the code addresses from one base, declared as scalars** (symbols/cr_types.h, cr_globals.txt).
   `cr_g_SeqCurrent` was a 4-byte `cr_Seq`, but dtk's object at 0x80321D88 is 0x48 bytes and the game code takes its
   address 643 times (`lis/addi`, then a field offset); `cr_g_FileSysReady` was a `u8`, dtk's object at 0x80321CB4 is
   0x8C bytes (the read queue that cr_globals.txt names field by field at addresses the code never reaches as symbols
   of their own). m2c then wrote `M2C_FIELD(&cr_g_SeqCurrent, s32 *, 0x40) = 2`, and the address of a scalar compiles
   differently from a member of a struct object (`li r0,2; stw` where the original keeps the 2 in r5 for a later
   store; tested: a cast to a struct type, `(*(T *) &x).f`, does not reproduce it either, only an object declared
   with the struct type). New structs `cr_SeqState` (current / previous / next sequence, the handlers' counters; 0x48)
   and `cr_FileSysState` (ready flag, queue head / tail, chunk length, current read, the 32-slot ring; 0x8C) with
   `/* +XX */` offsets and ASSERT_SIZE comments; the globals take them. Matched 6 (cr_SEQ_ENDING_FrameA, cr_SEQ_MENU_FrameA,
   cr_SEQ_ILLUSTRATED_BOOK_FrameA, cr_SEQ_TITLE_Enter, fn_8000DE08, fn_80125BB0: round 6's `li r5,2` class, which
   needed no inline function after all), fn_800191E8 too. `cr_FileSlot.read_buffer` is `u8 *` (the code advances it
   by the bytes read; as `void *` two drafts stopped compiling once they read the slot through the typed queue).
2. **Types of `lbl_` objects from the code** (`load_signs`, `data_decls`): a `2byte` object whose every halfword load
   is `lha` is `s16` (12 objects; dtk calls every one u16: `(s16) lbl_801C6C00[0x20]` compiled to lhz + extsh); an
   object the code reads or writes beyond its dtk size through its own address (`li r3,lbl_8044F87C@sda21; lbz
   r0,1(r3)`) is declared with that extent when it still ends before the next symbol (5 objects, 150 + 60 + 56
   accesses of the first three; dtk sized them by the first byte, and m2c wrote `M2C_FIELD(&lbl, u8 *, 1)` of a
   scalar).
3. **Stack locals as one struct** (`stack_struct`, a variant): m2c names a local `spN` only for a stack slot the
   original read or wrote in memory, so the source kept that value in memory (a member of a local struct or array:
   `Vec3 d; d.x = ...`). As scalars the compiler gave them registers (f30 / f31) and dropped the loads and stores. The
   variant declares all `spN` of a draft as members of one local struct at their offsets (padding between) and
   writes `sp.spN`. With it, **struct copies** (`struct_copies`): three word copies into consecutive members from
   consecutive offsets of one pointer, one of them an integer word between floats, are one `Vec3` assignment (the
   original copies through integer registers, `lwz` / `stw`: HSD_JObjGetTranslation's `*translate = jobj->translate`).
   241 kept drafts use it; 8 matches need it (fn_8006FBE0, fn_8001E3B8, fn_8002B284, fn_80050F90, fn_80085280,
   fn_80099DE0, fn_800AA024, fn_80029C84).
4. **Compound assignment in a condition** (`assign_in_cond`): `t = X & C; X = t; if ((t != 0) && ...` -> `if (((X &=
   C) != 0) && ...` (`rlwinm. r0; stw r0; beq`: the value the assignment stored is tested; as a temporary it took a
   callee-saved register). Changed 25; fn_8006FBE0, fn_80045C08, fn_800459A0.
5. **Aliases of a base plus an offset** (`alias_offsets`, a variant): `t = b + K;` and later `M2C_FIELD(b, T *, K)`:
   m2c folds the access at offset 0 of the alias back into its base (fn_80029C84's two Vec4s of a camera at +0x28 and
   +0x38, stored through r29 / r28 in the original) -> `M2C_FIELD(t, T *, 0)`; 3 matches (fn_80029C84, fn_80066904,
   fn_8008F890).
6. **Calls through a global's first word** (`global_call_self`): `((M2C_UNK (*)(u32 *)) *lbl_X)(lbl_X)` loses the
   argument (the original's r3 holds only `X@ha`), and the test of that word before the call is a pointer test,
   `(void *) *lbl_X != NULL` (with void_null `addi; li r0,0; lwz; cmplw`): fn_8006FB9C.
7. **More variants**: `hsd_inlines` (m2c's expansion of HSD_JObjMtxIsDirty, this revision's assert line 605, back
   into an inline function with its own `result`: right register count, no match yet; as a rule it lost
   fn_80095A30), `pointer_tests` with void_null (`(u32) lbl_X != 0U` -> a null-pointer test: fn_800DF2A0's `li r0,0;
   cmplw`), `drop_same_assign_casts` (`= (s16) lbl_801C6C00[0x20];` on an s16 array: the cast cost a register,
   fn_80045B40). The variants are tried for every compiled draft below 100%; the draft step now takes about 31
   minutes.
**Before -> after** (cycle 1, full runs; m2c, compiler, flags unchanged): compile 1,703 -> 1,701 of 1,751, **100% MATCH
451 -> 473** (22 new, none lost), bytes equal after alignment 490,720 -> 515,408; histogram 0-9% 46 -> 40, 10-19% 73
-> 63, 20-29% 104 -> 77, 30-39% 121 -> 109, 40-49% 268 -> 258, 50-59% 128 -> 143, 60-69% 152 -> 159, 70-79% 156 ->
165, 80-89% 133 -> 134, 90-99% 71 -> 80. **Linked: 436 game objects** (432 promoted, 3 file objects, 1 kept; was
420), 0xC4D0 bytes = 4.10% of the game code (was 3.84%); 638 of 911 units from C, 21.65% of all code (was 21.48%).
Batch 9: main.dol retail SHA-1, every object 100% code + data, static shift PASS; batch 8 0 FAIL; checksums written.
**Cycle 2** (the two variants of item 7 and `read_buffer`): 473 -> **477** (fn_80045B40, fn_80015E18, fn_800199AC,
fn_8010FD38, all with drop_same_assign_casts), compile 1,701 -> 1,703 (the two drafts of item 1's `read_buffer`),
bytes equal 517,256; histogram 0-9% 40, 10-19% 63, 20-29% 77, 30-39% 109, 40-49% 257, 50-59% 143, 60-69% 158, 70-79%
165, 80-89% 134, 90-99% 80, 100% 477. **Linked: 439 game objects** (435 promoted, 3 file objects, 1 kept), 0xC65C
bytes = 4.13% of the game code; 641 of 916 units from C, **21.68% of all code**, data 5.89%; batch 9 retail SHA-1,
static shift PASS, batch 8 0 FAIL, decomp checksums written. The cycle gained fewer than 10, so the round stops here
(39 matched drafts with data of their own wait for their file runs).
Not found / left for the next round:
- **file runs**: the smallest, fn_8006EEF8..fn_8006FDD0 (7 functions, own int-to-float constants), now waits on one
  function: fn_8006EEF8 (0xBCC bytes, 32%; the original keeps 14 float and 18 integer locals in callee-saved
  registers, f18-f31 / r14-r31, the draft three floats: a hand decompilation); fn_8002444C matches and owns two
  .sdata2 floats, but its compiled pool (literals merged, 88.0f once) does not lay out like the original's (two piece
  starts); the next runs wait on 5 functions each (0x800057B8.., 0x8008A0A8..);
- the script-command head (~40 drafts at 78-85%): a pointer to the command counter as a local reproduces the saved
  but unused r30 (the variable's register survives, its value moves to r5), but no statement order or struct form
  (also with the state as a struct object) gives the original's address scheduling (best 85%);
- m2c's `*(p + off)` through a `void *` parameter that the repair loop made `u8 *` loads a byte where the original
  loads a word (`lwzx`: fn_8008688C); parameter register order (fn_80075258 family: the parameters in r30 / r29
  without copies come out swapped); `|=` on an s8 local without the `extsb` (fn_80061BE8); stack frames larger than
  the struct of the named slots (fn_8002EBB0: 0x20 more, locals the code never touches);
- `lha` vs `lhz` through struct fields typed by m2c (`(s16) chara->angle` of the s32 at +0x58 reads its low half).

## Naming round 3 types (plan.md step 79, Oct 31, 2026)

Not a generator round: the script engine state (`cr_g_Script`, 0x803BE358, 0x484), the scenario state (`cr_g_Scenario`, 0x803BF914,
0x364, with two `cr_ScenarioSlot`), the controllers (`cr_g_Pads`, 4 x `cr_Pad` of 0xA4 at 0x8031FED8) and the game-mode block
(`cr_g_GameMode`, 0x80321D40, 0x48) became typed structs in symbols/cr_types.h, with the scalars `cr_g_ScriptPC`, `cr_g_ScriptDepth`,
`cr_g_ScriptReturnStack`, `cr_g_ScenarioSlotPtr` and `cr_g_PadStatus` (symbols/cr_globals.txt). The drafts of the 300 script-command
handlers now read `cr_g_Script.cmd_history[cr_g_Script.history_index] = *cr_g_ScriptPC;` instead of
`M2C_FIELD(&lbl_803BE358[lbl_803BE358[2]], s32 *, 0x4C)`. Two variants came with the types (`drop_same_member_casts`,
`expand_ptr_compound`: a typed member's same-type cast costs a register; a compound assignment through a global pointer loads the
pointer once where the original loads it twice). Result of the one full run: 100% MATCH 477 -> 477, compiled 1,703 -> 1700, bytes equal
517,256 -> 512,448; 439 game objects linked; 218 handler drafts about 8 points lower (m2c folds the counter pointer temporary that
reproduced the saved r30 into the typed member: a rule for the next round), 25 higher, 2 new compile failures.

## Ninth generator (plan.md step 80, Oct 31, 2026)

Round 8 (branch drafts3), aimed at the script-command head that round 3's types had cost 8 points on about 220
drafts. Baseline: the step 79 generator on the step 79 build gives 477 at 100%, 1,700 compile, 512,448 bytes equal,
192 drafts at 80-99%. Method as in rounds 6 and 7: a scratch compile-and-compare of hand-written spellings against
the original's asm (dtk's asm for the original, `dtk elf disasm` for the compiled draft, difflib side by side), the
pure head first (cr_CM_DUMMY_008: the 38-word handler that does nothing but the head), then the rule, then the full
run. Every change is in `tools/draft_batch.py`; no draft is edited.
1. **`script_counter_ptr`** (a default rule; the skip is a variant): the counter's address goes back into a local
   pointer for the struct-typed form, `cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth];` right after the PC is
   read, `(*cmd_cnt)++;` where the draft had `cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;`. Why the original saves
   an r30 it never uses, found with micro-tests: at level 0 every C local gets a callee-saved register; the pointer
   local is `addi r30, r4, 0xC` and the peephole then folds its only use back onto the base register that is still
   live (`lwz r3, 0xc(r4); addi r0, r3, 1; stw r0, 0xc(r4)`), so the instruction goes and the save stays; when the
   base register is clobbered first (the pointer set right before its use, or after the stores) the pointer is kept
   and used (`stw r0, 0(r30)`, one word more). The rule puts 220 handler drafts back where the untyped drafts were
   (cr_S_BGM_STOP 78 -> 85, cr_CHAR_MOVE 81 -> 85, cr_SYS_SEL_JP 70 -> 81); 26 kept the skip variant (cr_SYS_CALL,
   the cr_E_WIN group: handlers that also set the depth, or whose tail m2c misreads).
   **85% is the ceiling** of this head. Tried on the pure head and on cr_S_BGM_STOP, all compiled and compared: the
   pointer set at each of the six positions, four spellings of the increment (`(*p)++`, `*p += 1`, `*p = *p + 1`,
   `++*p`), three of the index step (`+= 1`/`&=`, `++`, `x = x + 1`/`x = x & 0x3F`, `%= 0x40`), `*pc` as `pc[0]`,
   the PC advance as `pc + n` and as `x = x + n`, a local index, a local command word, a pointer to the history
   array, a pointer to the state struct (as a local and as an inline function's parameter), the depth as a local
   or as an inline's parameter, commuted subscripts, `volatile`, a block scope, declarations with initializers, and
   all 120 orders of the five statements with and without the pointer (the compiler moves the PC store, which is
   to another symbol, across the struct's stores: the order `history, index + 1, PC, mask, count` gains one word).
   None reaches the original's schedule: it hoists the `addi r4, r4, cr_g_Script@l` of the history store into the
   prologue and loads cr_g_ScriptDepth before the `lha` of the command word, where ours materialises that base as
   `addi r0, rX` next to its `add` and loads the depth late; the register numbers say the original allocated the
   depth value into r0 before the history store's base, which no statement order of ours reproduces. Left as the
   scheduler's / allocator's (a different `-opt` or `-sched` setting for this file is the next thing to test).
2. **Width of a load through a `void *` / integer base** (m2c_run.py `_width_format`): `*(arg0 + i * 4 + 0xAC)` of a
   `void *` parameter that the repair loop makes `u8 *` compiled to `lbz` / `stb` where the original has `lwzx` /
   `stwx` (fn_8008688C): the access gets its own width through M2C_FIELD as the byte-array case already did (a
   word now; 96% stays: the original's `lwzx r0, r31, r0` keeps the sum `i * 4 + 0xAC` in a register where ours
   folds the constant into a displacement, `add; lwz 0xac(r3)`). **Cycle 2** tried the offset as the macro's own
   argument (`M2C_FIELD(arg0, u32 *, k)`, which does give the indexed load): 477 -> 461, it lost 16 matches whose
   originals fold the constant (fn_8007A1D0, fn_8007A32C, fn_8007A6D8, fn_8007AAB4, fn_8007BD64, fn_800AB468,
   fn_800ABA8C, fn_800AC0A8, fn_800AC2D0, fn_800AD5AC, fn_800AD984, fn_800ADD58, fn_800AE8AC, fn_800AEC80,
   fn_800AF054, fn_800FAC00: 100% -> 82-84%; 19 drafts up by 2-4 points, bytes equal 518,020) and gained no match,
   so it is reverted (`_split_field` keeps the note) and cycle 3 is cycle 1's generator again. The matched
   byte-array drafts (fn_8000768C, fn_800BC9A4, fn_8010D2D0, fn_8001B3F0, cr_C_ACT_MOT) are unchanged.
3. **A store through a pointer cast to a struct type** (`*((cr_Pad *) ((u8 *) p + off)) = 0`, fn_80020204: a byte at a
   variable offset into cr_g_Pads; the compiler cannot assign an int to a struct): the repair loop stores a byte
   there (compiles, 86%; the width is the asm's, not knowable from the C).
4. **A pointer difference of two addresses** (`cr_g_Scenario.part_bits[0] - &cr_g_Scenario`, fn_800EB518: the byte
   offset of a member as m2c spelled it; type mismatch): both as `u8 *` in the repair loop (compiles, 84%).
**Before -> after** (cycle 1, full run; m2c, compiler, flags unchanged): compile 1,700 -> 1,703 of 1,751 (+ fn_80020204,
fn_800EB518, cr_Chara_BuildModel; - cr_Scenario_Reset: m2c types a stack slot with a struct that has no `unk_470`),
**100% MATCH 477 -> 477**, bytes equal after alignment 512,448 -> 518,120 (+5,672); histogram 0-9% 42 10-19% 61 20-29% 72 30-39% 115 40-49% 253 50-59% 142 60-69% 157 70-79% 176 80-89% 133 90-99% 75; 80-99%
192 -> 208. Against the step 79 report 207 drafts up, 56 down (none of them a match; the ones down are handlers that
chose the skip variant on a tie of the full rules, within 1 point). **Linked: 439 game objects** (435 promoted, 3 file
objects, 1 kept), 0xC65C bytes = 4.13% of the game code; 642 of 916 units from C, 22.10% of main.dol's code: unchanged,
no new match. Batch 9: main.dol retail SHA-1, static shift PASS.
**Final state (cycle 3 = cycle 1's generator, verified)**: compile 1,703, 100% MATCH 477 (none), bytes equal
518,120; histogram 0-9% 42 10-19% 61 20-29% 72 30-39% 115 40-49% 253 50-59% 142 60-69% 157 70-79% 176 80-89% 133 90-99% 75; 80-99% 208. Linked 439 game objects, 642 of 916 units from C,
22.10% of main.dol's code; batch 9 retail SHA-1, static shift PASS, batch 8 0 FAIL, checksums written. No cycle
gained 10, so the round stops here: the head's 220 drafts moved up 7 points each but none to 100%.
Not found / left for the next round:
- the script-command head's schedule (item 1: the family is at its 85% ceiling with the compiler flags as they are;
  about 220 drafts, the single largest class: test `-sched` / `-opt` variants for this one file, then an inline
  function with the argument count once a spelling matches);
- cr_SYS_CALL and the depth-setting handlers (70%): `M2C_FIELD(cr_g_Script.script_name, s8 **, cr_g_ScriptDepth)`
  indexes a pointer array by bytes (m2c's offset for `script_name[depth]`); fn_8008688C (96%): item 2's indexed
  load needs the offset form that cycle 2 showed costs 16 matches elsewhere (a rule keyed on the variable part of
  the offset, `(i * 4) & 0x3FC`, might separate the two);
- round 7's leftovers unchanged: parameter register order (fn_80075258: `mr r30, r3` where the draft spills r3 and
  reloads it), `|=` on an s8 local without `extsb` (fn_80061BE8), stack frames larger than the named slots
  (fn_8002EBB0), `lha` through m2c-typed struct fields; the 46 compile failures (pointer/array required 9, illegal
  operand 9, illegal implicit conversion 9, illegal type 7, cr_Scenario_Reset's stack struct);
- file runs: fn_8006EEF8 (0xBCC bytes, 32%, 14 float and 18 integer callee-saved locals) still closes the smallest
  run alone; fn_8002444C's constant pool order: not reached this round.

## Naming round 4 types (plan.md step 81, Oct 31, 2026)

Not a generator round: the save data (`cr_g_SaveData`, 0x803BE7DC, 0x1138, with `cr_BattleRecord` and two `cr_UnlockBits`), the unlock bits
in use (`cr_g_UnlockBits`, 0x804198B0, 0xA8), the stage selector (`cr_g_StageSelector`, 0x8041A244, 0x354) and the customize screen
(`cr_g_Customize`, 0x8041CDE4, 0x1B54) became typed structs in symbols/cr_types.h, with the scalar `cr_g_ScriptCharaAct` (0x8044F774) and nine
function names (cr_Bits_Set / Clear / Test, cr_Mem_CopyWords, cr_Save_ApplyUnlocks, cr_StageSelector_Init / Update, cr_Customize_SetMode,
cr_Scenario_Init). `cr_ScriptState.script_name` is now `void *[16]`: m2c writes `script_name[cr_g_ScriptDepth]` for it where the `char *[16]`
gave a byte-offset `M2C_FIELD(cr_g_Script.script_name, s8 **, cr_g_ScriptDepth)` (cr_SYS_CALL 70 -> 80%; `u8 *` has the same problem, `s32` and
`void *` do not). cr_Scenario_Reset's compile failure is not a struct: m2c reuses one stack slot (`sp48`) for `&cr_g_Scenario` and
`&cr_g_Script`, so `sp48->unk_470` is typed with the wrong struct; the same slot reuse fails cr_Save_ResetData (`sp10[0x410]`), fn_8011023C
(`sp50[6]`) and fn_800F21C8 - a generator rule for the next round (a slot that holds two globals' addresses is two locals).
Result of the one full run: compile 1,703 -> 1,703 of 1,751, **100% MATCH 477 -> 477** (lost: fn_80111E0C 100->77, fn_80111EEC 100->92), bytes equal 518,120 -> 518,064; histogram 10-19% 61 -> 61 20-29% 72 -> 72 30-39% 115 -> 115 40-49% 253 -> 254 50-59% 142 -> 141 60-69% 157 -> 154 70-79% 176 -> 174 80-89% 133 -> 136 90-99% 75 -> 76; 80-99% 208 -> 212. Against the step 80 report 226 drafts up (cr_BattleRobo_InitFromLoadout 35->36, cr_Save_ApplyUnlocks 52->53, fn_80022830 32->34, cr_Mem_CopyWords 70->100, fn_8008E3F8 62->68, fn_8008FA8C 63->69), 22 down (cr_Bits_Clear 40->33, fn_8007E400 45->44, fn_8009AEC4 10->8, fn_800A0C20 97->86, fn_800AC158 78->75, fn_800AC910 64->53); newly compiling: fn_80020204 (86), cr_Chara_BuildModel (47), fn_800EB518 (86). **Linked: 441 game objects** (436 promoted, 3 file objects, 4 kept), 0xC704 bytes = 4.14% of the game code; 644 of 918 units from C, **22.11% of main.dol's code** (data 5.90%).

## Tenth generator (plan.md step 82, Oct 31, 2026)

Round 9 (branch drafts4), on the step 81 build and names. Baseline: the step 81 generator run again here gives the
committed counts exactly (477 at 100%, 1,703 compile, 518,064 bytes equal, 212 at 80-99%, 46 compile failures), so
the per-function comparison below is against that run. Method as in rounds 6-8: a scratch compile-and-compare of
hand-written spellings against the original (dtk's asm; `dtk elf disasm` of the compiled draft; side by side), then
the rule in `tools/draft_batch.py`, then `--only` on the targets and on every draft the rule's pattern occurs in
(stock against patched, same output), then the full run. No draft is edited.
1. **`split_slot_structs`** (default): m2c types a stack slot once, but the original reused the slot for the
   addresses of two typed globals (`sp48 = &cr_g_Scenario; ... sp48 = (cr_ScenarioState *) &cr_g_Script;
   sp48->unk_470 = 0;`), so the second's members were looked up in the first's struct and the draft did not compile
   ("'unk_470' is not a struct/union/class member", "pointer/array required" for a slot declared `u32`). Every
   assignment of a typed global's address to a slot local (`spN = &cr_g_X;`, with or without a cast) starts a binding;
   when a slot is bound to more than one struct type, or to one other than its declared type, each binding gets its
   own pointer local `spN_cr_g_X` typed with the global's struct, and the slot's uses up to its next assignment are
   renamed to it. cr_Scenario_Reset, cr_Save_ResetData and fn_8011023C compile (31%, 8%, 16%: far from a match, but
   they are in the loop now). Not fn_800F21C8: its failure is another class, `void *` arithmetic m2c writes for a
   slot it typed `void *` (`sp234 = sp230 + temp_r0_109;`, "illegal type").
2. **`unsigned_not_mask`** (default): `cr_g_StageSelector.flags &= ~arg0;` on a u32 member with an s32 operand gives
   the base register the compiler's second choice (`lis r5 ... lwz r0, 0x10(r5)` where the original has r4, 77%);
   `&= ~(u32) arg0` gives the original (fn_80111E0C 77 -> 100%). The ten other spellings tried (the expanded
   `x = x & ~a`, through M2C_FIELD at the offset, through a struct-pointer cast, `(u32) ~a`, `~(s32) a`, a `u32`
   parameter) give 10 or 2 of 18 words. The rule casts when the member is unsigned and the operand is an s32 local
   or parameter of the draft.
3. **`member_array_index`** (default): `M2C_FIELD((&cr_g_StageSelector + ((var_r28 * 2) & 0x1FE)), u16 *, 0x3E)` is
   pointer arithmetic on the struct (a step of sizeof, `mulli r6, r5, 0x354`); when the struct has an array of that
   element width at that offset (stage_msgs, u16[42] at +0x3E) the access is `cr_g_StageSelector.stage_msgs[var_r28]`
   (fn_80111EEC 92 -> 100%). The struct layouts come from symbols/cr_types.h (`member_offsets()`).
4. **`indexed_offset`** (default; round 8's cycle 2 with the gate it lacked): `M2C_FIELD((arg0 + (((var_r30 * 4) &
   0x3FC) + 0xAC)), u32 *, 0)` compiles to `add; lwz 0xac(r3)` (the constant folded into the displacement) where
   the original keeps the sum in a register and loads indexed (`addi r0, r3, 0xac; lwzx r0, r31, r0`), the form the
   compiler gives `p->array[i]`; the offset as the macro's argument, `M2C_FIELD((arg0 + ((var_r30 * 4) & 0x3FC)),
   u32 *, 0xAC)`, compiles so (micro-test: of ten spellings only this and `*(u32 *) ((p + i * 4) + 0xAC)` give the
   indexed load). Round 8 applied it to every such access and lost 16 matches whose originals fold the constant;
   those are record arrays (`lbl[i].field`, `(i * 0x78) + 0x2C` for an s32 field) and global bases. The gate: the
   base is a pointer variable (not an `lbl_` / `cr_g_` address) and the index's scale equals the access width (an
   array of the accessed type at the constant offset). fn_8008688C 96 -> 100% (its `HSD_JObj **` access included;
   the pattern occurs in no other draft of the run).
5. **The script-command head: flags and pragmas** (plan.md step 80 left this as "the scheduler's"). Compiled and
   compared on the pure head (cr_CM_DUMMY_008, 38 words) with every spelling at the ceiling (18 of 38 words equal
   by position, the same 38 instructions in another order and other register numbers): the game flags without
   `schedule` (1 of 39), without `peephole` (4 of 39), without both, `space` instead of `speed`, level 1 and 2 with
   and without peephole / schedule, `-O0`, `-O0,p`, `-O1`, `-O1,p`, `-O2`, `-O3`, `-O4,p`, `-schedule on` with
   `noschedule`, `cse` / `nocse`, `dead` / `nodead`, `prop` / `noprop`, `lifetimes` / `nolifetimes`, without
   `-use_lmw_stmw`, without `-fp_contract`, `-sdata 0 -sdata2 0`, `-inline off` / `all`, `-char unsigned`, `-str
   reuse`, `-common on`; the processor models `generic`, 505, 555, 603, 603e, 604, 604e, 750, 7400, 7450, 801, 821,
   823, 860 (the schedule changes with the model: 603 / generic / 5xx / 8xx give 10 of 38, 604 / 7450 give 2-3,
   750 / 7400 / gekko give the 18); and the pragmas `scheduling off / on / 603 / 604 / 750`, `peephole off`,
   `optimization_level 0 / 1 / 2`, `global_optimizer off / on`, `opt_common_subs off`, `opt_lifetimes off / on`,
   `opt_propagation off`, `opt_dead_assignments off`, `opt_dead_code off`, `optimize_for_size on / off`,
   `opt_strength_reduction off`, `opt_loop_invariants off`, `opt_unroll_loops off`, `opt_vectorize_loops off`,
   `optimizewithasm off`, `processor 750 / 603`, `fp_contract off`, `readonly_strings on`, `pool_data on / off`,
   `ANSI_strict on`, `sym on`, `no_register_save_helpers on`, `no_register_coloring on / off`, `inline_depth 0`,
   `dont_inline on`, `merge_float_consts off`, `explicit_zero_data on`, `function_align 4`, `force_active on`.
   **None changes the 18 of 38**; every setting that changes the code moves away from the original. One finding
   about the spelling: the original reloads cr_g_ScriptPC for the advance (`lwz r31, PC; lha r7, 0(r31); lwz r3, PC;
   addi r0, r3, 4; stw r0, PC`), so the head is `pc = cr_g_ScriptPC; ... = *pc; cr_g_ScriptPC += 2;`, not `pc + 2`
   (that spelling gives the same 38 instructions as the original; the drafts' `pc + 2` form gives 37 and computes
   the advance from r31). With that, all 60 statement orders (`++` before `&=`), with the counter pointer set before
   or after the PC local, still peak at 18. The register numbers (the original's three `lis` of the struct's address
   in r3, r4, r5 at the top, the `&=` statement's base allocated before the `++` statement's although its load comes
   after) are the scheduler's renaming of a pre-schedule order no statement order of ours produces, so the ~220
   handler drafts stay at 78-85% with the game flags; no per-file flag or pragma reproduces the head.
6. **fn_8006EEF8** (0xBCC bytes, 32%, the only function keeping the file run fn_8006EEF8..fn_8006FDD0 open): read
   with the draft and the asm side by side, not written. It is a debug grid drawer (GX line setup, PSMTXScale, a
   loop over -21..21 with two PSMTXMultVec per vertex pair and GXBegin GX_LINES / GX_LINESTRIP, the vertices written
   to the write-gather pipe at 0xCC008000 as 3 f32 + 4 u8). The original keeps the transformed vertex components in
   f23-f31 across the GXBegin calls, the pipe's high half `lis rN, 0xcc01` in r14-r18 (one per store, hoisted), and
   copies 12 three-float groups to the stack (sp8..sp70) before writing them: the shape of f32 parameters of an
   inlined helper (an inlined function's parameters live in the caller's frame at level 0), with the first four
   vertices written without copies (macros). The draft writes the pipe through casts of the constant address and
   keeps nothing in registers across the calls (32%). Matching it needs the helper's signature and the macro form
   found by trial against 755 instructions; not attempted in this round (an hour was the budget; the register
   analysis above is the start).

**Before -> after** (one full run each, tools/batches/batch12_drafts.ps1, 1,892 and 1,830 s; a second full run after the
rule-2 gate, which the first run showed was needed: cr_F_CLR 79 -> 76 with the cast through its global pointer):
compile 1,703 -> 1,706 of 1,751 (+ cr_Scenario_Reset, cr_Save_ResetData, fn_8011023C), **100% MATCH 477 -> 480**
(+ fn_80111E0C, fn_80111EEC, fn_8008688C), bytes equal 518,064 -> 520,140; histogram 0-9% 43 -> 44, 10-19% 61 -> 62, 20-29% 72 -> 72, 30-39% 115 -> 116, 40-49% 254 -> 253, 50-59% 141 -> 142, 60-69% 154 -> 152, 70-79% 174 -> 175, 80-89% 136 -> 135, 90-99% 76 -> 75; 80-99% 212 ->
210. Against the baseline report 21 drafts up (fn_800AAF74 64 -> 70, fn_800AC910 53 -> 58, fn_800AD094 53 -> 58,
fn_800AF0E0 66 -> 71, fn_800D0E60 47 -> 52: the `indexed_offset` form), 0 down, no match lost.
**Linked: 442 game objects** (438 promoted, 3 file objects, 1 kept: fn_800A0804; fn_8008688C and
fn_80111E0C promoted, fn_80111EEC's file object written from its draft again), 645 of 920 units from C,
**22.17% of main.dol's code** (game code 0xCB18 bytes = 4.23% of the game code). Checks: batch 9 retail SHA-1, static shift PASS; batch 8 0 FAIL;
checksums written (the 3 decompiled\ lines kept at the committed values).

**promote_drafts.py** (round 4's bug): a promoted function renamed in symbols/descriptive.txt (fn_80125884 ->
cr_Customize_SetMode) got its new file written while the old one stayed as "kept (no draft)", and batch 9 failed on
the overlapping split. The kept check now reads the file's header address (`.text 0xA-0xB`): a kept file is dropped
when its function's current name is not ours, when the name's address is not the header's, or when a file written
by this run owns an address in its range. The stale `fn_80125884 kept` line is gone from promote_report.txt.

**Left**: fn_800F21C8's `void *` arithmetic (and the 45 other compile failures: pointer/array 9, implicit
conversions 10, illegal operand 9, explicit conversions 6, illegal type 6), the handler head (item 5: a different
pre-schedule order, which only another source shape would give; the inline-function and macro forms of plan.md
step 80 were tried; a `do { } while (0)` macro of the whole head with the PC reload gives the same 18 of 38, an
inline function with the advance as its parameter 16),
fn_8006EEF8 (item 6), round 7's four classes.

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
Step 69: **the first three file objects link**: fn_8005C4D0 and fn_80111EEC (their int-to-float constants) and fn_800AA130 (its jump table:
a data section of its own with relocations is taken when every relocation is an ADDR32 into the file's own code and
main.elf relocates that word to the same address). Owned data that only its function uses, by the current drafts (the
compiled draft's own data, so a draft that matches only with `float_literals` owns that constant too): fn_800EECD0
36%, cr_FileSys_RegisterPath 42%, fn_8004FF64 51%, fn_800E4958 58%, fn_80021AB0 68%, fn_8002444C 68%, fn_80080E24 78%,
fn_80053A20 86% (fn_80111EEC 93% then matched: the third file object): each links by itself once it matches.

### Step 83: the run table, string literals, the pool order (Oct 31, 2026, branch fileobjs)

The file runs after step 82 (promote_report.txt "file runs" with the drafts' sizes and match percentages; the runs
ranked by how many functions still block them, a run of one function with a FILE OBJECT line is linked):

| run (first..last) | span | functions | at 100% | blockers | blocker bytes | blockers (size, match) |
|---|---|---|---|---|---|---|
| fn_8002444C..fn_8002444C | 0x8002444C-0x80024578 | 1 | 1 | 0 | 0x0 | .sdata2: its relocations give two piece starts (0x804504E8, 0x804504F0) |
| fn_8005C4D0..fn_8005C4D0 | 0x8005C4D0-0x8005C628 | 1 | 1 | 0 | 0x0 | FILE OBJECT game/fn_8005C4D0_fn_8005C4D0.c (.sdata2 0x804509A0-0x804509A8) |
| fn_800AA130..fn_800AA130 | 0x800AA130-0x800AA17C | 1 | 1 | 0 | 0x0 | FILE OBJECT game/fn_800AA130_fn_800AA130.c (.data 0x8022B130-0x8022B204) |
| fn_80111EEC..fn_80111EEC | 0x80111EEC-0x8011214C | 1 | 1 | 0 | 0x0 | FILE OBJECT game/fn_80111EEC_fn_80111EEC.c (.sdata2 0x80451888-0x80451890) |
| fn_8006EEF8..fn_8006FDD0 | 0x8006EEF8-0x8006FE38 | 7 | 6 | 1 | 0xBCC | fn_8006EEF8 (0xBCC, 32%) |
| fn_8008A0A8..fn_8008BD50 | 0x8008A0A8-0x8008BED4 | 11 | 6 | 5 | 0x14AC | fn_8008A484 (0x22C, 56%), fn_8008A6B0 (0x200, 64%), fn_8008A8B0 (0x238, 57%), fn_8008AAE8 (0x204, 56%), fn_8008ACEC (0xC44, compile failed illegal type) |
| fn_800057B8..fn_8000805C | 0x800057B8-0x8000860C | 16 | 11 | 5 | 0x171C | fn_800057B8 (0x3D0, 43%), fn_80005BC8 (0x170, 56%), cr_BattleRobo_InitFromLoadout (0x8B4, 36%), fn_80007CE4 (0x378, 65%), fn_8000805C (0x5B0, 67%) |
| fn_8008057C..fn_800812A4 | 0x8008057C-0x80081430 | 11 | 4 | 7 | 0xC40 | fn_8008057C (0x418, 64%), fn_80080A94 (0xEC, 98%), fn_80080B80 (0xFC, 92%), fn_80080C7C (0x1A8, 63%), fn_80080E24 (0xFC, 78%), fn_80080FD8 (0x210, 92%) ... +1 more |
| fn_80076B2C..fn_80079C70 | 0x80076B2C-0x80079CB8 | 31 | 19 | 12 | 0x24F8 | fn_80076B2C (0x764, 44%), fn_800779A8 (0x2B4, 57%), fn_80077C5C (0x2DC, 56%), fn_80078000 (0x438, 12%), fn_800784B0 (0x6B0, 47%), fn_80078B60 (0x140, 86%) ... +2 more |
| fn_8007D0D4..fn_8007FCD8 | 0x8007D0D4-0x8008052C | 18 | 6 | 12 | 0x2FF4 | fn_8007D3C8 (0x270, 82%), fn_8007D638 (0x1C8, 77%), fn_8007D800 (0x220, 89%), fn_8007DA20 (0x60, 79%), fn_8007DA80 (0x170, 66%), fn_8007DD60 (0x6A0, 38%) ... +2 more |
| fn_8007D3C8..fn_8007FCD8 | 0x8007D3C8-0x8008052C | 14 | 2 | 12 | 0x2FF4 | fn_8007D3C8 (0x270, 82%), fn_8007D638 (0x1C8, 77%), fn_8007D800 (0x220, 89%), fn_8007DA20 (0x60, 79%), fn_8007DA80 (0x170, 66%), fn_8007DD60 (0x6A0, 38%) ... +2 more |
| fn_8007C094..fn_8007FCD8 | 0x8007C094-0x8008052C | 22 | 9 | 13 | 0x3BF0 | fn_8007C094 (0xBFC, compile failed illegal operand), fn_8007D3C8 (0x270, 82%), fn_8007D638 (0x1C8, 77%), fn_8007D800 (0x220, 89%), fn_8007DA20 (0x60, 79%), fn_8007DA80 (0x170, 66%) ... +2 more |
| fn_8010E47C..fn_80112A0C | 0x8010E47C-0x80112AB8 | 27 | 13 | 14 | 0x3E70 | fn_8010E47C (0x3A4, 75%), fn_8010E820 (0x1BC, 53%), fn_8010E9DC (0x180, 48%), fn_8010EB5C (0x520, 55%), fn_8010F07C (0x800, 12%), fn_8010F87C (0x4BC, 20%) ... +2 more |
| cr_Bgm_GetVolume..fn_80098484 | 0x80096594-0x80098F4C | 25 | 8 | 17 | 0x272C | fn_800965F4 (0x160, 44%), fn_80096754 (0x160, 70%), fn_800968B4 (0x374, 57%), fn_80096C28 (0x100, 68%), fn_80096D28 (0x100, 75%), fn_80096E28 (0x94, 65%) ... +2 more |
| fn_800527D4..fn_8005BE70 | 0x800527D4-0x8005BEC8 | 18 | 1 | 17 | 0x9610 | fn_800528B8 (0xE4, 68%), fn_8005299C (0xA94, 17%), fn_80053430 (0x5F0, 27%), fn_80053A20 (0x388, 96%), fn_80053DA8 (0x490, 26%), fn_80054238 (0x1ECC, 16%) ... +2 more |
| fn_80081440..fn_80089140 | 0x80081440-0x80089550 | 27 | 3 | 24 | 0x77BC | fn_80081440 (0x4C8, 45%), fn_80081908 (0x24C, 7%), fn_80081B54 (0x248, 48%), fn_80081D9C (0x5C8, 34%), fn_80082364 (0x69C, 25%), fn_80082A00 (0x584, 21%) ... +2 more |
| fn_80067904..fn_8006B3D0 | 0x80067904-0x8006B46C | 42 | 10 | 32 | 0x3920 | fn_80067904 (0xA54, 10%), fn_80068420 (0x26C, 58%), fn_8006868C (0x2EC, 68%), fn_80068988 (0xEC, 77%), cr_Text_CountChars (0xDC, 60%), fn_80068B84 (0xF8, 51%) ... +2 more |
| fn_8001B1D4..fn_8001F060 | 0x8001B1D4-0x8001F350 | 72 | 31 | 41 | 0x3748 | fn_8001B29C (0xEC, 66%), cr_Rand_Float (0x68, 44%), cr_Vec3_Distance (0x1A8, 71%), fn_8001B6EC (0x668, 52%), cr_Vec3_RotateY (0x1B0, 87%), cr_Vec3_RotateAboutAxis (0x6A8, 70%) ... +2 more |
| fn_80024148..fn_80036FB0 | 0x80024148-0x800373AC | 76 | 15 | 61 | 0x127EC | fn_8002419C (0x2B0, 18%), fn_80024634 (0x19C, 34%), cr_CamMode_DefaultStart (0x60, 70%), fn_80024848 (0x17C, 74%), cr_CAMMODE_SNR_AUTO_Y_ANG_Frame (0xC4, 63%), fn_80024A88 (0x13C, 69%) ... +2 more |
| fn_8008A484..fn_80093AF0 | 0x8008A484-0x80093C30 | 87 | 25 | 62 | 0x8CA4 | fn_8008A484 (0x22C, 56%), fn_8008A6B0 (0x200, 64%), fn_8008A8B0 (0x238, 57%), fn_8008AAE8 (0x204, 56%), fn_8008ACEC (0xC44, compile failed illegal type), fn_8008BED4 (0x180, 96%) ... +2 more |
| fn_801147B4..fn_80124F5C | 0x801147B4-0x80125884 | 79 | 16 | 63 | 0x10850 | fn_801147B4 (0x294, 70%), fn_80114A48 (0x470, 44%), fn_80114EB8 (0x284, 62%), fn_8011513C (0x470, 44%), fn_801155AC (0x36C, 85%), fn_80115918 (0x1F8, 49%) ... +2 more |
| fn_8000768C..fn_801314D4 | 0x8000768C-0x80131910 | 1739 | 473 | 1266 | 0x11AF20 | fn_80007CE4 (0x378, 65%), fn_8000805C (0x5B0, 67%), fn_8000860C (0x180, 89%), cr_Game_Update (0x474, 75%), fn_80008CB0 (0x110, 37%), fn_80009018 (0x554, 79%) ... +2 more |

**fn_8002444C** (the one-function run that matched but could not be placed: "two piece starts") is now the fourth file
object. Its .sdata2 pool (0x804504E8-0x80450518) is 48, 56, 96, 88, "\x96\xb3\x82\xb5", 192, "\x97\x4c\x82\xe8", 72,
"\x81\x96", 160: the float literals (88.0f once, merged by the compiler) with the call's Shift-JIS string literal
AFTER its two floats, every string packed (5 or 3 bytes, the next float 4-aligned). Two things reproduce it byte for
byte: (1) **`-str reuse,readonly`** for the game (configure.py; a literal of 8 bytes or less is then an .sdata2
object of its own, longer ones go to .rodata, where the game's 2,565 strings are; with the default `-str reuse` the
compiler puts them in .sdata / .data, which main.dol never has); the drafts write strings as externs, so no draft's
code changes; (2) **the callee's parameter order**: fn_8006B144 takes `(f32 x, f32 y, const char *text)` (prototype
in symbols/cr_types.h with this evidence; the registers f1, f2, r3 fit either order, the pool order only the one with
the string last), and the draft generator's `float_literals` variant now also writes the **.sdata2 string literals**
the draft takes the address of (`pool_strings`: dtk's `data:string` objects and the untyped .sdata2 objects whose
bytes are a string followed by zeros, every byte a hex escape; only at `(s8 *) lbl_X` / `(s32) lbl_X`), so the draft
owns its whole pool. The same hex-escape form in the promoted file compiles to the same bytes. The 5 other matched
drafts that read .sdata2 strings (cr_SEQ_TITLE_FrameC, fn_8006FBE0, fn_80075120, fn_80094394, fn_80095A30) own no
float, so their strings stay externs and they stay one-function objects (their strings are shared by 2-6 functions
each: the SEQ FrameC functions, fn_80070170..fn_800730F8, fn_80095D7C..fn_80096374: one file each by the pool rule).

**fn_8006EEF8** (0xBCC, the only blocker of the smallest run) was hand-written against the asm (scratch harness:
compile with the game flags against the drafts' ctx.h, compare words with tools/draft_batch.py's compare, dtk elf
disasm side by side): 32% -> **73%**, same length (755 words), not matched. The shape that gives the 73%: the vertex
writes are `static inline` helpers with f32 parameters (`grid_pos(x, y, z)`, `grid_color(r, g, b, a)` on the
write-gather pipe); at `-opt level=0` every inlined f32 argument that is not a plain variable or constant becomes a
local of its own, the locals are coloured by use count into f31 downwards (the first 14: f18-f31), the rest get a stack
slot each (0x8C, 0x88..0x08), which is the original's "12 three-float groups copied to the stack"; `Mtx m; Vec p, q;`
in that order; `u16 n = 42` with `for (i = -n / 2; i <= n / 2; i++)` and `(f32) n * 0.5f`. Left, in the order they
appear: (a) the original keeps **r30 unused** between i (r31) and alpha (r29): a tiny-function test shows the
mechanism - a local referenced only in code the compiler removes (`if (0) { ... }`, an `if (x == 0) {}` with an empty
body) keeps the register its use count earned, so 198 game functions with a register gap (none matched) have such a
variable; no spelling of it ranked between i and alpha here (a 16-statement dead block did not); (b) `clrlwi r29,r27,24`
on `alpha = up` after `up -= 0x1C` but a plain `mr` for `down`: not an `s32` / `u8` type difference, not `& 0xFF`, not a
`(u8)` cast (all compile to `mr`); (c) the scheduler's register numbering in the vertex blocks (value / address pairs
r5/r4 vs r0/r5 and the 0x4330 conversion temporaries), which moves with (a). Also tried without effect: the SDK's own
`GXWGFifo` absolute declaration against `*(volatile f32 *) 0xCC008000`, colour parameters as u8 / u32 / int / a macro,
one combined position+colour helper, declaration order (register assignment is by use count, not declaration order),
constants held in variables (propagated away). The notes and the best C are in the step 83 log; the draft stays m2c's.

**Data objects from C** (plan.md step 83, not built): eight typed .bss globals have exactly dtk's sizes (compile-time
check against ctx.h): cr_SaveData 0x1138, cr_ScriptState 0x484, cr_ScenarioState 0x364, cr_UnlockBits 0xA8,
cr_GameMode 0x48, cr_SeqState 0x48, cr_StageSelectorState 0x354, cr_CustomizeState 0x1B54; cr_g_Script, cr_g_SaveData
and cr_g_Scenario are contiguous (0x803BE358-0x803BFC78, 8-aligned), and the compiler lays a file's globals out in
reverse declaration order (.bss align 8), so one data file declaring them in reverse order compiles to that piece.
What is missing is the unit kind: gen_dtk_splits.game_c_objects takes only files with function definitions (.text
ranges), game_file_data only the data of a file with code; a data-only game unit needs its .bss / .data range from the
compiled object's defined symbols at our addresses (the next dtk symbol must start exactly at its end, or the padding
rule of step 65 applies), the first build without the object (the batch 9 fixpoint), shift_test's insertion points,
and batch 9's 100% check (trivial for .bss; .data tables need every pointer relocated by name).

## Eleventh generator: the script-command head, read to the end (plan.md step 84, Oct 31, 2026)

Round 10 (branch handlers), the head of the ~300 script-command handlers alone: 220 drafts sit at 78-85% because the
first 38 words of every handler come out in another order with other register numbers (step 80's `script_counter_ptr`,
step 82's flag / processor / pragma sweep). Method: a scratch harness (hd/pr.py in the step 84 log: one C file compiled
with the game flags against the original's asm, `dtk elf disasm`, three scores: words equal by position, by sequence
alignment, and by sequence alignment with the register numbers masked), the pure handler cr_CM_DUMMY_008 (38 words,
nothing but the head) as the target, then 60 micro-programs on the compiler to learn how it allocates and schedules.
**No C shape reproduces the head; the 85% ceiling stands.** What was learned, so the next round does not repeat it:
1. **Register allocation is done after scheduling.** The same source compiled without `schedule` allocates r3 to every
   base (`lis r3 / addi r3, r3` five times); with it the hoisted bases get r3, r5, r4, r8, r3. So the original's
   register numbers carry no information about its source order beyond the schedule itself, and the "pre-schedule
   order reconstruction" the brief asked for is not possible from the numbers: only the ORDER of the 38 words is.
   Locals do keep their rule: the callee-saved registers go in declaration order from r31 down (micro-tests: `int a; int
   b; int c;` is r31 / r30 / r29 whatever the use order; a block-scoped local comes after the enclosing ones); an
   assigned-but-never-read local goes to the stack (`stw r0, 8(r1)`), a never-assigned one gets nothing, a dead `p = pc
   + 1` goes to a temporary (r9): the saved-and-unused r30 of every handler can only be a pointer local whose loads /
   stores the peephole folded back onto a live base (step 80's reading holds; `&cmd_count[depth]` and
   `&cmd_history[idx]` both fold, both compiled, both below the ceiling).
2. **Where the unused register sits** (the original's saved registers against the draft's locals, 246 handlers): r30
   in 182 handlers, r29 in 23, r27 in 15, r28 / r26 / r25 in 7 each, r24, r22. cr_CM_WAIT is cam r31, pc r30, X r29;
   cr_F_CHK_BIT is pc r31, bit r30, X r29, word r28, mask r27; cr_CHAR_MOVE is chara r31, pc r30, i r29, found r28,
   cur r27, X r26, id r25. So the folded pointer is declared at a different place in each handler, after the
   handler's own locals in most of them and between them in others: it is not a macro's local at a fixed position (a
   block, an inline's parameter and a declaration inside a statement list were tested: the compiler rejects
   declarations after statements, a block's locals come last). Pointer declared after the PC local is the one thing
   that matters for the pure handlers, and the generator had it first (item 7).
3. **The scheduler's dependence model** (micro-programs; this is what the DAG looks like): two accesses to one symbol
   at different constant offsets are independent (`A.i += 1; A.i &= 0x3F; A.j = 1` stores A.j before the `+= 1`),
   a variable-indexed access (`A.arr[dep]`) conflicts with every access of its symbol, a load or store through a
   pointer local conflicts with every memory access (`(*p)++` after `G1 += 1` keeps the G1 store before its load, and
   before it when written before), different symbols are free (`stw PC` moves across the struct's stores), the
   prologue / epilogue stack stores and loads bound everything (no global load is issued before `stw r30, 8(r1)`),
   stores to one address stay in order, loads pass loads. Independent statements come out in the same order whatever
   their source order (`A.i = 1; B.arr[dep] = 2; C.i += 1` and its five permutations give one listing up to the final
   stores, which keep source order among themselves): the priority is structural, not the source order.
4. **No simple list scheduler reproduces it.** A simulator over the unscheduled compile (that DAG, 144 policies: top-down
   / bottom-up, critical-path priority, load latency 1-3, 1-4 instructions per cycle, 1-2 memory ops per cycle,
   source-order or reverse tie-break, stack barriers on / off) gets at most 23 of 38 words of the pure head in order
   and 55% over six micro-programs. The visible facts a policy must explain and none does: the `lis` of the mask
   statement is hoisted above the `lis` of the increment it depends on (positions 4 and 10 in the original; ours has
   the increment's first), the count statement's `lis` stays at 17 while its depth load moves up to 14 (ours: `lis`
   at 7, the load at 21), and `addi` of a hoisted `lis` waits behind two more `lis`. Under any critical-path
   priority the increment's chain is taller than the mask's (its store feeds the mask's load), so the original's order
   is not a critical-path schedule of the DAG of item 3; the compiler's priority or dependence model has a component
   these micro-programs did not isolate.
5. **Shapes tried, all compiled and scored on cr_CM_DUMMY_008 (18 of 38 words by position is the ceiling, 21 with the
   registers masked):** the 120 statement orders of the five statements with the counter pointer at each of its
   positions (360 files: best 18 / 19 / 21, `hist, inc, adv, mask, cnt` with the pointer first), the 120 orders with
   a history pointer instead (best 12), the 120 orders of the direct form without a pointer (best 9 / 20, 36 words:
   no r30), the pointer set in the same statement as its use, the post-increment inside the index
   (`cmd_history[history_index++]`: 35 words, no reload), a comma expression, the whole head as a comma expression,
   the PC advance as `+= n` / `= cr_g_ScriptPC + n` / `pc + n`, the command word in a local, `register`, `volatile`
   on the state and on the PC, `(&cr_g_Script)->` member access, a state pointer local (30 words), a pointer to the
   index, an inline function with the state / PC / depth / count as parameters (the inliner substitutes the address
   arguments: 16), `#pragma scheduling off` with the original's own order (the unscheduled allocation is r3 for every
   base: 1 word by position, so the "hand-ordered C with scheduling off" fallback cannot give these bytes either),
   compilers GC/2.0, 2.5, 2.6, 2.7 (the same 18), 3.0a3-3.0a5 (another schedule, 14 / 22; 3.0a5.2: 8). Nothing moved
   the 85% family. Left as the compiler's schedule; the next lever for it would be the compiler's own scheduler
   (its DAG and priority read from the binary, or from a build log), not more C spellings.
6. **The pool symbols of a C file object** (the fn_8002444C regression of the step 83 run, batch 12 479 instead of
   480): tools/gen_c_symbols.py writes the C object's own local symbols into decomp/config/GXCE01/symbols.txt
   (`@22 = .sdata2:0x804504E8; ... data:float`), dtk's next asm names the constants `"@22"@sda21`, m2c declared
   `extern f32 @22;` and the draft did not compile. draft_batch now names a C object's non-jump-table local symbol
   `lbl_<address>` in the functions' asm (game_functions, next to the step 69 jump-table rename), and pool_floats /
   pool_strings / data_decls take `@NN` objects of symbols.txt under that name (`pool_name`), so the draft is the one
   that matched before the object was built from C (float_literals): fn_8002444C 100% again, still built from its
   draft (promote_report.txt: not "kept"). gen_c_symbols.py is unchanged: objdiff needs the C object's own names.
7. **`script_counter_ptr` declares the pointer after the PC local**, not first: with `u32 *cmd_cnt;` above `s16
   *var_r31;` the two registers were swapped (item 1) and the 40 cr_*_DUMMY_* handlers and 6 more were at 44%; now 50%
   (the pure head's own ceiling by position). No handler reached 100%; the family's ceiling is unchanged.
**Before -> after** (full run; m2c, compiler, flags unchanged): compile 1,705 -> 1,706 of 1,751, **100% MATCH
479 -> 480**, bytes equal after alignment 519,792 -> 521,296; histogram 0-9% 44 10-19% 62 20-29% 73 30-39% 112 40-49% 133 50-59% 262 60-69% 154 70-79% 171 80-89% 140 90-99% 75 (about 120 handler drafts from 44% to 50%: item 7); 80-99% 210 ->
215. Linked: 443 game objects (438 promoted, 4 file objects, 1 kept: fn_800A0804), 646 of 922 units from C, 22.18% of
main.dol's code; batch 9 retail SHA-1, static shift PASS, batch 8 0 FAIL, checksums written.

### Step 85: file runs by hand - the register rules of level 0, two generator rules (Oct 31, 2026, branch fileruns)

Goal: close the cheapest file runs of the step 83 table by hand-matching their blockers. Method as in step 83: a
scratch harness (the step 83 `hm.py`: one C file compiled with the game flags against the drafts' ctx.h, words
compared with tools/draft_batch.py's compare, `dtk elf disasm` side by side; `cc.py` for micro-programs), first on
fn_8006EEF8 (the smallest run's only blocker), then on the three 90%+ blockers of fn_8008057C..fn_800812A4. No draft
was edited; the two fixes that generalise went into tools/draft_batch.py.

**What the compiler does with locals at `-opt level=0` (micro-programs, GC/2.6, game flags; this corrects step 84
item 1):**
1. **Callee-saved registers go by use count, from r31 down; the declaration order is only the tie-break.** `u32 alpha;
   s32 i; u32 beta;` with i used most gives i r31, alpha r30, beta r29; `s32 i; u32 alpha; u32 beta; s32 k;` with k used
   27 times gives k r31, i r30. Step 84's `int a; int b; int c;` test had equal counts, which is why it read as
   declaration order. A local that is read keeps its rank; a local that is only written (`dead++;`, `if (i) dead = 1;
   else dead = 2;`, `dead = dead;`, or any local referenced only in code the compiler removes, `if (0) { dead = 1;
   GXSetNumChans(dead); }`) is appended AFTER the live locals (it still gets a saved register when its writes survive
   to allocation, none when the compiler drops them first), so it can never make a gap between two live locals.
2. **A gap register (saved, never used) between two live locals is a pointer local to a stack object, assigned (not
   initialised) and used through members only**: `Vec *pp; ... pp = &p; pp->x = ...;` ranks by its use count like any
   local and takes its register, and then the peephole folds every `pp->m` onto `r1` (`stfs f0, 0xc(r1)`) and drops
   the `addi r30, r1, 8`, leaving the register saved and unused. With an initialiser (`Vec *pp = &p;`) the `addi` stays
   and the accesses go through the register; as a call argument (`PSMTXMultVec(m, pp, pp)`) it stays too (`mr r4,
   r30`). The stores through the pointer are still pointer stores for the scheduler (they conflict with every memory
   access, step 84 item 3), so where the pointer is used changes the schedule of that block.
3. **`clrlwi rD, rS, 24` on a copy** is the widening of a u8 local into a wider one: `u32 alpha; u8 up; ... alpha =
   up;` (the compiler keeps a u8 register local untruncated after `up -= 0x1C`, so the copy into a u32 truncates);
   `alpha = down` with `s32 down` is a plain `mr`. Two u8 locals copy with `mr` whatever the casts.
4. **Initialisers run in declaration order; an assignment statement runs where it stands**: `li r27, 0xfc; li r26,
   0; li r28, 0x2a` with n's register above up's and down's is `u16 n; u8 up = 0xFC; s32 down = 0; ... n = 42;`.
5. **A pool constant the compiler owns moves freely, an extern does not**: `lfs f0, 0.0f` is scheduled above a `stb`
   through a pointer parameter when it is the file's own literal; written as `extern f32 lbl_80450E5C` it waits behind
   the store (an extern may alias with the pointer). The same function's `x + 3.875f < 255.0f` matches only with the
   externs: as literals the compiler swaps the `fadds` operands (fn_80080A94, generator rule `store_literals` below).

**fn_8006EEF8** (0xBCC, the only blocker of fn_8006EEF8..fn_8006FDD0): step 83's 73% (557 of 755 words) had the frame
wrong; with the rules above the frame is now exact - r31 i, **r30 the gap** (`Vec *pp;` declared between i and alpha,
`pp = &p;` before the second p group of each branch and that group's three stores through it: 8 uses, between alpha's
11 and i's 18), r29 `u32 alpha`, r28 `u16 n` (assigned `n = 42;` after the declarations), r27 `u8 up = 0xFC`, r26
`s32 down = 0`, `clrlwi r29, r27, 24` on `alpha = up`, the three initial `li` in the original's order, 755 words. Score
548 of 755 by position (72.6%; 673 of 755 with the register numbers masked). What is left is the scheduler's priority
inside the vertex blocks that follow a `GXBegin`: the original hoists the pool loads of the next conversion (`lfd`
0x4330..., `lfs` 0.5f) above the write-gather stores and issues each store's `lis 0xcc01` late, ours hoists the `lis`
and issues the loads late; the else branch's first group schedules the u16 conversion first in the original and in
our odd branch, the s32 one first in our else branch (the same source: the pointer stores of rule 2 perturb whichever
group carries them, and every placement of the pointer over the four p groups, with 1-3 members through it, was
tried: 503-548). Also without effect: volatile or not on the pipe union, `*(volatile f32 *) 0xCC008000`, the SDK's
`GXWGFifo`, a far extern symbol for the pipe (much worse: 643 words), colour parameters u8 / u32 / macro, one
position+colour helper, the six orders of the three-float groups (per group and globally). The run stays open; the
best C (`v13.c`) and the harness are in the step 85 log, the draft stays m2c's.

**fn_8008057C..fn_800812A4's near-misses**, hand-matched and then put into the generator (3+ drafts carry each
pattern):
- **fn_80080B80 92 -> 100%, `fold_temp_compound` (default)**: m2c's `temp_r3_44 = p->a->b; M2C_FIELD(temp_r3_44, f32 *,
  0x14) = (f32) ((f64) M2C_FIELD(temp_r3_44, f32 *, 0x14) + lbl_80450E70);` makes the temporary a local of the draft
  (a callee-saved register, `lwz r30, 4(r3)`, saved and restored); the original evaluated the address once inside a
  compound assignment and kept it in r3 (`lwz r3, 4(r3); lfs f1, 0x14(r3); ...; stfs f0, 0x14(r3)`). The rule folds
  `temp_rN = ADDR; FIELD(temp) op= RHS` and the `(f32) ((f64) FIELD(temp) + C)` form into `FIELD(ADDR) op= RHS;` when
  the register is r3-r12 (a temporary: a local the original kept across a call would be callee-saved), the temporary
  has no other use, and C is a double (a literal without f suffix or an .sdata2 double), then drops the declaration.
  The `|=` / `&=` form occurs in 67 drafts, the f64 form in 3.
- **fn_80080A94 98 -> 100%, `store_literals` (a variant)**: rule 5 above; the 0.0f written as the literal only where a
  constant is a whole stored value (`= (f32) lbl_80450E5C;` -> `= 0.0f;`), the constants in arithmetic and compares
  left as externs (`float_literals`, which converts all of them, gives 91%). Like `float_literals` the draft then owns
  that pool constant (the file run's data).
- **fn_80080FD8 92 -> 93%** (the fold applied to its `|=` and `+= 0.1` temporaries): the original saves r27-r31 and
  uses only r31 (arg0), r30 (`var_r30`) and r28 (`var_r28`): two more rule-2 pointer locals, in r29 and r27, whose
  placement is unknown (not tried: the run has four more blockers at 54-78%).

**Before -> after** (full batch 12 runs): compile 1,706 -> 1,706 of 1,751, **100% MATCH 480 -> 483**
(cr_Vec3_Normalize 98% -> 100%, fn_80080A94 98% -> 100%, fn_80080B80 92% -> 100%), bytes equal 521,296 -> 521,432; 80-99% 215 -> 213; 170 drafts up (cr_Vec3_Normalize 98 -> 100% through store_literals too, fn_8007D638 77 -> 92%, about 120 handler drafts by one point through the fold), 11 down by 1-4 points (fn_8006B144 31 -> 27%, fn_800FEFF8 93 -> 90%, fn_800FBDF8 86 -> 83%, cr_CM_WAIT 52 -> 49%: a folded temporary where the original did keep a local or evaluated the address twice), 0 match lost.
Linked: **444 game objects (439 promoted, 4 file objects, 1 kept)**, 647 of 924 units from C, **22.20% of main.dol's code**,
5.90% of its data. Checks: batch 9 retail SHA-1, every C object 100%, relocations OK, static shift PASS; batch 8 0 FAIL;
checksums written (the 3 decompiled\ lines kept at the committed values). No run closed, so batch 11 was not run.

The run table after step 85 (promote_report.txt "file runs"). fn_80080A94 now owns its 0.0f, so its run is recomputed from its
pool: fn_80080A94..fn_800812A4, 9 functions, 5 at 100%, 4 blockers (was fn_8008057C..fn_800812A4, 11 functions, 7 blockers);
fn_80080A94 and cr_Vec3_Normalize match but wait for their runs ("has data of its own"), fn_80080B80 is promoted:

| run (first..last) | span | functions | at 100% | blockers | blocker bytes | blockers (size, match) |
|---|---|---|---|---|---|---|
| fn_8002444C..fn_8002444C | 0x8002444C-0x80024578 | 1 | 1 | 0 | 0x0 | FILE OBJECT game/fn_8002444C_fn_8002444C.c (.sdata2 0x804504E8-0x80450518) |
| fn_8005C4D0..fn_8005C4D0 | 0x8005C4D0-0x8005C628 | 1 | 1 | 0 | 0x0 | FILE OBJECT game/fn_8005C4D0_fn_8005C4D0.c (.sdata2 0x804509A0-0x804509A8) |
| fn_800AA130..fn_800AA130 | 0x800AA130-0x800AA17C | 1 | 1 | 0 | 0x0 | FILE OBJECT game/fn_800AA130_fn_800AA130.c (.data 0x8022B130-0x8022B204) |
| fn_80111EEC..fn_80111EEC | 0x80111EEC-0x8011214C | 1 | 1 | 0 | 0x0 | FILE OBJECT game/fn_80111EEC_fn_80111EEC.c (.sdata2 0x80451888-0x80451890) |
| fn_8006EEF8..fn_8006FDD0 | 0x8006EEF8-0x8006FE38 | 7 | 6 | 1 | 0xBCC | fn_8006EEF8 (0xBCC, 32%) |
| fn_80080A94..fn_800812A4 | 0x80080A94-0x80081430 | 9 | 5 | 4 | 0x640 | fn_80080C7C (0x1A8, 63%), fn_80080E24 (0xFC, 78%), fn_80080FD8 (0x210, 93%), fn_800812A4 (0x18C, 54%) |
| fn_80080C7C..fn_800812A4 | 0x80080C7C-0x80081430 | 7 | 3 | 4 | 0x640 | fn_80080C7C (0x1A8, 63%), fn_80080E24 (0xFC, 78%), fn_80080FD8 (0x210, 93%), fn_800812A4 (0x18C, 54%) |
| fn_8008A0A8..fn_8008BD50 | 0x8008A0A8-0x8008BED4 | 11 | 6 | 5 | 0x14AC | fn_8008A484 (0x22C, 56%), fn_8008A6B0 (0x200, 63%), fn_8008A8B0 (0x238, 57%), fn_8008AAE8 (0x204, 55%), fn_8008ACEC (0xC44, compile failed illegal type) |
| fn_800057B8..fn_8000805C | 0x800057B8-0x8000860C | 16 | 11 | 5 | 0x171C | fn_800057B8 (0x3D0, 43%), fn_80005BC8 (0x170, 56%), cr_BattleRobo_InitFromLoadout (0x8B4, 36%), fn_80007CE4 (0x378, 65%), fn_8000805C (0x5B0, 67%) |
| fn_80076B2C..fn_80079C70 | 0x80076B2C-0x80079CB8 | 31 | 19 | 12 | 0x24F8 | fn_80076B2C (0x764, 44%), fn_800779A8 (0x2B4, 57%), fn_80077C5C (0x2DC, 56%), fn_80078000 (0x438, 12%), fn_800784B0 (0x6B0, 47%), fn_80078B60 (0x140, 86%) ... +6 more |
| fn_8007D0D4..fn_8007FCD8 | 0x8007D0D4-0x8008052C | 18 | 6 | 12 | 0x2FF4 | fn_8007D3C8 (0x270, 82%), fn_8007D638 (0x1C8, 92%), fn_8007D800 (0x220, 89%), fn_8007DA20 (0x60, 79%), fn_8007DA80 (0x170, 66%), fn_8007DD60 (0x6A0, 38%) ... +6 more |
| fn_8007D3C8..fn_8007FCD8 | 0x8007D3C8-0x8008052C | 14 | 2 | 12 | 0x2FF4 | fn_8007D3C8 (0x270, 82%), fn_8007D638 (0x1C8, 92%), fn_8007D800 (0x220, 89%), fn_8007DA20 (0x60, 79%), fn_8007DA80 (0x170, 66%), fn_8007DD60 (0x6A0, 38%) ... +6 more |
| fn_8007C094..fn_8007FCD8 | 0x8007C094-0x8008052C | 22 | 9 | 13 | 0x3BF0 | fn_8007C094 (0xBFC, compile failed illegal operand), fn_8007D3C8 (0x270, 82%), fn_8007D638 (0x1C8, 92%), fn_8007D800 (0x220, 89%), fn_8007DA20 (0x60, 79%), fn_8007DA80 (0x170, 66%) ... +7 more |
| fn_8010E47C..fn_80112A0C | 0x8010E47C-0x80112AB8 | 27 | 13 | 14 | 0x3E70 | fn_8010E47C (0x3A4, 75%), fn_8010E820 (0x1BC, 53%), fn_8010E9DC (0x180, 48%), fn_8010EB5C (0x520, 55%), fn_8010F07C (0x800, 12%), fn_8010F87C (0x4BC, 18%) ... +8 more |
| cr_Bgm_GetVolume..fn_80098484 | 0x80096594-0x80098F4C | 25 | 8 | 17 | 0x272C | fn_800965F4 (0x160, 44%), fn_80096754 (0x160, 70%), fn_800968B4 (0x374, 57%), fn_80096C28 (0x100, 68%), fn_80096D28 (0x100, 75%), fn_80096E28 (0x94, 65%) ... +11 more |
| fn_800527D4..fn_8005BE70 | 0x800527D4-0x8005BEC8 | 18 | 1 | 17 | 0x9610 | fn_800528B8 (0xE4, 68%), fn_8005299C (0xA94, 17%), fn_80053430 (0x5F0, 27%), fn_80053A20 (0x388, 96%), fn_80053DA8 (0x490, 26%), fn_80054238 (0x1ECC, 16%) ... +11 more |
| fn_80081440..fn_80089140 | 0x80081440-0x80089550 | 27 | 3 | 24 | 0x77BC | fn_80081440 (0x4C8, 45%), fn_80081908 (0x24C, 7%), fn_80081B54 (0x248, 48%), fn_80081D9C (0x5C8, 34%), fn_80082364 (0x69C, 25%), fn_80082A00 (0x584, 21%) ... +18 more |
| fn_80067904..fn_8006B3D0 | 0x80067904-0x8006B46C | 42 | 10 | 32 | 0x3920 | fn_80067904 (0xA54, 10%), fn_80068420 (0x26C, 58%), fn_8006868C (0x2EC, 68%), fn_80068988 (0xEC, 77%), cr_Text_CountChars (0xDC, 60%), fn_80068B84 (0xF8, 51%) ... +26 more |
| fn_8001B1D4..fn_8001F060 | 0x8001B1D4-0x8001F350 | 72 | 32 | 40 | 0x3680 | fn_8001B29C (0xEC, 66%), cr_Rand_Float (0x68, 44%), cr_Vec3_Distance (0x1A8, 71%), fn_8001B6EC (0x668, 52%), cr_Vec3_RotateY (0x1B0, 87%), cr_Vec3_RotateAboutAxis (0x6A8, 70%) ... +34 more |
| fn_80024148..fn_80036FB0 | 0x80024148-0x800373AC | 76 | 15 | 61 | 0x127EC | fn_8002419C (0x2B0, 18%), fn_80024634 (0x19C, 34%), cr_CamMode_DefaultStart (0x60, 70%), fn_80024848 (0x17C, 74%), cr_CAMMODE_SNR_AUTO_Y_ANG_Frame (0xC4, 63%), fn_80024A88 (0x13C, 69%) ... +55 more |
| fn_8008A484..fn_80093AF0 | 0x8008A484-0x80093C30 | 87 | 25 | 62 | 0x8CA4 | fn_8008A484 (0x22C, 56%), fn_8008A6B0 (0x200, 63%), fn_8008A8B0 (0x238, 57%), fn_8008AAE8 (0x204, 55%), fn_8008ACEC (0xC44, compile failed illegal type), fn_8008BED4 (0x180, 96%) ... +56 more |
| fn_801147B4..fn_80124F5C | 0x801147B4-0x80125884 | 79 | 16 | 63 | 0x10850 | fn_801147B4 (0x294, 70%), fn_80114A48 (0x470, 44%), fn_80114EB8 (0x284, 62%), fn_8011513C (0x470, 44%), fn_801155AC (0x36C, 85%), fn_80115918 (0x1F8, 49%) ... +57 more |
| fn_8000768C..fn_801314D4 | 0x8000768C-0x80131910 | 1739 | 476 | 1263 | 0x11AC70 | fn_80007CE4 (0x378, 65%), fn_8000805C (0x5B0, 67%), fn_8000860C (0x180, 89%), cr_Game_Update (0x474, 75%), fn_80008CB0 (0x110, 37%), fn_80009018 (0x554, 79%) ... +1257 more |

**Left**: fn_8006EEF8's vertex-block schedule (the compiler's priority, as the handler head of step 84: not a spelling),
fn_80080FD8's two gap pointers, the other blockers of the table; rule 2 is the mechanism behind the register gaps of
the ~198 unmatched game functions that save a register they never use (step 83): each needs a pointer local to a
stack object with the right use count, which a generator cannot guess but a hand match can.

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
