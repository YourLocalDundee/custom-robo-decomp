/* Custom Robo: from doldecomp/dolsdk2004 @ 2328b4164b1a98422a2255d83ce9a5a7548990cc, include/dolphin/os/OSTimer.h */ #ifndef _DOLPHIN_OSTIMER_H_
#define _DOLPHIN_OSTIMER_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OSTimerCallback)(void);

OSTimerCallback OSSetTimerCallback(OSTimerCallback callback);
void OSInitTimer(u32 time, u32 mode);
void OSStartTimer(void);
void OSStopTimer(void);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSTIMER_H_
