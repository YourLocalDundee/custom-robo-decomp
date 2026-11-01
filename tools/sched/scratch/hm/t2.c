#include "ctx.h"
extern s32 ext_a(void);
extern s32 ext_b(void);
extern void ext_use(s32);
void fn_80096754(void) {
    s32 y;
    s32 x;
    s32 dead;
    dead = ext_a();
    x = ext_b();
    y = ext_a();
    ext_use(x);
    ext_use(y);
    ext_use(x);
    ext_use(y);
    ext_use(y);
}
