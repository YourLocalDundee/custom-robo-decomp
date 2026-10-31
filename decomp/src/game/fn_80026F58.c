/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80026F58: .text 0x80026F58-0x80027170 (536 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F461;
extern u8 lbl_8024E120[840832];
extern u8 lbl_80321E90[5088];
extern u32 lbl_80387160[796];
extern u32 lbl_8037569C[2417];
extern u8 *lbl_8044F47C;

void fn_80026F58(s32 arg0) {
    u8 var_r31;

    var_r31 = 0;
    while (var_r31 < (u8) lbl_8044F461) {
        if (!(M2C_FIELD((lbl_8024E120 + (var_r31 * 0x33520)), s32 *, 0x1D8) & 0x10100000)) {
            M2C_FIELD((lbl_8024E120 + (var_r31 * 0x33520)), s32 *, 0x1D8) &= 0xFFDFFFFF;
            if (!((&lbl_80321E90[(arg0 * 4) & 0x3FC])[var_r31] & 1)) {
                M2C_FIELD((lbl_8024E120 + (var_r31 * 0x33520)), s32 *, 0x1D8) |= 0x200000;
            }
        } else if ((M2C_FIELD((lbl_8024E120 + (var_r31 * 0x33520)), s32 *, 0x1D8) & 0x10000000) && !(M2C_FIELD((((u32 *) ((u8 *) lbl_80387160 + (var_r31 * 0x31C)))), u8 *, 0x2E5) & 1)) {
            M2C_FIELD(((u32 *) ((u8 *) lbl_80387160 + (var_r31 * 0x31C))), u8 *, 0x2E5) &= 0xFC;
            if (!((&lbl_80321E90[(arg0 * 4) & 0x3FC])[var_r31] & 1)) {
                M2C_FIELD(((u32 *) ((u8 *) lbl_80387160 + (var_r31 * 0x31C))), u8 *, 0x2E5) |= 2;
            }
        }
        var_r31 += 1;
    }
    var_r31 = 0;
    while ((s32) var_r31 < (s16) M2C_FIELD(lbl_8037569C, s16 *, 8)) {
        if (!(M2C_FIELD((((u32 *) ((u8 *) lbl_8037569C + (var_r31 * 0x13C)))), s32 *, 0xF4) & 0x40000000)) {
            M2C_FIELD(((u32 *) ((u8 *) lbl_8037569C + (var_r31 * 0x13C))), s32 *, 0xF4) &= 0x7FFFFFFF;
            if (!(((u32) M2C_FIELD(lbl_8044F47C, u32 *, 0x160) >> var_r31) & 1)) {
                M2C_FIELD(((u32 *) ((u8 *) lbl_8037569C + (var_r31 * 0x13C))), s32 *, 0xF4) |= 0x80000000;
            }
        }
        var_r31 += 1;
    }
}
