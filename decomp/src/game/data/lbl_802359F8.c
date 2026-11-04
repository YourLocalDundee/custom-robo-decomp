/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x802359F8-0x80235DF8, 1 object, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   0x802359F8 lbl_802359F8 (cr_Record10[64], 0x400 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct cr_Record10
{
  u8 unk_0;
  u8 unk_1;
  u8 unk_2;
  u8 unk_3;
  f32 unk_4;
  u8 unk_8;
  u8 unk_9;
  u8 unk_a;
  u8 unk_b;
  f32 unk_c;
} cr_Record10;

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
cr_Record10 lbl_802359F8[64] = {
    { 0, 4, 23, 12, 0.0f, 0, 4, 23, 12, 0.0f },
    { 0, 8, 30, 12, 0.0f, 0, 4, 30, 12, 0.0f },
    { 0, 8, 22, 8, 0.0f, 0, 8, 19, 8, 0.0f },
    { 0, 4, 29, 11, 0.0f, 0, 4, 29, 11, 0.0f },
    { 0, 4, 26, 12, 0.0f, 0, 4, 26, 12, 0.0f },
    { 1, 130, 5, 1, 4.0f, 1, 130, 3, 1, 4.0f },
    { 0, 1, 35, 10, 0.0f, 0, 1, 31, 10, 0.0f },
    { 0, 16, 29, 15, 0.0f, 0, 16, 29, 15, 0.0f },
    { 0, 8, 30, 6, 0.0f, 0, 8, 24, 6, 0.0f },
    { 0, 16, 26, 15, 0.0f, 0, 16, 26, 15, 0.0f },
    { 0, 8, 20, 11, 0.0f, 0, 8, 20, 11, 0.0f },
    { 0, 8, 20, 10, 0.0f, 0, 8, 20, 10, 0.0f },
    { 1, 130, 13, 6, 5.0f, 1, 130, 14, 7, 5.0f },
    { 0, 8, 34, 5, 0.0f, 0, 8, 29, 5, 0.0f },
    { 0, 1, 60, 15, 0.0f, 0, 4, 70, 15, 0.0f },
    { 0, 8, 20, 8, 0.0f, 0, 4, 20, 8, 0.0f },
    { 0, 1, 27, 10, 0.0f, 0, 4, 22, 15, 0.0f },
    { 0, 16, 15, 7, 0.0f, 0, 16, 15, 7, 0.0f },
    { 0, 4, 29, 14, 0.0f, 0, 4, 26, 13, 0.0f },
    { 0, 4, 35, 15, 6.0f, 0, 8, 23, 15, 0.0f },
    { 0, 1, 25, 15, 0.0f, 0, 8, 23, 15, 0.0f },
    { 0, 4, 40, 15, 0.0f, 0, 8, 30, 15, 0.0f },
    { 0, 4, 32, 15, 0.0f, 0, 8, 25, 15, 0.0f },
    { 0, 2, 21, 10, 0.0f, 0, 2, 21, 10, 0.0f },
    { 0, 4, 60, 15, 0.0f, 0, 4, 79, 15, 0.0f },
    { 0, 8, 28, 15, 0.0f, 0, 8, 28, 15, 0.0f },
    { 1, 144, 9, 4, 4.0f, 1, 144, 6, 3, 4.0f },
    { 0, 8, 39, 5, 9.0f, 0, 8, 39, 5, 9.0f },
    { 0, 8, 23, 13, 0.0f, 0, 8, 13, 13, 0.0f },
    { 0, 16, 27, 15, 0.0f, 0, 16, 27, 15, 0.0f },
    { 0, 2, 76, 6, 0.0f, 0, 4, 27, 15, 0.0f },
    { 0, 16, 24, 15, 1e+01f, 1, 132, 13, 6, 5.0f },
    { 1, 136, 1, 6, 6.0f, 0, 1, 28, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f },
    { 0, 2, 24, 12, 0.0f, 0, 2, 24, 12, 0.0f },
    { 0, 2, 24, 12, 0.0f, 0, 2, 24, 12, 0.0f },
    { 1, 132, 15, 7, 7.0f, 1, 132, 15, 7, 7.0f },
    { 0, 8, 23, 5, 0.0f, 0, 4, 27, 5, 0.0f },
    { 0, 16, 25, 12, 8.0f, 0, 16, 25, 12, 8.0f },
    { 0, 1, 29, 7, 0.0f, 0, 1, 29, 7, 0.0f },
    { 0, 1, 29, 7, 0.0f, 0, 1, 29, 7, 0.0f },
    { 0, 132, 120, 15, 1e+01f, 1, 130, 25, 15, 5.0f },
    { 1, 130, 5, 2, 5.0f, 1, 130, 5, 2, 5.0f },
    { 0, 1, 139, 6, 0.0f, 0, 1, 139, 6, 0.0f },
    { 0, 4, 23, 15, 0.0f, 0, 4, 17, 15, 0.0f },
    { 0, 8, 60, 15, 0.0f, 0, 4, 27, 15, 0.0f },
    { 0, 4, 20, 15, 0.0f, 0, 4, 20, 15, 0.0f },
    { 0, 8, 24, 13, 0.0f, 0, 8, 24, 13, 0.0f },
    { 0, 8, 24, 14, 0.0f, 0, 8, 24, 14, 0.0f },
    { 0, 16, 27, 15, 0.0f, 0, 16, 27, 15, 0.0f },
    { 0, 16, 27, 15, 0.0f, 0, 16, 27, 15, 0.0f },
    { 0, 16, 27, 15, 0.0f, 0, 16, 27, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f },
    { 0, 4, 25, 15, 0.0f, 0, 4, 25, 15, 0.0f }
};
#pragma force_active reset
