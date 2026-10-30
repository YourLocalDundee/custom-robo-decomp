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
__declspec(section ".data") extern cr_Seq cr_g_SeqCurrent;
void cr_Cam_Init(void);
extern u8 lbl_8041A244[852];
void fn_8008F838(void);
void cr_Cam_SetMode(u8 mode);
s32 fn_80113934(void);
u8 fn_80113680(void);
u32 fn_80111D50(void);
u32 fn_80111CE8(void);

void cr_SEQ_STAGE_SELECTOR_FrameA(void) {
    u8 temp_r3_47;

    if (M2C_FIELD(&cr_g_SeqCurrent, u32 *, 0x24) == 0U) {
        M2C_FIELD(&cr_g_SeqCurrent, u32 *, 0x24) = 1U;
        cr_Cam_Init();
        if (M2C_FIELD(lbl_8041A244, s32 *, 0x10) & 2) {
            fn_8008F838();
            cr_Cam_SetMode(0x10U);
        } else {
            cr_Cam_SetMode(0xFU);
        }
        fn_80113934();
        if (M2C_FIELD(lbl_8041A244, s32 *, 0x10) & 1) {
            M2C_FIELD(&cr_g_SeqCurrent, s32 *, 0x38) |= 0x100;
        } else {
            M2C_FIELD(&cr_g_SeqCurrent, s32 *, 0x40) = 2;
        }
    }
    temp_r3_47 = fn_80113680();
    switch ((s32) temp_r3_47) {                     /* irregular */
    case 0x1:
        M2C_FIELD(&cr_g_SeqCurrent, u32 *, 8) = fn_80111D50();
        return;
    case 0x0:
        M2C_FIELD(&cr_g_SeqCurrent, u32 *, 8) = fn_80111CE8();
        return;
    case 0x2:
        M2C_FIELD(&cr_g_SeqCurrent, u32 *, 8) = 2U;
        /* fallthrough */
    case 0xFF:
        return;
    }
}
