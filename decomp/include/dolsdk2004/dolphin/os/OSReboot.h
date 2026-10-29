/* Custom Robo: from doldecomp/dolsdk2004 @ 2328b4164b1a98422a2255d83ce9a5a7548990cc, include/dolphin/os/OSReboot.h */ #ifndef _DOLPHIN_OSREBOOT_H_
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
