/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8002444C_fn_8002444C: .text 0x8002444C-0x80024578 (300 bytes, 1 functions: fn_8002444C .. fn_8002444C), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8006B144(f32 x, f32 y, const char *text);
extern u8 lbl_801BCB48[24];
extern s16 lbl_8044F5C8;
typedef struct cr_Pad
{
  u32 pressed;
  u32 released;
  u32 held;
  u32 not_held;
  u32 prev_held;
  u32 unk_14;
  u32 unk_18;
  u32 dir;
  u32 unk_20;
  u32 prev_dir;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[0x20];
  u8 unk_50[0x20];
  u8 unk_70[0x22];
  u16 unk_92;
  u16 trigger_peak[2];
  s8 stick_x;
  s8 stick_y;
  s8 substick_x;
  s8 substick_y;
  u8 trigger_l;
  u8 trigger_r;
  u8 analog_a;
  u8 analog_b;
  u8 err;
  u8 trigger_clicked[2];
  u8 unk_a3;
} cr_Pad;
__declspec(section ".data") extern cr_Pad cr_g_Pads[4];
typedef struct cr_GameMode
{
  u32 flags;
  u32 unk_04;
  u32 unk_08;
  u32 unk_0c;
  u32 unk_10;
  s16 unk_14;
  s16 unk_16;
  s16 unk_18;
  u8 unk_1a[2];
  u16 unk_1c[19];
  u8 unk_42;
  u8 unk_43;
  u8 unk_44;
  u8 unk_45;
  u8 unk_46[2];
} cr_GameMode;
__declspec(section ".data") extern cr_GameMode cr_g_GameMode;

/* the file's functions (each is defined below, in address order) */
s32 fn_8002444C(void);

s32 fn_8002444C(void) {
    fn_8006B144(48.0f, 56.0f, (const char *) ((s8 *) lbl_801BCB48));
    fn_8006B144(96.0f, 88.0f, "\x96\xb3\x82\xb5");
    fn_8006B144(192.0f, 88.0f, "\x97\x4c\x82\xe8");
    if (lbl_8044F5C8 == 0) {
        fn_8006B144(72.0f, 88.0f, "\x81\x96");
    } else if (lbl_8044F5C8 == 1) {
        fn_8006B144(160.0f, 88.0f, "\x81\x96");
    }
    if (cr_g_Pads->unk_14 & 0x02000000) {
        lbl_8044F5C8 -= 1;
    } else if (cr_g_Pads->unk_14 & 0x08000000) {
        lbl_8044F5C8 += 1;
    }
    if (lbl_8044F5C8 < 0) {
        lbl_8044F5C8 = 1;
    } else if (lbl_8044F5C8 > 1) {
        lbl_8044F5C8 = 0;
    }
    if (cr_g_Pads->unk_14 & 0x310) {
        cr_g_GameMode.unk_44 = (u8) lbl_8044F5C8;
        return 0;
    }
    return 9;
}
