/* fn_800AC7F4: Nov 14, 2026 hand draft: the screen rectangle from a layout record (two f32, two u16 at 0xB4 .. 0xBE of lbl_80229F68) built in a local and copied member by member into a second Rect for fn_8008E750 (a struct copy would be lwz / stw); the per-kind callback through the table (40%) */

extern u16 *lbl_8044F728;

typedef struct {
    f32 x, y, w, h;
} Rect7F4;

typedef struct {
    u8 pad[0xB4];
    f32 x;
    f32 y;
    u16 w;
    u16 h;
} Layout7F4;

typedef struct {
    s32 flags;
    u8 pad[0x48];
    s32 unk4C;
    u8 pad50[8];
    u8 kind;
} Obj7F4;

void fn_800AC7F4(Obj7F4 *arg0) {
    Rect7F4 r;
    Rect7F4 rect;

    if (!(*lbl_8044F728 & 2)) {
        ((void (**)(Obj7F4 *)) lbl_8044EB74)[arg0->kind](arg0);
    } else {
        arg0->flags &= ~1;
    }
    r.x = ((Layout7F4 *) lbl_80229F68)->x;
    r.y = lbl_8045140C + ((Layout7F4 *) lbl_80229F68)->y;
    r.w = ((Layout7F4 *) lbl_80229F68)->w;
    r.h = ((Layout7F4 *) lbl_80229F68)->h - lbl_80451418;
    rect.x = r.x;
    rect.y = r.y;
    rect.w = r.w;
    rect.h = r.h;
    arg0->unk4C = fn_8008E750(&rect);
    arg0->flags |= 0x20000;
}
