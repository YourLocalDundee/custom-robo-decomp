/* fn_80115918: Nov 14, 2026 hand draft: a records page: seven rectangles copied from the layout table lbl_80240904 (x, y, w, h, the two margins subtracted) into the list lbl_8041C0C0 at its count (u16 at 0x44 of lbl_8041C408), every member access through the count written out (the file reloads each global; the 0x1C-byte rect and the 0x10-byte source typed), then the A / B pad buttons (49%) */

typedef struct {
    f32 x;
    f32 y;
    f32 unk8;
    f32 w;
    f32 h;
    u8 pad[8];
} Rect918;

typedef struct {
    f32 x;
    f32 y;
    f32 w;
    f32 h;
} Src918;

typedef struct {
    u8 pad[0x44];
    u16 n;
} List918;

extern u32 *lbl_8044F85C;
extern u32 *lbl_8044F860;

void fn_80115918(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        ((Rect918 *) lbl_8041C0C0)[((List918 *) lbl_8041C408)->n].x = ((Src918 *) lbl_80240904)[((List918 *) lbl_8041C408)->n].x - lbl_80451928;
        ((Rect918 *) lbl_8041C0C0)[((List918 *) lbl_8041C408)->n].y = ((Src918 *) lbl_80240904)[((List918 *) lbl_8041C408)->n].y - lbl_8045192C;
        ((Rect918 *) lbl_8041C0C0)[((List918 *) lbl_8041C408)->n].w = ((Src918 *) lbl_80240904)[((List918 *) lbl_8041C408)->n].w - lbl_80451928;
        ((Rect918 *) lbl_8041C0C0)[((List918 *) lbl_8041C408)->n].h = ((Src918 *) lbl_80240904)[((List918 *) lbl_8041C408)->n].h - lbl_8045192C;
        ((List918 *) lbl_8041C408)->n++;
    }
    if (cr_g_Pads->unk_14 & 0x200) {
        lbl_8044F885[0] = 3;
        lbl_8044F885[1] = 0;
        *lbl_8044F85C |= 1;
        *lbl_8044F860 |= 1;
        fn_80098290(0);
    } else if (cr_g_Pads->unk_14 & 0x100) {
        lbl_8044F885[0] = 8;
        lbl_8044F885[1] = 0;
        fn_80098290(2);
    }
}
