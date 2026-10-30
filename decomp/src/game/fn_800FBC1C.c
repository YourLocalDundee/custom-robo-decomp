/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800FBC1C: .text 0x800FBC1C-0x800FBC48 (44 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F7F1;

/* nothing in main.dol references it: keep it (mwld strips unreferenced C functions) */
#pragma force_active on
void fn_800FBC1C(s32 *arg0) {
    if ((u8) lbl_8044F7F1 == 3) {
        *arg0 |= 1;
        return;
    }
    *arg0 &= 0xFFFFFFFE;
}
#pragma force_active reset
