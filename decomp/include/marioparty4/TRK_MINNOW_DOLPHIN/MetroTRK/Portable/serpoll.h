/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/TRK_MINNOW_DOLPHIN/MetroTRK/Portable/serpoll.h */ #ifndef METROTRK_PORTABLE_SERPOLL_H
#define METROTRK_PORTABLE_SERPOLL_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"

#ifdef __cplusplus
extern "C" {
#endif

DSError TRKInitializeSerialHandler(void);
DSError TRKTerminateSerialHandler(void);

void TRKGetInput(void);
MessageBufferID TRKTestForPacket();
void TRKProcessInput(int bufferIdx);

extern void* gTRKInputPendingPtr;

#ifdef __cplusplus
}
#endif

#endif /* METROTRK_PORTABLE_SERPOLL_H */
