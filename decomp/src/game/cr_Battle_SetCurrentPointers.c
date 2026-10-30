/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Battle_SetCurrentPointers: .text 0x80050F08-0x80050F90 (136 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_0e4c;
extern u8 *lbl_8044F348;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_11cc;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_06dc;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_09e8;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_026c;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_154c;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_0678;
__declspec(section ".sdata") extern f32 *cr_g_CurrentCharaLimbParams;
extern f32 lbl_8044F540;
extern u8 *lbl_8044F53C;
extern u8 *lbl_8044F534;

void cr_Battle_SetCurrentPointers(void) {
    cr_g_CurrentChara_0e4c = lbl_8044F348 + 0x974;
    cr_g_CurrentChara_11cc = lbl_8044F348 + 0xCF4;
    cr_g_CurrentChara_06dc = lbl_8044F348 + 0x53C;
    cr_g_CurrentChara_09e8 = lbl_8044F348 + 0x848;
    cr_g_CurrentChara_026c = lbl_8044F348 + 0x70;
    cr_g_CurrentChara_154c = lbl_8044F348 + 0x1074;
    cr_g_CurrentChara_0678 = lbl_8044F348 + 0x4BC;
    cr_g_CurrentCharaLimbParams = M2C_FIELD(lbl_8044F348, f32 **, 0x85C);
    lbl_8044F540 = M2C_FIELD(lbl_8044F348, f32 *, 0x17F8);
    lbl_8044F53C = lbl_8044F348 + 0x20;
    lbl_8044F534 = lbl_8044F348 + 0x60;
}
