/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_SEQ_STAGE_SELECTOR_Leave_cr_SEQ_STAGE_SELECTOR_Leave: .text 0x8000F900-0x8000FA54 (340 bytes, 1 functions: cr_SEQ_STAGE_SELECTOR_Leave .. cr_SEQ_STAGE_SELECTOR_Leave), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
void fn_8011214C(void);
typedef struct cr_StageSelectorState
{
  u8 state;
  u8 col;
  u8 row;
  u8 unk_003;
  u8 top_row;
  u8 stage_count;
  u8 stage;
  u8 cursor;
  u32 timer;
  u8 ready;
  u8 unk_00d;
  u8 unk_00e[2];
  u32 flags;
  s8 stage_ids[42];
  u16 stage_msgs[42];
  u8 stage_open[42];
  u8 unk_0bc;
  u8 unk_0bd;
  u8 unk_0be;
  u8 unk_0bf;
  u8 unk_0c0[2];
  u8 unk_0c2;
  u8 unk_0c3[7];
  u8 unk_0ca[2];
  f32 unk_0cc;
  f32 unk_0d0;
  u32 unk_0d4;
  u16 unk_0d8;
  u16 unk_0da;
  u16 unk_0dc;
  u16 unk_0de;
  u8 unk_0e0[0x5C];
  void *unk_13c;
  u8 windows[8][0x34];
  u8 unk_2e0[0x68];
  u32 unk_348;
  u8 unk_34c;
  u8 unk_34d[3];
  s8 unk_350;
  u8 unk_351[3];
} cr_StageSelectorState;
__declspec(section ".data") extern cr_StageSelectorState cr_g_StageSelector;
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
void fn_80097530(void);
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
extern u8 lbl_8044F848;
extern u8 lbl_8044F849;
extern u8 lbl_8044F84A;
extern u8 lbl_8044F84B[5];
void fn_80111E0C(s32 arg0);

/* the file's functions (each is defined below, in address order) */
void cr_SEQ_STAGE_SELECTOR_Leave(void);

/* cr_SEQ_STAGE_SELECTOR_Leave: Nov 14, 2026 hand draft: the clear colour saved and replaced: a GXColor local with a zero initialiser (.sbss2), and in the block two GXColor locals copied byte by byte (old from HSD_VIData's clear colour, new from the local), the new one declared first (42%) */

void cr_SEQ_STAGE_SELECTOR_Leave(void) {
    GXColor col = { 0, 0, 0, 0 };

    fn_8011214C();
    if ((cr_g_StageSelector.flags & 0x80) && (cr_g_SeqCurrent.next == SEQ_MODE_SELECTOR)) {
        cr_g_StageSelector.flags &= ~0x80;
        fn_80097530();
    }
    if ((cr_g_SeqCurrent.next == SEQ_CUSTOMIZE) || (cr_g_SeqCurrent.next == SEQ_MODE_SELECTOR) ||
        (cr_g_SeqCurrent.next == SEQ_MENU)) {
        GXColor nw;
        GXColor old;

        old = HSD_VIData.current.vi.clear_clr;
        nw = col;

        lbl_8044F848 = old.r;
        lbl_8044F849 = old.g;
        lbl_8044F84A = old.b;
        lbl_8044F84B[0] = old.a;
        HSD_VIData.current.vi.clear_clr = nw;
    }
    fn_80111E0C(0x5F);
}
