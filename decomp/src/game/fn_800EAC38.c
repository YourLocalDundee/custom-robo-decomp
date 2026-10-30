/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800EAC38: .text 0x800EAC38-0x800EAC48 (16 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

s32 fn_800EAC38(void *arg0, u8 *arg1) {
    *arg1 = M2C_FIELD(arg0, u8 *, 0x6B);
    return 1;
}
