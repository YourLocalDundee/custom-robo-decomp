/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8002B284: .text 0x8002B284-0x8002B3C4 (320 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentCam;
typedef f32 Mtx44[4][4];
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
void PSMTXMultVec(Mtx44 m, Vec *src, Vec *dst);
extern f32 lbl_8032344C[237];
typedef Vec Point3d;
extern f32 lbl_804504D0;

s32 fn_8002B284(void *arg0, f32 farg0) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
    } sp;
    f32 temp_f31_36;
    f32 temp_f30_54;
    u8 *temp_r31_17;

    temp_r31_17 = cr_g_CurrentCam;
    sp.sp8 = M2C_FIELD(arg0, f32 *, 0);
    sp.spC = M2C_FIELD(arg0, f32 *, 4);
    sp.sp10 = M2C_FIELD(arg0, f32 *, 8);
    PSMTXMultVec((f32 (*)[4]) lbl_8032344C, (Point3d *) &sp.sp8, (Point3d *) &sp.sp8);
    temp_f31_36 = (sp.sp10 * M2C_FIELD(temp_r31_17, f32 *, 0x14C)) + ((sp.sp8 * M2C_FIELD(temp_r31_17, f32 *, 0x144)) + (sp.spC * M2C_FIELD(temp_r31_17, f32 *, 0x148)));
    if ((temp_f31_36 < lbl_804504D0) && (-temp_f31_36 > farg0)) {
        return 0;
    }
    temp_f30_54 = (sp.sp10 * M2C_FIELD(temp_r31_17, f32 *, 0x158)) + ((sp.sp8 * M2C_FIELD(temp_r31_17, f32 *, 0x150)) + (sp.spC * M2C_FIELD(temp_r31_17, f32 *, 0x154)));
    if ((temp_f30_54 < lbl_804504D0) && (-temp_f30_54 > farg0)) {
        return 0;
    }
    if ((temp_f31_36 < lbl_804504D0) && (temp_f30_54 < lbl_804504D0)) {
        return 0;
    }
    return 1;
}
