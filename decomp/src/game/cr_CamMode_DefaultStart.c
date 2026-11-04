/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_CamMode_DefaultStart: .text 0x800247D0-0x80024830 (96 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef void (*cr_CamModeFunc)(u8 *cam);
__declspec(section ".data") extern cr_CamModeFunc cr_g_CamModeFuncs[42];

/* cr_CamMode_DefaultStart: Nov 3, 2026 hand draft (m2c 70%): the mode function called with the camera (arg0), `void (*)(void *)` */

void cr_CamMode_DefaultStart(void *arg0) {

    M2C_FIELD(arg0, u8 *, 0x13CC) = 1U;
    (*(void (**)(void *)) ((u8 *) cr_g_CamModeFuncs + (M2C_FIELD(arg0, u8 *, 0) * 8) + (M2C_FIELD(arg0, u8 *, 0x13CC) * 4)))(arg0);
}
