# Compiler and flags (Phase 4 Milestone 3)

Which Metrowerks CodeWarrior (`mwcceppc`) version and flags built each part of `main.dol` (GXCE01 v1.0), found by
compiling known C with every pinned GC compiler and comparing the code with the retail bytes.
Rerun: `pwsh tools\batches\batch10_compiler_probe.ps1` on Windows, `tools/batches/batch10_compiler_probe.sh` on Linux /
macOS (about 5 minutes, ~50,000 compiles; the full run includes it since plan.md step 52).
Script: `tools/compiler_probe/probe.py`; results (gitignored): `tools/compiler_probe/out/summary.txt`, `results.tsv`.

## Result

| Part of main.dol | Compiler (decomp.me / compilers zip name) | Flags | Confidence |
|---|---|---|---|
| Dolphin SDK (Apr 17 2003 build) | **GC/1.2.5n** (CW 2.3.3 build 163, the community's epilogue-fixed 1.2.5) | `-O4,p -inline auto -fp_contract off -char unsigned` (dvd, mtx, CARDOpen/CARDRename `-char signed`, as dolsdk2004); **EXIBios.c `-O3,p`** | exact: 336 of 406 functions, 103 match **only** 1.2.5n |
| HSD / sysdolphin (HAL) | **GC/1.3.2** (CW 2.4.2 build 81) | `-O4,p -inline auto -fp_contract on -str reuse` (Melee's sysdolphin set, its compiler changed) | exact: 174 of 370; 1.3 loses 5, 2.x differs in drawShapeAnim, 1.2.5n gets 49 |
| HSD audio (axdriver, synth, devcom) | assumed **GC/1.3.2**, same flags as HSD | as HSD | closest: 1 of 16 exact (Melee's audio is an older revision); 1.3.2+ beats 1.3 in 9 functions, nothing separates it from 2.x |
| MSL (C library) | **GC/2.5, 2.6 or 2.7** (CW 2.4.7 build 105..108; identical output here); use **GC/2.6** | `-O4,p -inline auto,deferred -use_lmw_stmw on -str pool,readonly -common off -fp_contract on` | exact: 70 of 107; 5 functions match only 2.5+, 3 more only 2.0+ |
| Runtime (`__va_arg`, `__mem`, `global_destructor_chain`, `runtime`) | **GC/1.3.2 .. 2.7** (no function separates them); use **GC/2.6** (same CW release as MSL) | `-O4,p -inline auto -use_lmw_stmw on -str reuse,pool,readonly -common off` | exact: 30 of 33; 1.3 loses 2 |
| MetroTRK | **GC/1.3.2 .. 2.7**; use **GC/2.6** | `-O4,p -inline auto,deferred -use_lmw_stmw on -str reuse,readonly -common off -sdata 0 -sdata2 0` (`-enum int`, not MP4's `-enum min`) | exact: 99 of 183; 1.3 loses 2 |
| Game code (Noise, plain C) | **GC/1.3 .. 2.7** at this level; use **GC/2.6** (same CW release as MSL / TRK) | `-opt level=0,peephole,schedule,speed -fp_contract on` (optimization level 0 with peephole and scheduling, speed: registers saved with `stw`, not `_savegpr`); default `-sdata 8 -sdata2 8` | exact: **13 of 13** hand-written game functions; 1.2.5n and older 6 of 13, 3.0 9-10 of 13 |

Common to every part: `-nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -align powerpc` (and, for
building, `-nosyspath`, warnings / includes per library).

Reading of the versions: the SDK came prebuilt from Nintendo (1.2.5-era, like every SDK of 2001-2004) and HSD prebuilt
from HAL (1.3.2, the compiler doldecomp/kar uses for Kirby Air Ride, HAL's 2003 game). MSL, Runtime and TRK ship as
libraries with CodeWarrior itself: MSL proves that release is 2.5..2.7, and TRK / Runtime fit it. The game code agrees
with every compiler from 1.3 to 2.7, so it is put on the same release (GC/2.6, also the linker most 2003-2005 GC decomps
use). The linker stays GC/1.3.2 for now (the asm build matches with it; the linker does not change code bytes).

## Method

For each library, `probe.py` compiles reference C from the pinned decomps with every GC compiler in
`decomp/build/compilers/GC` (1.0 .. 3.0a5.2, 19 versions), takes every function of the object whose name is also a
function in `decomp/config/GXCE01/symbols.txt`, and compares it with the retail bytes read from `main.dol` through the
DOL header. Fields a relocation fills in (call targets, `@ha` / `@l` halves, small-data offset + base register) are
masked on both sides; everything else must be equal. "Exact" = same size and every masked word equal; otherwise the
score is the share of equal words.

- Stage 1: every version with the library's base flags (the reference project's own).
- Stage 2: every version within half of the best stage-1 count, with 40+ flag sets: `-O4,p` / `-O4,s` / `-O3,p` /
  `-O2` x `-inline auto` / `auto,deferred` / `all` / `off` / `noauto` x `-fp_contract on` / `off`, plus single
  changes (`-char signed|unsigned`, `-use_lmw_stmw on|off`, `-sdata 0 -sdata2 0` or `8`, `-enum min`, `-pool off -rostr`).
  All candidate versions get the same flag sets, so versions are compared fairly.
- A function that matches under every candidate is no evidence; the summary lists the discriminating ones.
- `--show FUNCTION --version V --flags "..."` prints one function word by word against retail.

Sources (all pinned in `tools/pins.txt`): Dolphin SDK `tools/refs/dolsdk2004` (29 files, `SDK_REVISION=0`), HSD
`tools/melee/src/sysdolphin/baselib` (37 + 3 audio files), MSL from Mario Party 4 (38) and Melee (16), Runtime from both
(9), MetroTRK from Mario Party 4 (22) and Melee (20; `src/MetroTRK/` was added to Melee's sparse checkout for this, and
MSL's `math.h` includes it). Game: our hand-written C in `tools/compiler_probe/game/` (bitops.c, vec.c).

Flag sources: dolsdk2004 Makefile (SDK: `-O4,p -inline auto -char unsigned`, EXIBios `-O3,p`), Melee configure.py
(sysdolphin `-O4,p -inline auto -fp_contract on -str reuse` with GC/1.2.5n; Dolphin libs `-fp_contract off`; MSL
`-inline deferred,auto -use_lmw_stmw on -str pool,readonly -common off`; Runtime `-str reuse,pool,readonly`; TRK
`-sdata 0 -sdata2 0`, GC/1.1p1), Mario Party 4 (TRK `-inline auto,deferred -enum min`, MSL/TRK GC/1.3), KAR (game
GC/1.3.2 `-O4,p`, SDK GC/1.2.5n), MKDD / Pikmin 2 / TTYD / XD (GC/2.6 game and linker).

## Evidence

Counts are from one full run (`summary.txt`); "only" means no other version reproduces the function with any flag set.

| Library | Function | Address | Matches exactly with | Tells |
|---|---|---|---|---|
| SDK | OSCancelAlarm, DecrementerExceptionCallback (+101 more) | 0x80132B28, 0x80132C44 | only GC/1.2.5n | 1.2.5n, not 1.2.5 / 1.3+ |
| SDK | OSInitAlarm, InsertAlarm, OSAllocFromHeap (+244) | 0x80132808, 0x80132870, 0x80133010 | 1.0 .. 1.2.5n | 1.2.5-era, not 1.3+ |
| SDK | EXIImm, EXIDma, EXISync ... (all 22 EXIBios functions) | 0x8014.... | 1.2.5n with `-O3,p` only | per-file `-O3,p` (as dolsdk2004) |
| SDK | all | | `-fp_contract off` 336 vs `on` 330; `-inline auto` = `all` 336; `auto,deferred` 303 | flags |
| HSD | _hsdClassInfoInit, HSD_LObjRemoveAll, CObjInit, HSD_SetupRenderModeWithCustomPE, HSD_ShadowSetSize | 0x801A7B40, 0x8018EAD4, 0x80191908, 0x8018A038, 0x801A43F8 | 1.3.2 .. 2.7, not 1.3 | 1.3.2+ |
| HSD | drawShapeAnim | 0x80194EFC | 1.3 / 1.3.2: same size, 98.7% (only `__LINE__` / string offsets of 3 asserts differ: Melee's file revision); 2.0 .. 2.7: 77%, different float compare code | not 2.x |
| HSD | all | | 1.3.2 174, 1.3 168, 1.2.5n 49, 3.0 37; `-fp_contract on` 174 vs `off` 172; `-O4,s` 151; `-use_lmw_stmw on` 166; `-sdata 0` 112 | 1.3.2, flags |
| MSL | __copy_longs_aligned, __copy_longs_rev_aligned, parse_format | 0x80163644, 0x8016359C, 0x801651AC | 2.0 .. 2.7 (+3.0) | 2.0+ |
| MSL | __timesdec, float2str, double2hex, longlong2str, long2str | 0x801623CC, 0x801640C0, 0x80164908, 0x80164C40, 0x80164F54 | only 2.5, 2.6, 2.7 | 2.5+ |
| MSL | all | | `auto,deferred` 70 vs `auto` 66; `-use_lmw_stmw off` 63; `-fp_contract off` 64; `-sdata 0` 53 | flags |
| Runtime | __fill_mem, __va_arg | 0x8000543C, 0x8015F790 | 1.3.2 .. 2.7, not 1.3 | 1.3.2+ |
| TRK | usr_puts_serial, TRK_fill_mem | 0x8015A524, 0x8015BF90 | 1.3.2 .. 2.7, not 1.3 | 1.3.2+ |
| TRK | all | | `-enum int` 99 vs `min` 92; `-sdata 8` 86; `-use_lmw_stmw off` 89; `auto,deferred` 99 vs `all` 95 | flags |
| Game | fn_80023228 (set bit), fn_800231EC (clear), fn_800231B4 (test), fn_8002312C, fn_80023264 (word copy) | 0x80023228 .. | 1.3 .. 2.7 (+3.0 for some) with level 0 + peephole + schedule + speed | level 0 build, not -O4 |
| Game | fn_80023264 | 0x80023264 | `,speed` only (`,space` saves r29-r31 with `_savegpr_29`) | `speed` |
| Game | fn_8001CA28 (a += b * s), fn_8001E5E4 (dot) | 0x8001CA28, 0x8001E5E4 | only with `-fp_contract on` (fmadds) | `fp_contract on` |
| Game | fn_8001E684 (float swap: f31 saved with stfd + psq_st) | 0x8001E684 | 1.3+ | Gekko paired-single save |
| Game | fn_8001B1A0, fn_8001C9F4, fn_8001CA5C, fn_8001DE38 (r2 constant), fn_80023164 | | 1.3 .. 2.7 | consistent |

The game's level-0 code is recognisable by eye: every local lives in its own saved register (r31, r30, ...) even in
leaf functions, constants are not folded (`li r0,2; clrlwi r0,r0,24`), and the same global address is rebuilt with
`lis` / `addi` for every access, yet instructions are scheduled (stores of saved registers interleaved).

## Open questions

- **Game code: 1.3 .. 2.7.** At optimization level 0 the compilers differ very rarely (in 1,257 reference functions
  compiled at level 0: 92 differ 1.3 vs 1.3.2, 2 differ 1.3.2 vs 2.0 - a `u16 < u16` passed as a `u8` argument picks r4
  vs r0 -, 1 differs 2.0 vs 2.5, none 2.5 vs 2.7). GC/2.6 is the working choice; the first matched game function that
  hits such a construct decides it. Same for Runtime / TRK (1.3.2 .. 2.7).
- **MSL 2.5 vs 2.6 vs 2.7**: no function in these sources separates them (2.7 differs from 2.6 elsewhere in other decomps).
- **HSD audio**: Melee's axdriver / synth / devcom are an older revision (1 of 16 exact); the version is assumed from
  the rest of HSD. Kirby Air Ride's asm (tools/refs/kar/asm) is the better reference once those files are matched.
- **Never-exact functions** (SDK 48, HSD 200, MSL 36, TRK ~100) are source-revision differences (Apr 2003 SDK vs
  dolsdk2004's 2004 code; Melee's 2001 HSD; MP4 / Melee TRK vs ours, e.g. TRKDoReset calls memset + TRKWriteUARTN
  directly here), not flags: the same files' other functions match exactly. Milestone 4 adapts the sources.
- `-inline auto` and `-inline all` give the same code everywhere tested; `auto` is used (the reference projects' choice).
- `-str` / `-pool` / `-common` / `-sdata` thresholds affect data layout, which this probe (code only) checks only where
  they change instructions; the data sections are checked when objects start matching.
- The 1.2.5n compiler is a community-patched 1.2.5 (epilogue scheduling); Nintendo's real build is a 1.2.5-era
  release that behaves like it. Same caveat as in every SDK decomp.

## Flags as applied in decomp/configure.py (Phase 4 Milestone 4, step 54)

Edit `decomp/configure.py`, then `tools/make_configure_patch.py` (run with `python -I` / `python3 -I`) rewrites
`tools/patches/dtk_template_configure.patch` (applied to the pinned template by `tools/setup_decomp.ps1` /
`tools/setup_decomp.sh`). An object
is compiled with these flags whenever `decomp/src/<object>` exists; it is LINKED from C only when it is in `CR_MATCHING`.

| Top folder of splits.txt | Compiler | Flags (after the common ones) | Header roots |
|---|---|---|---|
| dolphin | GC/1.2.5n | `-O4,p -inline auto -fp_contract off -char unsigned -D__GEKKO__ -DSDK_REVISION=0 -DRELEASE` | `include/dolsdk2004`, `include/dolsdk2004/libc`, `-ir src/dolphin` (private `__os.h`-style headers) |
| sysdolphin | GC/1.3.2 | `-O4,p -inline auto -fp_contract on -str reuse -DMUST_MATCH` | `src`, `include/melee/doldecomp`, `include/melee/MSL`, `include/melee/dolphin` |
| MSL | GC/2.6 | `-O4,p -inline auto,deferred -use_lmw_stmw on -str pool,readonly -common off -fp_contract on -char signed` | `include/marioparty4`, `include/marioparty4/dolphin` |
| Runtime | GC/2.6 | `-O4,p -inline auto -use_lmw_stmw on -str reuse,pool,readonly -common off -fp_contract on` | as MSL |
| MetroTRK | GC/2.6 | `-O4,p -inline auto,deferred -use_lmw_stmw on -str reuse,readonly -common off -sdata 0 -sdata2 0 -fp_contract on` | as MSL |
| game | GC/2.6 | `-opt level=0,peephole,schedule,speed -fp_contract on` | `include` (`decomp/include/game/cr_game.h`, plan.md step 58) |

Common to all (`cflags_cr`): `-nodefaults -proc gekko -align powerpc -enum int -fp hardware -Cpp_exceptions off
-pragma "cats off" -pragma "warn_notinlined off" -maxerrors 1 -nosyspath -cwd source -multibyte -W off
-DBUILD_VERSION=0 -DVERSION_GXCE01 -DNDEBUG=1` (+ the template's `-lang=c`). Per object (`CR_OBJECT_CFLAGS`, appended,
so they win): `dolphin/exi/EXIBios.c` `-O3,p`; `dolphin/card/CARDOpen.c`, `CARDRename.c` `-char signed`; per folder
(`CR_DIR_CFLAGS`): `dolphin/dvd/`, `dolphin/mtx/` `-char signed`. `CR_OBJECT_INCLUDES` replaces a library's header
roots for a file taken from another reference (`Runtime/__mem.c`: Melee's). The linker stays GC/1.3.2.
Confirmed by the 66 objects matched in step 54 (dolphin, MSL, Runtime and MetroTRK all have objects at 100% with these
flags; sysdolphin's two tried files, util.c and object.c, are 100% code too), and since then by every object linked
from C: 92 library objects and 232 game functions (`decomp/reports/progress_report.txt`, plan.md step 59).

The Milestone 3 proposal that the table above replaced (plan.md step 50) is in this file's git history.
