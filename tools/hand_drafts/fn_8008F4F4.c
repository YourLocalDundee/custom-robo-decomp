/* fn_8008F4F4: hand-written from main.dol's asm on its m2c draft (step 109b): the palette slot tested as a pointer against ((void *) 0) (li r0, 0; cmplw) instead of a u32 against 0 */
void fn_8008F4F4(void *arg0, void *arg1) {
    GXTlutObj sp8;
    void *t;

    if (*(void **) &((u8 (*)[0x8]) (M2C_FIELD(arg1, s32 *, 8)))[M2C_FIELD(arg0, u16 *, 0x10)][0x4] != ((void *) 0)) {
        t = (*(void **) &((u8 (*)[0x8]) (M2C_FIELD(arg1, s32 *, 8)))[M2C_FIELD(arg0, u16 *, 0x10)][0x4]);
        GXInitTlutObj(&sp8, M2C_FIELD(t, void **, 8), M2C_FIELD(t, enum _GXTlutFmt *, 4), M2C_FIELD(t, u16 *, 0));
        GXLoadTlut(&sp8, 0U);
    }
}
