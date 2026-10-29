/* Custom Robo: from doldecomp/dolsdk2004 @ 2328b4164b1a98422a2255d83ce9a5a7548990cc, include/libc/stddef.h */ #ifndef _STDDEF_H_
#define _STDDEF_H_

#define offsetof(type, member)	((size_t) &(((type *) 0)->member))

typedef unsigned int size_t;

#ifndef NULL
#define NULL 0L
#endif

#endif
