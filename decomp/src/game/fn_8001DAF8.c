/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001DAF8: .text 0x8001DAF8-0x8001DB5C (100 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_80450408;
extern const f32 lbl_804503D0;

void fn_8001DAF8(void *arg0) {
    M2C_FIELD(arg0, f32 *, 0) = (f32) lbl_80450408;
    M2C_FIELD(arg0, f32 *, 0x10) = (f32) lbl_804503D0;
    M2C_FIELD(arg0, f32 *, 0x20) = (f32) lbl_804503D0;
    M2C_FIELD(arg0, f32 *, 4) = (f32) lbl_804503D0;
    M2C_FIELD(arg0, f32 *, 0x14) = (f32) lbl_80450408;
    M2C_FIELD(arg0, f32 *, 0x24) = (f32) lbl_804503D0;
    M2C_FIELD(arg0, f32 *, 8) = (f32) lbl_804503D0;
    M2C_FIELD(arg0, f32 *, 0x18) = (f32) lbl_804503D0;
    M2C_FIELD(arg0, f32 *, 0x28) = (f32) lbl_80450408;
    M2C_FIELD(arg0, f32 *, 0xC) = (f32) lbl_804503D0;
    M2C_FIELD(arg0, f32 *, 0x1C) = (f32) lbl_804503D0;
    M2C_FIELD(arg0, f32 *, 0x2C) = (f32) lbl_804503D0;
}
