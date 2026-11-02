/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80111EEC_fn_80111EEC: .text 0x80111EEC-0x8011214C (608 bytes, 1 functions: fn_80111EEC .. fn_80111EEC), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
extern u8 lbl_8023C6AC[416];
extern f32 lbl_8044ECDC[2];
extern f32 lbl_8044ECE8[2];
extern f32 lbl_80451858;
extern u16 lbl_8044ECE4;

/* the file's functions (each is defined below, in address order) */
void fn_80111EEC(void);

void fn_80111EEC(void) {
    s8 sp8;
    u8 var_r31;
    u8 var_r30;
    u8 var_r29;
    u8 var_r28;
    u8 var_r27;

    var_r30 = 0;
    var_r27 = cr_g_StageSelector.top_row;
    while (var_r30 < 2U) {
        var_r29 = 0;
        while (var_r29 < 6U) {
            sp8 = 0;
            var_r31 = var_r29 + (var_r30 * 6);
            var_r28 = var_r31 + (var_r27 * 6);
            if ((s8) cr_g_StageSelector.stage_count <= (s32) var_r28) {
                M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], u16 *, 0x10) = 0xFFFF;
            } else if (cr_g_StageSelector.stage_open[var_r28] != 0) {
                M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], u16 *, 0x10) = cr_g_StageSelector.stage_msgs[var_r28];
            } else {
                M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], s16 *, 0x10) = 0xB;
            }
            if (cr_g_StageSelector.unk_00d != 0) {
                if (M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], u16 *, 0x10) == 1) {
                    M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], s16 *, 0x10) = 0x11;
                } else if (M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], u16 *, 0x10) == 0x1F) {
                    M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], s16 *, 0x10) = 0x12;
                }
            }
            M2C_FIELD(lbl_8023C6AC, f32 *, (var_r31 << 5) & 0x1FE0) = (f32) (M2C_FIELD(lbl_8044ECDC, f32 *, 0) + ((M2C_FIELD(lbl_8044ECE8, f32 *, 0) * (f32) var_r29) + lbl_80451858));
            M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], f32 *, 4) = (f32) (lbl_8044ECDC[1] + ((lbl_8044ECE8[1] * (f32) var_r30) + lbl_80451858));
            M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], u16 *, 0xC) = M2C_FIELD(&lbl_8044ECE4, u16 *, 0);
            var_r29 += 1;
            M2C_FIELD(&lbl_8023C6AC[(var_r31 << 5) & 0x1FE0], u16 *, 0xE) = M2C_FIELD(&lbl_8044ECE4, u16 *, 2);
        }
        var_r30 += 1;
    }
    M2C_FIELD(lbl_8023C6AC, u16 *, 0x190) = 0xFFFF;
}
