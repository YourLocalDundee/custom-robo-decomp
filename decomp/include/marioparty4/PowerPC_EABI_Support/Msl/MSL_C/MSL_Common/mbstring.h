/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/mbstring.h */ #ifndef _MSL_COMMON_MBSTRING_H
#define _MSL_COMMON_MBSTRING_H

#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/wchar_io.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t wcstombs(char* dst, const wchar_t* src, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_MBSTRING_H */
