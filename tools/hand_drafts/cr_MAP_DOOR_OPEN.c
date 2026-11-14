/* cr_MAP_DOOR_OPEN: Nov 13, 2026 hand draft: script command on the pool (tools/script_pool.py): the door table lbl_803BA3A8 is 16 records of 0x94 bytes (active, flags at 4), indexed by the argument; flags |= 8 */

// also defines: lbl_803BA3A8 cr_g_Script cr_g_Scenario

struct cr_PF_14_s32 {
    u8 pad[0x14];
    s32 v;
};

/* the file's .bss from its first object (lbl_803B8C00): scoring only (tools/pool_prelude.py) */
u8 cr_s_8C00_0000[0x17A8];
typedef struct {
    u8 active;
    u8 pad1[3];
    s32 flags;
    u8 pad8[0x8C];
} Door8C00;
Door8C00 lbl_803BA3A8[16];
u8 cr_s_8C00_20E8[0x3670];
cr_ScriptState cr_g_Script;
u8 cr_s_8C00_5BDC[0x1138];
cr_ScenarioState cr_g_Scenario;
extern void cr_Pool_Use(void *);
void cr_Pool803B8C00_Order(void) {
    cr_Pool_Use(cr_s_8C00_0000);
    cr_Pool_Use(lbl_803BA3A8);
    cr_Pool_Use(cr_s_8C00_20E8);
    cr_Pool_Use(&cr_g_Script);
    cr_Pool_Use(cr_s_8C00_5BDC);
    cr_Pool_Use(&cr_g_Scenario);
}

void cr_MAP_DOOR_OPEN(void) {
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

    if (lbl_803BA3A8[pc[1]].active != 0) {
        lbl_803BA3A8[pc[1]].flags |= 8;
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
