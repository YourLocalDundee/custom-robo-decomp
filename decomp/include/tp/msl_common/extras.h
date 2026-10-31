/* Custom Robo: from zeldaret/tp @ c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0, libs/PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Include/extras.h */ #ifndef _MSL_COMMON_EXTRAS_H
#define _MSL_COMMON_EXTRAS_H

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

int strnicmp(const char* str1, const char* str2, int n);
int stricmp(const char* str1, const char* str2);
size_t wcslen(const wchar_t* s);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_EXTRAS_H */
