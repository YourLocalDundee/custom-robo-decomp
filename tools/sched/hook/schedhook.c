/* A replacement lmgr326b.dll for an ANALYSIS COPY of the GC/2.6 compiler folder (tools/sched/bin, never the
   build's): answers the three licence calls like the stub that ships with the compiler and, when loaded into
   mwcceppc.exe, hooks the scheduler and writes what it sees to the file named by SCHEDLOG (docs/sched_model.md):
     S / B / A  Scheduler_Schedule (0x507C70): pass argument, every block (index, count, flags) before and after
     K / I      schedule_block_body (0x507D80): the block and its PCode (opcode, flags +0x14, alias +0x18, operands)
     N          the DAG at the first pick of a block: node, instruction, latency, height, deadline, preds, edges
     P          every pick (select_ready_node 0x507F50): cycle, ready candidates (height, deadline, release count)
                and the one chosen
   Build: tools/sched/hook/build.ps1. */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define RD32(a) (*(u32 *)(a))
#define RD16(a) (*(u16 *)(a))
#define RDS16(a) (*(short *)(a))
#define RD8(a) (*(u8 *)(a))

static FILE *logf_;
static int new_block;

typedef u32(__cdecl *fn1)(u32);
typedef u32(__cdecl *fn2)(u32, u32);
static fn1 orig_sched, orig_block;
static fn2 orig_pick;

static void dump_alias(u32 al)
{
    if (!al) { fprintf(logf_, " al=0"); return; }
    fprintf(logf_, " al=%08X:t%u:o%08X:%d:%d:i%d", al, RD8(al + 0x2c), RD32(al + 0x10), (int)RD32(al + 0x14),
            (int)RD32(al + 0x18), (int)RD32(al + 0x28));
    if (RD8(al + 0x2c) == 2 && RD32(al + 0x24)) {      /* a set: its member bits (0x5E2FD8 = the alias count) */
        int w, nw = ((int)RD32(0x5E2FD8) + 31) >> 5;
        fputc(':', logf_);
        for (w = 0; w < nw && w < 64; w++) fprintf(logf_, "%08X", RD32(RD32(al + 0x24) + 4 * w));
    }
}

static void dump_ins(u32 p)
{
    int i, n = RDS16(p + 0x22);
    fprintf(logf_, "I %08X op=%d fl=%08X ln=%d", p, RDS16(p + 0x20), RD32(p + 0x14), (int)RD32(p + 0x1c));
    dump_alias(RD32(p + 0x18));
    for (i = 0; i < n && i < 40; i++) {
        u32 o = p + 0x24 + 12 * i;
        u8 k = RD8(o);
        if (k == 0) fprintf(logf_, " R%u:%u:%x", RD8(o + 1), RD16(o + 4), RD16(o + 2));
        else if (k == 1) fprintf(logf_, " S%u:%x", RD16(o + 4), RD16(o + 2));
        else if (k == 2 || k == 3) fprintf(logf_, " %c%d:%08X", k == 2 ? 'M' : 'X', (int)RD32(o + 2), RD32(o + 6));
        else fprintf(logf_, " K%u", k);
    }
    fputc('\n', logf_);
}

static void dump_blocks(char tag)
{
    u32 b;
    for (b = RD32(0x5EA748); b; b = RD32(b))
        fprintf(logf_, "%c %08X idx=%d n=%d fl=%04X\n", tag, b, (int)RD32(b + 0x1c), RDS16(b + 0x28), RD16(b + 0x2a));
}

static u32 __cdecl hook_sched(u32 force)
{
    u32 r;
    fprintf(logf_, "S force=%u\n", force & 0xff);
    dump_blocks('B');
    r = orig_sched(force);
    dump_blocks('A');
    fflush(logf_);
    return r;
}

