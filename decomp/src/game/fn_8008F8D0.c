/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008F8D0: .text 0x8008F8D0-0x8008F998 (200 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_80396484[3];
extern u8 lbl_80396478[12];

/* fn_8008F8D0: Nov 13, 2026 hand draft: the chunk size rounded down as (size >> 2) << 2 (clrrwi from its own register, not & ~3); three dead uses of the header copy */

extern u8 *lbl_8044F40C;

void fn_8008F8D0(void) {
    void *sp8;
    u8 *var_r31;
    void *var_r30;
    u8 *var_r29;
    u32 var_r28;

    (void) var_r30; (void) var_r30; (void) var_r30; 
    var_r28 = 0U;
    var_r29 = M2C_FIELD(lbl_8044F40C, void **, 8);
    var_r30 = var_r29;
    var_r31 = var_r29 + 0x20;
    sp8 = var_r29 + M2C_FIELD(var_r29, s32 *, 8);
    while (var_r28 < (u32) M2C_FIELD(var_r30, u32 *, 0xC)) {
        if ((u32) (M2C_FIELD(var_r31, s32 *, 0) + 0xB6B20000) == 0x4631U) {
            lbl_80396484[M2C_FIELD(lbl_8044F40C, s32 *, 0x10)] = (u32) var_r31;
        } else if ((u32) (M2C_FIELD(var_r31, s32 *, 0) + 0xBBBF0000) == 0x5431U) {
            M2C_FIELD((lbl_80396478 + (M2C_FIELD(lbl_8044F40C, s32 *, 0x10) * 4)), u32 *, 0) = (unsigned long) var_r31;
        }
        var_r28 += 1;
        var_r31 += (M2C_FIELD(var_r31, u32 *, 4) >> 2) << 2;
    }
}
