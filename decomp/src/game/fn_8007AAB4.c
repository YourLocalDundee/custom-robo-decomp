/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007AAB4: .text 0x8007AAB4-0x8007AB14 (96 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F61A[6];
extern u8 lbl_8044EA80[8];

void fn_8007AAB4(void *arg0) {
    if (!(*lbl_8044F61A & 9)) {
        (*(M2C_UNK (**)()) (lbl_8044EA80 + (M2C_FIELD(arg0, u8 *, 0x58) * 4)))(arg0);
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
