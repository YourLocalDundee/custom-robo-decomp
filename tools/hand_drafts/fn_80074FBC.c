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
