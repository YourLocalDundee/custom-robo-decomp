/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800EF2B0: .text 0x800EF2B0-0x800EF2E0 (48 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80023228(s32 arg0, u32 arg1);
extern u32 lbl_803BE7DC[1102];

void fn_800EF2B0(void) {
    fn_80023228((s32) &lbl_803BE7DC[0x418], 0x1DU);
}
