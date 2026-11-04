/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008E09C: .text 0x8008E09C-0x8008E108 (108 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80023394(s8 *arg0, u32 arg1);
typedef struct cr_TextRecord
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
} cr_TextRecord;
__declspec(section ".data") extern cr_TextRecord cr_g_TextRecords[2496];

void fn_8008E09C(void) {
    u16 var_r31_11;

    fn_80023394((s8 *) cr_g_TextRecords, 0x13800U);
    var_r31_11 = 0;
    while (var_r31_11 < 0x9C0U) {
        M2C_FIELD((((cr_TextRecord *) ((u8 *) cr_g_TextRecords + ((var_r31_11 << 5) & 0x1FFFE0)))), u16 *, 0x10) = 0xFFFF;
        var_r31_11 += 1;
    }
}
