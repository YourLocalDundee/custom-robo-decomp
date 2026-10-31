/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A4C38: .text 0x800A4C38-0x800A4CEC (180 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
extern u8 *lbl_8044F748;
extern f32 lbl_804512BC;
f32 fn_8006207C(f32 farg0, f32 farg1, f32 farg2, f32 farg3);
extern f32 lbl_80451388;
extern f32 lbl_80451304;
extern f32 lbl_80451280;
void fn_800B1544(void);
u32 cr_Bits_Test(u32 *set, u32 bit);
typedef struct cr_ScenarioSlot
{
  u32 status;
  u32 flags[32];
  u16 unk_84[6];
  u16 battle_count;
  u16 unk_92[2];
  u8 unk_96[2];
  u32 unk_98[5];
  u32 unk_ac[2];
  u32 unk_b4;
  u16 unk_b8;
  u16 unk_ba;
  u8 unk_bc[0x10];
} cr_ScenarioSlot;
__declspec(section ".sdata") extern cr_ScenarioSlot *cr_g_ScenarioSlotPtr;

void fn_800A4C38(void) {
    if ((s8) cr_g_Script.unk_467 != 0) {
        M2C_FIELD(lbl_8044F748, f32 *, 0xC) = (f32) (lbl_804512BC + fn_8006207C(M2C_FIELD(lbl_8044F748, f32 *, 8), lbl_80451388, M2C_FIELD(lbl_8044F748, f32 *, 0x10), lbl_80451304));
    } else {
        M2C_FIELD(lbl_8044F748, f32 *, 0xC) = (f32) lbl_80451280;
    }
    fn_800B1544();
    if (cr_Bits_Test(&cr_g_ScenarioSlotPtr->unk_b4, M2C_FIELD(lbl_8044F748, u16 *, 0x14E) - 0x36) != 0) {
        M2C_FIELD(lbl_8044F748, u8 *, 0x156) &= 0xFFFFFFFB;
        return;
    }
    M2C_FIELD(lbl_8044F748, u8 *, 0x156) |= 4;
}
