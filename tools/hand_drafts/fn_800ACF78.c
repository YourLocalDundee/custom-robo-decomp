/* fn_800ACF78: Nov 14, 2026 hand draft: fn_800AC7F4's twin: the layout fields at 0x9C .. 0xA6, the callback table lbl_8044EB6C (40%) */

extern u16 *lbl_8044F728;

typedef struct {
    f32 x, y, w, h;
} RectF78;

typedef struct {
    u8 pad[0x9C];
    f32 x;
    f32 y;
    u16 w;
    u16 h;
} LayoutF78;

typedef struct {
    s32 flags;
    u8 pad[0x48];
    s32 unk4C;
    u8 pad50[8];
    u8 kind;
} ObjF78;

void fn_800ACF78(ObjF78 *arg0) {
    RectF78 r;
    RectF78 rect;

    if (!(*lbl_8044F728 & 2)) {
        ((void (**)(ObjF78 *)) lbl_8044EB6C)[arg0->kind](arg0);
    } else {
        arg0->flags &= ~1;
    }
    r.x = ((LayoutF78 *) lbl_80229F68)->x;
    r.y = lbl_8045140C + ((LayoutF78 *) lbl_80229F68)->y;
    r.w = ((LayoutF78 *) lbl_80229F68)->w;
    r.h = ((LayoutF78 *) lbl_80229F68)->h - lbl_80451418;
    rect.x = r.x;
    rect.y = r.y;
    rect.w = r.w;
    rect.h = r.h;
    arg0->unk4C = fn_8008E750(&rect);
    arg0->flags |= 0x20000;
}
