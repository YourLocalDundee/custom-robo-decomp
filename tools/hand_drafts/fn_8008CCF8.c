/* fn_8008CCF8: Nov 13, 2026 hand draft: sibling of fn_8008CDE4: scaled key search with a found flag, u16 indices */

typedef struct cr_Key20b {
    u8 pad[4];
    f32 t;
    u8 pad1[6];
    u16 val;
    u16 end;
    u16 pad2;
    f32 scale;
    u8 pad3[8];
} cr_Key20b;

f32 fn_8008CCF8(cr_Key20b *k) {
    f32 best;
    u16 i;
    u8 found;
    u16 bi;

    best = lbl_80451008;
    found = 0;
    if (k == ((void *) 0)) {
        return best;
    }
    for (i = 0; k[i].end != 0xFFFF; i++) {
        if (k[i].t >= best) {
            bi = i;
            found = 1;
            best = k[i].t * k[i].scale;
        }
    }
    if (found != 0) {
        best += k[bi].val * k[bi].scale;
    }
    return best;
}
