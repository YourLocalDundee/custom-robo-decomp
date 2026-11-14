/* fn_80029924: Nov 13, 2026 hand draft: the screen projection written with typed locals: the camera rectangle copied as a struct, the near plane a dead stack local, the matrix parameter (one use) in its stack home, the point a Vec; five float locals (w has the most uses: f31); later-declared stack objects get the lower addresses */

typedef struct { f32 l, r, t, b; } PRect;
typedef struct { u8 pad0[8]; f32 near; u8 padC[0xC]; f32 fov; u8 pad1C[0xC]; PRect rect; } PCam;
void fn_80029924(PCam *cam, f32 (*m)[4], Vec *pos, f32 *outx, f32 *outy) {
    Vec v;
    PRect rect;
    f32 near;
    f32 w;
    f32 sx;
    f32 sy;
    f32 fov;
    f32 h;

    near = cam->near;
    rect = cam->rect;
    fov = cam->fov;
    v.x = pos->x;
    v.y = pos->y;
    v.z = pos->z;
    PSMTXMultVec(m, &v, &v);
    sx = fov / -v.z;
    sy = lbl_804504C8 / -v.z;
    w = rect.r - rect.l;
    h = rect.b - rect.t;
    *outx = rect.l + ((lbl_80450528 * w) + ((lbl_804505C0 * w) * (v.x * sx)));
    *outy = rect.t + ((lbl_804505C4 * h) + ((lbl_804505C8 * w) * (-v.y * sy)));
}
