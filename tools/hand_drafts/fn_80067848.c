/* fn_80067848: Nov 13, 2026 hand draft: m2c 63%: find the first vertex record with flag 0x8000, then colour count records from there: a 32-byte record type (flags at 0x12, r g b a at 0x18), s32 / u32 counters */

typedef struct cr_Vtx20 {
    u8 pad[0x12];
    u16 flags;
    u8 pad2[4];
    u8 r, g, b, a;
    u8 rest[4];
} cr_Vtx20;

void fn_80067848(u8 *col, s16 start, s16 count, void *dobj) {
    cr_Vtx20 *v;
    s32 i;
    u32 j;

    i = 0;
    v = (cr_Vtx20 *) M2C_FIELD(M2C_FIELD(dobj, void **, 0x1C), s32 *, 4);
    while (!(v[i].flags & 0x8000)) {
        i++;
    }
    i += start;
    for (j = 0; j < count; j++) {
        v[i + j].r = col[0];
        v[i + j].g = col[1];
        v[i + j].b = col[2];
        v[i + j].a = col[3];
    }
}
