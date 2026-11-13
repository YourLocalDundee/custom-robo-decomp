/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Heap2_AllocAligned: .text 0x80023678-0x80023780 (264 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
struct cr_HeapBlk;
extern u8 *lbl_8044F434;
extern u32 lbl_8044F430;

/* cr_Heap2_AllocAligned: Nov 13, 2026 hand draft: first-fit split; the new block address is spelled (u8 *) b + b->used + 0x20 (left to right), the size rounded with ((u32) (n + 0x1F) >> 5) << 5 */

typedef struct cr_HeapBlk {
    s32 used;
    s32 size;
    u8 pad[4];
    struct cr_HeapBlk *next;
} cr_HeapBlk;

void *cr_Heap2_AllocAligned(s32 n) {
    cr_HeapBlk *b;
    cr_HeapBlk *nx;

    n = ((u32) (n + 0x1F) >> 5) << 5;
    for (b = (cr_HeapBlk *) lbl_8044F434; b->next != ((void *) 0); b = b->next) {
        if ((u32) (b->size - b->used) >= (u32) (n + 0x20)) {
            goto found;
        }
    }
    if ((u32) (b->size - b->used) < (u32) (n + 0x20)) {
        return ((void *) 0);
    }
found:
    if (b->next == ((void *) 0)) {
        b->next = (cr_HeapBlk *) ((u8 *) b + b->used + 0x20);
        b->next->next = ((void *) 0);
    } else {
        nx = b->next;
        b->next = (cr_HeapBlk *) ((u8 *) b + b->used + 0x20);
        b->next->next = nx;
    }
    b->next->size = b->size - (b->used + 0x20);
    b->next->used = n;
    b->size = b->used;
    lbl_8044F430 -= n + 0x20;
    return (u8 *) b->next + 0x20;
}
