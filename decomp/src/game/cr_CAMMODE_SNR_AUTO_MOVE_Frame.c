/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_CAMMODE_SNR_AUTO_MOVE_Frame: .text 0x8002FAB4-0x8002FAD0 (28 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void cr_CAMMODE_SNR_AUTO_MOVE_Frame(void *arg0) {
    if ((s32) M2C_FIELD(arg0, s32 *, 0xFC) == 0) {
        M2C_FIELD(arg0, u8 *, 0x168) = (u8) (M2C_FIELD(arg0, u8 *, 0x168) & 0xFFFFFFFB);
    }
}
