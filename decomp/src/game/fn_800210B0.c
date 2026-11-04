/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800210B0: .text 0x800210B0-0x80021350 (672 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_FileSlot cr_FileSlot;
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
struct cr_FileSlot
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
};
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
BOOL DVDClose(DVDFileInfo *fileInfo);
typedef u32 OSTick;
OSTick OSGetTick(void);
typedef unsigned int uintptr_t;
typedef unsigned long size_t;
typedef int bool;
typedef void (*HSD_DevComCallback)(int, uintptr_t, void *, bool cancelflag);
int HSD_DevComRequest(int file, uintptr_t src, uintptr_t dest, size_t size, int type, int pri, HSD_DevComCallback callback, uintptr_t args);

/* fn_800210B0: Nov 3, 2026 hand draft (m2c 96%): the queue head advanced first, the current read taken after (the original materialises its address last) */

void fn_800210B0(s32 arg0, u32 arg1, void *arg2, s32 cancelflag) {
    cr_FileSlot *var_r31;
    s32 var_r30;
    s32 var_r29;

    cr_g_FileSysReady.queue_head += 1;
    cr_g_FileSysReady.queue_head &= 0x1F;
    var_r31 = cr_g_FileSysReady.current_read;
    if ((s8) var_r31->in_use == -1) {
        var_r31->in_use = 0;
        var_r31->index = 0;
        var_r31->state = 1;
        var_r31->offset = 0;
        var_r31->sectors = 0;
        var_r31->size = 0;
        DVDClose(&var_r31->file_info);
    } else {
        var_r31->read_done += var_r31->read_chunk;
        if ((s32) var_r31->read_done >= (s32) var_r31->read_left) {
            var_r31->unk_38 = OSGetTick();
            var_r31->state = 3;
        } else {
            cr_g_FileSysReady.queue[cr_g_FileSysReady.queue_tail] = cr_g_FileSysReady.current_read;
            var_r31->state = 5;
            var_r31->read_buffer = &var_r31->read_buffer[var_r31->read_chunk];
            var_r31->offset += var_r31->read_chunk;
            cr_g_FileSysReady.queue_tail += 1;
            cr_g_FileSysReady.queue_tail &= 0x1F;
        }
    }
    while ((u8) cr_g_FileSysReady.queue_head != (u8) cr_g_FileSysReady.queue_tail) {
        var_r31 = cr_g_FileSysReady.queue[cr_g_FileSysReady.queue_head];
        if ((s8) var_r31->in_use != -1) {
            cr_g_FileSysReady.current_read = var_r31;
            var_r30 = var_r31->read_left - var_r31->read_done;
            if (var_r30 > (s32) (cr_g_FileSysReady.chunk_sectors << 0xB)) {
                var_r29 = cr_g_FileSysReady.chunk_sectors << 0xB;
            } else {
                var_r29 = var_r30;
            }
            var_r31->read_chunk = (u32) var_r29;
            var_r31->state = 2;
            HSD_DevComRequest(var_r31->dvd_entry, var_r31->offset, (u32) var_r31->read_buffer, var_r31->read_chunk, 0x21, 1, (void (*)(int, unsigned int, void *, int)) fn_800210B0, (unsigned int) ((void (*)(int, unsigned int, void *, int)) 0U));
            return;
        }
        var_r31->in_use = 0;
        var_r31->index = 0;
        var_r31->state = 1;
        var_r31->offset = 0;
        var_r31->sectors = 0;
        var_r31->size = 0;
        DVDClose(&var_r31->file_info);
        cr_g_FileSysReady.queue_head += 1;
        cr_g_FileSysReady.queue_head &= 0x1F;
    }
    return;
}
