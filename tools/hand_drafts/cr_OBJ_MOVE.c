/* cr_OBJ_MOVE: Nov 13, 2026 hand draft: script command on the pool: the object table lbl_803BC028 is 24 records of 0x174 bytes; the search by id is an inline (obj_find: u16 counter, the pointer and the id tested, the s16 argument copied); the result starts as NULL; m2c's comma condition written as statements */

// also defines: lbl_803BC028 cr_g_Script cr_g_Scenario

struct cr_PF_14_s32 {
    u8 pad[0x14];
    s32 v;
};

/* the file's .bss from its first object (lbl_803B8C00): scoring only (tools/pool_prelude.py) */
u8 cr_s_8C00_0000[0x3428];
typedef struct {
    void *ptr;
    u8 pad4[4];
    Vec pos;
    Vec step;
    Vec target;
    u16 time;
    u8 pad2E[0x11E];
    u16 id;
    u8 pad14E[0xE];
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

static inline Obj8C00 *obj_find(s16 id) {
    u16 i;

    for (i = 0; i < 0x18U; i++) {
        if ((lbl_803BC028[i].ptr != ((void *) 0)) && (id == lbl_803BC028[i].id)) {
            return &lbl_803BC028[i];
        }
    }
    return NULL;
}

void cr_OBJ_MOVE(void) {
    Obj8C00 *o;
    s16 *pc;
    u32 *cmd_cnt;

    pc = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) pc[0];
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 0xA;
    if (0) {
        (*cmd_cnt)++;
    }
    o = NULL;
    o = obj_find(pc[1]);
    if (o != NULL) {
        o->flags |= 2;
        o->target.x = M2C_FIELD(pc, f32 *, 8);
        o->target.y = M2C_FIELD(pc, f32 *, 0xC);
        o->target.z = M2C_FIELD(pc, f32 *, 0x10);
        o->time = pc[2];
        o->step.x = (o->target.x - o->pos.x) / (f32) o->time;
        o->step.y = (o->target.y - o->pos.y) / (f32) o->time;
        o->step.z = (o->target.z - o->pos.z) / (f32) o->time;
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
