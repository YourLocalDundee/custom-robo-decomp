/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800FAC00: .text 0x800FAC00-0x800FADE0 (480 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];
extern u8 lbl_804026E8[10];

void fn_800FAC00(void *arg0) {
    u8 var_r30;
    u8 var_r31;

    var_r31 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    var_r30 = M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u8 *, 0x4860);
    if (M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), u8 *, 0x4855) != 3) {
        if (M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s32 *, 0x487C) & 0x04000000) {
            if (M2C_FIELD((lbl_804026E8 + (M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s8 *, 0x4850) * 2)), u16 *, 0) == 0xFFFF) {
                M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s8 *, 0x4850) = 0;
                M2C_FIELD((lbl_804026E8 + (M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s8 *, 0x4850) * 2)), s16 *, 0) = 0x30;
            }
            M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4), u16 *, 0x10) = M2C_FIELD((lbl_804026E8 + (M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s8 *, 0x4850) * 2)), u16 *, 0);
            M2C_FIELD(arg0, s32 *, 0) |= 1;
            if ((var_r30 == 0) && (M2C_FIELD((lbl_80407260 + (var_r31 * 0x4994)), s8 *, 0x46C2) != -1)) {
                M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFDF;
                M2C_FIELD(arg0, s32 *, 0) &= 0xFFFBFFFF;
                return;
            }
            M2C_FIELD(arg0, s32 *, 0) |= 0x20;
            M2C_FIELD(arg0, s32 *, 0) |= 0x40000;
            M2C_FIELD(arg0, u8 *, 0x54) = 0x80;
            M2C_FIELD(arg0, u8 *, 0x55) = 0x80;
            M2C_FIELD(arg0, u8 *, 0x56) = 0x80;
            return;
        }
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
