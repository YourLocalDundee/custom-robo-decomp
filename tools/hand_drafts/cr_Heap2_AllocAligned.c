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
