/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800F9014: .text 0x800F9014-0x800F91E0 (460 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
u16 fn_800F8820(void *arg0, u16 *arg1, u16 *arg2);
s32 cr_Text_GetBodyName(u16 arg0);
s32 cr_Text_GetGunName(s32 arg0);
__declspec(section ".sdata") extern s32 cr_g_BodyCount;
s32 cr_Text_GetBombName(s32 arg0);
__declspec(section ".sdata") extern s32 cr_g_GunCount;
s32 cr_Text_GetPodName(s32 arg0);
__declspec(section ".sdata") extern s32 cr_g_BombCount;
s32 cr_Text_GetLegName(s32 arg0);
__declspec(section ".sdata") extern s32 cr_g_PodCount;
void cr_Text_MaskUnownedSpecialPart(u16 arg0, u32 arg1, s16 *arg2);
extern u8 lbl_804005F4[20];
s32 cr_Text_GetPartDescription(u16 arg0);
extern u8 lbl_8023A004[4940];

/* fn_800F9014: Nov 6, 2026 hand draft (m2c 89%): the offsets summed index first then the counts in kind order, the kind local u32, a (void) mention of the dead r29 local */

void *fn_800F9014(void *arg0) {
    u16 spC;
    u16 spA;
    s8 sp8;
    u32 var_r30;
    s32 var_r29;

    (void) var_r29;
    var_r30 = 0;
    var_r29 = 0;
    spC = 0;
    spA = 0xFF;
    sp8 = 0;
    if ((M2C_FIELD(arg0, u8 *, 0x4855) == 1) || (M2C_FIELD(arg0, u8 *, 0x4855) == 0)) {
        fn_800F8820(arg0, &spA, &spC);
        var_r30 = spC;
        switch ((s32) spA) {                        /* irregular */
        case 0:
            M2C_FIELD(arg0, s32 *, 0x178) = cr_Text_GetBodyName(spC);
            break;
        case 1:
            M2C_FIELD(arg0, s32 *, 0x178) = cr_Text_GetGunName((s32) spC);
            spC += cr_g_BodyCount;
            break;
        case 2:
            M2C_FIELD(arg0, s32 *, 0x178) = cr_Text_GetBombName((s32) spC);
            spC = spC + (cr_g_BodyCount + cr_g_GunCount);
            break;
        case 3:
            M2C_FIELD(arg0, s32 *, 0x178) = cr_Text_GetPodName((s32) spC);
            spC = spC + ((cr_g_BodyCount + cr_g_GunCount) + cr_g_BombCount);
            break;
        case 4:
            M2C_FIELD(arg0, s32 *, 0x178) = cr_Text_GetLegName((s32) spC);
            spC = spC + (((cr_g_BodyCount + cr_g_GunCount) + cr_g_BombCount) + cr_g_PodCount);
            break;
        }
        cr_Text_MaskUnownedSpecialPart(spA, (u32) var_r30, (s16 *) &spC);
        M2C_FIELD(lbl_804005F4, s32 *, 0x10) = cr_Text_GetPartDescription(spC);
        M2C_FIELD((lbl_8023A004 + (spC * 0x14)), s32 *, 8) = (s32) spC;
        return lbl_8023A004 + (spC * 0x14);
    }
    return NULL;
}
