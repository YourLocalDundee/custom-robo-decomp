/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80096F54: .text 0x80096F54-0x80096FAC (88 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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

void fn_80096F54(void) {
    cr_BgmSlot *var_r31_5;
    u16 var_r30_7;

    var_r31_5 = cr_g_BgmSlots;
    var_r30_7 = 0;
    while (var_r30_7 < 2U) {
        if (var_r31_5->flags & 0x80000000) {
            var_r31_5->flags |= 0x80;
        }
        var_r30_7 += 1;
        var_r31_5 = (cr_BgmSlot *) ((u8 *) var_r31_5 + 0x38);
    }
}
