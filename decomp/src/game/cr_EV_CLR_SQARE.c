/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_EV_CLR_SQARE: .text 0x800CB7B4-0x800CB8D8 (292 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef Vec Vec3;
typedef struct cr_AreaBox
{
  u32 flags;
  Vec3 min;
  Vec3 max;
  u8 unk_1c[0xE];
  u16 chara;
  u8 unk_2c[0x14];
} cr_AreaBox;
__declspec(section ".data") extern cr_AreaBox cr_g_AreaBoxes[28];
__declspec(section ".sdata") extern s16 *cr_g_ScriptPC;
typedef struct cr_ScriptState
{
  u32 unk_000;
  u32 history_init;
  u32 history_index;
  u32 cmd_count[16];
  s32 cmd_history[64];
  u32 map_bef_script[50];
  u32 map_init_script[50];
  u32 map_no_enter_script[50];
  void *script_name[16];
  u32 unk_3e4;
  u8 unk_3e8;
  u8 unk_3e9[3];
  Vec3 char_pos[8];
  s32 unk_44c;
  s32 unk_450;
  s32 unk_454;
  u32 call_count;
  s32 battle_arg_slot0;
  s32 battle_arg;
  u16 unk_464;
  u8 unk_466;
  u8 unk_467;
  u8 unk_468;
  u8 unk_469;
  u8 unk_46a;
  u8 unk_46b;
  u32 unk_46c;
  u8 unk_470;
  u8 unk_471[5];
  u8 unk_476;
  u8 unk_477;
  u32 overlay_478;
  u32 overlay_47c;
  u32 overlay_480;
} cr_ScriptState;
__declspec(section ".data") extern cr_ScriptState cr_g_Script;
__declspec(section ".sdata") extern s32 cr_g_ScriptDepth;
void fn_80023394(s8 *arg0, u32 arg1);

/* cr_EV_CLR_SQARE: Nov 5, 2026 hand draft: clears the flags of the area box whose id is the command argument; the head rule plus 1 dead use of the PC copy in its `if (0)` block (the PC copy then outranks the found pointer, as in the original) */
static inline s8 *cr_EV_CLR_SQARE_search0(s32 id) {
    u16 var_r31_12;

    for (var_r31_12 = 0; var_r31_12 < 0x1CU; var_r31_12 += 1) {
        if ((M2C_FIELD(cr_g_AreaBoxes, s32 *, (var_r31_12 << 6) & 0x3FFFC0) & 1) && ((u32) id == (u16) M2C_FIELD((((cr_AreaBox *) ((u8 *) cr_g_AreaBoxes + ((var_r31_12 << 6) & 0x3FFFC0)))), u16 *, 0x28))) {
            return (signed char *) ((cr_AreaBox *) ((u8 *) cr_g_AreaBoxes + ((var_r31_12 << 6) & 0x3FFFC0)));
        }
    }
    return NULL;
}

void cr_EV_CLR_SQARE(void) {
    s16 *temp_r30_15;
    u32 *cmd_cnt;
    s8 *var_r28_59;

    var_r28_59 = NULL;
    temp_r30_15 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r30_15, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 2;
    if (0) {
        (*cmd_cnt)++;
        temp_r30_15 = temp_r30_15;
    }
    var_r28_59 = cr_EV_CLR_SQARE_search0(M2C_FIELD(temp_r30_15, s16 *, 2));
    if (var_r28_59 != NULL) {
        fn_80023394(var_r28_59, 0x40U);
    }
}
