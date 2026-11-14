/* cr_Cos16: Nov 13, 2026 hand draft: the cosine of a 16-bit angle from the quarter table, compiled at optimization level 4 like cr_SinCos16; the table is declared with its real type (f32 lbl_801E6580[1025], the context's u8 array renamed away by the `also defines` line), so its address is the array itself and the index takes r0; the quadrant index written inline in each branch */

// also defines: lbl_801E6580
extern f32 lbl_801E6580[1025];
#pragma push
#pragma optimization_level 4
f32 cr_Cos16(s32 a) {
    if (!(a & 0x8000)) {
        if (a & 0x4000) {
            return -lbl_801E6580[((a & 0x3FFF) >> 4) + 1];
        }
        return lbl_801E6580[((0x3FFF - (a & 0x3FFF)) >> 4) + 1];
    }
    if (a & 0x4000) {
        return lbl_801E6580[((a & 0x3FFF) >> 4) + 1];
    }
    return -lbl_801E6580[((0x3FFF - (a & 0x3FFF)) >> 4) + 1];
}
#pragma pop
