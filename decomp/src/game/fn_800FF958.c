/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800FF958: .text 0x800FF958-0x800FFA50 (248 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_80407260[75344];
extern f32 lbl_8044EC18[2];
extern u8 lbl_80239B48[32];

static inline u8 fn_800FF958_search0(u8 var_r28) {
    u8 var_r31;

    for (var_r31 = 0; var_r31 < 4U; var_r31 += 1) {
        if ((0x400 << var_r31) & M2C_FIELD((lbl_80407260 + (var_r28 * 0x4994)), s32 *, 0x498C)) {
            return var_r31;
        }
    }
    return 0xFF;
}

void fn_800FF958(void *arg0) {
    u8 var_r29;
    u8 var_r28;

    var_r28 = (u8) M2C_FIELD(arg0, s32 *, 0x10);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) = (f32) M2C_FIELD(lbl_8044EC18, f32 *, 0);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 4) = (f32) lbl_8044EC18[1];
    var_r29 = fn_800FF958_search0(var_r28);
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), f32 *, 0) += ((f32 (*)[2]) lbl_80239B48)[var_r29][0];
    M2C_FIELD((M2C_FIELD(arg0, void **, 0x5C)), f32 *, 4) += ((f32 (*)[2]) lbl_80239B48)[var_r29][1];
    M2C_FIELD(arg0, s32 *, 0) &= 0xFFFFFFFE;
}
