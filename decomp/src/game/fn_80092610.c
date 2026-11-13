/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80092610: .text 0x80092610-0x80092764 (340 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F504;
extern u8 lbl_8023BA70[20];
extern u8 lbl_80407260[75344];
void fn_800812A4(void);
void fn_800935F8(void);

/* fn_80092610: Nov 13, 2026 hand draft: per-robot slot handlers: ((void (**)(u8 *, u8)) tbl)[slot type](robot, slot) */

s32 fn_80092610(void) {
    u8 var_r31;
    u8 var_r30;
    u32 var_r29;
    s32 var_r28;

    var_r31 = 0;
    var_r29 = 0U;
    var_r28 = 3;
    while (var_r31 < (u32) lbl_8044F504) {
        var_r30 = 0;
        while (var_r30 < 5U) {
            ((void (**)(u8 *, u8)) lbl_8023BA70)[M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994) + (var_r30 * 0xC)), u8 *, 0x47F0)](lbl_80407260 + (var_r31 * 0x4994), var_r30);
            var_r30 += 1;
        }
        var_r31 += 1;
    }
    var_r31 = 0;
    while (var_r31 < (u32) lbl_8044F504) {
        var_r30 = 0;
        while (var_r30 < 5U) {
            if (M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994) + (var_r30 * 0xC)), u8 *, 0x47F0) == 4) {
                var_r29 += 1;
            }
            var_r30 += 1;
        }
        var_r31 += 1;
    }
    if (var_r29 == (u32) (lbl_8044F504 * 5)) {
        fn_800812A4();
        fn_800935F8();
        var_r28 = 4;
    }
    return var_r28;
}
