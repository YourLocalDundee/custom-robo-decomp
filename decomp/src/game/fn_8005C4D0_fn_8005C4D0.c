/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8005C4D0_fn_8005C4D0: .text 0x8005C4D0-0x8005C628 (344 bytes, 1 functions: fn_8005C4D0 .. fn_8005C4D0), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 *lbl_8044F590;
extern u16 lbl_803BFE68[16];
void fn_80098104(s32 arg0, u8 arg1);
extern u8 lbl_801F9E08[896];
extern u32 lbl_8044F538;
extern f32 lbl_8045099C;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_0678;
extern u16 lbl_8044F58C;
extern u32 lbl_8044F588;
extern u8 lbl_801F9DDC[44];

/* the file's functions (each is defined below, in address order) */
void fn_8005C4D0(void);

void fn_8005C4D0(void) {
    u16 *temp_r31_10;

    temp_r31_10 = M2C_FIELD(lbl_8044F590, u16 **, 4);
    if ((*lbl_803BFE68 == 0) || (*lbl_803BFE68 == 1)) {
        fn_80098104(M2C_FIELD((lbl_801F9E08 + (lbl_8044F538 * 0x10) + (M2C_FIELD(temp_r31_10, u16 *, 2) * 4)), s32 *, 0), (u8) (lbl_8045099C * (f32) M2C_FIELD(temp_r31_10, u16 *, 4)));
    } else {
        fn_80098104(M2C_FIELD((lbl_801F9E08 + (lbl_8044F538 * 0x10) + (M2C_FIELD(temp_r31_10, u16 *, 2) * 4)), s32 *, 0), (u8) M2C_FIELD(temp_r31_10, u16 *, 4));
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
