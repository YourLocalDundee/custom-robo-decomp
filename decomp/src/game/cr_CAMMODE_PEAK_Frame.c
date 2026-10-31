/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_CAMMODE_PEAK_Frame: .text 0x800343CC-0x80034498 (204 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
void fn_8002B3C4(void *arg0);
void fn_8002E04C(void *arg0);
extern u16 lbl_8044F44C;
__declspec(section ".sdata") extern u8 *cr_g_CurrentCam;

void cr_CAMMODE_PEAK_Frame(void *arg0) {
    if (!(cr_g_SeqCurrent.unk_38 & 0x4000) && ((fn_8002B3C4(arg0), fn_8002E04C(arg0), (((u32) cr_g_SeqCurrent.unk_10 == 5) != 0)) || ((u32) cr_g_SeqCurrent.unk_10 == 4))) {
        if (lbl_8044F44C & 0x200) {
            M2C_FIELD(cr_g_CurrentCam, u8 *, 0) = 0xA;
            M2C_FIELD(cr_g_CurrentCam, s8 *, 0x13CC) = 0;
            return;
        }
        M2C_FIELD(arg0, s32 *, 0x110) = 0;
        M2C_FIELD(arg0, s32 *, 0x114) = 0;
        M2C_FIELD(arg0, s32 *, 0xFC) = 0x10;
        M2C_FIELD(cr_g_CurrentCam, u8 *, 0) = 6;
        M2C_FIELD(cr_g_CurrentCam, s8 *, 0x13CC) = 0;
    }
}
