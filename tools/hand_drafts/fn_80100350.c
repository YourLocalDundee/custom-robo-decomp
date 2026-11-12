/* fn_80100350: Nov 6, 2026 hand draft (m2c 85%): two statements moved by the adjacent-swap sweep, and the unused byte local is a u8 (clrlwi, not extsb) */

void fn_80100350(void *arg0) {
    u8 sp8;

    sp8 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) lbl_80451768;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_80451768;
    *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4) = M2C_FIELD(lbl_80239F44, f32 *, 0);
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 4) = (f32) lbl_80239F44[1];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x20) = (f32) lbl_80239F44[2];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x24) = (f32) lbl_80239F44[3];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x28) = 0x15;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x40) = (f32) lbl_80239F44[4];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x44) = (f32) lbl_80239F44[5];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x60) = (f32) lbl_80239F44[6];
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x64) = (f32) lbl_80239F44[7];
}
