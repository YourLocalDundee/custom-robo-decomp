/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800AA17C: .text 0x800AA17C-0x800AA2E4 (360 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_801C6C44[10472];
extern u8 lbl_801CA560[41728];
extern u8 lbl_801D4860[19760];
extern u8 lbl_801D9590[21724];
extern u8 lbl_801C952C[4148];
extern u32 lbl_80321D40[18];

extern u8 *lbl_8044F794;

u8 fn_800AA17C(void) {
    u8 var_r31;

    var_r31 = M2C_FIELD((lbl_801C6C44 + (M2C_FIELD(lbl_8044F794, u8 *, 0xBD) * 0x134)), u8 *, 0x45);
    var_r31 |= M2C_FIELD((lbl_801CA560 + (M2C_FIELD(lbl_8044F794, u8 *, 0xC1) * 0x28C)), u8 *, 0xA0);
    var_r31 |= M2C_FIELD((lbl_801D4860 + (M2C_FIELD(lbl_8044F794, u8 *, 0xC3) * 0x17C)), u8 *, 0xA0);
    var_r31 |= M2C_FIELD((lbl_801D9590 + (M2C_FIELD(lbl_8044F794, u8 *, 0xC5) * 0x174)), u8 *, 0xE0);
    var_r31 |= M2C_FIELD((lbl_801C952C + (M2C_FIELD(lbl_8044F794, u8 *, 0xC7) * 0x8F)), u8 *, 0x80);
    if ((s32) lbl_80321D40[4] == 2) {
        var_r31 |= M2C_FIELD((lbl_801C6C44 + (M2C_FIELD(lbl_8044F794, u8 *, 0xBE) * 0x134)), u8 *, 0x45);
        var_r31 |= M2C_FIELD((lbl_801CA560 + (M2C_FIELD(lbl_8044F794, u8 *, 0xC2) * 0x28C)), u8 *, 0xA0);
        var_r31 |= M2C_FIELD((lbl_801D4860 + (M2C_FIELD(lbl_8044F794, u8 *, 0xC4) * 0x17C)), u8 *, 0xA0);
        var_r31 |= M2C_FIELD((lbl_801D9590 + (M2C_FIELD(lbl_8044F794, u8 *, 0xC6) * 0x174)), u8 *, 0xE0);
        var_r31 |= M2C_FIELD((lbl_801C952C + (M2C_FIELD(lbl_8044F794, u8 *, 0xC8) * 0x8F)), u8 *, 0x80);
    }
    return var_r31;
}
