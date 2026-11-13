/* cr_Archive_SetEntryTable: Nov 13, 2026 hand draft: m2c 47%: the archive record and the count named locals (r31 / r30), table advanced past its header (`table += 2`), the 16-bit count stored through a u16 lvalue (the struct field is s16 in our types; the original zero-extends), returns 0 */

s32 cr_Archive_SetEntryTable(s32 archive, s32 dvd_entry, u32 *table) {
    cr_PackedArchive *a = &cr_g_PackedArchives[archive];
    u32 n = *table;
    table += 2;

    a->unk_00 = 0;
    a->entry_count = n;
    *(u16 *) &a->entry_count16 = n;
    a->unk_0e = 0;
    a->dvd_entry = dvd_entry;
    a->entries = (cr_PackedEntry *) table;
    return 0;
}
