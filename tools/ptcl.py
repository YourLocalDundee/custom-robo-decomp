"""Custom Robo particles: read, list and recolour the 8 particle banks in logo_read_us.dbn entry 0.

Entry 0 layout (big-endian): u32 0x20, then 8 banks x 4 section offsets (+ the end offset), from the entry start:
  section 0  ptcl: the generator list (HSD particle group, as in Melee): u16 0x43, u16 bank, u32 first generator
             number, u32 count, count x u32 offsets (0 = empty slot); each generator is a 0x3C-byte header (shape,
             texture group, life, kind flags, gravity, friction, velocity, radius, angle, random, size, 3 params)
             followed by a byte-code track (waits, colour / size / velocity changes, child spawns), ending in FD/FE/FF
  section 1  texg: the pictures (u32 count, offsets; each: image count, GX format, palette format, w, h, offsets)
  section 2  u32 list: generator numbers (not decoded further)
  section 3  a second small texg-like table (not decoded further)
Bank header numbers: section index 0..7 hold banks 0, 1, 2, 3, 4, 6, 7, 5 (bank 5 = the common effects, ids 0..12).

The colours live in the track: C0..CF = primary colour, D0..DF = environment colour; the low 4 bits say which of
R, G, B, A follow (1 byte each), after an "e" time (1 or 2 bytes). Recolouring rewrites those bytes in place, so
the entry keeps its size and nothing moves.

  python -I tools/ptcl.py list [bank]                      generators with their colour commands
  python -I tools/ptcl.py users <gen> [<gen> ...]          which effect files fire these generators
  python -I tools/ptcl.py recolour <in.dbn> <out.dbn> <plan.json>
      plan: {"generators": [4000, 4001, ...], "palette": "gold" | {"hue": deg, "sat": x, "val": x}}
"""
import colorsys
import json
import struct
import sys
from pathlib import Path

G = Path(__file__).resolve().parents[1]
CLEAN = G / 'Custom Robo extracted' / 'files' / 'logo_read_us.dbn'

# opcode -> parameter kinds (HSDLib's ParticleEncoding table; 0xB7 is "b", its table lists 0xB6 twice)
OPS = {0xA0: 'ef', 0xA1: '', 0xA2: 'f', 0xA3: 'f', 0xA4: 'bb', 0xA5: 'bb', 0xA6: 'ss', 0xA7: 'b', 0xA8: 'fff',
       0xA9: 'f', 0xAA: 'ss', 0xAB: 'f', 0xAC: 'eff', 0xAD: '', 0xAE: '', 0xAF: '', 0xB0: '', 0xB1: '', 0xB2: '',
       0xB3: 'ebbb', 0xB4: '', 0xB5: '', 0xB6: 'ef', 0xB7: 'b', 0xB8: 'bff', 0xB9: 'bb', 0xBA: 'bbbb', 0xBB: 'bbbb',
       0xBC: 'bb', 0xBD: 'ff', 0xBE: 'fff', 0xBF: 'b', 0xE0: 'bbbb', 0xE1: 'b', 0xE2: '', 0xE3: 'b', 0xE4: 'b',
       0xE5: 'b', 0xE6: '', 0xE7: '', 0xE8: 'f', 0xE9: 'r', 0xEA: 'em', 0xEB: 'em', 0xEC: 'bf', 0xED: 'ffb',
       0xEE: 'ffb', 0xEF: 'sb', 0xF0: 'sb', 0xF1: 'sm', 0xFA: 'b', 0xFB: '', 0xFC: '', 0xFD: '', 0xFE: '', 0xFF: ''}
for c in range(0xF2, 0xFA):
    OPS[c] = 'sm'


def bundle_entry0(dbn):
    n = struct.unpack_from('>I', dbn)[0]
    offs = struct.unpack_from('>%dI' % (n + 1), dbn, 4)
    return offs[0], offs[1]


def sections(e):
    o = struct.unpack_from('>34I', e)
    if o[0] != 0x20:
        raise SystemExit('not the particle entry')
    return [[(o[1 + b * 4 + k], o[2 + b * 4 + k]) for k in range(4)] for b in range(8)]


