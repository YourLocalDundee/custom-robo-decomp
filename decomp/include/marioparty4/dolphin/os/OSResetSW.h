/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/dolphin/os/OSResetSW.h */ #ifndef _DOLPHIN_OSRESETSW
#define _DOLPHIN_OSRESETSW

#include <dolphin/os/OSContext.h>
#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OSResetCallback)(void);

BOOL OSGetResetButtonState(void);

BOOL OSGetResetSwitchState(void);
OSResetCallback OSSetResetCallback(OSResetCallback callback);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSRESETSW
