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

## Source hunt (Oct 30, 2026)

For every library function that had no exact reference source after step 66: which public source compiles to
main.dol's bytes. Every claim below was compiled with the library's pinned compiler and flags ("Flags as applied"
above) plus the per-file variants the reference itself uses, and compared with the retail bytes by probe.py's masked
compare (same size, every word equal once call targets, address halves and small-data offsets are masked). "Exact"
means that; nothing here was built or linked (no build directory in this worktree). Plan step 67.

Rerun (each about a minute; outputs gitignored in `tools/compiler_probe/out/`):

- `python -I tools/compiler_probe/sources.py --fetch` makes sure the two references the library hunt needs have their
  sources: the pinned clones `tools/refs/tp` (zeldaret/tp **c8fa8c9**: `libs/TRK_MINNOW_DOLPHIN`,
  `libs/PowerPC_EABI_Support`, `libs/dolphin`, `include`) and `tools/refs/pikmin2` (doldecomp/pikmin2 **999140f**:
  `src/Dolphin`, `include`); since step 68 tools/setup.ps1 / setup.sh check these folders out (tools/pins.txt), and
  `--fetch` only widens an older clone (until step 67 they were separate clones in the gitignored `tools/refs_new/`).
  It writes `sources.txt`: the SDK sweep (every function in the SDK address ranges against every file of BfBB, dolsdk2004,
  dolsdk2001 and TP's SDK, by name, and by bytes for unnamed ones), MetroTRK and MSL / Runtime per function and per object
  (the references that reproduce every function of the object, with the extra flags they need).
- `python -I tools/compiler_probe/hsd_hunt.py` writes `hsd_hunt.txt` / `hsd_hunt.tsv`: every HSD function against our
  own C (decomp/src/sysdolphin), Melee's C (tools/melee 24e685d, plus the 7 sysdolphin files changed up to doldecomp/melee
  598987c, fetched raw into tools/refs_new/melee_latest), wowjinxy/KAR's C (main cb94b15; its older `testing` branch too,
  `git archive` into tools/refs_new/kar_wj_testing), doldecomp/kar's C and asm (3ed51a1), with a classified diff against
  the closest one.

### Result per library