static u32 __cdecl hook_block(u32 blk)
{
    u32 p;
    fprintf(logf_, "K %08X idx=%d n=%d fl=%04X\n", blk, (int)RD32(blk + 0x1c), RDS16(blk + 0x28), RD16(blk + 0x2a));
    for (p = RD32(blk + 0x14); p; p = RD32(p))
        dump_ins(p);
    new_block = 1;
    return orig_block(blk);
}

static int release_count(u32 node)
{
    int c = 0;
    u32 e;
    for (e = RD32(node + 8); e; e = RD32(e))
        if (RDS16(RD32(e + 4) + 0x18) == 1) c++;
    return c;
}

static u32 __cdecl hook_pick(u32 list, u32 cycle)
{
    u32 n, e, r;
    u32 model = RD32(0x5E1858);
    fn1 check = (fn1)RD32(model + 0x14);
    cycle &= 0xffff;
    if (new_block) {
        new_block = 0;
        for (n = list; n; n = RD32(n)) {
            fprintf(logf_, "N %08X ins=%08X lat=%d h=%d d=%d np=%d e=%d", n, RD32(n + 0xc), RDS16(n + 0x10),
                    RDS16(n + 0x16), RDS16(n + 0x14), RDS16(n + 0x18), RDS16(n + 0x12));
            for (e = RD32(n + 8); e; e = RD32(e))
                fprintf(logf_, " %08X:%d", RD32(RD32(e + 4) + 0xc), RD16(e + 8));
            fputc('\n', logf_);
        }
    }
    fprintf(logf_, "P %u", cycle);
    for (n = list; n; n = RD32(n))
        if (RDS16(n + 0x18) == 0 && RD16(n + 0x12) <= cycle)
            fprintf(logf_, " %08X:%d:%d:%d:%d", RD32(n + 0xc), RDS16(n + 0x16), RDS16(n + 0x14), release_count(n),
                    check(RD32(n + 0xc)) ? 1 : 0);
    r = orig_pick(list, cycle);
    fprintf(logf_, " -> %08X\n", r ? RD32(r + 0xc) : 0);
    return r;
}

static void *hook(u32 addr, int len, void *fn)
{
    DWORD old;
    u8 *t = (u8 *)VirtualAlloc(0, 32, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(t, (void *)addr, len);
    t[len] = 0xE9;
    *(u32 *)(t + len + 1) = (addr + len) - ((u32)t + len + 5);
    VirtualProtect((void *)addr, len, PAGE_EXECUTE_READWRITE, &old);
    *(u8 *)addr = 0xE9;
    *(u32 *)(addr + 1) = (u32)fn - (addr + 5);
    VirtualProtect((void *)addr, len, old, &old);
    FlushInstructionCache(GetCurrentProcess(), (void *)addr, len);
    return t;
}

__declspec(dllexport) int __cdecl lp_checkin(void) { return 0; }
__declspec(dllexport) int __cdecl lp_checkout(void) { return 0; }
__declspec(dllexport) const char *__cdecl lp_errstring(void) { return ""; }

BOOL WINAPI DllMain(HINSTANCE h, DWORD reason, LPVOID r)
{
    char path[MAX_PATH];
    if (reason != DLL_PROCESS_ATTACH) return TRUE;
    if (!GetEnvironmentVariableA("SCHEDLOG", path, sizeof path)) return TRUE;   /* no log: a plain stub */
    /* only the GC/2.6 build this was written for: "Metrowerks" at 0x58D224 */
    if (IsBadReadPtr((void *)0x58D224, 10) || memcmp((void *)0x58D224, "Metrowerks", 10)) return TRUE;
    logf_ = fopen(path, "a");
    if (!logf_) return TRUE;
    setvbuf(logf_, 0, _IOFBF, 1 << 20);
    orig_sched = (fn1)hook(0x507C70, 6, hook_sched);
    orig_block = (fn1)hook(0x507D80, 7, hook_block);
    orig_pick = (fn2)hook(0x507F50, 7, hook_pick);
    return TRUE;
}
