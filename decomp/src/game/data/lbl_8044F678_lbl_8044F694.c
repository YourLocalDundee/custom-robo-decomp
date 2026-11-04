/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .sbss 0x8044F678-0x8044F698, 7 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x8044F694 lbl_8044F694 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F690 lbl_8044F690 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F68C lbl_8044F68C (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F688 lbl_8044F688 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F684 lbl_8044F684 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F67C lbl_8044F67C (u8[8], 0x8 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8044F678 lbl_8044F678 (u8[4], 0x4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_8044F694[4];
u8 lbl_8044F690[4];
u8 lbl_8044F68C[4];
u8 lbl_8044F688[4];
u8 lbl_8044F684[4];
u8 lbl_8044F67C[8];
u8 lbl_8044F678[4];
#pragma force_active reset
