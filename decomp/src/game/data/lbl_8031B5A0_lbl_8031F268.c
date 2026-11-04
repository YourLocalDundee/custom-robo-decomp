/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x8031B5A0-0x8031FE68, 17 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x8031F268 lbl_8031F268 (u8[3072], 0xC00 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031F240 lbl_8031F240 (u8[40], 0x28 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031F22C lbl_8031F22C (u8[20], 0x14 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031F21C lbl_8031F21C (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031F1A8 lbl_8031F1A8 (u8[116], 0x74 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031F020 lbl_8031F020 (u8[392], 0x188 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031EFE0 lbl_8031EFE0 (u8[64], 0x40 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031EAE0 lbl_8031EAE0 (u8[1280], 0x500 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031EAC4 lbl_8031EAC4 (u8[28], 0x1C bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031DAB4 lbl_8031DAB4 (u8[4112], 0x1010 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031C2B4 lbl_8031C2B4 (u8[6144], 0x1800 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031C29C lbl_8031C29C (u8[24], 0x18 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031BF54 lbl_8031BF54 (u8[840], 0x348 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031BED8 lbl_8031BED8 (u8[124], 0x7C bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031BEC4 lbl_8031BEC4 (u8[20], 0x14 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031BD24 lbl_8031BD24 (u8[416], 0x1A0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8031B5A0 lbl_8031B5A0 (u8[1924], 0x784 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_8031F268[3072];
u8 lbl_8031F240[40];
u8 lbl_8031F22C[20];
u8 lbl_8031F21C[16];
u8 lbl_8031F1A8[116];
u8 lbl_8031F020[392];
u8 lbl_8031EFE0[64];
u8 lbl_8031EAE0[1280];
u8 lbl_8031EAC4[28];
u8 lbl_8031DAB4[4112];
u8 lbl_8031C2B4[6144];
u8 lbl_8031C29C[24];
u8 lbl_8031BF54[840];
u8 lbl_8031BED8[124];
u8 lbl_8031BEC4[20];
u8 lbl_8031BD24[416];
u8 lbl_8031B5A0[1924];
#pragma force_active reset
