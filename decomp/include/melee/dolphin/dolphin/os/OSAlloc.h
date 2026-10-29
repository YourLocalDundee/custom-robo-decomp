/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, libs/dolphin/include/dolphin/os/OSAlloc.h */ #ifndef _DOLPHIN_OSALLOC_H_
#define _DOLPHIN_OSALLOC_H_

#include <dolphin/types.h>

typedef int OSHeapHandle;

extern volatile OSHeapHandle __OSCurrHeap;

void* OSAllocFromHeap(int heap, unsigned long size);
void* OSAllocFixed(void* rstart, void* rend);
void OSFreeToHeap(int heap, void* ptr);
int OSSetCurrentHeap(int heap);
void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps);
int OSCreateHeap(void* start, void* end);
void OSDestroyHeap(int heap);
void OSAddToHeap(int heap, void* start, void* end);
long OSCheckHeap(int heap);
unsigned long OSReferentSize(void* ptr);
void OSDumpHeap(int heap);
void OSVisitAllocated(void (*visitor)(void*, unsigned long));

#define OSAlloc(size) OSAllocFromHeap(__OSCurrHeap, (size))
#define OSFree(ptr) OSFreeToHeap(__OSCurrHeap, (ptr))

#endif
