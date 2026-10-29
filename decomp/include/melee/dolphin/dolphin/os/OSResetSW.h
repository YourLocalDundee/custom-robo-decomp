/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, libs/dolphin/include/dolphin/os/OSResetSW.h */ #ifndef _DOLPHIN_OSRESETSW_H_
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
