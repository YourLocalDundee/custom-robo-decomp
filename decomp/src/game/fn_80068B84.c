/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80068B84: .text 0x80068B84-0x80068C7C (248 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8032ECD0[7920];
s8 *fn_8006868C(void *arg0);
s8 fn_80069AD8(void *arg0, void *arg1, u8 *arg2);

/* fn_80068B84: Nov 13, 2026 hand draft: name entry setup then the fn_80068C7C name loop; the message parameter is void * cast at each use (that sets its register r27) */

u8 *fn_80068B84(void *msgv, volatile s32 arg1, u8 *v) {
    u8 id = 0;
    s32 i;
    u8 *name = 0;

    id = ((u8 *) msgv)[5];
    (lbl_8032ECD0 + id * 0x108)[5] = ((u8 *) msgv)[6];
    (lbl_8032ECD0 + id * 0x108)[6] = ((u8 *) msgv)[7];
    (lbl_8032ECD0 + id * 0x108)[4] = ((u8 *) msgv)[8];
    fn_8006868C(lbl_8032ECD0 + id * 0x108);
    i = 0;
    name = lbl_8032ECD0 + id * 0x108 + 7;
    while (name[(s16) i] != 0) {
        i += ((s32 (*)(u8 *, void *, u8 *)) fn_80069AD8)(v, (void *) arg1, &name[(s16) i]);
        v += 0x20;
    }
    return v;
}
