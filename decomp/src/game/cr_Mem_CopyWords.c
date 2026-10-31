/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Mem_CopyWords: .text 0x80023264-0x800232B4 (80 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

void cr_Mem_CopyWords(void *dst, void *src, u32 bytes) {
    u32 var_r31_4;
    u8 *var_r30_6;
    u8 *var_r29_8;

    var_r31_4 = bytes >> 2U;
    var_r30_6 = dst;
    var_r29_8 = src;
    while ((s32) var_r31_4 > 0) {
        M2C_FIELD(var_r30_6, u32 *, 0) = (M2C_FIELD(var_r29_8, u32 *, 0));
        var_r29_8 += 4;
        var_r31_4 -= 1;
        var_r30_6 += 4;
    }
}
