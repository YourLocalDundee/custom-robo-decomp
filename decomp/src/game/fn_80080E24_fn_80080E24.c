/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80080E24_fn_80080E24: .text 0x80080E24-0x80080F20 (252 bytes, 1 functions: fn_80080E24 .. fn_80080E24), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern const f32 lbl_80450E8C;
extern const f32 lbl_80450E7C;
extern const f32 lbl_80450E84;
extern const f32 lbl_80450E88;
extern const f32 lbl_80450E64;
extern u8 lbl_8044EAE0[8];

/* the file's functions (each is defined below, in address order) */
inline static void fn_80080E24_advance(f32 a, f32 *p);
inline static int fn_80080E24_below(f32 a, f32 *p, f32 lim);
void fn_80080E24(void *arg0);

/* fn_80080E24: Nov 4, 2026 hand draft (m2c 78%, hand 88%, permuter 40 showed the inline compare): the clamped step (*p + a * K < lim ? *p += a * K : *p = lim) through two static inlines, the compare and the add; written inline, the scheduler loads the operands in another order */
static inline void fn_80080E24_advance(f32 a, f32 *p) { *p += a * lbl_80450E8C; }
static inline int fn_80080E24_below(f32 a, f32 *p, f32 lim) { return ((a * lbl_80450E8C) + *p) < lim; }

void fn_80080E24(void *arg0) {
    f32 var_f31;
    f32 *var_r31;

    var_f31 = lbl_80450E7C;
    var_r31 = M2C_FIELD(arg0, f32 **, 0x5C);
    if (fn_80080E24_below((lbl_80450E84 + (lbl_80450E88 - var_f31)), var_r31, lbl_80450E88)) {
        fn_80080E24_advance((lbl_80450E84 + (lbl_80450E88 - var_f31)), var_r31);
    } else {
        *var_r31 = lbl_80450E88;
    }
    if ((f32) (M2C_FIELD(arg0, u8 *, 0x57) + ((s32) (0xFF - M2C_FIELD(arg0, s32 *, 0x14)) / 8)) < lbl_80450E64) {
        M2C_FIELD(arg0, u8 *, 0x57) += ((s32) (0xFF - M2C_FIELD(arg0, s32 *, 0x14)) / 8);
        return;
    }
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
    lbl_8044EAE0[M2C_FIELD(arg0, s32 *, 0x10) - 2] = 0;
}
