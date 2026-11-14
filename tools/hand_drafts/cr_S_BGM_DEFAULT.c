/* cr_S_BGM_DEFAULT: Nov 13, 2026 hand draft: script command without arguments, on the pool (tools/script_pool.py): the map's default music; cr_Bgm_GetVolume returns a u8, which goes through float to the volume argument */

// also defines: cr_g_Script cr_g_Scenario lbl_803BFE68

struct cr_PF_14_s32 {
    u8 pad[0x14];
    s32 v;
};

/* the file's .bss from its first object (lbl_803B8C00): scoring only (tools/pool_prelude.py) */
u8 cr_s_8C00_0000[0x5758];
cr_ScriptState cr_g_Script;
u8 cr_s_8C00_5BDC[0x1138];
cr_ScenarioState cr_g_Scenario;
u8 cr_s_8C00_7078[0x1F0];
u16 lbl_803BFE68[16];
extern void cr_Pool_Use(void *);
void cr_Pool803B8C00_Order(void) {
    cr_Pool_Use(cr_s_8C00_0000);
    cr_Pool_Use(&cr_g_Script);
    cr_Pool_Use(cr_s_8C00_5BDC);
    cr_Pool_Use(&cr_g_Scenario);
    cr_Pool_Use(cr_s_8C00_7078);
    cr_Pool_Use(lbl_803BFE68);
}

void cr_S_BGM_DEFAULT(void) {
    s16 *pc;
    u32 *cmd_cnt;

    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) pc[0];
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }

    cr_Bgm_FadeOut(0U, 0x1EU);
    cr_g_ScenarioSlotPtr->unk_ba = (u16) M2C_FIELD((lbl_8021B2FC + ((*(u16 *) &lbl_803BFE68) * 4)), u32 *, 0);
    if ((u16) cr_g_ScenarioSlotPtr->unk_ba != 0x47) {
        cr_Bgm_Play((u32) cr_g_ScenarioSlotPtr->unk_ba, 0U, (u8) (f32) (u8) cr_Bgm_GetVolume(), 0x1EU);
    }
    if (!(cr_g_ScenarioSlotPtr->status & 0x20000)) {
        if ((s16 *) cr_g_ScriptPC != NULL) {
            if ((s32) cr_g_ScriptPC & 0x80000000) {
                if ((s16) *cr_g_ScriptPC < 0x12C) {
                    if (cr_g_ScriptCharaAct != 0) {
                        if ((s16) *cr_g_ScriptPC >= 0x121) {
                            cr_g_ScrCmdFuncs[*cr_g_ScriptPC]();
                            return;
                        }
                        ((struct cr_PF_14_s32 *) &cr_g_Scenario)->v |= lbl_80451184;
                        return;
                    }
                    if ((s16) *cr_g_ScriptPC < 0x121) {
                        cr_g_ScrCmdFuncs[*cr_g_ScriptPC]();
                        return;
                    }
                    ((struct cr_PF_14_s32 *) &cr_g_Scenario)->v |= lbl_80451180;
                    return;
                }
                ((struct cr_PF_14_s32 *) &cr_g_Scenario)->v |= lbl_80451170;
                return;
            }
            ((struct cr_PF_14_s32 *) &cr_g_Scenario)->v |= lbl_80451168;
            return;
        }
        ((struct cr_PF_14_s32 *) &cr_g_Scenario)->v |= lbl_8045116C;
    }
}
