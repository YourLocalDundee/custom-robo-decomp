/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001DFE8: .text 0x8001DFE8-0x8001E054 (108 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];

/* fn_8001DFE8: Nov 13, 2026 hand draft: m2c 31%: copies the 3x3 part into the current matrix with post-increments (`*m++ = *s++`, a skip after each row), returns s: MWCC folds them into offsets and two final addi 0x2c */

f32 *fn_8001DFE8(f32 *s) {
    f32 *m = cr_g_CurrentMtx;

    *m++ = *s++;
    *m++ = *s++;
    *m++ = *s++;
    m++;
    s++;
    *m++ = *s++;
    *m++ = *s++;
    *m++ = *s++;
    m++;
    s++;
    *m++ = *s++;
    *m++ = *s++;
    *m++ = *s++;
    return s;
}
