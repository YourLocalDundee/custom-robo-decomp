/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Motion_PlayFromFrame1: .text 0x80037FBC-0x80037FF0 (52 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F4EC;
void cr_Motion_Play(u32 request, u32 frame);

void cr_Motion_PlayFromFrame1(u32 id) {

    lbl_8044F4EC = 0;
    cr_Motion_Play(id, 1U);
}
