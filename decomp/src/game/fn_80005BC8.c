/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80005BC8: .text 0x80005BC8-0x80005D38 (368 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
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
extern u8 lbl_8041E938[24];
__declspec(section ".sdata") extern u8 *cr_g_CurrentBattleRobo;
void fn_800DBAD0(void);
void fn_800057B8(void);
void fn_8000860C(s32 arg0);
extern const f32 lbl_804501B8;
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;

/* fn_80005BC8: Nov 3, 2026 hand draft (m2c 64%): two expansions of one input reset, each with its own u16 zero local (the saved registers r31 / r30; +0x23E is stored from it, not from a literal), the second through a Vec on the stack */
void fn_80005BC8(void) {
    if (((u32) cr_g_SeqCurrent.unk_10 == 2) && (lbl_8041E938[3] == 0)) {
        if (!(M2C_FIELD(cr_g_CurrentBattleRobo, s32 *, 0x1D8) & 0x200)) {
            if (!(M2C_FIELD(cr_g_CurrentBattleRobo, u32 *, 0x1D8) & 0x80000000)) {
                fn_800DBAD0();
                fn_800057B8();
            } else {
                fn_800DBAD0();
            }
            fn_8000860C(M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242));
        } else {
            u16 buttons = 0;

            M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x23E) = buttons;
            M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) = buttons & ~M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240);
            M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x244) = M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240) & ~buttons;
            M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240) = buttons;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x104) = lbl_804501B8;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x108) = lbl_804501B8;
            M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10C) = lbl_804501B8;
        }
    } else {
        u16 buttons = 0;
        Vec stick;

        M2C_FIELD(cr_g_CurrentBattleRobo, s16 *, 0x23E) = buttons;
        stick.x = lbl_804501B8;
        stick.y = lbl_804501B8;
        stick.z = lbl_804501B8;
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x242) = buttons & ~M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240);
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x244) = M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240) & ~buttons;
        M2C_FIELD(cr_g_CurrentBattleRobo, u16 *, 0x240) = buttons;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x104) = stick.x;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x108) = stick.y;
        M2C_FIELD(cr_g_CurrentBattleRobo, f32 *, 0x10C) = stick.z;
    }
}
