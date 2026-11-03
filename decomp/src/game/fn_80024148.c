/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80024148: .text 0x80024148-0x8002419C (84 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_804504BC;
extern const f32 lbl_804504C0;

void fn_80024148(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x70) = 0x14;
    M2C_FIELD(arg0, f32 *, 0x60) = (f32) lbl_804504BC;
    M2C_FIELD(arg0, f32 *, 0x64) = (f32) lbl_804504BC;
    M2C_FIELD(arg0, f32 *, 0x68) = (f32) lbl_804504C0;
    M2C_FIELD(arg0, f32 *, 0x6C) = (f32) lbl_804504C0;
    M2C_FIELD(arg0, s32 *, 0x48) = 0x14;
    M2C_FIELD(arg0, f32 *, 0x38) = (f32) lbl_804504BC;
    M2C_FIELD(arg0, f32 *, 0x3C) = (f32) lbl_804504BC;
    M2C_FIELD(arg0, f32 *, 0x40) = (f32) lbl_804504C0;
    M2C_FIELD(arg0, f32 *, 0x44) = (f32) lbl_804504C0;
}
