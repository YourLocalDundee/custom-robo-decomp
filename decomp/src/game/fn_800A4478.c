/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A4478: .text 0x800A4478-0x800A44D4 (92 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 *lbl_8044F748;
void fn_800B1544(void);
s32 fn_800A4784(void *arg0);
typedef struct HSD_JObj HSD_JObj;
void HSD_JObjUnref(HSD_JObj *jobj);
void fn_80023394(s8 *arg0, u32 arg1);

void fn_800A4478(void) {
    M2C_FIELD(lbl_8044F748, s32 *, 0x88) &= 0xFFDFFFFF;
    fn_800B1544();
    if (fn_800A4784(M2C_FIELD(lbl_8044F748, HSD_JObj **, 0x90)) != 0U) {
        HSD_JObjUnref(M2C_FIELD(lbl_8044F748, HSD_JObj **, 0x90));
        fn_80023394((s8 *) lbl_8044F748, 0x174U);
    }
}
