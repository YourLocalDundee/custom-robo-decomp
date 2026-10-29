/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/os/OSAlloc.h */ #ifndef _DOLPHIN_OSALLOC_H_
#define _DOLPHIN_OSALLOC_H_

#include <dolphin/types.h>

typedef int OSHeapHandle;

extern volatile OSHeapHandle __OSCurrHeap;

void * OSAllocFromHeap(int heap, unsigned long size);
void * OSAllocFixed(void * rstart, void * rend);
void OSFreeToHeap(int heap, void * ptr);
int OSSetCurrentHeap(int heap);
void * OSInitAlloc(void * arenaStart, void * arenaEnd, int maxHeaps);
int OSCreateHeap(void * start, void * end);
void OSDestroyHeap(int heap);
void OSAddToHeap(int heap, void * start, void * end);
long OSCheckHeap(int heap);
unsigned long OSReferentSize(void * ptr);
void OSDumpHeap(int heap);
void OSVisitAllocated(void (* visitor)(void *, unsigned long));

#define OSAlloc(size) OSAllocFromHeap(__OSCurrHeap, (size))
#define OSFree(ptr)   OSFreeToHeap(__OSCurrHeap, (ptr))

#endif
