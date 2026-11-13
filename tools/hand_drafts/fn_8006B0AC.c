/* fn_8006B0AC: Nov 13, 2026 hand draft: m2c 43%: a 32-byte point record (x, y at 0 / 4, id at 0x10) indexed with a u8 counter, the list pointer and the counter named locals (r31 / r30), the null test against `((void *) 0)` */

typedef struct cr_Pt20 {
    f32 x;
    f32 y;
    u8 pad[8];
    u16 id;
    u8 rest[0xE];
} cr_Pt20;

void fn_8006B0AC(void) {
    cr_Pt20 *v;
    u8 i;

    if (lbl_8044F5C0 != ((void *) 0)) {
        i = 0;
        v = (cr_Pt20 *) M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F5C0, void **, 0x5C), void **, 0x1C), s32 *, 4);
        while (v[i].id != 0xFFFF) {
            v[i].id = 0xFFFF;
            v[i].x = lbl_80450AAC;
            v[i].y = lbl_80450AAC;
            i++;
        }
    }
    lbl_8044F5C4 = 0;
}
