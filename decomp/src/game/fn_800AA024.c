/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800AA024: .text 0x800AA024-0x800AA130 (268 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
f32 fn_8006207C(f32 farg0, f32 farg1, f32 farg2, f32 farg3);
extern const f32 lbl_80451388;
extern const f32 lbl_80451304;
extern const f32 lbl_80451280;
void fn_800B1544(void);
s32 fn_8002B284(void *arg0, f32 farg0);
extern const f32 lbl_8045127C;

void fn_800AA024(void) {
    struct {
        s8 sp8;
        u8 pad_9[3];
        f32 spC;
        f32 sp10;
        f32 sp14;
    } sp;

    if ((s8) cr_g_Script.unk_467 != 0) {
        M2C_FIELD(lbl_8044F748, f32 *, 0xC) = fn_8006207C(M2C_FIELD(lbl_8044F748, f32 *, 8), lbl_80451388, M2C_FIELD(lbl_8044F748, f32 *, 0x10), lbl_80451304);
    } else {
        M2C_FIELD(lbl_8044F748, f32 *, 0xC) = (f32) lbl_80451280;
    }
    fn_800B1544();
    if (!(M2C_FIELD(lbl_8044F748, u8 *, 0x156) & 2)) {
        M2C_FIELD(lbl_8044F748, u8 *, 0x156) &= 0xFE;
        sp.spC = M2C_FIELD(lbl_8044F748, f32 *, 8);
        sp.sp10 = M2C_FIELD(lbl_8044F748, f32 *, 0xC);
        sp.sp14 = M2C_FIELD(lbl_8044F748, f32 *, 0x10);
        sp.sp8 = fn_8002B284(&sp.spC, lbl_8045127C);
    }
    M2C_FIELD(lbl_8044F748, s32 *, 0x88) &= 0xFFDFFFFF;
    if (M2C_FIELD(lbl_8044F748, u8 *, 0x156) & 1) {
        M2C_FIELD(lbl_8044F748, s32 *, 0x88) |= 0x200000;
    }
    if (M2C_FIELD(lbl_8044F748, u8 *, 0x156) & 8) {
        M2C_FIELD(lbl_8044F748, s32 *, 0x88) |= 0x200000;
    }
}
