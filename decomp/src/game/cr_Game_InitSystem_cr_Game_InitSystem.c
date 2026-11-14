/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Game_InitSystem_cr_Game_InitSystem: .text 0x80023A04-0x80023CBC (696 bytes, 1 functions: cr_Game_InitSystem .. cr_Game_InitSystem), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct _GXColor
{
  u8 r;
  u8 g;
  u8 b;
  u8 a;
} GXColor;
typedef struct _GXRenderModeObj GXRenderModeObj;
typedef struct _HSD_VIStatus HSD_VIStatus;
typedef enum 
{
  VI_TVMODE_NTSC_INT = (0 << 2) + 0,
  VI_TVMODE_NTSC_DS = (0 << 2) + 1,
  VI_TVMODE_NTSC_PROG = (0 << 2) + 2,
  VI_TVMODE_PAL_INT = (1 << 2) + 0,
  VI_TVMODE_PAL_DS = (1 << 2) + 1,
  VI_TVMODE_EURGB60_INT = (5 << 2) + 0,
  VI_TVMODE_EURGB60_DS = (5 << 2) + 1,
  VI_TVMODE_MPAL_INT = (2 << 2) + 0,
  VI_TVMODE_MPAL_DS = (2 << 2) + 1,
  VI_TVMODE_DEBUG_INT = (3 << 2) + 0,
  VI_TVMODE_DEBUG_PAL_INT = (4 << 2) + 0,
  VI_TVMODE_DEBUG_PAL_DS = (4 << 2) + 1,
  VI_TVMODE_3 = 3
} VITVMode;
typedef enum 
{
  VI_XFBMODE_SF = 0,
  VI_XFBMODE_DF
} VIXFBMode;
struct _GXRenderModeObj
{
  VITVMode viTVmode;
  u16 fbWidth;
  u16 efbHeight;
  u16 xfbHeight;
  u16 viXOrigin;
  u16 viYOrigin;
  u16 viWidth;
  u16 viHeight;
  VIXFBMode xFBmode;
  u8 field_rendering;
  u8 aa;
  u8 sample_pattern[12][2];
  u8 vfilter[7];
};
extern GXRenderModeObj GXNtsc480IntDf;
void *OSGetArenaHi(void);
void *OSInitAlloc(void *arenaStart, void *arenaEnd, int maxHeaps);
void *OSGetArenaLo(void);
void OSSetArenaLo(void *);
void AIInit(u8 *stack);
extern u8 *lbl_8044F448;
unsigned long OSGetResetCode();
u32 VIGetDTVStatus(void);
u32 OSGetProgressiveMode(void);
extern u8 lbl_8044F3C8;
extern GXRenderModeObj GXNtsc480Prog;
void DVDInit();
void **HSD_AllocateXFB(s32 nbuffer, GXRenderModeObj *rm);
typedef enum _GXGamma
{
  GX_GM_1_0,
  GX_GM_1_7,
  GX_GM_2_2
} GXGamma;
struct _HSD_VIStatus
{
  GXRenderModeObj rmode;
  s32 black;
  u8 vf;
  GXGamma gamma;
  GXColor clear_clr;
  u32 clear_z;
  u8 update_clr;
  u8 update_alpha;
  u8 update_z;
};
typedef struct _current
{
  struct _HSD_VIStatus vi;
  u8 chg_flag;
} Current;
typedef enum _HSD_VIXFBDrawDispStatus
{
  HSD_VI_XFB_NONE,
  HSD_VI_XFB_NOUSE,
  HSD_VI_XFB_FREE,
  HSD_VI_XFB_DRAWING,
  HSD_VI_XFB_WAITDONE,
  HSD_VI_XFB_DRAWDONE,
  HSD_VI_XFB_NEXT,
  HSD_VI_XFB_DISPLAY,
  HSD_VI_XFB_COPYEFB,
  HSD_VI_XFB_TERMINATE
} HSD_VIXFBDrawDispStatus;
typedef struct _XFB
{
  void *buffer;
  HSD_VIXFBDrawDispStatus status;
  Current vi_all;
} XFB;
typedef enum _HSD_VIEFBDrawDispStatus
{
  HSD_VI_EFB_FREE,
  HSD_VI_EFB_DRAWDONE,
  HSD_VI_EFB_TERMINATE
} HSD_VIEFBDrawDispStatus;
typedef void (*HSD_VIRetraceCallback)(u32);
typedef void (*HSD_VIGXDrawDoneCallback)(int);
typedef struct _HSD_VIInfo
{
  Current current;
  XFB xfb[3];
  struct _EFB
  {
    HSD_VIEFBDrawDispStatus status;
    Current vi_all;
  } efb;
  s32 nb_xfb;
  HSD_VIRetraceCallback pre_cb;
  HSD_VIRetraceCallback post_cb;
  struct drawdone
  {
    s32 waiting;
    s32 arg;
    HSD_VIGXDrawDoneCallback cb;
  } drawdone;
  struct perf
  {
    s32 frame_period;
    s32 frame_renew;
  } perf;
} HSD_VIInfo;
extern HSD_VIInfo HSD_VIData;
void HSD_VISetConfigure(GXRenderModeObj *rmode);
typedef struct 
{
  u8 pad[128];
} GXFifoObj;
void HSD_GXSetFifoObj(GXFifoObj *fifo);
GXFifoObj *GXInit(void *base, u32 size);
void *fn_8019CEBC(u32 arg0);
void HSD_InitComponent(void);
void fn_80023CBC(void);
typedef int bool;
void HSD_VISetBlack(bool);
void fn_8002312C(void);
void cr_FileSys_Init(void);
s32 fn_80020204(void);
typedef enum cr_Seq
{
  SEQ_LOGO,
  SEQ_MOVIE,
  SEQ_TITLE,
  SEQ_FIELD,
  SEQ_BATTLE,
  SEQ_MODE_SELECTOR,
  SEQ_CUSTOMIZE,
  SEQ_STAGE_SELECTOR,
  SEQ_SCENARIO,
  SEQ_OPTION,
  SEQ_NAME_ENTRY,
  SEQ_MEMORY_CARD,
  SEQ_MENU,
  SEQ_ILLUSTRATED_BOOK,
  SEQ_ENDING
} cr_Seq;
typedef struct cr_SeqState
{
  cr_Seq current;
  cr_Seq previous;
  cr_Seq next;
  u8 unk_0c[4];
  u32 unk_10;
  u8 unk_14[0x10];
  u32 unk_24;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[4];
  u32 unk_34;
  s32 unk_38;
  s16 unk_3c;
  u8 unk_3e[2];
  s32 unk_40;
  u8 unk_44[4];
} cr_SeqState;
__declspec(section ".data") extern cr_SeqState cr_g_SeqCurrent;
void fn_8001D9C8(void);
void fn_8001D618(void);
typedef enum _HSD_RenderPass
{
  HSD_RP_SCREEN,
  HSD_RP_TOPHALF,
  HSD_RP_BOTTOMHALF,
  HSD_RP_OFFSCREEN,
  HSD_RP_NUM
} HSD_RenderPass;
void HSD_StartRender(HSD_RenderPass pass);
void fn_800055E4(void);

