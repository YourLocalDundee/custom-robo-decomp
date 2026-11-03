/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008CE9C: .text 0x8008CE9C-0x8008CFD0 (308 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern f32 lbl_80388284[2063];
extern const f32 lbl_80451010;
extern u32 lbl_8044F67C[2];

extern u8 *lbl_8044F678;
extern u8 *lbl_8044F684;

void fn_8008CE9C(void) {
    if ((*(u32 *) &lbl_80388284[0x80C]) != 0U) {
        *(*(f32 **)((s8 *) ((u32 *) lbl_80388284)[0x80C] + (0x5C))) += (lbl_80451010 - lbl_80388284[0x80D]);
    }
    if ((void *) lbl_8044F684 != NULL) {
        *M2C_FIELD(lbl_8044F684, f32 **, 0x5C) += (lbl_80451010 - lbl_80388284[0x80D]);
    }
    if (M2C_FIELD(lbl_8044F67C, u32 *, 0) != 0) {
        *M2C_FIELD(M2C_FIELD(lbl_8044F67C, u32 *, 0), f32 **, 0x5C) += (lbl_80451010 - lbl_80388284[0x80D]);
    }
    if (lbl_8044F67C[1] != 0) {
        *M2C_FIELD(lbl_8044F67C[1], f32 **, 0x5C) += (lbl_80451010 - lbl_80388284[0x80D]);
    }
    if ((void *) lbl_8044F678 != NULL) {
        *M2C_FIELD(lbl_8044F678, f32 **, 0x5C) += (lbl_80451010 - lbl_80388284[0x80D]);
    }
}
