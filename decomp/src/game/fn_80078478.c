/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80078478: .text 0x80078478-0x800784B0 (56 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* used from the HSD / SDK headers and symbols/cr_types.h (generated from the drafts' context) */
typedef struct cr_FileSlot cr_FileSlot;
cr_FileSlot *cr_Archive_OpenEntry(s32 archive, u16 entry);
void cr_FileSlot_QueueRead(cr_FileSlot *slot, void *buffer);

extern void *lbl_8044F2C0;
extern cr_FileSlot *lbl_8044F600;

void fn_80078478(void) {
    lbl_8044F600 = cr_Archive_OpenEntry(0x18, 0U);
    cr_FileSlot_QueueRead(lbl_8044F600, lbl_8044F2C0);
}
