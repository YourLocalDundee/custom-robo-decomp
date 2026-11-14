/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80008CB0: .text 0x80008CB0-0x80008DC0 (272 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct _GXTlutObj
{
  u32 dummy[3];
} GXTlutObj;
typedef struct _GXTexObj GXTexObj;
extern u8 lbl_80396490[12];
extern u8 lbl_8038B430[24576];
extern u8 *lbl_8044F2F8;
void fn_8008E3F8(void *arg0);
struct _GXTexObj
{
  u32 dummy[8];
};
void TEXGetGXTexObjFromPalette(void *arg0, GXTexObj *arg1, s32 arg2);
void TEXGetGXTexObjFromPaletteCI(void *arg0, GXTexObj *arg1, GXTlutObj *arg2, s32 arg3, s32 arg4);
u32 cr_Archive_GetEntrySize(s32 archive, u16 entry);

/* fn_80008CB0: Nov 14, 2026 hand draft: the texture palette loader typed (TEXPalette / descriptor pairs, the GXTexObj array at lbl_8038B430 + 0x2000, the load record); the descriptor test against ((void *) 0) (li r0, 0; cmplw); the size check after the archive call an assert without its message, (offset + size < 0x154000) ? (void) 0 : (void) 0, with the call inside it (a size local took a fifth saved register and stmw); the GXTexObj base set before the palette store (42%) */

typedef struct {
    void *header;
    void *clut;
} TexDesc08;

typedef struct {
    u32 version;
    u32 count;
    TexDesc08 *desc;
} TexPal08;

typedef struct {
    u32 unk0;
    s32 entry;
    s8 state;
    s8 index;
    u8 pad[2];
    u32 offset;
} TexLoad08;

void fn_80008CB0(TexLoad08 *arg0) {
    GXTlutObj tlut;
    TexPal08 **pal;
    u8 i;
    GXTexObj *obj;

    pal = (TexPal08 **) &lbl_80396490[4];
    obj = (GXTexObj *) &lbl_8038B430[0x2000];
    *pal = (TexPal08 *) (lbl_8044F2F8 + arg0->offset);
    fn_8008E3F8(*pal);
    for (i = 0; i < (*pal)->count; i++) {
        if ((*pal)->desc[i].clut == ((void *) 0)) {
            TEXGetGXTexObjFromPalette(*pal, &obj[i], i);
        } else {
            TEXGetGXTexObjFromPaletteCI(*pal, &obj[i], &tlut, 0, i);
        }
    }
    (arg0->offset + cr_Archive_GetEntrySize(arg0->entry, arg0->index) < 0x154000) ? (void) 0 : (void) 0;
    arg0->state = 4;
}
