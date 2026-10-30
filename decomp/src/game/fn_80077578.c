/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80077578: .text 0x80077578-0x80077648 (208 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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

void fn_80077578(HSD_JObj **arg0) {
    u32 temp_r31_8;
    HSD_ShapeAnimJoint *var_r29_16;
    HSD_MatAnimJoint *var_r28_15;
    HSD_AnimJoint *var_r27_14;

    temp_r31_8 = *lbl_80379970;
    *arg0 = HSD_JObjLoadJoint(**M2C_FIELD(temp_r31_8, HSD_Joint ****, 0));
    var_r27_14 = NULL;
    var_r28_15 = NULL;
    var_r29_16 = NULL;
    if ((*(u32 *)((s8 *)((*(HSD_Joint ***)((s8 *)(*(s32 *) temp_r31_8) + (0)))) + (4))) != 0U) {
        var_r27_14 = *(*(HSD_AnimJoint ***)((s8 *)((*(HSD_Joint ***)((s8 *)(*(s32 *) temp_r31_8) + (0)))) + (4)));
    }
    if ((*(u32 *)((s8 *)((*(HSD_Joint ***)((s8 *)(*(s32 *) temp_r31_8) + (0)))) + (8))) != 0U) {
        var_r28_15 = *(*(HSD_MatAnimJoint ***)((s8 *)((*(HSD_Joint ***)((s8 *)(*(s32 *) temp_r31_8) + (0)))) + (8)));
    }
    if ((*(u32 *)((s8 *)((*(HSD_Joint ***)((s8 *)(*(s32 *) temp_r31_8) + (0)))) + (0xC))) != 0U) {
        var_r29_16 = *(*(HSD_ShapeAnimJoint ***)((s8 *)((*(HSD_Joint ***)((s8 *)(*(s32 *) temp_r31_8) + (0)))) + (0xC)));
    }
    HSD_JObjAddAnimAll(*arg0, var_r27_14, var_r28_15, var_r29_16);
}
