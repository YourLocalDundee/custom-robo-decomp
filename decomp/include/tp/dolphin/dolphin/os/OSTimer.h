/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/dolphin/include/dolphin/os/OSTimer.h */ #ifndef _DOLPHIN_OSTIMER_H_
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
