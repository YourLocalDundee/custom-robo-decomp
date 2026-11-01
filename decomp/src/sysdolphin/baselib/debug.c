/* Custom Robo: from doldecomp/melee @ 598987cf (tools/downloads/melee_latest), src/sysdolphin/baselib/debug.c (__assert, HSD_Panic); HSD_SaveContext transcribed from doldecomp/kar asm/sysdolphin/debug.s */ #include "debug.h"

#include <dolphin/os.h>

/* Custom Robo: the split debug.c (plan.md step 97) is .text 0x801AE9B4-0x801AEB40 with its .data strings and the
   .sdata "" only; its context buffer and panic callback are in automatic objects: main.dol's lbl_8044BFB0 (the
   OSContext HSD_SaveContext fills; Melee's HSD_Debug_804C2608) and lbl_80450068 (Melee's panicCallback). Melee's
   report_func, HSD_LogInit and the two setters are not in main.dol here. */
extern OSContext lbl_8044BFB0;
extern PanicCallback lbl_80450068;

void OSFillFPUContext(OSContext* context);

/* doldecomp/kar asm/sysdolphin/debug.s HSD_SaveContext (this revision; Melee calls OSSaveContext instead): every GPR
   with r3 kept in SPRG0, the GQRs, CR, LR, CTR, XER, SRR0/1, then state |= 1 and the FPU context */
asm void HSD_SaveContext(void)
{
    nofralloc
    mtsprg  0, r3
    lis     r3, lbl_8044BFB0@ha
    addi    r3, r3, lbl_8044BFB0@l
    stmw    r0, 0x0(r3)
    mfsprg  r4, 0
    stw     r4, 0xc(r3)
    mfspr   r4, GQR0
    stw     r4, 0x1a4(r3)
    mfspr   r4, GQR1
    stw     r4, 0x1a8(r3)
    mfspr   r4, GQR2
    stw     r4, 0x1ac(r3)
    mfspr   r4, GQR3
    stw     r4, 0x1b0(r3)
    mfspr   r4, GQR4
    stw     r4, 0x1b4(r3)
    mfspr   r4, GQR5
    stw     r4, 0x1b8(r3)
    mfspr   r4, GQR6
    stw     r4, 0x1bc(r3)
    mfspr   r4, GQR7
    stw     r4, 0x1c0(r3)
    mfcr    r4
    stw     r4, 0x80(r3)
    mflr    r4
    stw     r4, 0x84(r3)
    mfctr   r4
    stw     r4, 0x88(r3)
    mfxer   r4
    stw     r4, 0x8c(r3)
    mfsrr0  r4
    stw     r4, 0x198(r3)
    mfsrr1  r4
    stw     r4, 0x19c(r3)
    lhz     r4, 0x1a2(r3)
    ori     r4, r4, 0x1
    sth     r4, 0x1a2(r3)
    b       OSFillFPUContext
}

void __assert(const char* str, u32 arg1, const char* arg2)
{
    OSReport("assertion \"%s\" failed", arg2);
    HSD_Panic(str, arg1, "");
}

void HSD_Panic(const char* arg0, u32 line, const char* arg2)
{
    if (lbl_80450068 != NULL) {
        HSD_SaveContext(); /* Custom Robo: Melee OSSaveContext(&HSD_Debug_804C2608.context) */
        OSReport("%s in %s on line %d.\n", arg2, arg0, line);
        lbl_80450068(&lbl_8044BFB0);
    }
    OSPanic(arg0, line, arg2);
}
