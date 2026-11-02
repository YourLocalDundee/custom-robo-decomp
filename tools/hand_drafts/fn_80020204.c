/* fn_80020204: hand-written from main.dol's asm on its m2c draft (step 109b): the third counter array as p->unk_70[i] (m2c's byte address in a temporary took a saved register) */
s32 fn_80020204(void) {
    cr_Pad *var_r31_6;
    s32 var_r30_29;
    s32 var_r29_9;

    var_r31_6 = cr_g_Pads;
    var_r29_9 = 0;
    *lbl_8044F3F0 = 0;
    while (var_r29_9 < 4) {
        var_r31_6->prev_held = 0;
        var_r31_6->held = 0;
        var_r31_6->pressed = 0;
        var_r31_6->released = 0;
        var_r30_29 = 0;
        var_r31_6->not_held = -1;
        var_r31_6->prev_dir = 0;
        var_r31_6->dir = 0;
        var_r31_6->unk_14 = 0;
        var_r31_6->unk_18 = 0;
        var_r31_6->unk_20 = -1;
        var_r31_6->unk_28 = 0;
        var_r31_6->unk_2c = 0;
        while (var_r30_29 < 0x20) {
            var_r31_6->unk_30[var_r30_29] = 0x14;
            var_r31_6->unk_50[var_r30_29] = 0x14;
            var_r31_6->unk_70[var_r30_29] = 0;
            var_r30_29 += 1;
        }
        var_r29_9 += 1;
        var_r31_6 = (cr_Pad *) ((u8 *) var_r31_6 + 0xA4);
    }
    return 0;
}
