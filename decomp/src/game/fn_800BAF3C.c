/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800BAF3C: .text 0x800BAF3C-0x800BB034 (248 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803BE358[289];
__declspec(section ".data") extern u8 cr_g_MotionDataBit15[1280];
__declspec(section ".data") extern u32 *cr_g_MotionData[1280];

extern u32 *lbl_8044F338;

void fn_800BAF3C(void) {
    u16 *var_r31_26;
    u32 *temp_r30_21;
    u32 var_r29_12;
    u32 var_r28_20;

    if ((s32) lbl_803BE358[0x115] != -1) {
        var_r29_12 = 0;
        while (var_r29_12 < (u32) *lbl_8044F338) {
            var_r28_20 = 0U;
            temp_r30_21 = ((u32 *) ((u8 *) lbl_8044F338 + M2C_FIELD(&lbl_8044F338[var_r29_12], s32 *, 4)));
            var_r31_26 = (unsigned short *) (temp_r30_21 + *temp_r30_21 * 3 + 1);
            while (var_r28_20 < (u32) *temp_r30_21) {
                if ((u16) *var_r31_26 != 0xFFFF) {
                    cr_g_MotionDataBit15[*var_r31_26 & 0x7FFF] = 0;
                    *(((u32 * *) ((u8 *) cr_g_MotionData + ((*var_r31_26 * 4) & 0x1FFFC)))) = 0;
                }
                var_r28_20 += 1;
                var_r31_26 += 1;
            }
            var_r29_12 += 1;
        }
    }
    lbl_803BE358[0x115] = -1U;
}
