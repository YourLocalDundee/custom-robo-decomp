/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/os/OSL2.h */ #ifndef _DOLPHIN_OSL2_H_
#define _DOLPHIN_OSL2_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void L2Enable(void);
void L2Disable(void);
void L2GlobalInvalidate(void);
void L2SetDataOnly(BOOL dataOnly);
void L2SetWriteThrough(BOOL writeThrough);

#ifdef __cplusplus
}
#endif

#endif
