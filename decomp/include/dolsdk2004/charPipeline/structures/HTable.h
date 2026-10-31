/* Custom Robo: from doldecomp/dolsdk2004 @ 2328b4164b1a98422a2255d83ce9a5a7548990cc, include/charPipeline/structures/HTable.h */ #ifndef _CHARPIPELINE_STRUCTURES_HTABLE_H_
#define _CHARPIPELINE_STRUCTURES_HTABLE_H_

#include <charPipeline/structures/List.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef u16 (DSHashFunc)(Ptr);

typedef struct {
    DSList* table;
    u16 tableSize;
    DSHashFunc* hash;
} DSHashTable;

void DSInitHTable(DSHashTable* hTable, u16 size, DSList* listArray, DSHashFunc* hashFunc, Ptr obj, DSLinkPtr link);
void DSInsertHTableObj(DSHashTable* hTable, Ptr obj);
void DSHTableToList(DSHashTable* hTable, DSList* list);
void* DSNextHTableObj(DSHashTable* hTable, Ptr obj);
s32 DSHTableIndex(DSHashTable* hTable, Ptr obj);
void* DSHTableHead(DSHashTable* hTable, s32 index);

#ifdef __cplusplus
}
#endif

#endif
