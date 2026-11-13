/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800BD658: .text 0x800BD658-0x800BD720 (200 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8021E338[188];
extern u16 lbl_803BFE68[16];
extern u8 lbl_80222268[188];
extern u32 lbl_803B92CC[20];

/* fn_800BD658: Nov 6, 2026 hand draft (m2c 69%): the area records filled in order (pointer, then flag = 1, then i += 1, then the next 0x24-byte area): m2c read the scheduled increment as coming first and indexed the flag with i + 1; the table count read without a temp (volatile-temp sweep) */

void fn_800BD658(void) {
    u32 var_r31_8;
    s32 var_r30_13;

    var_r31_8 = 0;
    var_r30_13 = M2C_FIELD((lbl_8021E338 + (*lbl_803BFE68 * 4)), u32 *, 0);
    while (var_r31_8 < (u32) M2C_FIELD((lbl_80222268 + (*lbl_803BFE68 * 4)), u32 *, 0)) {
        *((s32 *) ((u32 *) ((u8 *) lbl_803B92CC + (var_r31_8 * 8)))) = var_r30_13;
        M2C_FIELD((((u32 *) ((u8 *) lbl_803B92CC + (var_r31_8 * 8)))), s32 *, 4) = 1;
        var_r31_8 += 1;
        var_r30_13 += 0x24;
    }
    while ((s32) var_r31_8 < 0xA) {
        M2C_FIELD(((u32 *) ((u8 *) lbl_803B92CC + (var_r31_8 * 8))), s32 *, 4) = 0;
        var_r31_8 += 1;
    }
}
