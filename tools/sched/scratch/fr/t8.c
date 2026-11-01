#include "t_hdr.h"
typedef union { u8 u8; u16 u16; u32 u32; s8 s8; s16 s16; s32 s32; f32 f32; } CRWGPipe;
volatile CRWGPipe cr_WGFifo : 0xCC008000;
void test8(void) {
    s32 i;
    volatile CRWGPipe *wg = &cr_WGFifo;
    u32 alpha;
    for (i = 0; i < 10; i++) { alpha = (u32) i; wg->u8 = alpha; wg->u8 = 3; GXSetNumChans(alpha); }
}
