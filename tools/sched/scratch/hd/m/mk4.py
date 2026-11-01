H = '''#include "ctx.h"
typedef struct { u32 a; u32 b; u32 i; u32 j; u32 k; u32 arr[16]; } T;
extern T A, B, C, E;
extern s32 dep, G1, G2, G3;
'''
V = {
'q1': 'A.i = 1; B.arr[dep] = 2; C.i += 1;',
'q2': 'C.i += 1; B.arr[dep] = 2; A.i = 1;',
'q3': 'B.arr[dep] = 2; C.i += 1; A.i = 1;',
'q4': 'A.i = 1; B.i = 2; C.i = 3; E.i = 4;',
'q5': 'A.i += 1; B.i += 1; C.i += 1; E.i += 1;',
'q6': 'A.i += 1; B.i &= 0x3F; C.arr[dep]++;',
'q7': 'A.i += 1; A.i &= 0x3F; A.arr[dep]++;',
'q8': 'A.i += 1; A.i &= 0x3F; B.arr[dep]++;',
}
for k, b in V.items():
    open(k + '.c', 'w', newline='\n').write(H + 'void f(void) {\n    ' + b + '\n}\n')
