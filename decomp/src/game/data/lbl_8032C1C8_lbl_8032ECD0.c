/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x8032C1C8-0x80330BC0, 9 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x8032ECD0 lbl_8032ECD0 (u8[7920], 0x1EF0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8032CC20 lbl_8032CC20 (u8[8368], 0x20B0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8032C2B0 lbl_8032C2B0 (u8[2416], 0x970 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8032C288 lbl_8032C288 (u8[40], 0x28 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8032C234 lbl_8032C234 (u8[84], 0x54 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8032C228 lbl_8032C228 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8032C214 lbl_8032C214 (u8[20], 0x14 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8032C208 lbl_8032C208 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8032C1C8 lbl_8032C1C8 (u8[64], 0x40 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_8032ECD0[7920];
u8 lbl_8032CC20[8368];
u8 lbl_8032C2B0[2416];
u8 lbl_8032C288[40];
u8 lbl_8032C234[84];
u8 lbl_8032C228[12];
u8 lbl_8032C214[20];
u8 lbl_8032C208[12];
u8 lbl_8032C1C8[64];
#pragma force_active reset
