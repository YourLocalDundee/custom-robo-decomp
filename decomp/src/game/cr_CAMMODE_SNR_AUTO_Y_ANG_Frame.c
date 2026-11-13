/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_CAMMODE_SNR_AUTO_Y_ANG_Frame: .text 0x800249C4-0x80024A88 (196 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_CamSystem
{
  u8 unk_00[8];
  u8 unk_08;
  u8 unk_09;
  u8 unk_0a[2];
  u8 *cams[4];
  u8 *unk_1c[4];
  u8 unk_2c;
  u8 unk_2d[3];
  u8 unk_30;
  u8 unk_31[3];
  u32 unk_34;
  u32 unk_38;
  u32 unk_3c;
  u32 unk_40;
  u32 unk_44;
  u32 unk_48[2];
} cr_CamSystem;
__declspec(section ".data") extern cr_CamSystem cr_g_CamSystem;
extern u16 lbl_8044F714;

/* cr_CAMMODE_SNR_AUTO_Y_ANG_Frame: Nov 13, 2026 hand draft: camera yaw wrap; the else spells x = x + (0x10000 - step), not += */

void cr_CAMMODE_SNR_AUTO_Y_ANG_Frame(void) {
    u8 i;

    for (i = 0; i < cr_g_CamSystem.unk_08; i++) {
        if (*(u32 *) (cr_g_CamSystem.cams[i] + 0x114) > lbl_8044F714) {
            *(u32 *) (cr_g_CamSystem.cams[i] + 0x114) -= lbl_8044F714;
        } else {
            *(u32 *) (cr_g_CamSystem.cams[i] + 0x114) = *(u32 *) (cr_g_CamSystem.cams[i] + 0x114) + (0x10000 - lbl_8044F714);
        }
    }
}
