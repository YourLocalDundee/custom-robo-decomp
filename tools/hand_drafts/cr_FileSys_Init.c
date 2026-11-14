/* cr_FileSys_Init: Nov 13, 2026 hand draft: the file system file's .bss pool (base cr_g_EntryTableLoad): the loader's 0xC bytes, its 0x10-byte record, the directory tree head (0x18 bytes) and the 0x2B entry numbers are separate objects, defined here at their offsets (scoring only); returns 0; the two ticks are dead stack locals; the single-file test compares the packed pointer as a number */

// also defines: cr_g_EntryTableLoad cr_g_DirTree cr_g_FileSysReady
extern u8 *lbl_8044F3FC;

typedef struct {
    u8 pad[8];
    s32 a;
    s32 b;
} LoadSub20168;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Node20168;

typedef struct {
    s32 unk0;
    Node20168 *root;
    u8 pad8[0x10];
} Tree20168;

/* the file system file's .bss from its first object (cr_g_EntryTableLoad): scoring only */
u8 cr_g_EntryTableLoad[0xC];
LoadSub20168 cr_s_0168_000C;
u8 cr_s_0168_001C[0xF00];
Tree20168 cr_g_DirTree;
s32 cr_s_0168_0F34[0x2B];
u8 cr_s_0168_0FE0[0xB6C];
cr_FileSysState cr_g_FileSysReady;
extern void cr_Pool_Use(void *);
void cr_Pool80320168_Order(void) {
    cr_Pool_Use(cr_g_EntryTableLoad);
    cr_Pool_Use(&cr_s_0168_000C);
    cr_Pool_Use(cr_s_0168_001C);
    cr_Pool_Use(&cr_g_DirTree);
    cr_Pool_Use(cr_s_0168_0F34);
    cr_Pool_Use(cr_s_0168_0FE0);
    cr_Pool_Use(&cr_g_FileSysReady);
}

s32 cr_FileSys_Init(void) {
    u32 t0;
    u32 t1;
    s32 i;
    cr_FileDesc *d;
    s32 e;

    t0 = OSGetTick();
    if (cr_g_FileSysReady.ready == 0) {
        for (i = 0; i < 0x2B; i++) {
            cr_s_0168_0F34[i] = -1;
        }
        cr_s_0168_000C.a = 0;
        cr_s_0168_000C.b = 0;
        fn_80066F58(cr_g_EntryTableLoad, lbl_801F12C0);
        lbl_8044F408 = 0;
        lbl_8044F400 = cr_Heap_AllocAligned(0x8000U);
        lbl_8044F3FC = lbl_8044F400;
        cr_g_DirTree.root = cr_Heap_AllocAligned(0x14U);
        cr_g_DirTree.root->unk0 = 0;
        cr_g_DirTree.root->unk4 = -1;
        cr_g_DirTree.root->unk8 = 0;
        cr_g_DirTree.root->unkC = 0;
        cr_g_DirTree.root->unk10 = 0;
        fn_800220C4((s8 *) lbl_80450470, &cr_g_DirTree, (s8 **) cr_g_DirTree.root);
        lbl_8044F404 = 0;
        cr_g_FileSysReady.unk_01[0] = lbl_8044F404;
        cr_g_FileSysReady.ready = 1;
        cr_g_FileSysReady.chunk_sectors = 0x80;
        cr_g_FileSysReady.queue_head = 0;
        cr_g_FileSysReady.queue_tail = 0;
        for (i = 0; i < 0x20; i++) {
            cr_g_FileSysReady.queue[i] = NULL;
        }
        for (i = 0; i < 0x2B; i++) {
            d = cr_g_FileDescs[i];
            if (d->packed != NULL) {
                cr_FileSys_RegisterPath(d);
                if ((u32) d->packed == 1) {
                    e = d->dvd_entry;
                    cr_s_0168_0F34[i] = e;
                } else {
                    cr_Archive_SetEntryTable(i, d->dvd_entry, (u32 *) d->packed);
                }
            } else {
                while (d->path != ((void *) 0)) {
                    cr_FileSys_RegisterPath(d);
                    d++;
                }
            }
        }
    }
    t1 = OSGetTick();
    return 0;
}
