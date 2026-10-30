/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80023780: .text 0x80023780-0x800237F8 (120 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 *lbl_8044F434;
void *OSAllocFromHeap(int heap, unsigned long size);
typedef int OSHeapHandle;
extern volatile OSHeapHandle __OSCurrHeap;
extern u32 lbl_8044F430;
extern u32 lbl_8044F42C;
extern u8 *lbl_8044F428;
extern u32 lbl_8044F424;

void fn_80023780(u32 arg0) {
    lbl_8044F434 = OSAllocFromHeap(__OSCurrHeap, arg0);
    M2C_FIELD(lbl_8044F434, s32 *, 0) = 0;
    M2C_FIELD(lbl_8044F434, s32 *, 4) = (s32) (arg0 - 0x20);
    M2C_FIELD(lbl_8044F434, s32 *, 0xC) = 0;
    lbl_8044F430 = arg0 - 0x20;
    lbl_8044F42C = arg0;
    lbl_8044F428 = &lbl_8044F434[arg0];
    lbl_8044F424 = arg0;
}
