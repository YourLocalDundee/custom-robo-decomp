/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80020A60: .text 0x80020A60-0x80020A70 (16 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_FileSlot cr_FileSlot;
typedef struct cr_FileSysState
{
  u8 ready;
  u8 unk_01[3];
  u8 queue_head;
  u8 queue_tail;
  u16 chunk_sectors;
  cr_FileSlot *current_read;
  cr_FileSlot *queue[32];
} cr_FileSysState;
__declspec(section ".data") extern cr_FileSysState cr_g_FileSysReady;

void fn_80020A60(u16 arg0) {
    cr_g_FileSysReady.chunk_sectors = arg0;
}
