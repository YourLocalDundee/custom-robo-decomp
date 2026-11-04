/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007D3C8: .text 0x8007D3C8-0x8007D638 (624 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803864F8[22];
extern const f32 lbl_80450D94;
extern const f32 lbl_80450D98;
extern const f32 lbl_80450D9C;

/* fn_8007D3C8: Nov 3, 2026 hand draft (m2c 82%): the alpha steps written out, `x = x - 5` (the original reloads each pointer), case 3 / 2 / 0 end in break, and case 1 writes its float step out too (the pointer chain walked again) */

void fn_8007D3C8(void *arg0) {

    switch (M2C_FIELD(lbl_803864F8, s8 *, 0xC)) { /* irregular */
    case 1:
        M2C_FIELD(lbl_803864F8, u8 *, 0xD) += 1;
        if (M2C_FIELD(lbl_803864F8, s8 *, 0xD) < 0x14) {
            M2C_FIELD(arg0, u8 *, 0x57) = M2C_FIELD(arg0, u8 *, 0x57) - 5;
            M2C_FIELD(lbl_803864F8[4], u8 *, 0x57) = M2C_FIELD(lbl_803864F8[4], u8 *, 0x57) - 5;
            *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4) = *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4) - lbl_80450D94;
            return;
        }
        *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4) = lbl_80450D98;
        M2C_FIELD(lbl_803864F8, u8 *, 0xC) = 2U;
        *M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_803864F8[4], void **, 0x5C), void **, 0x1C), f32 **, 4) = lbl_80450D98;
        return;
    case 3:
        M2C_FIELD(lbl_803864F8, u8 *, 0xD) += 1;
        if (M2C_FIELD(lbl_803864F8, s8 *, 0xD) > 0xA) {
            *M2C_FIELD((M2C_FIELD(M2C_FIELD(lbl_803864F8[2], void **, 0x5C), void **, 0x1C)), f32 **, 4) += lbl_80450D9C;
            *M2C_FIELD((M2C_FIELD(M2C_FIELD(lbl_803864F8[4], void **, 0x5C), void **, 0x1C)), f32 **, 4) -= lbl_80450D9C;
            return;
        }
        break;
    case 2:
    case 0:
        break;
    case 4:
        M2C_FIELD(lbl_803864F8, u8 *, 0xD) += 1;
        if (M2C_FIELD(lbl_803864F8, s8 *, 0xD) > 0xA) {
            *M2C_FIELD((M2C_FIELD(M2C_FIELD(lbl_803864F8[2], void **, 0x5C), void **, 0x1C)), f32 **, 4) -= lbl_80450D9C;
            *M2C_FIELD((M2C_FIELD(M2C_FIELD(lbl_803864F8[4], void **, 0x5C), void **, 0x1C)), f32 **, 4) -= lbl_80450D9C;
            if ((s32) (M2C_FIELD(arg0, u8 *, 0x57) + 5) < 0xFF) {
                M2C_FIELD(arg0, u8 *, 0x57) = M2C_FIELD(arg0, u8 *, 0x57) + 5;
                M2C_FIELD(lbl_803864F8[4], u8 *, 0x57) = M2C_FIELD(lbl_803864F8[4], u8 *, 0x57) + 5;
                return;
            }
            M2C_FIELD(lbl_803864F8, u8 *, 0xC) = 2U;
            return;
        }
        break;
    }
}
