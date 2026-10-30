/* Custom Robo: from doldecomp/kar @ 3ed51a1a008a05c33225cc286c046b8f90fb3d0e, src/sysdolphin/initialize.c */ #include "initialize.h"
/* Custom Robo: wowjinxy/KAR initialize.c functions that are unnamed in symbols/symbols.txt, under their main.dol
 * addresses so objdiff pairs them (each is wowjinxy's function at that place in wowjinxy's order, and its body
 * compiled here equals main.dol's but for the assert / HSD_Panic line literal noted at the function) */
#define HSD_AllocateFIFO fn_8019CEBC
#define _HSD_MemAllocDefaultCB fn_8019D15C
#define _HSD_MemGetRemainDefaultCB fn_8019D1F0
#define HSD_MemCheckOwnDefaultCB fn_8019D278
#define HSD_GetHeap fn_8019D2A0
#define HSD_EndRender fn_8019D38C
#define _HSD_NeedCacheInvalidate fn_8019D398
#define HSD_OSInit fn_8019CFB8
void* HSD_AllocateFIFO(u32 size); /* initialize.h declares it before the define above */

#include <sysdolphin/video.h>

/* Custom Robo (step 61 iteration 8): kar's names for globals main.dol has at these addresses (unnamed in
 * symbols/symbols.txt; from the instruction that uses each in main.dol's copy of the function): FrameBuffer
 * lbl_80449E80 (HSD_InitComponent's HSD_VIInit arguments), memReport lbl_80449E8C (HSD_OSInit), init_done lbl_80450020
 * (HSD_InitComponent), kar's rmode is lbl_8044F104 (defined below), kar's lbl_805DE2A8 = lbl_80450018 and lbl_805DCBBC =
 * lbl_8044F10C (HSD_GXSetFifoObj), kar's "initialize.c" lbl_80503C60 = lbl_8024CB00 (the asserts' file name) */
#define FrameBuffer lbl_80449E80
#define memReport lbl_80449E8C
#define init_done lbl_80450020
#define lbl_805DE2A8 lbl_80450018
#define lbl_805DCBBC lbl_8044F10C
#define lbl_80503C60 lbl_8024CB00
extern void* FrameBuffer[HSD_VI_XFB_MAX];
extern HSD_MemReport memReport;
extern BOOL init_done;


extern int lbl_805DCBBC, lbl_805DE2A8;
extern GXRenderModeObj GXNtsc480IntDf;

/* Custom Robo: initialize.c's initialized small data, main.dol .sdata 0x8044F100-0x8044F113 in this order (Melee's
 * current_heap = -1, rmode = &GXNtsc480IntDf, current_z_fmt = GX_ZC_MID, iparam_fifo_size = 0x40000 and
 * HSD_Init_804D5E1C = { 0 }, wowjinxy's HSD_CurrentHeap, HSD_RenderMode, HSD_ZBufferFormat, HSD_FIFOSize,
 * HSD_ClearColor; this revision has no xfb / heap / audio iparams there), under main.dol's addresses as above */
volatile s32 lbl_8044F100 = -1;
GXRenderModeObj* lbl_8044F104 = &GXNtsc480IntDf; /* kar's rmode */
s32 lbl_8044F108 = 2;
int lbl_805DCBBC = 0x40000;
GXColor lbl_8044F110 = { 0, 0, 0, 0 };
void HSD_OSInit();
/* Custom Robo: prototypes of the wowjinxy/KAR functions ported below (kar calls them undeclared) */
void** HSD_AllocateXFB(s32 nbuffer, GXRenderModeObj* rmode);
void HSD_GXInit(void);
void HSD_ObjInit(void);

char lbl_80503C60[] = "initialize.c";

