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
  The MetroTRK banner `v2.6` points to the CodeWarrior for GameCube 2.6 package ("Source revisions" below).
- **HSD audio**: Melee's axdriver / synth / devcom are an older revision (1 of 16 exact); the version is assumed from
  the rest of HSD. Kirby Air Ride's asm (tools/refs/kar/asm) is the better reference once those files are matched
  ("Source revisions" below: devcom / synth largely KAR's, axdriver mostly not).
- **Never-exact functions** (SDK 48, HSD 200, MSL 36, TRK ~100) are source-revision differences (Apr 2003 SDK vs
  dolsdk2004's 2004 code; Melee's 2001 HSD; MP4 / Melee TRK vs ours, e.g. TRKDoReset calls memset + TRKWriteUARTN
  directly here), not flags: the same files' other functions match exactly. Milestone 4 adapts the sources.
  **Answered in "Source revisions" below**: every never-exact SDK function is exact with BfBB's Apr 17 2003 SDK code
  (or dolsdk2004's; EXIBios was a configure.py flag-order bug), 22 MSL functions with BfBB's MSL, 30 TRK functions with
  KAR / BfBB / SMS; HSD is a revision later than KAR's PAL build.
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
from C: 92 library objects and 289 game functions (`decomp/reports/progress_report.txt`, plan.md step 62).

The Milestone 3 proposal that the table above replaced (plan.md step 50) is in this file's git history.

## Source revisions (Oct 30, 2026)

Which library SOURCE revisions (not compiler versions: those are above) built main.dol, from the version strings in
the DOL / apploader and from compiling the same file from every reference project in `tools/refs` and `tools/melee`
with the pinned compiler and flags and comparing each function with the retail bytes (probe.py's masked compare).
Rerun: `python -I tools/compiler_probe/revisions.py` (under a minute; writes the gitignored
`tools/compiler_probe/out/revisions.txt`: the strings, per library every function the pinned reference does not
reproduce and which references do, and HSD against doldecomp/kar's asm). Plan step 66.

### Version strings

main.dol (file offset / address; `\t` is one tab byte, every string NUL-terminated):

| Offset | Address | String | Meaning |
|---|---|---|---|
| 0x23F7F0 | 0x802427F0 | `<< Dolphin SDK - OS\trelease build: Jul 23 2003 11:27:16 (0x2301) >>` | OS library (also `Kernel built : %s %s` at 0x80242844 with `Jul 23 2003` 0x8024285C, `11:27:16` 0x80242868) |
| 0x245FA0 | 0x80248FA0 | `<< Dolphin SDK - GX\trelease build: Jul 23 2003 11:27:39 (0x2301) >>` | GX |
| 0x241980 | 0x80244980 | `<< Dolphin SDK - AX\trelease build: Jul 29 2003 16:15:36 (0x2301) >>` | AX |
| 0x2412A0 | 0x802442A0 | `<< Dolphin SDK - DVD\trelease build: Sep 16 2003 09:50:54 (0x2301) >>` | DVD |
| 0x2414D8 | 0x802444D8 | `<< Dolphin SDK - VI\trelease build: Apr 17 2003 12:33:22 (0x2301) >>` | VI |
| 0x2418A8 | 0x802448A8 | `<< Dolphin SDK - AI\trelease build: Apr 17 2003 12:33:54 (0x2301) >>` | AI |
| 0x2418F0 | 0x802448F0 | `<< Dolphin SDK - AR\trelease build: Apr 17 2003 12:33:55 (0x2301) >>` | AR |
| 0x241938 | 0x80244938 | `<< Dolphin SDK - ARQ\trelease build: Apr 17 2003 12:33:56 (0x2301) >>` | ARQ |
| 0x245BD0 | 0x80248BD0 | `<< Dolphin SDK - DSP\trelease build: Apr 17 2003 12:34:16 (0x2301) >>` | DSP (+ `DSPInit(): Build Date: %s %s`, `Apr 17 2003` 0x80248C38, `12:34:16` 0x80248C44) |
| 0x245D90 | 0x80248D90 | `<< Dolphin SDK - CARD\trelease build: Apr 17 2003 12:34:19 (0x2301) >>` | CARD |
| 0x247130 | 0x8024A130 | `<< Dolphin SDK - EXI\trelease build: Apr 17 2003 12:33:17 (0x2301) >>` | EXI |
| 0x247240 | 0x8024A240 | `<< Dolphin SDK - SI\trelease build: Apr 17 2003 12:33:19 (0x2301) >>` | SI |
| 0x24AAD0 | 0x8024DAD0 | `<< Dolphin SDK - THP\trelease build: Aug 27 2002 20:42:01 >>` | THP (no `(0x2301)`: the THP library is built separately) |
| 0x1C10E8 | 0x801C40E8 | `MetroTRK for GAMECUBE v2.6` | MetroTRK banner (TRKNubWelcome loads it at 0x80159860) |
| 0x0004AB | 0x800034AB | ` Metrowerks Target Resident Kernel for PowerPC` | TRK's `__exception` (.init) |
| 0x1C1980 | 0x801C4980 | `Metrowerks CW runtime library initializing default heap\n` | Runtime GCN_mem_alloc.c; MSL has no version string |
| 0x1B3700 | 0x801B6700 | `[Feb 17 2004 18:12:51]` | the game's own `__DATE__ __TIME__` (fn_80016600 addresses it): when main.dol was built |
| 0x1BE678 | 0x801C1678 | `2003/11/17 21:27:07` | game data (heads the .hps / SNRM name tables cr_Chara_UpdateAll and cr_SYS_BATTLE address); not a library version |

`(0x2301)` is in every SDK string of every reference from 2002 to 2004, so it tells nothing. The DOL header has no
timestamp. HSD has no version string (its asserts name only the module `sysdolphin_base_library` and the file). No
string names an SDK "patch" number, so the releases below are named by their build dates only.

The disc's apploader (`sys/apploader.img`, offset 0: `2003/09/16`; `This Apploader built %s %s` = `Sep 16 2003`,
`$Revision: 32 $`) is linked with a complete Sep 16 2003 SDK build: OS 09:50:12, EXI 09:50:25, SI 09:50:26, VI
09:50:29, DVD **09:50:54**, DSP 09:51:25, CARD 09:51:28. main.dol's DVD string is the same build to the second; its
OS / EXI / SI / VI / DSP / CARD are not.

### What the dates mean

main.dol links the SDK of **Apr 17 2003** (AI, AR, ARQ, VI, DSP, CARD, EXI, SI) plus newer builds of four libraries:
OS and GX of **Jul 23 2003**, AX of **Jul 29 2003**, DVD of **Sep 16 2003** (the release the apploader came from), and
the THP library of Aug 27 2002. **BfBB (`tools/refs/bfbb`, GQPE78, Oct 2003) has the Apr 17 2003 SDK**: its sources
carry the same build strings to the second for AI, AR, ARQ, CARD, DSP, EXI, SI, VI (its OS is Apr 17 12:33:06, GX
Apr 21 2003, DVD Apr 22 2003; THP Aug 27 2002 20:42:01 as ours). dolsdk2004 / XD are the Apr 5 2004 SDK, KAR
Sep 2002 .. Mar 2003 (doldecomp/kar's asm: GX Feb 7 2003, DVD Oct 29 2002; wowjinxy's GX Nov 10 2004 / AX Apr 5 2004
strings postdate KAR and are dolsdk2004's), MP4 Sep 2002, Melee / Pikmin / AC / SMS 2001-2002. No reference has the Jul 2003
builds; compiled function by function, the Jul 2003 OS / GX are BfBB's code except where the 2004 code already
appears: GXInit has dolsdk2004's `__GXSetIndirectMask(0);` before `__GXSetTmemConfig(2);` (BfBB's GXInit with that
one line is exact), and OSInitAlarm, OSGetResetButtonState, OSSetGbsMode are dolsdk2004's (BfBB's are 88-99%).

### SDK: what each never-exact function needs

All with GC/1.2.5n and the dolphin flags of the table above; "BfBB" = `tools/refs/bfbb/src/dolphin/src/<path>` with
`-DMATCHING` (its `OSErrorLine(line, ...)` is then `OSPanic(__FILE__, line, ...)`: VIConfigure's line 1908) and
BfBB's headers (`src/dolphin/include`, `include`, `src/PowerPC_EABI_Support/include`); "2004" = dolsdk2004 (the
pinned one). Every function was compiled and compared; the merged files themselves are not built yet (no build
directory in this worktree).

| Object (CR_NOT_MATCHING) | Blocked functions | Source that is exact | Result |
|---|---|---|---|
| dolphin/exi/EXIBios.c | 22 of 23 | **dolsdk2004 as it is**: the blocker is configure.py, not the revision. `-O4,p ... -O3,p` (CR_OBJECT_CFLAGS appended after the library's `-O4,p`) compiles 1 of 23 exact; with `-O3,p` in place of `-O4,p` all 23 are exact (an earlier `-O4,p` is not undone by a later `-O3,p` or `-opt level=3`). Fix: a per-object `-O` must replace the library's | 23/23 |
| dolphin/os/OS.c | ClearArena, OSInit | BfBB os/OS.c | 15/15 |
| dolphin/os/OSFont.c | 6 of 8 | BfBB os/OSFont.c with `-char signed` | 8/8 |
| dolphin/os/OSReboot.c | __OSReboot, fn_801367D8, fn_801367E8 | BfBB os/OSReboot.c (fn_801367D8 = `Run`, fn_801367E8 = `Callback`, by bytes) | 3/3 |
| dolphin/os/OSReset.c | OSResetSystem, OSGetResetCode | BfBB os/OSReset.c | 5/5 |
| dolphin/vi/vi.c | getTiming, __VIInit, VIConfigure, VIGetTvFormat | BfBB vi/vi.c (`-DMATCHING`); VIGetRetraceCount, VIGetDTVStatus (not in BfBB's file, presumably dead-stripped from BfBB's DOL) from 2004 | 22/22 |
| dolphin/gx/GXInit.c | GXInit | BfBB gx/GXInit.c + `__GXSetIndirectMask(0);` before `__GXSetTmemConfig(2);` | 5/5 |
| dolphin/gx/GXAttr.c | __GXSetVAT, GXSetArray | BfBB gx/GXAttr.c; GXSetVtxDescv, GXGetVtxDesc(v), GXGetVtxAttrFmt(v) from 2004 | 16/16 |
| dolphin/gx/GXGeometry.c | GXSetCullMode | BfBB | 9/9 |
| dolphin/gx/GXLight.c | GXSetChanAmbColor, GXSetChanMatColor, GXSetChanCtrl | BfBB | 11/11 |
| dolphin/gx/GXTev.c | GXSetTevColor, GXSetTevColorS10, GXSetTevKColor | BfBB | 16/16 |
| dolphin/ax/AXVPB.c | __AXSetPBDefault, fn_80146ED8, fn_801470D8, fn_8014719C | BfBB ax/AXVPB.c (fn_80146ED8 = `__AXVPBInit`, fn_801470D8 = `AXSetVoiceSrcType`, fn_8014719C = `AXSetVoiceState`, by bytes); AXSetVoiceMix / ItdOn / ItdTarget / Ve / VeDelta from 2004 | 22/22 |
| dolphin/card/CARDMount.c | DoMount | BfBB (also MP4, AC, SMS) | 7/7 |
| dolphin/card/CARDRdwr.c | BlockWriteCallback, __CARDWrite | BfBB (also MP4, AC, SMS) | 4/4 |
| dolphin/exi/EXIUart.c | WriteUARTN | BfBB exi/EXIUart.c with `-char signed` | 4/4 |
| dolphin/gx/GXFrameBuf.c (data) | render-mode table | lbl_80249494 is **GXNtsc480Prog** (its 0x3C bytes equal dolsdk2004's initializer). main.dol's table is dolsdk2004's GXNtsc480IntDf, GXNtsc480Prog, GXMpal480IntDf, GXPal528IntDf, GXEurgb60Hz480IntDf in the file's order, the other 27 modes dropped by the linker (BfBB's file has the same minus Prog, which BfBB does not use): keep those 5 in the C | code was already 100% |

Not in CR_NOT_MATCHING (no C imported yet) and exact only with BfBB: CARDBios.c Retry, __CARDStart,
__CARDWritePage, __CARDEraseSector (BfBB's CARDBios.c 25/25; fn_8014C1E4 is its static `OnReset`), GXPixel.c GXSetFog
(BfBB's GXPixel.c 12/13 + 2004's GXInitFogAdjTable).

Unnamed functions whose bytes equal exactly one reference function (unique, size > 0x10, in the matching library's
place; names for symbols.txt, not applied here):

- THP, the whole library, BfBB thp/THPDec.c and THPAudio.c: fn_801AEB40 THPVideoDecode, fn_801AED84
  __THPSetupBuffers, fn_801AEDC8 __THPReadFrameHeader, fn_801AEF04 __THPReadScaneHeader, fn_801AF020
  __THPReadQuantizationTable, fn_801AF3DC __THPReadHuffmanTableSpecification, fn_801AF5BC __THPHuffGenerateSizeTable,
  fn_801AF6AC __THPHuffGenerateCodeTable, fn_801AF714 __THPHuffGenerateDecoderTables, fn_801AF8D0
  __THPRestartDefinition, fn_801AF924 __THPPrepBitStream, fn_801AFB70 __THPDecompressYUV, fn_801AFC7C
  __THPDecompressiMCURow512x448, fn_801B1704 __THPDecompressiMCURow640x480, fn_801B3190 __THPDecompressiMCURowNxN,
  fn_801B4C3C / fn_801B52B8 / fn_801B5960 __THPHuffDecodeDCTCompY / U / V, fn_801B6008 THPInit, fn_801B60A8
  THPAudioDecode, fn_801B650C __THPAudioGetNewSample, fn_801B659C __THPAudioInitialize.
- AX / AXFX (dolsdk2004, AX ones also BfBB): fn_80144784 AXInitEx, fn_80144EC8 __AXProcessAux, fn_80145810
  __AXClInit, fn_80145834 AXSetMode, fn_80147A48 __AXGetCurrentProfile, fn_80147A90 ReverbHICreate, fn_80148018
  DoCrossTalk, fn_801481A8 HandleReverb (reverb_hi.c), fn_801486B4 ReverbHIFree, fn_801487D8 AXFXReverbHiInit,
  fn_80148894 AXFXReverbHiCallback, fn_80148E14 HandleReverb (reverb_std.c), fn_801491C8 ReverbSTDFree,
  fn_801492EC AXFXReverbStdInit, fn_801493A4 AXFXReverbStdCallback, fn_801493D4 do_src1, fn_8014956C do_src2,
  fn_80149758 AXFXChorusInit, fn_801499A0 AXFXChorusShutdown, fn_801499F0 AXFXChorusSettings, fn_80149B00
  AXFXChorusCallback, fn_80149E50 AXFXDelayCallback, fn_8014A020 AXFXDelaySettings, fn_8014A2E8 AXFXDelayInit,
  fn_8014A330 AXFXDelayShutdown.
- Others: fn_80136490 OnReset (OSMemory.c, static), fn_8013BC8C DVDReadDir (2004 dvdfs.c), fn_8013C9DC
  cbForStateCheckID3 (BfBB dvd.c), fn_801409B0 ClampCircle, fn_80140B58 PADClampCircle, fn_801415EC PADRecalibrate,
  fn_80141700 PADRead, fn_80141A00 PADControlMotor, fn_80141AB8 SPEC2_MakeStatus (2004 Pad.c / Padclamp.c),
  fn_80150DC4 EraseCallback, fn_80150E74 CARDWriteAsync (CARDWrite.c), fn_8015805C GXDrawCubeFace, fn_80159354
  TEXGetGXTexObjFromPalette, fn_80159428 TEXGetGXTexObjFromPaletteCI (2004 texPalette.c).
- Equal to several names, left alone: fn_80144760, fn_80144D70, fn_80145FB8, fn_80148848, fn_80149358,
  fn_80150C54, fn_8014A3D0, fn_8014A3F8; fn_8001B1A0 (game code) equals GXDraw's `vsub` by chance.

### MetroTRK and MSL

- **MetroTRK v2.6.** The references' banners: Melee, Pikmin, SMS, BfBB `MetroTRK for Dolphin v0.8`; MP4, AC
  `MetroTRK for GAMECUBE v0.10`; KAR (wowjinxy) `v2.0`. None has v2.6 source. By function sizes (symbols.txt of the
  config-only references) our 160 named TRK functions equal **XD (GXXE01) 160/160**, and TP (GZ2E01), TTYD (G8MJ01),
  Pikmin 2 (GPVE01) 159/160 (only InitMetroTRK: 0x98 there, 0x94 here and in XD); e.g. TRKDoReset 0x58,
  TRKDispatchMessage 0x170, InitMetroTRKCommTable 0x26C, __TRK_copy_vectors 0x12C in all of them. In those three maps
  fn_8015E09C (0x1EC) and fn_8015E288 (0x134) follow InitMetroTRK_BBA as **TRK__write_aram** and
  **TRK__read_aram** (position and size; bytes not compared). Compiled with GC/2.6 and the TRK flags, KAR's
  metrotrk.c reproduces 28 functions MP4 / Melee do not: InitMetroTRK_BBA, TRKInitializeDispatcher,
  TRK_board_display, ReserveEXI2Port, UnreserveEXI2Port, TRKInitializeIntDrivenUART, MWTRACE, the 9 udp_cc_*
  (UDP_Stubs.c), and ddh_cc_ / gdev_cc_ initinterrupts, post_stop, pre_continue, close, shutdown, initialize (12);
  BfBB / Pikmin / SMS reproduce TRKPostInterruptEvent, SMS TRKValidMemory32. Left: 51 functions no reference
  reproduces (msghndlr.c's 15, support.c, serpoll.c, most of targimpl.c, dispatch.c, msg.c, nubinit.c, main_TRK.c,
  the ddh / gdev peek / read / write / open, InitMetroTRKCommTable) and 17 with no source at all (CircleBuffer.c,
  MWCriticalSection_gc.cpp, targsupp, ReadFPSCR / WriteFPSCR / TRKTargetAccessARAM, OutputData, the two ARAM
  functions).
- **MSL / Runtime.** No version string. Sizes equal XD 83/83 and TP 81/81 (TTYD / Pikmin 2 differ in parse_format,
  BfBB in 11). Compiled with GC/2.6 and the MSL flags, **BfBB's `src/PowerPC_EABI_Support/src`** reproduces 22 of
  the 25 functions MP4 / Melee do not: exit, free, __close_all, __num2dec_internal, __two_exp, fwrite, fseek,
  vprintf, strcat, __write_console, __ieee754_acos / asin / exp / log / pow / sqrt, exp, log, powf and the three
  critical_regions.gamecube functions. Left: _fseek (86%), ftell (46%), Runtime __sys_free (65%). printf.c's
  parse_format / long2str / longlong2str are exact in the probe (MP4's C): their 0% in CR_NOT_MATCHING is objdiff not
  pairing the local symbols (`parse_format_801651AC`), not the revision.
- **Which CodeWarrior release.** TRK, MSL and Runtime ship with CodeWarrior. The banner says v2.6 where KAR (2003)
  says v2.0, and the sizes put all three with the 2004-2006 games (XD, TP); with the probe's "MSL 2.5+" above this
  points to **CodeWarrior for GameCube 2.6**. That the banner number is the CodeWarrior release number is an
  inference, not proven; it does not separate the 2.5 / 2.6 / 2.7 compilers (same code here) either.

### HSD (sysdolphin) and HSD audio

"Custom Robo's HSD = Kirby Air Ride PAL build" holds for KAR's PAL-only edits but is **refuted as an identical
revision**:

- KAR's three builds (wowjinxy symbols.txt GKYE01 / GKYJ01 / GKYP01) have the same size for every HSD function we
  share but two PAL-only differences, and main.dol has the PAL form of both: `HSD_GXSetFifoObj` (0x18) exists only in
  GKYP01 and here (0x8019CD6C); `HSD_VIWaitXFBFlushNoYield` (0x16C) exists in GKYE01 / GKYJ01, not in GKYP01 nor
  here. HSD_InitComponent matched in its `VERSION_GKYP01` form (step 61).
- But 31 HSD functions named in both differ in size from GKYP01 (27 of the 405 in split HSD objects) (HSD_TObjAssignResources 0x260 vs 0x290,
  HSD_SetupTevStage 0xF8 vs 0x390, HSD_CObjSetCurrent 0x504 vs 0x574, HSD_CObjSetRoll 0x428 vs 0x3D8,
  HSD_ByteCodeEval 0x15C8 vs 0x15C0, the TEV-state functions, the billboard matrices, ...), smaller and larger
  mixed: a later HAL revision, not KAR's. Against doldecomp/kar's asm (`tools/refs/kar/asm`; call targets, address
  halves and small-data offsets masked on both sides): sysdolphin core 267 of 504 functions identical, 65 more equal
  but for li immediates (assert line numbers), 172 differ (42% / 19% / 38% of the bytes).
- HSD audio is the same family, axdriver least: identical + li-only functions axdriver 14 + 1 of 34 (76% of its
  bytes differ), devcom 5 + 4 of 16, synth 27 + 6 of 53, particle and the rest up to the core 32 + 3 of 68. KAR's asm
  stays the closest reference by far (Melee's C: 1 of 16), but most of the audio is written from our own asm.

### Verdict

| Library | Release identified | Evidence | Import to unblock | Stays blocked |
|---|---|---|---|---|
| Dolphin SDK | **yes**: Apr 17 2003 base, OS / GX Jul 23 2003, AX Jul 29 2003, DVD Sep 16 2003, THP Aug 27 2002 | 13 build strings; BfBB's sources carry the Apr 17 2003 strings; every probed function exact with BfBB / 2004 code | BfBB `src/dolphin/src/` os/OS.c, os/OSFont.c, os/OSReboot.c, os/OSReset.c, vi/vi.c, gx/GXInit.c (+1 line), gx/GXAttr.c, gx/GXGeometry.c, gx/GXLight.c, gx/GXTev.c, gx/GXPixel.c, ax/AXVPB.c, card/CARDBios.c, card/CARDMount.c, card/CARDRdwr.c, exi/EXIUart.c, thp/THPDec.c, thp/THPAudio.c (+ the 2004 functions BfBB lacks); the EXIBios `-O3,p` fix; GXFrameBuf's 5 modes. The 15 SDK objects blocked by code (57 functions) all reach 100% | __start.c (its InitMetroTRK_BBA stub: a link question, not a revision); GXFifo / GXMisc / OSError (edges, padding: unchanged) |
| MetroTRK | **partly**: v2.6, the CodeWarrior for GameCube 2.6 era; same sizes as XD / TP / TTYD / Pikmin 2 | banner; 160/160 sizes vs XD | KAR `src/metrotrk.c` (28 functions: UDP_Stubs.c, ddh / gdev main.c parts, MWTrace.c, dolphin_trk / glue parts), BfBB / SMS targimpl.c (2) | 51 + 17 functions: no v2.6 source in tools/refs. A TRK from the TP / Pikmin 2 / XD decomps (same sizes) would very likely be exact: worth pinning one |
| MSL / Runtime | **partly**: the library of XD / TP (sizes); CodeWarrior for GameCube 2.6 by the TRK banner | sizes 83/83 vs XD; BfBB's code reproduces 22 | BfBB `src/PowerPC_EABI_Support/src` (MSL_Common, MSL_Common_Embedded/Math, PPC_EABI) for the 22 functions | _fseek, ftell, __sys_free (a TP / XD source would decide) |
| HSD | **no exact release**: descends from KAR's PAL revision (both PAL-only edits), later than it | sizes vs KAR E / J / P; 267 + 65 of 504 identical with KAR's asm | nothing beyond step 61's KAR / Melee files; KAR's asm stays the template | ~172 core + ~80 audio / particle functions: written from our asm |
