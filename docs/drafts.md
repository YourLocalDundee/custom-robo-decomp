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
3. **Post-process** (all of it in `draft_batch.py`, so a rerun reproduces it; no draft is edited by hand: the drafts written by hand are committed files in tools/hand_drafts, step 108): a stack array m2c
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

## Step 90: naming round 5 types, draft round 10 (plan.md step 90, Nov 1, 2026, branch data2)

Baseline: the step 86 generator and names run again in this worktree give the committed counts exactly (483 at 100%,
1,706 compile, 43 compile failures, 521,432 bytes equal). The types of naming round 5 (docs/decomp.md "Game data
objects", step 90) then went through the full run, and the drafts they touch were compared function by function.
1. **First full run with the new types**: 100% MATCH 483 -> 482, compile failures 43 -> 61, bytes equal 521,432 ->
   513,780; 19 drafts up (cr_BattleRobo_InitBody 69 -> 86, fn_80079214 43 -> 53, fn_800798CC 51 -> 60, fn_8006A00C /
   fn_8006A1A8 / fn_8006A344 16 -> 22: cr_g_WindowStyle's float fields), 33 down, 18 of them to compile failures.
   The failures had three causes, each measured with `--only` on the 23 drafts (out of tree, `--out
   decomp/build/drafts_try`):
   - **A struct whose first member is an array.** m2c reads the word at +0 of `cr_g_BattleRobos[i]` as the struct
     itself: `if ((u32) cr_g_BattleRobos[i] != 0U)` ("illegal explicit conversion from 'struct cr_BattleRobo'"),
     `cr_g_BattleRobos[n] = fn_80005B88;` (fn_80089550 / fn_80089BB0 store a function there). The word is the robo's
     update function, as cr_Chara.update is (non-zero = in use): typed `void (*update)(void)`, 13 of the 18 compile
     again and fn_8000768C is 100% again (fn_80092A5C 86, fn_8003633C 53 -> 56, fn_800294D8 48 -> 53,
     cr_CAMMODE_DICE_WAIT_Frame 26 -> 29).
   - **A struct-typed global that is a file's data-pool base.** The file system reaches cr_g_ArchiveDvdEntry and
     cr_g_FileSlots from the base of the object before them (+0xF34, +0x1C): m2c writes `*(&cr_g_EntryTableLoad +
     0xF34 + i * 4)` (pointer arithmetic scaled by the struct), and with a rule that makes the sum bytes
     (`(u8 *) &cr_g_EntryTableLoad + ...`, tried) it then writes `((u8 *) &cr_g_EntryTableLoad + 0xF34)->messages[i]`
     ("not a struct/union/class"). cr_g_EntryTableLoad is declared `u8[28]` (as cr_g_Cams is): cr_FileSys_Init 37,
     fn_80021674 16, cr_Archive_OpenRequestList 17 as before; the struct cr_EntryTableLoad documents the fields. The
     rule was not kept (nothing else triggers it). Same for cr_g_ModelSlots (`u8[48128]`): fn_80078F94 clears a slot
     with a byte loop through a pointer m2c types `cr_ModelSlot *` (`*var = 0`); 91% again.
   - **Byte offsets through a typed robo pointer** (`*((cr_BattleRobo *) ((u8 *) p + off)) |= 1`, `&(u8 *)
     p->body_data - ...`): fn_80063DA4 (80), fn_8003B37C (61), fn_8003AFF0 (52), fn_8004F6D4 (45), fn_80041358 (13) do
     not compile. Left for the next round: m2c needs the member at a variable byte offset of the robo; the struct is
     mostly `unk_` arrays, so a rule would have to choose the access width from the instruction, which the draft no
     longer shows.
2. **Second full run** (the types above): the rule below on, the full run gives **100% MATCH 483 -> 483**, compile
   1,706 -> 1,703 (46 failures: the five robo drafts above and cr_CAMMODE_CUSTOMIZE_Frame, 1% before; three drafts
   compile for the first time, fn_8006B46C 47, fn_8006B848 14, fn_8006C970 16), bytes equal 521,432 -> 521,292;
   histogram 0-9% 43, 10-19% 61, 20-29% 75, 30-39% 112, 40-49% 130, 50-59% 263, 60-69% 153, 70-79% 170, 80-89% 140,
   90-99% 73. Up against the baseline: cr_BattleRobo_InitBody 69 -> 86, fn_80079214 43 -> 53, fn_800798CC 51 -> 60,
   fn_8006A00C / fn_8006A1A8 / fn_8006A344 16 -> 22, fn_800294D8 48 -> 53, fn_8003633C 53 -> 56 and 15 more by 1-3
   points; down: the five compile failures, fn_80024BC4 31 -> 22, fn_800E36BC 22 -> 18 and eight more by 1-3 points.
   Promoted 439 (+4 file objects, 1 kept), 470 game units, 673 of 970 units from C. The drafts the new types touch at
   90-99% (cr_CAMMODE_MIDDLE_Frame 90, fn_80075258..fn_800755E8 96, fn_80079544 95, fn_80078F94 91 ...) are the same
   as before the types (only the names changed in them), so no rule was written for them.
3. **`struct_byte_offset`** (default): `M2C_FIELD((&cr_g_BattleRobos[arg0] + (var_r30 * 4)), s32 *, 0x864) = 0;` -
   m2c adds a byte offset to the address of a struct-typed global or of one of its elements, and C scales the sum by
   the struct's size (here the store would land 0x33520 * 4 * i bytes away). When the multiplier is no multiple of the
   struct's size (ASSERT_SIZE of symbols/cr_types.h, `struct_sizes()`) the sum is bytes: `(u8 *) &cr_g_BattleRobos[arg0]
   + (var_r30 * 4)`. fn_8010D2D0 65 -> 100% (it was promoted from the first run, when the robo's first member was a u8
   array that m2c indexed instead); on the 17 drafts with the pattern (`--only`, stock against patched) 9 up
   (fn_8003B1BC 60 -> 69, fn_8012BD68 43 -> 47, fn_800E6E34 3 -> 7 ...), none down; member_array_index's
   `mulli 0x354` case (a multiple) is left alone.

### Step 91: the near misses labelled, the handler head's missing instruction (Oct 31, 2026, branch sched-d)

**Labels** (tools/sched/label_drafts.py -> decomp/reports/sched_labels.txt): the 213 drafts at 80-99% of the last
batch 12, compiled again and compared with the original block by block (blocks end at branches, labels and calls, as
the compiler's PCode does; registers masked):

| Label | Drafts | of them handlers (cr_*_ script commands) |
|---|---|---|
| structural (other instructions in at least one block) | 156 (96 + 60 with the frame too) | 22 |
| schedule-only (every block has the original's instructions, in another order) | 36 (28 + 8 with the frame too) | 17 |
| register-only (the body is the original once register numbers are masked) | 19 (17 + 2 with the frame too) | 0 |
| frame (only the prologue / epilogue differ) | 2 | 0 |

A schedule-only draft needs only the right first-pass input order (a statement order or expression shape); a
register-only one is the colouring (locals' use counts, step 85 rule 1); structural ones need the generator.
The handlers at 80-99% are 39 of the ~220 script-command drafts (the others are below 80%).

**cr_CM_DUMMY_008** (docs/sched_model.md section 14): base.c's first-pass PCode, with one extra integer instruction
that the passes after the first scheduling delete, reproduces the original's 38 words exactly in the replay: a
copy into a register just before its definition (any of 1,092 placements found) or an in-place operation on the PC
local right after its load (156 placements); without it the ceiling is 25 of 38. The hook shows what the compiler
deletes between the passes for base.c: only the folded counter pointer's `addi`; for fn_8006EEF8 also redundant
`clrlwi` of the u8 alpha before its `stb` and duplicate `lis`. No C spelling tried produces the extra instruction:
A compiled sweep of 256 shapes (tools/sched/corpus/shp, the eight statement orders of the synthetic solutions x no
extra / a second `pc = cr_g_ScriptPC;` / `pc = 0;` / `pc = (s16 *) cmd_cnt;` / a second counter-pointer assignment /
`cmd_cnt = 0;` at three places x both declaration orders, scored with hd/pr.py) gives at best 18 of 38 by position
(the step 84 ceiling); every extra statement stays in the code (39-40 words): the level-0 compiler does not delete
dead local writes. The head stays at its 85% family ceiling; no generator rule, no batch run.

### Step 92: float temporaries first; the head's second deletable instruction (Oct 31 - Nov 1, 2026, branch sched-e)

- **`hoist_float_temps`** (a variant, tools/draft_batch.py): in a draft whose body is a flat list of one-line
  statements, each float temporary `var_fNN = EXPR;` moves up past the stores m2c interleaved with it (never past a
  store to a field it reads, the statement that assigns a local it reads, or another temporary). Found with
  tools/sched/permute_stmts.py: fn_8001CCD4, fn_8001CD64, fn_8001CDF4 (36 of 36 words) and fn_8001D254 (63 of 63)
  match with the temporaries first; `draft_batch.py --only` with the rule: the four at 100% (fn_80076404 and
  cr_Vec3_RotateY unchanged). A full batch 12 with the rule was not completed in this step (docs/sched_model.md
  section 16).
- **Grouping of the near misses** (tools/sched/group_near.py): schedule-only 36 = 20 in the script-command head block
  only + 7 float blocks + 9 other; register-only 19 = no block differs once registers are masked.
- **The handler head**: the history store written `(s16) *pc` gives the compiler an `extsh` it deletes later (the
  first real construct of the deleted-instruction class); best compiled 20 of 38 by position (was 18), inverse
  ceiling 29 of 38 (was 25); one more deletable instruction on the increment statement's address is still missing
  (docs/sched_model.md section 15). Applied to the 21 handler drafts whose head the pattern finds, the `(s16)` store is
  worse (16 lower by position, 4 equal, 1 higher), so it is not a rule.

### Step 93: narrow parameters declared int; the head's address is canonical (Nov 1, 2026, branch sched-f)

- **`widen_narrow_params`** (a variant, tools/draft_batch.py): the u8 / s8 / u16 / s16 parameters of a draft declared
  s32. m2c types a parameter by its uses (a `sth` of it makes it u16). When the original passed an int, the front end
  emits a `clrlwi` before the narrow store, the first scheduling places it, and the passes between the schedulings
  delete it. The deleted instruction takes an issue slot in the first pass and moves the instructions after it.
  fn_8001F6BC: with `u16 arg1` the call block issues `mulli` and `lis` together and the `PADControlMotor` arguments
  move above the store (34 of 39); with `s32 arg1` the `clrlwi` takes the second slot of that cycle and the block
  is the original's (39 of 39). `(u16) arg1` at the store or `s16 arg1` give the same. Full batch 12: 487 -> 488 at
  100% (fn_8001F6BC promoted), 46 compile failures unchanged; the variant also raised fn_80071974, fn_80090298,
  fn_80096E28 and fn_800F6A44 (draft_report.txt).
- **The handler head** (docs/sched_model.md section 17): every spelling of the increment's address gives the same
  first-pass PCode (22 spellings: `->`, `(*&x).`, `[0]`, casts through u8 * / u32 / s32, negative indexes, a
  volatile access; cr_g_Script declared as an array, defined in the unit, static, initialised). A literal address
  `(cr_ScriptState *) 0x803BE358` drops the `addi` (the low half goes into the displacement). Local pointer or
  value variables for the increment / mask add copies that stay in the code (39-43 words). Best stays B1 of step 92.
- **group_near.py** skips a labelled draft that no longer compiles instead of stopping (the four step 92 drafts and
  fn_8001F6BC are promoted now). Run it after a full batch 12 in the same checkout: a worktree's decomp/drafts copied
  from another checkout keeps that checkout's ctx_protos.h, and a batch 12 run before batch 9 rebuilds the worktree's
  asm writes a ctx_protos.h with stale types (85 compile failures, 470 at 100% in this step's first run).

### Step 95: a masked index into a field as byte arithmetic (Nov 1, 2026, branch sched-h)

- **`masked_index_field`** (a variant, tools/draft_batch.py): m2c reads a `clrlslwi` on a u8 / u16 index as
  `M2C_FIELD(&B[(X * N) & MASK], T, OFF)`; the variant writes `*(T) (B + OFF + X * N)`. The words are the same; the
  order differs. fn_800B4710 (schedule-only, the HSD_JObjDispAll arguments): `li r6, 0` issues before the jobj's
  load in the original and after it in m2c's form, 50 of 56 -> 56 of 56. Of the 15 drafts with the pattern, two go to
  100%: fn_800B4710 and fn_800A5C60. Full batch 12: 488 -> 490 at 100%, 46 compile failures, 521,988 bytes equal.

### Step 96: three order rules for the schedule-only drafts (Nov 1, 2026, branch sched-i)

- **`byte_compare_casts`** (a variant): a byte member compared through m2c's `(s32) X` written `(s32) (u8) X`. The
  front end's extra `clrlwi` is deleted between the schedulings after taking an issue slot (fn_800B47F0: the element
  offset's `mulli` after the `lbz`, 92 -> 99 of 99; `(s8) X` gives the same).
- **`shift_mask_order`** and **`sink_const_global_stores`** (variants, also together): `(E << N) & M` written
  `(E & (M >> N)) << N`; a constant store to a global moved below the simple statements after it, up to a control
  statement, another constant store or a statement naming the global (m2c places it where its `stw` was
  scheduled). fn_80037894 needs both (93 -> 104 of 104); the sink alone brings fn_80021AB0 and fn_800932D0 to 100%.
- **`masked_index_field`** also rewrites the bare form `+ ((X * N) & MASK)` as `+ (X * N)` (fn_800B497C 73 of 73).
- Full batch 12: 491 -> 496 at 100% (fn_80021AB0 as a file object with its .data, fn_80037894, fn_800932D0,
  fn_800B47F0, fn_800B497C; fn_800A6068 is at 100% too but its pool entry is shared with 310 functions), 46 compile
  failures, 523,032 bytes equal.
- An `--only` run rewrites decomp/drafts for the named functions only and removes the other drafts' sources; run a
  full batch 12 before reading other drafts.
- Left (tried, no gain): fn_80075950 (134 of 155; the slot-table store of loops 2-5 completes before the next
  statement's `lis` in the original: the table as a 1-D / 2-D typed array, with the offset in the index, a `for`
  loop, the buffer address as a multiply: 119-134); fn_800EB518 is a register-count difference (the original keeps
  the switch byte in r24 and saves r22-r31, ours r20-r31), not an order one.

### Step 110: a struct array element read as a scalar (Nov 2, 2026, on main)

After cr_g_BattleRoboStates was typed (cr_BattleRoboState[4], main session above), four drafts stopped compiling:
fn_8005098C, fn_8007A800, fn_80086CA0, fn_80089BB0. m2c writes `(u32) cr_g_BattleRoboStates[i]` for a word load at
offset 0 of an element, and the compiler refuses a struct-to-scalar cast. The repair of `illegal explicit conversion`
in tools/draft_batch.py handled `*(base + off)` and a plain identifier. It now also takes an array element
(`name[i]`, `a.b[i][j]`) and reads it as `*(T *) &name[i]`. `draft_batch.py --only` on the four: compile failures
4 -> 0. The full batch 12 is the check (plan.md step 110).

### Step 110: the script-command head (Nov 2, 2026, on main)

Rule `script_head_dead` (on by default; the step 80 `script_counter_ptr` form stays as the variant that skips it). It
writes the handler head as the original has it: the command word stored as `(s16)`, the history index stepped and
masked, the counter incremented in place, the PC advance (`+= 2`, `+= 4`, ...) last, and the counter pointer local
used only in `if (0) { (*cmd_cnt)++; }`. Why it matches is in docs/sched_model.md section 20. Full batch 12:
**609 -> 777 drafts at 100%** (+168; 68 script-command handlers among the 100%s by name pattern), 707 promoted (was 546),
0 compile failures.

Variant `cmd_cnt_last` (optional rule, one VARIANTS entry): the dead-code counter declared after the other locals.
Its dead uses rank it among the live locals for the callee-saved registers, and declared last it loses the ties.
cr_CM_RESRV goes from 98% to 100% with it (the camera local takes r30, as in the original). The other 90-99% handlers
differ in their bodies: a mask done in 8 bits (cr_S_BGM_CHANGE), a sign extension of a local (cr_CM_SHAKE). Those
are hand-draft work.

Note: `draft_batch.py --only` clears decomp/drafts/c, so afterwards only the listed drafts are there. The next full
batch 12 writes them all again.

### Step 110: the 29 script handlers at 90-99% (Nov 2, 2026, on main)

After the head rule, 29 handlers were at 90-99%. 28 now have hand drafts (tools/hand_drafts, step 110 in their first
line). cr_CM_ZOOM is fixed by a context rule. The fixes, each one a candidate generator rule:

| Fix | Handlers | What the original has |
|---|---|---|
| dead uses in the head's `if (0)` block (`v = v;`, 1-3 times) and the counter's declaration position | cr_CHAR_SET_MODEL, _SET_T_SCRIPT, _SET_A_SCRIPT, cr_CM_WAIT, cr_CHAR_COUSE_CLEAR_PS, _LOOK_UNSET, _LOOK_SET, _MOT_SET, _FOLLOW_UNSET, _MOVE_COURSE(_PS), cr_SYS_OVERLAY | dead uses count for the callee-saved order: they raise a local's rank without code (scripts tools/sched/scratch/hd/dead_pos110.py, dead_uses110.py, dead_pairs110.py search them) |
| three locals used only in dead code | cr_SYS_SEL_JP | saves r28-r31, uses only r30 |
| the character search as a plain loop with the ID read before it, the result copied into a second local | cr_CHAR_MOT_RET, _TURN_RET, _TURN | the ID in a saved register, `mr. r31, r27` |
| tied locals declared in the other order | cr_CHAR_FOLLOW_SET, cr_CHAR_TURN | declaration order breaks use-count ties |
| a byte offset added to `(u8 *) &cr_g_Scenario`, not to the struct pointer | cr_SYS_PICKUP_BATTLE_END, cr_F_PARTS_GET | m2c's `&cr_g_Scenario + (i << 5) + 0x298` is scaled by the struct size |
| the constant before the index: `(u8 *) p + 0x9FC + i * 0xC` | cr_CHAR_COUSE_SET_PS | stfsx with the index + constant |
| `&= ~1`; no (u8) cast in a test; an s32 local instead of s16 | cr_S_BGM_CHANGE, cr_CHAR_TALK, cr_CHAR_COUSE_SET_PS, cr_CM_SHAKE, cr_F_CHK_BIT | word masks and compares, no extsh |
| `>=` for m2c's `==` | cr_E_CIRCLE_SET | fcmpo + cror (ordered) |
| the callee called with its real parameter types | cr_S_BGM_FIN | fn_80097670's third parameter is u8 (m2c inferred s8) |
| the depth incremented in place after the call count | cr_SYS_CALL | no saved copy of the depth |
| **.sdata2 / .rodata declared const** (data_decls, for every draft) | cr_CM_ZOOM 82 -> 91 of 91 | the compiler moves a load of const data ahead of pointer stores |

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

### Step 108: hand drafts and six generator rules (Nov 1-2, 2026, branch record-index)

The owner's decision of Nov 1 (docs/roadblock_solutions.md "What to decide" item 2: `asm` functions do not count as
from C) moved the work to hand-writing. Method: the 90-99% drafts and the small 80-89% ones, each diffed against the
original with tools/sched/scratch/hd/pr.py (left column the original), fixed by hand, and where one fix is a pattern,
written as a rule. All rules that the original's text cannot decide are VARIANTS (tried per draft, kept when better).

- **Hand drafts** (`tools/hand_drafts/<name>.c`, committed; draft_batch.py's HAND DRAFTS hook uses one instead of m2c's
  text, so batch 12 compares it and promote_drafts.py links it like any draft; each file's first line says what was
  changed): fn_8005CDDC (`x = x | 2`: the global read twice), fn_8005CD90 (an s16 field), fn_80108230 (a u8 byte),
  fn_8008BED4 (`break`, one shared blr), fn_80019828 (the countdown as s32), fn_800DF2A0 / fn_80089F40 /
  fn_800A5894 (pointer tests against `((void *) 0)`), fn_8005D068 (`(x & 0xFF0000) >> 16`), fn_8003F1E8 /
  fn_800412F8 (the parameter added first), cr_SEQ_MEMORY_CARD_Enter (an int counter), fn_800EB0E0 (both parameters),
  fn_8008E8F4 (the matrix local), and two **empty debug macros**: `if (state == 3) { (void) 0; }`
  (cr_SEQ_MODE_SELECTOR_FrameA) and `if (arg0 == ((void *) 0)) { (void) 0; }` (fn_8008C054): the compiler keeps the
  compare and the peephole removes the branch to the next instruction, which m2c cannot see. 16 files.
- **`record_array_index`** (variant): an access at `B + ((I * S) + O)` written `*(T *) &((u8 (*)[S]) (B))[I][O]`: the
  compiler adds O to the scaled index and loads indexed (`addi; lwzx` / `stbx`) as the original, where the byte sum
  folds O into the displacement. O may be past S (`p->arr[i]`). fn_8008C3B4, fn_80064150, fn_8000B144, fn_8003F000,
  fn_8008C5F0, fn_800AF2A4, fn_8011F270 to 100%.
- **`for_break_loops`** (variant): m2c's `loop_N: if (C) { if (T) { ...; i += 1; goto loop_N; } }` written
  `for (; C; i += 1) { if (!T) break; ... }` (the compiler's `for` jumps to its test at the bottom): fn_80079544,
  fn_800796EC.
- **`r3_returns`** (pass 1, prototypes): a function whose own draft is void but whose callers declare an integer or
  pointer return for it (pass 1 drafts the callers without our prototypes) returns that type in ctx_protos.h: the
  callers then take the value (fn_800BD5F0: `lbl_8044F710 = fn_8008EE04(0xE, lbl_802200E4);`, fn_8006B3D0,
  fn_8008EE04 itself). **`void_self`** (variant): the function's own draft as void again where that is better. Float
  returns are not taken (m2c declares them from a caller's unrelated f1). `R3_KEEP_VOID` lists three functions left
  void: with a return type their callers that ignore the value compile to other registers (fn_8008B930:
  fn_8011DBB4, fn_80122044; fn_800972D8: cr_SEQ_TITLE_Enter, fn_80130848; fn_800932D0 itself), all 100% while void.
- **`clz_intrinsic`** (rule): m2c's `CLZ(x)` as mwcc's `__cntlzw(x)` (42 drafts called a function that does not
  exist); with **`plain_byte_tests`** (variant: `(u8) *(P) != 0` without the cast, `cmplwi` instead of `clrlwi.`)
  fn_80038BD8.
- **`bitwise_lvalues`** (variant): `M2C_BITWISE(T, G[i])` on a global as `(*(T *) &G[i])`: cr_game.h's macro is a value
  cast, which converts (fn_8008CE9C: one `lwz` in the original, `lfs` + `__cvt_fp2unsigned` in the draft).
- Types and names: `cr_StageSelectorState.unk_350` is s8 (fn_80111E54 stores -1, fn_8011023C tests it with extsb);
  fn_8017A0E4 = psInitParticle (symbols.txt; its caller passes the argument Melee's prototype has).
- Full batch 12: **496 -> 531 drafts at 100%**, compile failures 46 -> 44, 530,264 bytes equal; **721 units
  from C**, 23.81% of the code (690 before).
- Left (tried): the fn_80075258 family (arg0 / arg1 in each other's registers), fn_800F1860 (a saved r31 nothing
  uses), fn_8008E7EC, fn_800DF320 (scheduling), cr_CharaEntry_OnLoaded and fn_800F5EF0 (compile now by hand, far from
  equal).


### Step 109a: hand drafts and generator rules for the 90-99% drafts (Nov 2, 2026, branch hand109a)

Step 108's method on the 69 drafts at 90-99% of batch 12 (a second worker, branch hand109b, took the compile failures
and the 80-89% drafts at the same time). Each draft diffed against the original with tools/sched/scratch/hd/pr.py in
decomp/drafts/hand; where one fix is a pattern of m2c's reading, it became a draft_batch.py rule (all VARIANTS, tried
per draft and kept when better).

- **Hand drafts** (tools/hand_drafts; each file's first line says what was changed):
  fn_80069614 (the pointer advanced before the return, `p += 0x10; return *(u16 *) p;`: lhzu), fn_80114420 (the
  missing middle parameter declared: the u8 third one is homed on the stack and read there), fn_80095D7C (Melee's
  inlined HSD_JObjSetupMatrix, `if (j == NULL || !dirty(j)) return;`), fn_80076404 (two 8-byte struct copies),
  fn_80026A7C (record_pointers + local_same_casts with stack_struct: three variants together), cr_SEQ_BATTLE_FrameC
  (the camera matrix set through a static inline: each inlined parameter its own saved register), fn_800ADDBC /
  fn_800ADF7C / fn_800AE13C (the same body: four locals in r28-r31, the count copied once into the variable the test
  reads, the level an u32), fn_800A44D4 / fn_800A4CEC / fn_800B1FBC (the same block: the index `(s16)` of the u16
  field, the joint list against ((void *) 0), the 0x150 copy stored as s16), fn_800DF018 (the computed store before
  the constant one), fn_80019E84 (a GXColor struct copy, the loop counter after the lookup, with
  cr_FileSlot_QueueRead's s32), fn_80112294 (`x - (s8) ((s8) x / 6) * 6`: main.dol loads the byte twice),
  fn_800F6A44 (the if chain on an u32 parameter and `{ (void) 0; }` in an empty then-block), fn_8008578C (three Vec3
  and an Mtx: the frame of 0x70), fn_80086F54 (a 12-byte struct copy), fn_800440BC (`a = b = 0` for two flags),
  fn_8007D638 (a switch with a break per case, `x = x - 0xC`, the 0.1 step compared in double), fn_8012855C
  (fn_800AA17C called with its argument, s32 conversions, one constant left an extern). 21 files (37 in all).
- **Rules** (draft_batch.py; each a VARIANT unless said otherwise):
  - `byte_loops`: m2c's byte copy / fill loop as `for (c = 0, p = X; c < N; c += 1) { *p++ = V; }` (fn_80078F94,
    fn_80078F00: the counter and the pointer in main.dol's registers).
  - `local_same_casts`: `*d = (u8) (*s);` on a local `u8 *s` without the cast (drop_same_assign_casts for locals:
    fn_80026C50, fn_80026B40, fn_8008E598).
  - `record_pointers`: a pointer `p = B + ((I * S) + O)` or m2c's two statements `p = (I * S) + O; p += (s32) B;` as
    `&((u8 (*)[S]) (B))[I][O]` (fn_800442EC, fn_80026BF4).
  - `unsigned_immediates`: m2c's sign-extended `cmplwi` immediates (`!= -0x50D0U`, `(u32) (x + 0xFFE00000) !=
    -0x50D0U`) as the unsigned constants (`x != 0x20AF30`: fn_8008E3F8, fn_8008FA8C, fn_8008FC38; with the two below
    fn_8000E1E4, fn_8000F7A4).
  - `refold_shifts`: `v = K; v = K >> N;` as `v >>= N;` (srawi of the variable).
  - `irregular_switches`: m2c's `switch (X) { /* irregular */ ... }` with every case ending in break / return as the
    if / else-if chain (X reloaded per test: fn_80107310, cr_Pad_StepIndex, fn_800E4618, fn_8000E1E4, fn_8000F7A4).
  - `search_loops`: m2c's `loop: if (A) { R = X; } else if (B) { R = Y; } else { I; goto loop; }` (an inlined
    lookup) as `for (; !A; I) { if (B) { R = Y; goto found; } } R = X; found:` (fn_800AF398; 676 m2c loop labels,
    the CHAR / OBJ script commands' character lookup among them, which also need other things).
  - `for_break_tails`: `loop: if (C) { S; if (T) { I; goto loop; } }` as `for (; C; I) { S; if (!T) break; }`
    (fn_8011725C).
  - `advance_copies`: `t = G; G += K; v = t;` as `v = G; G += K;` (cr_C_ACT_REPEAT).
  - `wide_return_locals`: the returned local of a byte / halfword function as an int (`extsb r3` at the return:
    fn_800EA548, fn_800EA6BC).
  - `wide_field_locals` / `wide_const_locals`: a narrow local only loaded from a field of its type, or only set and
    stepped by constants, as an int (no extsh at each use: cr_C_ACT_CHK_BIT, fn_800A514C); `field_void_null`:
    void_null for fields only (cr_C_ACT_CHK_BIT).
  - `signed_temps` / `field_pointer_tests`: a u16 local used only as `(s16) v` loaded as `(s16) LOAD` into an s16,
    a `while` loop's word-field test against ((void *) 0) (fn_800BC768; the hand drafts fn_800A44D4 / A4CEC / B1FBC).
  - `struct_copy_macro`: m2c's `M2C_STRUCT_COPY(D, S, N)` (a call to nothing in the draft) as a struct assignment
    through a struct of N bytes (cr_BattleRobo_InitBody; 22 drafts have the macro).
  - `named_param_homes`: param_homes for a parameter our prototypes name (cr_Motion_Play's `frame`).
  - `byte_index_sums`: `B + (I + O)` as `&((u8 (*)[1]) (B))[I][O]` (addi + stbx: fn_80104F70).
  - `mask_compounds` / `macro_returns`: `L = (s32) (u16) L;` as `L &= 0xFFFF;` (the address formed once), and
    `{ (void) 0; return; }` for an early return alone in its block (beq / b kept: fn_8002557C);
    `empty_macro_blocks`: `{ (void) 0; }` in m2c's empty blocks (fn_800F6A44 by hand); `param_retypes`: a `void *`
    parameter copied once into a typed local takes its type (written for fn_800442EC, which record_pointers alone
    fixed). The last two gave no draft at 100% by themselves in this batch.
- **Prototypes** (pass 1, ctx_protos.h): `R3_SET` and `PARAM_SET`, return types and parameter lists that the callers'
  code shows and the callee's own draft cannot. A caller that ignores the value of a value-returning call keeps r3
  busy after it (its next temporary is r4), one that calls a void function takes r3: fn_8006A4E0 returns s32 (it
  ends with `bl fn_8006A7D8`; fn_800F8D4C 100%), fn_80098484 and fn_80098290 are void (m2c returned a leftover r3:
  main 100%, fn_80098290's 77 callers' near drafts up, none of the 100% ones changed); fn_80026F58 takes a u8 (its
  caller passes it unextended), fn_800AA17C an s32 (fn_8012855C passes 0 / 1; its own 100% draft never reads it).
  **symbols/cr_types.h**: cr_FileSlot_QueueRead returns s32 (`li r3, 0` before its blr; fn_80114214, fn_80019E84).
- **pool_floats**: a float object of 8 bytes whose second word is zero (dtk's last float of a file pool with the
  pool's padding, `f32 X[2]` in the context, read `*X`) is a literal for float_literals too (fn_8005C7F4's 0.0f; 69
  such objects).
- **Cost**: the new variants change the text of many drafts (loops, switches, returns), so they are tried only for a
  draft whose best so far is at least `NEAR_MIN` = 70% (`VARIANTS_NEAR`); without that the batch took several times
  as long.
- Full batch 12: **531 -> 583 drafts at 100%** (52 new, none lost; at 90-99% 69 -> 35), compile failures 44 (unchanged),
  533,876 bytes equal; promoted to decomp/src/game: **761 units from C** (721 before), 24.72% of the code; batch 9:
  main.dol retail SHA-1, relocations and static shift PASS. gen_dtk_splits.py's CDEF (also promote_drafts.py's)
  skips a `static inline` helper of a hand draft (cr_SEQ_BATTLE_FrameC, fn_80095D7C). The batch took 4,974 s with
  7 jobs while the other worker's batch 12 ran on the same desktop.
- New at 100% by rules alone (no hand draft): cr_BattleRobo_InitBody, cr_C_ACT_CHK_BIT, cr_C_ACT_REPEAT,
  cr_Motion_Play, cr_Pad_StepIndex, fn_8000E1E4, fn_8000F7A4, fn_8002557C, fn_80026B40, fn_80026BF4, fn_80026C50,
  fn_800442EC, fn_8005C7F4, fn_80078F00, fn_80078F94, fn_8008E3F8, fn_8008E598, fn_8008FA8C, fn_8008FC38,
  fn_800A514C, fn_800AF398, fn_800BC768, fn_800E4618, fn_800EA548, fn_800EA6BC, fn_800F8D4C, fn_80104F70,
  fn_80107310, fn_80114214, fn_8011725C, main (31; the other 21 are the hand drafts).
- Left (tried): fn_800A0804 (the epilogue's `lwz r0` / `lwz r31` order after a store:
  scheduling); fn_80080FD8 and cr_CAMMODE_MIDDLE_Frame (registers: main.dol saves r27-r31 with two of them unused,
  or has the parameter in r31 before the flag; no copy or inline form tried gave it; fn_80080FD8 93 -> 92%, its
  best variant changed); fn_80064FE4, fn_80050388, fn_80053A20, cr_Records_BuildBodyRows, fn_8001838C,
  fn_800EAC48 (the order of independent loads, which volatile register: scheduling); fn_80038CF8, fn_800B7D8C (float
  register numbering around a literal; fn_800B7D8C 249 of 256 with the record form by hand); fn_800B1B2C,
  fn_80118C08 (an inlined lookup with one more saved register and a larger frame than any form tried); the CHAR /
  OBJ commands search_loops brought to 90% (cr_CHAR_MOVE, cr_CHAR_MOVE_LENG, fn_800FBDF8: not looked at); step
  108's list (the fn_80075258 family, fn_800F1860, fn_8008E7EC, fn_800DF320, cr_Text_MaskUnownedSpecialPart), not
  taken up again.

### Step 109b: the compile failures, loop and search rules, 14 hand drafts (Nov 2, 2026, branch hand109b)

Two parts: (A) the 44 compile failures of step 108 and the 2 m2c failures; (B) the drafts at 80-89%, smallest first
(the 90-99% ones were branch hand109a's). Method as in step 108: tools/sched/scratch/hd/pr.py on a draft (left column
the original) in decomp/drafts/hand (gitignored), then a hand draft in tools/hand_drafts or a rule in
tools/draft_batch.py. All of this branch's generator code is one block before `main()` ("step 109b"): it wraps
`repair` and `postprocess` and appends to `OPTIONAL` / `VARIANTS`; outside it only three places changed (byte_steps'
empty operand, the `variants_skipped` test in one(), and gen_dtk_splits.py's reading of a `static inline` helper).

**(A) Compile failures: 44 -> 0.** `repair_109b` (its fixes are marked `[109b]` in the fix list) runs after repair()
when that has no fix, and before it for `pointer/array required` / `type mismatch` (repair()'s `load through a T`
reads `slot->member[i]` as a deref; its step 80 rule split `a[i].m - &a[i].n` inside the member). Its rules:
- struct-typed accesses used as scalars (`*(((cr_BattleRobo *) ((u8 *) p + off))) |= 1`): the width of the
  statement's other field, f32 for float messages, else s32; a scalar stored to or read from a struct / array local
  (stack_struct's `Point3d sp14`, which m2c also uses as a float): through its address; `*sp20` of a struct pointer
  as a scalar; a struct field pointer type (`cr_CustomizeState **`) as the value's;
- `void *` / function-pointer locals that m2c reads through (`void (*temp_r31_48)();`, `s32 (*sp2BC)[];`, with
  m2c's `/* compiler-managed */` comment, which repair()'s own rule did not read): `u8 *`;
- float stack slots used as integers (an index, a pointer step, a bit test, a product added to a pointer): `(s32)`
  or `*(s32 *) &slot`; pointers in products, masks, `~` or a bracketed byte offset: `(s32)`; `int + struct pointer`
  and a struct pointer indexed in bytes: through `u8 *`;
- comparisons of a pointer with an integer / a function / another pointer type: both sides `(u32)`; differences of
  two pointer types: both `(u8 *)` (operands found by bracket balance: `&((cr_Chara *) sp268)->unk_194c - &x->id`);
- `(f32) strcpy(...)`, `(f32) pointer` and `(s8 *) floatslot`: through `(s32)`; `(u8) sp8` of a GXColor: its first
  byte; `(void (*)(...)) x` stored to a `void (*)()` member: `(void (*)())`;
- `x = call(...)` / `x = array` that the target's type does not take: the value cast; `&(u8 *)0x801C0000[0x5BD0]`
  bracketed;
- an integer stack slot used as a pointer (`sp90->unk_00`, `sp3C[1]`: m2c reuses one slot for several values):
  `((__typeof__((SRC) + 0)) sp90)` with SRC what the draft stored there (`(long) (&cr_g_BodyData[i])`) or the same
  member read through a typed expression elsewhere; a member that is not in the slot's struct: the slot cast to a
  struct of the context (ctx_pp.c) that has it, the next such struct when that fails (`OSCalendarTime` has `min`);
- `case` / `default` labels m2c put outside their switch (irregular switches): commented out (the draft compiles;
  the jump into it is lost).

byte_steps (step 64) wrote `((T *) ((u8 *) sp26C - ))&x` for `sp26C - &x->m` (an operand starting with `&`): it now
leaves it alone (fn_800F21C8). The former failures now score 0-83%: cr_CharaEntry_OnLoaded 100% by hand (below),
fn_8010E47C 83%, fn_80023CBC 81%, cr_CHAR_COUSE_SET_PS 79%, fn_800F5EF0 75%, most of the 2-13 KB ones 1-20%.
`variants_skipped`: a draft with more than 3 fixes, some of them repair_109b's, is not tried with the VARIANTS (each
variant repeats the whole repair loop: up to 160 compiles of a 13 KB file).
The 2 m2c failures (fn_800A0E64, fn_800A63C0, 11.7 / 15.4 KB) are left: their jump table (lbl_8022AF44) is read
through a stack slot (`stw r12, 0x13c(r1); lwz; lwzx; stw; lwz r0; mtctr; bctr`), which m2c's jump-table pattern does
not recognise; the asm would need that sequence rewritten in m2c's form (and the table named) before drafting.

**Hand drafts** (tools/hand_drafts; the first line of each says what changed; 14 new, 12 of them 100%):
cr_CharaEntry_OnLoaded (a compile failure: the relocation loop with byte pointers, the header's pointer members
added as words), fn_80020204 (the third counter array as `p->unk_70[i]`: m2c's byte address in a temporary took a
saved register), fn_8008F4F4 and fn_80090190 (a table slot tested as a pointer against `((void *) 0)`: `li r0, 0;
cmplw`; fn_80090190 also has an empty debug check after the add: `lis r0, 1; cmplw`, no branch), fn_800BB7D8
(lbl_8037569C's address in a local before the load call), fn_80096374 (the camera tested against `((void *) 0)`, its
view matrix taken into a local), fn_800BD720 (one alpha variable for both fades, loaded then stepped in place), and
**static inline helpers**: fn_800BC4C0 / fn_800AE2FC (the rank count: in the original the inlined body's own
variables take r31 / r29, which no single function's C gives), fn_800AC158 (a speed-table search with an early
return and an empty debug check of i after the loop), fn_800FA5AC / fn_800FBDF8 (the slot-bit search; the rule below
now gives it too). Not 100%: fn_8008E158 (83 -> 94%: the step table as `*(s8 *) &((u8 (*)[1]) arg1)[idx][2]`; the u8
field sum lands in r5 where the original keeps r0) and cr_FileSlot_QueueRead (the buffer parameter itself, `return
0`; 106 of 116 by pr.py, the queue address loads in another order). A `static inline` function emits no code of its
own at this level, so promote_drafts.py links these files as they are; gen_dtk_splits.py now skips `static inline`
definitions when it reads which function a game file defines.

**Rules** (VARIANTS unless said otherwise: tried per draft, kept when better):
- **`inline_search_helpers`**: m2c's reading of an inlined search, `loop_N: if (I >= K) { R = D; } else if (C) { ...;
  R = V; } else { I += 1; goto loop_N; }`, as a `static inline` function before the draft (`for (I = 0; I < K; I +=
  1) { if (C) { ...; return V; } } return D;`), called where the loop was; the locals C reads are passed as
  parameters of the same names; `R = search(); X = R;` assigns X directly. fn_800FB688, fn_800FB924, fn_800FF958,
  fn_800FFA50, fn_800FFBB0, fn_800EAF34, fn_800EB004 to 100%; fn_800FA750 83 -> 91%, fn_800FA9CC 83 -> 90%,
  fn_800FBAB4 70 -> 98%, fn_800FEFF8 93 -> 97%, fn_800FE954 74 -> 87% (33 drafts have the form: the fn_800FA5AC family).
- **`word_table_rows`**: `M2C_FIELD(T, f32 *, (R * 8) & 0x7F8)`, `*(f32 *) (T + 4 + R * 8)` and
  `M2C_FIELD(&T[(R * 8) & 0x7F8], f32 *, 4)` as `((f32 (*)[2]) T)[R][0]` / `[1]` (the same family).
- **`stack_element_steps`** (on by default): `*(&sp8 + (I * 2))` and `*(&sp8 + ((I * 2) & 0x1FE))` on a u16 stack
  local as `(&sp8)[I]` (C scaled m2c's byte step by the element), a cast to the slot's own type dropped.
- **`goto_search_loops`**: `loop_N: if (C) { [S0;] if (T) { S; } else { ADV; goto loop_N; } }` as `for (; C; ADV) {
  [S0;] if (T) { S; break; } }` (ADV an increment or `p = p->next`): fn_80078B60, fn_8006E6E8.
- **`goto_while_loops`**: `loop_N: if (C) { S; goto loop_N; }` as `while (C) { S }`; **`goto_exit_loops`**:
  `loop_N: if (T) { return X; } S; goto loop_N;` as `while (!T) { S } return X;`: fn_800A4784, fn_800964B0,
  fn_800F8C04, fn_8008E9FC.
- **`stack_param_alias`**: m2c's `sp8 = arg0;` / `sp.sp8 = arg0;` (the parameter's home slot read as a local) as the
  parameter itself: fn_8006E690, fn_8006E778, cr_Motion_Play, cr_Motion_PlayFromFrame1; fn_800B1944 85 -> 87%.
- **`drop_local_deref_casts`**: `*q = (u8) (*p);` of a `u8 *p` without the cast: fn_8008E598, fn_80026B40,
  fn_80026C50.
- **`record_array_address`**: `B + ((I * S) + O)` of a byte pointer as `&((u8 (*)[S]) (B))[I][O]`: fn_80026BF4,
  fn_800442EC.
- **`irregular_switch_ifs`**: a switch m2c marks `/* irregular */` (a compare chain in source order, no jump table)
  as the if / else-if chain: cr_Pad_StepIndex, fn_80107310; with **`self_update_refs`** (`x = K; x = K >> 1;` as `x =
  x >> 1;`: srawi on the register) fn_8000F7A4, fn_8000E1E4.
- **`minus_one_compares`** (on by default): m2c's `(u32) (x + 0x10000) == -1U` (its reading of `addis r0, r3, 1;
  cmplwi r0, 0xffff`, another value) as `x == -1U`.
- Full draft runs: **531 -> 569 drafts at 100%** (38 new: 12 by hand drafts, 26 by rules), compile failures **44 ->
  0**, m2c failures 2 (left), 556,820 bytes equal; batch 12 + batch 9: **756 units from C** (721 before), 24.28% of
  the code, retail SHA-1 and the static shift check PASS. Three drafts lost a point from the default rules
  (fn_8000F230 36 -> 35%, fn_8008C430 18 -> 17%, fn_8011B1B0 4 -> 3%). A full run took 80 minutes here (26 before) with
  another branch's batch on the same machine at the same time; a 120-function sample took 234 s with step 108's
  draft_batch.py and 186 s with this one.
- Left (tried): fn_80095E00 (register-only: arg0 needs r31, which only a dead extra use of it gives), fn_800EA33C (81 ->
  85% by goto_while_loops; its two counters swap registers), fn_8008F998 (the u16 index's temporary; three spellings
  worse), fn_8008E158 and cr_FileSlot_QueueRead (above), fn_80015E2C (the original keeps its string pool's base in
  r31: string literals of its file, step 83), fn_800943D8 (a saved r28 nothing uses, as fn_800F1860), fn_800699E0 /
  fn_8000860C / fn_8001E488 (float register and load order), fn_800A0C20 / fn_800A0CB0 (r27 / r28 swapped),
  fn_800119A0 (a nested loop whose exit increments the outer counter: no rule yet), the script-command handlers at
  80-89% (cr_S_*, cr_SYS_*, cr_F_*, cr_CM_*: schedule-only, the step 91 wall), and the two m2c failures above.

### Main session, Nov 2, 2026: types from the community leads, effect on the drafts (on main)

Names and types added on main while steps 109a / 109b ran (docs/decomp.md "Community address leads checked",
docs/robo_tables.md "The stat triples and the leg values"): cr_BodyStats.leg_tri_* / air_type / air_jumps / air_dashes,
cr_LegData.leg_values, cr_RoboLegStats, cr_BattleRobo.action / tag_cooldown / air_actions_left / timer_len / timer_left,
cr_BattleParams.tag_cooldown, cr_GameMode.battle_mode, cr_StageSelectorState.col / row / top_row, cr_BgmSlot,
cr_BgmRequest, cr_TagTeam, cr_BattleRoboState. Drafts re-run on the functions each change touches (`--only`, compared
with the report before): fn_800F7914 82% -> 98%, fn_80086CA0 77% -> 83%, fn_80024848 74% -> 82%, fn_80048B10 79% -> 83%,
fn_80045CE8 71% -> 74%; cr_g_CurrentBodyStats is typed `u8 *`, not `cr_BodyStats *` (its readers index it by byte offsets,
which m2c writes through a struct pointer as a struct value: a new compile failure). Two hand drafts follow renamed
fields (fn_80112294, fn_8012855C).

- **fn_800F7914** by hand: the u16 parameter used directly (m2c's copy homes it on the stack) and the pointer copy placed
  after the first test give 185 of 186 words; one extra `stw` of the never-used copy is left (tried: `(void)` use,
  `register`, s32).
- **`--only` runs use the last full run's ctx_protos.h**: a function renamed since then (cr_Rand_Int = fn_8001B3F0) has
  no prototype there and its callers fail to compile in a partial run only; the full batch 12 is the test.

### Step 110: read-only data plain for one draft, the variant plain_readonly (Nov 2, 2026, drafted, not yet run)

data_decls declares the .sdata2 / .rodata objects with `CR_RO`, which ctx.h defines as `const` unless the draft defines
`CR_NO_RO` first. `const` lets a load move ahead of stores through pointers (cr_CM_ZOOM needs it), but four drafts
that were 100% before need the plain order (fn_800055E8, fn_80038BD8, fn_800FFA50, fn_800FFBB0). The variant
`plain_readonly` (alone and with register_order) marks the draft; build() writes `#define CR_NO_RO` before
`#include "ctx.h"`. The context is preprocessed a second time with CR_NO_RO (decomp/drafts/ctx_pp_noro.c), and
promote_drafts.py reads such a draft's declarations from it; a file object whose drafts differ in this is not formed.
The full batch 12 is the check (plan.md item 3).
Nov 3: a `--only` run of the four: fn_800055E8, fn_800FFA50, fn_800FFBB0 at 100%; fn_80038BD8 at 98% (one word:
`clrlwi.` for m2c's `(u8) *(...) != 0`, the original `cmplwi`) until the variant (plain_readonly, plain_byte_tests)
(step 108's byte test without the cast) was added: then 100% too. The full batch 12 is still the check.

### HSD near misses by hand, Nov 3, 2026 (no gain; for the permuter next)

Tried with a variant runner (each spelling compiled with the real build's flags and scored by nd.py; the scripts were
in the session scratchpad):
- **HSD_ShadowSetViewingRect (98.2% of words, one `fabs` one slot late)**: main.dol takes |right| before |left| in the
  second max, as in the first block (which the double temporaries fixed in step 104). Eleven spellings of the second
  block: double temporaries (plain, nested, declared apart, reused from the first block), the ternary, `!(l > r)`,
  `height = r; if (l > r) height = l;`, one temporary for |right| only, casts, f32 temporaries, the comparison
  reversed. Every double-temporary form puts the order right and hoists the call's `mr r3, r31` into the compare
  block (main.dol keeps it after the branch): 92.9%; the others order the registers or the branch wrongly (95.5% at
  best). Not reached by spelling.
- **mkBillBoardMtx (98.3%, f2 / f3 swapped)**: in the else branch main.dol gives `BB_EPSILON + sqrt` f2 and `-az.y` f3,
  and the quotient reuses the divisor's register; ours the other way. Thirteen spellings (chained assignment, through
  ay.y, a temporary before / after, a local only in the else branch, two locals, the quotient into mag or a new local,
  without the unused `f32 t`, negation first): identical code for all but one (negation first: 84.8%, the loads
  move). The allocation does not follow these spellings; it is decided earlier (the inline bb_sqrtf's volatile
  return is the last thing that differs from a plain sqrt).
Next for both: the permuter on the corrected setups (plan.md item 5: `LIST="HSD_ShadowSetViewingRect:shadow
mkBillBoardMtx:displayfunc ..." MINUTES=20 tools/sched/hsd_permute.sh`).

### The run 0x80080A94-0x800811E8 and fn_800528B8 by hand, Nov 3, 2026 (in progress)

The file runs one to three functions short of complete (promote_report.txt) are the cheapest code: one match links
the whole run with its data. Hand drafts written from the asm (a quick checker compiled them with draft_batch.py's
build() and compared in seconds; scratchpad scripts):
- **fn_800528B8** (the run cr_Motion_UpdateBlend..fn_800528B8, 1 of 2): m2c 68% -> **96%**. Found: the saved
  registers go by declaration order (u8 set, s16 kind, u32 flags, an unused s32, s32 diff -> r31..r27; the unused one
  is the r28 the original saves and never touches); the never-read u16 at sp+8 is a plain `u16 sp8` stored once (as
  in its matched neighbour cr_Motion_UpdateBlend); `sp8 = ...` before `set |= 1` makes the original's reload of
  cr_g_CurrentChara_0678. Left: the frame count before its int-to-float conversion loads into r3 in the original, r0
  in ours (32 spellings: casts, temporaries, inline helpers, dead uses; named locals and inline parameters always
  take a saved register).
- **fn_80080E24** (run of 7, 3 missing): 88% at the first draft; left: f2 / f3 of two constants (eight operand
  orders of the expression and const / plain constants give identical code).
- **fn_80080C7C**: 71% -> **83%**: the parameter used directly (the original keeps it in r30, no copy), the two
  never-read stack floats declared in reverse order, a third u8 declared first for the original's r29, the two
  unused saved registers r26 / r25 as dead locals after `b`, the store before the table load in each block. Left:
  the third block (a bool the original folds into the direct test while keeping its register).
- **A relocation dtk lacks**: both run functions index lbl_8044EAE0 by a register through r13 (`addi r4, rI,
  -0x58E2; stbx r0, r4, r13`). MWCC emits R_PPC_EMB_SDA21 to lbl_8044EAE0-2 on that addi (mwld keeps rA), dtk's
  object has no relocation there, so promote_drafts.py would refuse them even at 100% ("relocation sites differ").
  **Done in promote_drafts.py** (check_one): a relocation only the draft has is accepted when it is
  R_PPC_EMB_SDA21 on an `addi` whose rA is a general register (not r0 / r2 / r13); the bytes are compared before,
  batch 9's retail SHA-1 checks the linked value. Not exercised yet: neither function is at 100%.
Next for all three: the permuter on the hand drafts (with the HSD near misses, plan.md item 5).

- **fn_800528B8**: 96% -> **100%** with the permuter (12 minutes, 8 workers): a reload of `flags` (+0x10, never
  read) right after the `fn_80051384` call gives the original's r3 for the frame-count load. With it the file run
  cr_Motion_UpdateBlend..fn_800528B8 (0x800527D4-0x8005299C) is complete.

### The run 0x800057B8 by hand, Nov 3, 2026 (in progress)

The file run 0x800057B8 (the robo input / init file: fn_800057B8, fn_80005BC8, cr_BattleRobo_InitFromLoadout,
fn_80007CE4, fn_8000805C) was 5 short. fn_80005BC8 and fn_80007CE4 were done earlier the same day.

- **fn_8000805C** (0x5B0): m2c 70% -> **100%** (tools/hand_drafts). The MSL `sqrtf` with its NaN branches written
  out in the function: the `x > 0` Newton steps, `x < 0.0` (a double compare) and the `__fpclassifyf` switch on a
  stack float with its result in a saved register (if / else, not `?:`: the ternaries took two more saved
  registers). NAN is read through `__float_nan` as an array (`lis` / `lfs`: as a plain `f32` extern it was taken for
  small data). Three more points: `arg1` is read through its address (the original spills it to 0x8 and reads it
  back); the chained zero stores of the reset record are `a = b = 0` with the second store first; `fn_8001C98C` is
  called as returning its first argument (the length call that follows reuses r3; its file keeps the void
  prototype). The stack order (4 locals, 24 orders tried) and `dir.z * dir.z + dir.x * dir.x` set the rest.
- **fn_800057B8** (0x3D0): first hand draft 69% -> **99%**. buttons is a `u32` (no `clrlwi` after each `ori`), the
  stick bytes go through `(u8)` and then `(s8)` into s32 locals (the original's `lbz r28; extsb r28, r28`), the
  fabs results are `f64` locals copied to `f32` ones (the six saved FP registers), `(s32) battle_mode == 2` (cmpwi).
  Left: one `mulli` scheduled before the camera-pointer load (12 spellings of the stick reads and the camera test
  give the same order); for the permuter.
- **cr_BattleRobo_InitFromLoadout** (0x8B4): m2c 36%, hand draft ~50%. Every table it reads (the loadout rows at
  0x801C5BD0, the part records at 0x801C6C44 / 0x801C952C / 0x801D4860 / 0x801D9590, the 0x78 records at
  0x801C5C10, the floats at 0x801C5150) is reached from one base kept in r30 (`addi r30, lbl_801C50D0@l`), each
  table's address computed fresh (`addi rX, r30, 0xB00`, seven times, hoisted to the top by the scheduler) and
  indexed with the field offset in the index (`slwi 4; addi 2; lhzx`). The gun record is reached by its own symbol.
  Small-file tests: GC 1.3.2 to 3.0a5.2 at levels 0-4, static / const / initialised tables and `#pragma pool_data`
  never pool a file's data, so the base is a local of the source. A constant offset from it is always folded into
  the displacement (seven forms tried); only an explicit pointer (a local or an inline function's return value)
  gives the original's instruction pattern. The inline form takes saved registers for all seven pointers (r17-r24);
  the original keeps them in volatile registers except r22-r24 (its named locals take r25-r31). Left: the form that
  makes them temporaries.

### Near misses by hand, Nov 3, 2026 (the 85-97% drafts of decomp/build/draftscan)

Scored from the draftscan objects (761 drafts; 54 at 80-99%). Nine more at 100% (tools/hand_drafts, 94 files):

- **Stack structs that are Vec / Mtx locals** (fn_8002EBB0 92%, fn_8002E564 91%, fn_8002E944 91%, fn_8002EFB0 85%):
  m2c writes the stack as one struct with `f32 spXX[4]; u8 pad_YY[32];` for each matrix and drops the last
  matrix's padding, so the frame is 0x20 short. As `Vec` and `Mtx` locals the frame is the original's; the
  matrices are declared in reverse order of their stack slots. A converter (scratch `mtxconv.py`) does it; 15
  drafts of the draftscan set have such a struct, the four above match, fn_80015388 differs in more than the frame.
- **fn_8002E2A0** (97%): the two floats passed to fn_8001F060 are the y / z of a Vec (x unused).
- **fn_800210B0** (96%), **fn_80021350** (95%): the queue head is advanced before the current read is taken
  (the original materialises that address last). fn_80021350 also needs an empty statement in its early return,
  `if (result == -1) { (void) 0; return; }`: it gives the original's `bne +8; b end` where a bare `return` gives
  `beq end` (a compiled-out debug macro). Not a generator rule: `bne +8; b` is common for other reasons (3,328
  sites in 262 functions).
- **fn_80053A20** (96%): in each of three swaps, the two negated components read before the plain one.
- **fn_80050388** (95%): in the reset of the best-hp list the count and the first entry are cleared before the
  robo mask is read.
- **cr_SEQ_TITLE_FrameA** (94%): fn_800EB518 called through a void cast (its r3 at return is a coincidental -1
  that no caller reads; as an s32 call, r3 stays reserved) and in case 1 the flag set before the state store.
- Left for the permuter (scratch queue 2): the fn_80075258 family (fn_80075258 / 388 / 4B8 / 5E8, 96%, one source
  over four tables: the two parameters come out in swapped saved registers, a copy local spills to the stack),
  fn_80038CF8 (96%, a compare against a zero register), fn_8001838C (95%), fn_8000E3B8 (94%, scheduling),
  fn_800057B8 (99%), fn_8006EEF8 (74%, see above).

## Nov 3, 2026 (evening): two step 111 rules and 7 more hand drafts (main session, uncommitted, the full batch 12 not run)

Started from the 80-99% drafts of `scores.py decomp/build/draftscan` not yet in tools/hand_drafts (41), sorted by a
scratch triage: a draft whose differing words are the original's words with other register numbers is
**register-only** (12: the permuter's work, scratch queue 3 `tools/sched/scratch/hand/perm/run_queue3.sh`); the rest
are **structural** (29: by hand, smallest first).

- **param_self_copies** (draft_batch.py, step 111, a variant with param_copies and param_aliases left out): m2c's
  `var_r30 = arg0;` copy of a parameter written as the parameter itself plus `arg0 = arg0;` in the copy's place.
  param_copies drops the copy and the parameters come out in the other saved registers (arg0 r29, arg1 r30 where
  the original has r30 / r29); the copy itself costs a stack round trip (`stw r3,8(r1); lwz r30,8(r1)`). The
  self-assignment keeps the copy's place in the allocation without the spill. Found by decomp-permuter (queue 2,
  score 0 on fn_80075258 in 12 minutes). On the drafts with a parameter copy: fn_80075258 / 75388 / 754B8 / 755E8
  96% -> 100%, fn_80021014 65% -> 89%; others lower, which the variant scoring leaves out.
- **goto_for_loops** (draft_batch.py, step 111, a variant): m2c's unstructured loop with its exit test at the top,
  `X = I; loop_N: if (T) { E } S; INC; goto loop_N;`, as `for (X = I; !(T); INC) { S } E`, innermost loop first; E
  ends in a return or a goto (a nested loop's exit block is the outer loop's `step; goto loop_M;`, which the next pass
  reads as that loop's tail). goto_exit_loops (step 109b) takes only a single `return` as E. fn_800119A0 86% -> 100%.
  Of the 87 drafts below 100% with goto loops only 3 have this shape: the rest jump into switch cases or else
  branches (hand work).
- Hand drafts (tools/hand_drafts, now 101 files): **cr_SEQ_OPTION_FrameA** (86%: the sub-frame table's functions
  take no argument and return u8; m2c passed the index), **fn_800119A0** (the two loops as for loops),
  **fn_80075258 / fn_80075388 / fn_800754B8 / fn_800755E8** (param_self_copies), **fn_80021014** (param_self_copies,
  then the goto loop as `while (1) { if ((s32) b == 3 && n == 0) break; ... }`: the exit test at the top, the byte
  compared signed, cmpwi).
- **fn_80080C7C** (83%, one of the four drafts that hold back the 9-function file run 0x80080A94-0x80081430): the
  original indexes `lbl_8044EAE0[idx - 2]` with idx in r31 inside `if (idx == 2)`; our compiler folds the known
  value into a constant (`li r4, sym`). Not undone by: a copy of idx, compares written `2 == idx`, `idx - 2 == 0`,
  `!(idx != 2)`, `(idx ^ 2) == 0`, an unsigned idx, an inline index helper, reading the field each time (worse:
  reloads after each store). In queue 3.
- **fn_8001E488** (83%): only the float registers of three loads differ (f1/f2/f3 in the original, f2/f1/f2 in
  ours); Vec locals, declaration orders and sum orders tried, no gain. In queue 3.

- **null_reg_compares / null_reg_compares_u** (draft_batch.py, step 111, variants): every `x != NULL` / `== NULL`
  (the `_u` form also an unsigned `!= 0U` / `== 0U`) as `(u8 *) (x) != (u8 *) NULL`. mwcc then keeps the zero in a
  register, `li r0, 0; cmplw r3, r0`, where `x != NULL` gives `cmplwi r3, 0`. The original has that form at 154 sites
  in 68 drafts below 100% (a scan of draftscan/asm). All sites at once: on the 68, 20 gain (fn_80064234 73 -> 91%,
  fn_80064AB8 69 -> 86%, cr_Heap2_Free / fn_800233D4 / fn_800237F8 52 -> 66%, fn_8007A800 82 -> 85%), none reach
  100% alone; a function that mixes both forms needs a hand draft (fn_80038CF8: only the 0x964 / 0x968 tests, 97%).
- **stack_struct_param_homes** (draft_batch.py, step 111, a variant, also with either null_reg_compares form):
  m2c's `--stack-structs` puts a parameter's stack home into the stack struct (`sp.sp8 = arg0;`, sp.sp8 read for
  arg0). The field and the copy go and the parameter is used; with a field of its own, the struct's other members
  (spilled locals) sit one slot higher. 11 drafts have the form: fn_8003F248 46 -> 61%, fn_8006B46C 48 -> 62%,
  fn_8000805C 70 -> 76%; with null_reg_compares_u **fn_80064234 73 -> 100%** (eight hand-nested levels of a walk:
  the game code is built at level 0 without inlining, so the nesting is in the source).
- More hand drafts (tools/hand_drafts, 118 files): **fn_80024848** (the else store written out, `p->f = p->f +
  0xFFF7`, and the u32 counter compared as (u8)), **fn_8007D800** (a fade test in double, `0.05 + x < 1.0`; cases 2
  / 3 as breaks; the +5 written out), **fn_8001D618 / fn_8001D9C8** (store order: the scheduler moves the zero stores
  first), **fn_800443A0** (a chained assignment `p->x8 = p->xC = v`), **fn_80080558 / fn_80065DB0** (cast-NULL
  compares), **fn_80023354 / fn_80023394 / fn_800232B4 / fn_80023304** (the clear / copy helpers: `for (n = ...,
  d = dst, s = src; n > 0; n--) *d++ = *s++;`, counter r31, dst r30, src r29; the body form decides, not the for's
  init), **cr_CamMode_DefaultStart** (the mode function is called with the camera), **fn_80029C20 / fn_80029F70**
  (two destination pointers reserve r30 / r31, which the original saves unused), **fn_8007DA20** (the switch as its
  compare tree reads: m2c had the clear as the default), **fn_80067250** (the out-parameter is a record of 0x18 or
  more bytes: the 0x30 frame), **fn_80064234**.
- **Checking relocations**: hd.py masks relocated fields, so two orders of stores of the same register value give
  the same words with the symbols swapped (fn_8001D9C8: Top, MaxDepth, Depth matched word for word with the two zero
  stores swapped). `hd.py ... -r` now prints our object's relocations to compare with the original .s; a hand
  draft whose fix reorders statements is checked that way (promote_drafts.py checks them again at link time).
- Queue 3 (`run_queue3.sh`) also takes fn_8003B1BC (87%: the distance table as a member array `dist[i]` gives the
  original's `lfsx`; left: two saved registers swapped) and fn_80070130 (a table call whose first argument is the
  table's own address; not found by hand).
- **param_self_assign / param_self_assigns** (draft_batch.py, step 111, variants): `arg0 = arg0;` (or every
  parameter) as the first statement. The self-assignment changes the order in which a parameter gets its
  callee-saved register: **fn_800410C0 68 -> 100%** (the parameter r31, then the counter). On the 243 draftscan drafts
  at 40-99%: 21 gain (fn_8000B258 52 -> 70%, fn_800208F0 51 -> 60%, fn_8007BB60 60 -> 68%; all parameters:
  fn_8003F248 46 -> 60%, fn_8006B46C 48 -> 61%, fn_80066F58 59 -> 73%).
- **The file run 0x800057B8-0x8000860C is one function short**: decomp-permuter (queue 2) found fn_8000860C (93%):
  the hit flag `(arg0 & 0x3070) != 0` computed before the first counter `+= 1`; **fn_800057B8** 99% -> 100% with
  the stick bytes read as `*(s8 *) &pad.stick_x` (the `(u8)` read and a later `(s8)` put the second `mulli` after
  the camera load; the u8-then-s8 form is in the section above). Left: cr_BattleRobo_InitFromLoadout (~50%, the
  table-pointer temporaries, section above).
- More hand drafts (125 files): **cr_SEQ_STAGE_SELECTOR_FrameC** (both cameras typed `HSD_CObj *`; m2c had
  the first as u32 with its view matrix at +0x54), **cr_SEQ_CUSTOMIZE_FrameC** (the saved matrix is a `Mtx`, m2c
  f32[4]; cameras as HSD_CObj *; in the loop `flags | (0x80000000 | 2)`, one constant: a macro of the two flags),
  **fn_80069504** (a GXColor copied out of a colour table: m2c's four u8 stack fields; the compiler copies it byte by
  byte through the stack), **fn_800671C4** (the copy loop tests the terminator at the top: `for (;;)` with the end
  first), **fn_800410C0**, **fn_8000860C**, **fn_800057B8**.
- The `_Leave` functions (cr_SEQ_TITLE_Leave, cr_SEQ_STAGE_SELECTOR_Leave, ...) set the VI clear colour from a
  GXColor constant through a stack copy, and call HSD object methods with one argument (m2c passed the vtable as a
  second): cr_SEQ_TITLE_Leave 45 -> 97% (left: the order of the colour's byte loads around the register saves),
  cr_SEQ_STAGE_SELECTOR_Leave 42 -> 78% (the save of the old colour and the set of the new one interleave); both in
  queue 3.
- Checked through the pipeline: `draft_batch.py --only` on the 43 uncommitted hand drafts of Nov 3 (a scratch
  --out): compiled 43, **100% MATCH 43** (the 5 saved after that list was taken: in the next run).
- More hand drafts (127 files): **fn_80023F0C** (the search for the first live robo as a for loop over the bitmask
  with a signed test and an int bit index; the update pointer tested against a cast NULL; battle_mode without the
  (u8) cast), **fn_8007D3C8** (fn_8007D800's sibling: the alpha steps written out, `x = x - 5`, breaks for cases
  3 / 2 / 0, case 1's float step written out: the pointer chain is walked again).
- Queue 3 also takes, at their improved scores: cr_Save_ApplyUnlocks (92 -> 93%: `cr_Bits_Clear(p, idx)` with the
  index variable, not the constant m2c folded in; the bit-test result kept in a local, `mr. r24, r3`),
  fn_80044A90 (91 -> 95%: the 3-word copy is a `Vec` assignment; the sum `u16_2 + u16_4 + (n - 1) * step`),
  fn_800677C4 (68 -> 75%: m2c's offset temporary took a fourth saved register; left: two registers swapped),
  cr_Text_GetBodyName / cr_Text_GetPartDescription / fn_80066EEC / fn_80067778 / cr_Archive_GetEntrySize (the
  original adds `base + i * 4` and folds the field offset into the load, `lwz r0, 0x10(r5)`; every one-expression
  spelling is reassociated to `lwzx` by our compile).
- **fn_80015E2C** (84 -> 100%): the string table base kept in a local (r31) for the six text calls; the confirm /
  cancel test is an if chain (m2c read a switch with a case inside an if). **cr_CAMMODE_MIDDLE_Frame** (91 ->
  100%): param_self_assign, the "allowed" flag initialised to 0 at the top (m2c drops the dead store), unk_44 tested
  without the (u8) cast.
- **Relocations checked**: `tools/sched/scratch/hand/relcheck.py` compares the relocation symbol at every instruction
  with the original .s (our literal constants such as `@729` must map one to one onto the original's pooled
  `lbl_` constants). Every uncommitted hand draft passes.
- **fn_8000DE50** (86 -> 100%): the robo count read into a local before the time-limit branch (the original saves
  five registers, r29 holds it) and the time limit chosen by an if chain on it (unsigned compares; m2c read a
  switch).
- Queue 3 also takes fn_8003FB00 (84 -> 91%: the local matrix is a `Mtx`, m2c f32[4], which gives the 0x80 frame;
  the outer goto loop as `for`; `(u8 *) mobj == (u8 *) NULL`; left: arg0 and the inner counter swap r30 / r27) and
  cr_CAMMODE_SNR_AUTO_Y_ANG_Frame (63 -> 87%: fn_80024848's written-out update, `(u16) -v` for `0x10000 - v`).
- **fn_8003D500** (82 -> 100%): `__fabs` (m2c's `fabs()` is MSL's static inline wrapper, which at level 0 keeps its
  argument and result in three saved FP registers), the double constant compared without m2c's `(f32)` casts (an
  extra `frsp` each), and the matrix source read before the joint in the loop. **fabs_intrinsic** (draft_batch.py,
  step 111, a variant): every `fabs(` as `__fabs(`. On the 29 drafts with fabs: fn_800784B0 48 -> 67%,
  fn_8005E270 18 -> 26%, fn_8003C4F0 51 -> 58%, fn_8004E8D4 51 -> 57%, fn_8003D500 82 -> 91%; some lower (the
  variant scoring keeps the best).
- **fn_80048B10** (83 -> 100%): MSL's inline sqrtf written out as in fn_8000805C, and the s16 copy at 0xE4 without
  the (s16) cast. **msl_sqrtf_blocks** (draft_batch.py, step 111, a variant): m2c prints the inline sqrtf in one
  fixed shape (the `__frsqrte` Newton steps in f64, the `(f64) (f32)` result, the `(f32)`-cast zero compare, the
  fpclassify switch on `M2C_BITWISE(s32, sp.sp8)`); the rule writes the source form: an f32 result, the steps
  `g = h * g * (t - g * g * x)`, the NaN through `__float_nan` as an array (`CR_NAN`), the double compare, the switch
  on `*(s32 *) &sp.sp8`, and the copy declared before the value it copies (the copy takes f30, the value f29). On
  the 15 drafts with the shape: **cr_Vec3_LengthInline 67 -> 100%, cr_Vec3_Distance 71 -> 100%** (both saved as hand
  drafts too), fn_80048B10 83 -> 99%, cr_Vec3_RotateAboutAxis 70 -> 80%, fn_8000805C 70 -> 79%, fn_8001B6EC 52 ->
  61%, fn_80076B2C 45 -> 53%.
- Measured, not a rule: writing out every compound update through a pointer (`*p op= y`, `M2C_FIELD(...) op= y`,
  `p->f op= y`, `a[i] op= y` -> `x = x op y`) on the 134 drafts at 30-99% that have one: 5 gain (fn_80016600
  60 -> 68%, fn_8007B054 35 -> 45%), 81 lose. The written-out form is per statement (fn_80024848, fn_8007D800,
  fn_8007D3C8: the ones the original reloads), so it stays a hand fix.
- Queue 3 also takes fn_8006975C (80 -> 86%: m2c's `(x >> 31) + x >> 1` temporary is `/ 2` inline; left: the
  `base + i * 2` then `lhz 0xC` addressing, as fn_80066EEC) and fn_8004FF64 (80 -> 96%: the four stack words are
  one 16-byte local copied from the table lbl_801BCB60 (a struct assignment), the two counters an `u32[2]`, the
  score add `w[i] += hp` through the float conversion; left: the copy's loads scheduled around the game-mode test).
- Permuter queue 2 results: **fn_8001838C** (95 -> 100%: in the loop-restart branch of the THP read callback the
  frame counter is cleared before the next read size is set) and **fn_80038CF8** (97 -> 100%: score 30 from the
  permuter showed the `(s8)` cast on one float-to-byte colour store; without it on all three stores, and the 0x964 /
  0x968 tests as cast-NULL compares, the function matches). `tools/sched/scratch/hand/permdiff.py FUNC` prints what
  the permuter changed as a statement diff (it re-prints the whole file, so whitespace and parentheses are dropped
  before comparing).
- **The file run 0x80080A94-0x80081430 is two functions short**: **fn_800812A4** (58 -> 100%: the seven-object
  setup loop with only the index and the object as locals, m2c's temporaries took three more saved registers; the
  index compared unsigned); fn_80080E24 78 -> 88% (the last store is `lbl_8044EAE0[o->x10 - 2] = 0`, m2c invented a
  `saved_reg_r13` local that also moved the conversion slot; left: the float multiply's operand registers) and
  fn_80080C7C (83%) are first in queue 3 with double time. The `li r4, 0` / `addi r4, r4, -22754` lines hd.py shows
  at that store are the SDA21 relocation mask (it covers the register field; the score masks both sides, and
  promote_drafts.py's SDA21-on-addi exception links it).
- **fn_8007DA80** (67 -> 100%): the frame count 180 is a local (r31) converted at run time in the fade step (m2c
  folded it into the constant), the alpha stores unsigned (255, 200). With it the run 0x8007D0D4-0x8008052C (18
  functions) has 7 left: fn_8007DD60 39% (a table base kept in r31, three counters zeroed at the top, a goto loop),
  fn_8007E400 48%, the family fn_8007EA04 / EEB0 / F35C / F808 (56-57%: one more saved register and float local
  than the drafts), fn_8007FCD8 5%.
- Queue 2 finished 20:51: **fn_8000E3B8** 94 -> 100% (the 0x400000 mode flag set after the team flags, next to the
  0x01000000 one; checked by `draft_batch.py --only`, in tools/hand_drafts, not yet in a full batch 12);
  fn_8006EEF8's run did not start (a one-off failure, it runs by hand; last in queue 3). HSD near misses, best
  permuter scores (12 minutes each, none 0): mkBillBoardMtx 10, HSD_ShadowSetViewingRect 20, HSD_TExpSetReg 25,
  drawShapeAnim 1075 (base 1570), TExp2TevDesc 1965, HSD_JObjDispAll 9325, PObjLoad 9690, HSD_LObjSetupInit 37985;
  the outputs are in tools/sched/permuter/<func>/ (untracked); the three below 30 are worth a `permdiff.py` read.
  Queue 3 started 20:52 (30 functions, about 5.5 h, log tools/sched/scratch/hand/perm/queue3.log).
- Queue 3 finished Nov 4 02:01 (30 functions, 10 minutes each, 20 for the first two). **Score 0 for three**, each
  checked with hd.py (100%) and relcheck.py (relocations equal) and saved to tools/hand_drafts: **fn_800699E0**
  (the offset-4 sum computed before the offset-0 one), **fn_8000AFBC** (`temp_r31_14++; temp_r31_14--;` at the
  head of the `lbl_8044F377 == 0` branch: a dead use that keeps the original's register allocation),
  **fn_800677C4** (`t = t;` inside the first scan loop). Best scores of the rest: fn_8001E488 10,
  cr_Archive_GetEntrySize 15, cr_SEQ_TITLE_Leave 15, cr_SEQ_STAGE_SELECTOR_Leave 30, fn_80067778 35, fn_80080E24 40,
  fn_8001EA48 40, fn_8003B1BC 55, fn_80044A90 60, cr_Text_GetBodyName 65, cr_Save_ApplyUnlocks 75,
  cr_Text_GetPartDescription 75, fn_800389C8 80, fn_800192C4 100, fn_80066EEC 115, fn_800506DC 120,
  fn_80080C7C 175, fn_8004FF64 305, fn_80023CBC 320, cr_Vec3_RotateY 590, cr_CAMMODE_SNR_AUTO_Y_ANG_Frame 600,
  fn_80070130 615, fn_80075950 875, fn_8006975C 920, cr_Vec3_RotateAboutAxis 4860, fn_8006EEF8 5300;
  fn_8003FB00 wrote no output (its setup is to be checked). The outputs are in tools/sched/permuter/<func>/
  (untracked); `permdiff.py FUNC` shows each change.
- **Nov 4, 2026: the queue 3 near misses by hand** (owner order: apply the score-0 results, full run, then these).
  Four more at 100% (tools/hand_drafts, hd.py + relcheck.py): **fn_8001E488** (83%: the three differences as
  initialised declarations in x, y, z order; m2c wrote them z, y, x as assignments), **cr_Archive_GetEntrySize**
  (78%: typed, `&a->entries[entry]` and `cr_g_FileDescs[archive][entry].size`; m2c scaled the packed index twice),
  **cr_SEQ_TITLE_Leave** (97%: the clear colour as a literal initialiser `GXColor c = {0, 0, 0, 255}`, the pooled
  .sdata2 bytes lbl_8045025C..F; copied from the label, the scheduler pulls the four byte loads into the
  prologue), **fn_80080E24** (88%: the clamped step `*p + a * K < lim ? *p += a * K : *p = lim` through two static
  inlines, the compare and the add; the permuter's score 40 showed the inline compare). relcheck.py now keeps a
  literal's addend (`@715+0x1`), so a pooled GXColor that dtk labels byte by byte maps one to one.
  The file run 0x80080A94-0x80081430 is one function short again (fn_80080C7C 83%: the original keeps r29 saved and
  never uses it, and tests 0x80 directly where the draft has a flag; no dead-use position tried gives r29 to a dead
  local). Not closed by hand:
  cr_SEQ_STAGE_SELECTOR_Leave is 100% once lbl_8044F848 (.sbss, four byte labels 0x8044F848..B) is a `GXColor`
  object (the saved clear colour; any 4-byte type works): a data-typing change (merge the four labels in the
  config symbols.txt, type it in symbols/bss_untyped.txt / lbl_types.txt), not done yet. fn_80067778,
  cr_Text_GetBodyName and cr_Text_GetPartDescription are one shape (a message table: `u16` count at +8, `u32`
  offsets from +0x10, the text at base + 8 + offset): the original adds the index to the base and loads 0x10 from
  it, our compiler folds 0x10 into the index (`lwzx`) for every pointer, array, struct-member and cast spelling
  tried; only a named temporary (a saved register here) or the permuter's chain of 64-bit identity masks keeps the
  add; best fn_80067778 89% (wip/), cr_Text_GetBodyName 77%. fn_8001EA48 (89%) is volatile float register
  colouring only.
- Nov 4, the full batch 12 with the four: 880 drafts at 100%, main.dol = retail SHA-1. Promoted: fn_8001E488.
  Held back, and fixed in the tools: **cr_Archive_GetEntrySize** (the promoted file declared cr_PackedEntry
  incomplete; indexing a pointer to it is MWCC's "illegal type", not "incomplete struct", so promote_drafts.py did
  not retry with the structs defined: it now retries on both, promoted). **fn_80080E24** stays a 100% draft: the
  first real use of the SDA21-on-addi exception (`lbl_8044EAE0[o->x10 - 2] = 0`: `addi r4, r4, sym - 2 -
  _SDA_BASE_` then `stbx r0, r4, r13`). MWCC's object has R_PPC_EMB_SDA21 on that addi and mwld keeps rA, so the
  linked bytes are main.dol's, but dtk's split object has no relocation there and objdiff scores the unit 99.05%
  (batch 9's check_c_objects stops). Adding the relocation to dtk (config.yml add_relocations, type sda21, target
  lbl_8044EAE0 - 2) does not work: an @sda21 in dtk's asm leaves rA to the linker, which writes r13 (the SHA-1 broke
  at the 7 sites of fn_80080C7C). Kept from the attempt: tools/reloc_audit.py counts these sites (SDAIDX, 8 in
  main.dol: 7 in fn_80080C7C, 1 in fn_80080E24; relocs_report.txt), not as misses. **Then solved on the objdiff
  side**: check_c_objects.sdaidx_proof accepts a CR_MATCHING unit whose only differences from dtk's object are such
  sites (every other word equal or relocated alike; at each site dtk's word is ours with the immediate sym + addend
  - _SDA_BASE_), and the progress report names the sites; gen_dtk_splits.game_file_data takes the same sites.
  fn_80080E24 links from C as a file object (with its .sdata2 piece): 1,141 C objects, 29.56% code, retail SHA-1,
  batch 11 PASS. The file 0x80080A94-0x800811E8 waits only for fn_80080C7C now.
  Held back by data placement: **cr_SEQ_TITLE_Leave** (its GXColor pool, 4 bytes at 0x8045025C, is 4-aligned in
  main.dol, our object's .sdata2 is 8-aligned: it links only in a file object with the function whose pool ends
  there), like fn_8000AFBC.
- Nov 4: **cr_SEQ_STAGE_SELECTOR_Leave and lbl_8044F848** (the saved clear colour, .sbss 0x8044F848, written here
  and read back by cr_SEQ_CUSTOMIZE_Leave): the draft is 100% only when the object is declared with 4-byte
  alignment (any 4-byte type: GXColor is u8-aligned, so the original's variable is a word or a struct holding the
  colour). Casts in the draft do not do it (u32 casts, unions, an aligned wrapper: 78-84%); MWCC takes the alignment
  from the declaration. dtk labels the four bytes on their own (lbl_8044F848..B, the last 5 bytes long), and
  tools/gen_dtk_symbols.py only renames or cuts dtk symbols, never merges them (its rule: dtk's analysis and the
  linked main.dol cannot change). Needs a decision: a merge rule (symbols/data_merges.txt, the code that proves the
  4-byte access), then the type in symbols/bss_untyped.txt / the data unit lbl_8044F818_lbl_8044F87C.
- Permuter queue 4 (Nov 4, tools/sched/scratch/hand/perm/run_queue4.sh, bases in perm/q4/, gitignored; log
  perm/queue4.log; 20 minutes each, about 3.7 h): fn_80080C7C (double time), fn_80044A90 (96% base), fn_80067778
  (89% base), cr_Text_GetBodyName, cr_Text_GetPartDescription, fn_8001EA48, fn_8003B1BC, fn_800389C8,
  fn_800192C4, fn_80023CBC. Queue 3's fn_8003FB00 "best: []" is not a setup failure: nothing beat its base score.
- **Nov 4: where the game code's bytes are** (tools/sched/scratch/hand/structscore.py, on the full batch 12 drafts).
  draft_batch's score is exact words aligned by difflib; masking the register fields too (noreg) or keeping only the
  opcodes (ops) shows how much is already the right code. Of the 1.23 MB of game code: exact >= 90% 18.5%, >= 80%
  22.4%; noreg >= 90% 23.8%, >= 80% 34.0%; ops >= 90% 30.4%, >= 80% 45.2%. 334 unmatched functions (243 KB, 20%
  of the game code) have the instruction sequence >= 85% right, 214 (136 KB) are >= 85% with only the registers
  ignored: register allocation, not structure (fn_8000E64C 3 KB: exact 9%, ops 94%; cr_SEQ_BATTLE_FrameA 5 KB: 30% /
  95%; fn_800ECA28 8 KB: 50% / 93%). The near-miss pool for register work (the permuter, rules that remove m2c's
  extra temporaries as in fn_800812A4) is 2-4x the 65 KB the exact 80-99% band shows. The structure work is in the
  25% of bytes below ops 60%.
- Queue 4 finished Nov 4 16:58 (about 34 iterations a second over 8 workers, as queue 3): no score 0. Best:
  fn_80067778 35, fn_8003B1BC 45, fn_80044A90 50, cr_Text_GetBodyName 50, cr_Text_GetPartDescription 65,
  fn_8001EA48 40, fn_800389C8 80, fn_800192C4 85, fn_80023CBC 320; fn_80080C7C did not run ("Syntax error in
  base.c" from the permuter's parser; the setup itself succeeds). Applied by hand: fn_8003B1BC 87 -> 92% (the
  robo pointer computed before the xor; left: r25 / r26 swapped; wip/). The other outputs are identity-mask noise
  (`& 0xFFFF` on a u16, `0, x` comma forms).
- **Nov 4, evening: the permuter on Linux and a faster one on Windows.** tools/sched/permuter_setup.py: no cygpath in
  compile.sh, temp files in decomp/build/permtmp, the context pruned to the declarations the function names (about a
  third of the lines; kept only when it compiles to the same objdump), and on Linux the system cpp, the build's
  binutils, wibo in compile.sh and a process-group kill at the timeout. Two minutes on fn_8001EA48, 8 workers: 3,109
  tries (Windows, before), 10,148 (Windows, after; with the Defender exclusions of docs/INSTALL.md "Speed"), 24,075
  (cr-linux); the same base score (40) on both systems. The Linux permuter needs tools/downloads/decomp-permuter_dl
  (simonlindholm/decomp-permuter @ 8556c81d80d1c1af98a858c8f4dc951357f29139) and tools/downloads/permuter_venv
  (Levenshtein 0.27.5, RapidFuzz 3.14.6, toml 0.10.2), set up by hand as on Windows.
- **fn_800FEFF8** (99 -> 100%, tools/hand_drafts): the player index loaded from arg0->x10 as a u8 local (m2c: an s32
  temporary cast to u8 at every use; the original keeps the truncated byte in r31). As a rule,
  **narrow_cast_locals** (draft_batch.py, a variant: a local whose every use is the same narrowing cast takes that
  type): it changes 45 unmatched drafts; in a `--only` run 8 improve (fn_8007A230 70 -> 82%, fn_8011A8FC 87 -> 90%,
  fn_8011A37C 88 -> 91%, fn_80119C8C 84 -> 87%), none reaches 100%; fn_800FEFF8 itself is not one of them (it also
  passes the value uncast to its search helper).
- Permuter queue 5 (tools/sched/scratch/hand/perm/run_queue5.sh, portable, for cr-linux): the 14 largest unmatched
  drafts at noreg >= 90% plus fn_8000E64C and cr_SEQ_BATTLE_FrameA, 20 minutes each.
- Queue 5 finished Nov 5 04:31 (cr-linux): no new match. Best scores: fn_800B1B2C 210, fn_80101284 390, fn_801019D8 500, fn_80089550 515, fn_801314D4 720, fn_800381D0 880, fn_8008538C 1295, fn_800177F0 1630, fn_80039FAC 1635, fn_80090298 1940, fn_801166E4 2485, fn_80045FEC 4685, fn_8000E64C 9710, fn_8003C4F0 13695, cr_SEQ_BATTLE_FrameA 14595. fn_800B7D8C reached 0 (a `(long long)` byte offset), but its hand draft was already at 100% in tools/hand_drafts. The register-allocation pool past the first few needs more than 20 minutes per function.
- **Nov 5, 2026: the part chain's small functions (main session).** Six more hand drafts at 100% in tools/hand_drafts
  (hd.py 100%, relcheck.py equal): the matrix stack steps fn_8001D9E8 (scale), fn_8001DA70 (scale x / y / z),
  fn_8001DD68 (translate) and fn_8001DF6C (load), fn_80101950 (the battle-start forms), cr_BattleRobo_InitFromMenu.
  What they needed: one local pointer to cr_g_CurrentMtx (m2c re-read the global, 8-34%); the translate's three rows
  interleaved step by step, sums in locals (a sum per row written whole gives 44%, row by row 76%); the load's last copy
  as `*(m += 11) = *(v += 11)` (the original's lfsu / stfsu pair; the moved pointer is the return value); the robo
  pointer set before the index (fn_80101950); in InitFromMenu the 19 words of the model slot copied as struct array
  fields (`robo->a864[i] = slot->a8[i]`: (i << 2) + 0x864 and stwx, m2c had a byte store there).
- **fn_80095E6C 60% -> 96% (wip/, not linked)**: the game's own HSD_JObjDispAll with a specular light pass. The source
  shape: MWCC inlined the function into itself one level (written out as an inline copy `Disp_inl` of the body that
  calls the real functions fn_80096374 / fn_80095D7C / fn_80095E6C; the top level uses inline copies of those two
  helpers); the instance-matrix setup of the top level written flat (its two dirty flags are locals of the function:
  r18 / r17; as locals of an inline they go to the stack); **the child loops reuse the joint parameter**
  (`for (jobj = jobj->child; jobj != NULL; jobj = jobj->next)`, the original's `lwz r31,0x10(r31)`; a separate
  `child` variable took two more saved registers and pushed the dirty flags to the stack: 72 -> 87%); the NULL tests
  of both copies against `(void *) 0` (li r0,0; cmplw); the camera pointer assigned in the condition (no reload);
  and, from a Windows permuter run (score 0 in 20 minutes), one of the two light-flag clears as
  `lobj->flags = lobj->flags & ~0x100` (a register swap). Left: only stack slot offsets (the view-matrix getter's
  parameter and return slots and the light counts): the permuter's score ignores stack offsets, so its 0 is not a
  match. MWCC level 0 rule seen here and in cr_Chara_BuildModel: when the 15 saved registers r17-r31 run out, the
  locals that lose are kept on the stack, and the slot order follows the inline nesting (return slots of the inlines
  called from the function first, their insides after).
- **cr_Chara_BuildModel 47% -> 100% (tools/hand_drafts)**: written from the asm with inline helpers: child / next
  getters with `(void *) 0` tests (their return slots are m2c's sp40 / sp3C ...), the material lookup as an inline
  over `cr_DObjGetMObj(jobj->u.dobj)` (one inline level: two gave one stack copy too many), the role-joint walk with
  the 0xFF exit and the `path++` inside the loop, `(u32) number` compares (cmplwi), and **the extra-joint path walk as
  an inline that takes the path pointer by address**: `cr_WalkPath(root, &path2)`. MWCC folds `*pp` back to the
  caller's register variable (r31 stays the path; no stack home), so the walk advances it in place, and the walk's
  result and return slot get the original's registers and stack copies (86% written in the function, 42% as an inline
  taking the path by value: MWCC copies a parameter the inline modifies). Last fix: the header address, then path2,
  then has_extra_paths = 1 (other orders swap r3 / r4). The helpers are under `#ifndef CR_CHARA_PATH_INLINES` so the
  file's other function (cr_WeaponModel_OnLoaded, the same walks) can share them in a file object.
- **cr_WeaponModel_OnLoaded 48% -> 88% (wip/)**: the same walk and material inlines; the relocation loop; `(x &
  0xFFFF0000) >> 16` for the model id (the peephole folds the mask into the srwi and leaves the original's dead
  `clrrwi`); `addis r0,r3,1; cmplwi r0,0xFFFF` is `(u32) x != 0xFFFFFFFF`; **the stack homes of the locals that lose
  the register race are given in declaration order** (relocs, p, kind: first declared, highest slot). Left: two
  register swaps (path / header, out / slots) that declaration order and reference counts did not move.
- **fn_80039FAC 87% -> 92% (wip/)**: the 13 role / part matrix stores as one inline `cr_JObjSetMtx_9FAC(jobj, mtx)`
  (HSD's matrix copy with this build's assert lines 0x495 / 0x496, `flags |= 0x03800000`, then the dirty marking); each
  call's joint parameter is a register of its own (r31 down to r23) and the dirty results r19-r17 then stack slots, as
  in the original. Left: the scratch register of a few loads (r0 against r3 / r4).
- **fn_801076D8 (32% -> 44%): blocked by its data pool.** r27 is `lbl_80238008`, and the sound table (+0x38C8, read as
  `addi r3,r27,0x38C8; lwz r3,0x28(r3)`, index 0 folded), the state handlers (+0x3C20) and the input handlers
  (+0x3C30) are reached from it: other objects of the same file addressed from the file's data-pool base (the step 86
  rule). No C form against extern declarations keeps that `addi` (struct members, nested structs, byte arithmetic,
  local copies and inline parameters all fold, tested in small files); it can match only in a file object that defines
  the file's data.
- cr_BattleRobo_InitFromLoadout (~50%, wip/) has the same block: r30 is lbl_801C50D0 (a 232-byte object), and the
  tables it copies from (+0xB00, +0xB40, +0x1B74, +0x445C, +0xF790, +0x144C0) are reached as r30 + offset (the
  "unfolded table pointers" of Nov 3): the file's data-pool base again, so it waits for its file object too.
- Permuter on Windows: a draft for permuter_setup.py must be the wrapped form (`#define F F__proto`,
  `#include "ctx.h"`, `#undef F`, then the body; tools/sched/scratch/hand/perm/), else base.c has no context and the
  permuter stops with "Syntax error in base.c".
- **The model / stage file 0x800730F8-0x800751xx, Nov 5 (main session).** With the path inlines of cr_Chara_BuildModel:
  **cr_ModelEntry_LoadAndBuild 79 -> 100%** (the relocation loop and the role loop share ONE counter: r26 in both, one
  variable at level 0; signed for the `< 19` loop; the relocation table pointer a local declared before the entry
  pointer; the model's part list is one pointer level deeper than the weapon file's), **fn_80074FBC 77 -> 100%** (the
  file layout from its header: each size computed before its pointer is stored; a zero local for the +0x40 store),
  **fn_80073728 65 -> 100%** (the stage loader: it takes the archive as its SECOND argument, which m2c could not see;
  the counter s32; the model count passed on as fn_8006E83C's u8 second argument; the light count stored as `i - 1`
  before `i = 0`, a variable for it takes a saved register). Signatures read from the callers' code went into
  symbols/cr_types.h (fn_80073728, fn_8006E83C, fn_800B54E8; a context refresh with `--sample 1` compiles once the stale
  ctx_protos.h lines are gone, which a full run rewrites; m2c then drafts the other callers, cr_SEQ_SCENARIO_Enter and
  fn_800A0228, with the second argument). Near misses in wip/: fn_80073A74 60 -> 93% (registers of three locals left;
  `(u8 *) ofs + (u32) objs` gives the original's add operand order), fn_800735C8 29 -> 87% (two header variables, the
  copy `h = hdr` is the original's r30; the parameter's register is the one left), cr_MotionFile_Register 58 -> 71%
  (its record fields are reached as add + displacement, `(base + j * 12)` then +4, a form that every expression spelling
  tested folds into addi + lwzx; only a local pointer or an inline's return value keeps it, and both change the
  registers), cr_WeaponModel_OnLoaded 88% (a 20-minute permuter run found nothing better).
- Same file, Nov 5: **fn_80075950 86 -> 100%** (the five part-file queues; every buffer address in the first loop's
  form `(void *) (buf + ((s32) (i * N * 4) / 4))`; the slot table is a [5][4] array), **fn_80075D14 70 -> 100%** (the
  sound banks; the table words as `*(tbl + i + k)`, which gives i * 4 + 4k where `tbl[i + k]` gives (i + k) * 4; two
  local arrays, the first declared gets the higher slot). **fn_8007571C (17%) is pool-blocked** like fn_801076D8:
  r31 = lbl_80377E60 is the base of the file's data, and the slot table (+0), the address tables (+0x50 ...) and the
  archives (+0x90 ... +0x3C0) are reached as `addi rX, r31, off` (fn_80075950 touches only the slot table and
  addresses it directly). Pool-blocked so far: fn_801076D8, cr_BattleRobo_InitFromLoadout, fn_8007571C.
- **The epilogue order `lwz r0` before `lwz r31` (fn_800F1860 90 -> 100%)**: the final scheduling pass reschedules a
  block only when an earlier pass inserted or deleted an instruction in it (docs/sched_model.md); fn_800F1860 saves
  r31 without using it, the trace of a local the peephole folded away: `p = (s32 *) lbl_8044F304; fn_80023354(p,
  ...)` gives the saved r31, the deleted `mr r3,r31`, the rescheduled epilogue and 100% (a dead `x = 1; ... x = x;`
  does too). fn_800A0804 has the same epilogue with r31 in use (its parameter): no deleted instruction found for
  its last block yet (local pointer, dead locals, call result kept, mask spellings `~(1 << 10)`, `(u32) -0x401`,
  a mask local: 52-95%). Among the unmatched drafts at 85-99%, fn_800A0804 is the only one whose difference is
  only the epilogue.
- **Nov 5, more near misses (main session), all hand drafts at 100%:** fn_800A0C20 86 (the relocation loop as
  `p = (u8 **) (base + relocs[i++])`), fn_8008E7EC 92 (stores in field order), fn_8007A230 82 (the divisor 10 is a
  local: `li r30,10; divw`), fn_800EEC1C 89 (the markers are 0xFFFF, not -1; `addis r0,r4,0; cmplwi r0,0xFFFF` is a
  signed int against 65535), fn_80096C28 / fn_80096D28 81 (a channel local `ch = 0xFF` / `ch = 0` passed once: the
  peephole folds it into the `li r3` but it keeps its saved register r28, which main.dol saves and never uses; and
  `r1 = 1; if (r1 == 1)` as written).
- **The "folded local" rule for saved registers that are never used.** At level 0 every local that is assigned gets a
  saved register; when its only use is a copy into an argument register, the peephole removes the copy and the
  register stays saved but unused, and the deletion also makes the final pass reschedule that block (the epilogue
  order of fn_800F1860). A scan of the original asm finds **229 unmatched functions whose prologue saves a register
  their body never uses** (the "gap register" of the script handlers and others), e.g. fn_800FBFD4 / fn_800FBAB4 98%
  (r28), fn_800B1B2C 94% (r28), fn_80118C08 92% (r26). For each, the candidate is a local copied once into a call
  argument, declared where its register falls. Tried so far: fn_800943D8 (85 -> 89%, the local took the lowest
  register instead of r28), cr_EV_CLR_SQARE (89 -> 91%, the search inline takes the id as an s32 parameter, the
  original's r26; the PC and result registers stay swapped). Next: a generator variant that tries one such local per
  call argument.
- Nov 5, three more at 100%: fn_800FBAB4 98 (the unused local from +0x14 is a u8: clrlwi, not extsb), fn_800FBFD4 98
  (the player index kept as a u8 local: clrlwi once, not a cast at every use), fn_800FB688 92 (**four u16 constants
  copied into a local array are an initializer with literals**: `u16 v[4] = {36, 37, 38, 39}`, read from main.dol at
  0x804516F8; the compiler pools them and copies them with lhz / sth; assignments from extern consts add clrlwi; a
  generator rule could turn m2c's `spN = lbl_...;` runs from .sdata2 into such initializers).
- Nov 5, **the dead-use rule of the script handlers works outside them**: one `if (0) { x = x; }` with a local
  that should rank higher (a dead use counts toward its priority, adds no code) fixed three register-order near misses:
  cr_EV_CLR_SQARE 91 (the PC copy), fn_80073A74 93 (the record pointer `rec`), fn_8003B1BC 92 (var_r26). Two dead
  uses overshoot in all three; one is the usual answer. It does not help the folded-local misses (fn_800730F8 97:
  its spare register is the folded `u8 s`). Swept by a scratch script over the wip near misses and the m2c drafts at
  80% or more (one or two dead uses of each local of the main function).
- Nov 5, fn_80101284 88 -> 96: a zero-initialized local array (`u8 v[4] = {0, 0, 0, 0}`, the m2c `sp8..spB` from
  .sbss2 at 0x80452A84, outside main.dol's sections, so zero), the switch with its real case set (0 and 3-5 share,
  no default: the original compares 2, 0, then the 6 bound) and the pointer test against 0 (`cmplwi`, not a
  `(void *) 0` compare). Left: one volatile register (r4 / r5) and the f-register order of the three sums.
- Nov 5, cr_MotionFile_Register stays at 71%: the original reaches the 12-byte records as `add rX,base,j*12` and then
  a displacement (4 / 8 / 12); every plain C form tried (struct array, `rec[j].b`, 2-D array, byte offsets, an
  integer base) folds the field offset into the index (`lwzx`), and an inline getter gives the add but spills its
  temporaries.
- Nov 5, **a compare with no branch after it is an assert without its message call**: `(c) ? (void) 0 : (void) 0`
  (fn_80092A5C 87 -> 100 with it, a folded local `id` for the spare r29 and `return var_r30`). An empty `if`, an
  `if / else` with empty bodies and a bare `c;` all drop the compare. A scan of the original asm finds 31 unmatched
  functions with such a compare (fn_80091F84 87 -> 92 with `(lbl_8044F690 < 0x154000) ? (void) 0 : (void) 0`).
  Also fn_800192C4 88 -> 100: one dead use and the pairs of lbl_801E13F4 as a struct array.
- Nov 6, two more at 100%: fn_801019D8 93 (a local `ret = 10` set at the top for the **last** return only: the
  original keeps it in a saved register and returns it with `mr r3,r24`, while the earlier `return 10`s stay
  literals; plus a `for` loop for m2c's `goto loop` (test at the bottom)), cr_CHAR_POS_GET 87 (the shape of
  cr_CHAR_COUSE_CLEAR_PS: the chara pointer set to NULL before anything else, the id read into a local before the
  search loop, 2 dead uses of the PC copy). cr_CHAR_TURN_POINT stays at 98% (the id as an s32 inline parameter;
  left: the saved current-chara copy one register too low, with the unused saved register on the wrong side).
- Nov 6, **the script-handler head rule does not catch m2c's `cmd_cnt` pointer head**: 20 drafts start with
  `cmd_cnt = &cr_g_Script.cmd_count[cr_g_ScriptDepth]; ... (*cmd_cnt)++;` in m2c's order. Rewritten to the head of the
  matched handlers (history store, index += 1, &= 0x3F, `cmd_count[depth] += 1`, the PC step, then
  `if (0) { (*cmd_cnt)++; }`; any other head line, e.g. the saved current chara, tried in each place), six reach 100%:
  cr_CHAR_MOVE_REL 89 (plus dead uses), cr_CM_SET 80, cr_CM_ZOOM_XYZ 76, cr_LGT_MOVE 84, cr_SYS_SAVE_TABLE 44 and
  cr_S_BGM_CROSS 85 (plus fn_80097670 called with a u8 third parameter: the generated prototype's s8 adds an extsb).
  The other 13 gain 2-13 points (cr_CHAR_SET_POS_DIR 86, cr_CHAR_SET_WMOT_Y 85, ...). **Cause, fixed in the
  generator**: script_head_dead (step 110) only took a decimal PC step (`cr_g_ScriptPC += \d+;`); these 20 step by a
  hex amount (`+= 0xA`, `0xC`, ...), so the rule skipped them and the step 80 pointer form won. With hex accepted
  (`draft_batch.py --only` on the 20, hand drafts moved aside): cr_CM_SET, cr_CM_ZOOM_XYZ, cr_LGT_MOVE and
  **cr_SYS_GRID_SIZE** (no hand draft) at 100%, cr_CHAR_MOVE_REL 99, cr_S_BGM_CROSS 96, cr_CHAR_SET 73 -> 85,
  cr_CHAR_SET_POS_DIR 76 -> 89, cr_CHAR_SET_WMOT 75 -> 86, cr_CHAR_SET_WMOT_Y 76 -> 87, the camera handlers +7 to +13.
- Nov 6, two more by the compiled-out macros: fn_80091F84 87 (the 256 x 8-word table cleared as
  `((u32 (*)[8]) lbl_8044F694)[i][j] = 0`: index first, then the base, where byte offsets load the base first; and the
  bare compare is an assert **after** the last store in the source, which the compiler schedules before it) and
  fn_8008C664 78 (an assert on the slot counter before the call, `(void *) 0` compares, and **an empty then-branch
  that holds `(void) 0;`**: a compiled-out macro keeps the original's `bne else; b end`, a truly empty branch drops
  the jump). Generator: shift_mask_order and the struct-copy bias also took decimal constants only (m2c writes 10 and
  up in hex); both take hex now (no score change on the 7 drafts with a hex shift). After a batch 12 run the
  generated context declares fn_80073728 / fn_8006E83C / fn_800B54E8 again with the pass-1 types next to
  symbols/cr_types.h: delete those three lines from decomp/drafts/ctx_protos.h, ctx_pp.c and ctx_pp_noro.c before a
  permuter run (until the next full run picks up cr_types.h).
- Nov 6, fn_800F7914 98 -> 100: **m2c's parameter aliases** (`var_r29 = arg1; var_r28 = arg0;` at the top) spill the
  parameters to their stack homes; the original uses the parameters themselves, in r28 / r29, with 1 dead use of
  arg0 in an `if (0)` block for its rank. A sweep of the 9 unmatched drafts at 50% or more with such an alias: no
  other match (fn_800EA830 66 -> 76, fn_800EAA38 63 -> 73). fn_8001C5AC (97%) needs a context change: its file
  declared lbl_8044EEB0 without a size (`extern f32 lbl_8044EEB0[];`: `lis` / `@l`), our context declares it [2]
  (sda21); only 3 small-data symbols other than __float_nan are read through `@ha` in the original
  (lbl_8044EEB0, lbl_8044F7C0, lbl_8044ECAD).
- Nov 6, cr_CHAR_SET_POS_DIR 76 -> 100: the generator's draft with the hex-step head (89%) plus **4** dead uses of the
  PC copy in the head's `if (0)` block. 1 to 3 change nothing, so a greedy search that needs a gain at each step
  never gets there: try counts up to 6-8 per local. A 3-6 sweep over the 45 unmatched drafts at 85% or more found
  no other match (fn_80044A90, fn_800F9014 +2-4). Also tried and left: declaration order for the fn_800E9C90 family
  (5 copies, 73 -> 75% at best over all 720 orders), pairwise dead uses for cr_CHAR_TURN_POINT 98 and
  cr_SYS_OVERLAY_WAIT 96.
- Nov 6, **the adjacent-swap sweep** (scratch script: swap each pair of neighbouring one-line statements at the same
  indent, keep the best, 2 rounds; the statement order m2c reads back is often one step off where only volatile
  registers or the scheduling differ). On the 45 unmatched drafts at 85% or more: fn_800B1944 95, fn_8010E47C 92,
  cr_Vec3_RotateY 87 to 100% outright; with one more fix each: fn_80100484 95 (the robo record pointer before the
  joint pointer), fn_80100350 85 (the unused byte local u8, not s8), fn_800506DC 86 (bit loops as
  `for (m = mask; m != 0; m ^= 0x80000000 >> i) { i = __cntlzw(m); ... }`: the clear after the body). A 4-round run
  over the 170 drafts at 70% or more is next. **Check relocations after every sweep match** (hd.py scores words only).
- Nov 6, more single fixes: cr_Text_MaskUnownedSpecialPart 94 (`(arg1 - 0x1E) + count`: the compiler adds first and
  subtracts last; m2c's `(arg1 + count) - 0x1E` folds the constant into the count; and the plain sum body + gun +
  bomb + pod + leg for the load order), cr_Records_BuildBodyRows 94 (`tbl[i++]` inside the read, the row pointer
  stepped before the counter), fn_800DF320 93 (`(void *) 0` test, x negated before y copied), fn_800DF018 93 and
  fn_80086F54 92 (the drafts still called **fn_8001B3F0**, cr_Rand_Int's old name, with an s32 prototype: srawi
  instead of rlwinm; the only stale name in the drafts; and fn_80086F54's 12 bytes from lbl_801C0C8C are a local
  initializer `Vec v = {0.0f, 1.0f, 0.0f};`, which relcheck maps to the original's symbol).
- Nov 6, fn_801010A8 87 (the identity matrix local is a whole `Mtx`: m2c's `f32 spC[4]` left the frame 32 bytes
  short; the current robo pointer set before the index, as in fn_80101950) and fn_800FA9CC 90 (the switch reads the
  record byte itself: m2c's temp took a saved register the original does not have; then the swap sweep). Open
  idiom, 27 functions: the store pair `obj->x30->x5C[0] = lbl_8044ED08[0]; ...[1] = lbl_8044ED08[1];` at the end of
  the lbl_8041A648 UI functions (fn_80118468 89, fn_80118C08 92, fn_8011936C 87, fn_80119C8C 92, fn_8011A37C 91,
  fn_8011A8FC 90, fn_80121098 93, fn_8012180C 92) comes out with every register one lower; in permuter queue 7.
- Nov 6, three more: fn_800EAC48 93 (the last block's stores to cr_g_SeqCurrent in another order: both unk_38 updates
  before `unk_40 = 1`; found by trying every order of the 5 statements: a block-permutation sweep over the drafts
  at 80% or more is running), fn_80100C30 89 (m2c folded `0x80000000 >> var_r31` with var_r31 = 0 to `>> 0`; the
  loadouts compared as records `cr_g_BattleLoadouts[i].unk_0`; one store through an s8 field) and **fn_8008CFD0 87:
  the float parameters come first, `(f32, f32, f32, f32, u8)`**. m2c always lists the integer parameters first; the
  registers are the same either way (r3, f1-f4) but the stack homes follow the declaration order. Swept every
  interleaving for the 38 unmatched drafts with both kinds of parameters: no other match.
- Nov 6, five more: fn_80092138 87 (each call result stored straight into its global: **m2c's `temp_r0_NN` /
  `temp_r3_NN` locals are named after volatile registers but become saved-register locals when compiled**),
  fn_800BCD80 87 (an area bound read as `s32` and converted: it is an f32; no `(u8)` on slot_index), fn_801177CC 89
  (lbl_8041C828[1] is a signed byte: an s8 view of the context's u8[20]), fn_800BD658 69 (**m2c put a scheduled
  `i += 1` before a store that uses the old index** and wrote that store with i + 1; the loop body in source order
  matches) and fn_8003764C 78 (statement order inside the switch cases: adjacent swaps + block permutations).
  Sweeps now chained per function (scratch pipeline.sh: volatile temps -> adjacent swaps, 4 rounds -> block
  permutations of 3-5 statement runs), running over the 278 unmatched drafts at 60-95% under 2000 bytes.
- Nov 6, cr_CHAR_TURN_POINT 98 -> 100 from **permuter queue 7** (cr-linux, perm/q7: the 13 parked hand near misses,
  20 minutes each): the dead-code counter pointer `cmd_cnt` declared after the two chara pointers. Its place among the
  declarations decides which of the two gets r26 (the head rule's declare_cmd_cnt puts it right after the PC copy).
  fn_800B1B2C 94 -> 95: the id read into a local before the search loop; left: a folded local (r28) and a 16-byte
  larger frame.
- Nov 6, fn_8007A800 84 -> 100: m2c's `loop_N:` / `goto` nest as two `for` loops leaving through one goto when
  found (84 -> 94), the loadout field through the record `&cr_g_BattleLoadouts[i]` (m2c's byte offset
  `(i * 0x10) & 0xFFFF0` computes the address in another order), the slot table as `u8 [][4]`, no (u8) on a u8 field.
- Nov 6, fn_8008C8A8 84 -> 100: the "free every node of type N" walk over the lbl_8039649C list (0x74-byte
  nodes) is a `for` loop that breaks on a null node; m2c's phi copy (phi_r25_17) took a saved register of its own,
  so the node is one local. The scratch tool listfree.py makes this rewrite; it lifted fn_800935F8 to 97 (wip) and
  fn_8008F6E8 to 82.
- Nov 6, fn_8008F6E8 76 -> 100: also m2c's search loop `loop_N: if (i < 0x100) { if (skip) { i += 1; goto loop_N; } }`
  as `for (; i < 0x100; i += 1) { if (!(skip)) break; }` (86), and arg0 compared directly: m2c's stack copy
  `sp8 = arg0;` gave a `lwz r0,8(r1)` reload and one more saved register (100). A `spN = argN` copy in an m2c draft
  is a candidate for deletion; the sweep (scratch gotofor.py) tries both rewrites on every open draft.
- **Nov 6, fn_800A0804 95 -> 100: the epilogue order `lwz r0` before `lwz r31` solved.** A dead store to the
  parameter after the final store, `arg0 = 0;`, puts an `li` in the last block that a later pass deletes, and the
  final scheduling pass then reschedules the block (the rule in the fn_800F1860 bullet). A bare compare there
  (`(arg0 & 1) ? (void) 0 : (void) 0;`) gives the same order but keeps its compare, and a compare on the masked
  value folds into `rlwinm.`. `(arg0 & 1) && 0;` matches too. The sweep (scratch epidead.py) tries `v = 0;` for each
  int parameter and local, before the closing brace and before each return, on every open draft whose diff moves
  `lwz r0,N(r1)`.
- Nov 6, fn_8008CA14 87 -> 100: `u8 += (u8) (C / (f32) d)`: the float quotient cast straight to u8. m2c's
  `(s32)` cast schedules the int-to-float conversion differently, and `(u8) (s32)` adds a clrlwi.
- Nov 6, **fn_800B1B2C 95 -> 100: a dead local fills the unused saved register.** The original saves r28 and never
  uses it. A local copied into the call argument (scratch foldloc.py) got the register count right (98), but its
  `mr r3,r28` was not folded. What matches is `s32 x;` declared last, `x = 1;` in the else of the first block and
  `x = x;` after the last call (the fn_800F1860 form). The scratch sweep deadloc.py tries `x = 1;` before each
  statement, with `x = x;` at the end. It covers every open draft at 80% or more whose original saves a GPR its body
  never reads (the 229-function scan above). The same sweep on fn_80118C08 (wip 94) is running.
- Nov 6, **three-float copies are Vec struct copies** (the original copies words: `lwz` / `stw` pairs, where m2c's
  three f32 assignments give `lfs` / `stfs`). The scratch rewriter veccopy.py turns three consecutive copies
  `A.x/.y/.z = ...` or `M2C_FIELD(P, f32 *, o / o+4 / o+8) = ...` into one `Vec` assignment. Swept over every open
  draft it lifts the gun-mode family fn_80046A68 / fn_80046F58 / fn_8004752C / fn_80047AC0 / fn_80048050 /
  fn_80048708 (62-68 -> 69-84). Then, per member (wip, queue 8): lbl_80329B60 through a local declared first
  (the original keeps it in r30 / r31, +3-7), the three view vectors as one chained assignment
  `a6c = a78 = a60 = robo->x110` (fn_80048708 87 -> 92), the stack struct as a `Vec`, and the `(f32) fabs(x)`
  compare through a float local (the original saves one more FPR: fn_8004752C 85 -> 88). Now 75 / 86 / 88 / 84 /
  74 / 92. Still open: the original hoists the next statement's `cr_g_CurrentBattleRobo` load into the struct
  copy, and no spelling tried here does that.
- Nov 6, fn_80042AB8 (the gun updater, 5.6 KB; part-chain priority list), m2c 34%. The scratch scorer nscore.py
  ignores stack offsets, registers and the misdecoded psq_st prologue lines; it shows the structure at 79% -> 91.5%.
  Done: a second float parameter (f2 stored at entry and read later; the caller passes one float), Vec locals and
  struct copies, MSL sqrtf in source form (the generator rule missed this shape), s32 loop counters (m2c's s8
  added extsb), for loops for the shot-slot search, the flag word `((s32 *) lbl_8032C234)[0x10]` (m2c read the f32
  and converted it with fctiwz), and the history pointer temps inlined (they spilled to the stack). Left: the `% 60`
  index (writing it plainly shrinks the frame by 16 and moves every slot), then registers and stack slots. Wip.
- Nov 6, cr_WeaponModel_OnLoaded 88 -> 97 (four dead uses `path = path;`, deaduse6.py) and fn_80039FAC 92 -> 93
  (adjacent-swap sweep); both register-only, wip, queue 8.
- Nov 6, **the stack-layout method for the big part updaters** (fn_80042AB8 34 -> 55%, fn_8003FE98 38 -> 63%;
  frame and every stack slot now equal the original's). m2c's `spXX` names are the original's offsets in hex. MWCC at
  `level=0` gives the local declared first the highest address, so: (1) every stack local gets its true type
  (`M2C_UNK` / f32 triples whose address is taken are `Vec`, and m2c's `struct { ... } sp;` is flattened); (2) the
  declarations are sorted by offset, highest first, after the register locals; (3) a temp copied once into a stack
  local is that local (spC = fabs(...), not t = ...; spC = t); (4) m2c temps that spill and that the original
  lacks are removed: switch operands, fpclassify operands, `t = x - 1; x = t; if (t == 0)` as `if ((x -= 1) == 0)`,
  post-increments in compares, phi copies. Scratch tools: stackfix.py does (1)-(3); slotmap.py / slotagree.py
  map every `N(r1)` of mine to the original's and show what is left; nscore.py scores with stack offsets and registers
  normalized. A frame larger by 16 x n means n float temps too many (FPR save area); a scalar on the stack in the
  original but in a register in mine means mine has one register local too few, and the reverse. In both functions
  the local holding lbl_80329B60 only got its register (as in the original) once every reference went through it,
  except `&lbl_80329B60[0xA00]` in fn_8003FE98, which stays direct.
- Nov 6, **two call signatures m2c and ctx_protos.h have short**: fn_8003FE98 calls fn_80044A90 with four
  arguments, `(slot + 0xA8, rec + 8, 2, farg0)`: the original sets r5 = 2 and f1 = its own float parameter, and that
  parameter has a stack home only because of this call. ctx has `s32 fn_80044A90(void *, void *)`, and the draft
  calls it through a cast. Its own wip draft does not change with the two parameters added, because they are unused
  there. And fn_80042AB8 has a second float parameter (f2, stored at entry and read later) that its caller does
  not pass. A fix for symbols/cr_types.h when the next batch runs.
- Nov 6, **an indexed byte read `lbzx rD, base, idx` with `idx = i * stride + off` is a struct-array member**:
  `((Gun *) arg0)->shot[i].b[5]` computes `mulli; addi idx, idx, off` and then `lbzx`. Every pointer spelling
  (`((s8 *) arg0)[i * stride + off]`, `*(arg0 + ...)`, M2C_FIELD, the constant first, a cast to u32) folds the
  constant into the displacement (`add; lbz off(r)`), and only a local index variable gives lbzx too, at the cost
  of a register. The tiny-function harness (scratch tiny.c through nscore.sides) showed it. The gun and bomb
  updaters take this form for their shot-slot tests (scratch slotarr.py: FUNC IN OUT BASE STRIDE; fn_80042AB8
  55 -> 56%). The same probably holds for the `lhzx` / `lwzx` reads of other drafts with a `(i * size) + const`
  index.
- Nov 6, **`spA = spB = value` (two stack slots holding one float) is an inline function's return value**; at
  `level=0` it goes through stack temps. In the pods updater fn_8003DD38, m2c's `sp88 = __fabs(v.y); t = (f32) sp88;
  sp4C = sp50 = t;` is MSL's `fabsf(x)` (`(float) fabs((double) x)`, with fabs itself an inline returning double:
  the f64 slot), and `sp54 = sp58 = (f32) atan2(...)` is `atan2f`. With static inlines cr_fabs / cr_fabsf /
  cr_atan2f the draft goes from 69.6 to 79.3% normalized. m2c also printed the int-to-float conversion as stores
  (`spE8 = 0x43300000; spEC = (s32) x;`, the compiler's own temps, to be removed), and it cannot decompile a float
  compare kept as a value (`fcmpo; mfcr; srwi 31`), which it shows as `M2C_ERROR(mfcr) >> 31`. That is
  `(a < b)` in C; the four in fn_8003DD38 are `x*x + z*z < C * (C * (f32) (r * r))`, read from the asm.
- Nov 6, **the two m2c failures get drafts: a generator rule for switches spilled to the stack** (draft_batch.py,
  `spilled_switches`, in the asm preparation before m2c). fn_800A0E64 and fn_800A63C0 (11.7 / 15.4 KB) dispatch
  through stack slots (`stw r12, 0x13c(r1); lwz r3; lwzx; stw; lwz r0; mtctr; bctr`), and fn_800A63C0 builds its
  first table's address from plain numbers (`lis r0, 0x8023` ... `subi r0, r3, 0x4ef0`, no relocation). The rule
  takes each bctr whose window after the bound check (`cmplwi rK, N; bgt`) holds only lis / slwi / addi / subi /
  stack stw and lwz / lwzx / mtctr. It rebuilds the window as `lis r12, T@ha; slwi r11, rK, 2; addi r12, r12, T@l;
  lwzx r12, r12, r11; mtctr r12; bctr`. The table (a data object of `.4byte fn+0xOFF` entries, lbl_8022AF44 /
  lbl_8022B110) becomes `jumptable_<address>`, and the existing loop then adds the labels at its targets. A
  hand-patched copy of fn_800A63C0's asm let m2c decompile it (2,043 lines).
  With the rule, `draft_batch.py --only fn_800A63C0 fn_800A0E64` (out decomp/build/sw, 52 s): m2c ok 2, compile
  failed 0, fn_800A63C0 3% and fn_800A0E64 10%. So every one of the 1,751 game functions now has a C draft that
  compiles; the next full batch 12 puts these two in decomp/drafts.
- Nov 6, fn_8003FE98 70 -> 71: the robo-mask tables `M2C_FIELD((p + 0xA94 + (i * 4)), u32 *, 0)` are
  `((u32 *) (p + 0xA94))[i]`: the base plus the constant in one register (`addi r3, r29, 2708`), then `lwzx`
  with the scaled index. m2c's sum adds the constant to the index. Swept over the open drafts (scratch
  arrsweep.py). The saved registers of fn_8003FE98 now all correspond (scratch regmap.py); what is left is
  volatile numbering from the entry on: mine loads lbl_80329B60's address into r3 and copies it to r29, the
  original loads it straight into r29 (initializer, cast, `&a[0]`, `+ 0` tried).
- Nov 6, **cr_SYS_RET_GEKITOU 83 -> 100**. (1) m2c typed the sprintf buffer as one byte (`s8 sp8;`, passed as
  `(char *) (&sp8)`); the frame size gives it: `char sp8[0x108]` (original frame 304, m2c's 48). (2) m2c's
  sprintf had a fourth argument repeating the format; the original passes three. (3) The script head keeps the
  PC in a local (`pc = cr_g_ScriptPC;` and the history reads `*pc`: one more saved register, r26). (4) The script
  table is an array of 8-byte records `{ u32 pc; char *name; }` read as `((cr_ScrEntry *) lbl_8044F788)[i].name`,
  which gives the index sum then lwzx (the struct-array rule above), in a for loop. The scratch sweep bufsweep.py
  tries (1) and (2) on every open draft that has such a buffer or call.
- Nov 6, **cr_C_ACT_RET 86 -> 100**: the script return stack at chara + 0x1914 (cr_types.h: `u8 unk_1914[0x20]`)
  is an array member, `((cr_CharaRet *) chara)->stack[--chara->unk_1938]` (a draft typedef with the pad): the
  index sum `n * 4 + 0x1914` then lwzx off the reloaded chara pointer (the struct-array rule); m2c's temp for the
  decremented count took a saved register the original does not have. The PC copy in the head stays. For
  cr_types.h: unk_1914 is `s16 *stack[8]` (the next types round). The buffer sweep (bufsweep.py, 23 drafts)
  gave a few points here and there and no match.
- Nov 6, cr_C_ACT_CALL 83 -> 100 by the same array member, `stack[n++] = cr_g_ScriptPC` (the PC copy in the head is a
  local, `pc`, and the call target is read as `*(s16 ***) (pc + 4)`).
- Nov 6, **fn_80089550 88 -> 100**: (1) `Mtx sp10` for m2c's `f32 sp10[4]` (the frame 32 bytes short; the known Mtx
  rule). (2) Four byte copies `dst[k] = tbl[i * 4 + k]` through a pointer temp are a GXColor struct copy
  `*(GXColor *) (p + 0xE1) = ((GXColor *) lbl_80219448)[i]`. MWCC copies a 4-byte struct of byte alignment with lbz /
  stb through a compiler temp in r9, where m2c's temp took a saved register. The four bytes from lbl_8044EAE8 ..
  EB stay four copies, because they are four symbols (relcheck: a struct copy reads one symbol at +1 / +2 / +3).
  (3) `v = cr_Rand_Int() >> n; v = v % 690;`: m2c's temp for the call result took a saved register.
- Nov 6, **fn_800FA750 91 -> 100**: (1) the bit-search loop over the slot flags is an inline returning the index
  (0xFF when none); (2) before the sprintf the original compares the slot count with 9 and drops the result: a
  compiled-out assert inside a void inline taking a u8 (the clrlwi trace, as fn_8008CA14); (3) the original saves
  r25 that nothing uses: the assert inline copies its argument to a local (`u8 i = j;`), which takes one more
  saved register and changes nothing else (a copy of a stack-loaded value into a temp before its first use does
  the same). Dead locals (`x = 1; ... x = x;`) and pointer locals change the code; this does not.
- Nov 6, **fn_800FFFFC 77 -> 100**: the HUD layout loop (the sibling of fn_800FA750). (1) The two tables are
  struct arrays: the 0x18-byte layout records at lbl_80239BA8 `{ x, y, dx, dy, f10, f14 }` and the 0x20-byte
  sprite records under arg0 + 0x5C -> 0x1C -> 4 `{ f32 f0, f4; s16 s8, sA, sC, sE; u16 id; ... s8 b1B; }`
  (the id read ends the loop at 0xFFFF), indexed with a u8 (the rlwinm masks); the sprite base is reloaded
  through the whole chain each time (a macro). (2) The loop body keeps m2c's six float locals and its three
  pointer locals (`p = &sprites[i];` three times, each for one store pair), with the second float read
  before the first pointer; the increment at the loop end (m2c put it mid-body from the schedule). The
  offset table at lbl_80239B48 reads as `((f32 (*)[2]) tbl)[k][0]`. Saved as a template for its siblings.
- Nov 6, fn_800FFA50 and fn_800FFBB0 (already 100% in the batch by the plain_readonly variant since Nov 3; no hand
  draft kept, not counted): a second route to the same code, and likely the source's form: m2c's four u16 stack locals loaded from four consecutive
  2-byte .sdata2 objects (`sp8 = lbl_8045167C; spA = ...`, then read as `(&sp8)[i]`) are one local array with a
  constant initializer, `u16 sp8[4] = { 3, 4, 5, 6 };` (values from the .sdata2 listing): MWCC puts each element
  in .sdata2 and copies it with lhz / sth after the first statement; m2c's form loads them early and leaves
  dead clrlwi. relcheck maps the anonymous constants to the lbl_ objects ("4 literal constants mapped").
- Same rule, fn_800FFBB0 (initializer { 9, 10, 11, 12 }).
- Nov 6, **fn_800AC270 39 -> 100**: the same initializer rule with bytes: m2c's u8 stack locals loaded from the
  four consecutive 0xFF .sdata2 bytes are a local `GXColor c = { 255, 255, 255, 255 };` copied member by member
  into the object's colour (lbz from the stack, stb). m2c missed the fourth byte's local (it was split by a
  store), and its temp cost the original's frame (32 vs 16). The scratch sweep initsweep.py tries the rule on
  every open draft with a run of such copies (63 drafts: this one matched; some others gained a few points).
- Nov 6, **fn_800EA33C 85 -> 100: `(void) x;` sets the saved-register order with no code.** The original swapped two
  saved registers (the count r29, the inner loop index r30; m2c's draft the other way round). Declaration order does
  nothing (all 24 orders tried); a dead `i = 0;` at the top gives the right registers but leaves its `li`; the
  statement `(void) var_r30_35;` before the first assignment gives the registers and no instruction (MWCC numbers
  the variables by their first mention in the body, then colours in that order). `x;` alone and `x = x;` do not
  work. For any draft whose only differences are swapped saved registers, try `(void) v;` for each local at the
  top (scratch sweep voidsweep.py).
- Nov 6, **fn_801155AC 85 -> 100** (the debug line draw): (1) the colour writes to the GX pipe are bytes,
  `*(u8 *) 0xCC008000 = 0xFF` (GXColor4u8; m2c wrote `int` stores: stw for stb); (2) m2c's temp for the second
  record address took the saved register r28 the original does not have: computed straight into var_r29; (3) that
  address is computed before the two position writes (the statement order: m2c put it between them, from the
  schedule; found by trying the orders of the loop head's six statements that keep their dependencies).
- Nov 6, **fn_80064500 83 -> 100: recursion auto-inlined by the original's file settings.** m2c's draft is nine
  nested copies of one body (a node's child tested against NULL, the object's flag | 0x20, its colour bytes 0xB0,
  the node's flag = 1) with the deepest copy's locals on the stack. The source is the 12-line recursive function
  (`if (n->child != (void *) 0) fn_80064500(n->child);` then the body); MWCC expands the self-call when the file
  compiles with auto-inlining and a large enough inline size: `#pragma auto_inline on` with
  `#pragma inline_max_size(N)`, N 1000 or more (the draft has 10000; 512 gives six levels, 256 three; depth pragmas change nothing), inside
  `#pragma push` / `#pragma pop` so it stays with this function. Written-out `static inline` chains do not inline
  at all (a recursive inline is refused); plain `inline` on the function emits no body. So the original file of
  this function was built with `-inline auto` (and a raised size limit): **any draft with nested copies of its own
  body, or an inline-looking expansion of a small helper m2c could not name, may be this setting**; fn_80095E6C
  (one level of its own body, 96% in wip) is the next to try.
- Nov 6, fn_80095E6C by the same setting (wip/fn_80095E6C_auto.c, 91%, the plain source): the recursive display
  function calls fn_80096374 (instance matrix) and fn_80095D7C (setup matrix) and itself; with
  `#pragma auto_inline on` and `inline_max_size` 300 to 600, MWCC expands the two helpers at the top level and its
  own body one level, the shape found by hand on Nov 5 (800 and more expand further, 200 less). fn_80096374 needs
  its two dirty flags as its own locals (declared d2 before d1: the declaration order of an auto-inlined
  function's locals sets their registers) and the camera concat through an inline (its locals go to the stack, as
  the original). Left: the inline temps' stack slots 4 bytes off and the light loop's registers. So the display
  file (fn_80095D7C .. fn_80096374) and fn_80064500's file were built with auto-inlining; m2c's 13 other
  self-recursive drafts do not change with it (real recursion in the original).
- Nov 6, **fn_80093C5C 69 -> 100** by the same setting: m2c's nine nested goto loops (the deepest locals on the
  stack) are the 18-line recursive search for a joint tree's first DObj (`while (jobj) { dobj =
  HSD_JObjGetDObj(jobj); if (dobj) break; if (jobj->child) { dobj = fn_80093C5C(jobj->child); if (dobj) break; }
  jobj = jobj->next; }`), inline_max_size 600 or more (400 gives two levels). The scratch tool recsweep.py takes a
  plain body and sweeps the size.
- Nov 6, the same file's animation walkers (fn_80094DD8 79, fn_8009496C 59, fn_80095224 63, fn_800944E4 73,
  fn_80093F30 66: m2c drafts with nested copies of their own bodies) are this setting too: fn_80094DD8 as the plain
  recursive walk with the getters fn_80093C30 (dobj->next) / fn_800944B8 (dobj->mobj) defined before it gives the
  original's expansion shape (wip/fn_80094DD8_auto.c, 58%: getters inlined at the top levels and called at the
  deepest), with `} while (dobj != (void *) 0);`; left, one more stack temp per inlined getter result in the
  original (the getters' source form, next to try).
- Nov 6, **fn_80094DD8 79 -> 100** (the joint tree's animation-rate walk) by the auto-inline setting, size 600:
  the plain recursive source with the two DObj getters defined before it in the same file, written as
  `return (d != NULL) ? d->next : 0;` (the ternary gives the extra stack temp per inlined result; the getters
  still match on their own, fn_80093C30 / fn_800944B8 100%), `} while (dobj != (void *) 0);`, and mobj declared
  before dobj (the register pair). **Link note**: the hand draft defines fn_80093C30 and fn_800944B8 too (the
  inliner needs their bodies), so its object must replace theirs, not sit next to them (one file for the three;
  promote_drafts.py does not do that yet). relcheck.py fixed for such drafts: offsets now count from the
  function's first instruction (it compared section offsets; every earlier draft had the function at 0).
- Nov 6, fn_8009496C 59 -> 89 and fn_80095224 63 -> 90 (wip/*_auto.c) by the same rewrite: the DObj -> MObj -> TObj
  walk (rate / current frame; fn_80095224 also calls HSD_JObjAnim and clears the AObj's 0x40000000 flag first),
  size 600, loop ends against `(void *) 0`. Both are left with the top-level dobj in r23 where the original has r27
  (the TObj variables one lower): declaration order, block scopes and (void) mentions do not move it.
- Nov 6, **fn_80093F30 66 -> 100**: the recursive first-end-frame search (`while (jobj) { if (jobj->aobj) { frame =
  fn_80094394(jobj->aobj); break; } if (jobj->child) frame = fn_80093F30(jobj->child); jobj = jobj->next; }`,
  frame starting at 0.0f) with the end-frame getter fn_80094394 (assert + `return aobj->end_frame`) defined
  before it (link note as fn_80094DD8). It matches at inline_max_size 700 only (600 and 800 expand differently),
  where fn_80094DD8 needs 600..650 and fn_80093C5C 600 or more: if these share one file the original had one
  size, so the getters' or callers' exact source form (which sets MWCC's size estimate) is still open; each draft
  matches on its own.
- Nov 6, **fn_800A4834 68 -> 100** by the same setting (size 600 or more): the recursive "any AObj in the tree
  has flag 0x40000000" test, `while (jobj) { if (jobj->aobj && (jobj->aobj->flags & 0x40000000)) return 1; if
  (jobj->child && fn_800A4834(jobj->child)) return 1; jobj = jobj->next; } return 0;`.
- Nov 6, **fn_800995A0 44 -> 100** by the same setting (inline_max_size 400): a quicksort of a float key array with a
  parallel s32 value array, `(s32 *vals, f32 *keys, s16 lo, s16 hi)` (the parameters used directly: m2c's s32
  parameters and local pointer copies took registers), the pivot `keys[(i + j) / 2]`, both indices clamped at 0,
  the swap as `tk = keys[i]; tv = vals[i]; keys[i] = keys[j]; vals[i] = vals[j]; keys[j] = tk; vals[j] = tv; i++;
  j--;` (tv an s16; found by trying the swap orders that keep their dependencies), then the two recursive calls;
  MWCC expands each call one level.
- Nov 6, **fn_800F9014 89 -> 100** (the part description lookup): the part index plus the counts of the kinds before
  it, summed left to right, `spC = spC + ((cr_g_BodyCount + cr_g_GunCount) + cr_g_BombCount);` (index first, then
  the counts in kind order); the kind's local u32 (`mr`, not `clrlwi`); `(void) var_r29;` at the top for the
  dead r29 local (the fn_800EA33C rule). Note: hd.py masks relocated fields, so a sum order can match every word
  with the symbols swapped (a first save at "100%" had 7 relocations wrong and was undone): the scratch sweep
  f902.py scores words minus differing relocations.
- Nov 6, check: fn_80095700 (the set-frame walk the body / part poses use; already 100% from m2c) also matches as
  the 13-line recursive source in the auto-inline setting at inline_max_size 600 or more, the same range as
  fn_80093C5C / fn_800A4834 / fn_80094DD8 (600..650); fn_80093F30 (700 only) is the odd one.
- Nov 6, the number-drawing family fn_800AB124 .. fn_800AECE4 (9 drafts, 45..53%): m2c's byte copies between stack
  slots are **GXColor locals with constant initializers passed by value** (fn_80067848 takes the colour as a struct;
  MWCC copies it into a per-call stack temp and passes the address); the flag and digit tables are struct-array
  members (`->flag[i]`, `->dig[i].w`: index sum then lbzx, where `p[i + 0x5D]` folds the constant into the load).
  wip/fn_800ABC28.c 46 -> 62%; left: the stack order of colours vs temps and the width argument's extension.
- Nov 6, **the battle action file's statics: lbl_80329B60 (0xA84 bytes of .bss) is the file's own static
  variables**: a 0xA0C-byte block, then ten Vecs at +0xA0C .. +0xA78 (the view vectors at +0xA60 / +0xA6C /
  +0xA78, the action targets at +0xA30 / +0xA48 / +0xA54). MWCC addresses a file's statics from one base register
  loaded at the function entry, and copies between two of them load all three words before storing; a copy inside
  one object (the extern u8 array, one static struct, a local pointer) goes in pairs. So the about 20 functions of
  fn_8003FE98 .. fn_8005098C that touch it match only with the statics declared, in a file object. **MWCC lays a
  file's statics out in the order of their first reference in the file** (not declaration order; tested with
  cc1.py: three statics referenced as D, C, B land at 0, 12, 24). tools/file_drafts/battle_9B60/ holds these drafts:
  pool.inc declares the statics `cr_s_9B60_<offset>` and `cr_Pool9B60_Order`, a scoring-only function placed first
  that references them in address order; score.py compiles pool.inc + a draft and runs hd.py and relcheck.py
  (relcheck now maps our `...bss.0` one to one onto the original's pool symbol, like the literal constants). At
  100% there: fn_80045724 (lose, 62 -> 100), fn_800468B8 (landing, 60), fn_800456B8 (stance, 9: the pool was the
  whole difference), fn_80048E7C (pod, 42), fn_80046A68 (charge attack, 64: the stats' durations / speeds / angles
  as struct arrays, the climb angle computed before the three velocity copies). Wips there: fn_800490C8 99 (bomb),
  fn_80048708 96, fn_80046F58 95, fn_8004752C 94, fn_80045CE8 94 (magma), fn_80047AC0 85, fn_8004949C 82 (gun:
  small inlines; the original copies the anim pointer once and the progress f30 -> f31), fn_80048050 75. The file
  object itself (every function of the file in address order, the statics declared once, the real first references
  giving the layout) waits until all of them match.
- Nov 6, **fn_8004E8D4 51 -> 100** (the end-of-battle setup of one robo): of its three parameters only the action
  is `volatile` (the original stores it to its stack home and reloads it; the others are plain), `fabs` of a float
  against an f64 constant (no frsp: `fabs f29; fmr f28`), a 40-byte struct copy for the camera block, a lerp inline
  over two stack Vecs (b declared first) writing the robo fields directly (an output pointer parameter took a saved
  register), and the loads in the order t, b.x, a.x, b.y, a.y, b.z, a.z (a sweep of 7 orders: 92..100%).
- Nov 6, more animation code at 100% (tools/hand_drafts): fn_80045A48 (change: `E0 = E2 = x` chained),
  fn_8005CE74 (field command list: a while loop with a break), fn_80037FF0 (field motion step: an if-chain on a u32
  copy of the u16 command, `x != 0` tested first), fn_80072A58 (the stage collision fix-up: the 0x284-byte waypoint
  record as a u32-array struct, the stage id compare signed). Wips (scratch wip/): cr_MotionFile_Register 71 -> 79
  (the record as `(cr_MotionRec *) (base + 4) + j`: base + j * 12, then the field displacement; a struct-array member
  or a cast of base + j * 12 folds the 4 into the index), cr_Motion_SampleTrack 31 (key-decode inlines, {value,
  time} stack records, a 4-float basis array: the frame is still 0x50 short).
- **Nov 12, the statement-order sweep (`tools/order_sweep.py`), fn_8001EA48 89 -> 100.** MWCC numbers its virtual
  registers in the order the source creates values and the colouring hands out the scratch registers in that order,
  so two sources that differ only in the order of independent statements compile to the same instructions with
  different registers. m2c writes statements in instruction order (the scheduler's), not the source's. fn_8001EA48
  (the Hermite key interpolation; only f8 / f10 / f11 of three constants differed): of the legal orders of its
  statements one gives the original: `1 - t`, `2t`, `(1 - t)^2`, then the basis values in the order h00, h11, h10,
  h01. The tool does this for any draft: runs of consecutive simple statements, the pairs that must keep their
  order (def / use, memory writes against reads, calls), every legal order up to a cap scored with hd.py, the best
  kept run after run; a 100% is checked with relcheck.py and saved to tools/hand_drafts with `--save`.
  `python -I tools/order_sweep.py --open 50 99 --save` runs it over every open draft in that score range (results:
  `build/order_sweep_run1.log`, improved but unmatched drafts in `build/order_sweep/`).
- Nov 12, the first full order sweep (`--open 50 99`, 372 open drafts): **77 improved, none to 100% by order
  alone** (the largest: fn_80130B48 83 -> 96, fn_800999F0 71 -> 84, the seven number-drawing drafts 64 -> 72,
  fn_800FE954 87 -> 93, cr_Save_ApplyUnlocks 92 -> 95). Statement order is one of several differences in most
  drafts; the improved sources are the new starting points (`build/order_sweep/`, not committed).
- **Nov 12, fn_80130B48 96 -> 100: a division by a power of two.** `(f32) (0x10 - n) / 16.0f`: MWCC turns the
  division into a multiply by the reciprocal with the operands (x, c) (`fmuls f31, f1, f2`); m2c writes
  `x * 0.0625f`, which loads the constant first (f2 / f3 swapped, `fmuls f31, f3, f1`). `tools/recip_sweep.py`
  tries this on every open draft (literals and `lbl_` constants whose value in main.dol is 1 / 2^k): 6 more
  improved (fn_80101284 88 -> 90, cr_EV_SET_SQARE 52 -> 59, cr_E_CIRCLE_SET_POS 57 -> 61), no new match.
- **Nov 13, the 100 largest of the lowest-scoring drafts** (`build/triage/low100.txt`: the 100 largest open drafts
  under 40%, 2.0 - 12.9 KB, 0 - 38% at the start; 410 KB of the 997 KB in the 867 open drafts). New
  `tools/triage.py` says why a draft scores low: its size against the original, the instruction kinds aligned with
  registers and immediates dropped (*shape*), the words equal with only the register fields dropped (*regs*), both
  frames, and the instructions it has too many of. Over the 100 two groups: (a) drafts 1.3 - 1.8x the original
  with twice its frame (fn_800F21C8, fn_8010A5CC, fn_80128F20, fn_800DF4C8, cr_CAMMODE_CUSTOMIZE_Frame,
  fn_80101E00, fn_8002B3C4: the originals themselves spill, MWCC out of registers; the drafts spill far more), ~7,100
  extra stack loads and as many stores, ~4,000 extra `lis`; (b) drafts at the original's size whose shape is 75 -
  96% and whose score is low only because registers or the frame differ. The score (aligned whole words) cannot see
  progress in either: a fix that makes the structure right often moves registers and lowers it (fn_8004DC78's
  struct copy loops and switch: the original's loop exactly, 32 -> 23%). Hand work keeps the structurally right
  version in `build/hand100/`, the best-scoring one in `build/low100/` (neither committed).
- Nov 13, what helped (each kept only where the score went up): **`tools/base_regs.py`**, a global whose address
  the original holds in a saved register (79 of the 100) as a local pointer: 26 drafts up, +970 words (fn_8009DAD8
  20 -> 35, fn_8005D338 35 -> 46, fn_80124F5C 24 -> 37, fn_80072064 17 -> 27); **`tools/inline_temps.py`**, m2c's
  pure single-assignment temporaries back into their uses: +249 words; the order sweep over the 100 (`--list
  --from --keep`); **`tools/struct_copies.py`**, M2C_STRUCT_COPY with its one-unit tail as a struct assignment
  (the generator's struct_copy_macro misses the tail form): right loops, mixed scores (+10 words). By hand
  (`build/hand100/`): fn_8000E64C 9 -> 34 (a 23 x 4 byte table copied from read-only data onto the stack, m2c's
  `s32 sp8[34]` and a broken copy; m2c's `if ((u32) x <= 22)` around the jump-table switch dropped: the switch's own
  bound), cr_Battle_UpdateRobos 34 -> 37 (two stack Vec3s of a lerp were six loose floats: 11 saved FPRs, now the
  original's 5), fn_8007B054 35 -> 38 (a 32-byte element struct: `e[i].f10` gives the original's `lhzx` with
  `i * 32 + 0x10`; m2c's byte arithmetic is reassociated into add + lhz 0x10), cr_SEQ_SCENARIO_Enter compiles again
  (m2c dropped fn_80073728's second argument, `&lbl_803B8C00`), fn_80022830 (GXInitTexObjCI's ninth argument is
  on the stack: m2c's `sp8 = 1` + M2C_ERROR is the argument 1).
- Nov 13, more of the same kind: **`base_regs.py` `reuse`**: the draft already has m2c's register variable for
  the address (`var_r31 = lbl_80386550;`) but names the global at the later uses; through the variable, all such
  variables at once (fn_80082F84: either of its two alone 86 -> 59 / 64 words, both 118, 9 -> 14%; its 83 extra lis
  gone). **`tools/index_loads.py`**: m2c's byte arithmetic on an array of records (`M2C_FIELD((base + ((i << 5) +
  0x10)), u16 *, 0)`, which MWCC reassociates into add + lhz 0x10) as the record field `((struct E *) base)[i].v`,
  which gives the original's index-then-indexed-load (lhzx / lwzx / lfsx), and `M2C_FIELD((B + K), T *, i * size)`
  as `((T *) (B + K))[i]`. **`tools/unk_stack.py`**: m2c's untyped stack areas (`f32 unksp930;` indexed with byte
  offsets, `*(&unksp930 + ((i * 8) & 0x7FFF8))`: f32 * arithmetic scales them by 4 again, so the draft reads the
  wrong places and its frame lacks the area) as arrays to the next area with byte offsets that are bytes:
  fn_8002B3C4 39 -> 93 words; fn_800E4AC4 by hand (three 192-element arrays: {id, count} at 0x930, f32 at 0x630,
  {id, value} at 0x30) 7 -> 26%, its frame 4032 for the original's 4000 (the draft's: 256). Only those 4 drafts had
  it, all of them here. **`tools/reuse_exprs.py`** (a register variable's expression written out again later goes
  through the variable): +1 word over the 4 tried. cr_Battle_ApplyRoleMatrices: its two expanded
  HSD_JObjCopyMtx bodies (asserts at jobj.h 0x495 / 0x496, PSMTXCopy into jobj->mtx, flags |= 0x03800000, the
  dirty-mark inline) as inline functions: the same score (279 -> 274), the right shape; its frame is still 48
  bytes over (the original has a second stack Mtx at 0x60 and fewer spills).
- Nov 13, measured and dropped: **`tools/split_slots.py`** (one variable per value for m2c's reused stack-slot
  variables: no change in any draft, MWCC splits a variable's live ranges itself); **`tools/hi_consts.py`** (m2c's
  bare address halves, `sp3C = 0x80400000;` ... `sp3C + 0x7260`: the original's `lis` of a global spilled to the
  stack, which the disassembly cannot name; as symbols the drafts spill more, 2 -> 1%); cleaning m2c's reused
  `var_rNN` names into plain C (cr_SEQ_BATTLE_FrameA 34 -> 29: the original's temporaries really sit in r23 - r27,
  probably inlined helpers' locals, and m2c's names keep that). Two load / store forms are open: fn_8000E64C's first
  store of each case is `addi; sth 0(r)` in the original (no form of `cr_g_BattleLoadouts[0].unk_0` tried gives
  it), fn_801076D8's first load is folded (`lbz r0, sym@l(r5)`) where ours is not. cr_Save_ResetData (1.82x):
  the original copies cr_g_UnlockBits from cr_g_SaveData.unlocks (0xA8 bytes) with no loop, interleaved with the
  mode resets; m2c's ~60 temporaries are that schedule; inlining them (all single-assignment ones, stores between
  ignored) changes nothing. A stack-array cut at the original's save area (m2c sizes the last array to the frame's
  end: fn_8004DC78 `sp14[11]` for 5) changed no score; dropped.
- Nov 13, **`tools/cror_fix.py`**: m2c does not know `cror` (MWCC's float `a >= b` is `fcmpo; cror eq, gt, eq;
  beq`): it leaves `M2C_ERROR(/* unknown instruction: cror 0x2, 0x1, 0x2 */);` and the next condition tests `==`.
  The error line dropped and that `==` / `!=` made `>=` / `<` (`cror 2,0,2`: `<=` / `>`): all 12 drafts that have
  it up a little (cr_E_CIRCLE_SET_POS 57 -> 62, fn_8006207C 16 -> 19), none to 100.
- **Nov 13, where the 100 stand** (`tools/best_of.py`, table `build/triage/low100_progress.tsv`, the drafts in
  `build/low100/`, not committed): 91 of 100 up, none at 100%; equal words 16,161 -> 19,157 (+2,996); mean score
  15.8 -> 19.3%, shape 57 -> 60%, regs 43 -> 47%. Largest: cr_BattleRobo_UpdateAction 5 -> 16 (169 -> 421 words),
  fn_8000E64C 9 -> 34, cr_SEQ_SCENARIO_Enter 0 -> 31 (it compiles), fn_800F21C8 2 -> 5 (125 -> 260), fn_80109314
  11 -> 22, fn_8005D338 35 -> 52, cr_Chara_PoseLimbs 16 -> 29, fn_800E4AC4 7 -> 26, fn_8009DAD8 20 -> 35, fn_80124F5C
  24 -> 37. Unchanged: fn_80051384, fn_800AFE3C, fn_800B2370, fn_8008ACEC, fn_8006EEF8, fn_801076D8, fn_8005E270,
  cr_Pad_Update, fn_80022830. Next: the rewrites that are plain corrections of m2c (cror_fix, unk_stack, the R3_SET
  returns, struct_copies' tail form) as generator rules so every draft gets them at batch 12; the
  group (a) giants need the scheduled code read back into source order (cr_Save_ResetData), which no rewrite here
  does.
- Nov 13, the generator: `R3_SET` gains fn_8008EDA8 (`void *`: it ends with `bl fn_8008EA64` and leaves the pointer;
  fn_8000CFAC stores through it 9 times, compiled as 0), fn_80061B90 (s8), fn_8006A00C (s32), fn_8001C98C (`void *`,
  its first parameter, which its callers pass on): about 28 of the 42 `Read from unset register $r3` errors in 8 of
  the 100 follow calls to these four. Takes
  effect at the next full batch 12.
- **Nov 13, the next 100** (`build/triage/next100.txt`: the next 100 largest open drafts under 40%, 0.75 - 2.0 KB,
  129 KB; drafts in `build/next100/`, hand work in `build/hand200/`, table `build/triage/next100_progress.tsv`, the
  per-function results of both passes in [low_drafts_progress.md](low_drafts_progress.md)). The same chain:
  base_regs (+1,105 words), index_loads (+36), unk_stack, cror_fix (+1), struct_copies (+6), inline_temps (+93), the
  order sweep over 8 slices, the chain again: **73 of 100 up, none at 100%; equal words 9,157 -> 10,608; mean score
  25.0 -> 30.8%, shape 59 -> 64%, regs 47 -> 52%** (fn_80072B90 21 -> 63, fn_8009E6F8 29 -> 54, fn_80078000 12 ->
  43, fn_80082364 25 -> 46, cr_SEQ_MOVIE_FrameA 23 -> 44, fn_80061620 31 -> 50). Same profile as the first 100 at
  this size: ~2,450 extra `lis` (the base pointers), indexed loads and stores short. `base_regs.py` now also takes a
  small-data object's address held in a saved register (`li rN, sym@sda21`; cr_StageSelector_Init keeps two).
  By hand: **fn_800B7810 34 -> 57%** as plain C: lbl_803B8C00 as one struct (objects at 0x44, sixteen 0x94-byte
  slots at 0x17A8 that end where an array of 8-byte counters starts, 0x20E8, joints at 0x7240), HSD's `ref_DEC` /
  release / destroy as an inline, the frame now the original's; left: the original's base is r31 and its loop
  counter r30 (ours the other way round, whatever the declaration order or counters per loop), and it adds
  0x17A8 to the base before the index (`addi r27, r31, 0x17a8; add r27, r27, i * 0x94`), ours folds it.
  fn_80119680: a 4-byte colour copied from small data and passed by value to an inline that paints vertex colours
  (its two calls are the stack copies at 0xC and 0x8): shape 81 -> 87, score 44 -> 38 (the colour lands at 0x11,
  the original's at 0x10, in any declaration order; kept as `build/hand200/fn_80119680_inl.c`). Open by hand:
  fn_80065E10 (three nested inlines: `mr r29, r22; mr r20, r29; mr r31, r20`), fn_800443C0 (three parameters in
  stack homes), cr_Cam_Init (lbl_80321DD0 as a struct like fn_800B7810's: cameras at 0x2E00, stride 0x13D0, a table
  at 0x7D40).
- **Nov 13, the third set: the 200 lowest-scoring of the rest** (`build/triage/third200.txt`: after the first two
  sets only 81 open drafts under 40% were left; the 200 lowest-scoring remaining ones, 3 - 49%, 20 bytes - 3.1 KB,
  139 KB, largest first; drafts in `build/third200/`, hand work in `build/hand300/`, per function in
  [low_drafts_progress.md](low_drafts_progress.md)). The chain (base_regs +720 words, index_loads +16, cror_fix +7,
  struct_copies +10, inline_temps +68, reuse_exprs +66), the order sweep as a pool of 8 one-function processes (no
  slice waits on a large function), the chain again: **113 of 200 up, 28 matched (tools/hand_drafts); equal words
  14,942 -> 16,468; mean score 46.2 -> 49.9%.**
- **Nov 13, 28 matches by hand, and what they teach about this game's source** (small functions first; each checked
  with relcheck.py):
  - **Named locals take the saved registers, even in a leaf function.** cr_Bits_Test / Clear / Set (`u32 word =
    bit >> 5; u32 mask = 1 << (bit & 0x1F);` in r31 / r30, the array index computed again from bit), the matrix
    helpers (`f32 *m = cr_g_CurrentMtx;` in r31: cr_Mtx_TranslateX, fn_8001DC84, fn_8001DCD0, cr_Mtx_Store,
    fn_8001E0D0, fn_8001DFE8, fn_8005BE70), named f32 partial sums in f31 down (cr_Mtx_TranslateZX, fn_8001DBF0,
    fn_8001DE6C, cr_Rand_Float, fn_800AC10C). A saved register the code never uses is a named local the optimiser
    removed (fn_8012D7E8: `u16 v = ...; return v;` saves r31 and loads into r3).
  - **Declaration order decides which saved register a local gets**: cr_Mtx_Push (m before t), fn_8001D74C (t
    before m), and the three column rotations fn_8005BD7C / fn_8005BEC8 / fn_8005BFBC (six named products; which
    three are declared and computed first, and the column pair, tried by script: 6 column pairs x 4 orders, one matches).
  - **Inline helpers keep their own locals**: cr_Mtx_Push / fn_8001D74C / fn_8001D8DC copy twelve floats through an
    inline whose two local pointers take r30 / r31 (saved, then coalesced away); cr_Mtx_Pop 23 -> 91% the same way.
  - **`((void *) 0)` is not `0`**: a pointer test against `((void *) 0)` compiles to `li r0, 0; cmplw` (fn_8001D74C,
    fn_8001D8DC, fn_8006B0AC), a plain 0 / NULL to `cmplwi`.
  - **A record copied field by field is `lfs` / `stfs`**; a struct assignment copies words (cr_Vec3_Cross; the order
    sweep reaches it from the m2c draft as well).
  - **Our types are sometimes signed where the original is not**: cr_Archive_SetEntryTable stores its 16-bit count
    through a u16 lvalue (the struct field is s16 in our types), advances `table += 2` and returns 0.
  - **m2c drops parameters a callback does not read**: fn_800690F4 (three parameters, returns the third).
  - **Record arrays with a u8 index** (fn_8006B0AC, fn_80124EAC, fn_800AF2E0): `((T *) table)[i].field` per access.
  Near: cr_Mtx_Pop 91 (r3 / r5 swapped), cr_Cos16 / cr_Sin16 47 -> 63 (`((f32 *) (tbl + 4))[i]`; the original reuses
  the argument's r3 for the table address), fn_801307C0 61, fn_80069008 (16 of 52 words: an unused colour copied to
  the stack and a vertex record setter, the order not found).
- Nov 13, `order_sweep.py --save` no longer overwrites an existing hand draft (the third set's chain had read six of
  the new hand drafts back into `build/third200/` and the sweep rewrote them with its generic header; restored).
- **Nov 13, batch 12 and promotion after the three sets** (`tools/draft_batch.py --jobs 8`, 2,494 s; then
  `tools/promote_drafts.py`): **998 of 1,751 game functions at 100%** (882 at the last report), m2c failed 0 (2
  before), 605,340 of 1,230,376 bytes equal (592,624). The R3_SET returns took effect (pass 1: 15 prototypes made
  value-returning by their callers). Promoted: 904 functions (0x30E6C bytes) and 7 file objects (8 functions);
  `decomp/src/game` 822 -> 914 files; `main.dol` still builds with the retail SHA-1 (`ninja`: build/GXCE01/main.dol
  OK). Three older hand drafts stopped compiling (fn_800A0C20, cr_CharaEntry_OnLoaded, cr_ModelEntry_LoadAndBuild):
  the new drafts type lbl_8044F40C differently (CR_MIXED), so the context no longer declares it; each now declares
  `extern u8 *lbl_8044F40C;` itself and matches again (100%, relocations equal); the next batch counts them.
- **Nov 13, the fourth set: the next 200 lowest-scoring** (`build/triage/fourth200.txt`: the 200 lowest-scoring open
  drafts left after the first three sets and the hand drafts, 3 - 66%, 64 bytes - 15.4 KB, 177 KB, largest first;
  drafts in `build/fourth200/`, hand work in `build/hand400/`, per function in
  [low_drafts_progress.md](low_drafts_progress.md)). The chain (base_regs +358 words, index_loads +37, cror_fix +18,
  struct_copies +11, inline_temps +56, reuse_exprs +72), the order sweep as a pool of 7 one-function processes
  (52 functions up, +306 words), the chain again (+25), then hand work: **106 of 200 up, 15 matched
  (tools/hand_drafts); equal words 23,215 -> 24,446; mean score 60.7 -> 64.2%.**
- **Nov 13, 15 matches by hand, and what they teach** (each checked with relcheck.py):
  - **Function tables**: fn_80070130 (`((void (**)(void)) tbl)[(s8) state[4]]()`), fn_8011E9E0 / fn_801232E8 /
    fn_8012CDBC (a two-level table: `((void (*(*)[2])(void)) tbl)[(s8) st[0]][st[1]]()`), fn_80065BA0,
    fn_8010E3A4 (the handler takes the record and a u8 slot; the count is homed on the stack, `volatile`).
  - **A callee declared to return s8 adds an `extsb`**: fn_80068C7C / fn_80068B84 call fn_80069AD8 through an
    `s32`-returning cast and index the name with `(s16) i` (i itself s32).
  - **A `void *` parameter cast at each use drops in register priority**: fn_80068B84's message parameter is r27 in
    the original; typed `u8 *` it took r31 and shifted every local.
  - **Write address sums left to right**: the three heap allocators (cr_Heap2_AllocAligned, cr_Heap_AllocAligned,
    fn_80023464, one source on three heaps) build the new block as `(u8 *) b + b->used + 0x20`; every grouped
    spelling (`b + (used + 0x20)`, integer casts, `&((u8 *) b)[...]`) is reassociated to `(used + b) + 0x20`.
  - **Top-tested loops are `for (;;) { if (...) break; ...}`** (fn_8012F28C, cr_Text_CountChars); a plain `while`
    is rotated to a bottom test.
  - **`x = x + (a - b)` is not `x += a - b`** (cr_CAMMODE_SNR_AUTO_Y_ANG_Frame: the long form loads and stores
    through two separately computed addresses, as the original does).
  - **Record runs** (fn_80067848: a 32-byte vertex record, colour at 0x18).
  Near: fn_80069D20 93% and fn_80066948 80% (the window / message text lookups: the original adds the table to
  `off + 8`, ours is reassociated; the same idiom as cr_Text_Get*Name), fn_80069570 92%, fn_800F8C68 92% (an
  unused local kept in r25), fn_8000B42C 88% (colour initialisers `{ 0xFF, 0xFC, 0x00, 0xFF }` copied through an
  inline helper; the loop limit 110 is a named local; the parameter's register still differs), fn_80067000 75%,
  fn_800EA468 75%, fn_80068DD8 73%, fn_800B4208 64% (a 16-byte copy loaded as words 1, 0, 2, 3, not found).
- **Nov 13, batch 12 and promotion after the fourth set** (2,495 s): **1,016 of 1,751 game functions at 100%** (998
  before), 608,020 of 1,230,376 bytes equal (605,340). Promoted: 920 functions (0x319D8 bytes) and 7 file objects;
  the 15 new matches and the three re-declared lbl_8044F40C drafts are in `decomp/src/game`; `main.dol` still
  builds with the retail SHA-1 (6a73cd11...).
- **Nov 13, the rest: every open draft outside the first four sets** (`build/triage/rest176.txt`: the 176 open drafts
  left once the four sets and the hand drafts are taken out, 66 - 97%, 56 bytes - 3.1 KB, 118 KB, largest first;
  drafts in `build/rest176/`, hand work in `build/hand500/` and, while batch 12 ran, `build/hand600/`; per function in
  [low_drafts_progress.md](low_drafts_progress.md)). The chain (base_regs +73 words, index_loads +6, inline_temps +22,
  reuse_exprs +1), the order sweep as a pool of 7 (1 match, fn_800B14B0), the chain again (+0), then hand work:
  **24 of the 176 matched, and fn_8008891C from an earlier set: 25 new in tools/hand_drafts.** Equal words of the set
  against the drafts of the day before the batch: 23,040 -> 23,331 (the first 16).
- **Nov 13, the 25 new matches (24 by hand, fn_800B14B0 by the order sweep) and what they teach** (each checked
  with relcheck.py):
  - **A function-table call takes no arguments m2c gives it**: m2c passes the table and the state block it read
    (`(*(M2C_UNK (**)()) (tbl + st->f4 * 4))(tbl, st)`), but they are only the registers that held them. Written
    `((void (**)(void)) tbl)[st->f4]()` ten 56-byte dispatchers match (fn_800A4440, fn_800A46F0, fn_800A4B98,
    fn_800A4C00, fn_800A9FEC, fn_800B2300, fn_800B2338, fn_800B818C, fn_800BB454, fn_800BC730), with two levels
    `((void (*(*)[2])(void)) tbl)[st[0]][st[2]]()` fn_80130744, with the state block as a real argument fn_8011AF04,
    returning u8 fn_800929E0 (`state = ((u8 (**)(void)) tbl)[state - 1]()`), with (robot, slot) fn_80092610.
    `tblcalls.py` (scratch) tried the rewrite site by site on all 55 open drafts with an indirect call; only small
    gains outside these.
  - **A compare whose result nobody reads is a compiled-out debug test**: fn_8008891C loads a byte global and
    `cmplwi`s it with 1 right before an unrelated test overwrites cr0; `if (lbl_8044F650 == 1) { (void) 0; }` matches
    (an empty `{}` or a bare expression does not keep it). fn_80044174 has one on a table byte, and a saved register
    holding the constant 0x20 that nothing reads: the debug test names it, `{ (void) size; }`. A scan of the original
    code for cr0 compares overwritten unread (`deadcmp.py`, scratch) finds them also in fn_80041140, fn_80068420 /
    fn_80069214 / fn_80069AD8 (a null test of the glyph range pointer), fn_8006B144, fn_800764D0, fn_800968B4 (a call's
    result against -1) and fn_80125DCC.
  - **An unused saved register can be a narrow local the store does not need**: cr_Bgm_FadeOut / fn_80097488 save
    r29 and never use it; `s16 d = (time * 200) / 60;` stored to the s16 field matches (s32 or u16 locals do not, nor
    an inline helper). fn_80096E28, the third slot loop, reassigns its u16 parameter and stays at 97% (the store
    takes the untruncated r0).
  - **Records with u16 indices**: fn_8008CDE4 / fn_8008CCF8 (the largest keyframe of a 32-byte key list, both
    indices u16, `((void *) 0)` test), fn_80092CB8 (an alpha pulse over 32-byte vertices, u8 index, `-=` / `+=` on the
    byte so the address is computed once), fn_8008EE68 (a texture-object cache of 8-word entries, `u32 (*)[8]`;
    `GXInitTexObjCI` takes a ninth argument, the TLUT name 1, that m2c lost as a stack slot).
  - **A typed alias of the object outranks it**: fn_8008F3CC reads the flags through `s32 *flags = (s32 *) obj` (r31)
    and everything else through the parameter; a `void *` alias took the lower register. Its steps are 8-byte
    records and its `switch` is an if-chain on 0xFFFF / 0xFFFE / 0xFFFD.
  - **Top-tested loops and the exit store**: fn_800670F0 (text append) stores the terminator on the loop's exit path
    and returns there, and increments the copy count before the two indices.
  - **Chained stores**: fn_80044174 writes `a->f300 = a->f304 = c;` and `a->f296 = a->f297 = 0;` (one load, one
    zero register, the original's store order).
  Near: fn_8001C5AC 97% (two f64 locals are f32; the rest is one constant the original reads with `lis` / `lfs @l`:
  its 8-byte dtk size puts it in small data under `-sdata 8`), fn_80096E28 97%, fn_80044A90 96% (a struct copy of a
  Vec3 into a 0x26C-byte record, base-first sums; two address computations scheduled apart), fn_800666B8 96% (five
  u16 ids copied as a 10-byte struct; load scheduling), cr_Save_ApplyUnlocks 95%, fn_800389C8 94% (record
  addresses written `p = (void *) (i * 0xA60 + 0x1DC); p = base + (s32) p;`, base first; the bit index keeps the
  wrong register), fn_800FE954 93%, fn_8011A37C / fn_80118C08 92% (one shared window-text tail; a helper local takes
  r27 instead of a coalesced r26), fn_8006975C 86%, fn_80069AD8 49 -> 86% (the glyph decoder: a u16 read for
  double-byte characters, the counter s16 truncated to s8, the handler called with (code, index); registers left),
  fn_80068988 86%, fn_800943D8 85%, fn_80079F28 79% (one saved register only).
- Nov 13, while batch 12 ran (it rewrites `decomp/drafts`), hand drafts were scored against `build/regen`, a
  `draft_batch.py --out build/regen --only fn_8001C5AC --no-summary` run: it has the same context and every original
  object. That run was first needed because a wrong sed flag had emptied `decomp/drafts/c/fn_8001C5AC.c`.
- **Nov 13, batch 12 and promotion after the rest** (2,521 s): **1,033 of 1,751 game functions at 100%** (1,016
  before), 608,616 of 1,230,376 bytes equal. The batch reads `tools/hand_drafts` as it reaches each function, so the
  8 matches saved after it had passed them (fn_80044174, fn_800670F0, fn_8008CCF8, fn_8008CDE4, fn_8008EE68,
  fn_8008F3CC, fn_80092610, fn_80092CB8) were drafted again with `--out build/regen2 --only ...` and their rows,
  sources and objects copied into `decomp/drafts`: **1,041 at 100%**. Promoted: 943 functions (0x32A60 bytes) and 7
  file objects; 90 left out; `main.dol` still builds with the retail SHA-1 (6a73cd11...).
- **Nov 13, the 150 lowest-scoring open drafts again** (`build/triage/low150.txt`: by the best score any earlier set
  reached, 1 - 29%, 208 bytes - 15.4 KB, 437 KB, largest first; 76 from the first 100, 47 from the next 100, 25 from
  the third set, 2 from the fourth; drafts in `build/low150/`, hand work in `build/hand700/`, per function in
  [low_drafts_progress.md](low_drafts_progress.md); its start column is the current m2c draft, so most of the 107 "up"
  are the earlier sets' work carried over). Triage (`build/triage/low150_tri.tsv`): all but one compile; mean shape
  55%; the drafts store to and load from the stack about 7,600 times more than the originals (fn_8010A5CC: 1,303
  `stw` to the frame against 353), so the large ones need their temporaries restructured by hand.
  `site_rw.py` (scratch: the table-call, base-first, compound-assignment and u8-cast rewrites, site by site, kept
  by score) moved 8 of the 150 by a few words; the earlier chains had already rewritten these drafts.
  **14 matches by hand: 6 of the 150 and 8 related functions outside it.**
  - **An inline with one parameter per field**: fn_8006A00C / fn_8006A1A8 / fn_8006A344 save r15 - r31 and keep every
    zero of the window style in its own register. They are the arguments of an inline style setter
    (`cr_Window_SetStyle(x, y, size, spacing, c0 .. c3, f0c, f0d, f1c, f00, g0, g1)`), each its own parameter.
    The same inline gives fn_8006A4E0 (one position local per branch, as m2c's two temporaries said) and fn_80069DA4
    (the object a `void *` parameter, cast at each use).
  - **`((void **) (steps + i * 8))[1]` keeps `i * 8 + 4` as the index**: every grouped spelling
    (`steps + (i * 8 + 4)`, casts, operand orders) is reassociated to `(steps + i * 8) + 4`; `| 4` keeps the shape
    but compiles to `ori`.
  - **The text-lookup family**: written left to right, `(u8 *) table[a] + off + 8` (fn_80069D20) and
    `text + off + 8`, with the offset read as `((u32 *) (file + 0x10))[id]` (cr_Text_GetBodyName, GunName,
    PartDescription; BombName, PodName and LegName also need `arg0 + count1 + count2 ...`, flat, left to right).
    fn_80066948 stays at 84%: there the text base is added after 8, not before.
  - **An inline allocator**: fn_80097154 / fn_800972D8 (BGM play) find a free slot in an inline
    (`for (i = 0; i < 2; i++) if (free) { slot[i] = *default; return &slot[i]; } return NULL;`, the 56-byte
    default slot copied as a struct). cr_Bgm_Play, the same with an `s16` fade time, is 30 -> 97% (one register).
  Also up: fn_800735C8 29 -> 61% (the chara-entry relocator: the reloc table is words, not bytes; the original keeps a
  copy of the base and an alias of the header in their own registers), fn_8006B144 27 -> 42%, cr_SinCos16 17 -> 29%
  (a frameless leaf: no spelling of its two locals found that keeps them out of saved registers).
- **Nov 13, the 14 matches drafted again** (`draft_batch.py --out build/regen3 --only ...`, 126 s) and merged into
  `decomp/drafts` as before: **1,055 of 1,751 at 100%**. Promoted 957 functions (0x33BFC bytes) and 7 file objects;
  `main.dol` still builds with the retail SHA-1 (6a73cd11...).
