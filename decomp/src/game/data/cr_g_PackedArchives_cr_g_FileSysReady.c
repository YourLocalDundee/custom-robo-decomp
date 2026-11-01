/* Custom Robo game data, written by tools/gen_data_c.py; regenerate it, do not edit (python -I tools/gen_data_c.py --all).
 * A data-only game unit (plan.md step 86, docs/decomp.md "Game data objects"): .bss 0x80321148-0x80321D40, 2 objects, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).
 * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt; zero-initialised, as main.dol has it.
 * The compiler lays .bss / .sbss out in reverse declaration order, so the definitions are in that order:
 *   0x80321CB4 cr_g_FileSysReady (cr_FileSysState, 0x8C bytes)
 *   0x80321148 cr_g_PackedArchives (cr_PackedArchive[43], 0xB6C bytes)
 * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */
typedef struct cr_PackedEntry
{
  u32 offset;
  u32 size;
} cr_PackedEntry;
typedef struct cr_PackedArchive
{
  u32 unk_00;
  u32 unk_04;
  u32 entry_count;
  s16 entry_count16;
  u8 unk_0e;
  u8 unk_0f;
  u8 unk_10[0x2C];
  s32 dvd_entry;
  cr_PackedEntry *entries;
} cr_PackedArchive;
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

cr_FileSysState cr_g_FileSysReady;
cr_PackedArchive cr_g_PackedArchives[43];
