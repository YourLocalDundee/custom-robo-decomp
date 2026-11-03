/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800B47F0: .text 0x800B47F0-0x800B497C (396 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_803BA3A8[2368];
extern u8 *lbl_8044F738;
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
s8 fn_80029060(void *arg0, f32 (*arg1)[4], void *arg2, f32 farg0);
__declspec(section ".data") extern u8 cr_g_Cams[20288];
extern u8 lbl_8032338C[192];
extern const f32 lbl_804512BC;

void fn_800B47F0(void) {
    struct {
        f32 sp8;
        f32 spC;
        f32 sp10;
    } sp;
    u8 var_r31_6;

    var_r31_6 = 0;
    while (var_r31_6 < 0x10U) {
        if ((u8) *(lbl_803BA3A8 + (var_r31_6 * 0x94)) == 1) {
            lbl_8044F738 = lbl_803BA3A8 + (var_r31_6 * 0x94);
            if ((s32) (u8) cr_g_Script.unk_466 == 0xA) {
                if ((s32) var_r31_6 == (s8) cr_g_Script.unk_470) {
                    M2C_FIELD(lbl_8044F738, u32 *, 0x88) += 1;
                    if (M2C_FIELD(lbl_8044F738, u32 *, 0x88) >= 0x1EU) {
                        M2C_FIELD(lbl_8044F738, s32 *, 4) ^= 0x200000;
                        M2C_FIELD(lbl_8044F738, u32 *, 0x88) = 0U;
                    }
                } else {
                    M2C_FIELD(lbl_8044F738, s32 *, 4) &= 0xFFDFFFFF;
                    M2C_FIELD(lbl_8044F738, u32 *, 0x88) = 0U;
                }
            } else {
                M2C_FIELD(lbl_8044F738, s32 *, 4) &= 0xFFFFFFF7;
                M2C_FIELD(lbl_8044F738, u32 *, 0x88) = 0U;
            }
            sp.sp8 = M2C_FIELD(lbl_8044F738, f32 *, 0x34);
            sp.spC = M2C_FIELD(lbl_8044F738, f32 *, 0x44);
            sp.sp10 = M2C_FIELD(lbl_8044F738, f32 *, 0x54);
            if (fn_80029060(cr_g_Cams, (f32 (*)[4]) lbl_8032338C, &sp.sp8, lbl_804512BC) == 0) {
                M2C_FIELD(lbl_8044F738, s32 *, 4) |= 0x400000;
            } else {
                M2C_FIELD(lbl_8044F738, s32 *, 4) &= 0xFFBFFFFF;
            }
        }
        var_r31_6 += 1;
    }
}
