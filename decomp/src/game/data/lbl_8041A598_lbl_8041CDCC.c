/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x8041A598-0x8041CDD8, 21 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x8041CDCC lbl_8041CDCC (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041CDB4 lbl_8041CDB4 (u8[24], 0x18 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041CD44 lbl_8041CD44 (u8[112], 0x70 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041CD18 lbl_8041CD18 (u8[44], 0x2C bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041CD0C lbl_8041CD0C (u8[12], 0xC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041CCA0 lbl_8041CCA0 (u8[108], 0x6C bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041CC10 lbl_8041CC10 (u8[144], 0x90 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041CBFB lbl_8041CBFB (u8[21], 0x15 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041CBEE lbl_8041CBEE (u8[13], 0xD bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041CBD0 lbl_8041CBD0 (u8[30], 0x1E bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041CBC0 lbl_8041CBC0 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041CBAC lbl_8041CBAC (u8[20], 0x14 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041C83C lbl_8041C83C (u8[880], 0x370 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041C828 lbl_8041C828 (u8[20], 0x14 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041C408 lbl_8041C408 (u8[1056], 0x420 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041C0C0 lbl_8041C0C0 (u8[840], 0x348 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041C0A8 lbl_8041C0A8 (u8[24], 0x18 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041A67C lbl_8041A67C (u8[6700], 0x1A2C bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041A648 lbl_8041A648 (u8[52], 0x34 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041A608 lbl_8041A608 (u8[64], 0x40 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8041A598 lbl_8041A598 (u8[112], 0x70 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_8041CDCC[12];
u8 lbl_8041CDB4[24];
u8 lbl_8041CD44[112];
u8 lbl_8041CD18[44];
u8 lbl_8041CD0C[12];
u8 lbl_8041CCA0[108];
u8 lbl_8041CC10[144];
u8 lbl_8041CBFB[21];
u8 lbl_8041CBEE[13];
u8 lbl_8041CBD0[30];
u8 lbl_8041CBC0[16];
u8 lbl_8041CBAC[20];
u8 lbl_8041C83C[880];
u8 lbl_8041C828[20];
u8 lbl_8041C408[1056];
u8 lbl_8041C0C0[840];
u8 lbl_8041C0A8[24];
u8 lbl_8041A67C[6700];
u8 lbl_8041A648[52];
u8 lbl_8041A608[64];
u8 lbl_8041A598[112];
#pragma force_active reset
