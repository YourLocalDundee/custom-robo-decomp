/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80112940: .text 0x80112940-0x80112998 (88 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8041A244[852];

void fn_80112940(void *arg0) {
    if ((s8) lbl_8041A244[0xC] != 0) {
        if ((s32) (M2C_FIELD(arg0, u8 *, 0x57) + 0xF) < 0xFF) {
            M2C_FIELD(arg0, u8 *, 0x57) += 0xF;
        } else {
            M2C_FIELD(arg0, u8 *, 0x57) = 0xFFU;
        }
    } else {
        M2C_FIELD(arg0, u8 *, 0x57) = 0U;
    }
    M2C_FIELD(arg0, s32 *, 0) |= 0x10;
}
