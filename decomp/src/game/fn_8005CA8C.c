/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8005CA8C: .text 0x8005CA8C-0x8005CB28 (156 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 *lbl_8044F590;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_0678;
extern u16 lbl_8044F58C;
extern u32 lbl_8044F588;
extern u8 lbl_801F9DDC[44];

void fn_8005CA8C(void) {

    lbl_8044F590 += 8;
    if (M2C_FIELD(lbl_8044F590, s32 *, 0) == 0) {
        M2C_FIELD(cr_g_CurrentChara_0678, u8 **, 0x4C) = (u8 *) lbl_8044F590;
        return;
    }
    lbl_8044F58C = *M2C_FIELD(lbl_8044F590, u16 **, 4);
    if (lbl_8044F588 < (u16) lbl_8044F58C) {
        M2C_FIELD(cr_g_CurrentChara_0678, u8 **, 0x4C) = (u8 *) lbl_8044F590;
        return;
    }
    (*(M2C_UNK (**)()) (lbl_801F9DDC + (M2C_FIELD(lbl_8044F590, s32 *, 0) * 4)))();
}
