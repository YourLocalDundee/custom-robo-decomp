/* cr_CM_SET: Nov 6, 2026 hand draft (m2c 80%): the m2c draft with the head of the matched script handlers (history store, index, count, then the PC step, with the dead (*cmd_cnt)++ in an `if (0)` block) in place of m2c's cmd_cnt pointer head */

void cr_CM_SET(void) {
    u8 *temp_r31_34;
    s16 *temp_r30_17;
    u32 *cmd_cnt;

    temp_r30_17 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r30_17, s16 *, 0);
    temp_r31_34 = cr_g_CurrentCam;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 0xC;
    if (0) {
        (*cmd_cnt)++;
    }
    M2C_FIELD(temp_r31_34, f32 *, 0x98) = (f32) M2C_FIELD(temp_r30_17, f32 *, 4);
    M2C_FIELD(temp_r31_34, f32 *, 0x9C) = (f32) M2C_FIELD(temp_r30_17, f32 *, 8);
    M2C_FIELD(temp_r31_34, f32 *, 0xA0) = (f32) M2C_FIELD(temp_r30_17, f32 *, 0xC);
    M2C_FIELD(temp_r31_34, f32 *, 0x13C4) = (f32) M2C_FIELD(temp_r30_17, f32 *, 0x10);
    M2C_FIELD(temp_r31_34, f32 *, 0xC8) = (f32) M2C_FIELD(temp_r30_17, f32 *, 4);
    M2C_FIELD(temp_r31_34, f32 *, 0xCC) = (f32) M2C_FIELD(temp_r30_17, f32 *, 8);
    M2C_FIELD(temp_r31_34, f32 *, 0xD0) = (f32) M2C_FIELD(temp_r30_17, f32 *, 0xC);
    M2C_FIELD(temp_r31_34, f32 *, 0xD4) = (f32) M2C_FIELD(temp_r30_17, f32 *, 4);
    M2C_FIELD(temp_r31_34, f32 *, 0xD8) = (f32) M2C_FIELD(temp_r30_17, f32 *, 8);
    M2C_FIELD(temp_r31_34, f32 *, 0xDC) = (f32) M2C_FIELD(temp_r30_17, f32 *, 0xC);
    M2C_FIELD(temp_r31_34, f32 *, 0xE0) = (f32) M2C_FIELD(temp_r30_17, f32 *, 4);
    M2C_FIELD(temp_r31_34, f32 *, 0xE4) = (f32) M2C_FIELD(temp_r30_17, f32 *, 8);
    M2C_FIELD(temp_r31_34, f32 *, 0xE8) = (f32) M2C_FIELD(temp_r30_17, f32 *, 0xC);
    M2C_FIELD(temp_r31_34, u32 *, 0x110) = (u32) ((lbl_804514AC * M2C_FIELD(temp_r30_17, f32 *, 0x14)) / lbl_804512A8);
    M2C_FIELD(temp_r31_34, s32 *, 0xFC) = (s32) M2C_FIELD(temp_r30_17, s16 *, 2);
    cr_Cam_SetMode(0xCU);
    M2C_FIELD(temp_r31_34, s8 *, 0x16A) = 6;
    M2C_FIELD(temp_r31_34, u8 *, 0x168) |= 4;
    cr_g_ScenarioSlotPtr->status |= lbl_8045110C;
    fn_80026C50(temp_r31_34);
}
