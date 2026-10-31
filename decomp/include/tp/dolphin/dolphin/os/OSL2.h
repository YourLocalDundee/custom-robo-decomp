/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/dolphin/include/dolphin/os/OSL2.h */ #ifndef _DOLPHIN_OSL2_H_
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
