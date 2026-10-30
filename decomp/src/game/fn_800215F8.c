/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800215F8: .text 0x800215F8-0x80021674 (124 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct DVDFileInfo DVDFileInfo;
BOOL DVDClose(DVDFileInfo *fileInfo);
struct DVDFileInfo;

void fn_800215F8(u8 *arg0) {
    if (((s8) M2C_FIELD(arg0, u8 *, 0) != -1) && ((s8) M2C_FIELD(arg0, u8 *, 0) != 0)) {
        DVDClose((struct DVDFileInfo *) (arg0 + 0x3C));
        M2C_FIELD(arg0, u8 *, 0) = 0U;
        M2C_FIELD(arg0, s8 *, 2) = 0;
        M2C_FIELD(arg0, s8 *, 1) = 1;
        M2C_FIELD(arg0, s32 *, 0xC) = 0;
        M2C_FIELD(arg0, s32 *, 4) = 0;
        M2C_FIELD(arg0, s32 *, 8) = 0;
    }
}
