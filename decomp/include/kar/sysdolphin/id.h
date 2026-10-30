/* Custom Robo: from doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e, src/sysdolphin/id.h */ #ifndef _id_h_
#define _id_h_

#include "global.h"

#include "objalloc.h"

typedef struct _IDEntry {
    struct _IDEntry* next;
    u32 id;
    void* data;
} IDEntry;

typedef struct _HSD_IDTable {
    struct _IDEntry* table[101];
} HSD_IDTable;

HSD_ObjAllocData* HSD_IDGetAllocData(void);
void HSD_IDInitAllocData(void);
void HSD_IDSetup(void);
void HSD_IDInsertToTable(HSD_IDTable* table, u32 id, void* data);
void HSD_IDRemoveByIDFromTable(HSD_IDTable* table, u32 id);
void* HSD_IDGetDataFromTable(HSD_IDTable* table, u32 id, s32* success);
void _HSD_IDForgetMemory(void);

#endif
