r"""Read the emulated GameCube RAM of a running Dolphin (Windows), the way Dolphin Memory Engine does: find Dolphin's
mapped MEM1 block (it starts with the game ID, GXCE01 for the US disc) inside the Dolphin process and read from it.
Read-only by default.

  python -I tools/dolphin_mem.py u32 8044F2EC [8044F2E8 ...]   big-endian words at game addresses
  python -I tools/dolphin_mem.py dump 80000000 40                hex dump
  python -I tools/dolphin_mem.py arena                           the OS arena bounds (free memory left to the heap)
  python -I tools/dolphin_mem.py mem1                            the emulated MEM1 size (24 MB, or more with Dolphin's
                                                                 RAM override)
Library: open_mem1() -> Mem1 with .u32(addr), .read(addr, n).
"""
import ctypes
import ctypes.wintypes as wt
import struct
import subprocess
import sys

PROCESS_VM_READ, PROCESS_QUERY_INFORMATION = 0x0010, 0x0400
MEM_COMMIT, MEM_MAPPED = 0x1000, 0x40000
k32 = ctypes.WinDLL('kernel32', use_last_error=True)


class MBI(ctypes.Structure):
    _fields_ = [('BaseAddress', ctypes.c_void_p), ('AllocationBase', ctypes.c_void_p), ('AllocationProtect', wt.DWORD),
                ('PartitionId', wt.WORD), ('RegionSize', ctypes.c_size_t), ('State', wt.DWORD), ('Protect', wt.DWORD),
                ('Type', wt.DWORD)]


k32.OpenProcess.restype = wt.HANDLE
k32.VirtualQueryEx.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.POINTER(MBI), ctypes.c_size_t]
k32.ReadProcessMemory.argtypes = [wt.HANDLE, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]


def dolphin_pid():
    out = subprocess.run(['tasklist', '/FI', 'IMAGENAME eq Dolphin.exe', '/FO', 'CSV', '/NH'], capture_output=True,
                         text=True).stdout
    for line in out.splitlines():
        parts = [p.strip('"') for p in line.split('","')]
        if parts and parts[0].lower() == 'dolphin.exe':
            return int(parts[1])
    raise SystemExit('Dolphin is not running')


class Mem1:
    def __init__(self, h, base, size):
        self.h, self.base, self.size = h, base, size

    def read(self, addr, n):
        off = (addr & 0x7FFFFFFF) if addr >= 0x80000000 else addr
        off &= 0x3FFFFFFF
        if off + n > self.size:
            raise ValueError(f'{addr:#x} is outside MEM1 ({self.size:#x} bytes)')
        buf = ctypes.create_string_buffer(n)
        got = ctypes.c_size_t()
        if not k32.ReadProcessMemory(self.h, ctypes.c_void_p(self.base + off), buf, n, ctypes.byref(got)):
            raise OSError(ctypes.get_last_error())
        return buf.raw

    def u32(self, addr):
        return struct.unpack('>I', self.read(addr, 4))[0]


def open_mem1(game_id=b'GXCE01'):
    h = k32.OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, False, dolphin_pid())
    if not h:
        raise OSError(ctypes.get_last_error())
    addr, mbi = 0, MBI()
    while k32.VirtualQueryEx(h, ctypes.c_void_p(addr), ctypes.byref(mbi), ctypes.sizeof(mbi)):
        base, size = mbi.BaseAddress or 0, mbi.RegionSize
        if mbi.State == MEM_COMMIT and mbi.Type == MEM_MAPPED and size >= 0x1800000:
            buf = ctypes.create_string_buffer(6)
            got = ctypes.c_size_t()
            if k32.ReadProcessMemory(h, ctypes.c_void_p(base), buf, 6, ctypes.byref(got)) and buf.raw == game_id:
                return Mem1(h, base, size)
        addr = base + size
    raise SystemExit('no emulated RAM with the game ID found (is the game running?)')


def main():
    a = sys.argv[1:]
    m = open_mem1()
    if not a or a[0] == 'mem1':
        print(f'MEM1 block: {m.size:#x} bytes ({m.size / 1048576:.0f} MB) in the Dolphin process')
    elif a[0] == 'u32':
        for x in a[1:]:
            print(f'{int(x, 16):#010x}: {m.u32(int(x, 16)):#010x}')
    elif a[0] == 'dump':
        ad, n = int(a[1], 16), int(a[2], 16)
        b = m.read(ad, n)
        for i in range(0, n, 16):
            print(f'{ad + i:#010x}  ' + ' '.join(b[i + j:i + j + 4].hex() for j in range(0, 16, 4)))
    elif a[0] == 'arena':
        # OS globals: 0x80000028 physical memory size, 0x80000030 ArenaLo, 0x80000034 ArenaHi (Dolphin / SDK low mem)
        print(f'physical memory size {m.u32(0x80000028):#x}, ArenaLo {m.u32(0x80000030):#010x}, ArenaHi {m.u32(0x80000034):#010x}')


if __name__ == '__main__':
    main()
