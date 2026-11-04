/* fn_80015E2C: Nov 3, 2026 hand draft (m2c 84%): the string table base kept in a local (r31) for the six text calls, and the confirm / cancel test as an if chain (m2c read it as a switch with a case inside an if) */

s32 fn_80015E2C(void) {
    s8 sp8;
    char *str;

    str = (char *) lbl_801B6620;
    sp8 = 0;
    if ((s8) lbl_8044F365 >= 4) {
        lbl_8044F365 = 0;
    } else if ((s8) lbl_8044F365 < 0) {
        lbl_8044F365 = 3;
    }
    if (cr_g_Pads->unk_28 & 0x01000000) {
        lbl_8044F365 -= 1;
        fn_80098290((s32) lbl_801E1578[3]);
    } else if (cr_g_Pads->unk_28 & 0x04000000) {
        lbl_8044F365 += 1;
        fn_80098290((s32) lbl_801E1578[2]);
    }
    if ((s8) lbl_8044F365 >= 4) {
        lbl_8044F365 = 0;
    } else if ((s8) lbl_8044F365 < 0) {
        lbl_8044F365 = 3;
    }
    fn_8006B144(lbl_80450348, lbl_804502E8, str + 0x343);
    fn_8006B144(lbl_8045034C, lbl_80450348, str + 0x35C);
    fn_8006B144(lbl_8045034C, lbl_804503A0, str + 0x36B);
    fn_8006B144(lbl_8045034C, lbl_80450340, str + 0x376);
    fn_8006B144(lbl_8045034C, lbl_80450344, str + 0x385);
    fn_8006B144(lbl_804503A4, (f32) (((s8) lbl_8044F365 * 0x1E) + 0x96), (const char *) ((s8 *) lbl_804503A8));
    if ((u32) cr_g_Pads->unk_14 == 0x10 || (u32) cr_g_Pads->unk_14 == 0x100) {
        fn_80098290((s32) M2C_FIELD(lbl_801E1578, u32 *, 0));
        if ((s8) lbl_8044F365 == 0) {
            return 2;
        }
        if ((s32) lbl_8044F365 == 1) {
            return 4;
        }
        if ((s32) lbl_8044F365 == 2) {
            return 6;
        }
        if ((s32) lbl_8044F365 == 3) {
            return 8;
        }
    }
    if ((u32) cr_g_Pads->unk_14 == 0x200) {
        fn_80098290((s32) lbl_801E1578[1]);
        return 0xF0;
    }
    return 1;
}
