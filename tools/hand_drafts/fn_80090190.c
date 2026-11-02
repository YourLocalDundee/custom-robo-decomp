/* fn_80090190: hand-written from main.dol's asm on its m2c draft (step 109b): the table slot tested as a pointer against ((void *) 0) (li r0, 0; cmplw), and an empty debug check of the archive offset after the add (lis r0, 1; cmplw, no branch) */
extern u8 *lbl_8044F6B0;

void fn_80090190(void) {
    GXTlutObj sp8;
    u16 var_r31_19;

    M2C_FIELD(lbl_8021A940, u16 *, 4) = 0xAU;
    M2C_FIELD(lbl_8021A940, u8 **, 8) = &lbl_8044F2F8[lbl_8044F68C];
    cr_Archive_OpenRequestList(lbl_8021A940);
    var_r31_19 = 0;
    while (var_r31_19 < (u32) M2C_FIELD(lbl_8044F6B0, u32 *, 4)) {
        if (*(void **) &((u8 (*)[0x8]) (M2C_FIELD(lbl_8044F6B0, s32 *, 8)))[var_r31_19][0x4] == ((void *) 0)) {
            TEXGetGXTexObjFromPalette(lbl_8044F6B0, (GXTexObj *) &lbl_80387E84[(var_r31_19 << 5) & 0x1FFFE0], (s32) var_r31_19);
        } else {
            TEXGetGXTexObjFromPaletteCI(lbl_8044F6B0, (GXTexObj *) &lbl_80387E84[(var_r31_19 << 5) & 0x1FFFE0], &sp8, 0, (s32) var_r31_19);
        }
        var_r31_19 += 1;
    }
    lbl_8044F68C += cr_Archive_GetEntrySize(0xD, M2C_FIELD(lbl_8021A940, u16 *, 4));
    if (lbl_8044F68C > 0x10000) {
        (void) 0;
    }
}
