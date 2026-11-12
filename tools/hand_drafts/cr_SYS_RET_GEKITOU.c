/* cr_SYS_RET_GEKITOU: Nov 6, 2026 hand draft (m2c 83%): the sprintf buffer is char[0x108] (m2c's s8 sp8 gave a 48-byte frame, the original 304), sprintf has three arguments (m2c added the format again as a fourth), the script PC is a local in the head (pc = cr_g_ScriptPC; the history reads *pc: one more saved register), and the script table is read as an array of 8-byte records { pc, name } (the index sum then lwzx, as the original) in a for loop */


extern u32 *lbl_8044F338;
extern u32 *lbl_8044F788;

typedef struct { u32 pc; char *name; } cr_ScrEntry;
void cr_SYS_RET_GEKITOU(void) {
    s16 *pc;
    u32 *cmd_cnt;
    char sp8[0x108];
    u16 *var_r31_113;
    s32 var_r30_49;
    u32 *temp_r29_108;
    u32 var_r28_99;
    u32 var_r27_107;

    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(pc, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
    cr_Bgm_FadeOut(0U, 0xFU);
    sprintf(sp8, lbl_801C3C84, cr_g_Script.unk_454 + 1);
    for (var_r30_49 = 0; (u32) &((cr_ScrEntry *) lbl_8044F788)[var_r30_49] < (u32) *lbl_8044F788; var_r30_49 += 1) {
        if (strcmp(((cr_ScrEntry *) lbl_8044F788)[var_r30_49].name, sp8) == 0) {
            cr_g_ScriptPC = (short *) ((cr_ScrEntry *) lbl_8044F788)[var_r30_49].pc;
            cr_g_Script.script_name[cr_g_ScriptDepth] = ((cr_ScrEntry *) lbl_8044F788)[var_r30_49].name;
            cr_g_Script.cmd_count[cr_g_ScriptDepth] = 0;
        }
    }
    cr_g_ScenarioSlotPtr->unk_84[3] = 0;
    if ((s32) cr_g_Script.unk_454 != -1) {
        var_r28_99 = 0;
        while (var_r28_99 < (u32) *lbl_8044F338) {
            var_r27_107 = 0U;
            temp_r29_108 = ((u32 *) ((u8 *) lbl_8044F338 + M2C_FIELD(&lbl_8044F338[var_r28_99], s32 *, 4)));
            var_r31_113 = (unsigned short *) (temp_r29_108 + *temp_r29_108 * 3 + 1);
            while (var_r27_107 < (u32) *temp_r29_108) {
                if ((u16) *var_r31_113 != 0xFFFF) {
                    cr_g_MotionDataBit15[*var_r31_113 & 0x7FFF] = 0;
                    *(((u32 * *) ((u8 *) cr_g_MotionData + ((*var_r31_113 * 4) & 0x1FFFC)))) = 0;
                }
                var_r27_107 += 1;
                var_r31_113 += 1;
            }
            var_r28_99 += 1;
        }
    }
    cr_g_Script.unk_454 = -1;
    if (!(cr_g_ScenarioSlotPtr->status & 0x20000)) {
        if ((s16 *) cr_g_ScriptPC != NULL) {
            if ((s32) cr_g_ScriptPC & 0x80000000) {
                if ((s16) *cr_g_ScriptPC < 0x12C) {
                    if (cr_g_ScriptCharaAct != 0) {
                        if ((s16) *cr_g_ScriptPC >= 0x121) {
                            cr_g_ScrCmdFuncs[*cr_g_ScriptPC]();
                            return;
                        }
                        cr_g_Scenario.error_flags |= lbl_80451184;
                        return;
                    }
                    if ((s16) *cr_g_ScriptPC < 0x121) {
                        cr_g_ScrCmdFuncs[*cr_g_ScriptPC]();
                        return;
                    }
                    cr_g_Scenario.error_flags |= lbl_80451180;
                    return;
                }
                cr_g_Scenario.error_flags |= lbl_80451170;
                return;
            }
            cr_g_Scenario.error_flags |= lbl_80451168;
            return;
        }
        cr_g_Scenario.error_flags |= lbl_8045116C;
    }
}
