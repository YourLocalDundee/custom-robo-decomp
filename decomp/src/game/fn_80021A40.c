/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80021A40: .text 0x80021A40-0x80021AA4 (100 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F3F8;
void fn_800EAC48(void);
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
void HSD_StartRender(HSD_RenderPass pass);
void fn_80091518(u32 arg0);
void fn_800373AC(void);
void fn_800055E0(void);
void fn_80021AB0(void);
void fn_80098410(void);

void fn_80021A40(void) {
    do {
        fn_800EAC48();
        fn_8019D38C();
        fn_8019DB14(HSD_RP_SCREEN);
        fn_8019DC90();
        VIWaitForRetrace();
        fn_80023164();
        HSD_StartRender(HSD_RP_SCREEN);
        fn_80091518(0U);
        fn_800373AC();
        fn_800055E0();
        fn_80021AB0();
        fn_80098410();
    } while (lbl_8044F3F8 == 0);
}
