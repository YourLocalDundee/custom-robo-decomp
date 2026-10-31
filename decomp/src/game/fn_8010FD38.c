/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8010FD38: .text 0x8010FD38-0x8010FE6C (308 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8008D434(s32 arg0, s32 *arg1, u8 arg2, f32 farg0, f32 farg1);
s32 fn_80066D3C(u16 arg0);
extern u8 lbl_8041A244[852];
extern f32 lbl_80451874;
extern f32 lbl_80451878;
void fn_8008CE9C(void);
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
void fn_80098290(s32 arg0);
extern u32 lbl_8023CE54[43];
void fn_8008C8A8(void);
extern u32 lbl_8037569C[2417];
extern u8 *lbl_8044F5EC;
extern u8 lbl_8020F4C8[33280];

s32 fn_8010FD38(void) {
    s32 var_r31_7;

    var_r31_7 = 0xFF;
    fn_8008D434(fn_80066D3C(8U), (s32 *) &lbl_8041A244[0x348], 1U, lbl_80451874, lbl_80451878);
    fn_8008CE9C();
    if (cr_g_Pads->unk_14 & 0x100) {
        fn_80098290((s32) M2C_FIELD(lbl_8023CE54, u32 *, 0));
        fn_8008C8A8();
        if (M2C_FIELD(lbl_8041A244, s32 *, 0x348) == 0) {
            M2C_FIELD(lbl_8037569C, u8 *, 6) = lbl_8041A244[6];
            var_r31_7 = 0;
            lbl_8044F5EC = lbl_8020F4C8 + ((s8) lbl_8041A244[6] * 0x340);
            M2C_FIELD(lbl_8037569C, u32 *, 0) = 0;
        } else {
            M2C_FIELD(lbl_8041A244, u8 *, 0) = lbl_8041A244[0x34C];
        }
    } else if (cr_g_Pads->unk_14 & 0x200) {
        fn_80098290((s32) lbl_8023CE54[1]);
        fn_8008C8A8();
        M2C_FIELD(lbl_8041A244, u8 *, 0) = lbl_8041A244[0x34C];
    }
    return var_r31_7;
}
