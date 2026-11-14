/* cr_E_CIN: Nov 13, 2026 hand draft: the fade-in command on the script file's .bss pool (cr_g_Script, the fade record lbl_803BA350 and lbl_803BA1E8 defined at their offsets, scoring only); the prologue's history store has the cast written, (s16) pc[0]: without it one temporary is missing and the volatile registers of the whole block shift (81%); the order t, time, flags */

// also defines: lbl_803BA1E8 lbl_803BA350 cr_g_Script

typedef struct {
    s32 flags;
    u8 pad[0x20];
    f32 time;
    f32 t;
} Fade8C00;

/* the file's .bss from its first object (lbl_803B8C00): scoring only (tools/pool_prelude.py) */
u8 cr_s_8C00_0000[0x15E8];
u32 lbl_803BA1E8[82];
u8 cr_s_8C00_1730[0x20];
Fade8C00 lbl_803BA350;
u8 cr_s_8C00_177C[0x3FDC];
cr_ScriptState cr_g_Script;
extern void cr_Pool_Use(void *);
void cr_Pool803B8C00_Order(void) {
    cr_Pool_Use(cr_s_8C00_0000);
    cr_Pool_Use(lbl_803BA1E8);
    cr_Pool_Use(cr_s_8C00_1730);
    cr_Pool_Use(&lbl_803BA350);
    cr_Pool_Use(cr_s_8C00_177C);
    cr_Pool_Use(&cr_g_Script);
}

void cr_E_CIN(void) {
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
    lbl_803BA350.t = lbl_80451280;
    lbl_803BA350.time = pc[1];
    lbl_803BA350.flags = 0xC0000002;
    cr_g_ScenarioSlotPtr->status |= lbl_80451100;
    lbl_803BA1E8[0] = 0x10;
}
