/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x80386000-0x80387160, 14 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x80386E44 lbl_80386E44 (u8[796], 0x31C bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80386D58 lbl_80386D58 (u8[236], 0xEC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80386BFC lbl_80386BFC (u8[348], 0x15C bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8038684C lbl_8038684C (u8[944], 0x3B0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80386840 lbl_80386840 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803867F8 lbl_803867F8 (u8[72], 0x48 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80386628 lbl_80386628 (u8[464], 0x1D0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80386588 lbl_80386588 (u8[160], 0xA0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80386550 lbl_80386550 (u8[56], 0x38 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803864F8 lbl_803864F8 (u8[88], 0x58 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803864E8 lbl_803864E8 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80386448 lbl_80386448 (u8[160], 0xA0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80386378 lbl_80386378 (u8[208], 0xD0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80386000 lbl_80386000 (u8[888], 0x378 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_80386E44[796];
u8 lbl_80386D58[236];
u8 lbl_80386BFC[348];
u8 lbl_8038684C[944];
u8 lbl_80386840[12];
u8 lbl_803867F8[72];
u8 lbl_80386628[464];
u8 lbl_80386588[160];
u8 lbl_80386550[56];
u8 lbl_803864F8[88];
u8 lbl_803864E8[16];
u8 lbl_80386448[160];
u8 lbl_80386378[208];
u8 lbl_80386000[888];
#pragma force_active reset
