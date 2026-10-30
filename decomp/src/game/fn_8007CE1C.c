/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007CE1C: .text 0x8007CE1C-0x8007D0D4 (696 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803864E8[4];
void fn_8007CD3C(void *arg0);
void fn_8007CC90(void *arg0);
extern u32 lbl_8044F620;

void fn_8007CE1C(u32 arg0) {
    if ((u32) (0x78 - arg0) == 5U) {
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), s32 *, 0) |= 1;
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), void (**)(void *), 0x50) = fn_8007CD3C;
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), s32 *, 0x10) = 0x1E;
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), s32 *, 0x14) = 0x14;
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), s32 *, 0x18) = 0x64;
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), s32 *, 0x1C) = 0x14;
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), s32 *, 0x20) = 0x14;
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), s32 *, 0x24) = 0;
    }
    if ((u32) (0x78 - arg0) == 0xFU) {
        M2C_FIELD(lbl_803864E8[2], s32 *, 0) |= 1;
        M2C_FIELD(lbl_803864E8[2], void (**)(void *), 0x50) = fn_8007CD3C;
        M2C_FIELD(lbl_803864E8[2], s32 *, 0x10) = 0;
        M2C_FIELD(lbl_803864E8[2], s32 *, 0x14) = 0;
        M2C_FIELD(lbl_803864E8[2], s32 *, 0x18) = 0xFF;
        M2C_FIELD(lbl_803864E8[2], s32 *, 0x1C) = 0x14;
        M2C_FIELD(lbl_803864E8[2], s32 *, 0x20) = 0x14;
        M2C_FIELD(lbl_803864E8[2], s32 *, 0x24) = 0;
    }
    if (arg0 == 0x14) {
        M2C_FIELD(lbl_803864E8[2], s32 *, 0) |= 1;
        M2C_FIELD(lbl_803864E8[2], void (**)(void *), 0x50) = fn_8007CC90;
        M2C_FIELD(lbl_803864E8[2], s32 *, 0x10) = 0;
        M2C_FIELD(lbl_803864E8[2], s32 *, 0x14) = 0;
        M2C_FIELD(lbl_803864E8[2], s32 *, 0x18) = 0xFF;
        M2C_FIELD(lbl_803864E8[2], s32 *, 0x1C) = 0xA;
    }
    if (arg0 == 0xA) {
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), s32 *, 0) |= 1;
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), void (**)(void *), 0x50) = fn_8007CC90;
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), s32 *, 0x10) = 0x3C;
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), s32 *, 0x14) = 0xA;
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), s32 *, 0x18) = 0x64;
        M2C_FIELD(M2C_FIELD(lbl_803864E8, u32 *, 0), s32 *, 0x1C) = 0xA;
    }
    if (arg0 == 5) {
        lbl_8044F620 |= 0x2000;
    }
}
