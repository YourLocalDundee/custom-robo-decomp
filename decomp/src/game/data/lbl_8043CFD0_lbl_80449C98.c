/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x8043CFD0-0x80449D18, 29 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x80449C98 lbl_80449C98 (u8[128], 0x80 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80449C58 lbl_80449C58 (u8[64], 0x40 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80449C24 lbl_80449C24 (u8[52], 0x34 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80449B78 lbl_80449B78 (u8[172], 0xAC bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80449B58 lbl_80449B58 (u8[32], 0x20 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80449A58 lbl_80449A58 (u8[256], 0x100 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80449958 lbl_80449958 (u8[256], 0x100 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80449858 lbl_80449858 (u8[256], 0x100 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80449758 lbl_80449758 (u8[256], 0x100 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80449558 lbl_80449558 (u8[512], 0x200 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80448EB8 lbl_80448EB8 (u8[1696], 0x6A0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80448EA0 lbl_80448EA0 (u8[24], 0x18 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80448DA0 lbl_80448DA0 (u8[256], 0x100 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x804489A0 lbl_804489A0 (u8[1024], 0x400 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x804463A0 lbl_804463A0 (u8[9728], 0x2600 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80445F60 lbl_80445F60 (u8[1088], 0x440 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80445E80 lbl_80445E80 (u8[224], 0xE0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80445D80 lbl_80445D80 (u8[256], 0x100 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80445B80 lbl_80445B80 (u8[512], 0x200 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80445AE0 lbl_80445AE0 (u8[160], 0xA0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80445AD0 lbl_80445AD0 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80445AA0 lbl_80445AA0 (u8[48], 0x30 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80445A60 lbl_80445A60 (u8[64], 0x40 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8043DA60 lbl_8043DA60 (u8[32768], 0x8000 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8043D9E0 lbl_8043D9E0 (u8[128], 0x80 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8043D9D0 lbl_8043D9D0 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8043D9C0 lbl_8043D9C0 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8043D0D0 lbl_8043D0D0 (u8[2288], 0x8F0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8043CFD0 lbl_8043CFD0 (u8[256], 0x100 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_80449C98[128];
u8 lbl_80449C58[64];
u8 lbl_80449C24[52];
u8 lbl_80449B78[172];
u8 lbl_80449B58[32];
u8 lbl_80449A58[256];
u8 lbl_80449958[256];
u8 lbl_80449858[256];
u8 lbl_80449758[256];
u8 lbl_80449558[512];
u8 lbl_80448EB8[1696];
u8 lbl_80448EA0[24];
u8 lbl_80448DA0[256];
u8 lbl_804489A0[1024];
u8 lbl_804463A0[9728];
u8 lbl_80445F60[1088];
u8 lbl_80445E80[224];
u8 lbl_80445D80[256];
u8 lbl_80445B80[512];
u8 lbl_80445AE0[160];
u8 lbl_80445AD0[16];
u8 lbl_80445AA0[48];
u8 lbl_80445A60[64];
u8 lbl_8043DA60[32768];
u8 lbl_8043D9E0[128];
u8 lbl_8043D9D0[16];
u8 lbl_8043D9C0[16];
u8 lbl_8043D0D0[2288];
u8 lbl_8043CFD0[256];
#pragma force_active reset
