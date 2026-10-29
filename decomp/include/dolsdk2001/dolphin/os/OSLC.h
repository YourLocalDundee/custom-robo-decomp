/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/os/OSLC.h */ #ifndef _DOLPHIN_OSLC_H_
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
