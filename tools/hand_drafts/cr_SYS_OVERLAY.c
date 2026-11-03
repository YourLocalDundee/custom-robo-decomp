/* cr_SYS_OVERLAY: step 110 hand draft (the head rule plus 2 dead use(s) of temp_r31_13 in its `if (0)` block: dead uses count for the register order, so temp_r31_13 gets the register it has in the original) */
extern u8 *lbl_8044F320;
extern u32 *lbl_8044F338;

void cr_SYS_OVERLAY(void) {
    s16 *temp_r31_13;
    u32 *cmd_cnt;
    u16 *var_r30_56;
    u32 *temp_r29_51;
    u32 var_r28_42;
    u32 var_r27_50;

    temp_r31_13 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r31_13, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
        temp_r31_13 = temp_r31_13;
        temp_r31_13 = temp_r31_13;
    }
    if ((s32) cr_g_Script.unk_454 != -1) {
        var_r28_42 = 0;
        while (var_r28_42 < (u32) *lbl_8044F338) {
            var_r27_50 = 0U;
            temp_r29_51 = ((u32 *) ((u8 *) lbl_8044F338 + M2C_FIELD(&lbl_8044F338[var_r28_42], s32 *, 4)));
            var_r30_56 = (unsigned short *) (temp_r29_51 + *temp_r29_51 * 3 + 1);
            while (var_r27_50 < (u32) *temp_r29_51) {
                if ((u16) *var_r30_56 != 0xFFFF) {
                    cr_g_MotionDataBit15[*var_r30_56 & 0x7FFF] = 0;
                    *(((u32 * *) ((u8 *) cr_g_MotionData + ((*var_r30_56 * 4) & 0x1FFFC)))) = 0;
                }
                var_r27_50 += 1;
                var_r30_56 += 1;
            }
            var_r28_42 += 1;
        }
    }
    cr_g_Script.unk_454 = -1;
    cr_g_Script.unk_454 = M2C_FIELD(temp_r31_13, s16 *, 2) - 0x14;
    cr_g_Script.overlay_478 = (unsigned long) (cr_Archive_OpenEntry(2, (M2C_FIELD(temp_r31_13, s16 *, 2) * 2) & 0xFFFE));
    cr_g_Script.overlay_47c = (unsigned long) (cr_Archive_OpenEntry(2, (u16) ((M2C_FIELD(temp_r31_13, s16 *, 2) * 2) + 1)));
    cr_g_Script.overlay_480 = (unsigned long) (cr_Archive_OpenEntry(7, (u16) M2C_FIELD(temp_r31_13, s16 *, 2)));
    cr_FileSlot_QueueRead((cr_FileSlot *) cr_g_Script.overlay_478, lbl_8044F320);
    cr_FileSlot_QueueRead((cr_FileSlot *) cr_g_Script.overlay_47c, lbl_8044F318);
    cr_FileSlot_QueueRead((cr_FileSlot *) cr_g_Script.overlay_480, lbl_8044F338);
}
