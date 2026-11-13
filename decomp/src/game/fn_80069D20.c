/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80069D20: .text 0x80069D20-0x80069DA4 (132 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_WindowStyle
{
  u8 unk_00;
  u8 unk_01[3];
  f32 unk_04;
  f32 unk_08;
  u8 unk_0c;
  u8 unk_0d;
  u8 unk_0e[6];
  f32 unk_14;
  u8 unk_18[4];
  u8 unk_1c;
  u8 unk_1d[7];
  f32 unk_24;
  u8 unk_28;
  u8 unk_29[3];
  u32 unk_2c;
} cr_WindowStyle;
__declspec(section ".data") extern cr_WindowStyle cr_g_WindowStyle;
void fn_80068978(u8 *arg0);
extern u32 lbl_80396484[3];
extern u8 lbl_80396478[12];

/* fn_80069D20: Nov 13, 2026 hand draft: window text lookup: (u8 *) table[a] + off + 8, left to right (grouping (off + 8) is reassociated) */

s32 fn_80069D20(u8 a, s32 id) {
    u32 off;
    u8 *p;

    cr_g_WindowStyle.unk_2c = (u32) fn_80068978;
    off = ((struct { u32 off; } *) (((u32 *) lbl_80396484)[a] + 0x10))[id].off;
    if (off != 0) {
        p = (u8 *) ((u32 *) lbl_80396478)[a] + off + 8;
        return (s32) p;
    }
    return 0;
}
