/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800A0C20: .text 0x800A0C20-0x800A0CB0 (144 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_803E6688[3];

/* fn_800A0C20: Nov 5, 2026 hand draft (m2c 86%): relocates the loaded file (two header pointers, then the relocation table) and keeps its first table in lbl_8044F788. The relocation loop as `p = (u8 **) (base + relocs[i++]); *p += (u32) base;` (as in cr_ModelEntry_LoadAndBuild): m2c's separate `i += 1` swaps the table and entry registers */

// lbl_8044F40C: the drafts type it differently (CR_MIXED), so the context declares nothing (Nov 13, 2026)
extern u8 *lbl_8044F40C;
extern u8 *lbl_8044F788;

void fn_800A0C20(void) {
    u8 *hdr;
    u8 *base;
    s32 i;
    s32 *relocs;
    u8 **p;

    i = 0;
    hdr = M2C_FIELD(lbl_8044F40C, void **, 8);
    base = M2C_FIELD(lbl_8044F40C, u8 **, 8) + 0x40;
    M2C_FIELD(hdr, u8 **, 8) = (u8 *) ((u32) M2C_FIELD(hdr, u8 **, 8) + (u32) base);
    M2C_FIELD(hdr, u8 **, 0xC) = (u8 *) ((u32) M2C_FIELD(hdr, u8 **, 0xC) + (u32) base);
    relocs = (s32 *) (base + M2C_FIELD(hdr, s32 *, 0x18));
    while (i < M2C_FIELD(hdr, s32 *, 0x1C)) {
        p = (u8 **) (base + relocs[i++]);
        *p += (u32) base;
    }
    lbl_8044F788 = M2C_FIELD(hdr, void **, 8);
    lbl_803E6688[2] = 0;
}
