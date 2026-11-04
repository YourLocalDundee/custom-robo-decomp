/* fn_800443A0: Nov 3, 2026 hand draft (m2c 66%): the two equal fields set in one chained assignment, 8 = 0xC = value (relocations checked with hd.py -r) */

void fn_800443A0(void *arg0, s32 arg1, s32 arg2) {
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 4) = arg2;
    M2C_FIELD(arg0, f32 *, 8) = M2C_FIELD(arg0, f32 *, 0xC) = lbl_80450654;
    M2C_FIELD(arg0, f32 *, 0x10) = lbl_80450638;
}
