/* fn_8008CDE4: Nov 13, 2026 hand draft: keyframe max search over 32-byte records: u16 index and best index, ((void *) 0) test */

typedef struct cr_Key20 {
    f32 t;
    u8 pad[8];
    u16 val;
    u16 pad1;
    u16 end;
    u8 pad2[0xE];
} cr_Key20;

f32 fn_8008CDE4(cr_Key20 *k) {
    f32 best;
    u16 i;
    u16 bi;

    best = lbl_80451008;
    if (k == ((void *) 0)) {
        return best;
    }
    for (i = 0; k[i].end != 0xFFFF; i++) {
        if (k[i].t >= best) {
            bi = i;
            best = k[i].t;
        }
    }
    best += k[bi].val;
    best *= lbl_8045100C;
    return best;
}
