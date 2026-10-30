/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80008E80: .text 0x80008E80-0x80008ED8 (88 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_FileSlot cr_FileSlot;
cr_FileSlot *cr_Archive_OpenEntry(s32 archive, u16 entry);
void cr_FileSlot_QueueRead(cr_FileSlot *slot, void *buffer);
extern u8 *lbl_8044F2F8;

void fn_80008E80(void *arg0) {
    M2C_FIELD(arg0, cr_FileSlot **, 0) = cr_Archive_OpenEntry(M2C_FIELD(arg0, s32 *, 4), (u16) M2C_FIELD(arg0, s8 *, 9));
    cr_FileSlot_QueueRead(M2C_FIELD(arg0, cr_FileSlot **, 0), &lbl_8044F2F8[M2C_FIELD(arg0, s32 *, 0xC)]);
    M2C_FIELD(arg0, s8 *, 8) = 1;
}
