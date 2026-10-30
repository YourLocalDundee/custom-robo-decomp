/* Custom Robo: from doldecomp/dolsdk2004 @ 2328b4164b1a98422a2255d83ce9a5a7548990cc, src/axfx/__axfx.h */ #ifndef _DOLPHIN_AX_INTERNAL_H_
#define _DOLPHIN_AX_INTERNAL_H_

#include <dolphin/axfx.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void* (*__AXFXAlloc)(u32);
extern void (*__AXFXFree)(void*);

#ifdef __cplusplus
}
#endif

#endif
