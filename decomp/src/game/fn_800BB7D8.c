/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800BB7D8: .text 0x800BB7D8-0x800BB8F4 (284 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
extern u8 lbl_8044F755;
extern u32 lbl_8037569C[2417];
extern u8 *lbl_8044F70C;
u32 fn_800EB004(u32 arg0, s32 arg1, u32 arg2);
extern u8 *lbl_8044F2BC;
void fn_800EB0D4(s8 *arg0);
extern u8 lbl_803E6694[12];
void fn_800245E8(s32 arg0);
__declspec(section ".data") extern u8 cr_g_Cams[20288];
void fn_80023304(u8 *arg0, u8 *arg1, s32 arg2);
extern u16 lbl_803B931C[596];
extern f32 lbl_8044F6F8;
f32 fn_800683A4(void);
typedef struct cr_ScenarioSlot cr_ScenarioSlot;
__declspec(section ".sdata") extern cr_ScenarioSlot *cr_g_ScenarioSlotPtr;
struct cr_ScenarioSlot
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
};
typedef struct cr_ScenarioState
{
  u16 unk_000;
  char name[18];
  u32 error_flags;
  u32 unk_018;
  u32 unk_01c;
  u32 unk_020;
  s32 wait_timer;
  s32 unk_028;
  u8 unk_02c;
  u8 unk_02d;
  u8 unk_02e;
  u8 slot_index;
  cr_ScenarioSlot slot[2];
  u32 unk_1c8[4];
  u32 unk_1d8[32];
  u32 unk_258[10];
  u32 unk_280;
  u8 unk_284;
  u8 unk_285[3];
  u32 part_bits[5][8];
  u32 unk_328[2];
  u8 unk_330;
  u8 pickup_battle;
  u8 unk_332[2];
  u32 unk_334[4];
  u32 unk_344[4];
  u32 unk_354[4];
} cr_ScenarioState;
__declspec(section ".data") extern cr_ScenarioState cr_g_Scenario;

/* fn_800BB7D8: hand-written from main.dol's asm on its m2c draft (step 109b): lbl_8037569C's address taken into a local before the load call (addi r31; its words 3 and 4 read through it after) */
extern u8 *lbl_8044F728;

void fn_800BB7D8(void) {
    u32 *p;

    if (!(cr_g_SeqCurrent.unk_40 & 0x8001) && (cr_g_Pads->unk_14 & 0x100)) {
        if (lbl_8044F755 & 2) {
            p = (u32 *) lbl_8037569C;
            lbl_8044F70C = (unsigned char *) (fn_800EB004((u32) lbl_8044F2BC, 0, 0x340000U));
            do {

            } while ((s32) *lbl_8044F70C != 2);
            fn_800EB0D4((s8 *) lbl_8044F70C);
            M2C_FIELD(lbl_803E6694, u32 *, 4) = p[3];
            M2C_FIELD(lbl_803E6694, u32 *, 8) = p[4];
        }
        fn_800245E8((s32) cr_g_Cams);
        fn_80023304((u8 *) lbl_803B931C, lbl_8044F728, 0x4A8);
        *lbl_803B931C |= 1;
        lbl_8044F6F8 = fn_800683A4();
        if (!(cr_g_ScenarioSlotPtr->status & 0x20)) {
            cr_g_Scenario.unk_020 = cr_g_Scenario.unk_01c;
        }
        M2C_FIELD(lbl_8044F728, s8 *, 0x4A0) = 1;
    }
}
