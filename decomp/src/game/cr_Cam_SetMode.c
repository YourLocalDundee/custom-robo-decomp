/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Cam_SetMode: .text 0x80024830-0x80024848 (24 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentCam;

void cr_Cam_SetMode(u8 mode) {
    M2C_FIELD(cr_g_CurrentCam, u8 *, 0) = mode;
    M2C_FIELD(cr_g_CurrentCam, s8 *, 0x13CC) = 0;
}
