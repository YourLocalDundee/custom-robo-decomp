/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, src/sysdolphin/baselib/objalloc.c */ #include "objalloc.h"

#include <string.h>

#include "initialize.h"
#include "memory.h"
#include <dolphin/os/OSAlloc.h>
s32 HSD_GetNbBits(u32 value); /* Custom Robo: signed here (cmpwi), as wowjinxy/KAR declares it */

/* Custom Robo: the memory callbacks of this revision (wowjinxy/KAR memory.c); the 0xC one keeps its address name */
void* _HSD_MemAlloc(u32 size, u32 align, u32 flags);
u32 fn_801A408C(void);
#define _HSD_MemGetRemain fn_801A408C

static objheap obj_heap = { 0, 0, -1, -1 };

static HSD_ObjAllocData* alloc_datas;

void HSD_ObjSetHeap(u32 size, void* ptr)
{
    obj_heap.curr = (uintptr_t) ptr;
    obj_heap.top = (uintptr_t) ptr;
    obj_heap.remain = size;
    obj_heap.size = size;
}

/* Custom Robo: this revision (= doldecomp/kar's objalloc.s, KAR PAL): data->align holds the alignment itself (rounded
   with align - 1), the pool comes from _HSD_MemAlloc(size, align, 1), the heap's remain is not reduced then */
s32 HSD_ObjAllocAddFree(HSD_ObjAllocData* data, u32 num)
{
    uintptr_t pool_end;
    uintptr_t pool_size;
    uintptr_t pool_start;

    HSD_ASSERT(0xEE, data);
    pool_size = data->size * num;
    if (obj_heap.top != 0) {
        pool_end = obj_heap.top + obj_heap.size;
        pool_start = (obj_heap.curr + (data->align - 1)) & ~(data->align - 1);
        if (pool_start > pool_end) {
            return 0;
        }
        if (pool_end - pool_start < pool_size) {
            pool_size =
                pool_end - pool_start - (pool_end - pool_start) % data->size;
        }
        num = pool_size / data->size;
        if (num == 0) {
            return 0;
        }
        obj_heap.curr = pool_start + pool_size;
        obj_heap.remain = pool_end - obj_heap.curr;
    } else {
        pool_start = (uintptr_t) _HSD_MemAlloc(pool_size, data->align, 1);
        if (pool_start == 0) {
            return 0;
        }
    }

    {
        int i;
        HSD_ObjAllocLink* link;
        for (i = 0; (unsigned) i < num - 1; i++) {
            link = (HSD_ObjAllocLink*) (pool_start + data->size * i);
            link->next =
                (HSD_ObjAllocLink*) (pool_start + data->size * (i + 1));
        }
        link = (HSD_ObjAllocLink*) (pool_start + data->size * i);
        link->next = data->freehead;
    }

    data->freehead = (HSD_ObjAllocLink*) pool_start;
    data->free += num;
    return num;
}

#pragma push
#pragma dont_inline on /* Custom Robo: main.dol calls HSD_ObjAllocAddFree here */
void* HSD_ObjAlloc(HSD_ObjAllocData* data)
{
    HSD_ObjAllocLink* cur;
    u32 size;

    if (data->num_limit_flag && data->used >= data->num_limit) {
        return NULL;
    }
    if (data->heap_limit_flag) {
        if (data->heap_limit_num == (unsigned) -1) {
            if (obj_heap.top != 0) {
                size = obj_heap.remain;
            } else {
                size = _HSD_MemGetRemain(); /* Custom Robo: wowjinxy/KAR objalloc.c */
            }
            if (size <= data->heap_limit_size) {
                data->heap_limit_num = data->used + data->free;
            }
        } else {
            if (obj_heap.top != 0) {
                size = obj_heap.remain;
            } else {
                size = _HSD_MemGetRemain(); /* Custom Robo: wowjinxy/KAR objalloc.c */
            }
            if (size > data->heap_limit_size) {
                data->heap_limit_num = -1;
            }
        }
        if (data->used >= data->heap_limit_num) {
            return NULL;
        }
    }
    if (data->free == 0) {
        HSD_ObjAllocAddFree(data, 1);
        if (data->free == 0) {
            return NULL;
        }
    }
    cur = data->freehead;
    data->freehead = cur->next;
    data->used += 1;
    data->free -= 1;
    if (data->used > data->peak) {
        data->peak = data->used;
    }
    return cur;
}

#pragma pop

void HSD_ObjFree(HSD_ObjAllocData* data, void* obj)
{
    HSD_ObjAllocLink* link = obj;
    link->next = data->freehead;
    data->freehead = link;
    data->free += 1;
    data->used -= 1;
}

/* Custom Robo: wowjinxy/KAR objalloc.c HSD_ObjAllocInit (cb94b15), Melee's names: this revision checks the alignment and
   stores it as it is; assert lines main.dol's */
void HSD_ObjAllocInit(HSD_ObjAllocData* data, size_t size, u32 align)
{
    HSD_ObjAllocData** cur;
    HSD_ObjAllocData* entry;

    HSD_ASSERT(0x182, data);
    if (data != NULL) {
        cur = &alloc_datas;
        while ((entry = *cur) != NULL) {
            if (entry == data) {
                *cur = entry->next;
            } else {
                cur = &entry->next;
            }
        }
    } else {
        alloc_datas = NULL;
    }
    memset(data, 0, sizeof(HSD_ObjAllocData));
    data->num_limit = -1;
    data->heap_limit_size = 0;
    data->heap_limit_num = -1;
    HSD_ASSERT(0x18D, align <= 32);
    HSD_ASSERT(0x18E, HSD_GetNbBits(align) == 1);
    data->align = align;
    data->size = (size + (data->align - 1)) & ~(data->align - 1);
    data->next = alloc_datas;
    alloc_datas = data;
}

#if 0 /* Custom Robo: not in main.dol (HSD_RObjInitAllocData follows HSD_ObjAllocInit) */
void _HSD_ObjAllocForgetMemory(void* low, void* high)
{
    alloc_datas = NULL;
}
#endif
