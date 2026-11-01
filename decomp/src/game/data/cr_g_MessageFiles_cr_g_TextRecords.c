/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x80330BC0-0x8034454C, 3 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x80330D4C cr_g_TextRecords (cr_TextRecord[2496], 0x13800 bytes)
 *   0x80330D1C cr_g_WindowStyle (cr_WindowStyle, 0x30 bytes)
 *   0x80330BC0 cr_g_MessageFiles (u32[87], 0x15C bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
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
typedef struct cr_TextRecord
{
  u8 unk_00[0x10];
  u16 unk_10;
  u8 unk_12[0xE];
} cr_TextRecord;

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
cr_TextRecord cr_g_TextRecords[2496];
cr_WindowStyle cr_g_WindowStyle;
u32 cr_g_MessageFiles[87];
#pragma force_active reset
