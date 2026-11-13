/* cr_CharaEntry_OnLoaded: hand-written from main.dol's asm (step 109b): the relocation loop with byte pointers and the header's pointer members added as words (m2c stepped the struct pointer: it did not compile) */

// lbl_8044F40C: the drafts type it differently (CR_MIXED), so the context declares nothing (Nov 13, 2026)
extern u8 *lbl_8044F40C;
void cr_CharaEntry_OnLoaded(void) {
    cr_CharaEntryHeader *hdr;
    u32 i;
    u8 *base;
    u32 *relocs;
    u32 *slot;

    i = 0;
    hdr = *(cr_CharaEntryHeader **) (lbl_8044F40C + 8);
    base = *(u8 **) (lbl_8044F40C + 8) + hdr->data_base;
    relocs = (u32 *) (*(u8 **) (lbl_8044F40C + 8) + hdr->reloc_offset);
    while (i < hdr->reloc_count) {
        slot = (u32 *) (base + relocs[i++]);
        *slot += (u32) base;
    }
    *(u32 *) &hdr->model_desc += (u32) base;
    *(u32 *) &hdr->joint_paths += *(u32 *) (lbl_8044F40C + 8);
    *(u32 *) &hdr->material_path += *(u32 *) (lbl_8044F40C + 8);
    *(u32 *) &hdr->limb_params += *(u32 *) (lbl_8044F40C + 8);
    cr_g_CharaEntries[*(s32 *) (lbl_8044F40C + 0x10)].entry = *(cr_CharaEntryHeader **) (lbl_8044F40C + 8);
    cr_g_CharaEntries[*(s32 *) (lbl_8044F40C + 0x10)].loaded = 1;
}
