H = '''#include "ctx.h"
typedef struct { u32 a; u32 b; u32 i; u32 j; u32 k; u32 arr[16]; } T;
typedef union { struct { u32 a; u32 b; u32 i; } s; u32 w[3]; } U;
extern T A, B, C, E;
extern U UU;
extern s32 dep, G1, G2, G3;
'''
V = {
's0': 'A.i += 1; A.i &= 0x3F;',
's1': 'A.i++; A.i &= 0x3F;',
's2': '++A.i; A.i &= 0x3F;',
's3': 'A.i += 1; A.i = A.i & 0x3F;',
's4': 'A.i += 1; A.i %= 64;',
's5': 'A.i = A.i + 1; A.i &= 0x3F;',
's6': 'A.i &= (A.i++, 0x3F);',
's7': 'UU.w[2]++; UU.s.i &= 0x3F;',
's8': 'UU.s.i++; UU.w[2] &= 0x3F;',
's9': 'A.i += 1; A.i &= 0x3F; A.j = 1;',
's10': 'A.j = 1; A.i += 1; A.i &= 0x3F;',
's11': 'A.i += 1; A.i &= 0x3F; G1 = 1;',
}
for k, b in V.items():
    open(k + '.c', 'w', newline='\n').write(H + 'void f(void) {\n    ' + b + '\n}\n')
