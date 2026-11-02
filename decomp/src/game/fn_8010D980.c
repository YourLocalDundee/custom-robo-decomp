/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8010D980: .text 0x8010D980-0x8010DBA0 (544 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_800208F0(void *arg0);
void fn_800215F8(void *arg0);
typedef struct cr_BattleMotionSet cr_BattleMotionSet;
typedef struct cr_BodyData
{
  f32 unk_00;
  f32 unk_04;
  cr_BattleMotionSet *motion_set;
  f32 unk_0c[27];
} cr_BodyData;
typedef struct cr_BattleRobo
{
  void (*update)(void);
  u8 unk_00004[0x6C];
  f32 unk_00070;
  f32 hp;
  u8 unk_00078[0x14];
  u8 index;
  u8 unk_0008d;
  u16 unk_0008e;
  u8 unk_00090[0x13C];
  u16 parts[5];
  u8 unk_001d6[0x161E];
  cr_BodyData body_data;
  u8 unk_0186c[0x32C];
  u8 body_stats[0x134];
  u8 unk_01ccc[0x15C];
  u8 gun_data[0x28C];
  u8 unk_020b4[0x26C94];
  u8 bomb_data[0x17C];
  u8 unk_28ec4[0x5424];
  u8 pod_data[0x174];
  u8 unk_2e45c[0x4EAC];
  u8 leg_data[0x8F];
  u8 unk_33397[0x189];
} cr_BattleRobo;
__declspec(section ".data") extern cr_BattleRobo cr_g_BattleRobos[4];
extern f32 lbl_80451768;
void fn_8010D2D0(u8 arg0, u16 arg1);

s32 fn_8010D980(void *arg0, u8 arg1) {
    if (*(u32 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC] != 0U) {
        fn_800208F0((void *) (*(u32 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC]));
        fn_800215F8((void *) (*(u32 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC]));
        *(u32 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC] = 0;
    }
    switch ((s32) arg1) {                           /* irregular */
    case 0:
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x4854)], s32 *, 0x8B0) = 0;
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x4854)], s32 *, 0x860) = 0;
        break;
    case 1:
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x4854)], s32 *, 0x964) = 0;
        break;
    case 2:
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x4854)], s32 *, 0x968) = 0;
        break;
    case 3:
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x4854)], f32 *, 0x2E470) = (f32) lbl_80451768;
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x4854)], s32 *, 0x96C) = 0;
        break;
    case 4:
        M2C_FIELD(&cr_g_BattleRobos[M2C_FIELD(arg0, u8 *, 0x4854)], s32 *, 0x970) = 0;
        break;
    }
    *(s8 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47F0] = 0;
    if (*(u8 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47F4] != 0) {
        fn_8010D2D0(M2C_FIELD(arg0, u8 *, 0x4854), (u16) arg1);
        *(s8 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47F4] = 0;
    }
    M2C_FIELD(arg0, s8 *, 0x4867) = 0;
    return 0;
}
