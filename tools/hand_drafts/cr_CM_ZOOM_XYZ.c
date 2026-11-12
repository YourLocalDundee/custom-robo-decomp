/* cr_CM_ZOOM_XYZ: Nov 6, 2026 hand draft (m2c 76%): the m2c draft with the head of the matched script handlers (history store, index, count, then the PC step, with the dead (*cmd_cnt)++ in an `if (0)` block) in place of m2c's cmd_cnt pointer head */

void cr_CM_ZOOM_XYZ(void) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
    } sp;
    u8 *temp_r31_32;
    s16 *temp_r30_15;
    u32 *cmd_cnt;

    temp_r30_15 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r30_15, s16 *, 0);
    temp_r31_32 = cr_g_CurrentCam;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 0xA;
    if (0) {
        (*cmd_cnt)++;
    }
    sp.sp8 = M2C_FIELD(temp_r31_32, f32 *, 0xC8) - M2C_FIELD(temp_r31_32, f32 *, 0x98);
    sp.spC = M2C_FIELD(temp_r31_32, f32 *, 0xCC) - M2C_FIELD(temp_r31_32, f32 *, 0x9C);
    sp.sp10 = M2C_FIELD(temp_r31_32, f32 *, 0xD0) - M2C_FIELD(temp_r31_32, f32 *, 0xA0);
    cr_Vec3_Normalize(&sp.sp8);
    M2C_FIELD(temp_r31_32, f32 *, 0x98) = (f32) -((sp.sp8 * M2C_FIELD(temp_r30_15, f32 *, 0x10)) - M2C_FIELD(temp_r31_32, f32 *, 0xC8));
    M2C_FIELD(temp_r31_32, f32 *, 0x9C) = (f32) -((sp.spC * M2C_FIELD(temp_r30_15, f32 *, 0x10)) - M2C_FIELD(temp_r31_32, f32 *, 0xCC));
    M2C_FIELD(temp_r31_32, f32 *, 0xA0) = (f32) -((sp.sp10 * M2C_FIELD(temp_r30_15, f32 *, 0x10)) - M2C_FIELD(temp_r31_32, f32 *, 0xD0));
    M2C_FIELD(temp_r31_32, f32 *, 0x13B8) = (f32) M2C_FIELD(temp_r30_15, f32 *, 4);
    M2C_FIELD(temp_r31_32, f32 *, 0x13BC) = (f32) M2C_FIELD(temp_r30_15, f32 *, 8);
    M2C_FIELD(temp_r31_32, f32 *, 0x13C0) = (f32) M2C_FIELD(temp_r30_15, f32 *, 0xC);
    M2C_FIELD(temp_r31_32, s32 *, 0xFC) = (s32) M2C_FIELD(temp_r30_15, s16 *, 2);
    cr_Cam_SetMode(0xCU);
    M2C_FIELD(temp_r31_32, s8 *, 0x16A) = 3;
    M2C_FIELD(temp_r31_32, u8 *, 0x168) |= 4;
    cr_g_ScenarioSlotPtr->status |= lbl_8045110C;
}
