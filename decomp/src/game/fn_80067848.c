/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80067848: .text 0x80067848-0x80067904 (188 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* fn_80067848: Nov 13, 2026 hand draft: m2c 63%: find the first vertex record with flag 0x8000, then colour count records from there: a 32-byte record type (flags at 0x12, r g b a at 0x18), s32 / u32 counters */

typedef struct cr_Vtx20 {
    u8 pad[0x12];
    u16 flags;
    u8 pad2[4];
    u8 r, g, b, a;
    u8 rest[4];
} cr_Vtx20;

void fn_80067848(u8 *col, s16 start, s16 count, void *dobj) {
    cr_Vtx20 *v;
    s32 i;
    u32 j;

    i = 0;
    v = (cr_Vtx20 *) M2C_FIELD(M2C_FIELD(dobj, void **, 0x1C), s32 *, 4);
    while (!(v[i].flags & 0x8000)) {
        i++;
    }
    i += start;
    for (j = 0; j < count; j++) {
        v[i + j].r = col[0];
        v[i + j].g = col[1];
        v[i + j].b = col[2];
        v[i + j].a = col[3];
    }
}
