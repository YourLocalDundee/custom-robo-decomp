/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, include/TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msghndlr.h */ #ifndef METROTRK_PORTABLE_MSGHNDLR_H
#define METROTRK_PORTABLE_MSGHNDLR_H

#include "trk.h"

void SetTRKConnected(BOOL);
BOOL GetTRKConnected(void);
DSError TRKDoSetOption(TRKBuffer*);
DSError TRKDoStop(TRKBuffer*);
DSError TRKDoStep(TRKBuffer*);
DSError TRKDoContinue(TRKBuffer*);
DSError TRKDoWriteRegisters(TRKBuffer*);
DSError TRKDoReadRegisters(TRKBuffer*);
DSError TRKDoWriteMemory(TRKBuffer*);
DSError TRKDoReadMemory(TRKBuffer*);
DSError TRKDoSupportMask(TRKBuffer*);
DSError TRKDoVersions(TRKBuffer*);
DSError TRKDoOverride(TRKBuffer*);
DSError TRKDoReset(TRKBuffer*);
DSError TRKDoDisconnect(TRKBuffer*);
DSError TRKDoConnect(TRKBuffer*);

#endif /* METROTRK_PORTABLE_MSGHNDLR_H */
