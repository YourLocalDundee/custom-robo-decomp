/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x80320168-0x80321084, 2 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x80320184 cr_g_FileSlots (cr_FileSlot[32], 0xF00 bytes)
 *   0x80320168 cr_g_EntryTableLoad (u8[28], 0x1C bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct DVDFileInfo DVDFileInfo;
typedef struct DVDCommandBlock DVDCommandBlock;
typedef struct DVDDiskID DVDDiskID;
typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock *block);
struct DVDCommandBlock
{
  DVDCommandBlock *next;
  DVDCommandBlock *prev;
  u32 command;
  s32 state;
  u32 offset;
  u32 length;
  void *addr;
  u32 currTransferSize;
  u32 transferredSize;
  DVDDiskID *id;
  DVDCBCallback callback;
  void *userData;
};
typedef void (*DVDCallback)(s32 result, DVDFileInfo *fileInfo);
struct DVDFileInfo
{
  DVDCommandBlock cb;
  u32 startAddr;
  u32 length;
  DVDCallback callback;
};
typedef struct cr_FileSlot
{
  u8 in_use;
  u8 state;
  u8 index;
  u8 unk_03;
  u32 sectors;
  u32 size;
  u32 offset;
  u32 unk_10;
  u32 read_left;
  u32 read_done;
  u8 *read_buffer;
  u32 read_total;
  u32 read_chunk;
  u32 unk_28;
  s32 dvd_entry;
  s32 unk_30;
  u32 start_tick;
  u32 unk_38;
  DVDFileInfo file_info;
} cr_FileSlot;

/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */
#pragma force_active on
cr_FileSlot cr_g_FileSlots[32];
u8 cr_g_EntryTableLoad[28];
#pragma force_active reset
