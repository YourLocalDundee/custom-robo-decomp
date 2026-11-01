/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800B4710: .text 0x800B4710-0x800B47F0 (224 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_803BA3A8[2368];
extern u8 *lbl_8044F738;
typedef struct HSD_JObj HSD_JObj;
typedef f32 Mtx[3][4];
void HSD_JObjDispAll(HSD_JObj *jobj, Mtx vmtx, u32 flags, u32 rendermode);
extern f32 lbl_8032344C[237];

void fn_800B4710(void) {
    u8 var_r31_6;
    u8 var_r30_32;

    var_r31_6 = 0;
    while (var_r31_6 < 0x10U) {
        if ((u8) *(lbl_803BA3A8 + (var_r31_6 * 0x94)) == 1) {
            lbl_8044F738 = lbl_803BA3A8 + (var_r31_6 * 0x94);
            if (!(M2C_FIELD(lbl_8044F738, s32 *, 4) & 0x200000) && !(M2C_FIELD(lbl_8044F738, s32 *, 4) & 0x400000)) {
                var_r30_32 = 0;
                while (var_r30_32 < (u8) M2C_FIELD(lbl_8044F738, u8 *, 1)) {
                    HSD_JObjDispAll(*(HSD_JObj **) (lbl_8044F738 + 0x20 + var_r30_32 * 4), (f32 (*)[4]) lbl_8032344C, 5U, 0U);
                    var_r30_32 += 1;
                }
            }
        }
        var_r31_6 += 1;
    }
}
