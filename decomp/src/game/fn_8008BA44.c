/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008BA44: .text 0x8008BA44-0x8008BBC8 (388 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern f32 lbl_8021A9A4[10];
extern f32 lbl_8021A9CC[13];
extern f32 lbl_80450FB8;

void fn_8008BA44(void *arg0) {

    switch ((M2C_FIELD(arg0, s32 *, 0x10))) {                            /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        M2C_FIELD(arg0, s32 *, 0x14) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x14) == 0x28) {
            M2C_FIELD(arg0, s32 *, 0x10) = 1;
            M2C_FIELD(arg0, s32 *, 0x14) = 0;
        }
        M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) -= ((lbl_8021A9A4[7] - lbl_8021A9CC[7]) / lbl_80450FB8);
        break;
    case 1:                                         /* switch 1 */
        M2C_FIELD(arg0, s32 *, 0x14) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x14) == 0x3C) {
            M2C_FIELD(arg0, s32 *, 0x10) = 0;
            M2C_FIELD(arg0, s32 *, 0x14) = 0;
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_8021A9A4[7];
        }
        break;
    }
    switch ((M2C_FIELD(arg0, s32 *, 0x18))) {                           /* switch 2; irregular */
    case 0:                                         /* switch 2 */
        M2C_FIELD(arg0, u8 *, 0x57) -= 5;
        M2C_FIELD(arg0, s32 *, 0x1C) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x1C) == 0x14) {
            M2C_FIELD(arg0, s32 *, 0x18) += 1;
            M2C_FIELD(arg0, s32 *, 0x1C) = 0;
        }
        return;
    case 1:                                         /* switch 2 */
        M2C_FIELD(arg0, u8 *, 0x57) += 5;
        M2C_FIELD(arg0, s32 *, 0x1C) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x1C) == 0x14) {
            M2C_FIELD(arg0, s32 *, 0x18) += 1;
            M2C_FIELD(arg0, s32 *, 0x1C) = 0;
            return;
        }
        break;
    case 2:                                         /* switch 2 */
        M2C_FIELD(arg0, s32 *, 0x1C) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x1C) == 0x3C) {
            M2C_FIELD(arg0, s32 *, 0x18) = 0;
            M2C_FIELD(arg0, s32 *, 0x1C) = 0;
            return;
        }
        break;
    }
}
