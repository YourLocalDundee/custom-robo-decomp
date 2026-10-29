/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/TRK_MINNOW_DOLPHIN/MetroTRK/Portable/mutex_TRK.h */ #ifndef METROTRK_PORTABLE_MUTEX_TRK_H
#define METROTRK_PORTABLE_MUTEX_TRK_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"

DSError TRKInitializeMutex(void*);
DSError TRKAcquireMutex(void*);
DSError TRKReleaseMutex(void*);

#endif /* METROTRK_PORTABLE_MUTEX_TRK_H */
