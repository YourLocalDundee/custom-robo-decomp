/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007D0D4: .text 0x8007D0D4-0x8007D17C (168 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803864F8[22];
void fn_80098290(s32 arg0);
extern const f32 lbl_80450D90;

void fn_8007D0D4(u8 arg0) {
    void *temp_r31_20;

    if (M2C_FIELD(lbl_803864F8[6], s32 *, 0) & 1) {
        temp_r31_20 = M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_803864F8[6], void **, 0x5C), void **, 0x1C), void **, 4);
        if (M2C_FIELD(temp_r31_20, u32 *, 0x1C) != arg0) {
            fn_80098290(0);
        }
        M2C_FIELD(temp_r31_20, u32 *, 0x1C) = (u32) arg0;
        M2C_FIELD(temp_r31_20, f32 *, 0x14) = (f32) lbl_80450D90;
        M2C_FIELD(temp_r31_20, s16 *, 0xC) = 0x40;
        M2C_FIELD(temp_r31_20, s16 *, 0xE) = 0x40;
        M2C_FIELD(temp_r31_20, s16 *, 8) = (s16) ((arg0 << 6) & 0x3FC0);
        M2C_FIELD(temp_r31_20, s16 *, 0xA) = 0;
    }
}
