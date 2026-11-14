/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80066F58: .text 0x80066F58-0x80067000 (168 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
u32 cr_Archive_GetEntrySize(s32 archive, u16 entry);

/* fn_80066F58: Nov 13, 2026 hand draft: the chunk size rounded as (size >> 2) << 2; the size of the whole file read through the header copy, not the parameter; the result of cr_Archive_GetEntrySize compared with 0x1C000 in an empty debug test ({ (void) 0; }); dead uses for the order */

void fn_80066F58(void *arg0, u8 *arg1) {
    void *sp8;
    u8 *var_r31;
    void *var_r30;
    u32 var_r29;

    (void) var_r30; (void) var_r30; (void) var_r30; 
    (void) var_r29; 
    (void) arg0; (void) arg0; (void) arg0; 
    var_r29 = 0U;
    var_r30 = arg1;
    var_r31 = arg1 + 0x20;
    sp8 = arg1 + M2C_FIELD((u8 *) var_r30, s32 *, 8);
    while (var_r29 < (u32) M2C_FIELD(var_r30, u32 *, 0xC)) {
        if ((u32) (M2C_FIELD(var_r31, s32 *, 0) + 0xB6B20000) == 0x4631U) {
            M2C_FIELD(arg0, void **, 0) = var_r31;
        } else if ((u32) (M2C_FIELD(var_r31, s32 *, 0) + 0xBBBF0000) == 0x5431U) {
            M2C_FIELD(arg0, void **, 4) = var_r31;
        }
        var_r29 += 1;
        var_r31 += (M2C_FIELD(var_r31, u32 *, 4) >> 2) << 2;
    }
    if ((u32) cr_Archive_GetEntrySize(0x1E, 0U) > 0x1C000U) { (void) 0; }
}
