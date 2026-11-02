/* fn_800AC158: hand-written from main.dol's asm on its m2c draft (step 109b): the speed-table search as a static inline helper with an early return (its i / result in r31 / r29) and an empty debug check of i after the loop (extsh; cmplwi 5, no branch) */
extern u8 *lbl_8044F728;

static inline s16 speed_index(f32 v) {
    s16 i;

    for (i = 0; (u32) i < 5U; i++) {
        if (v == ((f32 *) lbl_8022A088)[i]) {
            return i;
        }
    }
    if ((u32) i >= 5U) {
        (void) 0;
    }
    return 0;
}

void fn_800AC158(void) {
    s16 var_r28;
    u8 var_r30;

    if ((M2C_FIELD(lbl_8044F728, u8 *, 3) != 8) && (cr_g_Pads->unk_14 & 0x100)) {
        fn_800AFE3C(8);
        var_r28 = speed_index(fn_800683A4());
        var_r30 = 0;
        M2C_FIELD(lbl_8044F728, s16 *, 0x38) = var_r28;
        while (var_r30 < 5U) {
            *(u32 *) &((u8 (*)[0x10]) (lbl_8044F728))[var_r30][0xA4] = (unsigned long) fn_800AC10C;
            var_r30 += 1;
        }
        M2C_FIELD(lbl_8044F728, s8 *, 0x5C) = 1;
    }
}
