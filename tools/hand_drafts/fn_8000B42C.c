/* fn_8000B42C: Nov 13, 2026 hand draft: the rank sweep: one dead use of the parameter ((void) p;) gives it its saved register in the original order */

typedef struct cr_Col {
    u8 r, g, b, a;
} cr_Col;

static inline void set_col(cr_Col c, u32 i, void *obj) {
    cr_Col *p = &c;

    fn_80067848((u8 *) p, (s16) i, 1, M2C_FIELD(obj, void **, 0x5C));
}

void fn_8000B42C(void *obj) {
    u32 i;
    u32 n = 0x6E;
    cr_Col on = { 0xFF, 0xFC, 0x00, 0xFF };
    cr_Col off = { 0xFF, 0xFF, 0xFF, 0xFF };

    (void) obj; 
    for (i = 0; i < n; i++) {
        if (i == M2C_FIELD(lbl_8031BEC4, u32 *, 0) + lbl_8031BEC4[1] * lbl_8044C3F7[M2C_FIELD(lbl_8031BEC4, s8 *, 0x10)]) {
            set_col(on, i, obj);
        } else {
            set_col(off, i, obj);
        }
    }
}
