/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x80324BD0-0x80329B60, 2 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x80329B10 cr_g_CamSystem (cr_CamSystem, 0x50 bytes)
 *   0x80324BD0 cr_g_Cams (u8[20288], 0x4F40 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct cr_CamSystem
{
  u8 unk_00[8];
  u8 unk_08;
  u8 unk_09;
  u8 unk_0a[2];
  u8 *cams[4];
  u8 *unk_1c[4];
  u8 unk_2c;
  u8 unk_2d[3];
  u8 unk_30;
  u8 unk_31[3];
  u32 unk_34;
  u32 unk_38;
  u32 unk_3c;
  u32 unk_40;
  u32 unk_44;
  u32 unk_48[2];
} cr_CamSystem;

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
cr_CamSystem cr_g_CamSystem;
u8 cr_g_Cams[20288];
#pragma force_active reset
