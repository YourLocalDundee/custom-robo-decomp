/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .sbss 0x8044F5B8-0x8044F5E0, 12 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x8044F5DC lbl_8044F5DC (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F5D8 lbl_8044F5D8 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F5D4 lbl_8044F5D4 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F5D1 lbl_8044F5D1 (u8[3], 0x3 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F5D0 lbl_8044F5D0 (u8[1], 0x1 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F5CD lbl_8044F5CD (u8[3], 0x3 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F5CC lbl_8044F5CC (u8[1], 0x1 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F5CA lbl_8044F5CA (u8[2], 0x2 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F5C8 lbl_8044F5C8 (u8[2], 0x2 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F5C4 lbl_8044F5C4 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F5C0 lbl_8044F5C0 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F5B8 lbl_8044F5B8 (u8[8], 0x8 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_8044F5DC[4];
u8 lbl_8044F5D8[4];
u8 lbl_8044F5D4[4];
u8 lbl_8044F5D1[3];
u8 lbl_8044F5D0[1];
u8 lbl_8044F5CD[3];
u8 lbl_8044F5CC[1];
u8 lbl_8044F5CA[2];
u8 lbl_8044F5C8[2];
u8 lbl_8044F5C4[4];
u8 lbl_8044F5C0[4];
u8 lbl_8044F5B8[8];
#pragma force_active reset
