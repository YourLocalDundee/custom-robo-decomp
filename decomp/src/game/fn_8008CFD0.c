/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008CFD0: .text 0x8008CFD0-0x8008D110 (320 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
s32 fn_8008EE04(s32 arg0, void *arg1);
extern f32 lbl_8021A754[8];
void fn_8008CA14(void *arg0);

/* fn_8008CFD0: Nov 6, 2026 hand draft (m2c 87%): the float parameters come before the byte, `(f32, f32, f32, f32, u8)`: the registers are the same either way (r3 and f1-f4), but the parameters' stack homes follow the declaration order (m2c always lists the integer parameters first) */

extern s32 lbl_8044F678;

void fn_8008CFD0(f32 farg0, f32 farg1, f32 farg2, f32 farg3, u8 arg0) {

    lbl_8044F678 = 0;
    lbl_8044F678 = fn_8008EE04(0x14, lbl_8021A754);
    M2C_FIELD(lbl_8044F678, s32 *, 0) |= 0x80000;
    M2C_FIELD(lbl_8044F678, s32 *, 0) |= 0x10;
    M2C_FIELD(lbl_8044F678, s32 *, 0) |= 0x100000;
    M2C_FIELD(lbl_8044F678, s32 *, 0) &= 0xFFFFFFFE;
    M2C_FIELD(lbl_8044F678, void (**)(void *), 0x50) = fn_8008CA14;
    M2C_FIELD(lbl_8044F678, s32 *, 0x10) = 0;
    M2C_FIELD(lbl_8044F678, s32 *, 0x14) = (s32) arg0;
    M2C_FIELD(lbl_8044F678, s32 *, 0) |= 1;
    M2C_FIELD(lbl_8021A754, f32 *, 0) = farg0;
    lbl_8021A754[1] = farg1;
    (*(s16 *)((s8 *)((*(void **)((s8 *) ((u32 *) lbl_8021A754)[7] + (4)))) + (0xC))) = (s16) farg2;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F678, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xE) = (s16) farg3;
}
