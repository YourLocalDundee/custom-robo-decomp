H = '''#include "ctx.h"
typedef struct { u32 a; u32 b; u32 i; u32 j; u32 k; u32 arr[16]; } T;
extern T A, B;
extern s32 dep, G1, G2, G3;
extern s16 *pcg;
'''
V = {
'o1': 'int a; int b; int c; c = G3; a = G1; b = G2; G1 = a + b + c;',
'o2': 'int a; int b; int c; a = G1; b = G2; c = G3; G1 = a + b + c;',
'o3': 'int a; { int b; b = G2; a = G1; G1 = a + b; }',
'o4': 'int a; int b; b = G2; G1 = b; a = G1; G2 = a;',
'o5': 'int a; int b; a = G1; G2 = a; b = G1; G1 = b;',
'o6': 'int a = G1; int b = G2; G3 = a + b;',
'o7': 'int a; int unused; a = G1; unused = 5; G3 = a;',
'o8': 'int a; int unused; a = G1; G3 = a;',
}
for k, b in V.items():
    open(k + '.c', 'w', newline='\n').write(H + 'void f(void) {\n    ' + b + '\n}\n')