/* wowjinxy/KAR initialize.c HSD_Init as compiled for VERSION_GKYP01 (KAR PAL: no DVDInit / XFB / FIFO block, which
 * kar's HSD_InitComponent has); main.dol's HSD_InitComponent is that form, 0x108 bytes. kar's names: HSD_RenderMode =
 * rmode, HSD_XFBs = FrameBuffer, HSD_InitComplete = init_done; HSD_DefaultClearColor = lbl_80452AA8 (main.dol's
 * clear_clr load) */
#define HSD_DefaultClearColor lbl_80452AA8
extern const u32 HSD_DefaultClearColor[2];
/* Custom Robo: the functions below are not in doldecomp/kar's initialize.c; each is from wowjinxy/KAR @
 * cb94b157612a1d8d187f0bd6ecb5c02aa4c77b57, src/sysdolphin/initialize.c (the same HSD revision as main.dol), adapted to
 * kar's headers: kar's names for the XFB array and the memory report (HSD_XFBs = FrameBuffer, HSD_MemReportData =
 * memReport, above), wowjinxy's other globals at main.dol's addresses (unnamed in symbols/symbols.txt), each from the
 * instruction that reads or writes it in main.dol's copy of the function: HSD_ClearColor lbl_8044F110 (HSD_GXInit's
 * GXInitLightColor argument), HSD_CurrentRenderPass lbl_80450030 (HSD_GetCurrentRenderPass, HSD_StartRender),
 * CObjRenderMode lbl_80449EA0, HSD_ZBufferFormat lbl_8044F108, HSD_PixelFormat lbl_8045001C, HSD_CacheInvalidateFlags
 * lbl_8045002C (HSD_StartRender); HSD_GetHeap is fn_8019D2A0 (_HSD_MemFreeDefaultCB's call; see the defines at the top). HSD_AllocateXFB's HSD_Panic line is the one main.dol passes (0xF0). */
#define HSD_XFBs FrameBuffer
#define HSD_MemReportData memReport
#define HSD_ClearColor lbl_8044F110
#define HSD_CurrentRenderPass lbl_80450030
#define CObjRenderMode lbl_80449EA0
#define HSD_ZBufferFormat lbl_8044F108
#define HSD_PixelFormat lbl_8045001C
#define HSD_CacheInvalidateFlags lbl_8045002C
typedef s32 HSD_RenderPass;
extern GXColor HSD_ClearColor;
extern s32 HSD_CurrentRenderPass;
extern HSD_VIStatus CObjRenderMode;
extern s32 HSD_ZBufferFormat;
extern s32 HSD_PixelFormat;
extern s32 HSD_CacheInvalidateFlags;
void* HSD_GetHeap(void);
void* OSAllocFromArenaLo(u32 size, u32 align);
void OSFreeToHeap(void* heap, void* ptr);
void HSD_Panic(char*, int, char*);
u32 HSD_Index2LightID(u32 index);
void HSD_StateInvalidate(s32 state);
void GXInitLightPos(GXLightObj* lt_obj, f32 x, f32 y, f32 z);
void GXInitLightDir(GXLightObj* lt_obj, f32 nx, f32 ny, f32 nz);
void GXInitLightAttn(GXLightObj* lt_obj, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2);
void GXInitLightColor(GXLightObj* lt_obj, GXColor color);
void GXLoadLightObjImm(GXLightObj* lt_obj, u32 light);
void GXSetPixelFmt(s32 pix_fmt, s32 z_fmt);
void GXSetFieldMode(u8 field_mode, u8 half_aspect_ratio);
void GXInvalidateVtxCache(void);
void GXInvalidateTexAll(void);
void HSD_ListInitAllocData(void);
void HSD_AObjInitAllocData(void);
void HSD_FObjInitAllocData(void);
void HSD_IDInitAllocData(void);
void HSD_VecInitAllocData(void);
void HSD_MtxInitAllocData(void);
void HSD_RObjInitAllocData(void);
void HSD_RenderInitAllocData(void);
void HSD_ShadowInitAllocData(void);
void HSD_ZListInitAllocData(void);

