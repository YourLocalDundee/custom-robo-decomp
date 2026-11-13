/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8006B0AC: .text 0x8006B0AC-0x8006B140 (148 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 *lbl_8044F5C0;
extern const f32 lbl_80450AAC;
extern u32 lbl_8044F5C4;

/* fn_8006B0AC: Nov 13, 2026 hand draft: m2c 43%: a 32-byte point record (x, y at 0 / 4, id at 0x10) indexed with a u8 counter, the list pointer and the counter named locals (r31 / r30), the null test against `((void *) 0)` */

typedef struct cr_Pt20 {
    f32 x;
    f32 y;
    u8 pad[8];
    u16 id;
    u8 rest[0xE];
} cr_Pt20;

void fn_8006B0AC(void) {
    cr_Pt20 *v;
    u8 i;

    if (lbl_8044F5C0 != ((void *) 0)) {
        i = 0;
        v = (cr_Pt20 *) M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_8044F5C0, void **, 0x5C), void **, 0x1C), s32 *, 4);
        while (v[i].id != 0xFFFF) {
            v[i].id = 0xFFFF;
            v[i].x = lbl_80450AAC;
            v[i].y = lbl_80450AAC;
            i++;
        }
    }
    lbl_8044F5C4 = 0;
}
