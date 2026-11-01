/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80021AB0_fn_80021AB0: .text 0x80021AB0-0x80021CB4 (516 bytes, 1 functions: fn_80021AB0 .. fn_80021AB0), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * A FILE OBJECT (plan.md step 63): these consecutive functions share their .sdata2 constant pool / string
 * literals (the per-file pool rule), so they are one source file; the file's real edges may lie further out.
 * The code is the functions' m2c drafts (docs/drafts.md), which compile to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
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
extern u8 lbl_8044F7DC;
long DVDGetDriveStatus();
extern u8 lbl_8044F404;
void fn_800224F0(s32 arg0);
s32 fn_80066EEC(void *arg0, u16 arg1);
__declspec(section ".data") extern u8 cr_g_EntryTableLoad[28];

/* the file's functions (each is defined below, in address order) */
void fn_80021AB0(void);

void fn_80021AB0(void) {
    s32 var_r31_9;

    var_r31_9 = 7;
    cr_g_SeqCurrent.unk_38 &= 0xFFF7FFFF;
    if ((cr_g_FileSysReady.ready != 0) && (lbl_8044F7DC == 0)) {
        switch ((DVDGetDriveStatus())) {
        case -1:
            var_r31_9 = 0;
            cr_g_SeqCurrent.unk_38 |= 0x80000;
            cr_g_SeqCurrent.unk_38 |= 0x80;
            lbl_8044F404 = 0xFF;
            break;
        case 0:
            cr_g_FileSysReady.unk_01[2] = 0;
            lbl_8044F404 = 0;
            break;
        case 5:
            var_r31_9 = 1;
            cr_g_FileSysReady.unk_01[2] = 1;
            cr_g_SeqCurrent.unk_38 |= 0x80000;
            lbl_8044F404 = 0xFC;
            break;
        case 3:
            var_r31_9 = 2;
            cr_g_FileSysReady.unk_01[2] = 2;
            cr_g_SeqCurrent.unk_38 |= 0x80000;
            lbl_8044F404 = 0xFE;
            break;
        case 1:
            lbl_8044F404 = 2;
            if (cr_g_FileSysReady.unk_01[2] != 0) {
                cr_g_SeqCurrent.unk_38 |= 0x80000;
            }
            break;
        case 4:
            var_r31_9 = 4;
            cr_g_SeqCurrent.unk_38 |= 0x80000;
            lbl_8044F404 = 0xFD;
            break;
        case 6:
            var_r31_9 = 5;
            cr_g_SeqCurrent.unk_38 |= 0x80000;
            lbl_8044F404 = 0xFB;
            break;
        case 11:
            var_r31_9 = 6;
            cr_g_SeqCurrent.unk_38 |= 0x80000;
            lbl_8044F404 = 0xFA;
            break;
        }
        if (var_r31_9 != 7) {
            fn_800224F0(fn_80066EEC(cr_g_EntryTableLoad, (u16) var_r31_9));
        }
        cr_g_FileSysReady.unk_01[0] = lbl_8044F404;
    }
}