/* wowjinxy/KAR initialize.c: more globals at main.dol's addresses, from the instruction that reads them in main.dol's
 * copy of the function: HSD_HeapLo lbl_80450008, HSD_HeapHi lbl_8045000C (HSD_MemCheckOwnDefaultCB),
 * HSD_HasCustomMemCallbacks lbl_80450028, HSD_CurrentHeap lbl_8044F100 (HSD_GetHeap); kar_diag__803d3884 is OSCheckHeap
 * (_HSD_MemGetRemainDefaultCB's call in main.dol) */
#define HSD_HeapLo lbl_80450008
#define HSD_HeapHi lbl_8045000C
#define HSD_HasCustomMemCallbacks lbl_80450028
#define HSD_CurrentHeap lbl_8044F100
#define HSD_RP_OFFSCREEN 3
extern u32 HSD_HeapLo;
extern u32 HSD_HeapHi;
extern s32 HSD_HasCustomMemCallbacks;
extern volatile s32 HSD_CurrentHeap;
void* OSAllocFromHeap(s32 heap, u32 size);
s32 OSCheckHeap(s32 heap);

#define assert_line_memcb(line) \
    ((HSD_HasCustomMemCallbacks == 0) ? (void) 0 : __assert("initialize.c", line, "!iparam_memory_callbacks"))
#define assert_line_addr(line, cond) ((cond) ? (void) 0 : __assert("initialize.c", line, "addr"))

/* wowjinxy/KAR initialize.c _HSD_MemClearDefaultCB without its HSD_FillHeapEnabled loop (main.dol's copy has none: 0x64
 * bytes); HSD_NextArenaLo lbl_80450010, HSD_NextArenaHi lbl_80450014 (read and cleared here in main.dol) */
#define _HSD_MemClearDefaultCB fn_8019D214
#define HSD_NextArenaLo lbl_80450010
#define HSD_NextArenaHi lbl_80450014
extern u32 HSD_NextArenaLo;
extern u32 HSD_NextArenaHi;
void OSDestroyHeap(s32 heap);
s32 OSCreateHeap(void* start, void* end);
s32 OSSetCurrentHeap(s32 heap);
/* wowjinxy/KAR initialize.c HSD_OSInit without its two HSD_FillHeapEnabled loops (main.dol's copy has none: 0x1A4
 * bytes); HSD_HeapMaxSize lbl_80450024 (read here in main.dol), OSGetPhysicalMemSize fn_80136484 and
 * _HSD_MemSetCallbacks fn_801A3FEC (main.dol's calls here, in wowjinxy's order) */
#define HSD_HeapMaxSize lbl_80450024
// (OSGetPhysicalMemSize: named, step 68)
#define _HSD_MemSetCallbacks fn_801A3FEC
#define OSRoundDown32B(x) (((u32) (x)) & ~(32 - 1))
#define OSRoundUp32B(x) (((u32) (x) + 32 - 1) & ~(32 - 1))
extern s32 HSD_HeapMaxSize;
typedef struct _HSD_MemCallbacks {
    void* (*alloc)(u32 size, u32 align, u32 flags);
    void (*free)(void* ptr);
    void (*clear)(void);
    u32 (*get_remain)(void);
    s32 (*check_own)(void* ptr);
} HSD_MemCallbacks; /* wowjinxy include/sysdolphin/memory.h */
void* OSGetArenaLo(void);
void* OSGetArenaHi(void);
void OSSetArenaLo(void* lo);
u32 OSGetPhysicalMemSize(void);
void _HSD_MemSetCallbacks(HSD_MemCallbacks* cb, s32 size);
void HSD_ObjSetHeap(u32 size, void* p);

/* Custom Robo (step 61 iteration 8): the functions in main.dol's order (the file above keeps the declarations
 * and defines in place; mwld places the functions in definition order) */

