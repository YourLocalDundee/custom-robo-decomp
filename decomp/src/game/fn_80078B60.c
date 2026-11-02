/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80078B60: .text 0x80078B60-0x80078CA0 (320 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F608[2];
extern u8 lbl_8037A190[624];
extern u8 lbl_80379FE0[432];
void fn_8017EE20(void *arg0);

void fn_80078B60(void) {
    s32 var_r31;
    s32 var_r30;
    u32 var_r29;

    var_r31 = 0;
    while (var_r31 < 4) {
        var_r30 = 0;
        for (; var_r30 < 0xC; var_r30 += 1) {
            if ((u32) (u32) M2C_FIELD(*lbl_8044F608, u8 **, 0x90) == (u32) M2C_FIELD((lbl_8037A190 + (var_r31 * 0x9C) + (var_r30 * 0xC)), u32 *, 4)) {
                *(lbl_8037A190 + (var_r31 * 0x9C) + (var_r30 * 0xC)) = 0;
                break;
            }
        }
        var_r30 = 0;
        for (; var_r30 < 8; var_r30 += 1) {
            if ((u32) (u32) M2C_FIELD(*lbl_8044F608, u8 **, 0x90) == (u32) M2C_FIELD((lbl_80379FE0 + (var_r31 * 0x6C) + (var_r30 * 0xC)), u32 *, 4)) {
                *(lbl_80379FE0 + (var_r31 * 0x6C) + (var_r30 * 0xC)) = 0;
                break;
            }
        }
        var_r31 += 1;
    }
    fn_8017EE20(M2C_FIELD(*lbl_8044F608, void **, 0x90));
    var_r29 = *lbl_8044F608;
    var_r31 = 0;
    while ((u32) var_r31 < 0xBCU) {
        var_r31 += 1;
        M2C_FIELD(var_r29, s8 *, 0) = 0;
        var_r29 += 1;
    }
}
