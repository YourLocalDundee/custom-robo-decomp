/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, libs/dolphin/include/dolphin/gx/GXDraw.h */ #ifndef _DOLPHIN_GX_GXDRAW_H_
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
u32 GXGenNormalTable(u8 depth, f32* table);

#ifdef __cplusplus
}
#endif

#endif
