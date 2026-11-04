/* fn_80023304: Nov 3, 2026 hand draft (m2c 70%): the copy loop as `for (n = ..., d = dst, s = src; n > 0; n--) *d++ = *s++;` (counter r31, dst r30, src r29) */

void fn_80023304(u8 *arg0, u8 *arg1, s32 arg2) {
    s32 n;
    u8 *d;
    u8 *s;
    for (n = arg2, d = arg0, s = arg1; n > 0; n--) {
        *d++ = *s++;
    }
}
