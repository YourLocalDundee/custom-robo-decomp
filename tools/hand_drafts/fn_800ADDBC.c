/* fn_800ADDBC: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): main.dol saves r28-r31 for four locals: the index (r31), the count read into r29 and copied to r30, which the test reads (lbz r29 / clrlwi. r0, r29: the copy is coalesced), and the level an u32 in r28 (lhz / cmplw); the u16 test without its cast (cmplwi) */
extern u8 *lbl_8044F728;

void fn_800ADDBC(void) {
    u8 var_r31;
    u8 var_r30;
    u8 var_r29;
    u32 var_r28;

    if (cr_g_Pads->unk_14 & 0x100) {
        lbl_8044F74C = 1;
        var_r31 = 0;
        lbl_8044F750 = lbl_8045147C;
        while (var_r31 < (u32) lbl_8044F504) {
            if (M2C_FIELD(cr_g_BattleLoadouts, u16 *, (var_r31 * 0x10) & 0xFF0) == 0) {
                M2C_FIELD((((cr_BattleLoadout *) ((u8 *) cr_g_BattleLoadouts + ((var_r31 * 0x10) & 0xFF0)))), s16 *, 0xC) = 0x3E8;
            } else {
                M2C_FIELD((((cr_BattleLoadout *) ((u8 *) cr_g_BattleLoadouts + ((var_r31 * 0x10) & 0xFF0)))), s16 *, 0xC) = (s16) (lbl_804513EC * lbl_8044F750);
            }
            var_r31 += 1;
        }
        M2C_FIELD(lbl_8044F728, u8 *, 0x4A1) = 0U;
        switch ((s32) (u8) (M2C_FIELD(lbl_8044F728, u8 *, 0x4A1))) {                 /* irregular */
        case 0:
            lbl_8044F756 = 1;
            M2C_FIELD(lbl_8044F728, u16 *, 0) |= 8;
            lbl_8044F72C = 0;
            cr_g_SeqCurrent.next = SEQ_STAGE_SELECTOR;
            fn_80111E54(1);
            fn_800F6F6C(1);
            return;
        case 1:
            var_r28 = cr_g_ScenarioSlotPtr->unk_84[3];
            lbl_8044F6F4 = 0;
            for (; lbl_8044F6F4 < 3U; lbl_8044F6F4 += 1) {
                if (!(var_r28 >= (u8) lbl_8044EB38[lbl_8044F6F4])) {
                    break;
                }
            }
            if (lbl_8044F6F4 != 0) {
                fn_800AFE3C(2);
            }
            var_r29 = lbl_8044F6F4;
            var_r30 = var_r29;
            if (var_r30 == 0) {
                M2C_FIELD(lbl_8044F728, u8 *, 0x4A1) = 0U;
            }
            break;
        }
    }
}
