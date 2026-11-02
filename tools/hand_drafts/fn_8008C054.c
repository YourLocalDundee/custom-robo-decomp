/* fn_8008C054: hand-written from main.dol's asm on its m2c draft (plan.md step 108): `if (arg0 == ((void *) 0)) { (void) 0; }` first: an empty debug macro (li 0 / cmplw, no branch); 2.0f is its file's pool constant */
void fn_8008C054(void *arg0) {

    if (arg0 == ((void *) 0)) {
        (void) 0;
    }
    M2C_FIELD(arg0, u8 *, 0x57) = 0xFF;
    M2C_FIELD(arg0, s32 *, 0x10) = 0;
    M2C_FIELD(arg0, s32 *, 0x14) = 0;
    M2C_FIELD(arg0, s32 *, 0x18) = 0;
    M2C_FIELD(arg0, s32 *, 0x1C) = 0;
    M2C_FIELD(arg0, s32 *, 0) |= 0x10;
    M2C_FIELD((M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4)), u16 *, 0x12) |= 0x1000;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) = 2.0f;
    M2C_FIELD(arg0, void (**)(void *), 0x50) = fn_8008BED4;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) M2C_FIELD(lbl_8021A9A4, f32 *, 0);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = lbl_8021A9A4[1];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u16 *, 0xC) = M2C_FIELD(lbl_8021A990, u16 *, 0);
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u16 *, 0xE) = lbl_8021A990[1];
    M2C_FIELD(arg0, u8 *, 0xCB) = 0xFF;
    M2C_FIELD(arg0, s32 *, 0x84) = 0;
    M2C_FIELD(arg0, s32 *, 0x88) = 0;
    M2C_FIELD(arg0, s32 *, 0x8C) = 0;
    M2C_FIELD(arg0, s32 *, 0x90) = 0;
    M2C_FIELD(arg0, s32 *, 0x74) |= 0x10;
    M2C_FIELD(arg0, void (**)(void *), 0xC4) = fn_8008BD50;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0xD0), f32 *, 0) = lbl_8021A9A4[2];
    M2C_FIELD(M2C_FIELD(arg0, void **, 0xD0), f32 *, 4) = lbl_8021A9A4[3];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0xD0), void **, 0x1C), void **, 4), u16 *, 0xC) = lbl_8021A990[2];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0xD0), void **, 0x1C), void **, 4), u16 *, 0xE) = lbl_8021A990[3];
    M2C_FIELD(arg0, u8 *, 0x13F) = 0xFF;
    M2C_FIELD(arg0, s32 *, 0xF8) = 0;
    M2C_FIELD(arg0, s32 *, 0xFC) = 0;
    M2C_FIELD(arg0, s32 *, 0x100) = 0;
    M2C_FIELD(arg0, s32 *, 0x104) = 0;
    M2C_FIELD(arg0, s32 *, 0xE8) |= 0x10;
    M2C_FIELD(arg0, void (**)(void *), 0x138) = fn_8008BBC8;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x144), f32 *, 0) = lbl_8021A9A4[4];
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x144), f32 *, 4) = lbl_8021A9A4[5];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x144), void **, 0x1C), void **, 4), u16 *, 0xC) = lbl_8021A990[4];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x144), void **, 0x1C), void **, 4), u16 *, 0xE) = lbl_8021A990[5];
    M2C_FIELD(arg0, u8 *, 0x1B3) = 0xFF;
    M2C_FIELD(arg0, s32 *, 0x16C) = 0;
    M2C_FIELD(arg0, s32 *, 0x170) = 0;
    M2C_FIELD(arg0, s32 *, 0x174) = 0;
    M2C_FIELD(arg0, s32 *, 0x178) = 0;
    M2C_FIELD(arg0, s32 *, 0x15C) |= 0x10;
    M2C_FIELD(arg0, void (**)(void *), 0x1AC) = fn_8008BA44;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x1B8), f32 *, 0) = lbl_8021A9A4[6];
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x1B8), f32 *, 4) = lbl_8021A9A4[7];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x1B8), void **, 0x1C), void **, 4), u16 *, 0xC) = lbl_8021A990[6];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x1B8), void **, 0x1C), void **, 4), u16 *, 0xE) = lbl_8021A990[7];
    M2C_FIELD(arg0, s8 *, 0x227) = 0;
    M2C_FIELD(arg0, s32 *, 0x1E0) = 0;
    M2C_FIELD(arg0, s32 *, 0x1E4) = 0;
    M2C_FIELD(arg0, s32 *, 0x1E8) = 0;
    M2C_FIELD(arg0, s32 *, 0x1EC) = 0;
    M2C_FIELD(arg0, s32 *, 0x1D0) |= 0x10;
    M2C_FIELD(arg0, void (**)(void *), 0x220) = fn_8008B95C;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x22C), f32 *, 0) = lbl_8021A9A4[8];
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x22C), f32 *, 4) = lbl_8021A9A4[9];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x22C), void **, 0x1C), void **, 4), u16 *, 0xC) = lbl_8021A990[8];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x22C), void **, 0x1C), void **, 4), u16 *, 0xE) = lbl_8021A990[9];
}
