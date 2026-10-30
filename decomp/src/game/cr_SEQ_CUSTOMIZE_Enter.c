/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_CUSTOMIZE_Enter: .text 0x800157F8-0x8001581C (36 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

M2C_UNK cr_Cam_Init();                              /* extern */
M2C_UNK fn_8010A5CC();                              /* extern */

void cr_SEQ_CUSTOMIZE_Enter(void) {
    cr_Cam_Init();
    fn_8010A5CC();
}
