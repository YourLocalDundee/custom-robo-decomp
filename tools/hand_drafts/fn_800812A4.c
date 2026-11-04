/* fn_800812A4: Nov 3, 2026 hand draft (m2c 58%): the seven-object setup loop with only the index and the object as locals (m2c temporaries took three more saved registers), the index compared unsigned */

void fn_800812A4(void) {
    s16 i;
    void *o;

    lbl_8044F638 = 0;
    lbl_8044F63A = 0;
    lbl_8044F63C = 0;
    fn_8008F6E8(0xCU);
    for (i = 0; (u32) i < 7U; i++) {
        o = (void *) fn_8008EDD8(0xC);
        M2C_FIELD(o, void **, 0x5C) = lbl_802191D8 + (i << 5);
        M2C_FIELD(M2C_FIELD(o, void **, 0x5C), f32 *, 8) = lbl_80450E40;
        M2C_FIELD(o, s32 *, 8) = **(u16 **) (lbl_802191D8 + (i << 5) + 0x1C);
        M2C_FIELD(o, s32 *, 0x10) = i;
        M2C_FIELD(o, s32 *, 0x14) = 0;
        M2C_FIELD(o, s32 *, 0) |= 0x10;
        M2C_FIELD(o, s32 *, 0) |= 0x8000;
        M2C_FIELD(o, s32 *, 0) |= 0x40;
        M2C_FIELD(o, s32 *, 0x50) = ((s32 *) lbl_802193A8)[i];
        M2C_FIELD(o, s8 *, 0x58) = 0;
        M2C_FIELD(o, s8 *, 0x57) = 0;
        M2C_FIELD(M2C_FIELD(o, void **, 0x5C), f32 *, 0) = lbl_80450E7C;
        M2C_FIELD(M2C_FIELD(o, void **, 0x5C), f32 *, 4) = lbl_80450E48;
    }
    M2C_FIELD(lbl_8044EAE0, u8 *, 0) = 0;
    lbl_8044EAE0[1] = 0;
    lbl_8044EAE0[2] = 0;
    *lbl_8044F640 = 0;
    if (!(cr_g_GameMode.flags & 0x08000000) && !(cr_g_GameMode.flags & 0x800000) && (lbl_8041E938[3] == 0)) {
        fn_8008057C();
    }
}
