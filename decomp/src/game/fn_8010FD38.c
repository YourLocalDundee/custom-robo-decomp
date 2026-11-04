/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8010FD38: .text 0x8010FD38-0x8010FE6C (308 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_StageData cr_StageData;
void fn_8008D434(s32 arg0, s32 *arg1, u8 arg2, f32 farg0, f32 farg1);
s32 fn_80066D3C(u16 arg0);
typedef struct cr_StageSelectorState
{
  u8 state;
  u8 col;
  u8 row;
  u8 unk_003;
  u8 top_row;
  u8 stage_count;
  u8 stage;
  u8 cursor;
  u32 timer;
  u8 ready;
  u8 unk_00d;
  u8 unk_00e[2];
  u32 flags;
  s8 stage_ids[42];
  u16 stage_msgs[42];
  u8 stage_open[42];
  u8 unk_0bc;
  u8 unk_0bd;
  u8 unk_0be;
  u8 unk_0bf;
  u8 unk_0c0[2];
  u8 unk_0c2;
  u8 unk_0c3[7];
  u8 unk_0ca[2];
  f32 unk_0cc;
  f32 unk_0d0;
  u32 unk_0d4;
  u16 unk_0d8;
  u16 unk_0da;
  u16 unk_0dc;
  u16 unk_0de;
  u8 unk_0e0[0x5C];
  void *unk_13c;
  u8 windows[8][0x34];
  u8 unk_2e0[0x68];
  u32 unk_348;
  u8 unk_34c;
  u8 unk_34d[3];
  s8 unk_350;
  u8 unk_351[3];
} cr_StageSelectorState;
__declspec(section ".data") extern cr_StageSelectorState cr_g_StageSelector;
extern const f32 lbl_80451874;
extern const f32 lbl_80451878;
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
typedef struct cr_KeyPath cr_KeyPath;
struct cr_StageData
{
  char name_jp[0x20];
  u8 unk_020[0x60];
  u8 unk_080;
  u8 unk_081[3];
  cr_KeyPath *path;
  u8 unk_088[0x29C];
  u32 unk_324;
  f32 unk_328[4];
  u8 unk_338[2];
  u16 msg;
  u8 unk_33c[4];
};
__declspec(section ".data") extern cr_StageData cr_g_StageData[40];

extern cr_StageData *lbl_8044F5EC;

s32 fn_8010FD38(void) {
    s32 var_r31_7;

    var_r31_7 = 0xFF;
    fn_8008D434(fn_80066D3C(8U), (s32 *) &cr_g_StageSelector.unk_348, 1U, lbl_80451874, lbl_80451878);
    fn_8008CE9C();
    if (cr_g_Pads->unk_14 & 0x100) {
        fn_80098290((s32) M2C_FIELD(lbl_8023CE54, u32 *, 0));
        fn_8008C8A8();
        if ((s32) cr_g_StageSelector.unk_348 == 0) {
            M2C_FIELD(lbl_8037569C, u8 *, 6) = cr_g_StageSelector.stage;
            var_r31_7 = 0;
            lbl_8044F5EC = &cr_g_StageData[(s8) cr_g_StageSelector.stage];
            M2C_FIELD(lbl_8037569C, u32 *, 0) = 0;
        } else {
            cr_g_StageSelector.state = cr_g_StageSelector.unk_34c;
        }
    } else if (cr_g_Pads->unk_14 & 0x200) {
        fn_80098290((s32) lbl_8023CE54[1]);
        fn_8008C8A8();
        cr_g_StageSelector.state = cr_g_StageSelector.unk_34c;
    }
    return var_r31_7;
}
