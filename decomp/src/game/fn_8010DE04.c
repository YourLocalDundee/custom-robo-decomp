/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8010DE04: .text 0x8010DE04-0x8010DF48 (324 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80020A60(u16 arg0);
typedef struct cr_FileSlot cr_FileSlot;
cr_FileSlot *cr_Archive_OpenEntry(s32 archive, u16 entry);
extern u8 lbl_8023BA84[836];
s32 cr_FileSlot_QueueRead(cr_FileSlot *slot, void *buffer);
extern u8 *lbl_8044F2EC;

/* fn_8010DE04: Nov 13, 2026 hand draft: the rank sweep: one dead use of the parameter ((void) p;) gives it its saved register in the original order */

s32 fn_8010DE04(void *arg0, u8 arg1) {
    (void) arg0; 
    fn_80020A60(0x100U);
    if (arg1 == 0) {
        *(cr_FileSlot **) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC] = cr_Archive_OpenEntry(M2C_FIELD((lbl_8023BA84 + ((arg1 + (M2C_FIELD(arg0, u8 *, 0x4854) * 5)) * 0x14)), s32 *, 0), (u16) (M2C_FIELD(arg0, s8 *, 0x47E5) + (*(u16 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47F2] * 5)));
    } else {
        *(cr_FileSlot **) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC] = cr_Archive_OpenEntry(M2C_FIELD((lbl_8023BA84 + ((arg1 + (M2C_FIELD(arg0, u8 *, 0x4854) * 5)) * 0x14)), s32 *, 0), *(u16 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47F2]);
    }
    cr_FileSlot_QueueRead(*(cr_FileSlot **) &((u8 (*)[0xC]) (arg0))[arg1][0x47EC], &lbl_8044F2EC[M2C_FIELD((lbl_8023BA84 + ((arg1 + (M2C_FIELD(arg0, u8 *, 0x4854) * 5)) * 0x14)), s32 *, 8)]);
    *(s8 *) &((u8 (*)[0xC]) (arg0))[arg1][0x47F0] = 2;
    return 0;
}
