/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/gx/GXCull.h */ #ifndef _DOLPHIN_GX_GXCULL_H_
#define _DOLPHIN_GX_GXCULL_H_

#include <dolphin/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXSetScissor(u32 left, u32 top, u32 wd, u32 ht);
void GXSetCullMode(GXCullMode mode);
void GXSetCoPlanar(GXBool enable);

#ifdef __cplusplus
}
#endif

#endif
