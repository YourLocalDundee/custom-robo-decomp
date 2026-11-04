/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80024848: .text 0x80024848-0x800249C4 (380 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_CamSystem
{
  u8 unk_00[8];
  u8 unk_08;
  u8 unk_09;
  u8 unk_0a[2];
  u8 *cams[4];
  u8 *unk_1c[4];
  u8 unk_2c;
  u8 unk_2d[3];
  u8 unk_30;
  u8 unk_31[3];
  u32 unk_34;
  u32 unk_38;
  u32 unk_3c;
  u32 unk_40;
  u32 unk_44;
  u32 unk_48[2];
} cr_CamSystem;
__declspec(section ".data") extern cr_CamSystem cr_g_CamSystem;
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
extern u8 lbl_8044F45A;
extern const f32 lbl_80450518;
extern const f32 lbl_8045051C;
extern const f32 lbl_80450520;
extern const f32 lbl_80450524;

/* fn_80024848: Nov 3, 2026 hand draft (m2c 82%): the else store written out, `p->f = p->f + 0xFFF7` (the original computes the element address twice), and the u32 counter compared as (u8) */

void fn_80024848(void) {
    f32 var_f31_37;
    u32 var_r31_6;

    var_r31_6 = 0;
    while ((u8) var_r31_6 < cr_g_CamSystem.unk_08) {
        if (M2C_FIELD(M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC), u32 *, 0x114) > 9U) {
            M2C_FIELD((M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC)), s32 *, 0x114) -= (cr_g_StageSelector.stage_open[(s8) cr_g_StageSelector.cursor] * 9);
            if (lbl_8044F45A != 0) {
                var_f31_37 = lbl_80450518;
            } else {
                var_f31_37 = lbl_8045051C;
            }
            M2C_FIELD((M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC)), f32 *, 0xA0) += var_f31_37;
            if (M2C_FIELD(M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC), f32 *, 0xA0) < lbl_80450520) {
                lbl_8044F45A = 0;
            } else if (M2C_FIELD(M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC), f32 *, 0xA0) > lbl_80450524) {
                lbl_8044F45A = 1;
            }
        } else {
            M2C_FIELD(M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC), s32 *, 0x114) = M2C_FIELD(M2C_FIELD(&cr_g_CamSystem.unk_00[(var_r31_6 * 4) & 0x3FC], void **, 0xC), s32 *, 0x114) + 0xFFF7;
        }
        var_r31_6 += 1;
    }
}
