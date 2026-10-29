/* Custom Robo: from doldecomp/dolsdk2001 @ eb1234c45e6df75757c652c835507ca89674f9a8, include/libc/stddef.h */ #ifndef _STDDEF_H_
#define _STDDEF_H_

#define offsetof(type, member)	((size_t) &(((type *) 0)->member))

typedef unsigned int size_t;

#ifndef NULL
#define NULL 0L
#endif

#endif
