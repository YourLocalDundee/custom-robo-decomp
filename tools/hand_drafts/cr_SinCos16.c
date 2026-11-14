/* cr_SinCos16: Nov 13, 2026 hand draft: compiled at optimization level 4 (#pragma optimization_level 4, push / pop): a frameless leaf with its three locals in volatile registers, which level 0 cannot produce; the quadrant tests and the table pointer written plainly */

#pragma push
#pragma optimization_level 4
void cr_SinCos16(s32 a, f32 *s, f32 *c) {
    s32 idx;
    s32 i2;
    f32 *p;

    idx = a & 0x3FFF;
    if (a & 0x4000) {
        i2 = ((idx * 2) - 0x3FFF) >> 4;
        p = &((f32 *) lbl_801E6580)[((0x3FFF - idx) >> 4) + 1];
        if (a & 0x8000) {
            *s = -p[0];
            *c = p[i2];
        } else {
            *s = p[0];
            *c = -p[i2];
        }
    } else {
        i2 = (0x3FFF - (idx * 2)) >> 4;
        p = &((f32 *) lbl_801E6580)[(idx >> 4) + 1];
        if (a & 0x8000) {
            *s = -p[0];
            *c = -p[i2];
        } else {
            *s = p[0];
            *c = p[i2];
        }
    }
}
#pragma pop
