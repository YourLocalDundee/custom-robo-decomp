/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Rand_Float: .text 0x8001B388-0x8001B3F0 (104 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044C408[2];
extern u8 lbl_801F10C0[256];
extern const f32 lbl_80450408;

/* cr_Rand_Float: Nov 13, 2026 hand draft: m2c 44%: the generator step, then the mantissa in a union on the stack and the result a named f32 local (f31) */

f32 cr_Rand_Float(void) {
    union {
        s32 i;
        f32 f;
    } u;
    f32 r;

    lbl_8044C408[0] = (lbl_8044C408[0] * 8 + ((u32 *) lbl_801F10C0)[lbl_8044C408[0] & 0x3F]) >> 4;
    u.i = (lbl_8044C408[0] & 0x7FFFFF) | 0x3F800000;
    r = u.f - lbl_80450408;
    return r;
}