void HSD_InitComponent(void);
void HSD_GXSetFifoObj(GXFifoObj* fifo);
void HSD_DVDInit(void);
void** HSD_AllocateXFB(s32 nbuffer, GXRenderModeObj* rmode);
void* HSD_AllocateFIFO(u32 size);
void HSD_GXInit(void);
void HSD_OSInit(void);
void* _HSD_MemAllocDefaultCB(u32 size, u32 align, u32 flags);
void _HSD_MemFreeDefaultCB(void* ptr);
u32 _HSD_MemGetRemainDefaultCB(void);
void _HSD_MemClearDefaultCB(void);
s32 HSD_MemCheckOwnDefaultCB(void* ptr);
void* HSD_GetHeap(void);
HSD_RenderPass HSD_GetCurrentRenderPass(void);
void HSD_StartRender(HSD_RenderPass pass);
void HSD_EndRender(void);
void _HSD_NeedCacheInvalidate(u32 flags);
void HSD_ObjInit(void);

void HSD_InitComponent(void)
{
    HSD_VIStatus vi_status;

    HSD_OSInit();

    vi_status.rmode = *lbl_8044F104; /* Custom Robo: kar's rmode */
    vi_status.black = GX_TRUE;
    vi_status.vf = GX_TRUE;
    vi_status.gamma = GX_GM_1_0;
    vi_status.clear_clr = *(GXColor*) HSD_DefaultClearColor;
    vi_status.clear_z = GX_MAX_Z24;
    vi_status.update_clr = GX_ENABLE;
    vi_status.update_alpha = GX_ENABLE;
    vi_status.update_z = GX_ENABLE;
    HSD_VIInit(&vi_status, FrameBuffer[0], FrameBuffer[1], FrameBuffer[2]);

    HSD_GXInit();
    HSD_DVDInit();
    HSD_IDSetup();
    VIWaitForRetrace();
    HSD_ObjInit();
    init_done = TRUE;
}

/* wowjinxy/KAR initialize.c HSD_GXSetFifoObj (VERSION_GKYP01); kar's names HSD_GXFifo = lbl_805DE2A8, HSD_FIFOSize =
 * lbl_805DCBBC (kar's HSD_InitComponent assigns GXInit's result to the first and passes the second as the FIFO size) */
void HSD_GXSetFifoObj(GXFifoObj* fifo)
{
    lbl_805DE2A8 = (int) fifo;
    memReport.gxfifo = lbl_805DCBBC;
}

void HSD_DVDInit(void)
{
}

/* wowjinxy/KAR initialize.c HSD_AllocateXFB */
void** HSD_AllocateXFB(s32 nbuffer, GXRenderModeObj* rmode)
{
    s32 i;
    u32 fb_size;

    if (rmode == NULL) {
        return NULL;
    }

    fb_size = (((rmode->fbWidth + 15) & 0xFFF0) * rmode->xfbHeight) << 1;
    HSD_MemReportData.xfb = fb_size * nbuffer;

    for (i = 0; i < nbuffer; i++) {
        if ((HSD_XFBs[i] = OSAllocFromArenaLo(fb_size, 0x20)) == NULL) {
            HSD_Panic("initialize.c", 0xF0, "No memory space remains for XFB.\n"); /* Custom Robo: main.dol passes 0xF0 here, wowjinxy 0xF4 */
        }
    }
    for (i = nbuffer; i < HSD_VI_XFB_MAX; i++) {
        HSD_XFBs[i] = NULL;
    }
    return HSD_XFBs;
}

/* wowjinxy/KAR initialize.c HSD_AllocateFIFO */
void* HSD_AllocateFIFO(u32 size)
{
    void* fifo = OSAllocFromArenaLo(size, 0x20);
    if (fifo == NULL) {
        HSD_Panic("initialize.c", 0x103, "no space remains for gx fifo.\n"); /* Custom Robo: main.dol passes 0x103 here, wowjinxy 0x107 */
    }
    return fifo;
}

