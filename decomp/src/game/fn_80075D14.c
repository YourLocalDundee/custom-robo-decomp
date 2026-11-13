/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80075D14: .text 0x80075D14-0x80075EE8 (468 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F5F8[8];
void DCFlushRange(void *addr, u32 nBytes);
void psInitDataBankLoad(int bank, const int *cmdBank, const int *texBank, const u32 *ref, const int *formBank);

/* fn_80075D14: Nov 5, 2026 hand draft (m2c 70%): the particle banks of lbl_8044F2F4 = logo_read_us.dbn entry 0 (u32 count, then 4 offsets per bank: generators, texture group, two more blocks; docs/WHERE_IS_EVERYTHING.md): the first time their ranges are flushed (sizes and pointers in two 10-entry arrays indexed by the same i that steps by 4; sizes declared first: the higher stack slot), then every bank (id 0x43, else hang) goes to psInitDataBankLoad. The table words as `*(tbl + i + k)` (i * 4 + 4k, as the original; `tbl[i + k]` computes (i + k) * 4), the id compared as (s32) (cmpwi) */
extern u32 *lbl_8044F2F4;

void fn_80075D14(void) {
    s32 i;
    u16 *bank;
    u32 sizes[10];
    void *ptrs[10];

    if (*lbl_8044F5F8 == 0) {
        for (i = 0; i < 10; i++) {
            sizes[i] = 0;
            ptrs[i] = NULL;
        }
        for (i = 0; (u32) i < lbl_8044F2F4[0]; i += 4) {
            sizes[i] = *(lbl_8044F2F4 + i + 2) - *(lbl_8044F2F4 + i + 1);
            ptrs[i] = (u8 *) lbl_8044F2F4 + *(lbl_8044F2F4 + i + 1);
        }
        for (i = 0; sizes[i] != 0; i++) {
            DCFlushRange(ptrs[i], (sizes[i] + 0x1F) & ~0x1F);
        }
        *lbl_8044F5F8 = 1;
    }
    for (i = 0; (u32) i < lbl_8044F2F4[0]; i += 4) {
        bank = (u16 *) ((u8 *) lbl_8044F2F4 + *(lbl_8044F2F4 + i + 1));
        if ((s32) bank[0] != 0x43) {
            while (1) {
            }
        }
        psInitDataBankLoad(bank[1], (const int *) ((u8 *) lbl_8044F2F4 + *(lbl_8044F2F4 + i + 1)), (const int *) ((u8 *) lbl_8044F2F4 + *(lbl_8044F2F4 + i + 2)),
                           (void *) ((u8 *) lbl_8044F2F4 + *(lbl_8044F2F4 + i + 3)), (const int *) ((u8 *) lbl_8044F2F4 + *(lbl_8044F2F4 + i + 4)));
    }
}
