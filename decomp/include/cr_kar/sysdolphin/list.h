/* Custom Robo override of doldecomp/kar src/sysdolphin/list.h (step 61): found before include/kar (decomp/configure.py
 * CR_OBJECT_INCLUDES), so kar's list.c compiles unchanged. Same types as kar's header. Names: kar's list.c defines
 * HSD_SListAppend / HSD_SListPrepend (the wowjinxy map's names), its own header and Melee call them
 * HSD_SListAllocAndAppend / HSD_SListAllocAndPrepend, and symbols/symbols.txt has the Melee name at 0x801A3E18 (other
 * objects call it by that name): the two definitions are renamed to the Melee names here. Their return type is
 * kar's (void; the code is the same either way). */
#ifndef _list_h_
#define _list_h_

#include <sysdolphin/objalloc.h>

typedef struct _HSD_SList {
    struct _HSD_SList* next;
    void* data;
} HSD_SList;

typedef struct _HSD_DList {
    struct _HSD_DList* next;
    struct _HSD_DList* prev;
    void* data;
} HSD_DList;

#define HSD_SListAppend HSD_SListAllocAndAppend
#define HSD_SListPrepend HSD_SListAllocAndPrepend

void HSD_ListInitAllocData();
HSD_ObjAllocData *HSD_SListGetAllocData();
HSD_ObjAllocData *HSD_DListGetAllocData();
HSD_SList *HSD_SListAlloc();
void HSD_SListAllocAndAppend(HSD_SList *list, void *data);
#ifdef CR_LIST_PREPEND_RETURNS
/* a file that uses the result (aobj.c, step 68; CR_OBJECT_CFLAGS): it returns the new head, as Melee declares it */
HSD_SList *HSD_SListAllocAndPrepend(HSD_SList *list, void *data);
#else
void HSD_SListAllocAndPrepend(HSD_SList *list, void *data);
#endif
HSD_SList *HSD_SListAppendList(HSD_SList *list, HSD_SList *next);
HSD_SList *HSD_SListPrependList(HSD_SList *list, HSD_SList *prev);
HSD_SList *HSD_SListRemove(HSD_SList *list);

#endif
