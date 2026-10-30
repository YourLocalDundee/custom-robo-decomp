/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/PowerPC_EABI_Support/include/wchar.h */ #ifndef _WCHAR_H
#define _WCHAR_H

#include "types.h"
#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

int fwide(FILE* stream, int mode);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
