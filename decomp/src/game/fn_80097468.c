/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80097468: .text 0x80097468-0x80097488 (32 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803B72E8[228];

void fn_80097468(s32 arg0) {

    *((s32 *) ((u32 *) ((u8 *) lbl_803B72E8 + ((arg0 * 0x10) & 0xFF0)))) &= 0xFFFFFFFD;
}
