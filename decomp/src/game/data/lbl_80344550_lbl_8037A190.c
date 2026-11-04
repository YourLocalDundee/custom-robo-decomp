/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x80344550-0x8037A400, 25 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x8037A190 lbl_8037A190 (u8[624], 0x270 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80379FE0 lbl_80379FE0 (u8[432], 0x1B0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80379ED0 lbl_80379ED0 (u8[272], 0x110 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80379970 lbl_80379970 (u8[1376], 0x560 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80379960 lbl_80379960 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80379950 lbl_80379950 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80379940 lbl_80379940 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80379930 lbl_80379930 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80379920 lbl_80379920 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80378330 lbl_80378330 (u8[5616], 0x15F0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80377EE0 lbl_80377EE0 (u8[1104], 0x450 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80377ED0 lbl_80377ED0 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80377EC0 lbl_80377EC0 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80377EB0 lbl_80377EB0 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80377E60 lbl_80377E60 (u8[80], 0x50 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80377C60 lbl_80377C60 (u8[512], 0x200 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8037569C lbl_8037569C (u8[9668], 0x25C4 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x803754BC lbl_803754BC (u8[480], 0x1E0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8035417C lbl_8035417C (u8[136000], 0x21340 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80354154 lbl_80354154 (u8[40], 0x28 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80354110 lbl_80354110 (u8[68], 0x44 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80354100 lbl_80354100 (u8[16], 0x10 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8034C100 lbl_8034C100 (u8[32768], 0x8000 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x8034BE10 lbl_8034BE10 (u8[752], 0x2F0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 *   0x80344550 lbl_80344550 (u8[30912], 0x78C0 bytes), UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
u8 lbl_8037A190[624];
u8 lbl_80379FE0[432];
u8 lbl_80379ED0[272];
u8 lbl_80379970[1376];
u8 lbl_80379960[16];
u8 lbl_80379950[16];
u8 lbl_80379940[16];
u8 lbl_80379930[16];
u8 lbl_80379920[16];
u8 lbl_80378330[5616];
u8 lbl_80377EE0[1104];
u8 lbl_80377ED0[16];
u8 lbl_80377EC0[16];
u8 lbl_80377EB0[16];
u8 lbl_80377E60[80];
u8 lbl_80377C60[512];
u8 lbl_8037569C[9668];
u8 lbl_803754BC[480];
u8 lbl_8035417C[136000];
u8 lbl_80354154[40];
u8 lbl_80354110[68];
u8 lbl_80354100[16];
u8 lbl_8034C100[32768];
u8 lbl_8034BE10[752];
u8 lbl_80344550[30912];
#pragma force_active reset
