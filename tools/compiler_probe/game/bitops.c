/* Hand-written C for tiny game functions (no game source exists), to find the game code's compiler flags.
 * Names are the dtk names (fn_<address>), so tools/compiler_probe/probe.py compares them with main.dol.
 * Written from the instruction semantics only; the probe reports which compiler + flags reproduce the bytes. */
typedef unsigned long u32;
typedef long s32;
typedef int BOOL;

/* 0x800231B4: test bit i of a bit array */
u32 fn_800231B4(u32 *bits, u32 i)
{
    u32 word = i >> 5;
    u32 mask = 1 << (i & 31);
    return mask & bits[word];
}

/* 0x800231EC: clear bit i */
void fn_800231EC(u32 *bits, u32 i)
{
    u32 word = i >> 5;
    u32 mask = 1 << (i & 31);
    bits[word] &= ~mask;
}

/* 0x80023228: set bit i */
void fn_80023228(u32 *bits, u32 i)
{
    u32 word = i >> 5;
    u32 mask = 1 << (i & 31);
    bits[word] |= mask;
}

/* 0x80023264: copy size bytes as words (the pointers are locals: level 0 keeps them in r30 / r29) */
void fn_80023264(void *dst, void *src, u32 size)
{
    s32 n = size >> 2;
    u32 *d = dst;
    u32 *s = src;
    while (n > 0) {
        *d++ = *s++;
        n--;
    }
}

/* 0x8002312C: copy a small-data global, init the ROM font, hang on failure */
extern u32 lbl_sda_a;
extern u32 lbl_sda_b;
extern BOOL OSInitFont(void *fontData);
void fn_8002312C(void)
{
    lbl_sda_b = lbl_sda_a;
    if (!OSInitFont((void *)lbl_sda_b)) {
        while (1) {
        }
    }
}

/* 0x80023164: count something when flag bit 28 is set, unless state +0x38 bit 14 is set and +0x10 != 3 */
struct S1 { u32 flags; u32 pad[2]; u32 count; u32 pad2[14]; };
struct S2 { u32 pad[4]; u32 state; u32 pad2[9]; u32 flags; u32 pad3[8]; };
extern struct S1 lbl_80321D40;
extern struct S2 lbl_80321D88;
void fn_80023164(void)
{
    if (lbl_80321D40.flags & 0x10000000) {
        if (!(lbl_80321D88.flags & 0x4000) || lbl_80321D88.state == 3) {
            lbl_80321D40.count++;
        }
    }
}
