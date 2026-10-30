/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8003D4C4: .text 0x8003D4C4-0x8003D500 (60 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern f32 lbl_8044F3E8[2];
extern u8 *cr_g_CurrentChara_06dc;
extern u8 *lbl_8044F57C;
void fn_800527D4(void);
void cr_Chara_PoseLimbs(void);

void fn_8003D4C4(void) {
    *lbl_8044F3E8 = M2C_FIELD(cr_g_CurrentChara_06dc, f32 *, 0xC);
    lbl_8044F57C = M2C_FIELD(cr_g_CurrentChara_06dc, u8 **, 8);
    fn_800527D4();
    cr_Chara_PoseLimbs();
}
