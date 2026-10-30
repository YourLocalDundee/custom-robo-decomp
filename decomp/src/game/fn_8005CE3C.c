/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8005CE3C: .text 0x8005CE3C-0x8005CE70 (52 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_0678;

extern u8 *lbl_8044F594;

void fn_8005CE3C(void) {
    void *temp_r31_6;

    temp_r31_6 = M2C_FIELD(lbl_8044F594, void **, 4);
    M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x1A) = M2C_FIELD(temp_r31_6, u16 *, 0);
    M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x48) = M2C_FIELD(temp_r31_6, u16 *, 2);
}
