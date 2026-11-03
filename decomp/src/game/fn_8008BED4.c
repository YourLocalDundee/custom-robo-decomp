/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008BED4: .text 0x8008BED4-0x8008C054 (384 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern f32 lbl_8021A9CC[13];
extern f32 lbl_8021A9A4[10];
extern const f32 lbl_80450FF0;

/* fn_8008BED4: hand-written from main.dol's asm on its m2c draft (plan.md step 108): case 2 `break` (one shared blr) */
void fn_8008BED4(void *arg0) {

    switch ((M2C_FIELD(arg0, s32 *, 0x10))) {                            /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        M2C_FIELD(arg0, s32 *, 0x14) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x14) == 0x50) {
            M2C_FIELD(arg0, s32 *, 0x10) = 1;
            M2C_FIELD(arg0, s32 *, 0x14) = 0;
        }
        *M2C_FIELD(arg0, f32 **, 0x5C) += ((*lbl_8021A9CC - *lbl_8021A9A4) / lbl_80450FF0);
        break;
    case 1:                                         /* switch 1 */
        M2C_FIELD(arg0, s32 *, 0x14) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x14) == 0x14) {
            M2C_FIELD(arg0, s32 *, 0x10) = 0;
            M2C_FIELD(arg0, s32 *, 0x14) = 0;
            *M2C_FIELD(arg0, f32 **, 0x5C) = *lbl_8021A9A4;
        }
        break;
    }
    switch ((M2C_FIELD(arg0, s32 *, 0x18))) {                           /* switch 2; irregular */
    case 0:                                         /* switch 2 */
        M2C_FIELD(arg0, u8 *, 0x57) -= 3;
        M2C_FIELD(arg0, s32 *, 0x1C) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x1C) == 0x1E) {
            M2C_FIELD(arg0, s32 *, 0x18) += 1;
            M2C_FIELD(arg0, s32 *, 0x1C) = 0;
            return;
        }
    case 2:                                         /* switch 2 */
        break;
    case 1:                                         /* switch 2 */
        M2C_FIELD(arg0, s32 *, 0x1C) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x1C) == 0x14) {
            M2C_FIELD(arg0, s32 *, 0x18) += 1;
            M2C_FIELD(arg0, s32 *, 0x1C) = 0;
            return;
        }
        break;
    case 3:                                         /* switch 2 */
        M2C_FIELD(arg0, u8 *, 0x57) += 3;
        M2C_FIELD(arg0, s32 *, 0x1C) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x1C) == 0x1E) {
            M2C_FIELD(arg0, s32 *, 0x18) = 0;
            M2C_FIELD(arg0, s32 *, 0x1C) = 0;
            return;
        }
        break;
    }
}
