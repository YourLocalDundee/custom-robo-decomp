/* fn_801010A8: Nov 6, 2026 hand draft (m2c 87%): the identity matrix local is a whole Mtx (m2c's f32[4] made the frame 32 bytes short), the current robo pointer is set before the index (as in fn_80101950), and one statement moved by the adjacent-swap sweep */

void fn_801010A8(u8 arg0) {
    Mtx spC;
    u32 sp8;
    HSD_JObj *temp_r31_73;
    void *temp_r30_12;
    s32 var_r28_101;
    u8 temp_r27_70;

    sp8 = 0;
    temp_r30_12 = lbl_80407260 + (arg0 * 0x4994);
    if ((M2C_FIELD(temp_r30_12, s32 *, 0x498C) & 0x200) && (HSD_LObjSetCurrentAll(lbl_8044F7F8), HSD_LObjSetupInit((struct HSD_CObj *) (*(((u32 *) ((u8 *) lbl_80323270 + ((arg0 * 4) & 0x3FC)))))), cr_g_CurrentBattleRobo = (u8 *) &cr_g_BattleRobos[arg0], cr_g_BattleRoboIndex = (s32) arg0, PSMTXCopy((float (*)[4]) (lbl_8032338C + (M2C_FIELD(temp_r30_12, u8 *, 0x4854) * 0x30)), (float (*)[4]) ((f32 (*)[4]) lbl_8032344C)), (((M2C_FIELD(temp_r30_12, s32 *, 0x498C) & 0x8000) == 0) != 0)) && !(M2C_FIELD(temp_r30_12, s32 *, 0x498C) & 0x40)) {
        if (M2C_FIELD(temp_r30_12, s32 *, 0x498C) & 0x80) {
            (void) 0;
            return;
        }
        if (M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x8B0) != 0U) {
            fn_80038CF8(&sp8);
        }
        if ((lbl_8044F7F1 <= 2U) || (M2C_FIELD(lbl_80407250, u16 *, 2) & 0x80)) {
            temp_r27_70 = M2C_FIELD(temp_r30_12, u8 *, 0x4854);
            PSMTXIdentity(spC);
            temp_r31_73 = lbl_8044F800;
            (temp_r31_73 != NULL) ? (void) 0 : __assert(lbl_804517EC, 0x495U, lbl_804517F3);
            PSMTXCopy(spC, (f32 (*)[4]) temp_r31_73->mtx[0]);
            temp_r31_73->flags |= 0x03800000;
            if (temp_r31_73 != NULL) {
                (temp_r31_73 != NULL) ? (void) 0 : __assert(lbl_804517EC, 0x25DU, lbl_804517F3);
                var_r28_101 = 0;
                if (!(temp_r31_73->flags & 0x800000) && (temp_r31_73->flags & 0x40)) {
                    var_r28_101 = 1;
                }
                if (var_r28_101 == 0) {
                    HSD_JObjSetMtxDirtySub(temp_r31_73);
                }
            }
            __GXSetIndirectMask(0U);
            HSD_JObjDispAll(lbl_8044F800, (float (*)[4]) (lbl_8032338C + (temp_r27_70 * 0x30)), (unsigned long) ((float (*)[4]) 1U), (unsigned long) ((float (*)[4]) 0U));
        }
    }
}
