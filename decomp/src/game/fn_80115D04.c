/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80115D04: .text 0x80115D04-0x80115EFC (504 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8041C0C0[840];
extern u8 lbl_8041C408[1056];
extern u8 lbl_80240904[112];
extern const f32 lbl_80451928;
extern const f32 lbl_8045192C;
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
__declspec(section ".sdata") extern u8 lbl_8044F885[11];
void fn_80098290(s32 arg0);

/* fn_80115D04: Nov 14, 2026 hand draft: fn_80115918's twin; the B button goes to page 9 (49%) */

typedef struct {
    f32 x;
    f32 y;
    f32 unk8;
    f32 w;
    f32 h;
    u8 pad[8];
} RectD04;

typedef struct {
    f32 x;
    f32 y;
    f32 w;
    f32 h;
} SrcD04;

typedef struct {
    u8 pad[0x44];
    u16 n;
} ListD04;

extern u32 *lbl_8044F85C;
extern u32 *lbl_8044F860;

void fn_80115D04(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        ((RectD04 *) lbl_8041C0C0)[((ListD04 *) lbl_8041C408)->n].x = ((SrcD04 *) lbl_80240904)[((ListD04 *) lbl_8041C408)->n].x - lbl_80451928;
        ((RectD04 *) lbl_8041C0C0)[((ListD04 *) lbl_8041C408)->n].y = ((SrcD04 *) lbl_80240904)[((ListD04 *) lbl_8041C408)->n].y - lbl_8045192C;
        ((RectD04 *) lbl_8041C0C0)[((ListD04 *) lbl_8041C408)->n].w = ((SrcD04 *) lbl_80240904)[((ListD04 *) lbl_8041C408)->n].w - lbl_80451928;
        ((RectD04 *) lbl_8041C0C0)[((ListD04 *) lbl_8041C408)->n].h = ((SrcD04 *) lbl_80240904)[((ListD04 *) lbl_8041C408)->n].h - lbl_8045192C;
        ((ListD04 *) lbl_8041C408)->n++;
    }
    if (cr_g_Pads->unk_14 & 0x200) {
        lbl_8044F885[0] = 3;
        lbl_8044F885[1] = 0;
        *lbl_8044F85C |= 1;
        *lbl_8044F860 |= 1;
        fn_80098290(0);
    } else if (cr_g_Pads->unk_14 & 0x100) {
        lbl_8044F885[0] = 9;
        lbl_8044F885[1] = 0;
        fn_80098290(2);
    }
}
