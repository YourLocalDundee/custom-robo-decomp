/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8006A00C: .text 0x8006A00C-0x8006A1A8 (412 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_WindowStyle
{
  u8 unk_00;
  u8 unk_01[3];
  f32 unk_04;
  f32 unk_08;
  u8 unk_0c;
  u8 unk_0d;
  u8 unk_0e[6];
  f32 unk_14;
  u8 unk_18[4];
  u8 unk_1c;
  u8 unk_1d[7];
  f32 unk_24;
  u8 unk_28;
  u8 unk_29[3];
  u32 unk_2c;
} cr_WindowStyle;
__declspec(section ".data") extern cr_WindowStyle cr_g_WindowStyle;
extern u8 lbl_8044F5D0;
extern u8 lbl_8044F5CC;
extern f32 lbl_8020DD70[4];
extern const f32 lbl_80450AA8;
extern const f32 lbl_80450AAC;
s32 fn_8006A7D8(s32 *arg0, s32 arg1, void *arg2, u8 arg3, u8 arg4, f32 farg0);

/* fn_8006A00C: Nov 13, 2026 hand draft: window text: the style setter is an inline with one parameter per field (each zero its own register), the step message read as ((void **) (steps + i * 8))[1] */

static inline void cr_Window_SetStyle(f32 x, f32 y, f32 sz, f32 sp, u8 c0, u8 c1, u8 c2, u8 c3, u8 b0c, u8 b0d,
                                      u8 b1c, u8 b00, u8 g0, u8 g1) {
    cr_g_WindowStyle.unk_04 = x;
    cr_g_WindowStyle.unk_08 = y;
    cr_g_WindowStyle.unk_14 = sz;
    cr_g_WindowStyle.unk_24 = sp;
    cr_g_WindowStyle.unk_18[0] = c0;
    cr_g_WindowStyle.unk_18[1] = c1;
    cr_g_WindowStyle.unk_18[2] = c2;
    cr_g_WindowStyle.unk_18[3] = c3;
    cr_g_WindowStyle.unk_0c = b0c;
    cr_g_WindowStyle.unk_0d = b0d;
    cr_g_WindowStyle.unk_1c = b1c;
    cr_g_WindowStyle.unk_00 = b00;
    lbl_8044F5D0 = g0;
    lbl_8044F5CC = g1;
}

s32 fn_8006A00C(void *arg0, s32 arg1) {
    void *msg;
    f32 *pos;

    msg = ((void **) (M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), u8 **, 0x1C) + M2C_FIELD(arg0, u8 *, 0x5A) * 8))[1];
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFBFFFF;
    M2C_FIELD(arg0, s32 *, 0) |= 1;
    pos = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4);
    cr_Window_SetStyle(pos[0] + M2C_FIELD(lbl_8020DD70, f32 *, 0), pos[1] + lbl_8020DD70[1], lbl_80450AA8, lbl_80450AAC,
                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    return fn_8006A7D8((s32 *) arg0, arg1, msg, 0U, 0x14U, *M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), f32 **, 4));
}
