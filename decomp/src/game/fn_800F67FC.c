/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_800F67FC: .text 0x800F67FC-0x800F6A1C (544 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
u8 fn_80099538(s32 arg0);
extern u8 lbl_80407250[16];
void fn_80075950(void);
void fn_80097488(s32 arg0, u8 arg1, u16 arg2);
typedef struct cr_CustomizeState
{
  u8 mode;
  u8 prev_mode;
  u8 unk_0002;
  u8 unk_0003;
  u8 unk_0004;
  u8 unk_0005;
  u8 unk_0006;
  u8 unk_0007;
  u8 unk_0008[0x1A00];
  u32 unk_1a08[4];
  f32 unk_1a18;
  f32 unk_1a1c;
  f32 unk_1a20;
  u32 unk_1a24[5];
  u32 unk_1a38;
  u32 unk_1a3c;
  u32 unk_1a40;
  u32 unk_1a44;
  u32 unk_1a48;
  u32 unk_1a4c;
  u32 unk_1a50;
  u32 unk_1a54;
  u32 unk_1a58;
  u32 unk_1a5c;
  u32 unk_1a60;
  u32 unk_1a64;
  u8 unk_1a68[0xCC];
  u32 unk_1b34;
  u32 unk_1b38;
  u32 unk_1b3c;
  u32 unk_1b40;
  u32 unk_1b44;
  u32 unk_1b48;
  u32 unk_1b4c;
  u32 unk_1b50;
} cr_CustomizeState;
__declspec(section ".data") extern cr_CustomizeState cr_g_Customize;
void fn_80078438(void);
void cr_Customize_SetMode(u8 arg0);
void fn_80124808(s8 arg0);
__declspec(section ".sdata") extern u8 lbl_8044F885[11];

u32 fn_800F67FC(void) {
    s32 var_r31;
    u32 var_r30;
    s32 var_r29;
    s32 var_r28;

    if (fn_80099538(0x10000) != 0) {
        var_r29 = 0x10000;
    } else {
        var_r29 = -1;
    }
    var_r28 = var_r29;
    var_r31 = var_r28;
    if (M2C_FIELD(lbl_80407250, u8 *, 0) & 1) {
        var_r30 = 4;
        fn_80075950();
        if (var_r31 != -1) {
            fn_80097488(var_r31, 0U, 0x1EU);
            lbl_80407250[0xC] = 0x20;
        }
    } else if (M2C_FIELD(lbl_80407250, u16 *, 2) & 0x80) {
        cr_g_Customize.mode = 1;
        var_r30 = 0xC;
        fn_80078438();
    } else if (!(M2C_FIELD(lbl_80407250, u16 *, 2) & 1) && !(M2C_FIELD(lbl_80407250, u16 *, 2) & 4) && !(M2C_FIELD(lbl_80407250, u16 *, 2) & 2)) {
        var_r30 = 4;
        fn_80075950();
        if (var_r31 != -1) {
            fn_80097488(var_r31, 0U, 0x1EU);
            lbl_80407250[0xC] = 0x20;
        }
    } else if ((M2C_FIELD(lbl_80407250, u16 *, 2) & 1) && (M2C_FIELD(lbl_80407250, u16 *, 2) & 4)) {
        var_r30 = 4;
        fn_80075950();
        if (var_r31 != -1) {
            fn_80097488(var_r31, 0U, 0x1EU);
            lbl_80407250[0xC] = 0x20;
        }
    } else {
        var_r30 = M2C_FIELD(lbl_80407250, u32 *, 4);
        fn_80078438();
        if (var_r30 == 0xC) {
            cr_Customize_SetMode(cr_g_Customize.mode);
            if (((u8) cr_g_Customize.mode != 1) && (var_r31 != -1)) {
                fn_80097488(var_r31, 0U, 0x1EU);
                lbl_80407250[0xC] = 0x20;
            }
        } else if (var_r30 == 5) {
            fn_80124808(5);
            lbl_8044F885[2] = 1;
        }
    }
    return var_r30;
}
