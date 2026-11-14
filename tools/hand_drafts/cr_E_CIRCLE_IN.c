/* cr_E_CIRCLE_IN: Nov 14, 2026 hand draft: the circle wipe in: cr_E_CIRCLE_OUT's form (the same store order in both branches: step, target, total, time, the flags); from a set circle the radius grows from 0 to its target, else from the current radius to the set start (44%) */

typedef struct {
    u16 flags;
    u16 time;
    u16 total;
    u16 pad6;
    f32 x;
    f32 y;
    f32 r;
    f32 step;
    f32 target;
    f32 start;
} CircleA330;

void cr_E_CIRCLE_IN(void) {
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
    if (((CircleA330 *) lbl_803BA330)->flags & 0x4000) {
        ((CircleA330 *) lbl_803BA330)->r = lbl_80451280;
        ((CircleA330 *) lbl_803BA330)->step = (((CircleA330 *) lbl_803BA330)->target - ((CircleA330 *) lbl_803BA330)->r) / pc[1];
        ((CircleA330 *) lbl_803BA330)->total = pc[1];
        ((CircleA330 *) lbl_803BA330)->time = 0;
        ((CircleA330 *) lbl_803BA330)->flags |= 1;
        ((CircleA330 *) lbl_803BA330)->flags |= 0x8000;
        ((CircleA330 *) lbl_803BA330)->flags &= ~0x4000;
    } else {
        ((CircleA330 *) lbl_803BA330)->step = (((CircleA330 *) lbl_803BA330)->start - ((CircleA330 *) lbl_803BA330)->r) / pc[1];
        ((CircleA330 *) lbl_803BA330)->target = ((CircleA330 *) lbl_803BA330)->start;
        ((CircleA330 *) lbl_803BA330)->total = pc[1];
        ((CircleA330 *) lbl_803BA330)->time = 0;
        ((CircleA330 *) lbl_803BA330)->flags |= 1;
        ((CircleA330 *) lbl_803BA330)->flags |= 0x8000;
    }
}
