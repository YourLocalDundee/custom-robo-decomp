/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/dolphin/include/dolphin/os/OSSemaphore.h */ #ifndef _DOLPHIN_OSSEMAPHORE_H_
#define _DOLPHIN_OSSEMAPHORE_H_

#include <dolphin/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSSemaphore {
    s32 count;
    OSThreadQueue queue;
} OSSemaphore;

void OSInitSemaphore(OSSemaphore* sem, s32 count);
s32 OSWaitSemaphore(OSSemaphore* sem);
s32 OSTryWaitSemaphore(OSSemaphore* sem);
s32 OSSignalSemaphore(OSSemaphore* sem);
s32 OSGetSemaphoreCount(OSSemaphore* sem);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSSEMAPHORE_H_
