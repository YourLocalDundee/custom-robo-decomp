/* fn_80089550: Nov 6, 2026 hand draft (m2c 88%): the rotation matrix is a whole Mtx (m2c's f32 sp10[4] left the frame 32 bytes short), the colour from the table at lbl_80219448 is a GXColor struct copy (bytes: lbz / stb through a compiler temp, where m2c's pointer temp took a saved register; the four bytes from lbl_8044EAE8-EB stay four copies: they are four symbols), and the random value is shifted straight into var_r29 and reduced in place (m2c's temp for cr_Rand_Int() took a saved register) */


extern u8 *lbl_8044F5EC;

void fn_80089550(u8 *arg0) {
    Mtx sp10;
    s32 spC;
    s32 sp8;
    u8 *var_r31;
    u32 var_r29;
    u32 var_r28;

    fn_80089140(arg0);
    cr_g_TagTeams[(s32) M2C_FIELD(arg0, u8 *, 0x2B7) >> 1].active = 0;
    cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)].update = fn_80005B88;
    M2C_FIELD(((cr_BattleRobo *) (&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x2B7)])), s32 *, 0x1D8) |= 0x10000000;
    if ((s32) cr_g_GameMode.battle_mode == 2) {
        cr_g_BattleRobos[(((s32) M2C_FIELD(arg0, u8 *, 0x2B7) >> 1) * 2) + (cr_g_TagTeams[(s32) M2C_FIELD(arg0, u8 *, 0x2B7) >> 1].active ^ 1)].update = fn_80005B88;
    }
    fn_800772C0((struct HSD_JObj **) (arg0 + 0x1C), 0);
    fn_800772C0((struct HSD_JObj **) (arg0 + 0x20), 1);
    fn_800772C0((struct HSD_JObj **) (arg0 + 0x24), 2);
    var_r31 = lbl_8038684C + (M2C_FIELD(arg0, u8 *, 0x2B7) * 0xEC);
    M2C_FIELD(var_r31, f32 *, 0x6C) = (f32) M2C_FIELD(arg0, f32 *, 0x94);
    M2C_FIELD(var_r31, f32 *, 0x70) = (f32) M2C_FIELD(arg0, f32 *, 0x98);
    M2C_FIELD(var_r31, f32 *, 0x74) = (f32) M2C_FIELD(arg0, f32 *, 0x9C);
    M2C_FIELD(var_r31, f32 *, 0x60) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0x64) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0x68) = (f32) lbl_80450EF8;
    M2C_FIELD(var_r31, f32 *, 0x78) = (f32) lbl_80450EFC;
    M2C_FIELD(var_r31, f32 *, 0x7C) = (f32) lbl_80450EFC;
    M2C_FIELD(var_r31, f32 *, 0x80) = (f32) lbl_80450EFC;
    M2C_FIELD(var_r31, f32 *, 0x94) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0x98) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0x9C) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0xD0) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0xD4) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0x30) = (f32) lbl_80450EFC;
    M2C_FIELD(var_r31, f32 *, 0x40) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0x50) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0x34) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0x44) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0x54) = (f32) lbl_80450F00;
    M2C_FIELD(var_r31, f32 *, 0x38) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0x48) = (f32) lbl_80450EFC;
    M2C_FIELD(var_r31, f32 *, 0x58) = (f32) lbl_80450EBC;
    PSMTXIdentity(sp10);
    PSMTXRotRad(sp10, 0x7A, lbl_80450F04);
    PSMTXConcat((float (*)[4]) (var_r31 + 0x30), sp10, (float (*)[4]) (var_r31 + 0x30));
    M2C_FIELD(var_r31, f32 *, 0xA0) = (f32) lbl_80450EFC;
    M2C_FIELD(var_r31, f32 *, 0xB0) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0xC0) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0xA4) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0xB4) = (f32) lbl_80450EBC;
    var_r28 = 0U;
    M2C_FIELD(var_r31, f32 *, 0xC4) = (f32) lbl_80450F00;
    M2C_FIELD(var_r31, f32 *, 0xA8) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, f32 *, 0xB8) = (f32) lbl_80450EFC;
    M2C_FIELD(var_r31, f32 *, 0xC8) = (f32) lbl_80450EBC;
    M2C_FIELD(var_r31, s8 *, 0xD8) = 1;
    M2C_FIELD(var_r31, s8 *, 0xD9) = 1;
    M2C_FIELD(var_r31, f32 *, 0xDC) = (f32) lbl_80450EBC;
    *(GXColor *) (var_r31 + 0xE1) = ((GXColor *) lbl_80219448)[M2C_FIELD(var_r31, u8 *, 0xE0)];
    M2C_FIELD(var_r31, u8 *, 0xE5) = lbl_8044EAE8;
    M2C_FIELD(var_r31, u8 *, 0xE6) = lbl_8044EAE9;
    M2C_FIELD(var_r31, u8 *, 0xE7) = lbl_8044EAEA;
    M2C_FIELD(var_r31, u8 *, 0xE8) = lbl_8044EAEB;
    M2C_FIELD(arg0, s32 *, 0x160) = 0;
    M2C_FIELD(arg0, s32 *, 0x14C) = 0;
    M2C_FIELD(arg0, s32 *, 0x150) = 1;
    M2C_FIELD(arg0, s32 *, 0x154) = 1;
    M2C_FIELD(arg0, s32 *, 0x144) = 0x14;
    M2C_FIELD(arg0, s32 *, 0x148) = 1;
    while (var_r28 < (u32) (M2C_FIELD(arg0, s32 *, 0x144) - 1)) {
        var_r28 += 1;
        M2C_FIELD(arg0, s32 *, 0x148) *= 2;
        M2C_FIELD(arg0, s32 *, 0x148) |= 1;
    }
    sp8 = 0;
    spC = 1;
    var_r29 = cr_Rand_Int() >> M2C_FIELD(arg0, u8 *, 0x2B7);
    var_r29 = var_r29 % 690;
    var_r29 += 0x1E;
    M2C_FIELD(arg0, s16 *, 0x184) = (s16) var_r29;
    M2C_FIELD(arg0, s32 *, 0x158) = (s32) (var_r29 / 120);
    M2C_FIELD(arg0, u16 *, 0x186) = (u16) ((var_r29 - (M2C_FIELD(arg0, s32 *, 0x158) * 0x78)) / 6);
    M2C_FIELD(arg0, s32 *, 0x168) = 0xA;
    M2C_FIELD(arg0, s32 *, 0x164) = 0;
    M2C_FIELD(arg0, s32 *, 0x16C) = 0;
    M2C_FIELD(arg0, s32 *, 0x178) = 0;
    M2C_FIELD(arg0, s32 *, 0x170) = 0;
    M2C_FIELD(arg0, s32 *, 0x174) = 0;
    M2C_FIELD(arg0, s32 *, 0x17C) = 0;
    fn_80023394((signed char *) (arg0 + 0x2E8), 0x30U);
    M2C_FIELD(arg0, s32 *, 0x308) |= 1;
    M2C_FIELD(arg0, s32 *, 0x30C) = (s32) M2C_FIELD(lbl_8044F5EC, u8 *, 0x338);
}
