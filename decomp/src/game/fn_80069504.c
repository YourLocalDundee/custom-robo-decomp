/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80069504: .text 0x80069504-0x80069570 (108 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct _GXColor
{
  u8 r;
  u8 g;
  u8 b;
  u8 a;
} GXColor;
extern u8 *lbl_8044F6AC;

/* fn_80069504: Nov 3, 2026 hand draft (m2c 51%): a GXColor copied from a table of colours (m2c: four u8 stack fields); the compiler copies it byte by byte through the stack */

s32 fn_80069504(void *arg0, void *arg1, s32 arg2) {
    GXColor c;
    GXColor *t;

    t = (GXColor *) (lbl_8044F6AC + 0xC);
    c = t[M2C_FIELD(arg0, u8 *, 5)];
    *(GXColor *) ((u8 *) arg1 + 0x18) = c;
    return arg2;
}
