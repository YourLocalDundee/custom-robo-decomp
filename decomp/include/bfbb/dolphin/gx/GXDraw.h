/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/dolphin/include/dolphin/gx/GXDraw.h */ #ifndef _DOLPHIN_GXDRAW
#define _DOLPHIN_GXDRAW

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXDrawSphere(u8 numMajor, u8 numMinor);
void GXDrawCylinder(u8 numEdges);
void GXDrawCube();

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_GXDRAW
