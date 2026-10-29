/* Custom Robo: from doldecomp/dolsdk2004 @ 2328b4164b1a98422a2255d83ce9a5a7548990cc, include/libc/wchar.h */ #ifndef _WCHAR_H_
#define _WCHAR_H_

#include <stdio.h>

typedef unsigned short wchar_t;

int fwide(FILE *stream, int mode);

#endif
