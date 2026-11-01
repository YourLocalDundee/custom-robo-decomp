/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, src/sysdolphin/baselib/archive.c (HSD_ArchiveGetPublicAddress, HSD_ArchiveGetExtern, HSD_ArchiveLocateExtern) */ #include "archive.h"

#include <string.h>

#include <dolphin/os.h>

/* Custom Robo: the split archive.c (plan.md step 98) is .text 0x801A5558-0x801A5708, these three functions (Melee's
   range). HSD_ArchiveParse (with its static inline Locate) is not in main.dol's code; its "byte-order mismatch"
   string at .data 0x8024D268 belongs with the functions after this object (fn_801A5708 ...). */

void* HSD_ArchiveGetPublicAddress(HSD_Archive* archive, const char* symbols)
{
    u32 i;

    for (i = 0; i < archive->header.nb_public; i++) {
        int comparison =
            strcmp(archive->symbols + archive->public_info[i].symbol, symbols);

        if (comparison == 0) {
            // If both strings are equal, we've found the node
            return archive->data + archive->public_info[i].offset;
        }
    }

    return NULL;
}

char* HSD_ArchiveGetExtern(HSD_Archive* archive, int index)
{
    if (index < 0 || archive->header.nb_extern <= (u32) index) {
        return NULL;
    }

    return archive->symbols + archive->extern_info[index].symbol;
}

void HSD_ArchiveLocateExtern(HSD_Archive* archive, const char* symbols,
                             void* addr)
{
    u32 offset = -1U;
    u32 i;

    for (i = 0; i < archive->header.nb_extern; i++) {
        int comparison =
            strcmp(symbols, archive->symbols + archive->extern_info[i].symbol);

        if (comparison == 0) {
            offset = archive->extern_info[i].offset;
            break;
        }
    }

    if (offset == -1U) {
        return;
    }

    while (offset != -1U && offset < archive->header.data_size) {
        u32* slot = (u32*) (archive->data + offset);
        u32 next = *slot;
        *slot = (uintptr_t) addr;
        offset = next;
    }
}

