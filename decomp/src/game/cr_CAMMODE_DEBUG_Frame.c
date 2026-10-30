/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_CAMMODE_DEBUG_Frame: .text 0x80035ED4-0x80035F18 (68 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

M2C_UNK fn_8002E2A0();                              /* extern */
M2C_UNK fn_8002E564(s32);                           /* extern */
M2C_UNK fn_8002E944(s32);                           /* extern */
M2C_UNK fn_8002EBB0(s32);                           /* extern */

void cr_CAMMODE_DEBUG_Frame(s32 arg0) {
    fn_8002E2A0();
    fn_8002E564(arg0);
    fn_8002EBB0(arg0);
    fn_8002E944(arg0);
}
