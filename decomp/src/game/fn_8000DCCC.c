/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8000DCCC: .text 0x8000DCCC-0x8000DE04 (312 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
extern u8 lbl_8044C3F4;
extern u8 lbl_801E1F90[1024];
void fn_8006A4E0(void *arg0, s32 arg1, s32 arg2, u8 arg3, u8 arg4);
extern u8 lbl_8031BED8[124];
extern u8 lbl_801E4824[576];

void fn_8000DCCC(s8 arg0) {
    u32 var_r30_51;

    if (cr_g_Pads->unk_14 & 0x10000) {
        (&lbl_8044C3F4)[arg0] -= 1;
    }
    if (cr_g_Pads->unk_14 & 0x100000) {
        (&lbl_8044C3F4)[arg0] += 1;
    }
    if ((s8) (&lbl_8044C3F4)[arg0] > 8) {
        (&lbl_8044C3F4)[arg0] = 0;
    }
    if ((s8) (&lbl_8044C3F4)[arg0] < 0) {
        (&lbl_8044C3F4)[arg0] = 8;
    }
    var_r30_51 = 0;
    while (var_r30_51 < 0x20U) {
        M2C_FIELD(&lbl_801E1F90[(var_r30_51 << 5)], u16 *, 0x410) = 0xFFFF;
        var_r30_51 += 1;
    }
    fn_8006A4E0(M2C_FIELD(lbl_8031BED8, void **, 0x10), 0, (s32) (lbl_801E4824 + ((s8) (&lbl_8044C3F4)[arg0] << 6)), 0U, 0U);
}
