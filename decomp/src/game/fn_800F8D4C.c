/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800F8D4C: .text 0x800F8D4C-0x800F8E4C (256 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80023394(s8 *arg0, u32 arg1);
s32 fn_8006A4E0(void *arg0, s32 arg1, s32 arg2, u8 arg3, u8 arg4);
extern char lbl_80451718[3];
void *fn_800F9014(void *arg0);
void fn_800F96A4(void *arg0, void *arg1);

void *fn_800F8D4C(void *arg0) {
    void *var_r30_39;
    void *temp_r29_15;

    if ((M2C_FIELD(arg0, u8 *, 0x4855) == 0) && ((s32) M2C_FIELD(arg0, u8 *, 0x4860) == 5)) {
        temp_r29_15 = M2C_FIELD(arg0, void **, 0x144);
        fn_80023394(M2C_FIELD(M2C_FIELD(M2C_FIELD(temp_r29_15, void **, 0x5C), void **, 0x1C), s8 **, 4), 0x4000U);
        M2C_FIELD(M2C_FIELD(M2C_FIELD(M2C_FIELD(temp_r29_15, void **, 0x5C), void **, 0x1C), void **, 4), u16 *, 0x10) = 0xFFFF;
        M2C_FIELD(temp_r29_15, s32 *, 0) &= 0xFFFFFFFE;
        M2C_FIELD(arg0, u32 *, 0x4878) = -1;
        fn_8006A4E0(M2C_FIELD(arg0, void **, 0x144), 0, (s32) lbl_80451718, 0U, 0U);
        var_r30_39 = NULL;
        M2C_FIELD(arg0, u32 *, 0x4878) = 0xFFFFU;
    } else {
        var_r30_39 = fn_800F9014(arg0);
    }
    if (var_r30_39 != ((void *) 0)) {
        if (M2C_FIELD(arg0, u32 *, 0x4878) != (u32) M2C_FIELD(var_r30_39, u32 *, 8)) {
            fn_800F96A4(arg0, var_r30_39);
        }
        M2C_FIELD(arg0, u32 *, 0x4878) = M2C_FIELD(var_r30_39, u32 *, 8);
    }
    return var_r30_39;
}
