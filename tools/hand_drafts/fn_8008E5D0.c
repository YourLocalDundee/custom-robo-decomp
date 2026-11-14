/* fn_8008E5D0: Nov 13, 2026 hand draft: the 2D view: the four floats copied as a struct into the port record (lbl_80391430 declared with its real type), up / eye / at as Vecs, the two half sizes as float locals (hx first), the statements in the order eye.x, eye.y, eye.z, at.x, at.z, at.y (found by tools/order_climb.py from the typed draft) */

// also defines: lbl_80391430
typedef struct {
    f32 x;
    f32 y;
    f32 c;
    f32 s;
} View8E5D0;

typedef struct {
    u16 x;
    u16 y;
    u16 w;
    u16 h;
    View8E5D0 view;
} Port8E5D0;

extern Port8E5D0 lbl_80391430;

void fn_8008E5D0(View8E5D0 *arg0) {
    Mtx m;
    Vec eye;
    Vec up;
    Vec at;
    f32 hx;
    f32 hy;

    lbl_80391430.view = *arg0;
    up.x = arg0->c;
    up.y = arg0->s;
    up.z = lbl_80451008;
    hx = ((0x280 - lbl_80391430.w) >> 1) - lbl_80391430.x;
    hy = ((0x1E0 - lbl_80391430.h) >> 1) - lbl_80391430.y;
    eye.x = arg0->x - up.x * hy - up.y * hx;
    eye.y = arg0->y + up.x * hx - up.y * hy;
    eye.z = lbl_80451030;
    at.x = eye.x;
    at.z = lbl_80451008;
    at.y = eye.y;
    C_MTXLookAt(m, &eye, &up, &at);
    GXLoadPosMtxImm(m, 0U);
    GXSetCurrentMtx(0U);
}
