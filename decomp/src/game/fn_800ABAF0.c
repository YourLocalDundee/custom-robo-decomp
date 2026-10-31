/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800ABAF0: .text 0x800ABAF0-0x800ABC28 (312 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8031FED8[5];
void fn_800AFE3C(s16 arg0);
extern u8 lbl_802249B0[1320];
extern u32 lbl_803BE358[289];
extern u32 lbl_80229B50[3];

extern u8 *lbl_8044F728;

void fn_800ABAF0(void) {
    void *var_r31;

    if ((M2C_FIELD(lbl_8044F728, u8 *, 3) != 9) && (M2C_FIELD(lbl_8031FED8, s32 *, 0x14) & 0x100)) {
        fn_800AFE3C(9);
        var_r31 = lbl_802249B0 + (lbl_803BE358[0x114] * 0x2C);
        if (M2C_FIELD(var_r31, u32 *, 0x20) == 0U) {
            if ((void *) lbl_8044F728 != ((void *) 0)) {
                M2C_FIELD(lbl_8044F728, u8 *, 0x5F) |= 1;
            }
            if ((void *) lbl_8044F728 != ((void *) 0)) {
                M2C_FIELD(lbl_8044F728, u8 *, 0x60) |= 1;
            }
            if ((void *) lbl_8044F728 != ((void *) 0)) {
                M2C_FIELD(lbl_8044F728, u8 *, 0x61) |= 1;
            }
            if ((void *) lbl_8044F728 != ((void *) 0)) {
                M2C_FIELD(lbl_8044F728, u8 *, 0x62) |= 1;
            }
            M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80229B50[M2C_FIELD(lbl_8044F728, u8 *, 0x18)], void **, 0x1C), void **, 4), s16 *, 0xE) = 0x80;
        }
        M2C_FIELD(lbl_8044F728, u16 *, 0) |= 4;
    }
}
