/* fn_80115D04: Nov 14, 2026 hand draft: fn_80115918's twin; the B button goes to page 9 (49%) */

typedef struct {
    f32 x;
    f32 y;
    f32 unk8;
    f32 w;
    f32 h;
    u8 pad[8];
} RectD04;

typedef struct {
    f32 x;
    f32 y;
    f32 w;
    f32 h;
} SrcD04;

typedef struct {
    u8 pad[0x44];
    u16 n;
} ListD04;

extern u32 *lbl_8044F85C;
extern u32 *lbl_8044F860;

void fn_80115D04(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        ((RectD04 *) lbl_8041C0C0)[((ListD04 *) lbl_8041C408)->n].x = ((SrcD04 *) lbl_80240904)[((ListD04 *) lbl_8041C408)->n].x - lbl_80451928;
        ((RectD04 *) lbl_8041C0C0)[((ListD04 *) lbl_8041C408)->n].y = ((SrcD04 *) lbl_80240904)[((ListD04 *) lbl_8041C408)->n].y - lbl_8045192C;
        ((RectD04 *) lbl_8041C0C0)[((ListD04 *) lbl_8041C408)->n].w = ((SrcD04 *) lbl_80240904)[((ListD04 *) lbl_8041C408)->n].w - lbl_80451928;
        ((RectD04 *) lbl_8041C0C0)[((ListD04 *) lbl_8041C408)->n].h = ((SrcD04 *) lbl_80240904)[((ListD04 *) lbl_8041C408)->n].h - lbl_8045192C;
        ((ListD04 *) lbl_8041C408)->n++;
    }
    if (cr_g_Pads->unk_14 & 0x200) {
        lbl_8044F885[0] = 3;
        lbl_8044F885[1] = 0;
        *lbl_8044F85C |= 1;
        *lbl_8044F860 |= 1;
        fn_80098290(0);
    } else if (cr_g_Pads->unk_14 & 0x100) {
        lbl_8044F885[0] = 9;
        lbl_8044F885[1] = 0;
        fn_80098290(2);
    }
}
