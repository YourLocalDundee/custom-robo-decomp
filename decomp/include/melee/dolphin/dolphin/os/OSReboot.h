/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, libs/dolphin/include/dolphin/os/OSReboot.h */ #ifndef _DOLPHIN_OSREBOOT_H_
#define _DOLPHIN_OSREBOOT_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*RunCallback)(void);

void Run(RunCallback);
void __OSReboot(u32 resetCode, u32 bootDol);

#ifdef __cplusplus
}
#endif

#endif
