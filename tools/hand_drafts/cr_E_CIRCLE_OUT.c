/* cr_E_CIRCLE_OUT: Nov 14, 2026 hand draft: the circle wipe out: the circle record lbl_803BA330 typed (u16 flags / time / total, f32 x, y, r, step, target, start); from a set circle the radius steps from start to target over pc[1] frames, else to 0; the statement order found by moving each store and permuting the else branch (51%) */

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

void cr_E_CIRCLE_OUT(void) {
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
        ((CircleA330 *) lbl_803BA330)->r = ((CircleA330 *) lbl_803BA330)->start;
        ((CircleA330 *) lbl_803BA330)->step = (((CircleA330 *) lbl_803BA330)->target - ((CircleA330 *) lbl_803BA330)->start) / pc[1];
        ((CircleA330 *) lbl_803BA330)->total = pc[1];
        ((CircleA330 *) lbl_803BA330)->time = 0;
        ((CircleA330 *) lbl_803BA330)->flags |= 2;
        ((CircleA330 *) lbl_803BA330)->flags |= 0x8000;
        ((CircleA330 *) lbl_803BA330)->flags &= ~0x4000;
    } else {
        ((CircleA330 *) lbl_803BA330)->step = -((CircleA330 *) lbl_803BA330)->r / pc[1];
        ((CircleA330 *) lbl_803BA330)->target = lbl_80451280;
        ((CircleA330 *) lbl_803BA330)->total = pc[1];
        ((CircleA330 *) lbl_803BA330)->time = 0;
        ((CircleA330 *) lbl_803BA330)->flags |= 2;
        ((CircleA330 *) lbl_803BA330)->flags |= 0x8000;
    }
}
