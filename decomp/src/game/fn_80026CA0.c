/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80026CA0: .text 0x80026CA0-0x80026D04 (100 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8032329C[240];
extern u8 lbl_8044F461;

void fn_80026CA0(void *arg0) {
    M2C_FIELD((lbl_8032329C + (lbl_8044F461 * 0xC)), f32 *, 0) = M2C_FIELD(arg0, f32 *, 0);
    M2C_FIELD((lbl_8032329C + (lbl_8044F461 * 0xC)), f32 *, 4) = (f32) M2C_FIELD(arg0, f32 *, 4);
    M2C_FIELD((lbl_8032329C + (lbl_8044F461 * 0xC)), f32 *, 8) = (f32) M2C_FIELD(arg0, f32 *, 8);
    lbl_8044F461 += 1;
}
