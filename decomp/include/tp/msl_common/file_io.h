/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include/file_io.h */ #ifndef _MSL_COMMON_FILE_IO_H
#define _MSL_COMMON_FILE_IO_H

#include <ansi_files.h>

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
