/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80097488: .text 0x80097488-0x80097530 (168 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803B76E8[1344];

/* fn_80097488: Nov 13, 2026 hand draft: the same slot loop as cr_Bgm_FadeOut (s16 fade-time local), matched by id, returns */

void fn_80097488(s32 arg0, u8 arg1, u16 arg2) {
    u32 *s = lbl_803B76E8;
    u16 i;
    s16 d;

    for (i = 0; i < 0x60U; i++, s += 0xE) {
        if (((s32) s[2] == arg0) && (s[0] & 0x80000000)) {
            d = (arg2 * 0xC8) / 60;
            M2C_FIELD(s, u8 *, 0xD) = arg1;
            M2C_FIELD(s, s16 *, 0x30) = d;
            s[0] |= 0x10;
            s[0] |= 0x410;
            return;
        }
    }
}
