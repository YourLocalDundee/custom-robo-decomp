/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80029A9C: .text 0x80029A9C-0x80029C20 (388 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef f32 Mtx44[4][4];
void PSMTXMultVec(Mtx44 m, Vec *src, Vec *dst);
extern const f32 lbl_804504C8;
extern const f32 lbl_8045057C;
extern const f32 lbl_804504BC;
extern const f32 lbl_80450584;
extern const f32 lbl_80450580;
extern const f32 lbl_804504D0;

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
