/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800972D8: .text 0x800972D8-0x80097468 (400 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_BgmSlot cr_BgmSlot;
struct cr_BgmSlot
{
  u32 flags;
  u32 unk_04;
  u32 song;
  u8 start_volume;
  u8 volume;
  u8 unk_0e[0x22];
  s16 fade_time;
  u8 unk_32[6];
};
__declspec(section ".data") extern cr_BgmSlot cr_g_BgmSlots[2];
extern u32 lbl_8021AC30[22];
extern u8 lbl_8044EAF8[8];
typedef struct cr_BgmRequest
{
  u32 song;
  u32 start_volume;
  u32 volume;
  u32 fade_frames;
  u32 state;
  u32 unk_14;
} cr_BgmRequest;
__declspec(section ".data") extern cr_BgmRequest cr_g_BgmRequest;
__declspec(section ".sdata") extern f32 cr_g_BgmVolume;

/* fn_800972D8: Nov 13, 2026 hand draft: BGM play at the default volume: fn_80097154 with the volume read first, same inline slot allocator */

static inline cr_BgmSlot *cr_Bgm_GetFreeSlot(void) {
    u8 i;

    for (i = 0; i < 2U; i++) {
        if (!(cr_g_BgmSlots[i].flags & 0x80000000)) {
            cr_g_BgmSlots[i] = *(cr_BgmSlot *) lbl_8021AC30;
            return &cr_g_BgmSlots[i];
        }
    }
    return NULL;
}

void fn_800972D8(s32 arg0) {
    cr_BgmSlot *s;
    u8 arg1 = *lbl_8044EAF8;

    if (arg0 < 0x48) {
        s = cr_Bgm_GetFreeSlot();
        if (s == ((void *) 0)) {
            cr_g_BgmRequest.song = (u32) arg0;
            cr_g_BgmRequest.start_volume = (u32) arg1;
            cr_g_BgmRequest.volume = (u32) arg1;
            cr_g_BgmRequest.state = 1;
            return;
        }
        s->song = (u32) arg0;
        s->start_volume = arg1;
        M2C_FIELD(s, f32 *, 0x10) = (f32) cr_g_BgmVolume;
        s->flags |= 0x40;
        s->flags |= 0x80000000;
    }
}
