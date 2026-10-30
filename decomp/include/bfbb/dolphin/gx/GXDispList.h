/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/dolphin/include/dolphin/gx/GXDispList.h */ #ifndef _DOLPHIN_GXDISPLIST
#define _DOLPHIN_GXDISPLIST

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXBeginDisplayList(void* list, u32 size);
u32 GXEndDisplayList(void);
void GXCallDisplayList(const void* list, u32 nbytes);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_GXDISPLIST
