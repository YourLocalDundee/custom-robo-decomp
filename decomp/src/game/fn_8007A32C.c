/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007A32C: .text 0x8007A32C-0x8007A39C (112 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F61A[6];
extern u32 lbl_80321D40[18];
extern u8 lbl_8044EA90[8];

void fn_8007A32C(void *arg0) {
    if (!(*lbl_8044F61A & 3) && !(*lbl_80321D40 & 0xC0000000)) {
        (*(M2C_UNK (**)()) (lbl_8044EA90 + (M2C_FIELD(arg0, u8 *, 0x58) * 4)))(arg0);
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
