/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/dolphin/os/OSResetSW.h */ #ifndef _DOLPHIN_OSRESETSW_H_
#define _DOLPHIN_OSRESETSW_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OSResetCallback)(void);

OSResetCallback OSSetResetCallback(OSResetCallback callback);
BOOL OSGetResetSwitchState();
BOOL OSGetResetButtonState(void);

#ifdef __cplusplus
}
#endif

#endif
