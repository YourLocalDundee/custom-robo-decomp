/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_LGT_SET_CAM_POS: .text 0x800C5CB4-0x800C5DB4 (256 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern s16 *cr_g_ScriptPC;
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef Vec Vec3;
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
typedef struct HSD_LObj HSD_LObj;
void HSD_LObjSetPosition(HSD_LObj *lobj, Vec3 *position);
extern u32 lbl_80354154[10];
typedef Vec Point3d;
__declspec(section ".sdata") extern u8 *cr_g_CurrentCam;
extern u32 lbl_803B8C44[10];

void cr_LGT_SET_CAM_POS(void) {
    s16 *temp_r31_14;
    u32 *cmd_cnt;

    temp_r31_14 = cr_g_ScriptPC;
    cr_g_Script.cmd_history[cr_g_Script.history_index] = (s16) M2C_FIELD(temp_r31_14, s16 *, 0);
    cr_g_Script.history_index += 1;
    cr_g_Script.history_index &= 0x3F;
    cr_g_Script.cmd_count[cr_g_ScriptDepth] += 1;
    cr_g_ScriptPC += 4;
    if (0) {
        (*cmd_cnt)++;
    }
    if (M2C_FIELD(temp_r31_14, s16 *, 4) == 0) {
        HSD_LObjSetPosition((HSD_LObj *) lbl_80354154[M2C_FIELD(temp_r31_14, s16 *, 2)], (Point3d *) (cr_g_CurrentCam + 0x74));
        return;
    }
    HSD_LObjSetPosition((HSD_LObj *) lbl_803B8C44[M2C_FIELD(temp_r31_14, s16 *, 2)], (Point3d *) (cr_g_CurrentCam + 0x74));
}
