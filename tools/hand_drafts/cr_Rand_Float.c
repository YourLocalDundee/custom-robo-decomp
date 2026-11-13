/* cr_Rand_Float: Nov 13, 2026 hand draft: m2c 44%: the generator step, then the mantissa in a union on the stack and the result a named f32 local (f31) */

f32 cr_Rand_Float(void) {
    union {
        s32 i;
        f32 f;
    } u;
    f32 r;

    lbl_8044C408[0] = (lbl_8044C408[0] * 8 + ((u32 *) lbl_801F10C0)[lbl_8044C408[0] & 0x3F]) >> 4;
    u.i = (lbl_8044C408[0] & 0x7FFFFF) | 0x3F800000;
    r = u.f - lbl_80450408;
    return r;
}
