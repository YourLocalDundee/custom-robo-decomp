/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80130848: .text 0x80130848-0x80130988 (320 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_800BAF3C(void);
__declspec(section ".data") extern u8 cr_g_MotionDataBit15[1280];
__declspec(section ".data") extern u32 *cr_g_MotionData[1280];
void fn_80092108(u8 arg0);
void fn_8008C788(void);
extern f32 lbl_80451BF8;
void *fn_8008C664(s32 arg0, u16 arg1, void *arg2, void *arg3);
void fn_800972D8(s32 arg0);
extern u8 lbl_8041E938[24];

extern u32 *lbl_8044F33C;

void fn_80130848(void) {
    f32 spC;
    f32 sp8;
    u16 *var_r31_24;
    u32 *temp_r30_19;
    u32 var_r29_10;
    u32 var_r28_18;

    fn_800BAF3C();
    var_r29_10 = 0;
    while (var_r29_10 < (u32) *lbl_8044F33C) {
        var_r28_18 = 0U;
        temp_r30_19 = ((u32 *) ((u8 *) lbl_8044F33C + M2C_FIELD(&lbl_8044F33C[var_r29_10], s32 *, 4)));
        var_r31_24 = (unsigned short *) (temp_r30_19 + *temp_r30_19 * 3 + 1);
        while (var_r28_18 < (u32) *temp_r30_19) {
            if ((u16) *var_r31_24 != 0xFFFF) {
                cr_g_MotionDataBit15[*var_r31_24 & 0x7FFF] = 0;
                *(((u32 * *) ((u8 *) cr_g_MotionData + ((*var_r31_24 * 4) & 0x1FFFC)))) = 0;
            }
            var_r28_18 += 1;
            var_r31_24 += 1;
        }
        var_r29_10 += 1;
    }
    fn_80092108(0xDU);
    fn_8008C788();
    sp8 = lbl_80451BF8;
    spC = lbl_80451BF8;
    fn_8008C664(0x64, 0U, &sp8, NULL);
    fn_800972D8(0x2E);
    M2C_FIELD(lbl_8041E938, s32 *, 4) = 0;
    lbl_8041E938[2] = 1;
}
