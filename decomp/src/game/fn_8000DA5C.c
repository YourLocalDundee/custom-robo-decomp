/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8000DA5C: .text 0x8000DA5C-0x8000DB94 (312 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8031FED8[5];
extern u8 lbl_8044C3F4;
extern u8 lbl_801E1F90[1024];
void fn_8006A4E0(void *arg0, s32 arg1, s32 arg2, u8 arg3, u8 arg4);
extern u8 lbl_8031BED8[124];
extern u8 lbl_801E5024[1505];

void fn_8000DA5C(s8 arg0) {
    u32 var_r30_51;

    if (M2C_FIELD(lbl_8031FED8, s32 *, 0x14) & 0x10000) {
        (&lbl_8044C3F4)[arg0] -= 1;
    }
    if (M2C_FIELD(lbl_8031FED8, s32 *, 0x14) & 0x100000) {
        (&lbl_8044C3F4)[arg0] += 1;
    }
    if ((s8) (&lbl_8044C3F4)[arg0] > 4) {
        (&lbl_8044C3F4)[arg0] = 0;
    }
    if ((s8) (&lbl_8044C3F4)[arg0] < 0) {
        (&lbl_8044C3F4)[arg0] = 4;
    }
    var_r30_51 = 0;
    while (var_r30_51 < 0x20U) {
        M2C_FIELD(&lbl_801E1F90[(var_r30_51 << 5)], u16 *, 0xC10) = 0xFFFF;
        var_r30_51 += 1;
    }
    fn_8006A4E0(M2C_FIELD(lbl_8031BED8, void **, 0x18), 0, (s32) (lbl_801E5024 + ((s8) (&lbl_8044C3F4)[arg0] << 6)), 0U, 0U);
}
