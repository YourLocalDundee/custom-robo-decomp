/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80074FBC: .text 0x80074FBC-0x800750F8 (316 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void cr_Mem_CopyWords(void *dst, void *src, u32 bytes);

/* fn_80074FBC: Nov 5, 2026 hand draft (m2c 77%): lays a loaded file out from its 0x20-byte header (copied into the 0x44-byte record arg1): data, relocation table, two 8-byte tables and the rest one after another (an offset that grows by each size; a NULL place ends it), then the relocations. A zero local (r25) for the +0x40 store; each size computed before its pointer is stored (the other order gives 88%) */
typedef struct {
    u32 size; s32 x4; u32 reloc_count; u32 count_c; u32 count_10; u32 pad14[3];
    u8 *data; u32 *relocs; void *tbl_c; void *tbl_10; void *rest; u32 pad34[3]; s32 x40;
} cr_FileHeader44;

void fn_80074FBC(u8 *src, cr_FileHeader44 *hdr) {
    s32 off;
    s32 size;
    u32 i;
    u8 **p;
    s32 zero;

    zero = 0;
    off = 0;
    cr_Mem_CopyWords(hdr, src, 0x20);
    off += 0x20;
    hdr->x40 = zero;
    hdr->data = src + off;
    size = hdr->x4;
    off += size;
    if (hdr->reloc_count != 0) {
        size = hdr->reloc_count * 4;
        hdr->relocs = (u32 *) (src + off);
        if (hdr->relocs == NULL) {
            return;
        }
        off += size;
    }
    if (hdr->count_c != 0) {
        size = hdr->count_c * 8;
        hdr->tbl_c = src + off;
        if (hdr->tbl_c == NULL) {
            return;
        }
        off += size;
    }
    if (hdr->count_10 != 0) {
        size = hdr->count_10 * 8;
        hdr->tbl_10 = src + off;
        if (hdr->tbl_10 == NULL) {
            return;
        }
        off += size;
    }
    size = hdr->size - off;
    if (size != 0) {
        hdr->rest = src + off;
        if (hdr->rest == NULL) {
            return;
        }
    }
    for (i = 0; i < hdr->reloc_count; ) {
        p = (u8 **) (hdr->data + hdr->relocs[i++]);
        *p += (u32) hdr->data;
    }
}
