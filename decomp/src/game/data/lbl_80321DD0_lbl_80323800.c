/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x80321DD0-0x80324BD0, 12 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x80323800 lbl_80323800 (u8[5072], 0x13D0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8032344C lbl_8032344C (u8[948], 0x3B4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8032338C lbl_8032338C (u8[192], 0xC0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8032329C lbl_8032329C (u8[240], 0xF0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8032328C lbl_8032328C (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80323280 lbl_80323280 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80323270 lbl_80323270 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80321E90 lbl_80321E90 (u8[5088], 0x13E0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80321E60 lbl_80321E60 (u8[48], 0x30 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80321E30 lbl_80321E30 (u8[48], 0x30 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80321E00 lbl_80321E00 (u8[48], 0x30 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80321DD0 lbl_80321DD0 (u8[48], 0x30 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_80323800[5072];
u8 lbl_8032344C[948];
u8 lbl_8032338C[192];
u8 lbl_8032329C[240];
u8 lbl_8032328C[16];
u8 lbl_80323280[12];
u8 lbl_80323270[16];
u8 lbl_80321E90[5088];
u8 lbl_80321E60[48];
u8 lbl_80321E30[48];
u8 lbl_80321E00[48];
u8 lbl_80321DD0[48];
#pragma force_active reset
