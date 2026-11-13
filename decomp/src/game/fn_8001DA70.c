/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001DA70: .text 0x8001DA70-0x8001DAF8 (136 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* fn_8001DA70: Nov 5, 2026 hand draft (m2c 34%): scale cr_g_CurrentMtx's columns by x, y, z (rows 0-2 of the 3x3 part); one local pointer as in fn_8001D9E8 */
void fn_8001DA70(f32 x, f32 y, f32 z) {
    f32 *m = cr_g_CurrentMtx;
    m[0] *= x;
    m[1] *= x;
    m[2] *= x;
    m[4] *= y;
    m[5] *= y;
    m[6] *= y;
    m[8] *= z;
    m[9] *= z;
    m[10] *= z;
}
