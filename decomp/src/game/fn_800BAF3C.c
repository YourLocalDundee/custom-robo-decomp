/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800BAF3C: .text 0x800BAF3C-0x800BB034 (248 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
  char *script_name[16];
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
__declspec(section ".data") extern u8 cr_g_MotionDataBit15[1280];
__declspec(section ".data") extern u32 *cr_g_MotionData[1280];

extern u32 *lbl_8044F338;

void fn_800BAF3C(void) {
    u16 *var_r31_26;
    u32 *temp_r30_21;
    u32 var_r29_12;
    u32 var_r28_20;

    if ((s32) cr_g_Script.unk_454 != -1) {
        var_r29_12 = 0;
        while (var_r29_12 < (u32) *lbl_8044F338) {
            var_r28_20 = 0U;
            temp_r30_21 = ((u32 *) ((u8 *) lbl_8044F338 + M2C_FIELD(&lbl_8044F338[var_r29_12], s32 *, 4)));
            var_r31_26 = (unsigned short *) (temp_r30_21 + *temp_r30_21 * 3 + 1);
            while (var_r28_20 < (u32) *temp_r30_21) {
                if ((u16) *var_r31_26 != 0xFFFF) {
                    cr_g_MotionDataBit15[*var_r31_26 & 0x7FFF] = 0;
                    *(((u32 * *) ((u8 *) cr_g_MotionData + ((*var_r31_26 * 4) & 0x1FFFC)))) = 0;
                }
                var_r28_20 += 1;
                var_r31_26 += 1;
            }
            var_r29_12 += 1;
        }
    }
    cr_g_Script.unk_454 = -1;
}
