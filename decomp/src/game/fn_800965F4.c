/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800965F4: .text 0x800965F4-0x80096754 (352 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
typedef struct cr_BgmSlot cr_BgmSlot;
__declspec(section ".sdata") extern f32 cr_g_BgmVolume;
struct cr_BgmSlot
{
  u32 flags;
  u32 unk_04;
  u32 song;
  u8 start_volume;
  u8 volume;
  u8 unk_0e[0x22];
  s16 fade_time;
  u8 unk_32[6];
};
__declspec(section ".data") extern cr_BgmSlot cr_g_BgmSlots[2];
extern u8 lbl_8044EAF8[8];

/* fn_800965F4: Nov 13, 2026 hand draft: two expansions of the volume loop: each starts from the slot table itself (not a copy), the second reads slot 1 before taking the table; the fade time a chained assignment through a u16 field as in fn_80096E28; rank_solve.py order */

void fn_800965F4(f32 farg0) {
    f32 var_f31;
    f32 var_f30;
    cr_BgmSlot *var_r31;
    cr_BgmSlot *var_r30;
    cr_BgmSlot *var_r29;
    u16 var_r28;
    u16 var_r27;
    u16 var_r26;
    u16 var_r25;
    u32 var_r24;
    u8 var_r23;
    u32 var_r22;
    u8 var_r21;

    (void) var_r29; 
    (void) var_r27; 
    cr_g_BgmVolume = farg0;
    var_r29 = cr_g_BgmSlots;
    var_r23 = *lbl_8044EAF8;
    var_f31 = cr_g_BgmVolume;
    var_r24 = var_r29->song;
    var_r31 = cr_g_BgmSlots;
    var_r27 = 1;
    var_r28 = 0;
    while (var_r28 < 2U) {
        if (((s32) var_r31->song == (s32) var_r24) && (var_r31->flags & 0x80000000)) {
            var_r31->volume = var_r23;
            M2C_FIELD(var_r31, f32 *, 0x10) = var_f31;
            *(u16 *) &var_r31->fade_time = var_r27 = (var_r27 * 0xC8) / 60;
            var_r31->flags |= 0x10;
        }
        var_r28 += 1;
        var_r31 = (cr_BgmSlot *) ((u8 *) var_r31 + 0x38);
    }
    var_f30 = cr_g_BgmVolume;
    var_r21 = *lbl_8044EAF8;
    var_r22 = cr_g_BgmSlots[1].song;
    var_r30 = cr_g_BgmSlots;
    var_r25 = 1;
    var_r26 = 0;
    while (var_r26 < 2U) {
        if (((s32) var_r30->song == (s32) var_r22) && (var_r30->flags & 0x80000000)) {
            var_r30->volume = var_r21;
            M2C_FIELD(var_r30, f32 *, 0x10) = var_f30;
            *(u16 *) &var_r30->fade_time = var_r25 = (var_r25 * 0xC8) / 60;
            var_r30->flags |= 0x10;
        }
        var_r26 += 1;
        var_r30 = (cr_BgmSlot *) ((u8 *) var_r30 + 0x38);
    }
}
