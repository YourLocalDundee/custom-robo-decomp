/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .sbss 0x8044FD48-0x8044FDCD, 36 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x8044FDCC lbl_8044FDCC (u8[1], 0x1 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDC8 lbl_8044FDC8 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDC4 lbl_8044FDC4 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDBC lbl_8044FDBC (u8[8], 0x8 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDB8 lbl_8044FDB8 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDB7 lbl_8044FDB7 (u8[1], 0x1 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDB6 lbl_8044FDB6 (u8[1], 0x1 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDB5 lbl_8044FDB5 (u8[1], 0x1 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDB4 lbl_8044FDB4 (u8[1], 0x1 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDB0 lbl_8044FDB0 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDAC lbl_8044FDAC (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDA8 lbl_8044FDA8 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDA4 lbl_8044FDA4 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FDA0 lbl_8044FDA0 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD9C lbl_8044FD9C (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD98 lbl_8044FD98 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD94 lbl_8044FD94 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD90 lbl_8044FD90 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD8C lbl_8044FD8C (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD88 lbl_8044FD88 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD84 lbl_8044FD84 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD80 lbl_8044FD80 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD7C lbl_8044FD7C (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD78 lbl_8044FD78 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD74 lbl_8044FD74 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD70 lbl_8044FD70 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD6C lbl_8044FD6C (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD68 lbl_8044FD68 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD64 lbl_8044FD64 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD60 lbl_8044FD60 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD5C lbl_8044FD5C (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD58 lbl_8044FD58 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD54 lbl_8044FD54 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD50 lbl_8044FD50 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD4C lbl_8044FD4C (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044FD48 lbl_8044FD48 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_8044FDCC[1];
u8 lbl_8044FDC8[4];
u8 lbl_8044FDC4[4];
u8 lbl_8044FDBC[8];
u8 lbl_8044FDB8[4];
u8 lbl_8044FDB7[1];
u8 lbl_8044FDB6[1];
u8 lbl_8044FDB5[1];
u8 lbl_8044FDB4[1];
u8 lbl_8044FDB0[4];
u8 lbl_8044FDAC[4];
u8 lbl_8044FDA8[4];
u8 lbl_8044FDA4[4];
u8 lbl_8044FDA0[4];
u8 lbl_8044FD9C[4];
u8 lbl_8044FD98[4];
u8 lbl_8044FD94[4];
u8 lbl_8044FD90[4];
u8 lbl_8044FD8C[4];
u8 lbl_8044FD88[4];
u8 lbl_8044FD84[4];
u8 lbl_8044FD80[4];
u8 lbl_8044FD7C[4];
u8 lbl_8044FD78[4];
u8 lbl_8044FD74[4];
u8 lbl_8044FD70[4];
u8 lbl_8044FD6C[4];
u8 lbl_8044FD68[4];
u8 lbl_8044FD64[4];
u8 lbl_8044FD60[4];
u8 lbl_8044FD5C[4];
u8 lbl_8044FD58[4];
u8 lbl_8044FD54[4];
u8 lbl_8044FD50[4];
u8 lbl_8044FD4C[4];
u8 lbl_8044FD48[4];
#pragma force_active reset
