/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80075258: .text 0x80075258-0x80075388 (304 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
typedef struct HSD_ShapeAnimJoint HSD_ShapeAnimJoint;
typedef struct HSD_MatAnimJoint HSD_MatAnimJoint;
typedef struct HSD_AnimJoint HSD_AnimJoint;
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
extern u8 lbl_80377EB0[16];
typedef struct HSD_Joint HSD_Joint;
HSD_JObj *HSD_JObjLoadJoint(HSD_Joint *);
void HSD_JObjAddAnimAll(HSD_JObj *, HSD_AnimJoint *, HSD_MatAnimJoint *, HSD_ShapeAnimJoint *);
void fn_8009496C(HSD_JObj *arg0, f32 farg0);
extern const f32 lbl_80450C58[2];

/* fn_80075258: Nov 3, 2026 hand draft (m2c 96%): the copy of arg0 written as `arg0 = arg0;` (found by decomp-permuter on fn_80075258; m2c's local copy was spilled through the stack) */


void fn_80075258(HSD_JObj **arg0, s8 arg1) {
    s32 *var_r31;
    HSD_ShapeAnimJoint *var_r28;
    HSD_MatAnimJoint *var_r27;
    HSD_AnimJoint *var_r26;
    u8 var_r25;

    arg0 = arg0;
    var_r25 = M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C);
    var_r31 = M2C_FIELD(lbl_80377EB0, s32 **, (var_r25 * 4) & 0x3FC);
    *arg0 = HSD_JObjLoadJoint(*M2C_FIELD((*var_r31 + (arg1 * 4)), HSD_Joint ***, 0));
    var_r26 = NULL;
    var_r27 = NULL;
    var_r28 = NULL;
    if (M2C_FIELD(M2C_FIELD((*var_r31 + (arg1 * 4)), u32 *, 0), u32 *, 4) != 0U) {
        var_r26 = *M2C_FIELD(M2C_FIELD((*var_r31 + (arg1 * 4)), u32 *, 0), HSD_AnimJoint ***, 4);
    }
    if (M2C_FIELD(M2C_FIELD((*var_r31 + (arg1 * 4)), u32 *, 0), u32 *, 8) != 0U) {
        var_r27 = *M2C_FIELD(M2C_FIELD((*var_r31 + (arg1 * 4)), u32 *, 0), HSD_MatAnimJoint ***, 8);
    }
    if (M2C_FIELD(M2C_FIELD((*var_r31 + (arg1 * 4)), u32 *, 0), u32 *, 0xC) != 0U) {
        var_r28 = *M2C_FIELD(M2C_FIELD((*var_r31 + (arg1 * 4)), u32 *, 0), HSD_ShapeAnimJoint ***, 0xC);
    }
    HSD_JObjAddAnimAll(*arg0, var_r26, var_r27, var_r28);
    fn_8009496C(*arg0, *lbl_80450C58);
}
