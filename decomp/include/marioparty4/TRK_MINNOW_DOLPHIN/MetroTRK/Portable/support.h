/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/TRK_MINNOW_DOLPHIN/MetroTRK/Portable/support.h */ #ifndef METROTRK_PORTABLE_SUPPORT_H
#define METROTRK_PORTABLE_SUPPORT_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

DSError TRKSuppAccessFile(u32 file_handle, u8* data, size_t* count,
                          DSIOResult* io_result, BOOL need_reply, BOOL read);

DSError TRKRequestSend(TRKBuffer* msgBuf, int* bufferId, u32 p1, u32 p2,
                       int p3);

DSError HandleOpenFileSupportRequest(const char* path, u8 replyError,
                                     u32* param_3, u8* ioResult);

DSError HandleCloseFileSupportRequest(int replyError, u8* ioResult);

DSError HandlePositionFileSupportRequest(u32 replyErr, u32* param_2,
                                         u8 param_3, u8* ioResult);

#ifdef __cplusplus
}
#endif

#endif /* METROTRK_PORTABLE_SUPPORT_H */
