/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_STAGE_SELECTOR_FrameA: .text 0x8000FCCC-0x8000FDEC (288 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef enum cr_Seq
{
  SEQ_LOGO,
  SEQ_MOVIE,
  SEQ_TITLE,
  SEQ_FIELD,
  SEQ_BATTLE,
  SEQ_MODE_SELECTOR,
  SEQ_CUSTOMIZE,
  SEQ_STAGE_SELECTOR,
  SEQ_SCENARIO,
  SEQ_OPTION,
  SEQ_NAME_ENTRY,
  SEQ_MEMORY_CARD,
  SEQ_MENU,
  SEQ_ILLUSTRATED_BOOK,
  SEQ_ENDING
} cr_Seq;
typedef struct cr_SeqState
{
  cr_Seq current;
  cr_Seq previous;
  cr_Seq next;
  u8 unk_0c[4];
  u32 unk_10;
  u8 unk_14[0x10];
  u32 unk_24;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[4];
  u32 unk_34;
  s32 unk_38;
  s16 unk_3c;
  u8 unk_3e[2];
  s32 unk_40;
  u8 unk_44[4];
} cr_SeqState;
__declspec(section ".data") extern cr_SeqState cr_g_SeqCurrent;
void cr_Cam_Init(void);
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
void fn_8008F838(void);
void cr_Cam_SetMode(u8 mode);
s32 cr_StageSelector_Init(void);
u8 cr_StageSelector_Update(void);
enum cr_Seq fn_80111D50(void);
u32 fn_80111CE8(void);

void cr_SEQ_STAGE_SELECTOR_FrameA(void) {
    u8 temp_r3_47;

    if ((u32) cr_g_SeqCurrent.unk_24 == 0) {
        cr_g_SeqCurrent.unk_24 = 1;
        cr_Cam_Init();
        if (cr_g_StageSelector.flags & 2) {
            fn_8008F838();
            cr_Cam_SetMode(0x10U);
        } else {
            cr_Cam_SetMode(0xFU);
        }
        cr_StageSelector_Init();
        if (cr_g_StageSelector.flags & 1) {
            cr_g_SeqCurrent.unk_38 |= 0x100;
        } else {
            cr_g_SeqCurrent.unk_40 = 2;
        }
    }
    temp_r3_47 = cr_StageSelector_Update();
    switch ((s32) temp_r3_47) {                     /* irregular */
    case 0x1:
        cr_g_SeqCurrent.next = fn_80111D50();
        return;
    case 0x0:
        cr_g_SeqCurrent.next = fn_80111CE8();
        return;
    case 0x2:
        cr_g_SeqCurrent.next = SEQ_TITLE;
        /* fallthrough */
    case 0xFF:
        return;
    }
}
