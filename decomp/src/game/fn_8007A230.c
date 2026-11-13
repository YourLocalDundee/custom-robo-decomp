/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007A230: .text 0x8007A230-0x8007A2C8 (152 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_GameMode
{
  u32 flags;
  u32 unk_04;
  u32 unk_08;
  u32 unk_0c;
  u32 battle_mode;
  s16 unk_14;
  s16 unk_16;
  s16 unk_18;
  u8 unk_1a[2];
  u16 unk_1c[19];
  u8 unk_42;
  u8 unk_43;
  u8 unk_44;
  u8 unk_45;
  u8 unk_46[2];
} cr_GameMode;
__declspec(section ".data") extern cr_GameMode cr_g_GameMode;
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

/* fn_8007A230: Nov 5, 2026 hand draft (m2c 82%): the divisor 10 is a local (li r30, 10; divw), not a literal (which compiles to the multiply-high form) */
void fn_8007A230(void *arg0) {
    u8 temp_r4_21;
    s32 ten;

    if (cr_g_GameMode.flags & 0x04000000) {
        M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFDF;
    }
    if ((u32) cr_g_SeqCurrent.unk_10 != 9) {
        ten = 10;
        temp_r4_21 = (s32) M2C_FIELD(arg0, u8 *, 0x57) / ten;
        if ((M2C_FIELD(arg0, u8 *, 0x57) - temp_r4_21) > 0) {
            M2C_FIELD(arg0, u8 *, 0x57) -= temp_r4_21;
        } else {
            M2C_FIELD(arg0, u8 *, 0x57) = 0U;
            M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFDF;
        }
        M2C_FIELD(arg0, s32 *, 0) |= 1;
    }
}
