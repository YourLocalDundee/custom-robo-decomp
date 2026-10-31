/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_C_ACT_JP: .text 0x800BDB24-0x800BDB54 (48 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern s16 *cr_g_ScriptPC;

void cr_C_ACT_JP(void) {
    s16 *temp_r31_5;

    temp_r31_5 = cr_g_ScriptPC;
    cr_g_ScriptPC += 4;
    cr_g_ScriptPC = *M2C_FIELD(temp_r31_5, s16 ***, 4);
}
