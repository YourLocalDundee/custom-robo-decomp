/* Custom Robo: from doldecomp/dolsdk2004 @ 2328b4164b1a98422a2255d83ce9a5a7548990cc, include/dolphin/os/OSL2.h */ #ifndef _DOLPHIN_OSL2_H_
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
