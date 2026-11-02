/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80090190: .text 0x80090190-0x80090298 (264 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct _GXTlutObj
{
  u32 dummy[3];
} GXTlutObj;
extern u8 lbl_8021A940[40];
extern u8 *lbl_8044F2F8;
extern u32 lbl_8044F68C;
s32 cr_Archive_OpenRequestList(void *arg0);
typedef struct _GXTexObj GXTexObj;
void TEXGetGXTexObjFromPalette(void *arg0, GXTexObj *arg1, s32 arg2);
extern u8 lbl_80387E84[1024];
void TEXGetGXTexObjFromPaletteCI(void *arg0, GXTexObj *arg1, GXTlutObj *arg2, s32 arg3, s32 arg4);
u32 cr_Archive_GetEntrySize(s32 archive, u16 entry);

/* fn_80090190: hand-written from main.dol's asm on its m2c draft (step 109b): the table slot tested as a pointer against ((void *) 0) (li r0, 0; cmplw), and an empty debug check of the archive offset after the add (lis r0, 1; cmplw, no branch) */
extern u8 *lbl_8044F6B0;

void fn_80090190(void) {
    GXTlutObj sp8;
    u16 var_r31_19;

    M2C_FIELD(lbl_8021A940, u16 *, 4) = 0xAU;
    M2C_FIELD(lbl_8021A940, u8 **, 8) = &lbl_8044F2F8[lbl_8044F68C];
    cr_Archive_OpenRequestList(lbl_8021A940);
    var_r31_19 = 0;
    while (var_r31_19 < (u32) M2C_FIELD(lbl_8044F6B0, u32 *, 4)) {
        if (*(void **) &((u8 (*)[0x8]) (M2C_FIELD(lbl_8044F6B0, s32 *, 8)))[var_r31_19][0x4] == ((void *) 0)) {
            TEXGetGXTexObjFromPalette(lbl_8044F6B0, (GXTexObj *) &lbl_80387E84[(var_r31_19 << 5) & 0x1FFFE0], (s32) var_r31_19);
        } else {
            TEXGetGXTexObjFromPaletteCI(lbl_8044F6B0, (GXTexObj *) &lbl_80387E84[(var_r31_19 << 5) & 0x1FFFE0], &sp8, 0, (s32) var_r31_19);
        }
        var_r31_19 += 1;
    }
    lbl_8044F68C += cr_Archive_GetEntrySize(0xD, M2C_FIELD(lbl_8021A940, u16 *, 4));
    if (lbl_8044F68C > 0x10000) {
        (void) 0;
    }
}
