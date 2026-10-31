/* Custom Robo game code, promoted by tools/promote_drafts.py; edit by hand only after removing this line.
 * fn_80017668: .text 0x80017668-0x800177F0 (392 bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).
 * One function per object: the game's own source files are not known yet, so this is not the original file.
 * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */
#include "game/cr_game.h"

/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */
extern u32 lbl_8044F360;
extern u32 lbl_8044F3C4;
void AIInitDMA(u32 start_addr, u32 length);
extern u8 lbl_8031EAE0[1280];
BOOL OSEnableInterrupts(void);
void fn_800177F0(void *arg0, void *arg1, u32 arg2);
void DCFlushRange(void *addr, u32 nBytes);
BOOL OSRestoreInterrupts(BOOL level);
extern u8 *lbl_8044F3C0;
extern u8 *lbl_8044F3BC;
extern u32 lbl_8044F35C;
u32 AIGetDMAStartAddr(void);
void DCInvalidateRange(void *addr, u32 nBytes);

void fn_80017668(void) {
    s32 var_r31;

    if ((s32) lbl_8044F360 == 0) {
        lbl_8044F3C4 ^= 1;
        AIInitDMA((u32) (lbl_8031EAE0 + (lbl_8044F3C4 * 0x280)), 0x280U);
        var_r31 = OSEnableInterrupts();
        fn_800177F0(lbl_8031EAE0 + (lbl_8044F3C4 * 0x280), NULL, 0xA0U);
        DCFlushRange(lbl_8031EAE0 + (lbl_8044F3C4 * 0x280), 0x280U);
        OSRestoreInterrupts(var_r31);
        return;
    }
    if ((s32) lbl_8044F360 == 1) {
        if ((u8 *) lbl_8044F3C0 != NULL) {
            lbl_8044F3BC = lbl_8044F3C0;
        }
        ((M2C_UNK (*)()) lbl_8044F35C)();
        lbl_8044F3C0 = (unsigned char *) (AIGetDMAStartAddr() + 0x80000000);
    } else {
        ((M2C_UNK (*)()) lbl_8044F35C)();
        lbl_8044F3BC = (unsigned char *) (AIGetDMAStartAddr() + 0x80000000);
    }
    lbl_8044F3C4 ^= 1;
    AIInitDMA((u32) (lbl_8031EAE0 + (lbl_8044F3C4 * 0x280)), 0x280U);
    var_r31 = OSEnableInterrupts();
    if ((u8 *) lbl_8044F3BC != NULL) {
        DCInvalidateRange(lbl_8044F3BC, 0x280U);
    }
    fn_800177F0(lbl_8031EAE0 + (lbl_8044F3C4 * 0x280), lbl_8044F3BC, 0xA0U);
    DCFlushRange(lbl_8031EAE0 + (lbl_8044F3C4 * 0x280), 0x280U);
    OSRestoreInterrupts(var_r31);
}
