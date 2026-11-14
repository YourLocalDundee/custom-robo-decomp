/* cr_SYS_OVERLAY_WAIT: Nov 13, 2026 hand draft: the register order computed by rank_solve.py: two dead uses of the first local and one of the fourth, the locals declared in register order */

extern u8 *lbl_8044F320;
extern u32 *lbl_8044F338;
extern u8 *lbl_8044F40C;
extern u8 *lbl_8044F784;

void cr_SYS_OVERLAY_WAIT(void) {
    u8 *var_r31;
    u32 *cmd_cnt;
    s32 var_r30;
    u8 *var_r29;
    u8 *var_r28;
    u8 **var_r27;
    s16 *var_r26;

    (void) var_r31; (void) var_r31; 
    (void) var_r28; 
    var_r26 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) *var_r26;
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
    fn_80021014((void *) cr_g_Script.overlay_478);
    fn_80021014((void *) cr_g_Script.overlay_47c);
    fn_80021014((void *) cr_g_Script.overlay_480);
    var_r30 = 0;
    while (var_r30 < 0xA) {
        cr_g_Scenario.unk_258[var_r30] = 0;
        var_r30 += 1;
    }
    cr_g_Script.battle_arg = 0;
    var_r29 = lbl_8044F320 + 0x40;
    var_r31 = lbl_8044F320;
    var_r30 = 0;
    M2C_FIELD(var_r31, u8 **, 8) = (unsigned char *) ((u32) (M2C_FIELD(var_r31, u8 **, 8) + (s32) var_r29));
    M2C_FIELD(var_r31, u8 **, 0xC) = (unsigned char *) ((u32) (M2C_FIELD(var_r31, u8 **, 0xC) + (s32) var_r29));
    var_r28 = var_r29 + M2C_FIELD(var_r31, s32 *, 0x18);
    while (var_r30 < (s32) M2C_FIELD(var_r31, s32 *, 0x1C)) {
        var_r27 = (unsigned char **) (var_r29 + (*(u32 *)((s8 *)((var_r28 + (var_r30 * 4))) + (0))));
        var_r30 += 1;
        *var_r27 += (s32) var_r29;
    }
    lbl_8044F784 = M2C_FIELD(var_r31, void **, 8);
    M2C_FIELD(lbl_8044F40C, u8 **, 8) = (u8 *) lbl_8044F318;
    M2C_FIELD(lbl_8044F40C, s32 *, 0x10) = 1;
    fn_8008F8D0();
    cr_MotionFile_Register(lbl_8044F338);
    fn_800215F8((void *) cr_g_Script.overlay_478);
    fn_800215F8((void *) cr_g_Script.overlay_47c);
    fn_800215F8((void *) cr_g_Script.overlay_480);
}
