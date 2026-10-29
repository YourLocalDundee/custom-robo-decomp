/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/wchar_io.h */ #ifndef _MSL_COMMON_WCHAR_IO_H
#define _MSL_COMMON_WCHAR_IO_H

#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/ansi_files.h"

#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif

int fwide(FILE* file, int mode);

#endif /* _MSL_COMMON_WCHAR_IO_H */
