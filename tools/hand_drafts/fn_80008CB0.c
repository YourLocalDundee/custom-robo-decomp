/* fn_80008CB0: Nov 14, 2026 hand draft: the texture palette loader typed (TEXPalette / descriptor pairs, the GXTexObj array at lbl_8038B430 + 0x2000, the load record); the descriptor test against ((void *) 0) (li r0, 0; cmplw); the size check after the archive call an assert without its message, (offset + size < 0x154000) ? (void) 0 : (void) 0, with the call inside it (a size local took a fifth saved register and stmw); the GXTexObj base set before the palette store (42%) */

typedef struct {
    void *header;
    void *clut;
} TexDesc08;

typedef struct {
    u32 version;
    u32 count;
    TexDesc08 *desc;
} TexPal08;

typedef struct {
    u32 unk0;
    s32 entry;
    s8 state;
    s8 index;
    u8 pad[2];
    u32 offset;
} TexLoad08;

void fn_80008CB0(TexLoad08 *arg0) {
    GXTlutObj tlut;
    TexPal08 **pal;
    u8 i;
    GXTexObj *obj;

    pal = (TexPal08 **) &lbl_80396490[4];
    obj = (GXTexObj *) &lbl_8038B430[0x2000];
    *pal = (TexPal08 *) (lbl_8044F2F8 + arg0->offset);
    fn_8008E3F8(*pal);
    for (i = 0; i < (*pal)->count; i++) {
        if ((*pal)->desc[i].clut == ((void *) 0)) {
            TEXGetGXTexObjFromPalette(*pal, &obj[i], i);
        } else {
            TEXGetGXTexObjFromPaletteCI(*pal, &obj[i], &tlut, 0, i);
        }
    }
    (arg0->offset + cr_Archive_GetEntrySize(arg0->entry, arg0->index) < 0x154000) ? (void) 0 : (void) 0;
    arg0->state = 4;
}
