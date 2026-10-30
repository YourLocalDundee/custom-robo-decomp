/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, include/TRK_MINNOW_DOLPHIN/MetroTRK/Portable/nubinit.h */ #ifndef METROTRK_PORTABLE_NUBINIT_H
#define METROTRK_PORTABLE_NUBINIT_H

#include "trk.h"

#ifdef __cplusplus
extern "C" {
#endif

void TRKNubWelcome(void);
void TRKNubMainLoop(void);
DSError TRKTerminateNub(void);
DSError TRKInitializeNub(void);

extern BOOL gTRKBigEndian;

#ifdef __cplusplus
}
#endif

#endif /* METROTRK_PORTABLE_NUBINIT_H */
