/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SinCos16: .text 0x8001F350-0x8001F428 (216 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_801E6580[4100];

/* cr_SinCos16: Nov 13, 2026 hand draft: compiled at optimization level 4 (#pragma optimization_level 4, push / pop): a frameless leaf with its three locals in volatile registers, which level 0 cannot produce; the quadrant tests and the table pointer written plainly */

#pragma push
#pragma optimization_level 4
void cr_SinCos16(s32 a, f32 *s, f32 *c) {
    s32 idx;
    s32 i2;
    f32 *p;

    idx = a & 0x3FFF;
    if (a & 0x4000) {
        i2 = ((idx * 2) - 0x3FFF) >> 4;
        p = &((f32 *) lbl_801E6580)[((0x3FFF - idx) >> 4) + 1];
        if (a & 0x8000) {
            *s = -p[0];
            *c = p[i2];
        } else {
            *s = p[0];
            *c = -p[i2];
        }
    } else {
        i2 = (0x3FFF - (idx * 2)) >> 4;
        p = &((f32 *) lbl_801E6580)[(idx >> 4) + 1];
        if (a & 0x8000) {
            *s = -p[0];
            *c = -p[i2];
        } else {
            *s = p[0];
            *c = p[i2];
        }
    }
}
#pragma pop
