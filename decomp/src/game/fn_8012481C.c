/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8012481C: .text 0x8012481C-0x80124A78 (604 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_804519FC;
extern const f32 lbl_80451A00;
extern const f32 lbl_804519F0;
extern const f32 lbl_804519F4;

extern u8 *lbl_8044F85C;
extern u8 *lbl_8044F860;

void fn_8012481C(s32 arg0) {
    M2C_FIELD(lbl_8044F860, s32 *, 0) &= 0xFFFFFFFE;
    M2C_FIELD(lbl_8044F85C, s32 *, 0) &= 0xFFFFFFFE;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xE) = 0;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x2E) = 0;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x4E) = 0;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x6E) = 0;
    if (arg0 & 2) {
        M2C_FIELD(lbl_8044F85C, s32 *, 0) |= 1;
        M2C_FIELD(lbl_8044F860, s32 *, 0) |= 1;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x4E) = 0x1B;
    }
    if ((arg0 & 4) || ((arg0 & 8) && (arg0 & 0x10))) {
        M2C_FIELD(lbl_8044F860, s32 *, 0) |= 1;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x10) = 0x22;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xE) = 0x33;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x2E) = 0x11;
    } else if (!(arg0 & 8) && (arg0 & 0x10)) {
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x10) = 0x24;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0xE) = 0x33;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x2E) = 0x11;
    }
    if (arg0 & 0x20) {
        M2C_FIELD(lbl_8044F860, s32 *, 0) |= 1;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x6E) = 0x2D;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x6E) = 0x2D;
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x40) = (f32) lbl_804519FC;
        *M2C_FIELD(lbl_8044F85C, f32 **, 0x5C) = lbl_80451A00;
        return;
    }
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), s16 *, 0x6E) = 0;
    M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F860, void **, 0x5C), void **, 0x1C), void **, 4), f32 *, 0x40) = (f32) lbl_804519F0;
    *M2C_FIELD(lbl_8044F85C, f32 **, 0x5C) = lbl_804519F4;
}
