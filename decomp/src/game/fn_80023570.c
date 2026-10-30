/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80023570: .text 0x80023570-0x800235E8 (120 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 *lbl_8044F420;
void *OSAllocFromHeap(int heap, unsigned long size);
typedef int OSHeapHandle;
extern volatile OSHeapHandle __OSCurrHeap;
extern u32 lbl_8044F41C;
extern u32 lbl_8044F418;
extern u8 *lbl_8044F414;
extern u32 lbl_8044F410;

void fn_80023570(u32 arg0) {
    lbl_8044F420 = OSAllocFromHeap(__OSCurrHeap, arg0);
    M2C_FIELD(lbl_8044F420, s32 *, 0) = 0;
    M2C_FIELD(lbl_8044F420, s32 *, 4) = (s32) (arg0 - 0x20);
    M2C_FIELD(lbl_8044F420, s32 *, 0xC) = 0;
    lbl_8044F41C = arg0 - 0x20;
    lbl_8044F418 = arg0;
    lbl_8044F414 = &lbl_8044F420[arg0];
    lbl_8044F410 = arg0;
}
