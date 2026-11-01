#include "ctx.h"
extern s32 ext_a(void);
extern s32 ext_b(void);
extern void ext_use(s32);
extern s32 ext_flag;
void fn_80096754(void) {
    s32 z;
    s32 x;
    s32 y;
    z = ext_a();
    if (z == 0) {
    }
    x = ext_b();
    y = ext_a();
    ext_use(x);
    ext_use(y);
    ext_use(x);
    ext_use(y);
    ext_use(y);
}
