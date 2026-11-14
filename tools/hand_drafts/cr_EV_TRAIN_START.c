/* cr_EV_TRAIN_START: Nov 13, 2026 hand draft: script command without arguments, on the pool: the object table typed (pointer, state, kind at 0x14E); the first object of kind 0x30 gets state 2 and the train sound */

// also defines: lbl_803BC028 cr_g_Script cr_g_Scenario

struct cr_PF_14_s32 {
    u8 pad[0x14];
    s32 v;
};

/* the file's .bss from its first object (lbl_803B8C00): scoring only (tools/pool_prelude.py) */
u8 cr_s_8C00_0000[0x3428];
typedef struct {
    void *ptr;
    s32 state;
    Vec pos;
    Vec step;
    Vec target;
    u16 time;
    u8 pad2E[0x11E];
    u16 id;
    u16 kind;
    u8 pad150[0xC];
    s32 flags;
    u8 pad160[0x14];
} Obj8C00;
Obj8C00 lbl_803BC028[24];
u8 cr_s_8C00_5708[0x50];
cr_ScriptState cr_g_Script;
u8 cr_s_8C00_5BDC[0x1138];
cr_ScenarioState cr_g_Scenario;
extern void cr_Pool_Use(void *);
void cr_Pool803B8C00_Order(void) {
    cr_Pool_Use(cr_s_8C00_0000);
    cr_Pool_Use(lbl_803BC028);
    cr_Pool_Use(cr_s_8C00_5708);
    cr_Pool_Use(&cr_g_Script);
    cr_Pool_Use(cr_s_8C00_5BDC);
    cr_Pool_Use(&cr_g_Scenario);
}

void cr_EV_TRAIN_START(void) {
    s32 var_r30_12;
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
    var_r30_12 = 0;
    for (; var_r30_12 < 0x18; var_r30_12 += 1) {
        if ((lbl_803BC028[var_r30_12].ptr != ((void *) 0)) && (lbl_803BC028[var_r30_12].kind == 0x30)) {
            fn_80098104(0x40077, 0x7FU);
            lbl_803BC028[var_r30_12].state = 2;
            break;
        }
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
