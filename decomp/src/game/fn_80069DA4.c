/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80069DA4: .text 0x80069DA4-0x8006A00C (616 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
void *fn_8008E9FC(void *arg0);
extern const f32 lbl_80450AA8;
extern const f32 lbl_80450AAC;
s32 fn_8006A7D8(s32 *arg0, s32 arg1, void *arg2, u8 arg3, u8 arg4, f32 farg0);

/* fn_80069DA4: Nov 13, 2026 hand draft: window text at a position: the style-setter inline, size scaled; the object a void * parameter cast at each use */

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

void fn_80069DA4(void *arg0, s32 arg1, void *arg2, u8 arg3, f32 farg0) {
    void *msg;

    msg = fn_8008E9FC(arg0);
    M2C_FIELD(msg, u16 *, 0x12) |= 0x8000;
    if (*(s32 *) arg0 & 0x40000) {
        cr_Window_SetStyle(M2C_FIELD(arg2, f32 *, 0), M2C_FIELD(arg2, f32 *, 4), lbl_80450AA8 * farg0, lbl_80450AAC,
                           0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0, 0);
    } else {
        cr_Window_SetStyle(M2C_FIELD(arg2, f32 *, 0), M2C_FIELD(arg2, f32 *, 4), lbl_80450AA8 * farg0, lbl_80450AAC,
                           0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    fn_8006A7D8((s32 *) arg0, arg1, msg, arg3, 0U, M2C_FIELD(arg2, f32 *, 0));
}
