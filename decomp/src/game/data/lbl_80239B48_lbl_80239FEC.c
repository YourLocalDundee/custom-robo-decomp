/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x80239B48-0x8023A004, 17 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   0x80239B48 lbl_80239B48 (f32[4][2], 0x20 bytes)
 *   0x80239B68 lbl_80239B68 (f32[4], 0x10 bytes)
 *   0x80239B78 lbl_80239B78 (cr_F32x10Pad, 0x30 bytes)
 *   0x80239BA8 lbl_80239BA8 (f32[20][6], 0x1E0 bytes)
 *   0x80239D88 lbl_80239D88 (f32[10][6], 0xF0 bytes)
 *   0x80239E78 lbl_80239E78 (f32[6][2], 0x30 bytes)
 *   0x80239EA8 lbl_80239EA8 (f32[5][2], 0x28 bytes)
 *   0x80239ED0 lbl_80239ED0 (f32[5][2], 0x28 bytes)
 *   0x80239EF8 lbl_80239EF8 (f32[5][2], 0x28 bytes)
 *   0x80239F20 lbl_80239F20 (f32[5], 0x14 bytes)
 *   0x80239F34 lbl_80239F34 (f32[4], 0x10 bytes)
 *   0x80239F44 lbl_80239F44 (f32[8], 0x20 bytes)
 *   0x80239F64 lbl_80239F64 (f32[6], 0x18 bytes)
 *   0x80239F7C lbl_80239F7C (cr_TextRecord[2], 0x40 bytes)
 *   0x80239FBC lbl_80239FBC (cr_TextRecordList[2], 0x10 bytes)
 *   0x80239FCC lbl_80239FCC (cr_TextBlock, 0x20 bytes)
 *   0x80239FEC lbl_80239FEC (f32[3][2], 0x18 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct cr_F32x10Pad
{
  f32 v[10];
  u8 unk_28[8];
} cr_F32x10Pad;
typedef struct cr_TextRecord
{
  f32 unk_00;
  f32 unk_04;
  u16 unk_08;
  u8 unk_0a[2];
  u16 unk_0c;
  u16 unk_0e;
  u16 unk_10;
  u16 unk_12;
  u8 unk_14[4];
  u8 unk_18;
  u8 unk_19;
  u8 unk_1a;
  u8 unk_1b;
  u8 unk_1c[4];
} cr_TextRecord;
typedef struct cr_TextRecordList
{
  u16 code;
  u8 unk_2[2];
  cr_TextRecord *records;
} cr_TextRecordList;
typedef struct cr_TextBlock
{
  f32 unk_00;
  f32 unk_04;
  f32 unk_08;
  u32 unk_0c;
  u32 unk_10;
  u32 unk_14;
  u8 unk_18[4];
  cr_TextRecordList *lists;
} cr_TextBlock;

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
f32 lbl_80239B48[4][2] = {
    { 0.0f, 0.0f },
    { 0.0f, 199.0f },
    { 303.0f, 0.0f },
    { 303.0f, 199.0f }
};
f32 lbl_80239B68[4] = { 33.0f, 193.0f, 181.0f, 8e+01f };
cr_F32x10Pad lbl_80239B78 = {
    {
        245.0f, 1.9e+02f, 33.0f, 0.0f, -15.0f, 0.0f, -33.0f, 23.0f, 25.0f, 34.0f
    },
    { 0, 0, 0, 0, 0, 0, 0, 0 }
};
f32 lbl_80239BA8[20][6] = {
    { 0.0f, 0.0f, 195.0f, 162.0f, 0.0f, 7.0f },
    { 0.0f, 0.0f, 181.0f, 7.0f, 7.0f, 7.0f },
    { 0.0f, 0.0f, 7.0f, 155.0f, 0.0f, 14.0f },
    { 0.0f, 0.0f, 181.0f, 7.0f, 7.0f, 169.0f },
    { 0.0f, 0.0f, 7.0f, 155.0f, 188.0f, 14.0f },
    { 0.0f, 0.0f, 7.0f, 7.0f, 0.0f, 7.0f },
    { 7.0f, 0.0f, 7.0f, 7.0f, 188.0f, 7.0f },
    { 14.0f, 0.0f, 7.0f, 7.0f, 0.0f, 169.0f },
    { 21.0f, 0.0f, 7.0f, 7.0f, 188.0f, 169.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 2.8e+02f, 162.0f, 0.0f, 7.0f },
    { 0.0f, 0.0f, 266.0f, 7.0f, 7.0f, 7.0f },
    { 0.0f, 0.0f, 7.0f, 155.0f, 0.0f, 14.0f },
    { 0.0f, 0.0f, 266.0f, 7.0f, 7.0f, 169.0f },
    { 0.0f, 0.0f, 7.0f, 155.0f, 273.0f, 14.0f },
    { 0.0f, 0.0f, 7.0f, 7.0f, 0.0f, 7.0f },
    { 7.0f, 0.0f, 7.0f, 7.0f, 273.0f, 7.0f },
    { 14.0f, 0.0f, 7.0f, 7.0f, 0.0f, 169.0f },
    { 21.0f, 0.0f, 7.0f, 7.0f, 273.0f, 169.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
};
f32 lbl_80239D88[10][6] = {
    { 0.0f, 0.0f, 181.0f, 26.0f, 7.0f, 7.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
};
f32 lbl_80239E78[6][2] = {
    { 33.0f, 44.0f },
    { 33.0f, 75.0f },
    { 33.0f, 108.0f },
    { 33.0f, 139.0f },
    { 33.0f, 171.0f },
    { 22.0f, 199.0f }
};
f32 lbl_80239EA8[5][2] = {
    { 238.0f, 45.0f },
    { 234.0f, 98.0f },
    { 296.0f, 98.0f },
    { 238.0f, 44.0f },
    { 246.0f, 146.0f }
};
f32 lbl_80239ED0[5][2] = {
    { 18.0f, 44.0f },
    { 18.0f, 75.0f },
    { 18.0f, 108.0f },
    { 18.0f, 139.0f },
    { 18.0f, 171.0f }
};
f32 lbl_80239EF8[5][2] = {
    { 39.0f, 72.0f },
    { 39.0f, 72.0f },
    { 39.0f, 72.0f },
    { 39.0f, 72.0f },
    { 0.0f, 0.0f }
};
f32 lbl_80239F20[5] = { 12.0f, 12.0f, 12.0f, 12.0f, 0.0f };
f32 lbl_80239F34[4] = { 15.0f, 131.0f, 201.0f, 0.0f };
f32 lbl_80239F44[8] = { -5.0f, -5.0f, 623.0f, 0.0f, 1.0f, 1.0f, 0.0f, 438.0f };
f32 lbl_80239F64[6] = { 38.0f, 1.1e+02f, 0.0f, 29.0f, 0.0f, 59.0f };
cr_TextRecord lbl_80239F7C[2] = {
    {
        0.0f,
        0.0f,
        0,
        { 0, 0 },
        300,
        200,
        49,
        0,
        { 0, 0, 0, 0 },
        0,
        0,
        0,
        0,
        { 0, 0, 0, 0 }
    },
    {
        0.0f,
        0.0f,
        0,
        { 0, 0 },
        0,
        0,
        0xFFFF,
        0,
        { 0, 0, 0, 0 },
        0,
        0,
        0,
        0,
        { 0, 0, 0, 0 }
    }
};
cr_TextRecordList lbl_80239FBC[2] = {
    {
        0,
        { 0, 0 },
        lbl_80239F7C
    },
    {
        0xFFFE,
        { 0, 0 },
        NULL
    }
};
cr_TextBlock lbl_80239FCC = {
    324.0f,
    239.0f,
    1.0f,
    0,
    0,
    0,
    { 0, 0, 0, 0 },
    lbl_80239FBC
};
f32 lbl_80239FEC[3][2] = {
    { 234.0f, 69.0f },
    { 2.3e+02f, 153.0f },
    { 19.0f, 196.0f }
};
#pragma force_active reset
