/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800AA130_fn_800AA130: .text 0x800AA130-0x800AA17C (76 bytes, 1 functions: fn_800AA130 .. fn_800AA130), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */

/* the file's functions (each is defined below, in address order) */
s32 fn_800AA130(u8 *arg0);

/* nothing in main.dol references it: keep it (mwld strips unreferenced C functions) */
#pragma force_active on
s32 fn_800AA130(u8 *arg0) {
    s32 var_r31_4;

    var_r31_4 = 0;
    switch ((s8) (u8) (*arg0)) {
    case 0x41:
    case 0x45:
    case 0x49:
    case 0x4F:
    case 0x55:
    case 0x61:
    case 0x65:
    case 0x69:
    case 0x6F:
    case 0x75:
        var_r31_4 = 1;
        break;
    }
    return var_r31_4;
}
#pragma force_active reset
