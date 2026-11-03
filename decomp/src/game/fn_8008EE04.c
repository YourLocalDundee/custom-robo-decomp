/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008EE04: .text 0x8008EE04-0x8008EE68 (100 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
M2C_UNK *fn_8008EA64(u32 arg0, u8 arg1);
extern const f32 lbl_8045102C;

s32 fn_8008EE04(s32 arg0, void *arg1) {
    s32 *temp_r3_10;

    temp_r3_10 = fn_8008EA64((u32) arg0, 0U);
    M2C_FIELD(temp_r3_10, void **, 0x5C) = arg1;
    M2C_FIELD(temp_r3_10, s32 *, 0) |= 1;
    M2C_FIELD(M2C_FIELD(temp_r3_10, void **, 0x5C), f32 *, 8) = (f32) lbl_8045102C;
    M2C_FIELD(temp_r3_10, s32 *, 8) = 0;
    return (s32) temp_r3_10;
}
