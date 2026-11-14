/* fn_80069AD8: Nov 13, 2026 hand draft: three dead uses of the text pointer, two each of the glyph record and the string (found by trying the three parameters together: one at a time does not get there); the page stored through a u16 field */

extern u8 *lbl_8044F6A8;

typedef struct cr_GlyphRange {
    u8 pad[8];
    u16 type;
    u16 lo;
    u16 hi;
} cr_GlyphRange;

s16 fn_80069AD8(u8 *v, u8 *g, void *s) {
    u16 c;
    s16 len;
    s16 i;
    u32 glyph;
    u8 page;

    (void) v; (void) v; (void) v; (void) g; (void) g; (void) s; (void) s; 
    len = 0;
    if (((u8 *) s)[len] >= 0x81U) {
        page = (((u8 *) s)[len] - 0x81) / 7;
        c = *(u16 *) &((u8 *) s)[len];
        len = (s8) (len + 2);
    } else {
        c = ((u8 *) s)[len];
        len = (s8) (len + 1);
        page = 0;
    }
    if (c == 0x8140) {
        c = 0x20;
    }
    i = 0;
    lbl_8044F6A8 = lbl_80396258 + (page * 0x88);
    for (; i < 0x20; i++) {
        if ((*(cr_GlyphRange **) &((u8 (*)[4]) lbl_8044F6A8)[i][8]) == ((void *) 0)) {
            (void) 0;
        }
        if ((c >= (*(cr_GlyphRange **) &((u8 (*)[4]) lbl_8044F6A8)[i][8])->lo) && (c <= (*(cr_GlyphRange **) &((u8 (*)[4]) lbl_8044F6A8)[i][8])->hi)) {
            glyph = ((u32 (**)(u16, s16)) lbl_8020DD2C)[(*(cr_GlyphRange **) &((u8 (*)[4]) lbl_8044F6A8)[i][8])->type](c, i);
            break;
        }
        if (i >= 0x20) {
            (void) 0;
        }
    }
    if ((u16) glyph > 0x1FU) {
        M2C_FIELD(g, u16 *, 0x10) = (u16) glyph >> 5;
    } else {
        M2C_FIELD(g, s16 *, 0x10) = 0;
    }
    M2C_FIELD(g, s16 *, 0xE) = (u16) glyph & 0x1F;
    fn_8006975C(g, v, glyph);
    M2C_FIELD(v, u16 *, 0x12) |= 0x4000;
    if ((c == 0x20) || (c == 0x81A5) || (c == 0x8163) || (c == 0x8141) || (c == 0x8142) || (c == 0x8148) || (c == 0x8149)) {
        M2C_FIELD(v, u16 *, 0x12) &= 0xFFFFBFFF;
    }
    M2C_FIELD(v, u16 *, 0x12) |= 0x3000;
    M2C_FIELD(g, u8 *, 0xC) += 1;
    return len;
}
