/* cr_Archive_GetEntrySize: Nov 4, 2026 hand draft (m2c 78%, permuter 15): typed, the packed entry as &a->entries[entry] and the loose one as cr_g_FileDescs[archive][entry].size (m2c scaled the packed index twice) */
u32 cr_Archive_GetEntrySize(s32 archive, u16 entry) {
    cr_PackedArchive *a;
    cr_PackedEntry *e;

    if (cr_g_FileDescs[archive]->packed != NULL) {
        a = &cr_g_PackedArchives[archive];
        e = &a->entries[entry];
        return e->size;
    }
    return cr_g_FileDescs[archive][entry].size;
}
