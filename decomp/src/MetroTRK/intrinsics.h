/* Custom Robo: from doldecomp/melee @ 24e685d57535c478a2e94802c85b31c2ba54ee67, src/MetroTRK/intrinsics.h */ #ifndef METROTRK_INTRINSICS_H
#define METROTRK_INTRINSICS_H

#include <stddef.h>

void __sync(void);
void __isync(void);
int __cntlzw(unsigned int);
float sqrtf__Ff(float);
float __fnmsubs(float, float, float);
double __fabs(double);
float __fabsf(float);
double __frsqrte(double);
int __rlwinm(int, int, int, int);
int __rlwimi(int, int, int, int, int);

void* __memcpy(void* dst, const void* src, size_t n);

#endif
