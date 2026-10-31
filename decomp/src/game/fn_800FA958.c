/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800FA958: .text 0x800FA958-0x800FA9CC (116 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];

void fn_800FA958(void *arg0) {
    u8 var_r31;

    var_r31 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    if ((M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x498C) & 0x80000000) && (M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u8 *, 0x4855) != 3)) {
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
