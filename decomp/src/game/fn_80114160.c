/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80114160: .text 0x80114160-0x80114214 (180 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_FileSlot cr_FileSlot;
cr_FileSlot *cr_Archive_OpenEntry(s32 archive, u16 entry);
void cr_FileSlot_QueueRead(cr_FileSlot *slot, void *buffer);
extern u8 *lbl_8044F2E8;
extern u8 lbl_8034C100[32768];
extern u32 lbl_8037569C[2417];

void fn_80114160(void *arg0, s8 arg1, u8 arg2) {
    if (arg2 == 0) {
        M2C_FIELD(arg0, cr_FileSlot **, 0) = cr_Archive_OpenEntry(0xC, (u16) (arg2 + (arg1 * 2)));
        if (arg2 == 0) {
            cr_FileSlot_QueueRead(M2C_FIELD(arg0, cr_FileSlot **, 0), lbl_8044F2E8);
        } else {
            cr_FileSlot_QueueRead(M2C_FIELD(arg0, cr_FileSlot **, 0), lbl_8034C100);
        }
        M2C_FIELD(arg0, s8 *, 4) = 2;
        M2C_FIELD(arg0, s8 *, 6) = arg1;
        M2C_FIELD(lbl_8037569C, s8 *, 6) = arg1;
        return;
    }
    M2C_FIELD(arg0, s8 *, 4) = 6;
}
