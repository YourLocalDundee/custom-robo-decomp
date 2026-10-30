/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * cr_Motion_LoadInfo: .text 0x80037A80-0x80037D14 (660 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
__declspec(section ".data") extern u32 *cr_g_MotionData[1280];
extern u32 lbl_8044F4E4;
extern u32 lbl_8044F4DC;
extern u32 lbl_8044F4CC;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_0678;
extern u32 lbl_8044F4E8;
__declspec(section ".data") extern u32 *cr_g_MotionInfo[1095];
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_06dc;
extern u32 lbl_8044F4D8;
__declspec(section ".sdata") extern u8 *cr_g_CurrentChara_026c;
void fn_8005CE74(void);

extern u32 *lbl_8044F594;

void cr_Motion_LoadInfo(void) {
    void *temp_r31_63;

    if ((u32 *) cr_g_MotionData[lbl_8044F4E4] == NULL) {
        lbl_8044F4E4 = 0x1E7;
    }
    lbl_8044F4DC = lbl_8044F4E4;
    lbl_8044F4CC = M2C_FIELD(cr_g_CurrentChara_0678, u32 *, 0x1C);
    M2C_FIELD(cr_g_CurrentChara_0678, u32 *, 8) = (u32) lbl_8044F4CC;
    lbl_8044F4CC = 0;
    M2C_FIELD(cr_g_CurrentChara_0678, s16 *, 0x1A) = (s16) lbl_8044F4CC;
    lbl_8044F4E8 = (u32) M2C_FIELD(cr_g_CurrentChara_0678, s16 *, 0x44);
    M2C_FIELD(cr_g_CurrentChara_0678, s16 *, 0x44) = (s16) lbl_8044F4CC;
    M2C_FIELD(cr_g_CurrentChara_0678, s16 *, 0x46) = (s16) lbl_8044F4E8;
    M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x36) = M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x34);
    if (lbl_8044F4DC != 0) {
        lbl_8044F594 = cr_g_MotionInfo[lbl_8044F4DC];
        temp_r31_63 = M2C_FIELD(lbl_8044F594, void **, 4);
        lbl_8044F4CC = (u32) M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x28);
        M2C_FIELD(cr_g_CurrentChara_0678, s16 *, 0x2A) = (s16) lbl_8044F4CC;
        lbl_8044F4E8 = lbl_8044F4DC;
        lbl_8044F4E8 = (u32) M2C_FIELD(temp_r31_63, u16 *, 0);
        lbl_8044F4E8 &= 0xFFF;
        M2C_FIELD(cr_g_CurrentChara_0678, s16 *, 0x18) = (s16) lbl_8044F4E8;
        M2C_FIELD(cr_g_CurrentChara_0678, s16 *, 0x26) = (s16) lbl_8044F4E8;
        M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x28) = (u16) lbl_8044F4E8;
        M2C_FIELD(cr_g_CurrentChara_06dc, s32 *, 8) = (s32) **(((u32 * *) ((u8 *) cr_g_MotionData + ((lbl_8044F4DC * 4) & 0x3FFC))));
        M2C_FIELD(cr_g_CurrentChara_06dc, s32 *, 0) = (s32) M2C_FIELD(*(((u32 * *) ((u8 *) cr_g_MotionData + ((lbl_8044F4DC * 4) & 0x3FFC)))), s32 *, 4);
        M2C_FIELD(cr_g_CurrentChara_06dc, s32 *, 4) = (s32) M2C_FIELD(*(((u32 * *) ((u8 *) cr_g_MotionData + ((lbl_8044F4DC * 4) & 0x3FFC)))), s32 *, 8);
        lbl_8044F4E8 = M2C_FIELD(cr_g_CurrentChara_0678, u32 *, 0x1C);
        lbl_8044F4D8 = M2C_FIELD(temp_r31_63, u32 *, 4);
        M2C_FIELD(cr_g_CurrentChara_0678, u32 *, 4) = (u32) lbl_8044F4E8;
        M2C_FIELD(cr_g_CurrentChara_0678, u32 *, 0x1C) = (u32) lbl_8044F4D8;
        lbl_8044F4D8 &= 0xFFDFFFFF;
        if (lbl_8044F4D8 & 0x40) {
            lbl_8044F4E8 = 0x8000;
            M2C_FIELD(cr_g_CurrentChara_0678, s16 *, 0x44) = (s16) lbl_8044F4E8;
        }
        if (lbl_8044F4D8 & 0x80000) {
            lbl_8044F4D8 &= 0xFFFFBFFF;
        }
        if (!(lbl_8044F4D8 & 0x100)) {
            lbl_8044F4E8 = M2C_FIELD(cr_g_CurrentChara_026c, u32 *, 0x168);
            M2C_FIELD(cr_g_CurrentChara_026c, u32 *, 0x168) = (u32) lbl_8044F4E8;
        }
        M2C_FIELD(cr_g_CurrentChara_0678, u32 *, 0x10) = (u32) lbl_8044F4D8;
        lbl_8044F4E8 = M2C_FIELD(temp_r31_63, u32 *, 0xC);
        M2C_FIELD(cr_g_CurrentChara_0678, u32 *, 0x20) = (u32) lbl_8044F4E8;
        M2C_FIELD(cr_g_CurrentChara_0678, u32 *, 0x14) = (u32) lbl_8044F4E8;
        M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x32) = M2C_FIELD(temp_r31_63, u16 *, 8);
        M2C_FIELD(cr_g_CurrentChara_0678, u16 *, 0x34) = M2C_FIELD(temp_r31_63, u16 *, 0xA);
        lbl_8044F594 += 2;
        fn_8005CE74();
    }
}
