/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008578C: .text 0x8008578C-0x800858B0 (292 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
typedef Vec Vec3;
typedef f32 Mtx[3][4];
void PSMTXIdentity(Mtx m);
void *fn_80084908(void *arg0, void *arg1, f64 farg0);
extern const f32 lbl_80450F58;
void fn_80084CFC(void *arg0, u8 arg1);
void PSMTXConcat(Mtx mA, Mtx mB, Mtx mAB);
typedef f32 Mtx44[4][4];
void PSMTXMultVec(Mtx44 m, Vec *src, Vec *dst);
void fn_80085280(void *arg0);

/* fn_8008578C: hand-written from main.dol's asm on its m2c draft (plan.md step 109a): the locals typed: three Vec3 (the matrix columns) and an Mtx, which gives the frame of 0x70 (m2c's stack struct stops at the last word read: 0x60), the columns copied out of and back into the matrix element by element */
void fn_8008578C(s32 arg0) {
    Vec3 a;
    Vec3 b;
    Vec3 c;
    Mtx m;

    PSMTXIdentity(m);
    a.x = m[0][0];
    a.y = m[1][0];
    a.z = m[2][0];
    b.x = m[0][1];
    b.y = m[1][1];
    b.z = m[2][1];
    c.x = m[0][2];
    c.y = m[1][2];
    c.z = m[2][2];
    fn_80084908(&a, (void *) (arg0 + 0x254), (f64) lbl_80450F58);
    fn_80084908(&b, (void *) (arg0 + 0x254), (f64) lbl_80450F58);
    fn_80084908(&c, (void *) (arg0 + 0x254), (f64) lbl_80450F58);
    m[0][0] = a.x;
    m[1][0] = a.y;
    m[2][0] = a.z;
    m[0][1] = b.x;
    m[1][1] = b.y;
    m[2][1] = b.z;
    m[0][2] = c.x;
    m[1][2] = c.y;
    m[2][2] = c.z;
    fn_80084CFC(m, 0U);
    PSMTXConcat(m, (float (*)[4]) (arg0 + 0x58), (float (*)[4]) (arg0 + 0x58));
    PSMTXMultVec(m, (void *) (arg0 + 0x248), (void *) (arg0 + 0x248));
    fn_80085280((void *) arg0);
}
