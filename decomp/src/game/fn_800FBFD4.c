/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800FBFD4: .text 0x800FBFD4-0x800FC290 (700 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];
extern f32 lbl_80239F64[6];
extern const f32 lbl_804517B0;
extern const f32 lbl_80451768;
extern const f32 lbl_804517B4;
extern u8 lbl_80239B48[32];

/* fn_800FBFD4: Nov 5, 2026 hand draft (m2c 98%): the player index from arg0 +0x10 kept as a u8 local (clrlwi r30 once, as the original), not an s32 cast at every use */
static inline u8 fn_800FBFD4_search0(s32 temp_r5_6) {
    u8 var_r29_132;

    for (var_r29_132 = 0; var_r29_132 < 4U; var_r29_132 += 1) {
        if ((0x400 << var_r29_132) & M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), s32 *, 0x498C)) {
            return var_r29_132;
        }
    }
    return 0xFF;
}

void fn_800FBFD4(void *arg0) {
    void *temp_r31_39;
    u8 var_r27_146;
    u8 temp_r5_6;

    temp_r5_6 = M2C_FIELD(arg0, s32 *, 0x10);
    if (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), s32 *, 0x487C) & 0x80000) {
        if (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), u8 *, 0x4866) == 0) {
            M2C_FIELD(arg0, s32 *, 0) |= 1;
            temp_r31_39 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) M2C_FIELD(lbl_80239F64, f32 *, 0);
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_80239F64[1];
            M2C_FIELD(temp_r31_39, f32 *, 0) = (f32) lbl_804517B0;
            M2C_FIELD(temp_r31_39, f32 *, 4) = (f32) lbl_80451768;
            M2C_FIELD(temp_r31_39, f32 *, 0x20) = (f32) (lbl_80239F64[2] - lbl_804517B4);
            M2C_FIELD(temp_r31_39, f32 *, 0x24) = (f32) lbl_80239F64[3];
            M2C_FIELD(temp_r31_39, f32 *, 0x40) = (f32) (lbl_80239F64[4] - lbl_804517B4);
            M2C_FIELD(temp_r31_39, f32 *, 0x44) = (f32) lbl_80239F64[5];
            M2C_FIELD(temp_r31_39, f32 *, 0x60) = (f32) lbl_80451768;
            M2C_FIELD(temp_r31_39, f32 *, 0x64) = (f32) lbl_80451768;
            M2C_FIELD(temp_r31_39, f32 *, 0x80) = (f32) lbl_80239F64[2];
            M2C_FIELD(temp_r31_39, f32 *, 0x84) = (f32) lbl_80239F64[3];
            M2C_FIELD(temp_r31_39, f32 *, 0xA0) = (f32) lbl_80239F64[4];
            M2C_FIELD(temp_r31_39, f32 *, 0xA4) = (f32) lbl_80239F64[5];
            M2C_FIELD(temp_r31_39, s16 *, 0x70) = 1;
            M2C_FIELD(temp_r31_39, s16 *, 0x90) = 0;
            M2C_FIELD(temp_r31_39, s16 *, 0xB0) = 2;
            M2C_FIELD(temp_r31_39, s16 *, 0x6C) = 0x50;
            M2C_FIELD(temp_r31_39, s16 *, 0x8C) = 0x43;
            M2C_FIELD(temp_r31_39, s16 *, 0xAC) = 0x3F;
            M2C_FIELD(temp_r31_39, s16 *, 0x6E) = 9;
            M2C_FIELD(temp_r31_39, s16 *, 0x8E) = 9;
            M2C_FIELD(temp_r31_39, s16 *, 0xAE) = 9;
            if (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), s16 *, 0x4862) > 7) {
                M2C_FIELD(temp_r31_39, s16 *, 0x6C) = 0x4F;
                M2C_FIELD(temp_r31_39, s16 *, 0x8C) = 0x4F;
                M2C_FIELD(temp_r31_39, s16 *, 0xAC) = 0x4F;
                M2C_FIELD(temp_r31_39, s16 *, 0x70) = 0x53;
                M2C_FIELD(temp_r31_39, s16 *, 0x90) = 0x53;
                M2C_FIELD(temp_r31_39, s16 *, 0xB0) = 0x53;
                if (M2C_FIELD((lbl_80407260 + (temp_r5_6 * 0x4994)), s16 *, 0x4862) == 0xB) {
                    M2C_FIELD(temp_r31_39, s16 *, 0x8E) = 0;
                    M2C_FIELD(temp_r31_39, s16 *, 0xAE) = 0;
                }
            }
            var_r27_146 = fn_800FBFD4_search0(temp_r5_6);
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += ((f32 (*)[2]) lbl_80239B48)[var_r27_146][0];
            M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) += ((f32 (*)[2]) lbl_80239B48)[var_r27_146][1];
        }
    } else {
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
    }
}
