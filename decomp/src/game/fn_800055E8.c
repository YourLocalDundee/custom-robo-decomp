/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800055E8: .text 0x800055E8-0x8000570C (292 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_804501A0;
extern u8 lbl_804501A1;
extern u8 lbl_804501A2;
extern u8 lbl_804501A3;
extern u8 lbl_804501A4;
extern u8 lbl_804501A5;
extern u8 lbl_804501A6;
extern u8 lbl_804501A7;
extern u8 lbl_804501A8;
extern u8 lbl_804501A9;
extern u8 lbl_804501AA;
extern u8 lbl_804501AB[5];
typedef void (*cr_SeqFunc)(void);
__declspec(section ".data") extern cr_SeqFunc cr_g_SeqFrameB[15];
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
void fn_8019D38C(void);
typedef enum _HSD_RenderPass
{
  HSD_RP_SCREEN,
  HSD_RP_TOPHALF,
  HSD_RP_BOTTOMHALF,
  HSD_RP_OFFSCREEN,
  HSD_RP_NUM
} HSD_RenderPass;
void fn_8019DB14(enum _HSD_RenderPass arg0);
void fn_8019DC90(void);
void VIWaitForRetrace(void);
void fn_80023164(void);
void fn_8006E7D0(void);
void HSD_StartRender(HSD_RenderPass pass);
void fn_80091518(u32 arg0);
__declspec(section ".data") extern cr_SeqFunc cr_g_SeqFrameC[15];
void fn_800373AC(void);

void fn_800055E8(void) {
    u8 sp13;
    u8 sp12;
    u8 sp11;
    u8 sp10;
    u8 spF;
    u8 spE;
    u8 spD;
    u8 spC;
    u8 spB;
    u8 spA;
    u8 sp9;
    u8 sp8;

    sp10 = lbl_804501A0;
    sp11 = lbl_804501A1;
    sp12 = lbl_804501A2;
    sp13 = lbl_804501A3;
    spC = lbl_804501A4;
    spD = lbl_804501A5;
    spE = lbl_804501A6;
    spF = lbl_804501A7;
    sp8 = lbl_804501A8;
    sp9 = lbl_804501A9;
    spA = lbl_804501AA;
    spB = *lbl_804501AB;
    cr_g_SeqFrameB[cr_g_SeqCurrent]();
    fn_8019D38C();
    fn_8019DB14(HSD_RP_SCREEN);
    fn_8019DC90();
    VIWaitForRetrace();
    fn_80023164();
    fn_8006E7D0();
    HSD_StartRender(HSD_RP_SCREEN);
    fn_80091518(1U);
    cr_g_SeqFrameC[cr_g_SeqCurrent]();
    fn_80091518(0U);
    fn_800373AC();
    fn_80091518(2U);
}
