/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80125C2C: .text 0x80125C2C-0x80125DCC (416 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8041CDE4[6996];
extern u16 lbl_803BF914[434];
extern u32 lbl_804198B0[42];
u8 fn_800964B0(s32 arg0);
void fn_800972D8(s32 arg0);
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
__declspec(section ".data") extern cr_Seq cr_g_SeqCurrent;
extern u8 lbl_8041CDB4[24];
f32 fn_800965EC(void);
void fn_800965F4(f32 farg0);
extern f32 lbl_80451A54;
extern u32 lbl_803BE7DC[1102];

extern u8 *lbl_8044F794;

void fn_80125C2C(void) {
    lbl_8041CDE4[2] = 0;
    lbl_8041CDE4[6] = 0;
    if (M2C_FIELD(lbl_8041CDE4, u8 *, 0) == 1) {
        if (M2C_FIELD(lbl_803BF914, u8 *, 0x2F) == 0) {
            lbl_8041CDE4[4] = 2;
        } else {
            lbl_8041CDE4[4] = 3;
        }
        lbl_804198B0[0x28] = M2C_FIELD(lbl_803BF914, u32 *, 0x328);
        lbl_804198B0[0x29] = M2C_FIELD(lbl_803BF914, u32 *, 0x32C);
        return;
    }
    if (M2C_FIELD(lbl_8041CDE4, u8 *, 0) == 2) {
        if ((fn_800964B0((s32) M2C_FIELD(lbl_8044F794, u16 *, 0xBA)) == 0) && (M2C_FIELD(lbl_8044F794, u16 *, 0xBA) != 0x47)) {
            fn_800972D8((s32) M2C_FIELD(lbl_8044F794, u16 *, 0xBA));
        }
        if (M2C_FIELD(&cr_g_SeqCurrent, s32 *, 4) != 6) {
            M2C_FIELD(lbl_8041CDB4, f32 *, 0x14) = fn_800965EC();
        }
        fn_800965F4(lbl_80451A54 * M2C_FIELD(lbl_8041CDB4, f32 *, 0x14));
        if (M2C_FIELD(lbl_803BF914, u8 *, 0x2F) == 0) {
            lbl_8041CDE4[4] = 0;
            return;
        }
        lbl_8041CDE4[4] = 1;
        return;
    }
    lbl_804198B0[0x28] = lbl_803BE7DC[0x418];
    lbl_804198B0[0x29] = lbl_803BE7DC[0x419];
}
