/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80021014: .text 0x80021014-0x800210B0 (156 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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

/* fn_80021014: Nov 3, 2026 hand draft (m2c 65%): the copy of arg0 as `arg0 = arg0;` (param_self_copies), the goto loop as `while (1)` with the exit test first, the byte compared as (s32) (cmpwi) */

void fn_80021014(void *arg0) {
    s8 n;

    n = 2;
    arg0 = arg0;
    while (1) {
        if ((s32) M2C_FIELD(arg0, u8 *, 1) == 3 && n == 0) {
            break;
        }
        fn_800EAC48();
        fn_8019D38C();
        fn_8019DB14(HSD_RP_SCREEN);
        fn_8019DC90();
        VIWaitForRetrace();
        fn_80023164();
        HSD_StartRender(HSD_RP_SCREEN);
        fn_80091518(0U);
        fn_80091518(2U);
        fn_800373AC();
        fn_800055E0();
        fn_80021AB0();
        fn_80098410();
        if (n != 0) {
            n -= 1;
        }
    }
}