/* wowjinxy/KAR initialize.c HSD_GXInit */
void HSD_GXInit(void)
{
    GXLightObj lightobj;
    s32 i;

    GXInitLightPos(&lightobj, 1.0F, 0.0F, 0.0F);
    GXInitLightDir(&lightobj, 1.0F, 0.0F, 0.0F);
    GXInitLightAttn(&lightobj, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F);
    GXInitLightColor(&lightobj, HSD_ClearColor);

    for (i = 0; i < 8; i++) {
        GXLoadLightObjImm(&lightobj, HSD_Index2LightID(i));
    }
    HSD_StateInvalidate(-1);
}

void HSD_OSInit(void)
{
    u32 new_arena_lo;
    u32 new_arena_hi;
    u32 old_arena_lo = (u32) OSGetArenaLo();
    u32 old_arena_hi = (u32) OSGetArenaHi();

    HSD_MemReportData.total = OSGetPhysicalMemSize();
    HSD_MemReportData.system = HSD_MemReportData.total - (u32) OSGetArenaHi() + (u32) OSGetArenaLo() -
                               HSD_MemReportData.xfb - HSD_MemReportData.gxfifo;

    if (!HSD_HasCustomMemCallbacks) {
        HSD_MemCallbacks cb;
        cb.alloc = _HSD_MemAllocDefaultCB;
        cb.free = _HSD_MemFreeDefaultCB;
        cb.get_remain = _HSD_MemGetRemainDefaultCB;
        cb.clear = _HSD_MemClearDefaultCB;
        cb.check_own = HSD_MemCheckOwnDefaultCB;
        _HSD_MemSetCallbacks(&cb, sizeof(cb));

        if (HSD_NextArenaLo != 0 && HSD_NextArenaHi != 0) {
            HSD_HeapLo = HSD_NextArenaLo;
            HSD_HeapHi = HSD_NextArenaHi;
            HSD_NextArenaLo = 0;
            HSD_NextArenaHi = 0;
            OSSetCurrentHeap(HSD_CurrentHeap = OSCreateHeap((void*) HSD_HeapLo, (void*) HSD_HeapHi));
            HSD_MemReportData.heap = HSD_HeapHi - HSD_HeapLo;
            HSD_ObjSetHeap(HSD_MemReportData.heap, NULL);
        } else {
            new_arena_lo = OSRoundUp32B((u32) OSGetArenaLo());
            new_arena_hi = (u32) OSGetArenaHi();
            /* Custom Robo: a store through the address (main.dol reloads HSD_HeapLo for OSCreateHeap below instead
             * of reusing new_arena_lo; the plain assignment reuses the register) */
            *(u32*) &HSD_HeapLo = new_arena_lo;
            if (HSD_HeapMaxSize > 0) {
                u32 want = new_arena_lo + HSD_HeapMaxSize;
                HSD_HeapHi = want;
                if (want > new_arena_hi) {
                    HSD_HeapHi = new_arena_hi;
                }
                HSD_HeapHi = OSRoundDown32B(HSD_HeapHi);
            } else {
                HSD_HeapHi = OSRoundDown32B(new_arena_hi);
            }
            OSSetCurrentHeap(HSD_CurrentHeap = OSCreateHeap((void*) HSD_HeapLo, (void*) HSD_HeapHi));
            HSD_MemReportData.heap = HSD_HeapHi - HSD_HeapLo;
            HSD_ObjSetHeap(HSD_MemReportData.heap, NULL);
            OSSetArenaLo((void*) HSD_HeapHi);
        }
    }
}

/* wowjinxy/KAR initialize.c _HSD_MemAllocDefaultCB */
void* _HSD_MemAllocDefaultCB(u32 size, u32 align, u32 flags)
{
    void* addr;

    if (size == 0) {
        return NULL;
    }
    addr = OSAllocFromHeap((s32) HSD_GetHeap(), size);
    assert_line_addr(0x179, addr); /* Custom Robo: main.dol passes 0x179 here, wowjinxy 0x18D */
    return addr;
}

