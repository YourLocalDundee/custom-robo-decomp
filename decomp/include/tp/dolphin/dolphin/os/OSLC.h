/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/dolphin/include/dolphin/os/OSLC.h */ #ifndef _DOLPHIN_OSLC_H_
#define _DOLPHIN_OSLC_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void LCAllocOneTag(BOOL invalidate, void *tag);
void LCAllocTags(BOOL invalidate, void *startTag, u32 numBlocks);
void LCAlloc(void *addr, u32 nBytes);
void LCAllocNoInvalidate(void *addr, u32 nBytes);

#ifdef __cplusplus
}
#endif

#endif
