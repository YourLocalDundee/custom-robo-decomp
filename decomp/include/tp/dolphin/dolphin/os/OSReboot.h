/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/dolphin/include/dolphin/os/OSReboot.h */ #ifndef _DOLPHIN_OSREBOOT_H_
#define _DOLPHIN_OSREBOOT_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void OSSetSaveRegion(void* start, void* end);
void OSGetSaveRegion(void** start, void** end);
void OSGetSavedRegion(void** start, void** end);
void __OSReboot(u32 resetCode, u32 bootDol);

#ifdef __cplusplus
}
#endif

#endif
