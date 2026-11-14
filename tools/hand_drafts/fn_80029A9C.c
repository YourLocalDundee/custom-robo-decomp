/* fn_80029A9C: Nov 13, 2026 hand draft: the projection of fn_80029924 with a result (in front of the camera or not): both camera rectangles copied as structs, the near plane, the width and the view origin dead stack locals (the width computed first: its float registers are the higher ones), the matrix parameter in its stack home */

typedef struct { f32 l, r, t, b; } PRect;
typedef struct { u8 pad0[8]; f32 near; u8 padC[0xC]; f32 fov; u8 pad1C[0xC]; PRect rect; u8 pad38[0x18]; PRect view; } PCam;
s32 fn_80029A9C(PCam *cam, f32 (*m)[4], Vec *pos, f32 *outx, f32 *outy) {
    Vec v;
    PRect rect;
    PRect view;
    f32 near;
    f32 w;
    f32 x0;
    f32 y0;
    f32 sx;
    f32 sy;
    f32 fov;

    near = cam->near;
    rect = cam->rect;
    view = cam->view;
    fov = cam->fov;
    v.x = pos->x;
    v.y = pos->y;
    v.z = pos->z;
    PSMTXMultVec(m, &v, &v);
    sx = fov / -v.z;
    sy = lbl_804504C8 / -v.z;
    w = cam->rect.r - cam->rect.l;
    x0 = cam->view.l;
    y0 = cam->view.t;
    *outx = (lbl_8045057C * (v.x * sx)) + lbl_804504BC;
    *outy = (lbl_80450584 * (-v.y * sy)) + lbl_80450580;
    if (v.z >= lbl_804504D0) {
        return 0;
    }
    return 1;
}