/* wowjinxy/KAR initialize.c _HSD_MemFreeDefaultCB */
void _HSD_MemFreeDefaultCB(void* ptr)
{
    OSFreeToHeap(HSD_GetHeap(), ptr);
}

/* wowjinxy/KAR initialize.c _HSD_MemGetRemainDefaultCB */
u32 _HSD_MemGetRemainDefaultCB(void)
{
    return OSCheckHeap((s32) HSD_GetHeap());
}

void _HSD_MemClearDefaultCB(void)
{
    OSDestroyHeap(HSD_CurrentHeap);
    if (HSD_NextArenaLo != 0 && HSD_NextArenaHi != 0) {
        HSD_HeapLo = HSD_NextArenaLo;
        HSD_HeapHi = HSD_NextArenaHi;
        HSD_NextArenaLo = 0;
        HSD_NextArenaHi = 0;
    }
    OSSetCurrentHeap(HSD_CurrentHeap = OSCreateHeap((void*) HSD_HeapLo, (void*) HSD_HeapHi));
}

/* wowjinxy/KAR initialize.c HSD_MemCheckOwnDefaultCB */
s32 HSD_MemCheckOwnDefaultCB(void* ptr)
{
    s32 within = 0;
    if (HSD_HeapLo <= (u32) ptr) {
        if ((u32) ptr < HSD_HeapHi) {
            within = 1;
        }
    }
    return within;
}

/* wowjinxy/KAR initialize.c HSD_GetHeap */
void* HSD_GetHeap(void)
{
    assert_line_memcb(0x1B5); /* Custom Robo: main.dol passes 0x1B5 here, wowjinxy 0x1D1 */
    return (void*) HSD_CurrentHeap;
}

/* wowjinxy/KAR initialize.c HSD_GetCurrentRenderPass */
HSD_RenderPass HSD_GetCurrentRenderPass(void)
{
    return HSD_CurrentRenderPass;
}

/* wowjinxy/KAR initialize.c HSD_StartRender */
void HSD_StartRender(HSD_RenderPass pass)
{
    GXRenderModeObj* rmode = &CObjRenderMode.rmode;

    HSD_CurrentRenderPass = pass;
    if (rmode->aa) {
        GXSetPixelFmt(GX_PF_RGB565_Z16, HSD_ZBufferFormat);
    } else {
        GXSetPixelFmt(HSD_PixelFormat, GX_ZC_LINEAR);
    }
    GXSetFieldMode(rmode->field_rendering, rmode->xfbHeight < rmode->viHeight);

    if (HSD_CacheInvalidateFlags != 0) {
        if (HSD_CacheInvalidateFlags & 1) {
            GXInvalidateVtxCache();
        }
        if (HSD_CacheInvalidateFlags & 2) {
            GXInvalidateTexAll();
        }
        HSD_CacheInvalidateFlags = 0;
    }
}

/* wowjinxy/KAR initialize.c HSD_EndRender */
void HSD_EndRender(void)
{
    if ((*(volatile s32*) &HSD_CurrentRenderPass) == HSD_RP_OFFSCREEN) {
        return;
    }
}

/* wowjinxy/KAR initialize.c _HSD_NeedCacheInvalidate */
void _HSD_NeedCacheInvalidate(u32 flags)
{
    HSD_CacheInvalidateFlags |= flags;
}

/* wowjinxy/KAR initialize.c HSD_ObjInit */
void HSD_ObjInit(void)
{
    HSD_ListInitAllocData();
    HSD_AObjInitAllocData();
    HSD_FObjInitAllocData();
    HSD_IDInitAllocData();
    HSD_VecInitAllocData();
    HSD_MtxInitAllocData();
    HSD_RObjInitAllocData();
    HSD_RenderInitAllocData();
    HSD_ShadowInitAllocData();
    HSD_ZListInitAllocData();
}
