/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80078DC0: .text 0x80078DC0-0x80078E54 (148 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80379FE0[432];
extern u8 *lbl_8044F348;

void fn_80078DC0(void *arg0) {
    s32 var_r31_10;

    if (M2C_FIELD(arg0, u32 *, 0) != 0U) {
        M2C_FIELD(arg0, s32 *, 8) = 1;
    }
    var_r31_10 = 0;
    while (var_r31_10 < 8) {
        if (M2C_FIELD(arg0, u32 *, 0x90) == (u32) M2C_FIELD((lbl_80379FE0 + (M2C_FIELD(lbl_8044F348, u8 *, 0x8C) * 0x6C) + (var_r31_10 * 0xC)), u32 *, 4)) {
            *(lbl_80379FE0 + (M2C_FIELD(lbl_8044F348, u8 *, 0x8C) * 0x6C) + (var_r31_10 * 0xC)) = 0;
        }
        var_r31_10 += 1;
    }
}
