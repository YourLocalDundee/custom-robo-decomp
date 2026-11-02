/* fn_80112294: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the remainder written out as `x - (s8) ((s8) x / 6) * 6` (main.dol loads the byte twice: extsb, mulhw, the s8 quotient, mulli, subf from the second load), where `x % 6` loads it once */
u8 fn_80112294(u8 arg0) {
    u8 var_r31_12;
    u32 var_r30_14;
    s8 var_r29_27;

    var_r31_12 = 0;
    var_r30_14 = -1;
    var_r29_27 = cr_g_StageSelector.stage_count - (s8) ((s8) cr_g_StageSelector.stage_count / 6) * 6;
    if (var_r29_27 == 0) {
        var_r29_27 = 6;
    }
    if (cr_g_Pads[arg0].unk_28 & 0x01000000) {
        var_r30_14 = lbl_8023CE54[2];
        var_r31_12 = 1;
        if (cr_g_Pads[arg0].dir & 0x100000) {
            cr_g_StageSelector.top_row -= 2;
        } else {
            cr_g_StageSelector.row -= 1;
            if ((s8) cr_g_StageSelector.row < 0) {
                cr_g_StageSelector.row = 0;
                cr_g_StageSelector.top_row -= 1;
            }
        }
        if ((s8) cr_g_StageSelector.top_row < 0) {
            var_r30_14 = -1;
            cr_g_StageSelector.top_row = 0;
            var_r31_12 = 0;
        }
    } else if (cr_g_Pads[arg0].unk_28 & 0x04000000) {
        var_r30_14 = lbl_8023CE54[3];
        var_r31_12 = 1;
        if (cr_g_Pads[arg0].dir & 0x100000) {
            cr_g_StageSelector.top_row += 2;
        } else {
            cr_g_StageSelector.row += 1;
            if ((s8) cr_g_StageSelector.row > 1) {
                cr_g_StageSelector.row = 1;
                cr_g_StageSelector.top_row += 1;
            }
        }
        if ((s8) cr_g_StageSelector.top_row >= 5) {
            var_r30_14 = -1;
            cr_g_StageSelector.top_row = 4;
            var_r31_12 = 0;
        }
    } else if (cr_g_Pads[arg0].unk_28 & 0x08000000) {
        var_r30_14 = lbl_8023CE54[5];
        var_r31_12 = 1;
        cr_g_StageSelector.col -= 1;
        if ((s8) cr_g_StageSelector.col < 0) {
            if (((s32) (u8) cr_g_StageSelector.row == 1) && ((s32) (u8) cr_g_StageSelector.top_row == 4)) {
                cr_g_StageSelector.col = var_r29_27 - 1;
            } else {
                cr_g_StageSelector.col = 5;
            }
        }
    } else if (cr_g_Pads[arg0].unk_28 & 0x02000000) {
        var_r30_14 = lbl_8023CE54[4];
        var_r31_12 = 1;
        cr_g_StageSelector.col += 1;
        if (((s32) (u8) cr_g_StageSelector.row != 1) || ((s32) (u8) cr_g_StageSelector.top_row != 4)) {
            var_r29_27 = 6;
        }
        if ((s8) cr_g_StageSelector.col > (s32) (var_r29_27 - 1)) {
            var_r31_12 = 1;
            cr_g_StageSelector.col = 0;
        }
    }
    if ((s32) var_r30_14 >= 0) {
        fn_80098290((s32) var_r30_14);
    }
    if (var_r31_12 != 0) {
        cr_g_StageSelector.unk_0bf = 0;
    }
    return var_r31_12;
}
