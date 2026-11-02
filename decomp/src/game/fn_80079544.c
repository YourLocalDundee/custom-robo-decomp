/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80079544: .text 0x80079544-0x800796EC (424 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
extern u8 lbl_80379FE0[432];
typedef struct HSD_JObj HSD_JObj;
u8 *fn_800798CC(void *arg0, HSD_JObj *arg1);

s32 fn_80079544(s32 arg0, s32 *arg1) {
    s32 var_r31;
    u32 var_r30;
    s32 var_r29;
    u32 var_r27;

    var_r31 = 0;
    var_r30 = M2C_FIELD(cr_g_CurrentBattleRobo, u8 *, 0x8C);
    for (; var_r31 < 8; var_r31 += 1) {
        if (*(lbl_80379FE0 + (var_r30 * 0x6C) + (var_r31 * 0xC)) == 0) {
            break;
        }
    }
    if (var_r31 != 8) {
        *(lbl_80379FE0 + (var_r30 * 0x6C) + (var_r31 * 0xC)) = 1;
        *arg1 = M2C_FIELD((lbl_80379FE0 + (var_r30 * 0x6C) + (var_r31 * 0xC)), s32 *, 4);
        return (long) (fn_800798CC((void *) arg0, M2C_FIELD((lbl_80379FE0 + (var_r30 * 0x6C) + (var_r31 * 0xC)), HSD_JObj **, 4)));
    }
    var_r27 = 0U;
    var_r29 = 0;
    var_r31 = 0;
    while (var_r31 < 8) {
        if (M2C_FIELD(M2C_FIELD((lbl_80379FE0 + (var_r30 * 0x6C) + (var_r31 * 0xC)), void **, 8), u32 *, 0xC) >= var_r27) {
            var_r29 = var_r31;
            var_r27 = M2C_FIELD(M2C_FIELD((lbl_80379FE0 + (var_r30 * 0x6C) + (var_r31 * 0xC)), void **, 8), u32 *, 0xC);
        }
        var_r31 += 1;
    }
    *(lbl_80379FE0 + (var_r30 * 0x6C) + (var_r29 * 0xC)) = 1;
    *arg1 = M2C_FIELD((lbl_80379FE0 + (var_r30 * 0x6C) + (var_r29 * 0xC)), s32 *, 4);
    return (long) (fn_800798CC((void *) arg0, M2C_FIELD((lbl_80379FE0 + (var_r30 * 0x6C) + (var_r29 * 0xC)), HSD_JObj **, 4)));
}
