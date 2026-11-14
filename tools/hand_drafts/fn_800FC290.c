/* fn_800FC290: Nov 13, 2026 hand draft: three locals with initialisers: f32 offs[5] (lbl_801C3D90, .rodata), u16 first[1] and u16 ids[4] (the five .sdata2 halfwords); the position a local f32[2]; the slot-bit search the inline of fn_800FBDF8; the right edge one flat sum n * 30 + n * 30 + 30 */

static inline u8 first_slot_bit(u8 n) {
    u8 i;

    for (i = 0; i < 4U; i++) {
        if ((0x400 << i) & M2C_FIELD((lbl_80407260 + (n * 0x4994)), s32 *, 0x498C)) {
            return i;
        }
    }
    return 0xFF;
}

void fn_800FC290(void *arg0) {
    f32 t0[5] = { 0.0f, 6.0f, 0.0f, 7.0f, 2.0f };
    f32 pos[2];
    u16 a[1] = { 0x4A };
    u16 ids[4] = { 0x47, 0x46, 0x49, 0x48 };
    void *temp_r31_50;
    u8 n;
    u8 var_r27_227;

    n = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    if ((M2C_FIELD((lbl_80407260 + (n * 0x4994)), s32 *, 0x487C) & 0x10000) && !(M2C_FIELD((lbl_80407260 + (n * 0x4994)), s32 *, 0x498C) & 0x80000000)) {
        temp_r31_50 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        M2C_FIELD(temp_r31_50, s16 *, 0xA) = (n * 0x1E) + (n * 0x1E);
        M2C_FIELD(temp_r31_50, s16 *, 0x2A) = n * 0x1E + n * 0x1E + 0x1E;
        M2C_FIELD(temp_r31_50, f32 *, 0x40) = (f32) M2C_FIELD(lbl_8044EC68, f32 *, 0);
        M2C_FIELD(temp_r31_50, f32 *, 0x44) = (f32) lbl_8044EC68[1];
        if ((M2C_FIELD((lbl_80407260 + (n * 0x4994)), u8 *, 0x4855) == 1) || (M2C_FIELD((lbl_80407260 + (n * 0x4994)), u8 *, 0x4855) == 2)) {
            M2C_FIELD(temp_r31_50, s16 *, 0xE) = 0x1E;
            M2C_FIELD(temp_r31_50, s16 *, 0x2E) = 0x1E;
            M2C_FIELD(temp_r31_50, s16 *, 0x4E) = 0x1E;
            if (M2C_FIELD((lbl_80407260 + (n * 0x4994)), u8 *, 0x4866) != 0) {
                M2C_FIELD(temp_r31_50, u16 *, 0x50) = ids[M2C_FIELD((lbl_80407260 + (n * 0x4994)), u8 *, 0x4866) - 1];
            } else {
                M2C_FIELD(temp_r31_50, u16 *, 0x50) = a[0];
            }
            pos[0] = M2C_FIELD(lbl_8044EC58, f32 *, 0);
            pos[1] = lbl_8044EC58[1];
        } else if (M2C_FIELD((lbl_80407260 + (n * 0x4994)), u8 *, 0x4855) == 0) {
            if (((s32) M2C_FIELD((lbl_80407260 + (n * 0x4994)), u8 *, 0x4860) != 5) && (M2C_FIELD((lbl_80407260 + (n * 0x4994)), s8 *, 0x46C2) != -1)) {
                M2C_FIELD(temp_r31_50, s16 *, 0xE) = 0;
                M2C_FIELD(temp_r31_50, s16 *, 0x2E) = 0;
                M2C_FIELD(temp_r31_50, s16 *, 0x4E) = 0x1E;
                if (M2C_FIELD((lbl_80407260 + (n * 0x4994)), s8 *, 0x4860) == 0) {
                    M2C_FIELD(temp_r31_50, u16 *, 0x50) = a[0];
                } else {
                    M2C_FIELD(temp_r31_50, u16 *, 0x50) = ids[M2C_FIELD((lbl_80407260 + (n * 0x4994)), s8 *, 0x4860) - 1];
                }
                pos[0] = (M2C_FIELD(lbl_8044ECA0, f32 *, 0) - lbl_804517B8) + t0[M2C_FIELD((lbl_80407260 + (n * 0x4994)), s8 *, 0x4860)];
                pos[1] = lbl_8044ECA0[1] - lbl_804517B4;
            } else {
                M2C_FIELD(temp_r31_50, s16 *, 0xE) = 0;
                M2C_FIELD(temp_r31_50, s16 *, 0x2E) = 0;
                M2C_FIELD(temp_r31_50, s16 *, 0x4E) = 0;
            }
        }
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = pos[0];
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = pos[1];
        M2C_FIELD(temp_r31_50, f32 *, 0x20) = (f32) lbl_80451758;
        var_r27_227 = first_slot_bit(n);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += ((f32 (*)[2]) lbl_80239B48)[var_r27_227][0];
        M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) += ((f32 (*)[2]) lbl_80239B48)[var_r27_227][1];
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
