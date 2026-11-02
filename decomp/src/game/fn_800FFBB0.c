/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800FFBB0: .text 0x800FFBB0-0x800FFD10 (352 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];
extern u16 lbl_80451674;
extern u16 lbl_80451676;
extern u16 lbl_80451678;
extern u16 lbl_8045167A;
extern f32 lbl_8044EC10[2];
extern u8 lbl_80239B48[32];

static inline u8 fn_800FFBB0_search0(u8 var_r30) {
    u8 var_r31;

    for (var_r31 = 0; var_r31 < 4U; var_r31 += 1) {
        if ((0x400 << var_r31) & M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), s32 *, 0x498C)) {
            return var_r31;
        }
    }
    return 0xFF;
}

void fn_800FFBB0(void *arg0) {
    u16 spE;
    u16 spC;
    u16 spA;
    u16 sp8;
    u8 var_r30;
    u8 var_r28;

    var_r30 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    sp8 = lbl_80451674;
    spA = lbl_80451676;
    spC = lbl_80451678;
    spE = lbl_8045167A;
    if (M2C_FIELD((lbl_80407260 + (var_r30 * 0x4994)), s32 *, 0x487C) & 0x100) {
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) M2C_FIELD(lbl_8044EC10, f32 *, 0);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_8044EC10[1];
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u16 *, 0x10) = (&sp8)[var_r30];
        var_r28 = fn_800FFBB0_search0(var_r30);
        M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += ((f32 (*)[2]) lbl_80239B48)[var_r28][0];
        M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) += ((f32 (*)[2]) lbl_80239B48)[var_r28][1];
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
