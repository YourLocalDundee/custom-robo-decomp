/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, include/TRK_MINNOW_DOLPHIN/MetroTRK/Portable/mutex_TRK.h */ #ifndef METROTRK_PORTABLE_MUTEX_TRK_H
#define METROTRK_PORTABLE_MUTEX_TRK_H

#include <types.h>

u8 TRKReleaseMutex();
u8 TRKAcquireMutex();
u8 TRKInitializeMutex();

#endif /* METROTRK_PORTABLE_MUTEX_TRK_H */
