/* cr_E_CLOCKOUT: Nov 13, 2026 hand draft: as cr_E_CLOCKIN with 0x80 */

// also defines: lbl_803BA114 lbl_803BA1E8 cr_g_Script
/* the script file's .bss from its first object (lbl_803B8C00): scoring only, as tools/file_drafts/battle_9B60/pool.inc */
u8 cr_s_8C00_0000[0x1514];
u32 lbl_803BA114[53];
u32 lbl_803BA1E8[82];
u8 cr_s_8C00_1730[0x4028];
cr_ScriptState cr_g_Script;
extern void cr_Pool_Use(void *);
void cr_Pool803B8C00_Order(void) {
    cr_Pool_Use(cr_s_8C00_0000);
    cr_Pool_Use(lbl_803BA114);
    cr_Pool_Use(lbl_803BA1E8);
    cr_Pool_Use(cr_s_8C00_1730);
    cr_Pool_Use(&cr_g_Script);
}

static inline void view_reset(void) {
    f32 a[4] = { 0.0f, 640.0f, 0.0f, 480.0f };
    f32 b[4] = { 0.0f, 640.0f, 0.0f, 480.0f };

    fn_80029F70(cr_g_CurrentCam, a);
    fn_80029C20(cr_g_CurrentCam, b);
}

static inline void clock_set(s32 arg0, u32 arg1) {

    if (arg0 & 0x80) {
        view_reset();
        *lbl_803BA1E8 = 0;
        *lbl_803BA1E8 |= arg0;
        fn_80023394((s8 *) lbl_803BA114, 0xD4U);
        M2C_FIELD(lbl_803BA114, u32 *, 0) |= 1;
        if (arg1 != 0) {
            M2C_FIELD(lbl_803BA114, f32 *, 0xD0) = lbl_804512A8 / (f32) arg1;
            M2C_FIELD(lbl_803BA114, u32 *, 0) |= 4;
        }
    } else if (arg0 & 0x40) {
        view_reset();
        *lbl_803BA1E8 = 0;
        *lbl_803BA1E8 |= arg0;
        fn_80023394((s8 *) lbl_803BA114, 0xD4U);
        M2C_FIELD(lbl_803BA114, u32 *, 0) |= 2;
        if (arg1 != 0) {
            M2C_FIELD(lbl_803BA114, f32 *, 0xD0) = lbl_804512A8 / (f32) arg1;
            M2C_FIELD(lbl_803BA114, u32 *, 0) |= 4;
        }
    }
}

void cr_E_CLOCKOUT(void) {
    s16 *var_r31;
    u32 *cmd_cnt;
    s32 var_r29;

    var_r31 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) var_r31[0];
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
    }
    var_r29 = var_r31[1];
    clock_set(0x80, var_r29);
    cr_g_ScenarioSlotPtr->status |= lbl_80451100;
}
