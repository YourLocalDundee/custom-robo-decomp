/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80112AD8: .text 0x80112AD8-0x80112B88 (176 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_StageSelectorState
{
  u8 state;
  u8 unk_001;
  u8 unk_002;
  u8 unk_003;
  u8 unk_004;
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
  u8 unk_350;
  u8 unk_351[3];
} cr_StageSelectorState;
__declspec(section ".data") extern cr_StageSelectorState cr_g_StageSelector;

void fn_80112AD8(void *arg0) {
    void *temp_r31_18;

    if ((cr_g_StageSelector.flags & 4) && !(cr_g_StageSelector.flags & 8)) {
        temp_r31_18 = M2C_FIELD(M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), void **, 0x1C), void **, 4);
        M2C_FIELD(arg0, s32 *, 0) |= 1;
        M2C_FIELD(arg0, s32 *, 0x10) += 1;
        if (M2C_FIELD(arg0, s32 *, 0x10) > 2) {
            M2C_FIELD(arg0, s32 *, 0x10) = 0;
            M2C_FIELD(temp_r31_18, u16 *, 0x10) += 1;
            if ((s32) (M2C_FIELD(temp_r31_18, u16 *, 0x10) - 0x2C) >= 3) {
                M2C_FIELD(temp_r31_18, u16 *, 0x10) = 0x2CU;
            }
        }
    } else {
        M2C_FIELD(arg0, s32 *, 0x10) = 0;
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
    }
}
