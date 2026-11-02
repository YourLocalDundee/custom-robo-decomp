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


#include <dolphin/dvd.h>
void* HSD_Alloc(u32 size);
void HSD_Free(void* ptr);

void HSD_ArchiveFree(HSD_Archive* archive);

static void ArchiveParseString(void)
{
    OSReport("HSD_ArchiveParse: byte-order mismatch! Please check data format\n");
}

HSD_Archive* fn_801A5708(char* name)
{
    DVDFileInfo fi;
    HSD_Archive* archive;
    u32 size;
    u32 alloc;
    u32 offset;
    u32 i;
    size_t len;

    OSReport("Trying to open file %s\n", name);
    if (!DVDOpen(name, &fi)) {
        OSReport("Cannot open file.\n");
        return NULL;
    }
    archive = HSD_Alloc(sizeof(HSD_Archive));
    if (archive == NULL) {
        goto error;
    }
    memset(archive, 0, sizeof(HSD_Archive));
    if ((u32) DVDReadPrio(&fi, archive, 0x20, 0, 2) != 0x20) {
        OSReport("Error occured when reading file (header).");
        goto error;
    }
    if (archive->header.file_size != fi.length) {
        OSReport("Unexpected binary file size. Broken file or binary order mismatch\n");
        goto error;
    }
    size = archive->header.data_size;
    alloc = OSRoundUp32B(size);
    archive->data = HSD_Alloc(alloc);
    if (archive->data == NULL) {
        goto error;
    }
    archive->top_ptr = NULL;
    if (DVDReadPrio(&fi, archive->data, alloc, 0x20, 2) < size) {
        OSReport("Error occured when reading file (data body).");
        goto error;
    }
    offset = size + 0x20;
    if (archive->header.nb_reloc != 0) {
        size = archive->header.nb_reloc * 4;
        alloc = OSRoundUp32B(size);
        archive->reloc_info = HSD_Alloc(alloc);
        if (archive->reloc_info == NULL) {
            goto error;
        }
        if (DVDReadPrio(&fi, archive->reloc_info, alloc, offset, 2) < size) {
            OSReport("Error occured when reading file (reloc table).");
            goto error;
        }
        offset += size;
    }
    if (archive->header.nb_public != 0) {
        size = archive->header.nb_public * 8;
        alloc = OSRoundUp32B(size);
        archive->public_info = HSD_Alloc(alloc);
        if (archive->public_info == NULL) {
            goto error;
        }
        if (DVDReadPrio(&fi, archive->public_info, alloc, offset, 2) < size) {
            OSReport("Error occured when reading file (public table).");
            goto error;
        }
        offset += size;
    }
    if (archive->header.nb_extern != 0) {
        size = archive->header.nb_extern * 8;
        alloc = OSRoundUp32B(size);
        archive->extern_info = HSD_Alloc(alloc);
        if (archive->extern_info == NULL) {
            goto error;
        }
        if (DVDReadPrio(&fi, archive->extern_info, alloc, offset, 2) < size) {
            OSReport("Error occured when reading file (extern table).");
            goto error;
        }
        offset += size;
    }
    size = archive->header.file_size - offset;
    if (size != 0) {
        alloc = OSRoundUp32B(size);
        archive->symbols = HSD_Alloc(alloc);
        if (archive->symbols == NULL) {
            goto error;
        }
        if (DVDReadPrio(&fi, archive->symbols, alloc, offset, 2) < size) {
            OSReport("Error occured when reading file (symbol).");
            goto error;
        }
    }
    len = strlen(name);
    archive->name = HSD_Alloc(len + 1);
    if (archive->name == NULL) {
        goto error;
    }
    memcpy(archive->name, name, len + 1);
    for (i = 0; i < archive->header.nb_reloc; i++) {
        *(u32*) (archive->data + archive->reloc_info[i].offset) += (u32) archive->data;
    }
    return archive;

error:
    DVDClose(&fi);
    if (archive != NULL) {
        HSD_ArchiveFree(archive);
    }
    return NULL;
}

void HSD_ArchiveFree(HSD_Archive* archive)
{
    if (archive != NULL) {
        if (!(archive->flags & HSD_ARCHIVE_DONT_FREE)) {
            if (archive->data != NULL) {
                HSD_Free(archive->data);
            }
            if (archive->reloc_info != NULL) {
                HSD_Free(archive->reloc_info);
            }
            if (archive->public_info != NULL) {
                HSD_Free(archive->public_info);
            }
            if (archive->extern_info != NULL) {
                HSD_Free(archive->extern_info);
            }
            if (archive->symbols != NULL) {
                HSD_Free(archive->symbols);
            }
            if (archive->name != NULL) {
                HSD_Free(archive->name);
            }
        }
        HSD_Free(archive);
    }
}
