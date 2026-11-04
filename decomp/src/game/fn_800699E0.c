/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800699E0: .text 0x800699E0-0x80069AD8 (248 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern f32 lbl_8020DD70[4];
extern const f32 lbl_80450AA8;
extern const f32 lbl_80450AAC;
extern u8 lbl_8044F5D0;
extern u8 lbl_8044F5CC;

/* fn_800699E0: Nov 4, 2026 hand draft (permuter queue 3, score 0): the second sum (var_f30, offset 4) computed before the first */
void fn_800699E0(void *arg0, void *arg1, s32 arg2, u8 arg3) {
    f32 var_f31;
    f32 var_f30;

    var_f30 = M2C_FIELD(arg1, f32 *, 4) + M2C_FIELD((((f32 *) ((u8 *) lbl_8020DD70 + ((arg2 * 8) & 0x7FFF8)))), f32 *, 4);
    var_f31 = M2C_FIELD(arg1, f32 *, 0) + *(((f32 *) ((u8 *) lbl_8020DD70 + ((arg2 * 8) & 0x7FFF8))));
    M2C_FIELD(arg0, f32 *, 4) = var_f31;
    M2C_FIELD(arg0, f32 *, 8) = var_f30;
    M2C_FIELD(arg0, f32 *, 0x14) = (f32) lbl_80450AA8;
    M2C_FIELD(arg0, f32 *, 0x24) = (f32) lbl_80450AAC;
    if (arg3 != 0) {
        M2C_FIELD(arg0, u8 *, 0x18) = 0xFF;
        M2C_FIELD(arg0, u8 *, 0x19) = 0xFF;
        M2C_FIELD(arg0, u8 *, 0x1A) = 0xFF;
        M2C_FIELD(arg0, u8 *, 0x1B) = 0xFF;
    } else {
        M2C_FIELD(arg0, s8 *, 0x18) = 0;
        M2C_FIELD(arg0, s8 *, 0x19) = 0;
        M2C_FIELD(arg0, s8 *, 0x1A) = 0;
        M2C_FIELD(arg0, s8 *, 0x1B) = 0;
    }
    M2C_FIELD(arg0, s8 *, 0xC) = 0;
    M2C_FIELD(arg0, s8 *, 0xD) = 0;
    M2C_FIELD(arg0, s8 *, 0x1C) = 0;
    M2C_FIELD(arg0, s8 *, 0) = 0;
    lbl_8044F5D0 = 0;
    lbl_8044F5CC = 0;
}
