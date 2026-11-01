#define fn_8006EEF8 fn_8006EEF8__proto
#include "ctx.h"
#undef fn_8006EEF8

typedef union {
    u8 pad[16];
    u8 u8;
    u16 u16;
    u32 u32;
    s8 s8;
    s16 s16;
    s32 s32;
    f32 f32;
} CRWGPipe;
extern volatile CRWGPipe cr_WGFifo;

static inline void grid_pos(const f32 x, const f32 y, const f32 z) {
    cr_WGFifo.f32 = x;
    cr_WGFifo.f32 = y;
    cr_WGFifo.f32 = z;
}

static inline void grid_color(const u8 r, const u8 g, const u8 b, const u8 a) {
    cr_WGFifo.u8 = r;
    cr_WGFifo.u8 = g;
    cr_WGFifo.u8 = b;
    cr_WGFifo.u8 = a;
}

void fn_8006EEF8(void) {
    Mtx m;
    Vec p;
    Vec q;
    s32 i;
    Vec *pp;
    u32 alpha;
    u16 n;
    u8 up = 0xFC;
    s32 down = 0;

    n = 42;
    GXSetZMode(1, GX_LEQUAL, 1);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetLineWidth(6, GX_TO_ZERO);
    GXLoadPosMtxImm((f32 (*)[4]) lbl_8032344C, 0);
    GXSetCurrentMtx(0);
    PSMTXScale(m, lbl_80450BB0, lbl_80450B98, lbl_80450BB0);
    for (i = -n / 2; i <= n / 2; i++) {
        if (i >= -12 && i <= 12) {
            alpha = 0xFF;
        } else if (i < -12) {
            down += 0x1C;
            alpha = down;
        } else if (i > 12) {
            up -= 0x1C;
            alpha = up;
        }
        if (i & 1) {
            if (i >= -12 && i <= 12) {
                p.x = (f32) i;
                p.y = lbl_80450BB4;
                p.z = (f32) n * lbl_80450BB8;
                PSMTXMultVec(m, &p, &p);
                q.x = (f32) i;
                q.y = lbl_80450BB4;
                q.z = lbl_80450BBC;
                PSMTXMultVec(m, &q, &q);
                GXBegin(GX_LINES, GX_VTXFMT0, 2);
                grid_pos(p.x, p.y, -p.z);
                grid_color(0, 0xFF, 0xD9, 0);
                grid_pos(q.x, q.y, -q.z);
                grid_color(0, 0xFF, 0xD9, alpha);
                GXBegin(GX_LINES, GX_VTXFMT0, 2);
                grid_pos(q.x, q.y, q.z);
                grid_color(0, 0xFF, 0xD9, alpha);
                grid_pos(p.x, p.y, p.z);
                grid_color(0, 0xFF, 0xD9, 0);
                p.x = (f32) n * lbl_80450BB8;
                p.y = lbl_80450BB4;
                p.z = (f32) i;
                PSMTXMultVec(m, &p, &p);
                q.x = lbl_80450BBC;
                q.y = lbl_80450BB4;
                q.z = (f32) i;
                PSMTXMultVec(m, &q, &q);
                GXBegin(GX_LINES, GX_VTXFMT0, 2);
                grid_pos(-p.x, p.y, p.z);
                grid_color(0, 0xFF, 0xD9, 0);
                grid_pos(-q.x, q.y, q.z);
                grid_color(0, 0xFF, 0xD9, alpha);
                GXBegin(GX_LINES, GX_VTXFMT0, 2);
                grid_pos(q.x, q.y, q.z);
                grid_color(0, 0xFF, 0xD9, alpha);
                grid_pos(p.x, p.y, p.z);
                grid_color(0xC8, 0xFF, 0xD9, 0);
            } else {
                pp = &p;
                pp->x = (f32) i;
                pp->y = lbl_80450BB4;
                pp->z = (f32) n * lbl_80450BB8;
                PSMTXMultVec(m, &p, &p);
                q.x = (f32) i;
                q.y = lbl_80450BB4;
                q.z = lbl_80450BBC;
                PSMTXMultVec(m, &q, &q);
                GXBegin(GX_LINESTRIP, GX_VTXFMT0, 4);
                grid_pos(p.x, p.y, -p.z);
                grid_color(0, 0xFF, 0xD9, 0);
                grid_pos(q.x, q.y, -q.z);
                grid_color(0, 0xFF, 0xD9, alpha);
                grid_pos(q.x, q.y, q.z);
                grid_color(0, 0xFF, 0xD9, alpha);
                grid_pos(p.x, p.y, p.z);
                grid_color(0, 0xFF, 0xD9, 0);
                pp = &p;
                pp->x = (f32) n * lbl_80450BB8;
                pp->y = lbl_80450BB4;
                pp->z = (f32) i;
                PSMTXMultVec(m, &p, &p);
                q.x = lbl_80450BBC;
                q.y = lbl_80450BB4;
                q.z = (f32) i;
                PSMTXMultVec(m, &q, &q);
                GXBegin(GX_LINESTRIP, GX_VTXFMT0, 4);
                grid_pos(-p.x, p.y, p.z);
                grid_color(0, 0xFF, 0xD9, 0);
                grid_pos(-q.x, q.y, q.z);
                grid_color(0, 0xFF, 0xD9, alpha);
                grid_pos(q.x, q.y, q.z);
                grid_color(0, 0xFF, 0xD9, alpha);
                grid_pos(p.x, p.y, p.z);
                grid_color(0xC8, 0xFF, 0xD9, 0);
            }
        }
    }
}
