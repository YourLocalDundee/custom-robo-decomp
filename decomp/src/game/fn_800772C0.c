/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800772C0: .text 0x800772C0-0x800773D0 (272 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
typedef struct HSD_ShapeAnimJoint HSD_ShapeAnimJoint;
typedef struct HSD_MatAnimJoint HSD_MatAnimJoint;
typedef struct HSD_AnimJoint HSD_AnimJoint;
extern u32 lbl_80379970[344];
typedef struct HSD_Joint HSD_Joint;
HSD_JObj *HSD_JObjLoadJoint(HSD_Joint *);
void HSD_JObjAddAnimAll(HSD_JObj *, HSD_AnimJoint *, HSD_MatAnimJoint *, HSD_ShapeAnimJoint *);

void fn_800772C0(HSD_JObj **arg0, s8 arg1) {
    u32 temp_r31_12;
    HSD_ShapeAnimJoint *var_r28_23;
    HSD_MatAnimJoint *var_r27_22;
    HSD_AnimJoint *var_r26_20;

    temp_r31_12 = lbl_80379970[1];
    *arg0 = HSD_JObjLoadJoint(*(*(HSD_Joint ***)((s8 *)((*(s32 *) temp_r31_12 + (arg1 * 4))) + (0))));
    var_r26_20 = NULL;
    var_r27_22 = NULL;
    var_r28_23 = NULL;
    if ((*(u32 *)((s8 *)(*(s32 *) (*(s32 *) temp_r31_12 + (arg1 * 4))) + (4))) != 0U) {
        var_r26_20 = *(*(HSD_AnimJoint ***)((s8 *)(*(s32 *) (*(s32 *) temp_r31_12 + (arg1 * 4))) + (4)));
    }
    if ((*(u32 *)((s8 *)(*(s32 *) (*(s32 *) temp_r31_12 + (arg1 * 4))) + (8))) != 0U) {
        var_r27_22 = *(*(HSD_MatAnimJoint ***)((s8 *)(*(s32 *) (*(s32 *) temp_r31_12 + (arg1 * 4))) + (8)));
    }
    if ((*(u32 *)((s8 *)(*(s32 *) (*(s32 *) temp_r31_12 + (arg1 * 4))) + (0xC))) != 0U) {
        var_r28_23 = *(*(HSD_ShapeAnimJoint ***)((s8 *)(*(s32 *) (*(s32 *) temp_r31_12 + (arg1 * 4))) + (0xC)));
    }
    HSD_JObjAddAnimAll(*arg0, var_r26_20, var_r27_22, var_r28_23);
}
