/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001D9E8: .text 0x8001D9E8-0x8001DA70 (136 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* fn_8001D9E8: Nov 5, 2026 hand draft (m2c 34%): scale the 3x3 part of cr_g_CurrentMtx by one factor; every access through one local pointer (the original keeps it in r31), m2c re-read the global each time */
void fn_8001D9E8(f32 s) {
    f32 *m = cr_g_CurrentMtx;
    m[0] *= s;
    m[1] *= s;
    m[2] *= s;
    m[4] *= s;
    m[5] *= s;
    m[6] *= s;
    m[8] *= s;
    m[9] *= s;
    m[10] *= s;
}
