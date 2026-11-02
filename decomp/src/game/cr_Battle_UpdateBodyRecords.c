/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Battle_UpdateBodyRecords: .text 0x8004D790-0x8004DC78 (1256 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u16 lbl_8044F44C;
extern u32 lbl_8044F500;
typedef enum cr_Seq
{
  SEQ_LOGO,
  SEQ_MOVIE,
  SEQ_TITLE,
  SEQ_FIELD,
  SEQ_BATTLE,
  SEQ_MODE_SELECTOR,
  SEQ_CUSTOMIZE,
  SEQ_STAGE_SELECTOR,
  SEQ_SCENARIO,
  SEQ_OPTION,
  SEQ_NAME_ENTRY,
  SEQ_MEMORY_CARD,
  SEQ_MENU,
  SEQ_ILLUSTRATED_BOOK,
  SEQ_ENDING
} cr_Seq;
typedef struct cr_SeqState
{
  cr_Seq current;
  cr_Seq previous;
  cr_Seq next;
  u8 unk_0c[4];
  u32 unk_10;
  u8 unk_14[0x10];
  u32 unk_24;
  u32 unk_28;
  u32 unk_2c;
  u8 unk_30[4];
  u32 unk_34;
  s32 unk_38;
  s16 unk_3c;
  u8 unk_3e[2];
  s32 unk_40;
  u8 unk_44[4];
} cr_SeqState;
__declspec(section ".data") extern cr_SeqState cr_g_SeqCurrent;
int __cntlzw(unsigned int);
__declspec(section ".sdata") extern u8 lbl_8044F44E[10];
typedef struct cr_GameMode
{
  u32 flags;
  u32 unk_04;
  u32 unk_08;
  u32 unk_0c;
  u32 unk_10;
  s16 unk_14;
  s16 unk_16;
  s16 unk_18;
  u8 unk_1a[2];
  u16 unk_1c[19];
  u8 unk_42;
  u8 unk_43;
  u8 unk_44;
  u8 unk_45;
  u8 unk_46[2];
} cr_GameMode;
__declspec(section ".data") extern cr_GameMode cr_g_GameMode;
typedef struct cr_BattleRecord
{
  u32 battles;
  u32 wins;
  u32 losses;
} cr_BattleRecord;
typedef struct cr_UnlockBits
{
  u32 parts[5][8];
  u32 stages[2];
} cr_UnlockBits;
typedef struct cr_SaveData
{
  u32 battles;
  u32 battles_by_mode[3];
  s32 battles_finished;
  u32 unk_014[3];
  cr_BattleRecord body_records[64];
  cr_BattleRecord unk_records[4][64];
  u32 stage_battles[40];
  cr_UnlockBits unlocks;
  cr_UnlockBits new_unlocks;
  u32 used_parts[5][2];
} cr_SaveData;
__declspec(section ".data") extern cr_SaveData cr_g_SaveData;
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

void cr_Battle_UpdateBodyRecords(void) {
    s32 var_r31;
    u32 var_r30;
    s32 var_r29;
    s32 var_r28;
    s32 var_r27;
    u32 var_r26;
    s32 var_r25;
    u32 var_r24;
    s32 var_r23;
    u8 var_r22;
    s32 var_r21;
    s32 var_r20;
    u8 var_r19;
    s32 var_r18;
    s32 var_r17;

    var_r27 = 0;
    lbl_8044F44C = 0;
    var_r30 = lbl_8044F500;
    if ((u32) cr_g_SeqCurrent.unk_10 == 3) {
        lbl_8044F44C |= 0x40;
    } else {
        lbl_8044F44C |= 0x100;
    }
    var_r30 = lbl_8044F500;
    while ((s32) var_r30 != 0) {
        var_r31 = __cntlzw(var_r30);
        if (lbl_8044F44E[var_r31] & 1) {
            var_r27 += 1;
        }
        var_r30 ^= 0x80000000U >> var_r31;
    }
    if (var_r27 == 0) {
        var_r30 = lbl_8044F500;
        lbl_8044F44C |= 0x200;
        while ((s32) var_r30 != 0) {
            var_r31 = __cntlzw(var_r30);
            if (!(cr_g_GameMode.flags & 0x01000000) && !(lbl_8044F44E[var_r31] & 8) && !(cr_g_GameMode.flags & 0x0A800000)) {
                if ((u32) cr_g_SaveData.body_records[cr_g_BattleRobos[var_r31].parts[0]].losses < 0x3E7U) {
                    cr_g_SaveData.body_records[(u16) (cr_g_BattleRobos[var_r31].parts[0])].losses += 1;
                }
                if ((u32) cr_g_SaveData.body_records[cr_g_BattleRobos[var_r31].parts[0]].battles < 0x3E7U) {
                    cr_g_SaveData.body_records[(u16) (cr_g_BattleRobos[var_r31].parts[0])].battles += 1;
                }
            }
            var_r30 ^= 0x80000000U >> var_r31;
        }
        return;
    }
    if (cr_g_GameMode.flags & 0x01000000) {
        if (!(*lbl_8044F44E & 1)) {
            lbl_8044F44C |= 0x20;
            return;
        }
        var_r25 = 0;
        var_r26 = lbl_8044F500;
        var_r20 = 0;
        var_r22 = 0;
        lbl_8044F44C |= 0x10;
        var_r21 = 0;
        while ((s32) var_r26 != 0) {
            var_r29 = __cntlzw(var_r26);
            if (lbl_8044F44E[var_r29] & 1) {
                if (!(lbl_8044F44E[var_r29] & 8)) {
                    if (lbl_8044F44E[var_r29] & 2) {
                        var_r25 += 1;
                    }
                } else {
                    var_r20 += 1;
                    if (lbl_8044F44E[var_r29] & 2) {
                        var_r21 += 1;
                    }
                }
            }
            var_r26 ^= 0x80000000U >> var_r29;
        }
        if (var_r25 == 0) {
            if (var_r27 == var_r21) {
                var_r22 = 1;
            }
        } else if (var_r27 == (s32) (var_r20 + var_r25)) {
            var_r22 = 1;
        }
        if (var_r22 != 0) {
            lbl_8044F44C |= 0x80;
        }
    } else {
        var_r30 = lbl_8044F500;
        while ((s32) var_r30 != 0) {
            var_r31 = __cntlzw(var_r30);
            if (!(lbl_8044F44E[var_r31] & 8) && !(cr_g_GameMode.flags & 0x0A800000) && ((u32) cr_g_SaveData.body_records[cr_g_BattleRobos[var_r31].parts[0]].battles < 0x3E7U)) {
                cr_g_SaveData.body_records[(u16) (cr_g_BattleRobos[var_r31].parts[0])].battles += 1;
            }
            if (lbl_8044F44E[var_r31] & 1) {
                lbl_8044F44C |= 1 << var_r31;
                if (!(lbl_8044F44E[var_r31] & 8) && !(cr_g_GameMode.flags & 0x0A800000) && ((u32) cr_g_SaveData.body_records[cr_g_BattleRobos[var_r31].parts[0]].wins < 0x3E7U)) {
                    cr_g_SaveData.body_records[(u16) (cr_g_BattleRobos[var_r31].parts[0])].wins += 1;
                }
            }
            var_r30 ^= 0x80000000U >> var_r31;
        }
        var_r24 = lbl_8044F500;
        var_r23 = 0;
        var_r17 = 0;
        var_r19 = 0;
        var_r18 = 0;
        while ((s32) var_r24 != 0) {
            var_r28 = __cntlzw(var_r24);
            if (lbl_8044F44E[var_r28] & 1) {
                if (!(lbl_8044F44E[var_r28] & 8)) {
                    if (lbl_8044F44E[var_r28] & 2) {
                        var_r23 += 1;
                    }
                } else {
                    var_r17 += 1;
                    if (lbl_8044F44E[var_r28] & 2) {
                        var_r18 += 1;
                    }
                }
            }
            var_r24 ^= 0x80000000U >> var_r28;
        }
        if (var_r23 == 0) {
            if (var_r27 == var_r18) {
                var_r19 = 1;
            }
        } else if (var_r27 == (s32) (var_r17 + var_r23)) {
            var_r19 = 1;
        }
        if (var_r19 != 0) {
            lbl_8044F44C |= 0x80;
        }
    }
}
