/* cr_LGT_SET_COLOR: Nov 13, 2026 hand draft: script command on the pool: the light record typed; the current colour read into a GXColor local, converted to float for the record and converted again (from the byte) for the reserved colour table */

// also defines: lbl_803B8C44 lbl_803BAD98 lbl_803BADA8 cr_g_Script cr_g_Scenario

struct cr_PF_14_s32 {
    u8 pad[0x14];
    s32 v;
};

/* the file's .bss from its first object (lbl_803B8C00): scoring only (tools/pool_prelude.py) */
u8 cr_s_8C00_0000[0x44];
HSD_LObj *lbl_803B8C44[10];
u8 cr_s_8C00_006C[0x212C];
GXColor lbl_803BAD98[2][2];
typedef struct {
    s32 flags;
    u8 pad4[0x28];
    f32 cur[4];
    f32 target[4];
    f32 step[4];
    u16 time;
    u8 pad5E[2];
} Light8C00;
Light8C00 lbl_803BADA8[2][2];
u8 cr_s_8C00_2328[0x3430];
cr_ScriptState cr_g_Script;
u8 cr_s_8C00_5BDC[0x1138];
cr_ScenarioState cr_g_Scenario;
extern void cr_Pool_Use(void *);
void cr_Pool803B8C00_Order(void) {
    cr_Pool_Use(cr_s_8C00_0000);
    cr_Pool_Use(lbl_803B8C44);
    cr_Pool_Use(cr_s_8C00_006C);
    cr_Pool_Use(lbl_803BAD98);
    cr_Pool_Use(lbl_803BADA8);
    cr_Pool_Use(cr_s_8C00_2328);
    cr_Pool_Use(&cr_g_Script);
    cr_Pool_Use(cr_s_8C00_5BDC);
    cr_Pool_Use(&cr_g_Scenario);
}

void cr_LGT_SET_COLOR(void) {
    GXColor sp8;
    Light8C00 *l;
    s16 *pc;
    u32 *cmd_cnt;

    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) pc[0];
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 0xC;
    if (0) {
        (*cmd_cnt)++;
    }
    if (pc[2] < 2) {
        l = &lbl_803BADA8[pc[2]][pc[1]];
        l->flags |= 2;
        if (pc[2] == 0) {
            HSD_LObjGetColor((HSD_LObj *) lbl_80354154[pc[1]], &sp8);
        } else {
            HSD_LObjGetColor(lbl_803B8C44[pc[1]], &sp8);
        }
        l->target[0] = M2C_FIELD(pc, f32 *, 8);
        l->target[1] = M2C_FIELD(pc, f32 *, 0xC);
        l->target[2] = M2C_FIELD(pc, f32 *, 0x10);
        l->target[3] = M2C_FIELD(pc, f32 *, 0x14);
        l->time = pc[3];
        l->cur[0] = sp8.r;
        l->cur[1] = sp8.g;
        l->cur[2] = sp8.b;
        l->cur[3] = sp8.a;
        lbl_803BAD98[pc[2]][pc[1]].r = (f32) sp8.r;
        lbl_803BAD98[pc[2]][pc[1]].g = (f32) sp8.g;
        lbl_803BAD98[pc[2]][pc[1]].b = (f32) sp8.b;
        lbl_803BAD98[pc[2]][pc[1]].a = (f32) sp8.a;
        l->step[0] = (l->target[0] - l->cur[0]) / (f32) l->time;
        l->step[1] = (l->target[1] - l->cur[1]) / (f32) l->time;
        l->step[2] = (l->target[2] - l->cur[2]) / (f32) l->time;
        l->step[3] = (l->target[3] - l->cur[3]) / (f32) l->time;
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
