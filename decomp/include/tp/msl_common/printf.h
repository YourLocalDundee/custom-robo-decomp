/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include/printf.h */ #ifndef _MSL_COMMON_PRINTF_H
#define _MSL_COMMON_PRINTF_H

#include <ansi_files.h>
#include <cstdarg>

#ifdef __cplusplus
extern "C" {
#endif

int fprintf(FILE* stream, const char* format, ...);
int printf(const char* format, ...);
int sprintf(char* s, const char* format, ...);
int snprintf(char* s, size_t n, const char* format, ...);
int vsnprintf(char* s, size_t n, const char* fmt, va_list args);
int vsprintf(char* s, const char* format, va_list arg);
int vprintf(const char* format, va_list arg);

int vswprintf(wchar_t* s, size_t n, const wchar_t* format, va_list arg);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_PRINTF_H */
