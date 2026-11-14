/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80096E28: .text 0x80096E28-0x80096EBC (148 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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

/* fn_80096E28: Nov 13, 2026 hand draft: the fade time is stored and assigned in one chained assignment through a u16 field (slot->fade_time = time = time * 200 / 60: the store takes the untruncated r0, the parameter the clrlwi) */

void fn_80096E28(s32 arg0, u8 arg1, u16 arg2) {
    cr_BgmSlot *s = cr_g_BgmSlots;
    u16 i;

    for (i = 0; i < 2U; i++, s++) {
        if (((s32) s->song == arg0) && (s->flags & 0x80000000)) {
            s->volume = arg1;
            *(u16 *) &s->fade_time = arg2 = (arg2 * 0xC8) / 60;
            s->flags |= 0x10;
        }
    }
}
