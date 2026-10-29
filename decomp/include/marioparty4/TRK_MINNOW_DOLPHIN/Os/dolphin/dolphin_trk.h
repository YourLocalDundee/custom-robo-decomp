/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/TRK_MINNOW_DOLPHIN/Os/dolphin/dolphin_trk.h */ #ifndef OS_DOLPHIN_DOLPHIN_TRK_H
#define OS_DOLPHIN_DOLPHIN_TRK_H

#include "dolphin/types.h"
#include "PowerPC_EABI_Support/MetroTRK/trk.h"

#ifdef __cplusplus
extern "C" {
#endif

DSError TRKInitializeTarget();

void EnableMetroTRKInterrupts();
u32 TRKTargetTranslate(u32 param_0);
void TRK__read_aram(register int c, register u32 p2, void* p3);
void TRK__write_aram(register int c, register u32 p2, void* p3);

void __TRK_copy_vectors(void);

#ifdef __cplusplus
}
#endif

#endif /* OS_DOLPHIN_DOLPHIN_TRK_H */
