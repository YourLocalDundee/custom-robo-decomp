/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/TRK_MINNOW_DOLPHIN/MetroTRK/Portable/mem_TRK.h */ #ifndef METROTRK_PORTABLE_MEM_TRK_H
#define METROTRK_PORTABLE_MEM_TRK_H

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

void* TRK_memset(void* dst, int val, size_t n);
void* TRK_memcpy(void* dst, const void* src, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* METROTRK_PORTABLE_MEM_TRK_H */
