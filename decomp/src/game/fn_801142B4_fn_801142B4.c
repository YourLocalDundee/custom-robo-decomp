/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_801142B4_fn_801142B4: .text 0x801142B4-0x801143D0 (284 bytes, 1 functions: fn_801142B4 .. fn_801142B4), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
void fn_80074FBC(void *arg0, void *arg1);
extern u8 *lbl_8044F2E8;
extern u8 lbl_80354110[68];
extern u32 lbl_8037569C[2417];
void fn_80072A58(void *arg0, void *arg1);
extern u8 lbl_8034C100[32768];
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
typedef struct _GXRenderModeObj
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
} GXRenderModeObj;
typedef enum _GXGamma
{
  GX_GM_1_0,
  GX_GM_1_7,
  GX_GM_2_2
} GXGamma;
typedef struct _HSD_VIStatus
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
} HSD_VIStatus;
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
void fn_80072064(void);
void fn_8006FB88(void);
void fn_80070130(void);

/* the file's functions (each is defined below, in address order) */
void fn_801142B4(void *arg0, s32 arg1, u8 arg2);

/* fn_801142B4: Nov 13, 2026 hand draft: the mode is the third parameter (u8); the clear colour a GXColor local with a zero initialiser (the four .sbss2 bytes), copied through a block local into HSD_VIData; the scene pointer a local */

void fn_801142B4(void *arg0, s32 arg1, u8 arg2) {
    GXColor c = { 0, 0, 0, 0 };
    u32 *var_r30;

    if (arg2 == 0) {
        fn_80074FBC(lbl_8044F2E8, lbl_80354110);
        M2C_FIELD(arg0, s8 *, 4) = 4;
        M2C_FIELD(arg0, s8 *, 5) = 1;
    } else if (arg2 == 1) {
        var_r30 = lbl_8037569C;
        fn_80072A58(var_r30, lbl_8034C100);
        M2C_FIELD(arg0, s8 *, 4) = 4;
        {
            GXColor t = c;
            HSD_VIData.current.vi.clear_clr = t;
        }
        fn_80072064();
        fn_8006FB88();
        *lbl_8037569C = (u32) fn_80070130;
        M2C_FIELD(arg0, s8 *, 5) = 1;
    }
}
