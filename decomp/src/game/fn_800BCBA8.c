/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800BCBA8: .text 0x800BCBA8-0x800BCBFC (84 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct HSD_JObj HSD_JObj;
void fn_8006E690(HSD_JObj *arg0);

void fn_800BCBA8(void *arg0) {
    if ((M2C_FIELD(arg0, u16 *, 0x1942) & 0x10) && ((HSD_JObj *) M2C_FIELD(arg0, HSD_JObj **, 0xB40) != NULL)) {
        fn_8006E690(M2C_FIELD(arg0, HSD_JObj **, 0xB40));
        M2C_FIELD(arg0, u16 *, 0x1942) = (u16) (M2C_FIELD(arg0, u16 *, 0x1942) & 0xFFFFFFEF);
    }
}
