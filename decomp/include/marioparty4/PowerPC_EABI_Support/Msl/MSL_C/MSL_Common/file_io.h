/* Custom Robo: from mariopartyrd/marioparty4 @ 5b01c79b4910525e3fc5aa312eddd6d014e7f337, include/PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/file_io.h */ #ifndef _MSL_COMMON_FILE_IO_H
#define _MSL_COMMON_FILE_IO_H

#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/ansi_files.h"

#ifdef __cplusplus
extern "C" {
#endif

int __msl_strnicmp(const char* str1, const char* str2, int n);
int fflush(FILE* file);
int fclose(FILE* file);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_FILE_IO_H */
