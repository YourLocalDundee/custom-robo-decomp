/* cr_MAP_LINK: Nov 13, 2026 hand draft: as cr_MAP_NEXT_SET, with the link position (two s16 arguments to float) and four words of arguments */

// also defines: cr_g_Script cr_g_Scenario lbl_803BFE68
typedef struct {
    u16 cur;
    u16 pending;
    u16 next;
    u16 unk6;
    s16 change;
    u8 padA[2];
    f32 x;
    f32 y;
    u8 pad14[4];
    cr_FileSlot *a;
    cr_FileSlot *b;
} Map8C00;

/* the file's .bss from its first object (lbl_803B8C00): scoring only (tools/pool_prelude.py) */
u8 cr_s_8C00_0000[0x5758];
cr_ScriptState cr_g_Script;
u8 cr_s_8C00_5BDC[0x1138];
cr_ScenarioState cr_g_Scenario;
u8 cr_s_8C00_7078[0x1F0];
Map8C00 lbl_803BFE68;
extern void cr_Pool_Use(void *);
void cr_Pool803B8C00_Order(void) {
    cr_Pool_Use(cr_s_8C00_0000);
    cr_Pool_Use(&cr_g_Script);
    cr_Pool_Use(cr_s_8C00_5BDC);
    cr_Pool_Use(&cr_g_Scenario);
    cr_Pool_Use(cr_s_8C00_7078);
    cr_Pool_Use(&lbl_803BFE68);
}

void cr_MAP_LINK(void) {
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
    lbl_803BFE68.x = pc[2];
    lbl_803BFE68.y = pc[3];
    if ((lbl_803BFE68.pending != pc[1]) && (lbl_803BFE68.next != pc[1])) {
        if (lbl_803BFE68.next != 0xFFFF) {
            fn_800208F0(lbl_803BFE68.a);
            fn_800215F8(lbl_803BFE68.a);
            fn_800208F0(lbl_803BFE68.b);
            fn_800215F8(lbl_803BFE68.b);
        }
        if (cr_g_Scenario.slot_index == 0) {
            lbl_803BFE68.a = cr_Archive_OpenEntry(4, pc[1]);
        } else {
            lbl_803BFE68.a = cr_Archive_OpenEntry(4, pc[1] + 0x2F);
        }
        if (cr_g_Scenario.slot_index == 0) {
            lbl_803BFE68.b = cr_Archive_OpenEntry(5, pc[1]);
        } else {
            lbl_803BFE68.b = cr_Archive_OpenEntry(6, pc[1]);
        }
        lbl_803BFE68.next = pc[1];
        cr_FileSlot_QueueRead(lbl_803BFE68.a, lbl_8044F2B8);
        cr_FileSlot_QueueRead(lbl_803BFE68.b, lbl_8044F2AC);
        lbl_803BFE68.pending = 0xFFFF;
    }
}
