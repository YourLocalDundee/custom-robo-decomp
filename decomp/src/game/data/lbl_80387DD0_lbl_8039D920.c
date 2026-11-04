/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x80387DD0-0x8039F120, 23 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x8039D920 lbl_8039D920 (u8[6144], 0x1800 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8039D8E0 lbl_8039D8E0 (u8[64], 0x40 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8039D8A0 lbl_8039D8A0 (u8[64], 0x40 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8039649C lbl_8039649C (u8[29700], 0x7404 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80396490 lbl_80396490 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80396484 lbl_80396484 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80396478 lbl_80396478 (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80396258 lbl_80396258 (u8[544], 0x220 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80392A54 lbl_80392A54 (u8[14340], 0x3804 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80392A44 lbl_80392A44 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80391D44 lbl_80391D44 (u8[3328], 0xD00 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80391544 lbl_80391544 (u8[2048], 0x800 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80391468 lbl_80391468 (u8[220], 0xDC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80391458 lbl_80391458 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80391430 lbl_80391430 (u8[40], 0x28 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8038B430 lbl_8038B430 (u8[24576], 0x6000 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8038B420 lbl_8038B420 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8038A420 lbl_8038A420 (u8[4096], 0x1000 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8038A2C0 lbl_8038A2C0 (u8[352], 0x160 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80388284 lbl_80388284 (u8[8252], 0x203C bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80387E84 lbl_80387E84 (u8[1024], 0x400 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80387E70 lbl_80387E70 (u8[20], 0x14 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80387DD0 lbl_80387DD0 (u8[160], 0xA0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_8039D920[6144];
u8 lbl_8039D8E0[64];
u8 lbl_8039D8A0[64];
u8 lbl_8039649C[29700];
u8 lbl_80396490[12];
u8 lbl_80396484[12];
u8 lbl_80396478[12];
u8 lbl_80396258[544];
u8 lbl_80392A54[14340];
u8 lbl_80392A44[16];
u8 lbl_80391D44[3328];
u8 lbl_80391544[2048];
u8 lbl_80391468[220];
u8 lbl_80391458[16];
u8 lbl_80391430[40];
u8 lbl_8038B430[24576];
u8 lbl_8038B420[16];
u8 lbl_8038A420[4096];
u8 lbl_8038A2C0[352];
u8 lbl_80388284[8252];
u8 lbl_80387E84[1024];
u8 lbl_80387E70[20];
u8 lbl_80387DD0[160];
#pragma force_active reset
