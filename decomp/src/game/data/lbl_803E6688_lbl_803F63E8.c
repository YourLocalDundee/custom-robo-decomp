/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x803E6688-0x803F6420, 17 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x803F63E8 lbl_803F63E8 (u8[56], 0x38 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803F63D8 lbl_803F63D8 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803F63CC lbl_803F63CC (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803F6354 lbl_803F6354 (u8[120], 0x78 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803F6348 lbl_803F6348 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803F633C lbl_803F633C (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803F623C lbl_803F623C (u8[256], 0x100 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803F6230 lbl_803F6230 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803F6224 lbl_803F6224 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803F6218 lbl_803F6218 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803E83B8 lbl_803E83B8 (u8[56928], 0xDE60 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803E7138 lbl_803E7138 (u8[4736], 0x1280 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803E67F8 lbl_803E67F8 (u8[2368], 0x940 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803E66B4 lbl_803E66B4 (u8[324], 0x144 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803E66A0 lbl_803E66A0 (u8[20], 0x14 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803E6694 lbl_803E6694 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803E6688 lbl_803E6688 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_803F63E8[56];
u8 lbl_803F63D8[16];
u8 lbl_803F63CC[12];
u8 lbl_803F6354[120];
u8 lbl_803F6348[12];
u8 lbl_803F633C[12];
u8 lbl_803F623C[256];
u8 lbl_803F6230[12];
u8 lbl_803F6224[12];
u8 lbl_803F6218[12];
u8 lbl_803E83B8[56928];
u8 lbl_803E7138[4736];
u8 lbl_803E67F8[2368];
u8 lbl_803E66B4[324];
u8 lbl_803E66A0[20];
u8 lbl_803E6694[12];
u8 lbl_803E6688[12];
#pragma force_active reset
