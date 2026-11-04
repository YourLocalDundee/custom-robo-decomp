/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x803B8C00-0x803BAF28, 19 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x803BADA8 lbl_803BADA8 (u8[384], 0x180 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803BAD98 lbl_803BAD98 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803BAD68 lbl_803BAD68 (u8[48], 0x30 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803BACE8 lbl_803BACE8 (u8[128], 0x80 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803BA3A8 lbl_803BA3A8 (u8[2368], 0x940 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803BA37C lbl_803BA37C (u8[44], 0x2C bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803BA350 lbl_803BA350 (u8[44], 0x2C bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803BA330 lbl_803BA330 (u8[32], 0x20 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803BA1E8 lbl_803BA1E8 (u8[328], 0x148 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803BA114 lbl_803BA114 (u8[212], 0xD4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803B97C4 lbl_803B97C4 (u8[2384], 0x950 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803B931C lbl_803B931C (u8[1192], 0x4A8 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803B92CC lbl_803B92CC (u8[80], 0x50 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803B909C lbl_803B909C (u8[560], 0x230 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803B8EBC lbl_803B8EBC (u8[480], 0x1E0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803B8CBC lbl_803B8CBC (u8[512], 0x200 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803B8C6C lbl_803B8C6C (u8[80], 0x50 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803B8C44 lbl_803B8C44 (u8[40], 0x28 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803B8C00 lbl_803B8C00 (u8[68], 0x44 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_803BADA8[384];
u8 lbl_803BAD98[16];
u8 lbl_803BAD68[48];
u8 lbl_803BACE8[128];
u8 lbl_803BA3A8[2368];
u8 lbl_803BA37C[44];
u8 lbl_803BA350[44];
u8 lbl_803BA330[32];
u8 lbl_803BA1E8[328];
u8 lbl_803BA114[212];
u8 lbl_803B97C4[2384];
u8 lbl_803B931C[1192];
u8 lbl_803B92CC[80];
u8 lbl_803B909C[560];
u8 lbl_803B8EBC[480];
u8 lbl_803B8CBC[512];
u8 lbl_803B8C6C[80];
u8 lbl_803B8C44[40];
u8 lbl_803B8C00[68];
#pragma force_active reset
