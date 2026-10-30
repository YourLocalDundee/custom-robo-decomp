/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8000B100: .text 0x8000B100-0x8000B144 (68 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F374;

void fn_8000B100(void *arg0) {
    void *temp_r31_6;

    temp_r31_6 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
    if (M2C_FIELD(temp_r31_6, u16 *, 0xC) < 0x280U) {
        M2C_FIELD(temp_r31_6, u16 *, 0xC) += 0x18;
        lbl_8044F374 += 1;
    }
}
