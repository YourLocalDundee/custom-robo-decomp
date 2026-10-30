/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800EC9F0: .text 0x800EC9F0-0x800ECA28 (56 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_800F56B0(void *arg0);
extern u8 lbl_80400420[360];

void fn_800EC9F0(s32 arg0) {

    fn_800F56B0(lbl_80400420 + (arg0 * 0xB4));
}
