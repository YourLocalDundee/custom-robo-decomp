/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800B5418: .text 0x800B5418-0x800B54E8 (208 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 *lbl_8044F738;
extern u8 lbl_803BA3A8[2368];
extern u8 lbl_803BACE8[128];
typedef struct HSD_JObj HSD_JObj;
typedef struct HSD_Joint HSD_Joint;
HSD_JObj *HSD_JObjLoadJoint(HSD_Joint *);

void fn_800B5418(s32 arg0) {
    u8 var_r31_13;
    s32 *temp_r30_23;

    var_r31_13 = 0;
    lbl_8044F738 = lbl_803BA3A8 + (arg0 * 0x94);
    temp_r30_23 = M2C_FIELD((lbl_803BACE8 + (M2C_FIELD(lbl_8044F738, s16 *, 0x80) * 8)), s32 **, 4);
    M2C_FIELD(lbl_8044F738, u8 *, 1) = 0U;
    while (var_r31_13 < 2U) {
        if ((u32) *(u32 *) (*temp_r30_23 + ((var_r31_13 * 4) & 0x3FC)) != 0U) {
            M2C_FIELD(&lbl_8044F738[(var_r31_13 * 4) & 0x3FC], HSD_JObj **, 0x20) = HSD_JObjLoadJoint(*M2C_FIELD((*temp_r30_23 + ((var_r31_13 * 4) & 0x3FC)), HSD_Joint ***, 0));
            M2C_FIELD(lbl_8044F738, u8 *, 1) += 1;
        }
        var_r31_13 += 1;
    }
}
