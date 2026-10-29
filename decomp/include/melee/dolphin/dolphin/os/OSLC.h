/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, libs/dolphin/include/dolphin/os/OSLC.h */ #ifndef _DOLPHIN_OSLC_H_
#define _DOLPHIN_OSLC_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void LCAllocOneTag(BOOL invalidate, void* tag);
void LCAllocTags(BOOL invalidate, void* startTag, u32 numBlocks);
void LCAlloc(void* addr, u32 nBytes);
void LCAllocNoInvalidate(void* addr, u32 nBytes);

#ifdef __cplusplus
}
#endif

#endif
