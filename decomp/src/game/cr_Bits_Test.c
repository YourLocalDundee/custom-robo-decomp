/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Bits_Test: .text 0x800231B4-0x800231EC (56 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* cr_Bits_Test: Nov 13, 2026 hand draft (m2c 33-40%): the word index and the mask are named locals (`u32 word = bit >> 5; u32 mask = 1 << (bit & 0x1F);`), which MWCC keeps in r31 / r30 even in this leaf function; the array index set[word] is computed again from bit */

u32 cr_Bits_Test(u32 *set, u32 bit) {
    u32 word = bit >> 5;
    u32 mask = 1 << (bit & 0x1F);

    return mask & set[word];
}
