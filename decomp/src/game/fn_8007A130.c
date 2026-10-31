/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007A130: .text 0x8007A130-0x8007A1D0 (160 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80386378[208];

void fn_8007A130(void *arg0) {
    u8 temp_r4_11;

    M2C_FIELD(arg0, s32 *, 8) = (s32) *M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), u16 **, 0x1C);
    temp_r4_11 = M2C_FIELD(arg0, s32 *, 0x64) - 6;
    if (M2C_FIELD((lbl_80386378 + (temp_r4_11 * 0x34)), u16 *, 0) & 0x20) {
        M2C_FIELD(arg0, s32 *, 0) |= 0x4000;
        M2C_FIELD((lbl_80386378 + (temp_r4_11 * 0x34)), u8 *, 2) |= 0x20;
        *((u16 *) (lbl_80386378 + (temp_r4_11 * 0x34))) &= 0xFFFFFFDF;
        M2C_FIELD(arg0, s8 *, 0x58) = 1;
    }
}
