/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800BC4C0: .text 0x800BC4C0-0x800BC5C8 (264 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
extern u8 lbl_8044F6F4;
extern u8 lbl_8044EB38[4];
void fn_800AFE3C(s16 arg0);
extern u8 lbl_8044F756;
extern u8 lbl_8044F72C;
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
void fn_80111E54(s32 arg0);
void fn_800F6F6C(s32 arg0);

/* fn_800BC4C0: hand-written from main.dol's asm on its m2c draft (step 109b): the rank count as a static inline helper (its locals take the saved registers r31 / r29 in the original: the inlined body's own variables), the threshold compared as u32 */
extern u8 *lbl_8044F728;

static inline u8 rank_of_slot(void) {
    u32 v;

    v = cr_g_ScenarioSlotPtr->unk_84[3];
    lbl_8044F6F4 = 0;
    for (; lbl_8044F6F4 < 3U; lbl_8044F6F4 += 1) {
        if (!(v >= lbl_8044EB38[lbl_8044F6F4])) {
            break;
        }
    }
    if (lbl_8044F6F4 != 0) {
        fn_800AFE3C(2);
    }
    return lbl_8044F6F4;
}

void fn_800BC4C0(void) {
    u8 var_r30;

    switch (M2C_FIELD(lbl_8044F728, u8 *, 0x4A1)) {
    case 0:
        lbl_8044F756 = 1;
        M2C_FIELD(lbl_8044F728, u16 *, 0) |= 8;
        lbl_8044F72C = 0;
        cr_g_SeqCurrent.next = SEQ_STAGE_SELECTOR;
        fn_80111E54(1);
        fn_800F6F6C(1);
        return;
    case 1:
        var_r30 = rank_of_slot();
        if (var_r30 == 0) {
            M2C_FIELD(lbl_8044F728, u8 *, 0x4A1) = 0U;
        }
        return;
    }
}
