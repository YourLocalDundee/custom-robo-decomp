/* fn_8012F28C: Nov 13, 2026 hand draft: vertex weight sum: top-tested loop with break, two += statements, (void *) 0 test */

typedef struct cr_Vtx20F {
    u8 pad[0xC];
    u16 c;
    u8 pad2[2];
    u16 flags;
    u8 pad3[2];
    f32 w;
    u8 rest[8];
} cr_Vtx20F;

f32 fn_8012F28C(void *arg0) {
    f32 sum;
    cr_Vtx20F *v;

    sum = lbl_80451BE4;
    v = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
    if (v != ((void *) 0)) {
        while (1) {
            if (v->flags == 0xFFFF) {
                break;
            }
            sum += v->c * v->w;
            sum += lbl_80451BE8 * v->w;
            v++;
        }
    }
    return sum * lbl_80451A54;
}
