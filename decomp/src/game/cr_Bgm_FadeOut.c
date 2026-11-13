/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Bgm_FadeOut: .text 0x80096EBC-0x80096F54 (152 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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

/* cr_Bgm_FadeOut: Nov 13, 2026 hand draft: slot loop; the fade time goes through an s16 local (the saved r29 the code never uses) */

void cr_Bgm_FadeOut(u8 arg0, u16 arg1) {
    cr_BgmSlot *s = cr_g_BgmSlots;
    u16 i;
    s16 d;

    for (i = 0; i < 2U; i++, s++) {
        if (s->flags & 0x80000000) {
            d = (arg1 * 0xC8) / 60;
            s->volume = arg0;
            s->fade_time = d;
            s->flags |= 0x10;
            s->flags |= 0x410;
        }
    }
}