def decode_track(t, pos, end):
    """-> list of (offset, opcode, [(kind, offset, value)]) up to and including the terminator"""
    out = []
    while pos < end:
        at, c = pos, t[pos]
        pos += 1
        if c < 0x80:
            if (c & 0xC0) == 0x40:
                out.append((at, 0x40, [('b', pos, t[pos])])); pos += 1
            else:
                if c & 0x20:
                    pos += 1
                out.append((at, 0x00, []))
            continue
        ps = []

        def e():
            nonlocal pos
            v = t[pos]; p0 = pos; pos += 1
            if v & 0x80:
                v = ((v & 0x7F) << 8) | t[pos]; pos += 1
            ps.append(('e', p0, v))
        if c >= 0xC0 and c < 0xE0:            # colour: time, then the bytes the low 4 bits select (R, G, B, A)
            e()
            for j in range(4):
                if (c >> j) & 1:
                    ps.append(('rgba'[j], pos, t[pos])); pos += 1
        elif 0x80 <= c < 0xA0:                 # position / velocity: up to 3 floats
            for j in range(3):
                if (c >> j) & 1:
                    ps.append(('f', pos, struct.unpack_from('>f', t, pos)[0])); pos += 4
        else:
            kinds = OPS.get(c)
            if kinds is None:
                raise ValueError('unknown op %02X at %#x' % (c, at))
            for k in kinds:
                if k == 'e':
                    e()
                elif k == 'b':
                    ps.append(('b', pos, t[pos])); pos += 1
                elif k == 's':
                    pos += 2
                elif k == 'f':
                    pos += 4
                elif k == 'r':
                    beh = t[pos]; pos += 2
                    pos += bin(beh & 0xF).count('1')
                elif k == 'm':
                    beh = t[pos]; pos += 1
                    pos += (beh & 1) + ((beh >> 3) & 1)
        out.append((at, c, ps))
        if c in (0xFD, 0xFE, 0xFF):
            break
    return out, pos


def generators(e):
    """-> {(bank, generator number): (section, bank, start, end, decoded track, stop)}; bank 5's numbers (0..12)
    repeat bank 0's, so the bank is part of the key (find() looks a number up the way the effects name it)"""
    gens = {}
    for b, secs in enumerate(sections(e)):
        a, z = secs[0]
        ver, bank, first, count = struct.unpack_from('>HHII', e, a)
        offs = struct.unpack_from('>%dI' % count, e, a + 12)
        starts = sorted(set(x for x in offs if x))
        for i, x in enumerate(offs):
            if not x:
                continue
            nxt = next((s for s in starts if s > x), z - a)
            g0, g1 = a + x, a + nxt
            track, stop = decode_track(e, g0 + 0x3C, g1)
            gens[(bank, first + i)] = (b, bank, g0, g1, track, stop)
    return gens


def find(gens, g, bank=None):
    """a generator by number (and bank when given; without it bank 5 is only found for numbers bank 0 lacks)"""
    if bank is not None:
        return gens.get((bank, g))
    hits = [k for k in gens if k[1] == g]
    hits.sort(key=lambda k: k[0] == 5)
    return gens[hits[0]] if hits else None


def colours(track):
    for at, c, ps in track:
        if 0xC0 <= c < 0xE0:
            yield at, ('prim' if c < 0xD0 else 'env'), {k: (p, v) for k, p, v in ps if k in 'rgba'}


def cmd_list(bank=None):
    e = CLEAN.read_bytes()
    a, z = bundle_entry0(e)
    e = e[a:z]
    gens = generators(e)
    bad = 0
    for (bk, g), (sec, bank_id, g0, g1, track, stop) in sorted(gens.items()):
        if bank is not None and bank_id != bank:
            continue
        ended = track and track[-1][1] in (0xFD, 0xFE, 0xFF)
        bad += not ended
        cs = ['%s%s' % (kind, ''.join('%s%02x' % (k, v) for k, (p, v) in sorted(ch.items(), key=lambda x: 'rgba'.index(x[0]))))
              for at, kind, ch in colours(track)]
        print('%5d bank %d  %4d bytes  tex %3d  %s%s' % (g, bank_id, g1 - g0, struct.unpack_from('>h', e, g0 + 2)[0],
              ' '.join(cs[:6]) + (' ...' if len(cs) > 6 else ''), '' if ended else '  (no terminator)'))
    print('%d generators, %d without a terminator' % (len(gens) if bank is None else 0, bad), file=sys.stderr)


