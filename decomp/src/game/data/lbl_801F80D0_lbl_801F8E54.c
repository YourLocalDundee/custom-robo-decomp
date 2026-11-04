/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x801F80D0-0x801F8E74, 71 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   1 x char*[], 2 x cr_CamRect*[], 20 x cr_CamRect[], 33 x cr_Key7[], 1 x cr_KeyTrack*[], 6 x cr_KeyTrack[], 2 x f32*[], 6 x f32[] (each object's type: symbols/lbl_types.txt / cr_globals.txt / bss_untyped.txt)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct cr_Key7
{
  f32 unk_00;
  f32 unk_04;
  f32 unk_08;
  f32 unk_0c;
  f32 unk_10;
  f32 unk_14;
  f32 unk_18;
} cr_Key7;
typedef struct cr_KeyTrack
{
  cr_Key7 *keys;
  f32 count;
} cr_KeyTrack;
typedef struct cr_CamRect
{
  f32 unk_0;
  f32 unk_4;
  f32 unk_8;
  f32 unk_c;
} cr_CamRect;

/* what the pointers point at (names of decomp/config/GXCE01/symbols.txt) */
extern char lbl_801BC928[];
extern char lbl_801BC936[];
extern char lbl_801BC945[];
extern char lbl_801BC954[];
extern char lbl_801BC964[];
extern char lbl_801BC975[];
extern char lbl_801BC987[];
extern char lbl_801BC996[];
extern char lbl_801BC9A8[];
extern char lbl_801BC9B9[];
extern char lbl_801BC9C6[];
extern char lbl_801BC9D3[];
extern char lbl_801BC9E5[];
extern char lbl_801BC9FB[];
extern char lbl_801BCA12[];
extern char lbl_801BCA28[];
extern char lbl_801BCA3D[];
extern f32 lbl_80321DD0[];
extern f32 lbl_80321E00[];
extern f32 lbl_80321E30[];
extern f32 lbl_80321E60[];
extern f32 lbl_8044C43C[];
extern f32 lbl_8044C444[];

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
cr_Key7 lbl_801F80D0[5] = {
    { 0.0f, 0.0f, 0.0f, 47.5f, 0.5f, 19.3f, 26.5f },
    { 0.0f, 0.5f, 19.3f, 26.5f, 1.0f, 24.6f, -3.5f },
    { 0.0f, 1.0f, 24.6f, -3.5f, 1.5f, 18.2f, -20.5f },
    { 0.0f, 1.5f, 18.2f, -20.5f, 2.5f, 1.2f, -5.75f },
    { 0.0f, 2.5f, 1.2f, -5.75f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F815C[1] = {
    { 0.0f, 0.0f, 26.7f, -12.5f, 3.0f, 1.8f, 0.0f }
};
cr_Key7 lbl_801F8178[4] = {
    { 0.0f, 0.0f, -3e+01f, 0.0f, 1.0f, -4.2f, 32.5f },
    { 0.0f, 1.0f, -4.2f, 32.5f, 1.5f, 8.3f, 17.0f },
    { 0.0f, 1.5f, 8.3f, 17.0f, 2.5f, 1e+01f, -6.5f },
    { 0.0f, 2.5f, 1e+01f, -6.5f, 3.0f, 8.0f, 0.0f }
};
cr_Key7 lbl_801F81E8[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F8204[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F8220[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_KeyTrack lbl_801F823C[6] = {
    { lbl_801F80D0, 5.0f },
    { lbl_801F815C, 1.0f },
    { lbl_801F8178, 4.0f },
    { lbl_801F81E8, 1.0f },
    { lbl_801F8204, 1.0f },
    { lbl_801F8220, 1.0f }
};
cr_Key7 lbl_801F826C[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F8288[1] = {
    { 0.0f, 0.0f, 26.7f, -25.0f, 3.0f, 1.8f, 0.0f }
};
cr_Key7 lbl_801F82A4[1] = {
    { 0.0f, 0.0f, 3e+01f, -15.0f, 3.0f, 8.0f, 0.0f }
};
cr_Key7 lbl_801F82C0[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F82DC[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F82F8[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_KeyTrack lbl_801F8314[6] = {
    { lbl_801F826C, 1.0f },
    { lbl_801F8288, 1.0f },
    { lbl_801F82A4, 1.0f },
    { lbl_801F82C0, 1.0f },
    { lbl_801F82DC, 1.0f },
    { lbl_801F82F8, 1.0f }
};
cr_Key7 lbl_801F8344[5] = {
    { 0.0f, 0.0f, 0.0f, -47.5f, 0.5f, -19.3f, -26.5f },
    { 0.0f, 0.5f, -19.3f, -26.5f, 1.0f, -24.6f, 3.5f },
    { 0.0f, 1.0f, -24.6f, 3.5f, 1.5f, -18.2f, 20.5f },
    { 0.0f, 1.5f, -18.2f, 20.5f, 2.5f, -1.2f, 5.75f },
    { 0.0f, 2.5f, -1.2f, 5.75f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F83D0[1] = {
    { 0.0f, 0.0f, 26.7f, -12.5f, 3.0f, 1.8f, 0.0f }
};
cr_Key7 lbl_801F83EC[4] = {
    { 0.0f, 0.0f, -3e+01f, 0.0f, 1.0f, -4.2f, 32.5f },
    { 0.0f, 1.0f, -4.2f, 32.5f, 1.5f, 8.3f, 17.0f },
    { 0.0f, 1.5f, 8.3f, 17.0f, 2.5f, 1e+01f, -6.5f },
    { 0.0f, 2.5f, 1e+01f, -6.5f, 3.0f, 8.0f, 0.0f }
};
cr_Key7 lbl_801F845C[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F8478[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F8494[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_KeyTrack lbl_801F84B0[6] = {
    { lbl_801F8344, 5.0f },
    { lbl_801F83D0, 1.0f },
    { lbl_801F83EC, 4.0f },
    { lbl_801F845C, 1.0f },
    { lbl_801F8478, 1.0f },
    { lbl_801F8494, 1.0f }
};
cr_Key7 lbl_801F84E0[5] = {
    { 0.0f, 0.0f, 0.0f, -47.5f, 0.5f, -19.3f, -26.5f },
    { 0.0f, 0.5f, -19.3f, -26.5f, 1.0f, -24.6f, 3.5f },
    { 0.0f, 1.0f, -24.6f, 3.5f, 1.5f, -18.2f, 20.5f },
    { 0.0f, 1.5f, -18.2f, 20.5f, 2.5f, -1.2f, 5.75f },
    { 0.0f, 2.5f, -1.2f, 5.75f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F856C[1] = {
    { 0.0f, 0.0f, 75.0f, -5e+01f, 3.0f, 1.8f, 0.0f }
};
cr_Key7 lbl_801F8588[4] = {
    { 0.0f, 0.0f, -3e+01f, 0.0f, 1.0f, -4.2f, 32.5f },
    { 0.0f, 1.0f, -4.2f, 32.5f, 1.5f, 8.3f, 17.0f },
    { 0.0f, 1.5f, 8.3f, 17.0f, 2.5f, 1e+01f, -6.5f },
    { 0.0f, 2.5f, 1e+01f, -6.5f, 3.0f, 8.0f, 0.0f }
};
cr_Key7 lbl_801F85F8[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F8614[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F8630[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_KeyTrack lbl_801F864C[6] = {
    { lbl_801F84E0, 5.0f },
    { lbl_801F856C, 1.0f },
    { lbl_801F8588, 4.0f },
    { lbl_801F85F8, 1.0f },
    { lbl_801F8614, 1.0f },
    { lbl_801F8630, 1.0f }
};
cr_Key7 lbl_801F867C[5] = {
    { 0.0f, 0.0f, 0.0f, -47.5f, 0.5f, -19.3f, -26.5f },
    { 0.0f, 0.5f, -19.3f, -26.5f, 1.0f, -24.6f, 3.5f },
    { 0.0f, 1.0f, -24.6f, 3.5f, 1.5f, -18.2f, 20.5f },
    { 0.0f, 1.5f, -18.2f, 20.5f, 2.5f, -1.2f, 5.75f },
    { 0.0f, 2.5f, -1.2f, 5.75f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F8708[1] = {
    { 0.0f, 0.0f, 5.0f, 0.0f, 3.0f, 1.8f, 0.0f }
};
cr_Key7 lbl_801F8724[4] = {
    { 0.0f, 0.0f, -3e+01f, 0.0f, 1.0f, -4.2f, 32.5f },
    { 0.0f, 1.0f, -4.2f, 32.5f, 1.5f, 8.3f, 17.0f },
    { 0.0f, 1.5f, 8.3f, 17.0f, 2.5f, 1e+01f, -6.5f },
    { 0.0f, 2.5f, 1e+01f, -6.5f, 3.0f, 8.0f, 0.0f }
};
cr_Key7 lbl_801F8794[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F87B0[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F87CC[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_KeyTrack lbl_801F87E8[6] = {
    { lbl_801F867C, 5.0f },
    { lbl_801F8708, 1.0f },
    { lbl_801F8724, 4.0f },
    { lbl_801F8794, 1.0f },
    { lbl_801F87B0, 1.0f },
    { lbl_801F87CC, 1.0f }
};
cr_KeyTrack *lbl_801F8818[4] = { lbl_801F823C, lbl_801F8314, lbl_801F864C, lbl_801F87E8 };
cr_Key7 lbl_801F8828[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, -5.0f, 0.0f }
};
cr_Key7 lbl_801F8844[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }
};
cr_Key7 lbl_801F8860[1] = {
    { 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, 5.0f, 0.0f }
};
cr_KeyTrack lbl_801F887C[3] = {
    { lbl_801F8828, 1.0f },
    { lbl_801F8844, 1.0f },
    { lbl_801F8860, 1.0f }
};
f32 *lbl_801F8894[4] = { lbl_80321E60, lbl_80321E30, lbl_80321E00, lbl_80321DD0 };
char *cr_g_CamModeNames[18] = {
    lbl_801BC928, lbl_801BC936, lbl_801BC945, lbl_801BC954, lbl_801BC964, lbl_801BC975, lbl_801BC987,
    lbl_801BC996, lbl_801BC9A8, lbl_801BC9B9, lbl_801BC9C6, lbl_801BC9D3, lbl_801BC9E5, lbl_801BC9FB,
    lbl_801BCA12, lbl_801BCA28, lbl_801BCA3D, lbl_801BCA3D
};
f32 lbl_801F88EC[4] = { 1.26f, 1.26f, 1.26f, 1.26f };
f32 lbl_801F88FC[3] = { 0.63f, 1.26f, 1.26f };
f32 lbl_801F8908[3] = { 1.26f, 1.26f, 0.63f };
f32 lbl_801F8914[3] = { 2.52f, 1.26f, 1.26f };
f32 lbl_801F8920[3] = { 1.26f, 1.26f, 2.52f };
f32 lbl_801F892C[4] = { 1.26f, 1.26f, 1.26f, 1.26f };
f32 *lbl_801F893C[8] = {
    lbl_801F88EC, lbl_8044C43C, lbl_8044C444, lbl_801F88FC, lbl_801F8908, lbl_801F8914, lbl_801F8920,
    lbl_801F892C
};
cr_CamRect lbl_801F895C[2] = {
    { 0.0f, 6.4e+02f, 0.0f, 4.8e+02f },
    { 228.0f, 6.2e+02f, 4e+01f, 284.0f }
};
cr_CamRect lbl_801F897C[4] = {
    { 0.0f, 6.4e+02f, 0.0f, 4.8e+02f },
    { 0.0f, 6.4e+02f, 0.0f, 4.8e+02f },
    { 0.0f, 6.4e+02f, 0.0f, 4.8e+02f },
    { 0.0f, 6.4e+02f, 0.0f, 4.8e+02f }
};
cr_CamRect lbl_801F89BC[2] = {
    { 0.0f, 318.0f, 0.0f, 4.8e+02f },
    { 322.0f, 6.4e+02f, 0.0f, 4.8e+02f }
};
cr_CamRect lbl_801F89DC[2] = {
    { 0.0f, 6.4e+02f, 0.0f, 238.0f },
    { 0.0f, 6.4e+02f, 242.0f, 4.8e+02f }
};
cr_CamRect lbl_801F89FC[3] = {
    { 0.0f, 318.0f, 0.0f, 4.8e+02f },
    { 322.0f, 6.4e+02f, 0.0f, 238.0f },
    { 322.0f, 6.4e+02f, 242.0f, 4.8e+02f }
};
cr_CamRect lbl_801F8A2C[3] = {
    { 0.0f, 318.0f, 0.0f, 238.0f },
    { 0.0f, 318.0f, 242.0f, 4.8e+02f },
    { 322.0f, 6.4e+02f, 0.0f, 4.8e+02f }
};
cr_CamRect lbl_801F8A5C[3] = {
    { 0.0f, 6.4e+02f, 0.0f, 238.0f },
    { 0.0f, 318.0f, 242.0f, 4.8e+02f },
    { 322.0f, 6.4e+02f, 242.0f, 4.8e+02f }
};
cr_CamRect lbl_801F8A8C[3] = {
    { 0.0f, 318.0f, 0.0f, 238.0f },
    { 322.0f, 6.4e+02f, 0.0f, 238.0f },
    { 0.0f, 6.4e+02f, 242.0f, 4.8e+02f }
};
cr_CamRect lbl_801F8ABC[6] = {
    { 0.0f, 318.0f, 0.0f, 238.0f },
    { 0.0f, 318.0f, 242.0f, 4.8e+02f },
    { 322.0f, 6.4e+02f, 0.0f, 238.0f },
    { 322.0f, 6.4e+02f, 242.0f, 4.8e+02f },
    { 3.3e+02f, 621.0f, 42.0f, 439.0f },
    { 19.0f, 621.0f, 42.0f, 439.0f }
};
cr_CamRect lbl_801F8B1C[4] = {
    { 323.0f, 6.3e+02f, 2e+01f, 2.4e+02f },
    { 323.0f, 6.3e+02f, 2.4e+02f, 4.4e+02f },
    { 19.0f, 3.2e+02f, 42.0f, 438.0f },
    { 3.2e+02f, 621.0f, 42.0f, 438.0f }
};
cr_CamRect lbl_801F8B5C[8] = {
    { 105.0f, 3.2e+02f, 3e+01f, 235.0f },
    { 105.0f, 3.2e+02f, 2.5e+02f, 4.5e+02f },
    { 445.0f, 6.3e+02f, 3e+01f, 235.0f },
    { 445.0f, 6.3e+02f, 2.5e+02f, 4.5e+02f },
    { 19.0f, 3.2e+02f, 42.0f, 2.4e+02f },
    { 19.0f, 3.2e+02f, 2.4e+02f, 438.0f },
    { 3.2e+02f, 621.0f, 42.0f, 2.4e+02f },
    { 3.2e+02f, 621.0f, 2.4e+02f, 438.0f }
};
cr_CamRect *lbl_801F8BDC[10] = {
    lbl_801F897C, lbl_801F89BC, lbl_801F89DC, lbl_801F89FC, lbl_801F8A2C, lbl_801F8A5C, lbl_801F8A8C,
    lbl_801F8ABC, lbl_801F8B1C, lbl_801F8B5C
};
cr_CamRect lbl_801F8C04[2] = {
    { 0.0f, 6.4e+02f, 0.0f, 4.8e+02f },
    { 228.0f, 6.2e+02f, 4e+01f, 284.0f }
};
cr_CamRect lbl_801F8C24[4] = {
    { 0.0f, 6.4e+02f, 0.0f, 4.8e+02f },
    { 0.0f, 6.4e+02f, 0.0f, 4.8e+02f },
    { 0.0f, 6.4e+02f, 0.0f, 4.8e+02f },
    { 0.0f, 6.4e+02f, 0.0f, 4.8e+02f }
};
cr_CamRect lbl_801F8C64[2] = {
    { 0.0f, 3.2e+02f, 0.0f, 4.8e+02f },
    { 3.2e+02f, 6.4e+02f, 0.0f, 4.8e+02f }
};
cr_CamRect lbl_801F8C84[2] = {
    { 0.0f, 6.4e+02f, 0.0f, 2.4e+02f },
    { 0.0f, 6.4e+02f, 2.4e+02f, 4.8e+02f }
};
cr_CamRect lbl_801F8CA4[3] = {
    { 0.0f, 3.2e+02f, 0.0f, 4.8e+02f },
    { 3.2e+02f, 6.4e+02f, 0.0f, 2.4e+02f },
    { 3.2e+02f, 6.4e+02f, 2.4e+02f, 4.8e+02f }
};
cr_CamRect lbl_801F8CD4[3] = {
    { 0.0f, 3.2e+02f, 0.0f, 2.4e+02f },
    { 0.0f, 3.2e+02f, 2.4e+02f, 4.8e+02f },
    { 3.2e+02f, 6.4e+02f, 0.0f, 4.8e+02f }
};
cr_CamRect lbl_801F8D04[3] = {
    { 0.0f, 6.4e+02f, 0.0f, 2.4e+02f },
    { 0.0f, 3.2e+02f, 2.4e+02f, 4.8e+02f },
    { 3.2e+02f, 6.4e+02f, 2.4e+02f, 4.8e+02f }
};
cr_CamRect lbl_801F8D34[3] = {
    { 0.0f, 3.2e+02f, 0.0f, 2.4e+02f },
    { 3.2e+02f, 6.4e+02f, 0.0f, 2.4e+02f },
    { 0.0f, 6.4e+02f, 2.4e+02f, 4.8e+02f }
};
cr_CamRect lbl_801F8D64[15] = {
    { 0.0f, 3.2e+02f, 0.0f, 2.4e+02f },
    { 0.0f, 3.2e+02f, 2.4e+02f, 4.8e+02f },
    { 3.2e+02f, 6.4e+02f, 0.0f, 2.4e+02f },
    { 3.2e+02f, 6.4e+02f, 2.4e+02f, 4.8e+02f },
    { 1.6e+02f, 8e+02f, 2e+01f, 5e+02f },
    { 3.2e+02f, 6.4e+02f, 2e+01f, 2.6e+02f },
    { 3.2e+02f, 6.4e+02f, 2.2e+02f, 4.6e+02f },
    { 105.0f, 425.0f, 0.0f, 2.4e+02f },
    { 105.0f, 425.0f, 2e+02f, 4.4e+02f },
    { 415.0f, 735.0f, 0.0f, 2.4e+02f },
    { 415.0f, 735.0f, 2e+02f, 4.4e+02f },
    { 0.0f, 315.0f, 2e+01f, 2.4e+02f },
    { 0.0f, 315.0f, 2.4e+02f, 4.4e+02f },
    { 325.0f, 6.3e+02f, 2e+01f, 2.4e+02f },
    { 325.0f, 6.3e+02f, 2.4e+02f, 4.4e+02f }
};
cr_CamRect *lbl_801F8E54[8] = {
    lbl_801F8C24, lbl_801F8C64, lbl_801F8C84, lbl_801F8CA4, lbl_801F8CD4, lbl_801F8D04, lbl_801F8D34,
    lbl_801F8D64
};
#pragma force_active reset
