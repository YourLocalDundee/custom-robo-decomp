/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8006B3D0: .text 0x8006B3D0-0x8006B46C (156 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 *lbl_8044F5C0;
s32 fn_8008EDD8(s32 arg0);
extern u8 lbl_8020DC2C[256];
extern const f32 lbl_80450ABC;
void fn_8006B140(void);
extern u32 lbl_8044F5C4;

void fn_8006B3D0(void) {
    lbl_8044F5C0 = (unsigned char *) (fn_8008EDD8(1));
    M2C_FIELD(lbl_8044F5C0, u8 **, 0x5C) = lbl_8020DC2C;
    M2C_FIELD(M2C_FIELD(lbl_8044F5C0, u8 **, 0x5C), f32 *, 8) = (f32) lbl_80450ABC;
    M2C_FIELD(lbl_8044F5C0, s32 *, 0) |= 1;
    M2C_FIELD(lbl_8044F5C0, s32 *, 0) |= 0x40;
    M2C_FIELD(lbl_8044F5C0, s32 *, 0) |= 0x20;
    M2C_FIELD(lbl_8044F5C0, void (**)(), 0x50) = fn_8006B140;
    lbl_8044F5C4 = 0;
}
