/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8001838C: .text 0x8001838C-0x80018630 (676 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct DVDFileInfo DVDFileInfo;
extern u8 lbl_8031F020[392];
typedef void (*DVDCallback)(s32 result, DVDFileInfo *fileInfo);
BOOL DVDReadAsyncPrio(DVDFileInfo *fileInfo, void *addr, s32 length, s32 offset, DVDCallback callback, s32 prio);

/* fn_8001838C: Nov 3, 2026 hand draft (m2c 95%, then decomp-permuter): the THP read callback; in the loop-restart branch the frame counter is cleared before the next read size is set */

void fn_8001838C(s32 result, DVDFileInfo *fileInfo) {
    s32 var_r31_73;

    if (result == -1) {
        M2C_FIELD(lbl_8031F020, s32 *, 0xAC) = 1;
        return;
    }
    if (result != -3) {
        M2C_FIELD(lbl_8031F020, s32 *, 0xB0) = 0;
        M2C_FIELD((lbl_8031F020 + (M2C_FIELD(lbl_8031F020, s32 *, 0xB8) * 0xC)), u32 *, 0xD8) = M2C_FIELD(lbl_8031F020, u32 *, 0xC0);
        M2C_FIELD(lbl_8031F020, u32 *, 0xC0) += 1;
        M2C_FIELD((lbl_8031F020 + (M2C_FIELD(lbl_8031F020, s32 *, 0xB8) * 0xC)), s32 *, 0xDC) = 1;
        M2C_FIELD(lbl_8031F020, s32 *, 0xA8) += M2C_FIELD(lbl_8031F020, s32 *, 0xBC);
        M2C_FIELD(lbl_8031F020, s32 *, 0xBC) = (s32) *M2C_FIELD((lbl_8031F020 + (M2C_FIELD(lbl_8031F020, s32 *, 0xB8) * 0xC)), s32 **, 0xD4);
        if ((s32) (M2C_FIELD(lbl_8031F020, s32 *, 0xB8) + 1) >= 0xA) {
            var_r31_73 = 0;
        } else {
            var_r31_73 = M2C_FIELD(lbl_8031F020, s32 *, 0xB8) + 1;
        }
        M2C_FIELD(lbl_8031F020, s32 *, 0xB8) = var_r31_73;
        if ((M2C_FIELD((lbl_8031F020 + (M2C_FIELD(lbl_8031F020, s32 *, 0xB8) * 0xC)), s32 *, 0xDC) == 0) && (M2C_FIELD(lbl_8031F020, s32 *, 0xAC) == 0) && (lbl_8031F020[0xA4] == 1)) {
            if (M2C_FIELD(lbl_8031F020, u32 *, 0xC0) > (u32) (M2C_FIELD(lbl_8031F020, s32 *, 0x50) - 1)) {
                if (lbl_8031F020[0xA6] == 1) {
                    M2C_FIELD(lbl_8031F020, u32 *, 0xC0) = 0U;
                    M2C_FIELD(lbl_8031F020, s32 *, 0xA8) = (s32) M2C_FIELD(lbl_8031F020, s32 *, 0x64);
                    M2C_FIELD(lbl_8031F020, s32 *, 0xBC) = (s32) M2C_FIELD(lbl_8031F020, s32 *, 0x54);
                    goto block_12;
                }
            } else {
block_12:
                M2C_FIELD(lbl_8031F020, s32 *, 0xB0) = 1;
                if (DVDReadAsyncPrio((DVDFileInfo *) lbl_8031F020, M2C_FIELD((lbl_8031F020 + (M2C_FIELD(lbl_8031F020, s32 *, 0xB8) * 0xC)), void **, 0xD4), M2C_FIELD(lbl_8031F020, s32 *, 0xBC), M2C_FIELD(lbl_8031F020, s32 *, 0xA8), fn_8001838C, 2) != 1) {
                    M2C_FIELD(lbl_8031F020, s32 *, 0xB0) = 0;
                    M2C_FIELD(lbl_8031F020, s32 *, 0xAC) = 1;
                }
            }
        }
    }
}
