/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8012F258: .text 0x8012F258-0x8012F28C (52 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_8008E158(void *arg0, void *arg1);
typedef struct cr_CustomizeState
{
  u8 mode;
  u8 prev_mode;
  u8 unk_0002;
  u8 unk_0003;
  u8 unk_0004;
  u8 unk_0005;
  u8 unk_0006;
  u8 unk_0007;
  u8 unk_0008[0x1A00];
  u32 unk_1a08[4];
  f32 unk_1a18;
  f32 unk_1a1c;
  f32 unk_1a20;
  u32 unk_1a24[5];
  u32 unk_1a38;
  u32 unk_1a3c;
  u32 unk_1a40;
  u32 unk_1a44;
  u32 unk_1a48;
  u32 unk_1a4c;
  u32 unk_1a50;
  u32 unk_1a54;
  u32 unk_1a58;
  u32 unk_1a5c;
  u32 unk_1a60;
  u32 unk_1a64;
  u8 unk_1a68[0xCC];
  u32 unk_1b34;
  u32 unk_1b38;
  u32 unk_1b3c;
  u32 unk_1b40;
  u32 unk_1b44;
  u32 unk_1b48;
  u32 unk_1b4c;
  u32 unk_1b50;
} cr_CustomizeState;
__declspec(section ".data") extern cr_CustomizeState cr_g_Customize;

void fn_8012F258(s32 arg0) {

    fn_8008E158((void *) arg0, &cr_g_Customize.unk_1b38);
}
