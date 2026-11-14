/* cr_CM_PLAYER_RESET: Nov 13, 2026 hand draft: script command on the pool, straight from tools/script_pool.py (the m2c draft on defined objects with the prologue in its source form) */

// also defines: lbl_803BE308 lbl_803BE330 cr_g_Script

struct cr_PF_4_f32 {
    u8 pad[0x4];
    f32 v;
};
struct cr_PF_8_f32 {
    u8 pad[0x8];
    f32 v;
};
struct cr_PF_C_f32 {
    u8 pad[0xC];
    f32 v;
};
struct cr_PF_10_f32 {
    u8 pad[0x10];
    f32 v;
};
struct cr_PF_14_f32 {
    u8 pad[0x14];
    f32 v;
};
struct cr_PF_18_f32 {
    u8 pad[0x18];
    f32 v;
};
struct cr_PF_1C_f32 {
    u8 pad[0x1C];
    f32 v;
};
struct cr_PF_20_f32 {
    u8 pad[0x20];
    f32 v;
};
struct cr_PF_24_f32 {
    u8 pad[0x24];
    f32 v;
};

/* the file's .bss from its first object (lbl_803B8C00): scoring only (tools/pool_prelude.py) */
u8 cr_s_8C00_0000[0x5708];
f32 lbl_803BE308[10];
u8 lbl_803BE330[40];
cr_ScriptState cr_g_Script;
extern void cr_Pool_Use(void *);
void cr_Pool803B8C00_Order(void) {
    cr_Pool_Use(cr_s_8C00_0000);
    cr_Pool_Use(lbl_803BE308);
    cr_Pool_Use(lbl_803BE330);
    cr_Pool_Use(&cr_g_Script);
}

void cr_CM_PLAYER_RESET(void) {
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 temp_f31_111;
    f32 temp_f30_104;
    f32 temp_f29_97;
    f32 temp_f28_95;
    u8 *temp_r30_37;
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
    temp_r30_37 = cr_g_CurrentCam;
    M2C_FIELD(temp_r30_37, s8 *, 0x16A) = 6;
    M2C_FIELD(temp_r30_37, u8 *, 0x168) |= 4;
    M2C_FIELD(temp_r30_37, s32 *, 0xFC) = (s32) pc[1];
    cr_g_ScenarioSlotPtr->status |= lbl_8045110C;
    cr_Cam_SetMode(0xBU);
    (*(f32 *) &lbl_803BE308) = (f32) cr_g_FirstChara->pos.x;
    ((struct cr_PF_4_f32 *) &lbl_803BE308)->v = (f32) cr_g_FirstChara->pos.y;
    ((struct cr_PF_4_f32 *) &lbl_803BE308)->v = (f32) lbl_80451280;
    ((struct cr_PF_8_f32 *) &lbl_803BE308)->v = (f32) cr_g_FirstChara->pos.z;
    temp_f28_95 = ((*(f32 *) &lbl_803BE308) + ((struct cr_PF_C_f32 *) &lbl_803BE308)->v) - lbl_80451284;
    temp_f29_97 = lbl_80451284 + ((*(f32 *) &lbl_803BE308) + ((struct cr_PF_10_f32 *) &lbl_803BE308)->v);
    temp_f30_104 = (((struct cr_PF_8_f32 *) &lbl_803BE308)->v + ((struct cr_PF_14_f32 *) &lbl_803BE308)->v) - lbl_80451284;
    temp_f31_111 = lbl_80451284 + (((struct cr_PF_8_f32 *) &lbl_803BE308)->v + ((struct cr_PF_18_f32 *) &lbl_803BE308)->v);
    sp8 = ((struct cr_PF_C_f32 *) &lbl_803BE330)->v - temp_f28_95;
    spC = ((struct cr_PF_10_f32 *) &lbl_803BE330)->v - temp_f29_97;
    sp10 = ((struct cr_PF_14_f32 *) &lbl_803BE330)->v - temp_f30_104;
    sp14 = ((struct cr_PF_18_f32 *) &lbl_803BE330)->v - temp_f31_111;
    if (temp_f28_95 < ((struct cr_PF_C_f32 *) &lbl_803BE330)->v) {
        (*(f32 *) &lbl_803BE308) = (f32) (lbl_80451284 + (((struct cr_PF_C_f32 *) &lbl_803BE330)->v - ((struct cr_PF_C_f32 *) &lbl_803BE308)->v));
    }
    if (((struct cr_PF_10_f32 *) &lbl_803BE330)->v < temp_f29_97) {
        (*(f32 *) &lbl_803BE308) = (f32) ((((struct cr_PF_10_f32 *) &lbl_803BE330)->v - ((struct cr_PF_10_f32 *) &lbl_803BE308)->v) - lbl_80451284);
    }
    if (temp_f30_104 < ((struct cr_PF_14_f32 *) &lbl_803BE330)->v) {
        ((struct cr_PF_8_f32 *) &lbl_803BE308)->v = (f32) (lbl_80451284 + (((struct cr_PF_14_f32 *) &lbl_803BE330)->v - ((struct cr_PF_14_f32 *) &lbl_803BE308)->v));
    }
    if (((struct cr_PF_18_f32 *) &lbl_803BE330)->v < temp_f31_111) {
        ((struct cr_PF_8_f32 *) &lbl_803BE308)->v = (f32) ((((struct cr_PF_18_f32 *) &lbl_803BE330)->v - ((struct cr_PF_18_f32 *) &lbl_803BE308)->v) - lbl_80451284);
    }
    ((struct cr_PF_1C_f32 *) &lbl_803BE308)->v = (f32) (*(f32 *) &lbl_803BE308);
    ((struct cr_PF_20_f32 *) &lbl_803BE308)->v = (f32) ((struct cr_PF_4_f32 *) &lbl_803BE308)->v;
    ((struct cr_PF_24_f32 *) &lbl_803BE308)->v = (f32) ((struct cr_PF_8_f32 *) &lbl_803BE308)->v;
    fn_80026C50(temp_r30_37);
}
