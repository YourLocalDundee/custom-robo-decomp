H = '''#include "ctx.h"
typedef struct { u32 a; u32 b; u32 i; u32 j; u32 arr[16]; } T;
extern T A, B;
extern s32 dep;
extern s16 *pcg;
'''
V = {
'm1': 'A.i += 1; B.i &= 0x3F;',
'm2': 'B.i &= 0x3F; A.i += 1;',
'm3': 'A.i += 1; A.i &= 0x3F;',
'm4': 'A.i &= 0x3F; A.i += 1;',
'm5': 'A.i += 1; A.j &= 0x3F;',
'm6': 'A.i += 1; B.i += 1; A.j += 1;',
'm7': 'A.i += 1; A.arr[dep] += 1;',
'm8': 'A.arr[dep] += 1; A.i += 1;',
'm9': 'A.i = *pcg; A.j += 1;',
'm10': 'A.i += 1; A.j &= 0x3F; A.arr[dep] += 1;',
}
for k, b in V.items():
    open(k + '.c', 'w', newline='\n').write(H + 'void f(void) {\n    ' + b + '\n}\n')
