/* fn_8007571C: Nov 14, 2026 hand draft: the battle archives reload: the file's .bss pool from lbl_80377E60 defined (a 5x4 pointer table, four 4-pointer tables, four arrays of four 0x44-byte archives at 0x90 / 0x1A0 / 0x2B0 / 0x3C0: each addressed with its own addi, so separate objects although the symbol table has one 0x450 block), the buffers stepped as (k * size) / 4, the dead mask kept; the inner counter declared before the outer one (the original's r28 / r27) (46%) */

// also defines: lbl_80377E60 lbl_80377EB0 lbl_80377EC0 lbl_80377ED0 lbl_80377EE0

typedef struct {
    u8 b[0x44];
} Arc571;

extern u8 *lbl_8044F2E4;

/* the file's .bss from its first object (lbl_80377E60): scoring only (tools/pool_prelude.py) */
void *lbl_80377E60[5][4];
void *lbl_80377EB0[4];
void *lbl_80377EC0[4];
void *lbl_80377ED0[4];
void *lbl_80377EE0[4];
Arc571 cr_s_7E60_0090[4];
Arc571 cr_s_7E60_01A0[4];
Arc571 cr_s_7E60_02B0[4];
Arc571 cr_s_7E60_03C0[4];
extern void cr_Pool_Use(void *);
void cr_Pool80377E60_Order(void) {
    cr_Pool_Use(lbl_80377E60);
    cr_Pool_Use(lbl_80377EB0);
    cr_Pool_Use(lbl_80377EC0);
    cr_Pool_Use(lbl_80377ED0);
    cr_Pool_Use(lbl_80377EE0);
    cr_Pool_Use(cr_s_7E60_0090);
    cr_Pool_Use(cr_s_7E60_01A0);
    cr_Pool_Use(cr_s_7E60_02B0);
    cr_Pool_Use(cr_s_7E60_03C0);
}

void fn_8007571C(void) {
    s32 j;
    s32 i;
    s32 k;
    u8 *p;
    u32 mask;

    i = 0;
    mask = lbl_8044F4FC;
    while (i < 5) {
        j = 0;
        while (j < 4) {
            fn_80021014(lbl_80377E60[i][j]);
            j++;
        }
        i++;
    }
    i = 0;
    while (i < 5) {
        j = 0;
        while (j < 4) {
            fn_800215F8(lbl_80377E60[i][j]);
            j++;
        }
        i++;
    }
    for (k = 0; k < 4; k++) {
        p = lbl_8044F2E4 + (s32) (k * 0x16000 * 4) / 4;
        fn_80074FBC(p + *(s32 *) (p + 4), &cr_s_7E60_03C0[k]);
        p = lbl_8044F2E0 + (s32) (k * 0x40000 * 4) / 4;
        fn_80074FBC(p + *(s32 *) (p + 4), &cr_s_7E60_02B0[k]);
        p = lbl_8044F2DC + (s32) (k * 0x40000 * 4) / 4;
        fn_80074FBC(p + *(s32 *) (p + 4), &cr_s_7E60_01A0[k]);
        p = lbl_8044F2D8 + (s32) (k * 0x14000 * 4) / 4;
        fn_80074FBC(p + *(s32 *) (p + 4), &cr_s_7E60_0090[k]);
        lbl_80377EE0[k] = HSD_ArchiveGetPublicAddress((HSD_Archive *) &cr_s_7E60_03C0[k], lbl_801C0B80);
        lbl_80377EC0[k] = HSD_ArchiveGetPublicAddress((HSD_Archive *) &cr_s_7E60_02B0[k], lbl_801C0B80);
        lbl_80377ED0[k] = HSD_ArchiveGetPublicAddress((HSD_Archive *) &cr_s_7E60_01A0[k], lbl_801C0B80);
        lbl_80377EB0[k] = HSD_ArchiveGetPublicAddress((HSD_Archive *) &cr_s_7E60_0090[k], lbl_801C0B80);
        mask ^= 0x80000000 >> k;
    }
}
