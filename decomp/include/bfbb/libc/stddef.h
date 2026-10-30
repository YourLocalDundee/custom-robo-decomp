/* Custom Robo: from bfbbdecomp/bfbb @ d8e8eac38dd617ee72161f4cb2da4498bc89aad6, src/PowerPC_EABI_Support/include/stddef.h */ #ifndef _MSL_STDDEF_H
#define _MSL_STDDEF_H

#ifdef _MSC_VER
typedef size_t; // hack to make VS Code not complain about size_t in operator new
#elif !defined(__MWERKS__) || __MWERKS__ < 0x2400
typedef unsigned long size_t;
#else
typedef __typeof__(sizeof(0)) size_t;
#endif

#endif
