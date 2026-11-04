/* fn_80023354: Nov 3, 2026 hand draft (m2c 62-68%): the clear loop as `for (n = ..., p = arg; ...; n--) *p++ = 0;` (the counter takes r31, the pointer r30) */

void fn_80023354(s32 *arg0, u32 arg1) {
    u32 n;
    s32 *p;
    for (n = arg1 >> 2, p = arg0; n != 0; n--) {
        *p++ = 0;
    }
}
