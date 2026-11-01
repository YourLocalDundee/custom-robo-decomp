H = '''#include "ctx.h"
typedef struct { u32 a; u32 b; u32 i; u32 j; u32 k; u32 arr[16]; } T;
extern T A, B;
extern s32 dep, G1, G2;
extern s16 *pcg;
'''
V = {
'n1': 'u32 x; x = G1; A.i += 1; A.j += 1; A.k += 1; B.i = x;',
'n2': 'A.i += 1; A.j += 1; A.k += 1; B.i = G1;',
'n3': 'A.i = G1; A.j = G2;',
'n4': 'A.arr[dep] = 1; A.i += 1; A.j += 1;',
'n5': 'A.i += 1; A.j += 1; A.arr[dep] = 1;',
'n6': 'A.i += 1; A.j += 1; A.arr[G1] = G2;',
'n7': 'A.i += 1; B.i = A.i;',
'n8': 'u32 *p; p = &A.arr[dep]; A.i += 1; A.j &= 0x3F; (*p)++;',
'n9': 'A.i += 1; A.j &= 0x3F; A.arr[dep]++;',
}
for k, b in V.items():
    open(k + '.c', 'w', newline='\n').write(H + 'void f(void) {\n    ' + b + '\n}\n')
