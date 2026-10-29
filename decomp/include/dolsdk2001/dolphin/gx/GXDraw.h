/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/gx/GXDraw.h */ #ifndef _DOLPHIN_GX_GXDRAW_H_
#define _DOLPHIN_GX_GXDRAW_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXDrawCylinder(u8 numEdges);
void GXDrawTorus(f32 rc, u8 numc, u8 numt);
void GXDrawSphere(u8 numMajor, u8 numMinor);
void GXDrawCube(void);
void GXDrawDodeca(void);
void GXDrawOctahedron(void);
void GXDrawIcosahedron(void);
void GXDrawSphere1(u8 depth);
u32 GXGenNormalTable(u8 depth, f32 *table);

#ifdef __cplusplus
}
#endif

#endif
