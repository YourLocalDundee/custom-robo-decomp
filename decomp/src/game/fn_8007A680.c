/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8007A680: .text 0x8007A680-0x8007A6D8 (88 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".sdata") extern u8 *cr_g_CurrentCam;
typedef struct cr_GameMode
{
  u32 flags;
  u32 unk_04;
  u32 unk_08;
  u32 unk_0c;
  u32 battle_mode;
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

void fn_8007A680(void *arg0) {
    M2C_FIELD(arg0, s32 *, 8) = (s32) *M2C_FIELD(M2C_FIELD(arg0, void **, 0x5C), u16 **, 0x1C);
    if ((u8) *cr_g_CurrentCam == 8) {
        M2C_FIELD(arg0, u8 *, 0x57) = 0xFF;
        M2C_FIELD(arg0, s32 *, 0) |= 0x20;
        M2C_FIELD(arg0, s8 *, 0x58) = 1;
    }
    if (cr_g_GameMode.flags & 0x04000000) {
        M2C_FIELD(arg0, s8 *, 0x58) = 1;
    }
}
