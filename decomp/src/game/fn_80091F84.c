/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80091F84: .text 0x80091F84-0x800920D0 (332 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern f32 lbl_8038B420[4];
extern const f32 lbl_80451008;
extern const f32 lbl_80451058;
extern u8 lbl_8038B430[24576];
void fn_80023394(s8 *arg0, u32 arg1);
extern u8 lbl_8039649C[29700];
extern u8 *lbl_8044F5C0;
extern u8 *lbl_8044F2F8;
extern u32 lbl_8044F690;
extern u32 lbl_8044F68C;
void fn_80090298(s8 arg0, u8 arg1, u8 arg2, u32 arg3);
void fn_8006B3D0(void);
void fn_80093AF0(void);
extern u32 lbl_8044F674;

/* fn_80091F84: Nov 6, 2026 hand draft (m2c 87%): clears the 256 x 8-word table at lbl_8044F694 and the 256 records of lbl_8039649C, then starts state 0xD through fn_80090298; the table cleared as a [][8] array (index first, then the base), and the compare after the last store with no branch is an assert without its message call, `(lbl_8044F690 < 0x154000) ? (void) 0 : (void) 0` (it comes after the store in the source although the compiler schedules it before) */

extern u8 *lbl_8044F694;

void fn_80091F84(u8 arg0, u8 arg1, u32 arg2) {
    s32 var_r31_33;
    s32 var_r30_25;
    s32 var_r29_50;

    var_r30_25 = 0;
    M2C_FIELD(lbl_8038B420, f32 *, 0) = lbl_80451008;
    lbl_8038B420[1] = lbl_80451008;
    lbl_8038B420[2] = lbl_80451008;
    lbl_8038B420[3] = lbl_80451058;
    lbl_8044F694 = &lbl_8038B430[(arg1 << 0xD) & 0x1FE000];
    while (var_r30_25 < 0x100) {
        var_r31_33 = 0;
        while (var_r31_33 < 8) {
            ((u32 (*)[8]) lbl_8044F694)[var_r30_25][var_r31_33] = 0;
            var_r31_33 += 1;
        }
        var_r30_25 += 1;
    }
    var_r29_50 = 0;
    while (var_r29_50 < 0x100) {
        fn_80023394((signed char *) (lbl_8039649C + (var_r29_50 * 0x74)), 0x74U);
        var_r29_50 += 1;
    }
    lbl_8044F5C0 = NULL;
    if ((u32) lbl_8044F2F8 == arg2) {
        lbl_8044F690 = lbl_8044F68C;
    } else {
        lbl_8044F690 = 0;
    }
    fn_80090298(0xD, arg0, arg1, arg2);
    fn_8006B3D0();
    fn_80093AF0();
    lbl_8044F674 = 0;
    ((u32) lbl_8044F690 < 0x154000) ? (void) 0 : (void) 0;
}
