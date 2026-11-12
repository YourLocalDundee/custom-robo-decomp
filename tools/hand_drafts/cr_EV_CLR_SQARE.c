/* cr_EV_CLR_SQARE: Nov 5, 2026 hand draft: clears the flags of the area box whose id is the command argument; the head rule plus 1 dead use of the PC copy in its `if (0)` block (the PC copy then outranks the found pointer, as in the original) */
static inline s8 *cr_EV_CLR_SQARE_search0(s32 id) {
    u16 var_r31_12;

    for (var_r31_12 = 0; var_r31_12 < 0x1CU; var_r31_12 += 1) {
        if ((M2C_FIELD(cr_g_AreaBoxes, s32 *, (var_r31_12 << 6) & 0x3FFFC0) & 1) && ((u32) id == (u16) M2C_FIELD((((cr_AreaBox *) ((u8 *) cr_g_AreaBoxes + ((var_r31_12 << 6) & 0x3FFFC0)))), u16 *, 0x28))) {
            return (signed char *) ((cr_AreaBox *) ((u8 *) cr_g_AreaBoxes + ((var_r31_12 << 6) & 0x3FFFC0)));
        }
    }
    return NULL;
}

void cr_EV_CLR_SQARE(void) {
    s16 *temp_r30_15;
    u32 *cmd_cnt;
    s8 *var_r28_59;

    var_r28_59 = NULL;
    temp_r30_15 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r30_15, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
        temp_r30_15 = temp_r30_15;
    }
    var_r28_59 = cr_EV_CLR_SQARE_search0(M2C_FIELD(temp_r30_15, s16 *, 2));
    if (var_r28_59 != NULL) {
        fn_80023394(var_r28_59, 0x40U);
    }
}
