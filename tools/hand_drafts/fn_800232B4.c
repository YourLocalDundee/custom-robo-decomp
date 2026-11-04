/* fn_800232B4: Nov 3, 2026 hand draft (m2c 70%): the copy loop as `for (n = ..., d = dst, s = src; n > 0; n--) *d++ = *s++;` (counter r31, dst r30, src r29) */

void fn_800232B4(u16 *arg0, u16 *arg1, u32 arg2) {
    s32 n;
    u16 *d;
    u16 *s;
    for (n = arg2 >> 1, d = arg0, s = arg1; n > 0; n--) {
        *d++ = *s++;
    }
}
