/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8006FAC4: .text 0x8006FAC4-0x8006FB74 (176 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8037569C[2417];
typedef struct HSD_JObj HSD_JObj;
void fn_80196C50(HSD_JObj *arg0, f32 farg0);
extern f32 lbl_80450BAC;

void fn_8006FAC4(void) {
    s32 var_r31_6;

    var_r31_6 = 0;
    while (var_r31_6 < (s16) M2C_FIELD(lbl_8037569C, s16 *, 8)) {
        fn_80196C50(M2C_FIELD((((u32 *) ((u8 *) lbl_8037569C + (var_r31_6 * 0x13C)))), HSD_JObj **, 0xB8), lbl_80450BAC);
        var_r31_6 += 1;
    }
    if (M2C_FIELD((((u32 *) ((u8 *) lbl_8037569C + (M2C_FIELD(lbl_8037569C, s16 *, 8) * 0x13C)))), u32 *, 0xB8) != 0U) {
        fn_80196C50(M2C_FIELD((((u32 *) ((u8 *) lbl_8037569C + (M2C_FIELD(lbl_8037569C, s16 *, 8) * 0x13C)))), HSD_JObj **, 0xB8), lbl_80450BAC);
    }
}
