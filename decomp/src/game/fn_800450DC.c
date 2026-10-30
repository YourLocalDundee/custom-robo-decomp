/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800450DC: .text 0x800450DC-0x800450EC (16 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

s32 fn_800450DC(void *arg0) {
    return M2C_FIELD(arg0, s16 *, 0x146) == 0;
}
