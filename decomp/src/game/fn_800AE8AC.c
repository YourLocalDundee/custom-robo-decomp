/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800AE8AC: .text 0x800AE8AC-0x800AE910 (100 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044EB4C[8];

extern u16 *lbl_8044F728;

void fn_800AE8AC(void *arg0) {
    if (!(*lbl_8044F728 & 2)) {
        (*(M2C_UNK (**)()) (lbl_8044EB4C + (M2C_FIELD(arg0, u8 *, 0x58) * 4)))(arg0);
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
