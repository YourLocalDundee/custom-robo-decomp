/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_801177CC: .text 0x801177CC-0x80117938 (364 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
extern u8 lbl_8041A67C[6700];
extern u8 lbl_8041C828[20];
void fn_80096544(u32 arg0);
void fn_80098290(s32 arg0);

/* fn_801177CC: Nov 6, 2026 hand draft (m2c 89%): the selection byte lbl_8041C828[1] is signed (read and written through an s8 view; the context types the table u8[20]) */

void fn_801177CC(void) {

    if (cr_g_Pads->unk_2c & 0x08000000) {
        *M2C_FIELD((lbl_8041A67C + (((s8 *) lbl_8041C828)[1] * 0x34)), s32 **, 0x30) &= 0xFFFFFFFE;
        ((s8 *) lbl_8041C828)[1] += 1;
        ((s8 *) lbl_8041C828)[1] &= 1;
        fn_80096544(((s8 *) lbl_8041C828)[1]);
        fn_80098290(1);
    } else if (cr_g_Pads->unk_2c & 0x02000000) {
        *M2C_FIELD((lbl_8041A67C + (((s8 *) lbl_8041C828)[1] * 0x34)), s32 **, 0x30) &= 0xFFFFFFFE;
        fn_80098290(1);
        ((s8 *) lbl_8041C828)[1] -= 1;
        ((s8 *) lbl_8041C828)[1] &= 1;
        fn_80096544(((s8 *) lbl_8041C828)[1]);
    }
    *M2C_FIELD((lbl_8041A67C + (((s8 *) lbl_8041C828)[1] * 0x34)), s32 **, 0x30) |= 1;
}
