/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800964B0: .text 0x800964B0-0x80096528 (120 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_BgmSlot
{
  u32 flags;
  u32 unk_04;
  u32 song;
  u8 start_volume;
  u8 volume;
  u8 unk_0e[0x22];
  s16 fade_time;
  u8 unk_32[6];
} cr_BgmSlot;
__declspec(section ".data") extern cr_BgmSlot cr_g_BgmSlots[2];

u8 fn_800964B0(s32 arg0) {
    u8 var_r31_4;

    var_r31_4 = 0;
    while (!(var_r31_4 >= 2U)) {
        if ((arg0 == (s32) cr_g_BgmSlots[var_r31_4].song) && (cr_g_BgmSlots[var_r31_4].flags & 0x80000000)) {
            return 1U;
        }
        var_r31_4 += 1;
    }
    return 0U;
}
