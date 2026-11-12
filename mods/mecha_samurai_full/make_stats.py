"""The mecha samurai's stats: his gun (Basic Gun's record, gun 0) and his charge attack (body 0's record).

  python -I mods/mecha_samurai_full/make_stats.py      -> code/stats.txt

Gun record: cr_g_GunData 0x801CA560 + gun x 0x28C. Field layout from CRBR-Parts-Expansion's gun files
(PartData/CRBR/Gun/*.asm), checked against Basic Gun's bytes in main.dol (docs/full_custom_robo.md, finding 2):
+0xA7 trail (0 none / 1 trail effect), +0xA8 / +0xAC / +0xB0 / +0xB4 trail RGBA (left, middle, right, unknown),
+0xB8 u32 trail duration, +0xD8 float trail width; shot blocks at +0xDC (ground) and +0x1B4 (air; found in main.dol, Basic Gun's air block: 5, 3, 1, 0, 10, 50): +0 startup,
+1 shots, +2 rounds per shot, +3 spread (0 vertical, 1 horizontal), +4 delay, +5 end lag, +6 s16 randomisation,
+0xA s16 first round's angle, +0xC s16 angle between rounds (65536 = 360 degrees; Triple Row Gun 10922 / 10922).
The samurai's twin-barrel cannon: two rounds side by side (+-8 degrees) per shot, a cyan / gold trail.

Body record: 0x801C6C44 + body x 0x134, charge attack (fn_80046A68, docs/battle_animations.md): +0xD8 homing,
+0xDA/DC/DE phase durations, +0xE0/E2/E4 speeds (x 0.1), +0xE6/E8/EA climb angles. His iai: a still draw, a fast
low lunge, a short glide (the motion's three 20-frame blocks are stretched over these durations).
"""
import struct
import sys
from pathlib import Path

G = Path(__file__).resolve().parents[2]
MOD = Path(__file__).resolve().parent
sys.path.insert(0, str(G / 'tools'))
import crmotion as c  # noqa: E402

dol = c.load_dol(None)
GUN, BODY = 0x801CA560 + 0 * 0x28C, 0x801C6C44 + 0 * 0x134
edits = {}      # address -> bytes


def put(a, fmt, *v):
    edits[a] = struct.pack('>' + fmt, *v)


put(GUN + 0xA7, 'B', 1)                                  # trail on
put(GUN + 0xA8, '4B', 0, 230, 255, 255)                  # left: cyan
put(GUN + 0xAC, '4B', 255, 205, 70, 255)                 # middle: gold
put(GUN + 0xB0, '4B', 0, 230, 255, 255)                  # right: cyan
put(GUN + 0xB4, '4B', 255, 255, 255, 255)
put(GUN + 0xB8, 'I', 42)                                 # trail duration: 60 stock; owner (Nov 11): "a little too long"
for blk in (0xDC, 0x1B4):
    put(GUN + blk + 2, 'BB', 2, 1)                       # two rounds per shot, side by side
    put(GUN + blk + 0xA, 'hh', 1456, 2912)               # +-8 degrees
put(BODY + 0xD8, 'h', 25)                                # homing
put(BODY + 0xDA, 'hhh', 12, 16, 28)                      # draw, lunge, glide (frames)
put(BODY + 0xE0, 'hhh', 0, 320, 40)                      # speeds (x 0.1)
put(BODY + 0xE6, 'hhh', 0, 0, -90)

words = {}
for a, b in edits.items():
    for i, x in enumerate(b):
        ad = a + i
        wa = ad & ~3
        words.setdefault(wa, bytearray(dol.read(wa, 4)))[ad - wa] = x
lines = ['; The mecha samurai\'s gun (gun 0) and charge attack (body 0) stats (make_stats.py; docs/full_custom_robo.md)']
for wa in sorted(words):
    old = struct.unpack('>I', dol.read(wa, 4))[0]
    new = struct.unpack('>I', bytes(words[wa]))[0]
    if new != old:
        lines.append(f'{wa:08X}: {new:08X} [was {old:08X}]')
(MOD / 'code' / 'stats.txt').write_text('\n'.join(lines) + '\n')
print(f'code/stats.txt: {len(lines) - 1} words')
