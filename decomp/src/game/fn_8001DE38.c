/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001DE38: .text 0x8001DE38-0x8001DE6C (52 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern f32 cr_g_CurrentMtx[12];
extern f32 lbl_804503D0;

void fn_8001DE38(void) {
    cr_g_CurrentMtx[3] = lbl_804503D0;
    cr_g_CurrentMtx[7] = lbl_804503D0;
    cr_g_CurrentMtx[0xB] = lbl_804503D0;
}
