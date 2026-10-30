/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80021A40: .text 0x80021A40-0x80021AA4 (100 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* used from the HSD / SDK headers and symbols/cr_types.h (generated from the drafts' context) */
typedef enum _HSD_RenderPass
{
  HSD_RP_SCREEN,
  HSD_RP_TOPHALF,
  HSD_RP_BOTTOMHALF,
  HSD_RP_OFFSCREEN,
  HSD_RP_NUM
} HSD_RenderPass;
void HSD_StartRender(HSD_RenderPass pass);

M2C_UNK VIWaitForRetrace();                         /* extern */
M2C_UNK fn_800055E0();                              /* extern */
M2C_UNK fn_80021AB0();                              /* extern */
M2C_UNK fn_80023164();                              /* extern */
M2C_UNK fn_800373AC();                              /* extern */
M2C_UNK fn_80091518(M2C_UNK);                       /* extern */
M2C_UNK fn_80098410();                              /* extern */
M2C_UNK fn_800EAC48();                              /* extern */
M2C_UNK fn_8019D38C();                              /* extern */
M2C_UNK fn_8019DB14(M2C_UNK);                       /* extern */
M2C_UNK fn_8019DC90();                              /* extern */
extern u32 lbl_8044F3F8;

void fn_80021A40(void) {
    do {
        fn_800EAC48();
        fn_8019D38C();
        fn_8019DB14(0);
        fn_8019DC90();
        VIWaitForRetrace();
        fn_80023164();
        HSD_StartRender(HSD_RP_SCREEN);
        fn_80091518(0);
        fn_800373AC();
        fn_800055E0();
        fn_80021AB0();
        fn_80098410();
    } while ((u32) lbl_8044F3F8 == 0U);
}
