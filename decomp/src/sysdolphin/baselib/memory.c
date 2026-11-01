/* Custom Robo: from wowjinxy/KAR @ cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/memory.c (HSD_Free, HSD_Alloc) */ #include "memory.h"

#include <dolphin/types.h>

/* Custom Robo: the split memory.c (plan.md step 99) is .text 0x801A40EC-0x801A4154, these two functions; the rest of
   wowjinxy's memory.c (_HSD_MemSetCallbacks ... _HSD_MemCheckOwn, the callback state) is in the automatic object
   before it, so the state is main.dol's lbl_8044B768 there (HSD_MemCallbackState in wowjinxy/KAR) */
typedef struct _HSD_MemCallbacks {
    void* (*alloc)(u32 size, u32 align, u32 flags);
    void (*free)(void* ptr);
    void (*clear)(void);
    u32 (*get_remain)(void);
    s32 (*check_own)(void* ptr);
} HSD_MemCallbacks;

typedef struct _HSD_MemCallbackData {
    HSD_MemCallbacks callbacks;
    u32 unused;
} HSD_MemCallbackData;

extern HSD_MemCallbackData lbl_8044B768;
#define HSD_MemCallbackState lbl_8044B768

/* wowjinxy/KAR memory.c HSD_Free */
void HSD_Free(void* ptr)
{
    HSD_MemCallbackState.callbacks.free(ptr);
}

/* wowjinxy/KAR memory.c HSD_Alloc */
void* HSD_Alloc(u32 size)
{
    return HSD_MemCallbackState.callbacks.alloc(size, 0x20, 0);
}
