/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80114214: .text 0x80114214-0x801142B0 (156 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8041A234[4];
typedef struct cr_FileSlot cr_FileSlot;
cr_FileSlot *cr_Archive_OpenEntry(s32 archive, u16 entry);
s32 cr_FileSlot_QueueRead(cr_FileSlot *slot, void *buffer);
extern u8 lbl_8034C100[32768];
extern u32 lbl_8037569C[2417];

void fn_80114214(void *arg0, s8 arg1, u8 arg2) {

    if ((arg2 == 1) && (M2C_FIELD(lbl_8041A234, u8 *, 5) == 1)) {
        M2C_FIELD(arg0, cr_FileSlot **, 0) = cr_Archive_OpenEntry(0xC, (u16) ((arg1 * 2) + 1));
        cr_FileSlot_QueueRead(M2C_FIELD(arg0, cr_FileSlot **, 0), lbl_8034C100);
        M2C_FIELD(arg0, s8 *, 4) = 2;
        M2C_FIELD(arg0, s8 *, 6) = arg1;
        M2C_FIELD(lbl_8037569C, s8 *, 6) = arg1;
    }
}
