"""Write code/samurai.txt: robo slots in Dolphin's expanded RAM, the SDK memory map, the per-mesh material patch.

  python -I mods/mecha_samurai_full/make_code.py

Layout (docs/full_custom_robo.md): robo r's slot is SLOT bytes at 0x81A00000 + r * SLOT; the body at +0 (up to
BODY bytes), then gun / bomb / pod / legs PART bytes each. 4 x 3 MB ends at 0x82600000, inside the 48 MB map
(0x82FFFFFF) and above everything the game uses (its ArenaHi is 0x817F9BC0).
"""
import struct
from pathlib import Path

G = Path(__file__).resolve().parents[2]
MOD = Path(__file__).resolve().parent
REGION = 0x80C07040          # *lbl_8044F2EC at run time (tools/dolphin_mem.py)
BASE, SLOT, BODY, PART = 0x81A00000, 0x300000, 0x200000, 0x40000
REQ = 0x8023BA84             # part load requests, 0x14 per record, 5 per robo (body, gun, bomb, pod, legs)
DRAW = [(0x80039078, 'fn_80038CF8 body'), (0x800390E0, 'fn_80038CF8 gun'), (0x80039148, 'fn_80038CF8 bomb'),
        (0x800391AC, 'fn_80038CF8 pod A'), (0x800391C8, 'fn_80038CF8 pod B'), (0x8003922C, 'fn_80038CF8 legs'),
        (0x80039248, 'fn_80038CF8 legs'), (0x80039264, 'fn_80038CF8 legs'), (0x80039280, 'fn_80038CF8 legs'),
        (0x800A625C, 'fn_800A6068 story robo'), (0x800A637C, 'fn_800A6068 story robo'),
        (0x800BCD34, 'fn_800BCC60 chara loop')]

d = (G / 'Custom Robo extracted' / 'sys' / 'main.dol').read_bytes()
offs, addrs, sizes = (struct.unpack('>18I', d[i:i + 72]) for i in (0, 72, 144))


def rd(a):
    for o, ad, s in zip(offs, addrs, sizes):
        if ad <= a < ad + s:
            return struct.unpack('>I', d[o + a - ad:o + a - ad + 4])[0]
    raise KeyError(hex(a))


L = ["""; Mecha samurai, fully custom (docs/full_custom_robo.md; written by make_code.py). Needs Dolphin MEM1 64 MB
; (Config > Advanced > Enable Emulated Memory Size Override). Three changes:
; 1. Each robo's body AND parts load into its own %d MB slot of the expanded RAM, robo r at %#x + r * %#x:
;    body +0 (up to %#x bytes), then gun / bomb / pod / legs (%#x each). The part load requests lbl_8023BA84
;    (0x14 per record: archive, 0, offset, loader, part index) give offsets from the part region *lbl_8044F2EC =
;    0x80C07040 (fn_8010DE04 / fn_8010DBA8: destination = region + offset).
; 2. The SDK maps that RAM for the CPU (Config48MB) and does not write-protect it (mods/mecha_samurai_big).
; 3. Every mesh draws with its own material: the robo draw loops call fn_80095E6C (HSD_JObjDispAll with a render
;    mode) with 0x04000000, which makes HSD skip each mesh's material setup after the loop set up one material for
;    the whole body / part; with 0 each mesh sets up its own material and textures (a texture per body section).
;    Retail models share one material in every mesh, so they draw the same.""" % (SLOT >> 20, BASE, SLOT, BODY, PART)]
names = ['body', 'gun', 'bomb', 'pod', 'legs']
sub = [0, BODY, BODY + PART, BODY + 2 * PART, BODY + 3 * PART]
assert BODY + 4 * PART <= SLOT and BASE + 4 * SLOT <= 0x83000000
for r in range(4):
    for k in range(5):
        a = REQ + (r * 5 + k) * 0x14 + 8
        dest = BASE + r * SLOT + sub[k]
        L.append(f'{a:08X}: {dest - REGION:08X} [was {rd(a):08X}] ; robo {r} {names[k]}: {dest:#010x}')
L.append('801366EC: 48000024 [was 40800024] ; bge -> b: no memory-interface protection above 24 MB')
L.append('80136718: 48000014 [was 41810014] ; bgt -> b: Config48MB instead of Config24MB')
for a, n in DRAW:
    assert rd(a) == 0x3CC00400, hex(a)
    L.append(f'{a:08X}: 38C00000 [was 3CC00400] ; lis r6, 0x400 -> li r6, 0: per-mesh materials ({n})')
(MOD / 'code').mkdir(exist_ok=True)
(MOD / 'code' / 'samurai.txt').write_text('\n'.join(L) + '\n')
print(f'code/samurai.txt: {len(L) - 1} patches, body {BODY:#x}, parts {PART:#x}')
