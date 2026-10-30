/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/dolphin/include/dolphin/gx/GXCull.h */ #ifndef _DOLPHIN_GXCULL
#define _DOLPHIN_GXCULL

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXSetCullMode(GXCullMode mode);
void GXSetCoPlanar(GXBool enable);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_GXCULL