/* the file's functions (each is defined below, in address order) */
void cr_Game_InitSystem(void);

/* cr_Game_InitSystem: Nov 14, 2026 hand draft: the system start: the render mode through a local pointer (r31), the reset-code and progressive tests written out with their result variables, the new arena low in a variable declared before the high one (the original's unused-looking fifth saved register), the VI status pointer taken after the first render-mode store, a GXColor local with a zero initialiser (.sbss2) (44%) */

void cr_Game_InitSystem(void) {
    GXColor black = { 0, 0, 0, 0 };
    GXRenderModeObj *rmode;
    void *lo;
    void *hi;
    s32 reset;
    s32 prog;
    HSD_VIStatus *vi;

    rmode = &GXNtsc480IntDf;
    hi = OSGetArenaHi();
    lo = OSInitAlloc(OSGetArenaLo(), hi, 1);
    OSSetArenaLo(lo);
    AIInit(NULL);
    lbl_8044F448 = OSGetArenaLo();
    if (OSGetResetCode() == 0x80000000) {
        reset = 1;
    } else {
        reset = 0;
    }
    if (reset == 0) {
        prog = 0;
    } else if (VIGetDTVStatus() != 0) {
        prog = OSGetProgressiveMode();
    } else {
        prog = 0;
    }
    if (prog != 0) {
        lbl_8044F3C8 = 1;
        *rmode = GXNtsc480Prog;
    }
    DVDInit();
    HSD_AllocateXFB(2, rmode);
    rmode->fbWidth = 0x260;
    vi = &HSD_VIData.current.vi;
    rmode->efbHeight = 0x1C0;
    rmode->xfbHeight = 0x1C0;
    rmode->viWidth = 0x29A;
    rmode->viHeight = 0x1C0;
    rmode->viXOrigin = 0x1B;
    rmode->viYOrigin = 0x10;
    vi->rmode = *rmode;
    HSD_VISetConfigure(&vi->rmode);
    HSD_GXSetFifoObj(GXInit(fn_8019CEBC(0x40000), 0x40000));
    HSD_InitComponent();
    fn_80023CBC();
    HSD_VISetBlack(0);
    fn_8002312C();
    cr_FileSys_Init();
    fn_80020204();
    cr_g_SeqCurrent.unk_38 |= 0x2000;
    cr_g_SeqCurrent.current = SEQ_LOGO;
    cr_g_SeqCurrent.next = SEQ_LOGO;
    cr_g_SeqCurrent.unk_24 = 0;
    cr_g_SeqCurrent.unk_10 = 0;
    fn_8001D9C8();
    fn_8001D618();
    HSD_StartRender(HSD_RP_SCREEN);
    fn_800055E4();
}
