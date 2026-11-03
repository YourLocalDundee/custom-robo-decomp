/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Vec3_Normalize: .text 0x8001CA84-0x8001CB4C (200 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
f64 sqrt(f64 x);
extern const f32 lbl_804503D0;
extern const f32 lbl_80450408;

void cr_Vec3_Normalize(void *arg0) {
    f32 temp_f31_27;
    f32 temp_f30_21;

    temp_f30_21 = (f32) sqrt((f64) ((M2C_FIELD(arg0, f32 *, 0) * M2C_FIELD(arg0, f32 *, 0)) + ((M2C_FIELD(arg0, f32 *, 4) * M2C_FIELD(arg0, f32 *, 4)) + (M2C_FIELD(arg0, f32 *, 8) * M2C_FIELD(arg0, f32 *, 8)))));
    if (temp_f30_21 != lbl_804503D0) {
        temp_f31_27 = lbl_80450408 / temp_f30_21;
        M2C_FIELD(arg0, f32 *, 0) *= temp_f31_27;
        M2C_FIELD(arg0, f32 *, 4) *= temp_f31_27;
        M2C_FIELD(arg0, f32 *, 8) *= temp_f31_27;
        return;
    }
    M2C_FIELD(arg0, f32 *, 0) = (f32) lbl_804503D0;
    M2C_FIELD(arg0, f32 *, 4) = (f32) lbl_804503D0;
    M2C_FIELD(arg0, f32 *, 8) = (f32) lbl_804503D0;
}
