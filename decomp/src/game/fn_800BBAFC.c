/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800BBAFC: .text 0x800BBAFC-0x800BBD04 (520 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80222A88[12];
extern u8 lbl_8044F756;
void cr_Bits_Set(u32 *set, u32 bit);
typedef struct cr_UnlockBits
{
  u32 parts[5][8];
  u32 stages[2];
} cr_UnlockBits;
__declspec(section ".data") extern cr_UnlockBits cr_g_UnlockBits;
void cr_Bits_Clear(u32 *set, u32 bit);
void fn_800F6A30(void);
void fn_800F6A44(u16 arg0);

extern u8 *lbl_8044F728;

void fn_800BBAFC(void) {
    s32 var_r31_18;

    (*(M2C_UNK (**)()) (lbl_80222A88 + (M2C_FIELD(lbl_8044F728, u8 *, 0x4A0) * 4)))();
    if (lbl_8044F756 != 4) {
        var_r31_18 = 0;
        while (var_r31_18 < 0x40) {
            cr_Bits_Set(cr_g_UnlockBits.parts[0], (u32) var_r31_18);
            cr_Bits_Set(cr_g_UnlockBits.parts[1], (u32) var_r31_18);
            cr_Bits_Set(cr_g_UnlockBits.parts[2], (u32) var_r31_18);
            cr_Bits_Set(cr_g_UnlockBits.parts[3], (u32) var_r31_18);
            cr_Bits_Set(cr_g_UnlockBits.parts[4], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[0][2], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[1][2], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[2][2], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[3][2], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[4][2], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[0][4], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[1][4], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[2][4], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[3][4], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[4][4], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[0][6], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[1][6], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[2][6], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[3][6], (u32) var_r31_18);
            cr_Bits_Clear(&cr_g_UnlockBits.parts[4][6], (u32) var_r31_18);
            var_r31_18 += 1;
        }
        lbl_8044F756 = 4;
        fn_800F6A30();
        fn_800F6A44(2U);
    }
}
