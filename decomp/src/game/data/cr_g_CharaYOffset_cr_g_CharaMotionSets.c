/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .data 0x8021B080-0x8021B240, 2 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; the initialisers are main.dol's bytes, element by element.
 * The compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order, so the definitions are in that order:
 *   0x8021B080 cr_g_CharaYOffset (f32[56], 0xE0 bytes)
 *   0x8021B160 cr_g_CharaMotionSets (cr_MotionSet*[56], 0xE0 bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct cr_MotionSet cr_MotionSet;
struct cr_MotionSet
{
  u16 wait;
  u16 walk_fa;
  u16 run;
  u16 turn_l;
  u16 turn_r;
  u16 talk_a;
  u16 talk_b;
  u16 talk_c;
  u16 talk_d;
  u16 talk_e;
  u16 talk_question;
  u16 talk_yes;
  u16 talk_no;
  u16 talk_oh;
  u16 battle_in;
  u16 battle_wait;
  u16 battle_out;
};

/* what the pointers point at (names of decomp/config/GXCE01/symbols.txt) */
extern cr_MotionSet lbl_8021D364[];
extern cr_MotionSet lbl_8021D386[];
extern cr_MotionSet lbl_8021D3A8[];
extern cr_MotionSet lbl_8021D3CA[];
extern cr_MotionSet lbl_8021D3EC[];
extern cr_MotionSet lbl_8021D40E[];
extern cr_MotionSet lbl_8021D430[];
extern cr_MotionSet lbl_8021D452[];
extern cr_MotionSet lbl_8021D474[];

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
f32 cr_g_CharaYOffset[56] = {
    0.93103445f, 0.9741379f, 1.025862f, 1.0301723f, 1.0948275f, 1.025862f, 1.025862f, 1.025862f,
    1.025862f, 1.0f, 1.0517241f, 0.93103445f, 1.0517241f, 0.93103445f, 0.57931036f, 0.57931036f,
    0.7586207f, 0.9353448f, 0.9439655f, 0.9353448f, 0.8793103f, 1.0810344f, 1.0810344f, 0.6034483f,
    0.9353448f, 0.88793105f, 0.86206895f, 0.6637931f, 0.9137931f, 0.95689654f, 0.93103445f, 0.9741379f,
    0.95689654f, 0.9655172f, 0.9456896f, 1.0060345f, 0.93103445f, 0.9482758f, 0.9741379f, 0.93103445f,
    0.55172414f, 0.4741379f, 0.93103445f, 0.93103445f, 0.9439655f, 0.9439655f, 0.9741379f, 0.9655172f,
    1.0051724f, 0.9741379f, 0.9741379f, 1.025862f, 0.86206895f, 0.83620685f, 0.9741379f, 0.9741379f
};
cr_MotionSet *cr_g_CharaMotionSets[56] = {
    lbl_8021D364, lbl_8021D386, lbl_8021D430, lbl_8021D3EC, lbl_8021D40E, lbl_8021D474, lbl_8021D3A8,
    lbl_8021D452, lbl_8021D3EC, lbl_8021D3CA, lbl_8021D452, lbl_8021D364, lbl_8021D452, lbl_8021D364,
    lbl_8021D3EC, lbl_8021D3EC, lbl_8021D3EC, lbl_8021D3EC, lbl_8021D474, lbl_8021D3EC, lbl_8021D3EC,
    lbl_8021D3EC, lbl_8021D3EC, lbl_8021D3EC, lbl_8021D3EC, lbl_8021D474, lbl_8021D474, lbl_8021D3EC,
    lbl_8021D3EC, lbl_8021D3EC, lbl_8021D474, lbl_8021D40E, lbl_8021D3EC, lbl_8021D3EC, lbl_8021D474,
    lbl_8021D474, lbl_8021D3EC, lbl_8021D3EC, lbl_8021D3EC, lbl_8021D3EC, lbl_8021D3EC, lbl_8021D474,
    lbl_8021D3EC, lbl_8021D3EC, lbl_8021D474, lbl_8021D474, lbl_8021D3EC, lbl_8021D3EC, lbl_8021D452,
    lbl_8021D3EC, lbl_8021D3EC, lbl_8021D474, lbl_8021D474, lbl_8021D3EC, lbl_8021D3EC, lbl_8021D3EC
};
#pragma force_active reset
