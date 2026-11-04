/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001D9C8: .text 0x8001D9C8-0x8001D9E8 (32 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern f32 *cr_g_MtxStackTop;
extern u8 lbl_8031F268[3072];
__declspec(section ".sdata") extern s32 cr_g_MtxStackDepth;
__declspec(section ".sdata") extern s32 cr_g_MtxStackMaxDepth;

/* fn_8001D9C8: Nov 3, 2026 hand draft (m2c 50%): source order Top, Depth, MaxDepth (Top, MaxDepth, Depth gives the same words with the two zero stores swapped: checked with hd.py -r) */

void fn_8001D9C8(void) {
    cr_g_MtxStackTop = (f32 *) lbl_8031F268;
    cr_g_MtxStackDepth = 0;
    cr_g_MtxStackMaxDepth = 0;
}
