/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8012B8DC: .text 0x8012B8DC-0x8012BA00 (292 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8008D434(s32 arg0, s32 *arg1, u8 arg2, f32 farg0, f32 farg1);
s32 fn_80066948(u8 arg0, u16 arg1);
extern u8 lbl_8041CDB4[24];
extern const f32 lbl_80451B1C;
extern const f32 lbl_80451BB4;
void fn_8008CE9C(void);
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
void fn_8008C8A8(void);
void fn_800965F4(f32 farg0);
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
void fn_8011413C(void);
void fn_800BA7E0(void);
void fn_80098290(s32 arg0);

void fn_8012B8DC(void) {
    s32 var_r31_18;

    fn_8008D434(fn_80066948(0xAU, 0x2DU), (s32 *) &lbl_8041CDB4[0xC], 1U, lbl_80451B1C, lbl_80451BB4);
    fn_8008CE9C();
    var_r31_18 = -1;
    if (cr_g_Pads->unk_14 & 0x100) {
        var_r31_18 = 2;
        fn_8008C8A8();
        if (M2C_FIELD(lbl_8041CDB4, s32 *, 0xC) == 0) {
            fn_800965F4(M2C_FIELD(lbl_8041CDB4, f32 *, 0x14));
            cr_g_SeqCurrent.next = SEQ_TITLE;
            fn_8011413C();
            fn_800BA7E0();
            lbl_8041CDB4[0x10] = 1;
        }
        M2C_FIELD(lbl_8041CDB4, u8 *, 0) = 0;
        lbl_8041CDB4[1] = 1;
    } else if (cr_g_Pads->unk_14 & 0x200) {
        var_r31_18 = 0;
        fn_8008C8A8();
        M2C_FIELD(lbl_8041CDB4, u8 *, 0) = 0;
        lbl_8041CDB4[1] = 1;
    }
    if (var_r31_18 > -1) {
        fn_80098290(var_r31_18);
    }
}
