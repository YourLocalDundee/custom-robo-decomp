/* fn_800BC4C0: hand-written from main.dol's asm on its m2c draft (step 109b): the rank count as a static inline helper (its locals take the saved registers r31 / r29 in the original: the inlined body's own variables), the threshold compared as u32 */
extern u8 *lbl_8044F728;

static inline u8 rank_of_slot(void) {
    u32 v;

    v = cr_g_ScenarioSlotPtr->unk_84[3];
    lbl_8044F6F4 = 0;
    for (; lbl_8044F6F4 < 3U; lbl_8044F6F4 += 1) {
        if (!(v >= lbl_8044EB38[lbl_8044F6F4])) {
            break;
        }
    }
    if (lbl_8044F6F4 != 0) {
        fn_800AFE3C(2);
    }
    return lbl_8044F6F4;
}

void fn_800BC4C0(void) {
    u8 var_r30;

    switch (M2C_FIELD(lbl_8044F728, u8 *, 0x4A1)) {
    case 0:
        lbl_8044F756 = 1;
        M2C_FIELD(lbl_8044F728, u16 *, 0) |= 8;
        lbl_8044F72C = 0;
        cr_g_SeqCurrent.next = SEQ_STAGE_SELECTOR;
        fn_80111E54(1);
        fn_800F6F6C(1);
        return;
    case 1:
        var_r30 = rank_of_slot();
        if (var_r30 == 0) {
            M2C_FIELD(lbl_8044F728, u8 *, 0x4A1) = 0U;
        }
        return;
    }
}
