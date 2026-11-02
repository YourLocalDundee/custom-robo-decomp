/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800EAF34: .text 0x800EAF34-0x800EB004 (208 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_803F63E8[56];
typedef unsigned int uintptr_t;
typedef unsigned long size_t;
typedef int bool;
typedef void (*HSD_DevComCallback)(int, uintptr_t, void *, bool cancelflag);
int HSD_DevComRequest(int file, uintptr_t src, uintptr_t dest, size_t size, int type, int pri, HSD_DevComCallback callback, uintptr_t args);
extern u32 lbl_8044F7D0[2];
void fn_800EB0E0(s8 *arg1);

static inline u8 *fn_800EAF34_search0(void) {
    s32 var_r31;

    for (var_r31 = 0; var_r31 < 0x20; var_r31 += 1) {
        if ((s8) lbl_803F63E8[var_r31] == 0) {
            lbl_803F63E8[var_r31] = 1;
            return &lbl_803F63E8[var_r31];
        }
    }
    return NULL;
}

u32 fn_800EAF34(u32 arg0, s32 arg1, u32 arg2) {
    u8 *var_r30;

    var_r30 = fn_800EAF34_search0();
    if (var_r30 != NULL) {
        HSD_DevComRequest(0, *lbl_8044F7D0 + arg1, arg0, arg2, 0x19, 0, (void (*)(int, unsigned int, void *, int)) ((void (*)(s32, u32, void *, s32)) fn_800EB0E0), (unsigned int) ((void (*)(int, unsigned int, void *, int)) ((u32) var_r30)));
    }
    return (u32) var_r30;
}
