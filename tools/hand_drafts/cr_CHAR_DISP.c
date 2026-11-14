/* cr_CHAR_DISP: Nov 13, 2026 hand draft: script command on the pool: the character search is the inline of cr_CHAR_TURN_POINT (s32 id); some member reads of the character go through a cast (the pointer then ranks below the pool base) */

// also defines: cr_g_Script cr_g_Scenario lbl_803BFE40

struct cr_PF_14_s32 {
    u8 pad[0x14];
    s32 v;
};

/* the file's .bss from its first object (lbl_803B8C00): scoring only (tools/pool_prelude.py) */
u8 cr_s_8C00_0000[0x5758];
cr_ScriptState cr_g_Script;
u8 cr_s_8C00_5BDC[0x1138];
cr_ScenarioState cr_g_Scenario;
u8 cr_s_8C00_7078[0x1C8];
u32 lbl_803BFE40[10];
extern void cr_Pool_Use(void *);
void cr_Pool803B8C00_Order(void) {
    cr_Pool_Use(cr_s_8C00_0000);
    cr_Pool_Use(&cr_g_Script);
    cr_Pool_Use(cr_s_8C00_5BDC);
    cr_Pool_Use(&cr_g_Scenario);
    cr_Pool_Use(cr_s_8C00_7078);
    cr_Pool_Use(lbl_803BFE40);
}

static inline cr_Chara *chara_find(s32 id) {
    s32 i;

    for (i = 0; i < 0x18; i++) {
        if ((((void (*)()) cr_g_Charas[i].update) != NULL) && (id == (s32) cr_g_Charas[i].id)) {
            return &cr_g_Charas[i];
        }
    }
    return NULL;
}

void cr_CHAR_DISP(void) {
    s16 *pc;
    u32 *cmd_cnt;
    cr_Chara *var_r27_55;

    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) pc[0];
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
    var_r27_55 = chara_find(pc[1]);
    if (var_r27_55 != NULL) {
        ((cr_Chara *) var_r27_55)->flags &= 0xFFEFFFFF;
        ((cr_Chara *) var_r27_55)->unk_0bc6 = 3;
    } else {
        ((struct cr_PF_14_s32 *) &cr_g_Scenario)->v |= 0x6000;
    }
    if (((*(u32 *) &lbl_803BFE40) != 0U) && !(((cr_Chara *) var_r27_55)->flags_1942 & 0x10) && ((HSD_JObj *) ((cr_Chara *) var_r27_55)->root_joint != NULL)) {
        fn_8006E6E8(((cr_Chara *) var_r27_55)->root_joint);
        ((cr_Chara *) var_r27_55)->flags_1942 |= 0x10;
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
