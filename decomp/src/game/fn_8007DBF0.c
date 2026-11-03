/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007DBF0: .text 0x8007DBF0-0x8007DD14 (292 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F620;
extern const f32 lbl_80450D98;
extern const f32 lbl_80450DE4;
extern const f32 lbl_80450DE8;
extern const f32 lbl_80450DEC;

void fn_8007DBF0(void *arg0) {

    if (!(lbl_8044F620 & 0x1000)) {
        M2C_FIELD(arg0, s8 *, 0x57) = 0x64;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) = (f32) lbl_80450D98;
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        return;
    }
    if (lbl_8044F620 & 0x2000) {
        if (M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) > lbl_80450D98) {
            M2C_FIELD((M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4)), f32 *, 0x14) -= lbl_80450DE4;
            return;
        }
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) = (f32) lbl_80450D98;
        lbl_8044F620 &= 0xFFFFEFFF;
        lbl_8044F620 &= 0xFFFFDFFF;
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) |= 1;
    M2C_FIELD((M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4)), u16 *, 0x12) |= 0x2000;
    if (M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x14) < lbl_80450DE8) {
        M2C_FIELD((M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4)), f32 *, 0x14) += lbl_80450DEC;
    }
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xC) = 0x280;
}
