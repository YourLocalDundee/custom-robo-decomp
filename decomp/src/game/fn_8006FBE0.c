/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_8006FBE0: .text 0x8006FBE0-0x8006FDD0 (496 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8037569C[2417];
void __assert(const char *, u32, const char *);
extern const char lbl_80450BD0[7];
extern const char lbl_80450BD7[5];
extern const u8 lbl_801C0B58[24];
typedef struct 
{
  f32 x;
  f32 y;
  f32 z;
} Vec;
typedef Vec Vec3;
extern const f32 lbl_80450BAC;
extern const f32 lbl_80450B90;
extern const f32 lbl_80450B94;

void fn_8006FBE0(void *arg0) {
    struct {
        f32 sp8;
        u8 pad_C[4];
        f32 sp10;
        f32 sp14;
        s32 sp18;
        f32 sp1C;
    } sp;
    void *temp_r30_33;

    if (((M2C_FIELD(arg0, s32 *, 0x334F0) &= 0x30) != 0) && (M2C_FIELD((((u32 *) ((u8 *) lbl_8037569C + (M2C_FIELD(arg0, u8 *, 0x33500) * 0x13C)))), s32 *, 0xF0) == 0) && (M2C_FIELD(lbl_8037569C, u8 *, 0x25BC) != 0)) {
        temp_r30_33 = M2C_FIELD((((u32 *) ((u8 *) lbl_8037569C + (M2C_FIELD(arg0, u8 *, 0x33500) * 0x13C)))), void **, 0xB8);
        (temp_r30_33 != NULL) ? (void) 0 : __assert(lbl_80450BD0, 0x3E4U, lbl_80450BD7);
        (&sp.sp14 != NULL) ? (void) 0 : __assert(lbl_80450BD0, 0x3E5U, (const char *) ((s8 *) lbl_801C0B58));
        *(Vec3 *) &sp.sp14 = *(Vec3 *) ((u8 *) temp_r30_33 + 0x38);
        sp.sp8 = M2C_FIELD(arg0, f32 *, 0x334BC) - sp.sp14;
        sp.sp10 = M2C_FIELD(arg0, f32 *, 0x334C4) - sp.sp1C;
        if (sp.sp8 > lbl_80450BAC) {
            if (sp.sp10 > lbl_80450BAC) {
                M2C_FIELD((((u32 *) ((u8 *) lbl_8037569C + (M2C_FIELD(arg0, u8 *, 0x33500) * 0x13C)))), f32 *, 0x128) = (f32) lbl_80450B90;
            } else {
                M2C_FIELD((((u32 *) ((u8 *) lbl_8037569C + (M2C_FIELD(arg0, u8 *, 0x33500) * 0x13C)))), f32 *, 0x128) = (f32) lbl_80450B94;
            }
        } else if (sp.sp10 > lbl_80450BAC) {
            M2C_FIELD((((u32 *) ((u8 *) lbl_8037569C + (M2C_FIELD(arg0, u8 *, 0x33500) * 0x13C)))), f32 *, 0x128) = (f32) lbl_80450B94;
        } else {
            M2C_FIELD((((u32 *) ((u8 *) lbl_8037569C + (M2C_FIELD(arg0, u8 *, 0x33500) * 0x13C)))), f32 *, 0x128) = (f32) lbl_80450B90;
        }
        M2C_FIELD((((u32 *) ((u8 *) lbl_8037569C + (M2C_FIELD(arg0, u8 *, 0x33500) * 0x13C)))), s32 *, 0xF0) = 0xB4;
    }
}
