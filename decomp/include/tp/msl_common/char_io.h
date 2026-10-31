/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include/char_io.h */ #ifndef _MSL_COMMON_CHAR_IO_H
#define _MSL_COMMON_CHAR_IO_H

#include <ansi_files.h>

#ifdef __cplusplus
extern "C" {
#endif

int fputs(const char* str, FILE* stream);
int __put_char(int c, FILE* stream);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_CHAR_IO_H */
