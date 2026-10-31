/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8005C628: .text 0x8005C628-0x8005C720 (248 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef Vec Point3d;
extern u8 *lbl_8044F590;
void fn_80097CA8(s32 arg0, Point3d *arg1);
void fn_80098290(s32 arg0);
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_0678;
extern u16 lbl_8044F58C;
extern u32 lbl_8044F588;
extern u8 lbl_801F9DDC[44];

extern u8 *lbl_8044F348;
extern Point3d *lbl_8044F53C;

void fn_8005C628(void) {
    u16 *temp_r31_9;
    u32 temp_r30_14;

    temp_r31_9 = M2C_FIELD(lbl_8044F590, u16 **, 4);
    temp_r30_14 = ((u32) M2C_FIELD(lbl_8044F348, u32 *, 0x33498) >> 8U) & 0xFF;
    if (M2C_FIELD(temp_r31_9, u16 *, 8) != 0xFFFF) {
        fn_80097CA8(M2C_FIELD((M2C_FIELD(temp_r31_9, s32 *, 4) + (temp_r30_14 * 4)), s32 *, 0), lbl_8044F53C);
    } else {
        fn_80098290(M2C_FIELD((M2C_FIELD(temp_r31_9, s32 *, 4) + (temp_r30_14 * 4)), s32 *, 0));
    }
    lbl_8044F590 += 8;
    if (M2C_FIELD(lbl_8044F590, s32 *, 0) == 0) {
        M2C_FIELD(cr_g_CurrentChara_0678, u8 **, 0x4C) = (u8 *) lbl_8044F590;
        return;
    }
    lbl_8044F58C = *M2C_FIELD(lbl_8044F590, u16 **, 4);
    if (lbl_8044F588 < (u16) lbl_8044F58C) {
        M2C_FIELD(cr_g_CurrentChara_0678, u8 **, 0x4C) = (u8 *) lbl_8044F590;
        return;
    }
    (*(M2C_UNK (**)()) (lbl_801F9DDC + (M2C_FIELD(lbl_8044F590, s32 *, 0) * 4)))();
}
