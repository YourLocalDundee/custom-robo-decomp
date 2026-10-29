/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/dolphin/gx/GXDispList.h */ #ifndef _DOLPHIN_GXDISPLIST
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
