#include "t_hdr.h"
void test1(void) {
    s32 i;
    s32 dead;
    u32 alpha;
    if (0) { GXSetNumChans(dead); }
    for (i = 0; i < 10; i++) { alpha = (u32) i; GXSetNumChans(alpha); }
}