| Library | Functions | Exact source now | New in this step | Still without an exact source |
|---|---|---|---|---|
| Dolphin SDK (all of 0x80003100-0x80003458, 0x80131C08-0x80159530, 0x801AEB40-, split or not) | 761 | 760: 676 compiled by name, 81 unnamed by bytes, 3 with a one-line edit (GXInit, dvd.c's two) | dvd.c's stateReadingFST / cbForStateCheckID2 (BfBB with `#line 650`); the sweep confirms step 66's list for every other function | 1: fn_80148998 (AXFX ReverbSTDCreate) |
| MetroTRK | 163 (162 split + `__TRK_reset` in .init) | **163** | 69: every function step 66 left (51 + 17), and fn_800053E0 | 0 |
| MSL + Runtime | 103 | **103** | _fseek, ftell (TP / Pikmin 2), __sys_free (a flag) | 0 |
| HSD (sysdolphin, audio, particle) | 678 | 392 (358 by our C, 34 new) | 34 (list below) | 286: 137 identical to KAR asm, 30 KAR asm but for assert lines, 37 small edits, 23 moderate, 18 rewrites, 41 with no reference |

### Dolphin SDK

The sweep compiles every .c of BfBB (`src/dolphin/src`), dolsdk2004, dolsdk2001 and Twilight Princess's SDK
(`libs/dolphin/src`) with GC/1.2.5n and the three SDK flag sets of revisions.py; each of our functions in the SDK ranges
is compared under its own name (duplicate static names such as the three `AlarmHandler` are each compared), and every
unnamed `fn_` against every compiled function of its size. Step 66's per-object table stands; in addition:

- **OSAlloc.c OSDestroyHeap / OSCheckHeap**: dolsdk2001 (what decomp/src already uses; dolsdk2004 is 43% / 91%).
- **dvd.c stateReadingFST (0x8013BE00), cbForStateCheckID2 (0x8013C8F8)** (DVD is the Sep 16 2003 build): BfBB's dvd.c
  with `#line 644` changed to `#line 650` (the OSHalt line of the "FST in the new disc is too big" message, inlined
  in both) compiles every one of our 45 dvd.c functions exact (tested; BfBB as it is: 97% / 98%, `li r4, 0x284` vs 0x28A).
- **GXInit**: step 66's BfBB + `__GXSetIndirectMask(0);` (BfBB as it is 99%).
- **fn_80148998** (0x47C, the first function of reverb_std.c = `ReverbSTDCreate` by position): dolsdk2004 63%, dolsdk2001
  49%, TP 47%; the same instructions with other registers (`rv` in r31 here, r23 in dolsdk2004) and the delay-line loop
  counters set up in another order; GC/1.2.5 / 1.2.5n / 1.3 / 1.3.2 / 2.0 / 2.6 with -O4,p / -O4,s / -O3,p / -O2 and
  char / inline / fp_contract variants tried, best 63%. No reference: write from our asm starting from dolsdk2004.
- 81 unnamed functions equal one compiled function byte for byte (68 to exactly one name: the THP / AXFX / Pad / CARD /
  texPalette names of step 66 and more; 13 to several names, e.g. fn_80153070 = `GXGetCPUFifo` or one of three other
  one-line getters). Every other SDK function therefore has C; the THP / AXFX / Pad files are BfBB `thp/`, dolsdk2004
  `axfx/`, `pad/`.

### MetroTRK v2.6: Twilight Princess / Pikmin 2

**TP's and Pikmin 2's TRK is C (not asm-only) and is v2.6's**: compiled with GC/2.6 and our MetroTRK flags they
reproduce all 162 TRK functions (targsupp is assembler in both: four `twi 31, r0, 0; blr` stubs, equal to ours).
Per object, import from TP (`tools/refs/tp/`, zeldaret/tp c8fa8c9; Pikmin 2's `src/Dolphin/TRK_MINNOW_DOLPHIN/`
is equal wherever TP is, and is the only exact one for targimpl.c):

| Object | Functions | Exact references (file; extra flags) | Import from |
|---|---|---|---|
| MetroTRK/mem_TRK.c | 3 | mp4 (mem_TRK.c), melee (mem_TRK.c), ac (mem_TRK.c), pikmin (mem_TRK.c), bfbb (mem_TRK.c), pikmin2 (mem_TRK.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mem_TRK.c` |
| fn_800053E0 (.init, automatic object) | 1 | tp (dolphin_trk.c), pikmin2 (dolphin_trk.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk.c`: `__TRK_reset` = `OSResetSystem(FALSE, 0, FALSE)` (MP4 / Melee / AC: `__TRK_copy_vectors()`, 8%) |
| MetroTRK/mainloop.c | 1 | mp4 (mainloop.c), sms (mainloop.c), tp (mainloop.c), pikmin2 (mainloop.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mainloop.c` |
| MetroTRK/nubevent.c | 5 | mp4 (nubevent.c), sms (nubevent.c), tp (nubevent.c), pikmin2 (nubevent.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/nubevent.c` |
| MetroTRK/nubinit.c | 3 | tp (nubinit.c), pikmin2 (nubinit.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/nubinit.c` |
| MetroTRK/msg.c | 1 | tp (msg.c), pikmin2 (msg.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msg.c` |
| MetroTRK/msgbuf.c | 14 | tp (msgbuf.c), pikmin2 (msgbuf.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msgbuf.c` |
| MetroTRK/serpoll.c | 5 | tp (serpoll.c), pikmin2 (serpoll.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/serpoll.c` |
| MetroTRK/usr_put.c | 2 | mp4 (usr_put.c), ac (usr_put.c), tp (usr_put.c), pikmin2 (usr_put.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/usr_put.c` |
| MetroTRK/dispatch.c | 2 | tp (dispatch.c), pikmin2 (dispatch.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/dispatch.c` |
| MetroTRK/msghndlr.c | 17 | tp (msghndlr.c), pikmin2 (msghndlr.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/msghndlr.c` |
| MetroTRK/support.c | 5 | tp (support.c), pikmin2 (support.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/support.c` |
| MetroTRK/mutex_TRK.c | 3 | mp4 (mutex_TRK.c), melee (mutex_TRK.c), ac (mutex_TRK.c), pikmin (mutex_TRK.c), sms (mutex_TRK.c), bfbb (mutex_TRK.c)... | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/mutex_TRK.c` |
| MetroTRK/notify.c | 1 | tp (notify.c), pikmin2 (notify.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/notify.c` |
| MetroTRK/flush_cache.c | 1 | mp4 (flush_cache.c), melee (flush_cache.c), ac (flush_cache.c), pikmin (flush_cache.c), sms (flush_cache.c), bfbb (fl... | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/flush_cache.c` |
| MetroTRK/targimpl.c | 29 | pikmin2 (targimpl.c) | pikmin2 `src/Dolphin/TRK_MINNOW_DOLPHIN/targimpl.c` (the only file exact for all 29: ReadFPSCR, WriteFPSCR, TRKTargetAccessARAM, TRKTargetSupportRequest ...) |
| MetroTRK/targsupp.c | 4 | tp/pikmin2 targsupp.s | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Export/targsupp.s` |
| MetroTRK/mpc_7xx_603e.c | 2 | mp4 (mpc_7xx_603e.c), melee (mpc_7xx_603e.c), ac (mpc_7xx_603e.c), pikmin (mpc_7xx_603e.c), sms (mpc_7xx_603e.c), bfb... | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Processor/ppc/Generic/mpc_7xx_603e.c` |
| MetroTRK/dolphin_trk.c | 8 | tp / pikmin2 for 7 of 8, mp4 / melee / ac / bfbb / kar for InitMetroTRK | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk.c` with InitMetroTRK from MP4 / BfBB (TP / Pikmin 2: 0x98, ours and XD 0x94); fn_8015E09C = TRK__write_aram, fn_8015E288 = TRK__read_aram (exact under those names) |
| MetroTRK/main_TRK.c | 1 | tp (main_TRK.c), pikmin2 (main_TRK.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/main_TRK.c` |
| MetroTRK/dolphin_trk_glue.c | 13 | tp (dolphin_trk_glue.c), pikmin2 (dolphin_trk_glue.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/dolphin_trk_glue.c` |
| MetroTRK/targcont.c | 1 | mp4 (targcont.c), melee (targcont.c), ac (targcont.c), pikmin (targcont.c), sms (targcont.c), bfbb (targcont.c), kar ... | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/targcont.c` |
| MetroTRK/target_options.c | 2 | mp4 (target_options.c), ac (target_options.c), tp (target_options.c), pikmin2 (target_options.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/target_options.c` |
| MetroTRK/mslsupp.c | 2 | mp4 (mslsupp.c), ac (mslsupp.c), tp (mslsupp.c), pikmin2 (mslsupp.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Export/mslsupp.c` |
| MetroTRK/UDP_Stubs.c | 9 | kar (metrotrk.c), tp (UDP_Stubs.c), pikmin2 (UDP_Stubs.c) | tp `libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Os/dolphin/UDP_Stubs.c` |
| MetroTRK/ddh/main.c | 10 | tp (main.c; -sdata 8), pikmin2 (main.c; -sdata 8) | tp `libs/TRK_MINNOW_DOLPHIN/gamedev/cust_connection/cc/exi2/GCN/EXI2_DDH_GCN/main.c`, with `-sdata 8` (CR_OBJECT_CFLAGS; TP / Pikmin 2 set it for this file) |
| MetroTRK/CircleBuffer.c | 4 | tp (CircleBuffer.c), pikmin2 (CircleBuffer.c) | tp `libs/TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/common/CircleBuffer.c` |
| MetroTRK/gdev/main.c | 10 | tp (main.c; -sdata 8), pikmin2 (main.c; -sdata 8) | tp `libs/TRK_MINNOW_DOLPHIN/gamedev/cust_connection/cc/exi2/GCN/EXI2_GDEV_GCN/main.c`, with `-sdata 8` (CR_OBJECT_CFLAGS; TP / Pikmin 2 set it for this file) |
| MetroTRK/MWTrace.c | 1 | kar (metrotrk.c), tp (MWTrace.c), pikmin2 (MWTrace.c) | tp `libs/TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/common/MWTrace.c` |
| MetroTRK/MWCriticalSection_gc.cpp | 3 | tp (MWCriticalSection_gc.c), pikmin2 (MWCriticalSection_gc.cpp) | tp `libs/TRK_MINNOW_DOLPHIN/gamedev/cust_connection/utils/gc/MWCriticalSection_gc.c` |

The 12 TRK objects in CR_NOT_MATCHING (dispatch, dolphin_trk, dolphin_trk_glue, main_TRK, msg, msgbuf, msghndlr,
notify, nubinit, serpoll, support, targimpl) and the 7 not imported (UDP_Stubs, ddh / gdev main.c, CircleBuffer, MWTrace,
MWCriticalSection_gc, targsupp) all become exact. The header roots are TP's (`include`, `libs/dolphin/include`,
`libs/PowerPC_EABI_Support/...`, `-DVERSION=0`), so the import needs those headers next to the MP4 ones (CR_OBJECT_INCLUDES).

Applied in step 68 (plan.md): all 29 MetroTRK objects are linked from C as this table says; two findings on the way:
InitMetroTRK is 0x98 here too (TP's file as it is: the `blr` after its last branch is part of it; dtk's signature size
0x94 left it out), and serpoll.c needs TP's `-sdata 8` as well (gTRKInputPendingPtr is in .sbss). The 0x84
`__close_console` before mslsupp.c's two functions is Pikmin 2's mslsupp.c function (TP has none there), so mslsupp.c
is Pikmin 2's file.

### MSL and Runtime

All 103 functions have an exact source. For the objects not linked yet (CR_NOT_MATCHING) and the files not imported:

| Object (functions) | Exact references | Import from |
|---|---|---|
| MSL/FILE_POS.c (3) | tp (FILE_POS.c), pikmin2 (FILE_POS.C) | tp `libs/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/FILE_POS.c` (_fseek, ftell, fseek: only TP / Pikmin 2) |
| MSL/abort_exit.c (1) | bfbb (abort_exit.c), kar (runtime_libc.c), tp (abort_exit.c), pikmin2 (abort_exit.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/alloc.c (4) | bfbb (alloc.c), tp (alloc.c), pikmin2 (alloc.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/ansi_files.c (2) | bfbb (ansi_files.c), tp (ansi_files.c), pikmin2 (ansi_files.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/ansi_fp.c (4) | bfbb (ansi_fp.c), tp (ansi_fp.c), pikmin2 (ansi_fp.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/direct_io.c (2) | bfbb (direct_io.c), tp (direct_io.c), pikmin2 (direct_io.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/e_acos.c (1) | bfbb (e_acos.c), tp (e_acos.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/e_asin.c (1) | bfbb (e_asin.c), tp (e_asin.c), pikmin2 (e_asin.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/e_pow.c (1) | bfbb (e_pow.c), tp (e_pow.c), pikmin2 (e_pow.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/printf.c (11) | pikmin2 (printf.c) | pikmin2 `src/Dolphin/MSL_C/MSL_Common/printf.c` (the only file exact for all 11; MP4's 7 + BfBB's vprintf also work; the 0% of parse_format / long2str / longlong2str in objdiff is its local-symbol pairing, step 66) |
| MSL/string.c (7) | ac (string.c), bfbb (string.c), tp (string.c), pikmin2 (string.c) | bfbb `src/PowerPC_EABI_Support/src/MSL_C/MSL_Common/string.c` or tp `.../MSL_Common/Src/string.c` (strcat) |
| Runtime/GCN_mem_alloc.c (1) | mp4 (GCN_Mem_Alloc.c; runtime -str reuse,nopool,readonly), bfbb (GCN_mem_alloc.c; runtime -str reuse,nopool,readonly), tp (GCN_mem_alloc.... | the C already in decomp/src (MP4's) with `-str reuse,nopool,readonly` (CR_OBJECT_CFLAGS, as TP's configure.py): __sys_free exact; with our Runtime flags 98% |
| MSL/e_exp.c (1) | bfbb (e_exp.c), tp (e_exp.c), pikmin2 (e_exp.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/e_log.c (1) | bfbb (e_log.c), tp (e_log.c), pikmin2 (e_log.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/e_sqrt.c (1) | bfbb (e_sqrt.c), tp (e_sqrt.c), pikmin2 (e_sqrt.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/w_exp.c (1) | bfbb (w_exp.c), tp (w_exp.c), pikmin2 (w_exp.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/w_log.c (1) | bfbb (w_log.c) | bfbb `src/PowerPC_EABI_Support/src/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_log.c` (only BfBB has it) |
| MSL/exponentialsf.c (1) | bfbb (math_ppc.c), tp (math_double.c) | bfbb `src/PowerPC_EABI_Support/src/MSL_C/PPC_EABI/math_ppc.c` (powf) or tp `.../MSL_Common/Src/math_double.c` |
| MSL/PPC_EABI/critical_regions.gamecube.c (3) | bfbb (critical_regions.gamecube.c), kar (runtime_libc.c), tp (critical_regions.gamecube.c), pikmin2 (critical_regions.gamecube.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |
| MSL/uart_console_io_gcn.c (1) | bfbb (uart_console_io_gcn.c), tp (uart_console_io_gcn.c), pikmin2 (uart_console_io_gcn.c) | bfbb `src/PowerPC_EABI_Support/src/` (step 66) or tp `libs/PowerPC_EABI_Support/` (same file name) |

### HSD (sysdolphin, HSD audio, particle)

No public HSD source is this revision. Checked: tools/melee (24e685d) and doldecomp/melee's newest sysdolphin (598987c: 7
files changed since, only .c files; no function changes its score), doldecomp/kar (3ed51a1 = its head), wowjinxy/KAR main
(cb94b15 = its head; branch `long-auto-test` has the same sysdolphin, `testing` (066a737) is older: no function exact
there that main lacks; `master` and the `agent/*` branches have less), HSDLib (file-format structs only: particle
groups, FOBJ; no runtime structs such as the voice / particle records the diffs below are about). KAR stays the closest by
far; Melee's C is the closest for only a handful.

Per part (functions; exact C; how far the rest is from the closest reference):

| Part | Functions | Exact C (new) | KAR asm identical, no C | assert lines only | small edit (>=90% aligned) | moderate (70-90%) | rewrite (<70%) | no reference |
|---|---|---|---|---|---|---|---|---|
| axdriver | 34 | 0 (0) | 14 | 1 | 6 | 5 | 2 | 6 |
| devcom | 16 | 2 (2) | 5 | 4 | 0 | 0 | 4 | 1 |
| synth | 53 | 1 (1) | 26 | 6 | 4 | 1 | 3 | 12 |
| particle+ | 68 | 0 (0) | 32 | 3 | 14 | 5 | 4 | 10 |
| core | 507 | 389 (31) | 60 | 16 | 13 | 12 | 5 | 12 |

Address ranges: axdriver 0x8016D42C-0x80170ECC, devcom -0x80172DD8, synth -0x80179C14, particle and the rest up to the
core -0x80186490, core -0x801AEB40. KAR asm partners: by name (our name = the name in wowjinxy's GKYE01 map at that
KAR address); for our unnamed functions, the KAR function with equal masked bytes ("bytes") or else the most similar
one in the same file / size band ("similar"; under 50% aligned it counts as no reference). KAR names quoted here come
from wowjinxy's map and include its invented `kar_*` names: they locate the code, they are not names for us.

**New exact C (34)**, not in our C yet (every exact reference listed; `wowjinxy` = tools/refs/kar_wowjinxy/src/sysdolphin,
`melee` = tools/melee/src/sysdolphin/baselib, `kar` = tools/refs/kar/src/sysdolphin):

| Function | Address | Size | Our object | Exact in |
|---|---|---|---|---|
| HSD_DevComDVDStdCallback | 801715C4 | 0x78 | (unsplit devcom) | wowjinxy:devcom.c |
| HSD_DevComDVDMemCallback | 801717DC | 0x198 | (unsplit devcom) | wowjinxy:devcom.c |
| HSD_SynthSFXLoadNewProc | 801733F0 | 0x7C | (unsplit synth) | melee:synth.c, wowjinxy:dvdsche.c |
| LObjInfoInit | 8018F4C8 | 0x80 | lobj.c | wowjinxy-testing:lobj.c, wowjinxy:lobj.c |
| interpretShapeAnimDisplayList | 80194BD8 | 0x324 | pobj.c | melee:pobj.c, melee_latest:pobj.c |
| PObjInfoInit | 801963A4 | 0x98 | pobj.c | wowjinxy-testing:pobj.c, wowjinxy:pobj.c |
| HSD_ObjSetHeap | 801A0568 | 0x1C | objalloc.c | melee:objalloc.c, wowjinxy-testing:util.c, wowjinxy:util.c |
| HSD_ObjAlloc | 801A07D4 | 0x150 | objalloc.c | wowjinxy-testing:objalloc.c, wowjinxy:objalloc.c |
| HSD_ObjFree | 801A0924 | 0x28 | objalloc.c | melee:objalloc.c, wowjinxy-testing:objalloc.c, wowjinxy:objalloc.c |
| _HSD_MemAlloc | 801A4060 | 0x2C | (unsplit core) | wowjinxy-testing:memory.c, wowjinxy:memory.c |
| HSD_Free | 801A40EC | 0x30 | (unsplit core) | wowjinxy-testing:memory.c, wowjinxy:memory.c |
| HSD_Alloc | 801A411C | 0x38 | (unsplit core) | wowjinxy-testing:memory.c, wowjinxy:memory.c |
| HSD_ShadowInitAllocData | 801A4154 | 0x30 | (unsplit core) | kar:shadow.c, melee:shadow.c, wowjinxy-testing:memory.c, wowjinxy:memory.c |
| HSD_ShadowAlloc | 801A4184 | 0xFC | (unsplit core) | kar:shadow.c, melee:shadow.c, wowjinxy-testing:memory.c, wowjinxy:memory.c |
| HSD_ShadowRemove | 801A4280 | 0x108 | (unsplit core) | melee:shadow.c, wowjinxy-testing:memory.c, wowjinxy:memory.c |
| HSD_ShadowInit | 801A4388 | 0x70 | (unsplit core) | kar:shadow.c, melee:shadow.c, wowjinxy-testing:shadow.c, wowjinxy:shadow.c |
| HSD_ShadowSetSize | 801A43F8 | 0x160 | (unsplit core) | kar:shadow.c, melee:shadow.c, wowjinxy-testing:shadow.c, wowjinxy:shadow.c |
| HSD_ShadowAddObject | 801A4C84 | 0xAC | (unsplit core) | kar:shadow.c, melee:shadow.c, wowjinxy-testing:shadow.c, wowjinxy:shadow.c |
| HSD_ShadowDeleteObject | 801A4D30 | 0x94 | (unsplit core) | kar:shadow.c, melee:shadow.c, wowjinxy-testing:shadow.c, wowjinxy:shadow.c |
| HSD_ViewingRectCheck | 801A52CC | 0x7C | (unsplit core) | wowjinxy-testing:shadow.c, wowjinxy:shadow.c |
| HSD_ViewingRectAddRect | 801A5348 | 0x210 | (unsplit core) | wowjinxy-testing:shadow.c, wowjinxy:shadow.c |
| HSD_ArchiveGetPublicAddress | 801A5558 | 0xA4 | (unsplit core) | kar:archive.c, melee:archive.c, wowjinxy-testing:archive.c, wowjinxy:archive.c |
| HSD_ArchiveGetExtern | 801A55FC | 0x38 | (unsplit core) | kar:archive.c, melee:archive.c, wowjinxy-testing:archive.c, wowjinxy:archive.c |
| HSD_TExpRef | 801A7E20 | 0x88 | texp.c | wowjinxy-testing:texpdag.c, wowjinxy:texpdag.c |
| HSD_TExpFreeList | 801A8424 | 0x320 | texp.c | wowjinxy-testing:texpdag.c, wowjinxy:texpdag.c |
| HSD_TExpCnst | 801A8804 | 0x120 | texp.c | wowjinxy-testing:texpdag.c, wowjinxy:texpdag.c |
| HSD_TExpColorOp | 801A8924 | 0xE8 | texp.c | wowjinxy-testing:texpdag.c, wowjinxy:texpdag.c |
| HSD_TExpAlphaOp | 801A8A0C | 0xE8 | texp.c | wowjinxy-testing:texpdag.c, wowjinxy:texpdag.c |
| AssignColorReg | 801A9778 | 0x178 | texp.c | wowjinxy-testing:texpdag.c, wowjinxy:texpdag.c |
| AssignColorKonst | 801A98F0 | 0x22C | texp.c | wowjinxy-testing:texpdag.c, wowjinxy:texpdag.c |
| AssignAlphaKonst | 801A9B1C | 0x164 | texp.c | wowjinxy-testing:texpdag.c, wowjinxy:texpdag.c |
| HSD_SaveContext | 801AE9B4 | 0x98 | (unsplit core) | wowjinxy-testing:gobjproc.c, wowjinxy:gobjproc.c |
| __assert | 801AEA4C | 0x5C | (unsplit core) | wowjinxy-testing:gobjproc.c, wowjinxy:gobjproc.c |
| HSD_Panic | 801AEAA8 | 0x98 | (unsplit core) | wowjinxy-testing:gobjproc.c, wowjinxy:gobjproc.c |

**KAR asm equal, no C anywhere** (C to write to KAR's asm: the instructions are the same) and **KAR asm equal but for
assert line numbers** (li immediates; the C needs our line numbers):

| Our object | Kind | Count | Functions (address; = KAR function where ours is unnamed) |
|---|---|---|---|
| (unsplit axdriver) | asm identical | 14 | fn_8016D48C (8016D48C = KAR fn_80347C40), fn_8016F294 (8016F294 = KAR kar_axdriver_dispatch_play_sequence_voice), fn_8016FA5C (8016FA5C = KAR kar_axdriver_bind_sound_archive), fn_8016FBC8 (8016FBC8 = KAR kar_axdriver_set_channel_mix_override_enable), fn_8016FF44 (8016FF44 = KAR kar_axdriver_calc_effect_work_size), fn_8017004C (8017004C = KAR kar_axdriver_configure_output_effect), fn_801700BC (801700BC = KAR kar_axdriver_init_effect_params), fn_80170708 (80170708 = KAR kar_axdriver_play_file_in_slot), fn_80170C70 (80170C70 = KAR kar_axdriver_stop_slot), fn_80170CD8 (80170CD8 = KAR kar_axdriver_set_slot_volume_ramp), fn_80170D4C (80170D4C = KAR kar_axdriver_set_backend_master_mix), fn_80170D7C (80170D7C = KAR kar_axdriver_pause_all_slots), fn_80170DEC (80170DEC = KAR kar_axdriver_pause_all_slots), fn_80170EAC (80170EAC = KAR kar_a2d_bg8000__near_80343a74) |
| (unsplit axdriver) | assert lines only | 1 | AXDriverSetupAux (8016FC38) |
| (unsplit devcom) | asm identical | 5 | HSD_DevComDVDWakeUp (80171C18), HSD_DevComRequest (80171EC4), fn_80172134 (80172134 = KAR kar_dvdsche_configure_stream_scheduler), fn_8017277C (8017277C = KAR kar_dvdsche_dispatch_stream_scheduler_callbacks), fn_80172DCC (80172DCC = KAR fn_803EC99C) |
| (unsplit devcom) | assert lines only | 4 | fn_80172178 (80172178 = KAR kar_dvdsche_alloc_stream_scheduler_slot), fn_801723E4 (801723E4 = KAR kar_dvdsche_free_stream_scheduler_slot), fn_801725D0 (801725D0 = KAR kar_dvdsche_set_stream_scheduler_slot_key), fn_801726B0 (801726B0 = KAR kar_dvdsche_set_stream_scheduler_slot_callback) |
| (unsplit synth) | asm identical | 26 | HSD_AudioMalloc (80172DD8), fn_801735EC (801735EC = KAR HSD_SynthSFXGetPendingLoadCount), fn_801735FC (801735FC = KAR HSD_SynthSFXNewBank), fn_80173654 (80173654 = KAR HSD_SynthSFXBankClear), fn_80173774 (80173774 = KAR HSD_SynthVolToDBx), fn_801737E4 (801737E4 = KAR HSD_SynthDBxToVol), fn_80173850 (80173850 = KAR HSD_SynthSetSoundMode), fn_80173F64 (80173F64 = KAR HSD_SynthSFXStart), fn_801746A4 (801746A4 = KAR HSD_SynthSFXKeyOff), fn_80174928 (80174928 = KAR HSD_SynthSFXStopRange), fn_801749B4 (801749B4 = KAR HSD_SynthSFXStopBank), fn_801749F4 (801749F4 = KAR HSD_SynthSFXPause), fn_80174A9C (80174A9C = KAR HSD_SynthSFXResume), fn_80174B40 (80174B40 = KAR HSD_SynthSFXCheck), fn_80174FBC (80174FBC = KAR HSD_SynthSFXSetMix), fn_801753A0 (801753A0 = KAR HSD_SynthSFXSetPriority), fn_80175E10 (80175E10 = KAR HSD_SynthSFXUpdateVolume), fn_80175E6C (80175E6C = KAR HSD_SynthSFXMixEnvelope), fn_801760BC (801760BC = KAR HSD_SynthSFXMixShort), fn_80176250 (80176250 = KAR HSD_SynthSFXInitMix), fn_801772E0 (801772E0 = KAR HSD_SynthSFXUpdateAllVolume), fn_8017755C (8017755C = KAR HSD_SynthSFXSetMasterVolume), fn_8017776C (8017776C = KAR fn_803ED76C), fn_80177774 (80177774 = KAR fn_803ED76C), fn_8017777C (8017777C = KAR fn_803ED76C), fn_8017933C (8017933C = KAR HSD_SynthPStreamStart) |
| (unsplit synth) | assert lines only | 6 | fn_80174BFC (80174BFC = KAR HSD_SynthSetVolFader), fn_80174E1C (80174E1C = KAR HSD_SynthSFXSetPan), fn_80174EF4 (80174EF4 = KAR HSD_SynthSFXSetSPan), fn_8017506C (8017506C = KAR HSD_SynthSFXLinkUpdateRearPitch), fn_801750DC (801750DC = KAR HSD_SynthSFXUpdatePitch), HSD_SynthSFXSetPitchRatio (801752AC) |
| (unsplit particle+) | asm identical | 32 | fn_80179C14 (80179C14 = KAR kar_particle__near_8042a874), fn_8017A638 (8017A638 = KAR psGenerateParticle), fn_8017A96C (8017A96C = KAR psDeletePntJObjwithParticle), fn_8017A9C8 (8017A9C8 = KAR psKillGeneratorChild), fn_8017AADC (8017AADC = KAR kar_particle__near_8042ba68), fn_8017AC4C (8017AC4C = KAR kar_particle__near_8042bbd8), fn_8017AC84 (8017AC84 = KAR kar_particle__near_8042bc10), fn_8017B1B8 (8017B1B8 = KAR kar_particle__near_8042c9fc), fn_8017B360 (8017B360 = KAR kar_particle__near_8042cba4), fn_8017E3D4 (8017E3D4 = KAR kar_particle__near_8042fe34), fn_8017E484 (8017E484 = KAR kar_particle__near_8042fee4), fn_8017E534 (8017E534 = KAR kar_particle__near_8042ff94), fn_8017E5E4 (8017E5E4 = KAR HSD_JObjSetupMatrix), fn_8017E654 (8017E654 = KAR fn_803BD490), fn_8017E6B8 (8017E6B8 = KAR kar_particle__near_804300b4), fn_8017E710 (8017E710 = KAR kar_a2d_bg8000__near_80343a74), fn_8017E730 (8017E730 = KAR kar_particle__near_8043012c), fn_8017E794 (8017E794 = KAR kar_particle__near_80430190), fn_8017E874 (8017E874 = KAR kar_psinterpret__near_80430270), fn_8017F1AC (8017F1AC = KAR HSD_TObjGetNext), fn_8017F1C4 (8017F1C4 = KAR kar_gmlanmenu__near_80053758), fn_8017F1DC (8017F1DC = KAR kar_psinterpret__near_8043070c), fn_80180AC8 (80180AC8 = KAR kar_generator__near_804328a0), fn_801812A4 (801812A4 = KAR kar_generator__near_80433098), fn_80181360 (80181360 = KAR kar_generator__near_80433154), fn_80184B44 (80184B44 = KAR kar_psdisp__near_80436d4c), fn_80184F60 (80184F60 = KAR kar_psdisp__near_80437168), fn_801851D4 (801851D4 = KAR kar_psdisp__near_804373dc), fn_80185304 (80185304 = KAR kar_lbkdcoll__near_80076a84), fn_80185310 (80185310 = KAR kar_psdisp__near_80437518), fn_801859E8 (801859E8 = KAR kar_psdisp__near_80437bf0), fn_80186010 (80186010 = KAR _psListDelete) |
| (unsplit particle+) | assert lines only | 3 | psInitDataBankLoad (80179F88), fn_80185E28 (80185E28 = KAR kar_psdisp__near_80438084), fn_80185F34 (80185F34 = KAR kar_psdisp__near_80438238) |
| state.c | asm identical | 1 | HSD_SetupChannelMode (80189C88) |
| tev.c | asm identical | 1 | HSD_DisableChannelLighting (8018AB28) |
| tev.c | assert lines only | 1 | fn_8018AFE0 (8018AFE0 = KAR HSD_Channel2Num) |
| lobj.c | asm identical | 1 | fn_8018E604 (8018E604 = KAR HSD_LObjSetCurrentAll) |
| cobj.c | asm identical | 9 | HSD_CObjEraseScreen (8018F548), fn_8018F830 (8018F830 = KAR setupTopHalfCamera), fn_8018FA94 (8018FA94 = KAR setupBottomHalfCamera), fn_80190EB8 (80190EB8 = KAR HSD_CObjSetMtxDirty), fn_80191424 (80191424 = KAR HSD_CObjSetAspect), fn_80191494 (80191494 = KAR HSD_CObjSetScissor), fn_801914B0 (801914B0 = KAR HSD_CObjSetScissorx4), fn_801914CC (801914CC = KAR HSD_CObjSetViewportf), fn_801914F8 (801914F8 = KAR HSD_CObjSetViewportfx4) |
| cobj.c | assert lines only | 1 | CObjInfoInit (80191B60) |
| fobj.c | assert lines only | 2 | FObjLoadData (80191EA4), HSD_FObjInterpretAnim (80192F74) |
| pobj.c | assert lines only | 6 | PObjLoad (80193B14), HSD_PObjResolveRefsAll (80193EB8), get_shape_vertex_xyz (801940B8), get_shape_normal_xyz (80194444), get_shape_nbt_xyz (801947D0), HSD_PObjDisp (80195EE4) |
| pobj.c | asm identical | 2 | SetupSharedVtxModelMtx (80195650), PObjRelease (801961CC) |
| jobj.c | asm identical | 15 | fn_80196568 (80196568 = KAR HSD_JObjMtxIsDirty), HSD_JObjSetMtxDirty (801965C0), HSD_JObjReqAnimAllByFlags (80196988), fn_80196C50 (80196C50 = KAR HSD_JObjRemoveAnimAll), fn_80198014 (80198014 = KAR setupInstanceMtx), fn_80198144 (80198144 = KAR HSD_JObjDispAll), fn_80198690 (80198690 = KAR HSD_JObjSetupMatrix), HSD_IDGetData (80198DC4), ref_INC (80198DF0), fn_80199178 (80199178 = KAR HSD_JObjGetDObj), fn_801991A4 (801991A4 = KAR HSD_JObjGetFlags), HSD_JObjSetFlagsAll (80199258), fn_801995CC (801995CC = KAR HSD_JObjClearFlagsAll), HSD_JObjSetMtxDirtySub (8019AA60), JObjInfoInit (8019B3D8) |
| jobj.c | assert lines only | 3 | HSD_JObjResolveRefsAll (80198A9C), resolveIKJoint1 (80199A9C), resolveIKJoint2 (8019A28C) |
| displayfunc.c | asm identical | 2 | fn_8019C918 (8019C918 = KAR fn_803ED76C), HSD_EraseRect (8019C920) |
| mtx.c | asm identical | 2 | fn_8019F2AC (8019F2AC = KAR HSD_MtxGetRotationMtx), HSD_VecGetOrthogonal (801A015C) |
| objalloc.c | asm identical | 2 | HSD_ObjAllocAddFree (801A0584), HSD_ObjAllocInit (801A094C) |
| robj.c | asm identical | 5 | HSD_RObjGetGlobalPosition (801A0CCC), resolveCnsDirUp (801A0E80), HSD_RObjUpdateAll (801A1670), HSD_RObjResolveRefsAll (801A1AF4), HSD_RvalueResolveRefsAll (801A28A0) |
| (unsplit core) | asm identical | 8 | fn_801A3C9C (801A3C9C = KAR HSD_PerfCountEnvelopeBlending), fn_801A3FEC (801A3FEC = KAR _HSD_MemSetCallbacks), fn_801A408C (801A408C = KAR UnreserveEXI2Port), fn_801A40BC (801A40BC = KAR _HSD_MemCheckOwn), fn_801A5A48 (801A5A48 = KAR HSD_ArchiveFree), fn_801A5AE8 (801A5AE8 = KAR kar_a2d_bg8000__near_80343a74), fn_801A5E7C (801A5E7C = KAR HSD_Randf), fn_801A5ED4 (801A5ED4 = KAR HSD_Randi) |
| hash.c | asm identical | 1 | HSD_HashSearch (801A7CE4) |
| texp.c | asm identical | 3 | HSD_TExpUnref (801A7EA8), HSD_TExpColorInSub (801A8AF4), HSD_TExpAlphaInSub (801A91B4) |
| texp.c | assert lines only | 3 | TExpAssignReg (801A9C80), TExp2TevDesc (801AA34C), HSD_TExpSetReg (801AA74C) |
| texpdag.c | asm identical | 8 | assign_reg (801AAF9C), CalcDistance (801AB384), HSD_TExpMakeDag (801AB7BC), SimplifySrc (801ABFD4), SimplifyThis (801AC46C), SimplifyByMerge (801ACCAC), CalcTevRange (801AE474), HSD_TExpSimplify (801AE728) |

**The rest (119)**: closest reference, kinds of difference against it and the share of aligned instructions, then
what to do. Kinds: `lines` li immediates (assert lines), `regs` register numbers only (allocation: declaration /
expression order), `offsets` same load / store with another displacement (struct layout; first pairs shown as
reference->ours), `consts` other immediates (constants, flag masks, stack sizes), `+check` / `-check` compare-and-branch
only in ours / only in the reference (an added / dropped NULL or range check), `+call` / `-call` calls only in ours /
the reference (helper not inlined / inlined), `branches` branch displacements (follow from inserted code), `+N` / `-N`
other instructions only in ours / the reference, `rewrite` under half aligned. "start from" = the best C over 50%.

| Function | Address | Size | Our object | Closest reference | Difference | Suggested edit |
|---|---|---|---|---|---|---|
| AXDriverAlloc | 8016D42C | 0x60 | (unsplit axdriver) | melee:axdriver.c | lines regs(4) 79% C size 0x60; kar asm 92% lines -N | the C, then: assert lines, declaration / expression order (register allocation) |
| fn_8016D490 | 8016D490 | 0x14C | (unsplit axdriver) | KAR asm kar_axdriver_stop_voice 80440844 (similar) | lines other 98%  | C written from the KAR asm, then: assert lines, code changes |
| fn_8016D5DC | 8016D5DC | 0x4C4 | (unsplit axdriver) | KAR asm kar_axdriver_try_start_hardware_voice 80440CD4 (similar) | lines(2) offsets(20) consts other(2) 92% ld +0x1C->+0x1B imm 0xBFC->0xDFC ld +0x30->+0x2E ld +0x3A->+ | C written from the KAR asm, then: assert lines, struct layout (field offsets), constants / flag masks, code changes |
| fn_8016DAA0 | 8016DAA0 | 0x590 | (unsplit axdriver) | KAR asm kar_axdriver_apply_voice_pending_flags 80441198 (similar) | lines(3) offsets(14) branches(13) +N(2) -N(16) 87% ld +0x24->+0x22 ld +0x26->+0x24 ld +0x20->+0x1F st +0x18->+0 | C written from the KAR asm, then: assert lines, struct layout (field offsets), code changes |
| fn_8016E030 | 8016E030 | 0xB4C | (unsplit axdriver) | - | no reference 0%  | write from our asm |
| fn_8016EB7C | 8016EB7C | 0x208 | (unsplit axdriver) | - | no reference 20%  | write from our asm |
| fn_8016ED84 | 8016ED84 | 0xD4 | (unsplit axdriver) | - | no reference 43%  | write from our asm |
| fn_8016EE58 | 8016EE58 | 0xA4 | (unsplit axdriver) | - | no reference 49%  | write from our asm |
| HSD_AudioSFXStartParam | 8016EEFC | 0x398 | (unsplit axdriver) | KAR asm kar_axdriver_play_sequence_voice 80442674 (similar) | lines(3) offsets(7) consts +N -N(2) other 85% st +0x44->+0x40 st +0x1C->+0x1B ld +0x1C->+0x1B st +0x3D->+0 | C written from the KAR asm, then: assert lines, struct layout (field offsets), constants / flag masks, code changes |
| fn_8016F2D8 | 8016F2D8 | 0x164 | (unsplit axdriver) | - | no reference 34%  | write from our asm |
| fn_8016F43C | 8016F43C | 0x158 | (unsplit axdriver) | KAR asm kar_axdriver_set_voice_pitch_bend 80443398 (similar) | lines regs(14) offsets consts +call(2) +N(3) -N(22) other(2) 51% st +0xFFE0->+0xFFD0 imm 0x20->0x30 | C written from the KAR asm, then: assert lines, declaration / expression order (register allocation), struct layout (field offsets), constants / flag masks, call / inline changes, code changes |
| fn_8016F594 | 8016F594 | 0x350 | (unsplit axdriver) | KAR asm kar_axdriver_set_voice_channel_mix_level 804438C4 (similar) | lines offsets(6) other 96% st +0x38->+0x34 ld +0x3A->+0x36 ld +0x38->+0x34 ld +0x3B->+0 | C written from the KAR asm, then: assert lines, struct layout (field offsets), code changes |
| fn_8016F8E4 | 8016F8E4 | 0xC8 | (unsplit axdriver) | KAR asm kar_axdriver_set_owner_channel_mix_level 80443C14 (similar) | offsets 98% ld +0x3D->+0x39 | C written from the KAR asm, then: struct layout (field offsets) |
| fn_8016F9AC | 8016F9AC | 0xB0 | (unsplit axdriver) | KAR asm kar_axdriver_is_voice_playing 80443CDC (similar) | other 98%  | C written from the KAR asm, then: code changes |
| fn_801701D8 | 801701D8 | 0x2F0 | (unsplit axdriver) | KAR asm kar_axdriver_initialize 804449C4 (similar) | lines(2) consts other 98% imm 0x48->0x44 | C written from the KAR asm, then: assert lines, constants / flag masks, code changes |
| fn_801704C8 | 801704C8 | 0x120 | (unsplit axdriver) | KAR asm kar_axdriver_pause_owner_voices 80444CC4 (similar) | lines -check +N(2) -N(9) other(2) 84%  | C written from the KAR asm, then: assert lines, drop a check, code changes |
| fn_801705E8 | 801705E8 | 0x120 | (unsplit axdriver) | KAR asm kar_axdriver_resume_owner_voices 80444FE0 (similar) | lines -check +N(2) -N(9) other(2) 84%  | C written from the KAR asm, then: assert lines, drop a check, code changes |
| fn_80170B28 | 80170B28 | 0x148 | (unsplit axdriver) | - | no reference 24%  | write from our asm |
| fn_80170E5C | 80170E5C | 0x50 | (unsplit axdriver) | KAR asm HVQM4DecSoundClose 80465D18 (similar) | +N(7) other 55%  | C written from the KAR asm, then: code changes |
| fn_80170ECC | 80170ECC | 0x7C | (unsplit devcom) | KAR asm kar_lbaudio_alloc_vehicle_track_subtype_2 8005D1B0 (similar) | lines +check +call(2) -call(2) +N(9) -N(10) 53%  | C written from the KAR asm, then: assert lines, add the extra check, call / inline changes, code changes |
| fn_80170F48 | 80170F48 | 0x1A4 | (unsplit devcom) | KAR asm kar_axdriver_is_slot_playing 80445A84 (similar) | -call(4) -N(51) 67%  | C written from the KAR asm, then: call / inline changes, code changes |
| fn_801710EC | 801710EC | 0x4D8 | (unsplit devcom) | - | no reference 0%  | write from our asm |
| HSD_DevComDVDARAMEndCallback | 8017163C | 0x1A0 | (unsplit devcom) | wowjinxy:devcom.c | lines(3) regs(22) +call branches(2) +N(10) -N(9) 60% C size 0x19C; kar asm 47% rewrite | the C, then: assert lines, declaration / expression order (register allocation), call / inline changes, code changes |
| HSD_DevComDVDCallback | 80171974 | 0x2A4 | (unsplit devcom) | melee:devcom.c | lines(6) regs(16) -call(5) branches(2) +N -N(44) 67% C size 0x350; kar asm 31% rewrite | the C, then: assert lines, declaration / expression order (register allocation), call / inline changes, code changes |
| HSD_SynthSFXSampleLoadCallback | 80172E44 | 0x430 | (unsplit synth) | KAR asm HSD_SynthSFXSampleLoadCallback 80447A74 | -call(9) -N(95) 74%  | C written from the KAR asm, then: call / inline changes, code changes |
| HSD_SynthSFXHeaderLoadCallback | 80173274 | 0x17C | (unsplit synth) | wowjinxy:dvdsche.c | lines(4) regs(16) branches +N(6) other(11) 53% C size 0x164; kar asm 23% rewrite | the C, then: assert lines, declaration / expression order (register allocation), code changes |
| HSD_SynthSFXLoad | 8017346C | 0x180 | (unsplit synth) | KAR asm HSD_SynthSFXLoad 8044809C | branches -N other 96%  | C written from the KAR asm (start from melee:synth.c 61%), then: code changes |
| fn_80173918 | 80173918 | 0x368 | (unsplit synth) | - | no reference 48%  | write from our asm |
| fn_80173C80 | 80173C80 | 0x2E4 | (unsplit synth) | - | no reference 17%  | write from our asm |
| fn_80175458 | 80175458 | 0x9B8 | (unsplit synth) | KAR asm HSD_SynthSFXVolFaderProc 8044A3FC (similar) | lines(4) consts(4) 99% imm 0x670->0x668 imm 0x640->0x638 imm 0x6A0->0x698 imm 0x6D4 | C written from the KAR asm, then: assert lines, constants / flag masks |
| fn_8017631C | 8017631C | 0xFC4 | (unsplit synth) | KAR asm HSD_SynthSFXUpdateMix 8044B2C0 (similar) | lines(4) consts(8) 99% imm 0x1B0->0x1A8 imm 0x640->0x638 imm 0x788->0x780 imm 0x708 | C written from the KAR asm, then: assert lines, constants / flag masks |
| fn_80177784 | 80177784 | 0x1A8 | (unsplit synth) | - | no reference 24%  | write from our asm |
| fn_8017792C | 8017792C | 0x10C | (unsplit synth) | - | no reference 25%  | write from our asm |
| fn_80177A38 | 80177A38 | 0x2EC | (unsplit synth) | - | no reference 0%  | write from our asm |
| fn_80177D24 | 80177D24 | 0x140 | (unsplit synth) | - | no reference 0%  | write from our asm |
| fn_80177E64 | 80177E64 | 0x16C | (unsplit synth) | - | no reference 22%  | write from our asm |
| fn_80177FD0 | 80177FD0 | 0x828 | (unsplit synth) | - | no reference 0%  | write from our asm |
| fn_801787F8 | 801787F8 | 0x408 | (unsplit synth) | - | no reference 0%  | write from our asm |
| fn_80178C00 | 80178C00 | 0x2D0 | (unsplit synth) | - | no reference 0%  | write from our asm |
| fn_80178ED0 | 80178ED0 | 0x100 | (unsplit synth) | - | no reference 28%  | write from our asm |
| HSD_SynthPStreamHeaderCallback | 80178FD0 | 0x2CC | (unsplit synth) | melee:synth.c | rewrite 20% C size 0x1C4 | write from our asm (no close reference) |
| fn_8017929C | 8017929C | 0xA0 | (unsplit synth) | - | no reference 42%  | write from our asm |
| fn_801796F4 | 801796F4 | 0x4C | (unsplit synth) | KAR asm kar_gcpmemcard__near_8045daa4 8045DAA4 (similar) | +N(5) other 63%  | C written from the KAR asm, then: code changes |
| fn_80179740 | 80179740 | 0x4D4 | (unsplit synth) | KAR asm HSD_SynthInit 8044E7F8 (similar) | lines(2) consts(4) 98% imm 0x868->0x860 imm 0x890->0x888 imm 0x8C0->0x8B8 imm 0x8EC | C written from the KAR asm, then: assert lines, constants / flag masks |
| fn_8017A0E4 | 8017A0E4 | 0x164 | (unsplit particle+) | KAR asm kar_particle__near_8042af18 8042AF18 (similar) | regs(2) -call(3) +N(21) -N(11) 67%  | C written from the KAR asm, then: declaration / expression order (register allocation), call / inline changes, code changes |
| fn_8017A248 | 8017A248 | 0x2F8 | (unsplit particle+) | KAR asm psGenerateParticle0 8042B054 (similar) | -N 100%  | C written from the KAR asm, then: code changes |
| fn_8017A540 | 8017A540 | 0xF8 | (unsplit particle+) | KAR asm psGenerateParticleID0 8042B350 (similar) | consts 98% imm 0x20->0x8 | C written from the KAR asm, then: constants / flag masks |
| fn_8017A69C | 8017A69C | 0xFC | (unsplit particle+) | KAR asm psKillParticle 8042B4AC (similar) | consts 98% imm 0x20->0x10 | C written from the KAR asm, then: constants / flag masks |
| fn_8017A798 | 8017A798 | 0xF8 | (unsplit particle+) | KAR asm kar_particle__near_8042b5a8 8042B5A8 (similar) | consts(2) 97% imm 0x100->0x8 | C written from the KAR asm, then: constants / flag masks |
| fn_8017A890 | 8017A890 | 0xDC | (unsplit particle+) | KAR asm kar_particle__near_8042b6a0 8042B6A0 (similar) | consts(2) 96% imm 0x100->0x8 | C written from the KAR asm, then: constants / flag masks |
| fn_8017ACB4 | 8017ACB4 | 0x294 | (unsplit particle+) | - | no reference 35%  | write from our asm |
| fn_8017AF48 | 8017AF48 | 0x270 | (unsplit particle+) | - | no reference 45%  | write from our asm |
| fn_8017B4A4 | 8017B4A4 | 0x2F30 | (unsplit particle+) | - | no reference 0%  | write from our asm |
| fn_8017E79C | 8017E79C | 0xD8 | (unsplit particle+) | KAR asm psInterpretParticles 80430198 (similar) | lines consts 96% imm 0x20->0x10 | C written from the KAR asm, then: assert lines, constants / flag masks |
| fn_8017E89C | 8017E89C | 0x98 | (unsplit particle+) | KAR asm kar_psinterpret__near_80430298 80430298 (similar) | branches +N(6) -N(9) 73%  | C written from the KAR asm, then: code changes |
| fn_8017E934 | 8017E934 | 0x140 | (unsplit particle+) | - | no reference 49%  | write from our asm |
| fn_8017EA74 | 8017EA74 | 0x1BC | (unsplit particle+) | KAR asm kar_psinterpret__near_8043051c 8043051C (similar) | regs(4) offsets(2) consts -call +N(10) -N(23) 72% st +0xFFE0->+0xFFF0 ld +0x3E->+0x32 imm 0x20->0x10 | C written from the KAR asm, then: declaration / expression order (register allocation), struct layout (field offsets), constants / flag masks, call / inline changes, code changes |
| fn_8017EC30 | 8017EC30 | 0x1F0 | (unsplit particle+) | KAR asm kar_psinterpret__near_8043051c 8043051C (similar) | offsets +N(7) -N(7) 94% ld +0x3E->+0x32 | C written from the KAR asm, then: struct layout (field offsets), code changes |
| fn_8017EE20 | 8017EE20 | 0x38C | (unsplit particle+) | - | no reference 35%  | write from our asm |
| fn_8017F4B8 | 8017F4B8 | 0x1270 | (unsplit particle+) | - | no reference 0%  | write from our asm |
| fn_80180728 | 80180728 | 0x3A0 | (unsplit particle+) | KAR asm kar_generator__near_804324ec 804324EC (similar) | offsets branches +N(7) -N(12) 94% ld +0x3E->+0x32 | C written from the KAR asm, then: struct layout (field offsets), code changes |
| fn_80180B74 | 80180B74 | 0x730 | (unsplit particle+) | - | no reference 0%  | write from our asm |
| fn_8018144C | 8018144C | 0xE84 | (unsplit particle+) | KAR asm kar_psdisp__80433f00 80433F00 (similar) | lines regs(6) consts branches(4) +N(5) -N(5) 98% imm 0x20->0x10 | C written from the KAR asm, then: assert lines, declaration / expression order (register allocation), constants / flag masks, code changes |
| fn_801822D0 | 801822D0 | 0x700 | (unsplit particle+) | - | no reference 25%  | write from our asm |
| fn_801829D0 | 801829D0 | 0xBE0 | (unsplit particle+) | KAR asm kar_psdisp__near_80435268 80435268 (similar) | regs(58) offsets(23) consts +check(2) +call(14) branches(4) +N(159) -N(16) other(15) 56% st +0xFEC0->+0xFE60 st +0x1C->+0x10 st +0x1D->+0x11 st +0x1E | C written from the KAR asm, then: declaration / expression order (register allocation), struct layout (field offsets), constants / flag masks, add the extra check, call / inline changes, code changes |
| fn_801835B0 | 801835B0 | 0xA3C | (unsplit particle+) | - | no reference 35%  | write from our asm |
| fn_80183FEC | 80183FEC | 0x464 | (unsplit particle+) | KAR asm kar_psdisp__near_80436460 80436460 (similar) | regs(2) offsets(3) consts +check +call(6) +N(96) -N(12) other(3) 57% st +0xFFD0->+0xFF60 st +0x18->+0x10 ld +0x1C->+0x14 imm 0x30 | C written from the KAR asm, then: declaration / expression order (register allocation), struct layout (field offsets), constants / flag masks, add the extra check, call / inline changes, code changes |
| fn_80184450 | 80184450 | 0x6F4 | (unsplit particle+) | - | no reference 50%  | write from our asm |
| fn_80185A88 | 80185A88 | 0xF8 | (unsplit particle+) | KAR asm kar_psdisp__near_80437cd8 80437CD8 (similar) | offsets(4) branches -N(3) 85% st +0x3E->+0x32 st +0x3D->+0x31 st +0x3C->+0x30 st +0x7C->+0 | C written from the KAR asm, then: struct layout (field offsets), code changes |
| fn_80185B80 | 80185B80 | 0x3C | (unsplit particle+) | KAR asm kar_psdisp__near_80437ddc 80437DDC (similar) | offsets(2) 87% ld +0x3E->+0x32 st +0x3E->+0x32 | C written from the KAR asm, then: struct layout (field offsets) |
| fn_80185BBC | 80185BBC | 0x3C | (unsplit particle+) | KAR asm kar_psdisp__near_80437e18 80437E18 (similar) | offsets(2) 87% ld +0x3E->+0x32 st +0x3E->+0x32 | C written from the KAR asm, then: struct layout (field offsets) |
| fn_80185BF8 | 80185BF8 | 0x6C | (unsplit particle+) | KAR asm kar_psdisp__near_80437e54 80437E54 (similar) | offsets(2) 93% ld +0x3E->+0x32 st +0x3E->+0x32 | C written from the KAR asm, then: struct layout (field offsets) |
| fn_80185C64 | 80185C64 | 0x6C | (unsplit particle+) | KAR asm kar_psdisp__near_80437ec0 80437EC0 (similar) | offsets(2) 93% ld +0x3E->+0x32 st +0x3E->+0x32 | C written from the KAR asm, then: struct layout (field offsets) |
| fn_80185CD0 | 80185CD0 | 0xA0 | (unsplit particle+) | KAR asm psRemoveParticleAppSRT 80437F2C (similar) | offsets(3) 92% ld +0x3E->+0x32 st +0x3E->+0x32 ld +0x78->+0x6C | C written from the KAR asm, then: struct layout (field offsets) |
| fn_80185D70 | 80185D70 | 0xB8 | (unsplit particle+) | KAR asm kar_psdisp__near_80437fcc 80437FCC (similar) | offsets(3) 94% ld +0x3E->+0x32 st +0x3E->+0x32 ld +0x78->+0x6C | C written from the KAR asm, then: struct layout (field offsets) |
| fn_801860EC | 801860EC | 0x6C | (unsplit particle+) | KAR asm HSD_AllocateFIFO 8040FE90 (similar) | lines -call +N(11) -N(4) 56%  | C written from the KAR asm, then: assert lines, call / inline changes, code changes |
| fn_80186158 | 80186158 | 0x338 | (unsplit particle+) | KAR asm kar_pslist__near_8043845c 8043845C (similar) | consts(4) 98% imm 0x80->0x40 imm 0x100->0x80 | C written from the KAR asm, then: constants / flag masks |
| fn_8018AEA4 | 8018AEA4 | 0x13C | tev.c | - | no reference 32%  | write from our asm |
| HSD_SetTevRegAll | 8018B230 | 0xC0 | tev.c | ours:tev.c | +N(4) -N(4) 92% C size 0xC0; kar asm 2% rewrite | the C, then: code changes |
| fn_8018B3B8 | 8018B3B8 | 0x58 | tev.c | - | no reference 46%  | write from our asm |
| fn_8018CCDC | 8018CCDC | 0x50 | aobj.c | KAR asm kar_grlib2__near_800d51fc 800D51FC (similar) | lines regs +N -N(2) other 71%  | C written from the KAR asm, then: assert lines, declaration / expression order (register allocation), code changes |
| fn_8018CD2C | 8018CD2C | 0x80 | aobj.c | KAR asm kar_lbaudio__near_800628a0 800628A0 (similar) | +check(3) -call(3) +N(9) -N 50%  | C written from the KAR asm, then: add the extra check, call / inline changes, code changes |
| HSD_LObjSetupSpecularInit | 8018D7D0 | 0x278 | lobj.c | wowjinxy:lobj.c | lines(2) regs(11) branches(5) -N(3) other 80% C size 0x284; kar asm 29% rewrite | the C, then: assert lines, declaration / expression order (register allocation), code changes |
| HSD_LObjSetupInit | 8018DA48 | 0xBBC | lobj.c | KAR asm HSD_LObjSetupInit 803FE4B8 | regs(2) branches(22) +N(2) -N(5) 96%  | C written from the KAR asm (start from wowjinxy:lobj.c 53%), then: declaration / expression order (register allocation), code changes |
| fn_8018FD60 | 8018FD60 | 0xCC | cobj.c | KAR asm HSD_CObjSetupViewingMtx 80401014 (similar) | -call(2) branches(2) -N(28) 62%  | C written from the KAR asm, then: call / inline changes, code changes |
| HSD_CObjSetCurrent | 8018FE2C | 0x504 | cobj.c | KAR asm HSD_CObjSetCurrent 80401150 | lines -call(2) branches(2) -N(28) 91%  | C written from the KAR asm (start from ours:cobj.c 68%), then: assert lines, call / inline changes, code changes |
| upvec2roll | 80190688 | 0x1E8 | cobj.c | KAR asm upvec2roll 80401BA0 | lines(4) regs(3) -call -N(13) 84%  | C written from the KAR asm, then: assert lines, declaration / expression order (register allocation), call / inline changes, code changes |
| roll2upvec | 80190870 | 0x2E4 | cobj.c | KAR asm roll2upvec 80401DBC | lines(4) regs(2) -call -N(12) 90%  | C written from the KAR asm (start from wowjinxy:cobj.c 59%), then: assert lines, declaration / expression order (register allocation), call / inline changes, code changes |
| fn_80190BBC | 80190BBC | 0x2FC | cobj.c | KAR asm HSD_CObjSetUpVector 80402138 (similar) | lines(5) regs(3) -call branches(2) +N(2) -N(16) 85%  | C written from the KAR asm, then: assert lines, declaration / expression order (register allocation), call / inline changes, code changes |
| HSD_CObjSetRoll | 80190FDC | 0x428 | cobj.c | KAR asm HSD_CObjSetRoll 804027CC | lines regs(4) offsets(6) consts +check +call(7) -call branches(6) +N(32) -N(12) other(9) 73% imm 0x34->0x1C ld +0x34->+0x1C ld +0x38->+0x20 ld +0x3C->+0x | C written from the KAR asm (start from ours:cobj.c 61%), then: assert lines, declaration / expression order (register allocation), struct layout (field offsets), constants / flag masks, add the extra check, call / inline changes, code changes |
| drawShapeAnim | 80194EFC | 0x73C | pobj.c | KAR asm drawShapeAnim 804069AC | lines(3) branches +N(5) 97%  | C written from the KAR asm, then: assert lines, code changes |
| SetupEnvelopeModelMtx | 801958D0 | 0x380 | pobj.c | wowjinxy:pobj.c | lines(13) regs(38) consts +check -check +call(2) -call +N(8) -N(10) 67% imm 0x58->0x4C C size 0x388; kar asm 63% lines(4) regs(50) o | the C, then: assert lines, declaration / expression order (register allocation), constants / flag masks, add the extra check, drop a check, call / inline changes, code changes |
| fn_80198700 | 80198700 | 0x1FC | jobj.c | KAR asm HSD_JObjSetJSoundCallback 8040ADCC (similar) | +check -check -N(8) 87%  | C written from the KAR asm, then: add the extra check, drop a check, code changes |
| mkVBillBoardMtx | 8019B4C4 | 0x2DC | displayfunc.c | KAR asm mkVBillBoardMtx 8040E424 | branches(2) +N(2) -N(8) 93%  | C written from the KAR asm, then: code changes |
| mkHBillBoardMtx | 8019B7A0 | 0x344 | displayfunc.c | KAR asm mkHBillBoardMtx 8040E718 | branches(2) +N(3) -N(8) other 94%  | C written from the KAR asm (start from wowjinxy:displayfunc.c 64%), then: code changes |
| mkBillBoardMtx | 8019BAE4 | 0x398 | displayfunc.c | KAR asm mkBillBoardMtx 8040EA70 | regs branches(2) +N(3) -N(10) other(3) 90%  | C written from the KAR asm, then: declaration / expression order (register allocation), code changes |
| fn_8019C86C | 8019C86C | 0xAC | displayfunc.c | - | no reference 35%  | write from our asm |
| fn_8019CFB8 | 8019CFB8 | 0x1A4 | initialize.c | ours:initialize.c | regs(22) other 78% C size 0x1A4; kar asm 22% rewrite | the C, then: declaration / expression order (register allocation), code changes |
| fn_8019DB14 | 8019DB14 | 0x17C | video.c | KAR asm HSD_VICopyXFBAsync 804112B4 (similar) | regs(19) -call(3) branches +N -N(36) other 55%  | C written from the KAR asm, then: declaration / expression order (register allocation), call / inline changes, code changes |
| fn_8019DC90 | 8019DC90 | 0x1C4 | video.c | - | no reference 36%  | write from our asm |
| fn_8019EB5C | 8019EB5C | 0x430 | mtx.c | - | no reference 0%  | write from our asm |
| fn_8019EF8C | 8019EF8C | 0x320 | mtx.c | KAR asm HSD_MtxGetRotation 80416048 (similar) | regs branches(2) -N(3) other 95%  | C written from the KAR asm, then: declaration / expression order (register allocation), code changes |
| fn_8019FAD8 | 8019FAD8 | 0x13C | mtx.c | KAR asm HSD_MkRotationMtx 80416FFC (similar) | regs +N(4) -N(3) other(12) 72%  | C written from the KAR asm, then: declaration / expression order (register allocation), code changes |
| fn_8019FC14 | 8019FC14 | 0x2D8 | mtx.c | KAR asm HSD_MtxSRT 80417580 (similar) | regs(9) branches(4) +N -N(6) other(17) 79%  | C written from the KAR asm, then: declaration / expression order (register allocation), code changes |
| HSD_MtxSRTQuat | 8019FEEC | 0x1AC | mtx.c | KAR asm HSD_MtxSRTQuat 8041786C | branches(4) -N(6) other(6) 83%  | C written from the KAR asm (start from wowjinxy:mtx.c 54%), then: code changes |
| expEvaluate | 801A21A0 | 0x49C | robj.c | KAR asm expEvaluate 80419C50 | -N(2) 99%  | C written from the KAR asm (start from wowjinxy:robj.c 73%), then: code changes |
| HSD_ShadowStartRender | 801A4558 | 0x2A8 | (unsplit core) | wowjinxy:shadow.c | lines regs(6) branches(2) -N 94% C size 0x2AC; kar asm 23% rewrite | the C, then: assert lines, declaration / expression order (register allocation), code changes |
| fn_801A4800 | 801A4800 | 0x198 | (unsplit core) | - | no reference 22%  | write from our asm |
| fn_801A4998 | 801A4998 | 0x124 | (unsplit core) | - | no reference 34%  | write from our asm |
| fn_801A4ABC | 801A4ABC | 0x1C8 | (unsplit core) | - | no reference 22%  | write from our asm |
| makeMatrix | 801A4DC4 | 0x13C | (unsplit core) | KAR asm makeMatrix 8041DA78 | -N 98%  | C written from the KAR asm (start from wowjinxy:shadow.c 72%), then: code changes |
| HSD_ShadowSetViewingRect | 801A4F00 | 0x1C0 | (unsplit core) | KAR asm HSD_ShadowSetViewingRect 8041DBB8 | regs branches(3) -N(3) other 91%  | C written from the KAR asm (start from wowjinxy:shadow.c 62%), then: declaration / expression order (register allocation), code changes |
| HSD_ViewingRectInit | 801A50C0 | 0x20C | (unsplit core) | KAR asm HSD_ViewingRectInit 8041DD84 | regs(39) -N(2) 66%  | C written from the KAR asm (start from wowjinxy:shadow.c 54%), then: declaration / expression order (register allocation), code changes |
| fn_801A5634 | 801A5634 | 0xD4 | (unsplit core) | - | no reference 38%  | write from our asm |
| fn_801A5708 | 801A5708 | 0x340 | (unsplit core) | - | no reference 18%  | write from our asm |
| fn_801A5B08 | 801A5B08 | 0x160 | (unsplit core) | - | no reference 23%  | write from our asm |
| fn_801A5C68 | 801A5C68 | 0x214 | (unsplit core) | - | no reference 0%  | write from our asm |
| HSD_ByteCodeEval | 801A5F10 | 0x15C8 | bytecode.c | KAR asm HSD_ByteCodeEval 8041E6D8 | branches(41) +N(2) 97%  | C written from the KAR asm (start from wowjinxy:bytecode.c 58%), then: code changes |
| HSD_TExpCompile | 801AACB4 | 0x2A8 | texp.c | ours:texp.c | lines(7) offsets(4) 94% ld +0x16->+0x10 st +0x16->+0x10 C size 0x2A8; kar asm 82% li | the C, then: assert lines, struct layout (field offsets) |

What the HSD diffs show, by part:

- **core**: 60 functions identical to KAR asm and 16 with only assert lines are transcription (most are unnamed here:
  their KAR names above locate the Melee / KAR C to start from). The real changes are few: cobj's HSD_CObjSetRoll (camera
  fields moved: +0x34 / +0x38 / +0x3C -> +0x1C / +0x20 / +0x24), HSD_CObjSetCurrent / upvec2roll / roll2upvec (a helper
  call inlined here), the billboard matrices (a few instructions fewer), texp's HSD_TExpCompile (TETev field +0x16 ->
  +0x10), HSD_ByteCodeEval (branch displacements + 2 instructions). 12 have no reference (mtx.c fn_8019EB5C, the shadow /
  archive neighbours, tev.c fn_8018AEA4 / fn_8018B3B8, video.c fn_8019DC90, displayfunc.c fn_8019C86C).
- **axdriver**: KAR's driver with a re-packed voice record: the same code reads +0x1C / +0x30 / +0x38..+0x3D in KAR and
  +0x1B / +0x2E / +0x34..+0x39 here (byte fields shifted by 1-4); 14 functions identical. 6 have no reference
  (fn_8016E030, 0xB4C, the largest).
- **synth**: 26 identical, 6 assert lines; the SFX mixer's two big functions differ only in a work-area offset 8 bytes
  smaller (imm 0x670 -> 0x668 ...) and assert lines; 12 (0x80177784-0x8017929C, the PStream / stream part) have no reference.
- **devcom**: 5 identical, 4 assert lines, the 2 new exact callbacks from wowjinxy; HSD_DevComDVDCallback /
  HSD_DevComDVDARAMEndCallback are 60-67% to Melee / wowjinxy: rewrite.
- **particle**: 32 identical; the psdisp / generator group reads the particle record's +0x3E / +0x78 at +0x32 / +0x6C
  (a record 0xC bytes shorter) and some allocation sizes halve (imm 0x20 -> 0x10, 0x100 -> 0x8): struct edits, then KAR's
  code; 10 have no reference, among them fn_8017B4A4 (0x2F30) and fn_8017F4B8 (0x1270).

### What remains unknown

- SDK: fn_80148998 (AXFX ReverbSTDCreate, Jul 2003 AX build): no reference above 63%; register allocation differs.
- HSD: 41 functions with no reference ("-" above) and 18 rewrites; the HSD revision itself has no public source.
  Everything else in every library has an exact reference or is KAR asm to transcribe.
- Names not applied: fn_8015E09C TRK__write_aram, fn_8015E288 TRK__read_aram, fn_800053E0 __TRK_reset (all exact under
  those names in TP / Pikmin 2's C), and the SDK by-bytes names. No configure.py change was made: the CR_OBJECT_CFLAGS
  needed (`-sdata 8` for MetroTRK/ddh/main.c and gdev/main.c, `-str reuse,nopool,readonly` for Runtime/GCN_mem_alloc.c)
  and the imports go with a build.
