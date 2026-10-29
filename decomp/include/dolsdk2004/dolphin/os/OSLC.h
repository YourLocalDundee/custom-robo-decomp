/* Custom Robo: from doldecomp/dolsdk2004 @ 2328b4164b1a98422a2255d83ce9a5a7548990cc, include/dolphin/os/OSLC.h */ #ifndef _DOLPHIN_OSLC_H_
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
