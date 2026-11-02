/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * main: .text 0x8000570C-0x800057B8 (172 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void cr_Game_InitSystem(void);
void cr_Particle_InitCallbacks(void);
void fn_80023EC8(void);
typedef struct HSD_CObj HSD_CObj;
extern HSD_CObj *lbl_8044F468;
typedef union HSD_CObjDesc HSD_CObjDesc;
HSD_CObj *HSD_CObjLoadDesc(HSD_CObjDesc *desc);
extern u8 lbl_801C4FA8[56];
extern u32 lbl_80323270[4];
extern u8 lbl_801C50C0[16];
void cr_Game_Update(void);
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
void cr_Cam_Update(void);
void fn_800055E8(void);
void fn_800055E0(void);
void fn_80098484(void);
extern u32 lbl_8044F2A0;

void main(void) {
    s32 sp8;
    u32 var_r31_15;

    sp8 = 0;
    cr_Game_InitSystem();
    cr_Particle_InitCallbacks();
    fn_80023EC8();
    lbl_8044F468 = HSD_CObjLoadDesc((HSD_CObjDesc *) lbl_801C4FA8);
    var_r31_15 = 0;
    while (var_r31_15 < 4U) {
        *(((u32 *) ((u8 *) lbl_80323270 + (var_r31_15 * 4)))) = (unsigned long) ((HSD_CObj *) (HSD_CObjLoadDesc(M2C_FIELD((lbl_801C50C0 + (var_r31_15 * 4)), HSD_CObjDesc **, 0))));
        var_r31_15 += 1;
    }
loop_3:
    cr_Game_Update();
    if (!(cr_g_SeqCurrent.unk_38 & 0x80000)) {
        cr_Cam_Update();
    }
    fn_800055E8();
    fn_800055E0();
    fn_80098484();
    lbl_8044F2A0 += 1;
    goto loop_3;
}
