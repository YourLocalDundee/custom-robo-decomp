/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800ABAF0: .text 0x800ABAF0-0x800ABC28 (312 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_Pad
{
  u32 pressed;
  u32 released;
  u32 held;
  u32 not_held;
  u32 prev_held;
  u32 unk_14;
  u32 unk_18;
  u32 dir;
  u32 unk_20;
  u32 prev_dir;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[0x20];
  u8 unk_50[0x20];
  u8 unk_70[0x22];
  u16 unk_92;
  u16 trigger_peak[2];
  s8 stick_x;
  s8 stick_y;
  s8 substick_x;
  s8 substick_y;
  u8 trigger_l;
  u8 trigger_r;
  u8 analog_a;
  u8 analog_b;
  u8 err;
  u8 trigger_clicked[2];
  u8 unk_a3;
} cr_Pad;
__declspec(section ".data") extern cr_Pad cr_g_Pads[4];
void fn_800AFE3C(s16 arg0);
extern u8 lbl_802249B0[1320];
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
extern u32 lbl_80229B50[3];

extern u8 *lbl_8044F728;

void fn_800ABAF0(void) {
    void *var_r31;

    if ((M2C_FIELD(lbl_8044F728, u8 *, 3) != 9) && (cr_g_Pads->unk_14 & 0x100)) {
        fn_800AFE3C(9);
        var_r31 = lbl_802249B0 + (cr_g_Script.unk_450 * 0x2C);
        if (M2C_FIELD(var_r31, u32 *, 0x20) == 0U) {
            if ((void *) lbl_8044F728 != ((void *) 0)) {
                M2C_FIELD(lbl_8044F728, u8 *, 0x5F) |= 1;
            }
            if ((void *) lbl_8044F728 != ((void *) 0)) {
                M2C_FIELD(lbl_8044F728, u8 *, 0x60) |= 1;
            }
            if ((void *) lbl_8044F728 != ((void *) 0)) {
                M2C_FIELD(lbl_8044F728, u8 *, 0x61) |= 1;
            }
            if ((void *) lbl_8044F728 != ((void *) 0)) {
                M2C_FIELD(lbl_8044F728, u8 *, 0x62) |= 1;
            }
            M2C_FIELD(M2C_FIELD(M2C_FIELD(lbl_80229B50[M2C_FIELD(lbl_8044F728, u8 *, 0x18)], void **, 0x1C), void **, 4), s16 *, 0xE) = 0x80;
        }
        M2C_FIELD(lbl_8044F728, u16 *, 0) |= 4;
    }
}
