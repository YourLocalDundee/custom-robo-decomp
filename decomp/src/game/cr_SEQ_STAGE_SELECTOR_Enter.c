/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_STAGE_SELECTOR_Enter: .text 0x8000FA54-0x8000FBC0 (364 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_80321D40[18];
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
__declspec(section ".data") extern cr_Seq cr_g_SeqCurrent;
extern u8 lbl_8031F240[40];
extern u32 lbl_8044F504;
extern u32 lbl_8037569C[2417];
extern u16 lbl_801C5BD0[32];
extern u8 lbl_8024E120[840832];

void cr_SEQ_STAGE_SELECTOR_Enter(void) {
    if ((M2C_FIELD(lbl_80321D40, u32 *, 0) & 0x02000000) && (M2C_FIELD(&cr_g_SeqCurrent, s32 *, 4) == 4)) {
        M2C_FIELD(lbl_80321D40, u32 *, 0) = M2C_FIELD(lbl_8031F240, u32 *, 8);
        M2C_FIELD(lbl_80321D40, s16 *, 0x14) = M2C_FIELD(lbl_8031F240, s16 *, 2);
        lbl_80321D40[4] = M2C_FIELD(lbl_8031F240, u32 *, 4);
        lbl_8044F504 = M2C_FIELD(lbl_8031F240, u32 *, 0xC);
        M2C_FIELD(lbl_8037569C, u8 *, 6) = M2C_FIELD(lbl_8031F240, u8 *, 0);
        M2C_FIELD(lbl_801C5BD0, u16 *, 0) = M2C_FIELD(lbl_8031F240, u16 *, 0x10);
        lbl_801C5BD0[8] = M2C_FIELD(lbl_8031F240, u16 *, 0x12);
        lbl_801C5BD0[0x10] = M2C_FIELD(lbl_8031F240, u16 *, 0x14);
        lbl_801C5BD0[0x18] = M2C_FIELD(lbl_8031F240, u16 *, 0x16);
        M2C_FIELD(lbl_8024E120, s32 *, 0x1D8) = M2C_FIELD(lbl_8031F240, s32 *, 0x18);
        M2C_FIELD(lbl_8024E120, s32 *, 0x336F8) = M2C_FIELD(lbl_8031F240, s32 *, 0x1C);
        M2C_FIELD(lbl_8024E120, s32 *, 0x66C18) = M2C_FIELD(lbl_8031F240, s32 *, 0x20);
        M2C_FIELD(lbl_8024E120, s32 *, 0x9A138) = M2C_FIELD(lbl_8031F240, s32 *, 0x24);
    }
}
