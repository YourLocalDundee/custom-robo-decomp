/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x801F9B00-0x801F9D08, 1 object, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   0x801F9B00 lbl_801F9B00 (cr_SkelSlot14[26], 0x208 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct cr_SkelSlot14
{
  u8 flags;
  u8 unk_01[3];
  f32 unk_04;
  u8 unk_08[4];
  f32 unk_0c;
  void *unk_10;
} cr_SkelSlot14;

/* what the pointers point at (names of decomp/config/GXCE01/symbols.txt) */
extern u8 lbl_8044C468[];
extern u8 lbl_8044C46C[];
extern u8 lbl_8044C46E[];
extern u8 lbl_8044C474[];
extern u8 lbl_8044C476[];
extern u8 lbl_8044C478[];
extern u8 lbl_8044C47A[];
extern u8 lbl_8044C47C[];
extern u8 lbl_8044C47E[];
extern u8 lbl_8044C480[];
extern u8 lbl_8044C482[];
extern u8 lbl_8044C484[];
extern u8 lbl_8044C486[];
extern u8 lbl_8044C48A[];
extern u8 lbl_8044C48C[];
extern u8 lbl_8044C48E[];
extern u8 lbl_8044C490[];
extern u8 lbl_8044C492[];
extern u8 lbl_8044C494[];
extern u8 lbl_8044C496[];
extern u8 lbl_8044C498[];

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
cr_SkelSlot14 lbl_801F9B00[26] = {
    {
        7,
        { 0, 0, 2 },
        0.0f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C468
    },
    {
        14,
        { 1, 0, 1 },
        0.0f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C46C
    },
    {
        17,
        { 0, 0, 3 },
        0.38f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C46E
    },
    {
        18,
        { 1, 0, 2 },
        0.02f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C474
    },
    {
        17,
        { 0, 0, 0 },
        0.21f,
        { 0, 0, 0, 0 },
        0.0f,
        NULL
    },
    {
        30,
        { 0, 0, 1 },
        -0.08f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C476
    },
    {
        30,
        { 2, 0, 1 },
        0.03f,
        { 0, 0, 0, 0 },
        -0.18f,
        lbl_8044C478
    },
    {
        28,
        { 0, 0, 1 },
        0.28f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C47A
    },
    {
        17,
        { 0, 0, 1 },
        0.26f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C47C
    },
    {
        30,
        { 0, 0, 0 },
        0.0f,
        { 0, 0, 0, 0 },
        -0.07f,
        NULL
    },
    {
        30,
        { 0, 0, 1 },
        -0.08f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C47E
    },
    {
        30,
        { 2, 0, 1 },
        0.03f,
        { 0, 0, 0, 0 },
        0.18f,
        lbl_8044C480
    },
    {
        28,
        { 0, 0, 1 },
        0.28f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C482
    },
    {
        17,
        { 0, 0, 1 },
        0.26f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C484
    },
    {
        30,
        { 0, 0, 0 },
        0.0f,
        { 0, 0, 0, 0 },
        0.07f,
        NULL
    },
    {
        30,
        { 1, 0, 2 },
        0.22f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C486
    },
    {
        30,
        { 2, 0, 1 },
        -0.03f,
        { 0, 0, 0, 0 },
        -0.09f,
        lbl_8044C48A
    },
    {
        28,
        { 1, 0, 1 },
        0.39f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C48C
    },
    {
        17,
        { 0, 1, 1 },
        0.44f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C48E
    },
    {
        14,
        { 0, 0, 1 },
        0.0f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C490
    },
    {
        30,
        { 0, 0, 0 },
        0.0f,
        { 189, 245, 194, 143 },
        -0.14f,
        NULL
    },
    {
        30,
        { 2, 0, 1 },
        -0.03f,
        { 0, 0, 0, 0 },
        0.09f,
        lbl_8044C492
    },
    {
        28,
        { 1, 0, 1 },
        0.39f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C494
    },
    {
        17,
        { 0, 1, 1 },
        0.44f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C496
    },
    {
        14,
        { 0, 0, 1 },
        0.0f,
        { 0, 0, 0, 0 },
        0.0f,
        lbl_8044C498
    },
    {
        30,
        { 0, 0, 0 },
        0.0f,
        { 189, 245, 194, 143 },
        -0.14f,
        NULL
    }
};
#pragma force_active reset
