/* cr_LGT_POP_POS: Nov 13, 2026 hand draft: script command on the pool: the saved light positions lbl_803BAD68 are Vec [2][2], the scene lights lbl_803B8C44 a pointer table */

// also defines: lbl_803B8C44 lbl_803BAD68 cr_g_Script cr_g_Scenario

struct cr_PF_14_s32 {
    u8 pad[0x14];
    s32 v;
};

/* the file's .bss from its first object (lbl_803B8C00): scoring only (tools/pool_prelude.py) */
u8 cr_s_8C00_0000[0x44];
HSD_LObj *lbl_803B8C44[10];
u8 cr_s_8C00_006C[0x20FC];
Vec lbl_803BAD68[2][2];
u8 cr_s_8C00_2198[0x35C0];
cr_ScriptState cr_g_Script;
u8 cr_s_8C00_5BDC[0x1138];
cr_ScenarioState cr_g_Scenario;
extern void cr_Pool_Use(void *);
void cr_Pool803B8C00_Order(void) {
    cr_Pool_Use(cr_s_8C00_0000);
    cr_Pool_Use(lbl_803B8C44);
    cr_Pool_Use(cr_s_8C00_006C);
    cr_Pool_Use(lbl_803BAD68);
    cr_Pool_Use(cr_s_8C00_2198);
    cr_Pool_Use(&cr_g_Script);
    cr_Pool_Use(cr_s_8C00_5BDC);
    cr_Pool_Use(&cr_g_Scenario);
}

void cr_LGT_POP_POS(void) {
    s16 *pc;
    u32 *cmd_cnt;

    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) pc[0];
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    if (0) {
        (*cmd_cnt)++;
    }

    if (pc[2] < 2) {
        if (pc[2] == 0) {
            HSD_LObjSetPosition((HSD_LObj *) lbl_80354154[pc[1]], &lbl_803BAD68[pc[2]][pc[1]]);
        } else {
            HSD_LObjSetPosition(lbl_803B8C44[pc[1]], &lbl_803BAD68[pc[2]][pc[1]]);
        }
        if (!(cr_g_ScenarioSlotPtr->status & 0x20000)) {
            if ((s16 *) cr_g_ScriptPC != NULL) {
                if ((s32) cr_g_ScriptPC & 0x80000000) {
                    if (M2C_FIELD(cr_g_ScriptPC, s16 *, 0) < 0x12C) {
                        if (cr_g_ScriptCharaAct != 0) {
                            if (M2C_FIELD(cr_g_ScriptPC, s16 *, 0) >= 0x121) {
                                cr_g_ScrCmdFuncs[M2C_FIELD(cr_g_ScriptPC, s16 *, 0)]();
                                return;
                            }
                            ((struct cr_PF_14_s32 *) &cr_g_Scenario)->v |= lbl_80451184;
                            return;
                        }
                        if (M2C_FIELD(cr_g_ScriptPC, s16 *, 0) < 0x121) {
                            cr_g_ScrCmdFuncs[M2C_FIELD(cr_g_ScriptPC, s16 *, 0)]();
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
}
