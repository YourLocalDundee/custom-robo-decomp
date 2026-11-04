/* fn_800677C4: Nov 4, 2026 hand draft (permuter queue 3, score 0): `t = t;` inside the first scan loop */
void fn_800677C4(s16 arg0, s16 arg1, void *arg2) {
    s32 i;
    s32 t;
    u32 j;

    i = 0;
    t = M2C_FIELD(M2C_FIELD(arg2, void **, 0x1C), s32 *, 4);
    while (!(*(u16 *) &((u8 (*)[0x20]) (t))[i][0x12] & 0x8000)) {
        t = t;
        i += 1;
    }
    j = 0U;
    i += arg0;
    while (j < (u32) arg1) {
        *(u16 *) (t + ((i + j) << 5) + 0x12) |= 1;
        j += 1;
    }
}
