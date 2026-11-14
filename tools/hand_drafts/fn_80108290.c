/* fn_80108290: Nov 13, 2026 hand draft: the name buffer is char[84] (frame 128); one u8 counter for both loops (m2c had two for r30); fn_8006A344 returns a value that is dropped (r3 stays busy: the next statement loads through r4), the context says void */

void fn_80108290(u8 *arg0) {
    char sp8[84];
    u8 var_r30_69;
    u8 *var_r29_102;
    s32 var_r27_160;
    u8 *base_lbl_80238008;
    base_lbl_80238008 = lbl_80238008;

    (void) var_r27_160; 
    (void) base_lbl_80238008; 
    fn_80023394((signed char *) (arg0 + 0x22C), 0x40U);
    fn_80023394((signed char *) (arg0 + 0x21C), 0x10U);
    fn_80023394((signed char *) (arg0 + 0x1FC), 0x20U);
    fn_8008C5F0(arg0 + 0x1FC, arg0 + 0x21C, (s32) (arg0 + 0x22C), 2U);
    M2C_FIELD(arg0, s32 *, 0x26C) = fn_8008EDD8(8);
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), void **, 0x5C) = (void *) (arg0 + 0x1FC);
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), s32 *, 0) |= 0x40;
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), s32 *, 0) |= 0x20;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), void **, 0x5C), f32 *, 8) = (f32) lbl_804517A4;
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), s32 *, 0x10) = (s32) M2C_FIELD(arg0, u8 *, 0x4854);
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), void (**)(void *), 0x50) = fn_800FB924;
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), s8 *, 0x58) = 0;
    M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), s8 *, 0x57) = 0;
    if (*lbl_8040709C != 0) {
        M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x26C), s32 *, 0) |= 0x10000;
    }
    var_r30_69 = 0;
    M2C_FIELD(arg0, s16 *, 0x23C) = (s16) (M2C_FIELD(arg0, u8 *, 0x4854) + 0x20);
    M2C_FIELD(arg0, s16 *, 0x238) = 0x12D;
    M2C_FIELD(arg0, s16 *, 0x23A) = 0xC7;
    while (var_r30_69 < 0xBU) {
        fn_80023394((signed char *) (&((u8 (*)[0x634]) (arg0))[var_r30_69][0x278]), 0x600U);
        fn_80023394((signed char *) (&((u8 (*)[0x634]) (arg0))[var_r30_69][0x878]), 0x10U);
        fn_80023394((signed char *) (&((u8 (*)[0x634]) (arg0))[var_r30_69][0x888]), 0x20U);
        var_r30_69 += 1;
    }
    M2C_FIELD(arg0, s8 *, 0x274) = 1;
    var_r29_102 = NULL;
    if (cr_g_GameMode.flags & 0x800000) {
        if (M2C_FIELD(lbl_80407250, u16 *, 2) & 1) {
            if ((s32) cr_g_GameMode.battle_mode == 2) {
                var_r29_102 = base_lbl_80238008 + 0x398C;
            } else {
                var_r29_102 = base_lbl_80238008 + 0x3968;
            }
        }
    } else if (cr_g_GameMode.flags & 0x08000000) {
        if (M2C_FIELD(lbl_80407250, u16 *, 2) & 1) {
            if ((s32) cr_g_GameMode.battle_mode == 2) {
                var_r29_102 = base_lbl_80238008 + 0x39D0;
            } else {
                var_r29_102 = base_lbl_80238008 + 0x39AC;
            }
        }
    } else if (M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x4854)], s32 *, 0x1D8) & 0x80000000) {
        var_r29_102 = base_lbl_80238008 + 0x3A18;
    } else {
        var_r29_102 = base_lbl_80238008 + 0x39F0;
    }
    if (var_r29_102 != ((void *) 0)) {
        var_r27_160 = 0;
        var_r30_69 = 0;
        while ((u32) M2C_FIELD(var_r29_102, u32 *, (var_r30_69 * 4) & 0x3FC) != 0xBU) {
            fn_8008C5F0(&((u8 (*)[0x634]) (arg0))[var_r30_69][0x888], &((u8 (*)[0x634]) (arg0))[var_r30_69][0x878], (s32) (&((u8 (*)[0x634]) (arg0))[var_r30_69][0x278]), 0x18U);
            M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), s32 *, 0) = fn_8008EDD8(8);
            M2C_FIELD(M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0), void **, 0x5C) = (void *) (&((u8 (*)[0x634]) (arg0))[var_r30_69][0x888]);
            M2C_FIELD(M2C_FIELD(M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0), void **, 0x5C), f32 *, 8) = (f32) lbl_804517A4;
            M2C_FIELD(M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0), s32 *, 0x10) = (s32) M2C_FIELD(arg0, u8 *, 0x4854);
            M2C_FIELD(M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0), s32 *, 0x14) = (s32) var_r30_69;
            M2C_FIELD(M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0), s32 *, 0x18) = var_r27_160;
            if ((u32) M2C_FIELD(var_r29_102, u32 *, (var_r30_69 * 4) & 0x3FC) == 6U) {
                var_r27_160 += 0xE;
            } else {
                var_r27_160 += 0x12;
            }
            M2C_FIELD(M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0), void (**)(void *), 0x50) = fn_800FBAB4;
            fn_800671C4((s32) sp8, fn_80066948(0x15U, (u16) M2C_FIELD(var_r29_102, s32 *, (var_r30_69 * 4) & 0x3FC)));
            fn_80068988((s32) sp8, 0x30U);
            ((s32 (*)(void *, s32)) fn_8006A344)((void *) (M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), u32 *, 0)), (s32) sp8);
            if (*lbl_8040709C != 0) {
                *M2C_FIELD((&((u8 (*)[0x634]) (arg0))[var_r30_69][0x8A8]), s32 **, 0) |= 0x10000;
            }
            var_r30_69 += 1;
        }
        return;
    }
    M2C_FIELD(arg0, s8 *, 0x274) = 0;
}
