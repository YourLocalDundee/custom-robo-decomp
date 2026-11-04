/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .sbss 0x80450008-0x80450038, 11 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x80450030 lbl_80450030 (u8[8], 0x8 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8045002C lbl_8045002C (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80450028 lbl_80450028 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80450024 lbl_80450024 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80450020 lbl_80450020 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8045001C lbl_8045001C (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80450018 lbl_80450018 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80450014 lbl_80450014 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80450010 lbl_80450010 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8045000C lbl_8045000C (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80450008 lbl_80450008 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_80450030[8];
u8 lbl_8045002C[4];
u8 lbl_80450028[4];
u8 lbl_80450024[4];
u8 lbl_80450020[4];
u8 lbl_8045001C[4];
u8 lbl_80450018[4];
u8 lbl_80450014[4];
u8 lbl_80450010[4];
u8 lbl_8045000C[4];
u8 lbl_80450008[4];
#pragma force_active reset
