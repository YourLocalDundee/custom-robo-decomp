/* cr_SYS_RET: Nov 13, 2026 hand draft: script command on the pool (tools/script_pool.py): the return address read with the depth pre-decremented in the index (cr_g_ScriptReturnStack[--cr_g_ScriptDepth]), after the call count is decremented; the (u8) casts of the m2c draft on byte members removed */

// also defines: cr_g_Script cr_g_Scenario lbl_803BFE68

struct cr_PF_458_s32 {
    u8 pad[0x458];
    s32 v;
};
struct cr_PF_14_s32 {
    u8 pad[0x14];
    s32 v;
};
struct cr_PF_3E8_s8 {
    u8 pad[0x3E8];
    s8 v;
};
struct cr_PF_2_u16 {
    u8 pad[0x2];
    u16 v;
};
struct cr_PF_4_u16 {
    u8 pad[0x4];
    u16 v;
};
struct cr_PF_18_void_p {
    u8 pad[0x18];
    void * v;
};
struct cr_PF_1C_void_p {
    u8 pad[0x1C];
    void * v;
};
struct cr_PF_18_cr_FileSlot_p {
    u8 pad[0x18];
    cr_FileSlot * v;
};
struct cr_PF_1C_cr_FileSlot_p {
    u8 pad[0x1C];
    cr_FileSlot * v;
};
struct cr_PF_4_s16 {
    u8 pad[0x4];
    s16 v;
};
struct cr_PF_2F_u8 {
    u8 pad[0x2F];
    u8 v;
};
struct cr_PF_2D_u8 {
    u8 pad[0x2D];
    u8 v;
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

void cr_SYS_RET(void) {
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
    if ((u32) cr_g_ScriptDepth != 0U) {
        ((struct cr_PF_458_s32 *) &cr_g_Script)->v -= 1;
        cr_g_ScriptPC = cr_g_ScriptReturnStack[--cr_g_ScriptDepth];
        if (cr_g_ScenarioSlotPtr->status & 0x2000) {
            if ((u32) cr_g_ScriptDepth == (u32) ((struct cr_PF_3E8_s8 *) &cr_g_Script)->v) {
                if ((((struct cr_PF_2_u16 *) &lbl_803BFE68)->v != 0x1B) && (((struct cr_PF_4_u16 *) &lbl_803BFE68)->v != 0x1B)) {
                    if (((struct cr_PF_4_u16 *) &lbl_803BFE68)->v != 0xFFFF) {
                        fn_800208F0(((struct cr_PF_18_void_p *) &lbl_803BFE68)->v);
                        fn_800215F8(((struct cr_PF_18_void_p *) &lbl_803BFE68)->v);
                        fn_800208F0(((struct cr_PF_1C_void_p *) &lbl_803BFE68)->v);
                        fn_800215F8(((struct cr_PF_1C_void_p *) &lbl_803BFE68)->v);
                    }
                    if (((struct cr_PF_2F_u8 *) &cr_g_Scenario)->v == 0) {
                        ((struct cr_PF_18_cr_FileSlot_p *) &lbl_803BFE68)->v = cr_Archive_OpenEntry(4, 0x1BU);
                    } else {
                        ((struct cr_PF_18_cr_FileSlot_p *) &lbl_803BFE68)->v = cr_Archive_OpenEntry(4, 0x4AU);
                    }
                    cr_FileSlot_QueueRead(((struct cr_PF_18_cr_FileSlot_p *) &lbl_803BFE68)->v, lbl_8044F2B8);
                    if (((struct cr_PF_2F_u8 *) &cr_g_Scenario)->v == 0) {
                        ((struct cr_PF_1C_cr_FileSlot_p *) &lbl_803BFE68)->v = cr_Archive_OpenEntry(5, 0x1BU);
                    } else {
                        ((struct cr_PF_1C_cr_FileSlot_p *) &lbl_803BFE68)->v = cr_Archive_OpenEntry(6, 0x1BU);
                    }
                    cr_FileSlot_QueueRead(((struct cr_PF_1C_cr_FileSlot_p *) &lbl_803BFE68)->v, lbl_8044F2AC);
                    ((struct cr_PF_4_s16 *) &lbl_803BFE68)->v = 0x1B;
                    ((struct cr_PF_2_u16 *) &lbl_803BFE68)->v = 0xFFFF;
                    ((struct cr_PF_2D_u8 *) &cr_g_Scenario)->v = 0xC;
                    cr_g_FirstChara->flags |= 0x80000000;
                    cr_g_ScenarioSlotPtr->status |= lbl_80451128;
                    if ((s32) cr_g_ScenarioSlotPtr->unk_ba == (s32) M2C_FIELD(lbl_8021B2FC, s32 *, 0x6C)) {
                        cr_g_ScenarioSlotPtr->unk_bc[0] |= 2;
                    }
                }
                cr_g_ScenarioSlotPtr->status &= 0xFFFFDFFF;
            }
        } else if (!(cr_g_ScenarioSlotPtr->status & 0x4000)) {
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
        } else {
            cr_g_ScenarioSlotPtr->status &= 0xFFFFBFFF;
            cr_g_ScenarioSlotPtr->status |= lbl_80451128;
            ((struct cr_PF_2D_u8 *) &cr_g_Scenario)->v = 0xA;
        }
    } else {
        ((struct cr_PF_14_s32 *) &cr_g_Scenario)->v |= lbl_8045115C;
    }
}
