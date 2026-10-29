/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/vi/vifuncs.h */ #ifndef _DOLPHIN_VIFUNCS_H_
#define _DOLPHIN_VIFUNCS_H_

#include <dolphin/types.h>
#include <dolphin/vi/vitypes.h>
#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb);
VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback cb);
void VIInit(void);
void VIWaitForRetrace(void);
void VIConfigure(GXRenderModeObj *rm);
void VIConfigurePan(u16 xOrg, u16 yOrg, u16 width, u16 height);
void VIFlush(void);
void VISetNextFrameBuffer(void *fb);
void VISetNextRightFrameBuffer(void *fb);
void VISetBlack(BOOL black);
void VISet3D(BOOL threeD);
u32 VIGetRetraceCount(void);
u32 VIGetNextField(void);
u32 VIGetCurrentLine(void);
u32 VIGetTvFormat(void);

#ifdef __cplusplus
}
#endif

#endif
