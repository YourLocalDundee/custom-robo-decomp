/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800812A4: .text 0x800812A4-0x80081430 (396 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u8 lbl_8044F638;
extern u16 lbl_8044F63A;
extern u8 lbl_8044F63C;
void fn_8008F6E8(u32 arg0);
s32 fn_8008EDD8(s32 arg0);
extern u8 lbl_802191D8[224];
extern const f32 lbl_80450E40;
extern u8 lbl_802193A8[28];
extern const f32 lbl_80450E7C;
extern const f32 lbl_80450E48;
extern u8 lbl_8044EAE0[8];
extern u32 lbl_8044F640[2];
typedef struct cr_GameMode
{
  u32 flags;
  u32 unk_04;
  u32 unk_08;
  u32 unk_0c;
  u32 battle_mode;
  s16 unk_14;
  s16 unk_16;
  s16 unk_18;
  u8 unk_1a[2];
  u16 unk_1c[19];
  u8 unk_42;
  u8 unk_43;
  u8 unk_44;
  u8 unk_45;
  u8 unk_46[2];
} cr_GameMode;
__declspec(section ".data") extern cr_GameMode cr_g_GameMode;
extern u8 lbl_8041E938[24];
void fn_8008057C(void);

/* fn_800812A4: Nov 3, 2026 hand draft (m2c 58%): the seven-object setup loop with only the index and the object as locals (m2c temporaries took three more saved registers), the index compared unsigned */

void fn_800812A4(void) {
    s16 i;
    void *o;

    lbl_8044F638 = 0;
    lbl_8044F63A = 0;
    lbl_8044F63C = 0;
    fn_8008F6E8(0xCU);
    for (i = 0; (u32) i < 7U; i++) {
        o = (void *) fn_8008EDD8(0xC);
        M2C_FIELD(o, void **, 0x5C) = lbl_802191D8 + (i << 5);
        M2C_FIELD(M2C_FIELD(o, void **, 0x5C), f32 *, 8) = lbl_80450E40;
        M2C_FIELD(o, s32 *, 8) = **(u16 **) (lbl_802191D8 + (i << 5) + 0x1C);
        M2C_FIELD(o, s32 *, 0x10) = i;
        M2C_FIELD(o, s32 *, 0x14) = 0;
        M2C_FIELD(o, s32 *, 0) |= 0x10;
        M2C_FIELD(o, s32 *, 0) |= 0x8000;
        M2C_FIELD(o, s32 *, 0) |= 0x40;
        M2C_FIELD(o, s32 *, 0x50) = ((s32 *) lbl_802193A8)[i];
        M2C_FIELD(o, s8 *, 0x58) = 0;
        M2C_FIELD(o, s8 *, 0x57) = 0;
        M2C_FIELD(M2C_FIELD(o, void **, 0x5C), f32 *, 0) = lbl_80450E7C;
        M2C_FIELD(M2C_FIELD(o, void **, 0x5C), f32 *, 4) = lbl_80450E48;
    }
    M2C_FIELD(lbl_8044EAE0, u8 *, 0) = 0;
    lbl_8044EAE0[1] = 0;
    lbl_8044EAE0[2] = 0;
    *lbl_8044F640 = 0;
    if (!(cr_g_GameMode.flags & 0x08000000) && !(cr_g_GameMode.flags & 0x800000) && (lbl_8041E938[3] == 0)) {
        fn_8008057C();
    }
}
