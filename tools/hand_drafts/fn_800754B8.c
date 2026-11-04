/* fn_800754B8: Nov 3, 2026 hand draft (m2c 96%): the copy of arg0 written as `arg0 = arg0;` (found by decomp-permuter on fn_80075258; m2c's local copy was spilled through the stack) */


void fn_800754B8(HSD_JObj **arg0, s8 arg1) {
    s32 *var_r31;
    HSD_ShapeAnimJoint *var_r28;
    HSD_MatAnimJoint *var_r27;
    HSD_AnimJoint *var_r26;
    u8 var_r25;

    arg0 = arg0;
    var_r25 = M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C);
    var_r31 = M2C_FIELD(lbl_80377EC0, s32 **, (var_r25 * 4) & 0x3FC);
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
    fn_80094DD8(*arg0, *lbl_80450C58);
}