def cmd_users(want):
    sys.path.insert(0, str(G / 'tools'))
    import scene  # noqa: E402
    want = set(want)
    for p in sorted((G / 'models_dat').glob('*_effect/*.dat')):
        found = set(scene.particles_fired(p)) & want if hasattr(scene, 'particles_fired') else set()
        if found:
            print(p.name, sorted(found))


PALETTES = {
    # target hue (degrees), saturation multiplier, value multiplier; the brightness of each colour is kept
    'gold': (44, 1.15, 1.0),
    'crimson': (354, 1.2, 0.95),
}


def shift(rgb, hue, sat, val):
    h, s, v = colorsys.rgb_to_hsv(*(x / 255 for x in rgb))
    if s < 0.08:                               # whites / greys stay (hot cores), tinted a little toward the hue
        s = 0.18 if v > 0.5 else s
    r, g, b = colorsys.hsv_to_rgb(hue / 360, min(1, s * sat), min(1, v * val))
    return [round(x * 255) for x in (r, g, b)]


def palette_args(pal):
    return PALETTES[pal] if isinstance(pal, str) else (pal['hue'], pal.get('sat', 1), pal.get('val', 1))


def recolour_bytes_file(src, dst, plan):
    """plan: {"generators": [...], "palette": name or {...}} or {"groups": [{"generators": [...], "palette": ...}]};
    the env (glow) colours are recoloured too unless they are black. Returns the number of colour commands changed."""
    plan = json.loads(Path(plan).read_text())
    groups = plan.get('groups') or ([plan] if plan.get('generators') else [])
    d = bytearray(Path(src).read_bytes())
    a, z = bundle_entry0(d)
    e = bytearray(d[a:z])
    gens = generators(bytes(e))
    n = 0
    for grp in groups:
        hue, sat, val = palette_args(grp['palette'])
        for g in grp['generators']:
            bank = None
            if isinstance(g, str):                 # "5:3" = bank 5, generator 3
                bank, g = (int(x) for x in g.split(':'))
            gen = find(gens, g, bank)
            if gen is None:
                raise SystemExit(f'no generator {g}')
            for at, kind, ch in colours(gen[4]):
                if not all(k in ch for k in 'rgb'):
                    continue                   # partial colour changes (alpha only, one channel) are left alone
                old = [ch[k][1] for k in 'rgb']
                if max(old) < 8:
                    continue                   # black (no glow) stays black
                for k, v in zip('rgb', shift(old, hue, sat, val)):
                    e[ch[k][0]] = v
                n += 1
    # "tune" (Nov 12, 2026): generator header fields scaled in place (HSD_ParticleGenerator, HSDRaw
    # Common/HSD_ParticleGroup.cs): {"3002": {"size": 1.5, "life": 1.3, "speed": 1.25, "angle": 1.4, "random": 1.3}}
    FIELDS = {'life': (0x06, '>h'), 'speed': (0x1C, '>f'), 'angle': (0x24, '>f'), 'random': (0x28, '>f'),
              'size': (0x2C, '>f'), 'friction': (0x10, '>f')}
    for g, mul in (plan.get('tune') or {}).items():
        bank = None
        if isinstance(g, str) and ':' in g:
            bank, g = (int(x) for x in g.split(':'))
        gen = find(gens, int(g), bank)
        if gen is None:
            raise SystemExit(f'no generator {g}')
        g0 = gen[2]
        for k, m in mul.items():
            off, fmt = FIELDS[k]
            v = struct.unpack_from(fmt, e, g0 + off)[0]
            nv = max(1, min(32767, int(round(v * m)))) if fmt == '>h' else v * m
            struct.pack_into(fmt, e, g0 + off, nv)
            n += 1
    d[a:z] = e
    Path(dst).write_bytes(bytes(d))
    return n


def cmd_recolour(src, dst, plan):
    n = recolour_bytes_file(src, dst, plan)
    print(f'recoloured {n} colour commands -> {dst}')


if __name__ == '__main__':
    cmd = sys.argv[1] if len(sys.argv) > 1 else ''
    if cmd == 'list':
        cmd_list(int(sys.argv[2]) if len(sys.argv) > 2 else None)
    elif cmd == 'users':
        cmd_users([int(x) for x in sys.argv[2:]])
    elif cmd == 'recolour':
        cmd_recolour(*sys.argv[2:5])
    else:
        print(__doc__)
