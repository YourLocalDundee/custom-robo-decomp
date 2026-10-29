/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/gx/GXDispList.h */ #ifndef _DOLPHIN_GX_GXDISPLIST_H_
#define _DOLPHIN_GX_GXDISPLIST_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXBeginDisplayList(void *list, u32 size);
u32 GXEndDisplayList(void);
void GXCallDisplayList( void *list, u32 nbytes);

#ifdef __cplusplus
}
#endif

#endif
