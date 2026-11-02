/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80111CE8: .text 0x80111CE8-0x80111D50 (104 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
  s8 unk_350;
  u8 unk_351[3];
} cr_StageSelectorState;
__declspec(section ".data") extern cr_StageSelectorState cr_g_StageSelector;
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
void cr_Customize_SetMode(u8 arg0);
typedef struct cr_CustomizeState
{
  u8 mode;
  u8 prev_mode;
  u8 unk_0002;
  u8 unk_0003;
  u8 unk_0004;
  u8 unk_0005;
  u8 unk_0006;
  u8 unk_0007;
  u8 unk_0008[0x1A00];
  u32 unk_1a08[4];
  f32 unk_1a18;
  f32 unk_1a1c;
  f32 unk_1a20;
  u32 unk_1a24[5];
  u32 unk_1a38;
  u32 unk_1a3c;
  u32 unk_1a40;
  u32 unk_1a44;
  u32 unk_1a48;
  u32 unk_1a4c;
  u32 unk_1a50;
  u32 unk_1a54;
  u32 unk_1a58;
  u32 unk_1a5c;
  u32 unk_1a60;
  u32 unk_1a64;
  u8 unk_1a68[0xCC];
  u32 unk_1b34;
  u32 unk_1b38;
  u32 unk_1b3c;
  u32 unk_1b40;
  u32 unk_1b44;
  u32 unk_1b48;
  u32 unk_1b4c;
  u32 unk_1b50;
} cr_CustomizeState;
__declspec(section ".data") extern cr_CustomizeState cr_g_Customize;

u32 fn_80111CE8(void) {
    enum cr_Seq var_r31_13;

    if (cr_g_StageSelector.flags & 2) {
        var_r31_13 = cr_g_SeqCurrent.previous;
    } else {
        var_r31_13 = SEQ_MODE_SELECTOR;
    }
    if ((u32) var_r31_13 == 0xCU) {
        cr_Customize_SetMode(cr_g_Customize.mode);
    }
    return (u32) var_r31_13;
}
