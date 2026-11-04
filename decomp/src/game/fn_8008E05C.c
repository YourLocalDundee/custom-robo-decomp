/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008E05C: .text 0x8008E05C-0x8008E09C (64 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_TextRecord cr_TextRecord;
struct cr_TextRecord
{
  f32 unk_00;
  f32 unk_04;
  u16 unk_08;
  u8 unk_0a[2];
  u16 unk_0c;
  u16 unk_0e;
  u16 unk_10;
  u16 unk_12;
  u8 unk_14[4];
  u8 unk_18;
  u8 unk_19;
  u8 unk_1a;
  u8 unk_1b;
  u8 unk_1c[4];
};
__declspec(section ".data") extern cr_TextRecord cr_g_TextRecords[2496];

void fn_8008E05C(void *arg0, void *arg1, s32 arg2) {
    M2C_FIELD(arg1, u16 *, 8) = 0xFFFE;
    M2C_FIELD(arg0, void **, 0x1C) = arg1;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x1C), cr_TextRecord **, 4) = &cr_g_TextRecords[arg2];
    M2C_FIELD(M2C_FIELD(arg0, void **, 0x1C), cr_TextRecord **, 4)->unk_10 = 0xFFFF;
}
