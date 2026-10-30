/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80077F38: .text 0x80077F38-0x80078000 (200 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
void fn_80021014(void *arg0);
typedef struct cr_FileSlot cr_FileSlot;
extern cr_FileSlot *lbl_8044F600;
void fn_800215F8(void *arg0);
void fn_80074FBC(s32 arg0, void *arg1);
extern u8 lbl_80379ED0[272];
extern u32 lbl_80379970[344];
typedef struct HSD_Archive HSD_Archive;
void *HSD_ArchiveGetPublicAddress(HSD_Archive *, const char *);
struct HSD_Archive;
extern char lbl_801C0B90[11];
void fn_80078000(void);
typedef struct HSD_JObj HSD_JObj;
extern HSD_JObj *lbl_8044F604;
typedef struct HSD_Joint HSD_Joint;
HSD_JObj *HSD_JObjLoadJoint(HSD_Joint *);
extern u8 lbl_802178E8[64];

extern u32 *lbl_8044F2C0;

void fn_80077F38(void) {
    u32 var_r31_11;

    fn_80021014(lbl_8044F600);
    fn_800215F8(lbl_8044F600);
    var_r31_11 = 0;
    lbl_8044F600 = NULL;
    while (var_r31_11 < (u32) *lbl_8044F2C0) {
        fn_80074FBC((s32) (((u32 *) ((u8 *) lbl_8044F2C0 + M2C_FIELD(&lbl_8044F2C0[var_r31_11], s32 *, 4)))), lbl_80379ED0 + (var_r31_11 * 0x44));
        *(((u32 *) ((u8 *) lbl_80379970 + (var_r31_11 * 4)))) = (unsigned long) (HSD_ArchiveGetPublicAddress((struct HSD_Archive *) (lbl_80379ED0 + (var_r31_11 * 0x44)), lbl_801C0B90));
        var_r31_11 += 1;
    }
    fn_80078000();
    lbl_8044F604 = HSD_JObjLoadJoint((HSD_Joint *) lbl_802178E8);
}
