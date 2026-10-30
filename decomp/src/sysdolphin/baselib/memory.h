/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, src/sysdolphin/baselib/memory.h */ #ifndef _memory_h_
#define _memory_h_

#include <Runtime/platform.h>

void HSD_Free(void* ptr);
void* HSD_MemAlloc(ssize_t size);

#endif
