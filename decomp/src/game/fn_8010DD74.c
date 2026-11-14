/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8010DD74: .text 0x8010DD74-0x8010DE04 (144 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
s32 fn_80020A40(void *arg0);
void fn_800215F8(void *arg0);

/* fn_8010DD74: Nov 13, 2026 hand draft: the rank sweep: one dead use of the parameter ((void) p;) gives it its saved register in the original order */

s32 fn_8010DD74(s32 arg0, u8 arg1) {
    (void) arg0; 
    if (fn_80020A40((void *) (*(u32 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC])) != 0) {
        fn_800215F8((void *) (*(u32 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC]));
        *(u32 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC] = 0;
        *(s8 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47F0] = 3;
    }
    return 0;
}
