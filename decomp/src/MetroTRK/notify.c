/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/TRK_MINNOW_DOLPHIN/debugger/embedded/MetroTRK/Portable/notify.c */ #include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/notify.h"
#include "trk.h"

DSError TRKDoNotifyStopped(MessageCommandID cmd) {
    int reqIdx;
    int bufIdx;
    TRKBuffer* msg;
    DSError err;
    DSError bufError;

    bufError = TRKGetFreeBuffer(&bufIdx, &msg);
    if ((err = bufError) == FALSE) {
        if (err == DS_NoError) {
            if (cmd == DSMSG_NotifyStopped) {
                TRKTargetAddStopInfo(msg);
            } else {
                TRKTargetAddExceptionInfo(msg);
            }
        }
        bufError = TRKRequestSend(msg, &reqIdx, 2, 3, 1);
        err = bufError;
        if (err == DS_NoError) {
            TRKReleaseBuffer(reqIdx);
        }
        TRKReleaseBuffer(bufIdx);
    }
    return err;
}
