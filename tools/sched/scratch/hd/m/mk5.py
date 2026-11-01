H = '''#include "ctx.h"
typedef struct { u32 a; u32 b; u32 i; u32 j; u32 k; u32 arr[16]; } T;
extern T A, B, C, E;
extern s32 dep, G1, G2, G3;
'''
V = {
'r1': 'u32 *p; p = &A.arr[dep]; (*p)++; A.i += 1; A.j += 1;',
'r2': 'u32 *p; p = &A.arr[dep]; A.i += 1; (*p)++; A.j += 1;',
'r3': 'u32 *p; p = &A.arr[dep]; A.i += 1; A.j += 1; (*p)++;',
'r4': 'A.arr[dep]++; A.i += 1; A.j += 1;',
'r5': 'A.i += 1; A.arr[dep]++; A.j += 1;',
'r6': 'A.i += 1; A.j += 1; A.arr[dep]++;',
'r7': 'u32 *p; p = &A.arr[dep]; A.i += 1; A.j += 1; G1 += 1; (*p)++;',
'r8': 'u32 *p; p = &A.arr[dep]; A.i += 1; A.j += 1; (*p)++; G1 += 1;',
}
for k, b in V.items():
    open(k + '.c', 'w', newline='\n').write(H + 'void f(void) {\n    ' + b + '\n}\n')
