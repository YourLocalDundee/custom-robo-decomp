/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8008F4F4: .text 0x8008F4F4-0x8008F584 (144 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct _GXTlutObj
{
  u32 dummy[3];
} GXTlutObj;
typedef enum _GXTlutFmt
{
  GX_TL_IA8,
  GX_TL_RGB565,
  GX_TL_RGB5A3,
  GX_MAX_TLUTFMT
} GXTlutFmt;
void GXInitTlutObj(GXTlutObj *tlut_obj, void *lut, GXTlutFmt fmt, u16 n_entries);
void GXLoadTlut(GXTlutObj *tlut_obj, u32 tlut_name);

/* fn_8008F4F4: hand-written from main.dol's asm on its m2c draft (step 109b): the palette slot tested as a pointer against ((void *) 0) (li r0, 0; cmplw) instead of a u32 against 0 */
void fn_8008F4F4(void *arg0, void *arg1) {
    GXTlutObj sp8;
    void *t;

    if (*(void **) &((u8 (*)[0x8]) (M2C_FIELD(arg1, s32 *, 8)))[M2C_FIELD(arg0, u16 *, 0x10)][0x4] != ((void *) 0)) {
        t = (*(void **) &((u8 (*)[0x8]) (M2C_FIELD(arg1, s32 *, 8)))[M2C_FIELD(arg0, u16 *, 0x10)][0x4]);
        GXInitTlutObj(&sp8, M2C_FIELD(t, void **, 8), M2C_FIELD(t, enum _GXTlutFmt *, 4), M2C_FIELD(t, u16 *, 0));
        GXLoadTlut(&sp8, 0U);
    }
}
