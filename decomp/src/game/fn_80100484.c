/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80100484: .text 0x80100484-0x8010071C (664 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F7F1;
extern u8 lbl_80407260[75344];
extern const f32 lbl_804517E4;
extern const f32 lbl_804517E8;
extern f32 lbl_8044EC90[2];
extern const f32 lbl_804517C0;
extern u8 lbl_80239B48[32];
extern f32 lbl_8044EC98[2];

/* fn_80100484: Nov 6, 2026 hand draft (m2c 95%): the robo record pointer (lbl_80407260 + i * 0x4994) is set before the joint pointer, the reverse of m2c's order (only r4 / r5 differed) */

void fn_80100484(void *arg0) {
    void *temp_r31_16;
    void *temp_r30_17;
    u8 temp_r6_9;

    if (lbl_8044F7F1 <= 2U) {
        temp_r6_9 = M2C_FIELD(arg0, s32 *, 0x10);
        temp_r30_17 = lbl_80407260 + (temp_r6_9 * 0x4994);
        temp_r31_16 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
        if ((M2C_FIELD(temp_r30_17, s32 *, 0x498C) & 0x80) || (M2C_FIELD(temp_r30_17, s32 *, 0x498C) & 0x40)) {
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) lbl_804517E4;
            M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_804517E8;
            M2C_FIELD(arg0, s32 *, 0x14) += 1;
            if (M2C_FIELD(arg0, s32 *, 0x14) > 2) {
                M2C_FIELD(arg0, s32 *, 0x14) = 0;
                M2C_FIELD(temp_r31_16, u16 *, 0x10) += 1;
                M2C_FIELD(temp_r31_16, u16 *, 0x30) += 1;
                if ((s32) (M2C_FIELD(temp_r31_16, u16 *, 0x10) - 0x3B) >= 3) {
                    M2C_FIELD(temp_r31_16, u16 *, 0x10) = 0x3BU;
                    M2C_FIELD(temp_r31_16, u16 *, 0x30) = 0x3BU;
                }
            }
            M2C_FIELD(temp_r31_16, s16 *, 8) = 0;
            M2C_FIELD(temp_r31_16, s16 *, 0xA) = 0;
            M2C_FIELD(temp_r31_16, s16 *, 0xC) = 0x12B;
            M2C_FIELD(temp_r31_16, s16 *, 0xE) = 0xC8;
            M2C_FIELD(temp_r31_16, s16 *, 0x48) = 0;
            M2C_FIELD(temp_r31_16, s16 *, 0x4A) = 0;
            M2C_FIELD(temp_r31_16, f32 *, 0x40) = (f32) M2C_FIELD(lbl_8044EC90, f32 *, 0);
            M2C_FIELD(temp_r31_16, f32 *, 0x44) = (f32) lbl_8044EC90[1];
            if ((M2C_FIELD(temp_r30_17, s32 *, 0x498C) & 0x80) && !(M2C_FIELD(temp_r30_17, s32 *, 0x498C) & 0x40)) {
                M2C_FIELD(temp_r31_16, f32 *, 0x40) = (f32) (M2C_FIELD(lbl_8044EC90, f32 *, 0) - lbl_804517C0);
                M2C_FIELD(temp_r31_16, s16 *, 0x4C) = 0x74;
                M2C_FIELD(temp_r31_16, s16 *, 0x4E) = 0x10;
                M2C_FIELD(temp_r31_16, s16 *, 0x50) = 0x55;
            } else {
                M2C_FIELD(temp_r31_16, s16 *, 0x50) = 0x3A;
                M2C_FIELD(temp_r31_16, s16 *, 0x4C) = 0x65;
                M2C_FIELD(temp_r31_16, s16 *, 0x4E) = 0x10;
            }
            if (lbl_8044F7F1 == 1) {
                M2C_FIELD(temp_r31_16, f32 *, 0) = (f32) M2C_FIELD(lbl_80239B48, f32 *, 0x10);
                M2C_FIELD(temp_r31_16, f32 *, 4) = (f32) M2C_FIELD(lbl_80239B48, f32 *, 0x14);
                M2C_FIELD(temp_r31_16, f32 *, 0x20) = (f32) M2C_FIELD(lbl_80239B48, f32 *, 0x18);
                M2C_FIELD(temp_r31_16, f32 *, 0x24) = (f32) M2C_FIELD(lbl_80239B48, f32 *, 0x1C);
                M2C_FIELD(temp_r31_16, s16 *, 0x2C) = 0x12B;
                M2C_FIELD(temp_r31_16, s16 *, 0x2E) = 0xC8;
                M2C_FIELD(temp_r31_16, s16 *, 0x4C) = 0x74;
                M2C_FIELD(temp_r31_16, s16 *, 0x4E) = 0x10;
                M2C_FIELD(temp_r31_16, f32 *, 0x40) = (f32) (M2C_FIELD(lbl_8044EC98, f32 *, 0) - lbl_804517C0);
                M2C_FIELD(temp_r31_16, f32 *, 0x44) = (f32) lbl_8044EC98[1];
            } else {
                M2C_FIELD(temp_r31_16, s16 *, 0x2C) = 0;
                M2C_FIELD(temp_r31_16, s16 *, 0x2E) = 0;
                M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += M2C_FIELD((lbl_80239B48 + ((temp_r6_9 + 2) * 8)), f32 *, 0);
                M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) += M2C_FIELD((lbl_80239B48 + ((temp_r6_9 + 2) * 8)), f32 *, 4);
            }
            M2C_FIELD(arg0, s32 *, 0) |= 1;
            return;
        }
        M2C_FIELD(arg0, s32 *, 0x14) = 0;
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
    }
}
