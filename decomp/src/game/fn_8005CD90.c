/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8005CD90: .text 0x8005CD90-0x8005CDDC (76 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_0678;

/* fn_8005CD90: hand-written from main.dol's asm on its m2c draft (plan.md step 108): the angle stored to an s16 field (extsh) */
extern u8 *lbl_8044F594;

void fn_8005CD90(void) {
    u16 *var_r31;

    var_r31 = M2C_FIELD(lbl_8044F594, u16 **, 4);
    M2C_FIELD(cr_g_CurrentChara_0678, s16 *, 0x44) = (s16) ((*var_r31 << 0x10) / 360);
}
