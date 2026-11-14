/* cr_E_COUT: Nov 13, 2026 hand draft: the fade-out command: as cr_E_CIN, with the colour (four bytes of the arguments to float) and four words of arguments */

// also defines: lbl_803BA1E8 lbl_803BA350 cr_g_Script

typedef struct {
    s32 flags;
    u8 pad[0x10];
    f32 r;
    f32 g;
    f32 b;
    f32 a;
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

void cr_E_COUT(void) {
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
    lbl_803BA350.r = ((u8 *) pc)[4];
    lbl_803BA350.g = ((u8 *) pc)[5];
    lbl_803BA350.b = ((u8 *) pc)[6];
    lbl_803BA350.a = ((u8 *) pc)[7];
    lbl_803BA350.t = lbl_80451280;
    lbl_803BA350.time = pc[1];
    lbl_803BA350.flags = 0xC0000001;
    cr_g_ScenarioSlotPtr->status |= lbl_80451100;
    lbl_803BA1E8[0] = 0x20;
}
