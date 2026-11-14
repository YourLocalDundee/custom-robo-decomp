/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Sin16: .text 0x8001F4C0-0x8001F558 (152 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* cr_Sin16: Nov 13, 2026 hand draft: the sine of a 16-bit angle from the quarter table, compiled at optimization level 4 like cr_SinCos16; the table is declared with its real type (f32 lbl_801E6580[1025], the context's u8 array renamed away by the `also defines` line), so its address is the array itself and the index takes r0; the quadrant index written inline in each branch */

// also defines: lbl_801E6580
extern f32 lbl_801E6580[1025];
#pragma push
#pragma optimization_level 4
f32 cr_Sin16(s32 a) {
    if (!(a & 0x8000)) {
        if (a & 0x4000) {
            return lbl_801E6580[((0x3FFF - (a & 0x3FFF)) >> 4) + 1];
        }
        return lbl_801E6580[((a & 0x3FFF) >> 4) + 1];
    }
    if (a & 0x4000) {
        return -lbl_801E6580[((0x3FFF - (a & 0x3FFF)) >> 4) + 1];
    }
    return -lbl_801E6580[((a & 0x3FFF) >> 4) + 1];
}
#pragma pop
