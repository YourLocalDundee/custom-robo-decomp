"""Custom Robo (GC) motion files (motion.DBN, the pieces of snr_motion.BIN) <-> JSON, with a byte-identical round trip.

Read from the game's own code (main.dol, USA):
  cr_MotionFile_Register 0x80074DCC   registers a file: u32 group count, u32 group offsets; per group u32 n, n entries
                                      (tracks, floats, counts: offsets from the group start), n u16 motion IDs (bit 15 set:
                                      78 tracks, clear: 69)
  cr_Motion_LoadInfo     0x80037A80   copies an entry to the playing object: +0 floats, +4 counts, +8 tracks
  fn_8001EB48 (sampler)               evaluates one track at the current frame: a track is `count` u16 key frames and
                                      `count` u16 key words; a key word is a 2-bit kind and a 14-bit index into the group's
                                      float pool; between two keys a cubic Hermite curve (time unit 1/30 s, tangents per
                                      second); at or before the first key, and exactly on a key, the key's value
  fn_80056104 (root)                  tracks 69-71 root translation, 72-74 root rotation (degrees), 75-77 a vector turned by
                                      the root matrix; tracks 0-2 the root point
  fn_80054238 (points, joints)        tracks 0, 9, 15, 24, 36, 48, 60 (x, y, z each): the 7 IK points the limbs are built
                                      to (24 <-> 36 and 48 <-> 60 swap when the robo is mirrored); for the 16 joints of the
                                      game skeleton table 0x801F9B00 with flag 2, three rotation tracks each (degrees, base
                                      track from the byte table 0x801F9D94); cr_Chara_PoseLimbs then builds the limbs
  The key word's kind (bits 14-15): 0 = value, in-tangent, out-tangent (3 floats); 1 = zero, flat; 2 = value, flat
  (1 float); 3 = value, one tangent for both sides (2 floats).

Usage:
  python -I tools/crmotion.py list <motion file> [--dol main.dol]          every motion: ID, game name, frames, tracks used
  python -I tools/crmotion.py export <motion file> <out.json> [--dol ...]  decoded keys (and the raw words, for exactness)
  python -I tools/crmotion.py import <in.json> <out file>                  rebuild the file; edited keys get new floats
  python -I tools/crmotion.py roundtrip <motion file or folder>...         file -> JSON -> file, compare bytes
  python -I tools/crmotion.py sample <motion file> <motion ID> <frame>     every channel's value at that frame, by role
"""
import json
import math
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEFAULT_DOL = ROOT / 'Custom Robo extracted' / 'sys' / 'main.dol'
NAMES_ADDR = 0x80206DB0   # cr_g_MotionNames: 3 string pointers per motion ID
INFO_ADDR = 0x80205C94    # cr_g_MotionInfo: pointer per motion ID; +4 -> u16 header, & 0xFFF = frame count
NUM_IDS = 1095
TIME_UNIT = struct.unpack('>f', struct.pack('>f', 1 / 30))[0]   # FLOAT_80450428
KINDS = {0: 'full', 1: 'zero', 2: 'flat', 3: 'smooth'}
KIND_BITS = {v: k for k, v in KINDS.items()}
WIDTH = {0: 3, 1: 0, 2: 1, 3: 2}

# track roles (fn_80056104, fn_80054238, tables 0x801F9B00 / 0x801F9D94 of main.dol)
POINT_TRACKS = [0, 9, 15, 24, 36, 48, 60]
JOINT_TRACKS = {0: 3, 1: 6, 3: 12, 5: 18, 6: 21, 9: 27, 10: 30, 11: 33, 14: 39, 15: 42, 16: 45, 19: 51, 20: 54, 21: 57,
                24: 63, 25: 66}   # game skeleton slot -> first of its three rotation tracks


def track_roles(ntracks=78):
    roles = {}
    for i, t in enumerate(POINT_TRACKS):
        for a in range(3):
            roles[t + a] = 'point%d.%s' % (i, 'xyz'[a])
    for slot, t in JOINT_TRACKS.items():
        for a in range(3):
            roles[t + a] = 'joint%d.rot%s' % (slot, 'xyz'[a])
    if ntracks == 78:
        for a in range(3):
            roles[69 + a] = 'root.pos%s' % 'xyz'[a]
            roles[72 + a] = 'root.rot%s' % 'xyz'[a]
            roles[75 + a] = 'root.vec%s' % 'xyz'[a]
    return [roles[k] for k in range(ntracks)]


def f32(x):
    return struct.unpack('>f', struct.pack('>f', x))[0]


def u32(d, o):
    return struct.unpack_from('>I', d, o)[0]


def align(n, a):
    return (n + a - 1) // a * a


# ---- the game's tables in main.dol (names, frame counts)

class Dol:
    def __init__(self, path):
        self.d = Path(path).read_bytes()
        d = self.d
        self.secs = list(zip(struct.unpack('>18I', d[0:72]), struct.unpack('>18I', d[72:144]),
                             struct.unpack('>18I', d[144:216])))

    def read(self, addr, n):
        for off, start, size in self.secs:
            if start <= addr < start + size:
                return self.d[off + addr - start:off + addr - start + n]
        return None

    def cstr(self, addr):
        b = self.read(addr, 128)
        return b[:b.index(0)].decode('ascii', 'replace') if b else ''

    def name(self, mid):
        mid &= 0x7FFF
        if mid >= NUM_IDS:
            return ''
        p = struct.unpack('>I', self.read(NAMES_ADDR + 12 * mid, 4))[0]
        return self.cstr(p) if p else ''

    def frames(self, mid):
        mid &= 0x7FFF
        if mid >= NUM_IDS:
            return None
        p = struct.unpack('>I', self.read(INFO_ADDR + 4 * mid, 4))[0]
        if not p:
            return None
        h = struct.unpack('>I', self.read(p + 4, 4))[0]
        return struct.unpack('>H', self.read(h, 2))[0] & 0xFFF if h else None


def load_dol(path):
    p = Path(path) if path else DEFAULT_DOL
    return Dol(p) if p.exists() else None


# ---- parse

def parse(d):
    """A motion file (u32 group count, the group offsets, the file length, zeros to 0x20) -> {'groups': [{'floats': [...], 'motions': [{'id', 'tracks': [None | {'times', 'words'}]}]}],
    'layout': what the writer needs to put the bytes back exactly}."""
    ng = u32(d, 0)
    goffs = [u32(d, 4 + 4 * i) for i in range(ng)]
    groups = []
    for gi, g in enumerate(goffs):
        n = u32(d, g)
        ents = [struct.unpack_from('>3I', d, g + 4 + 12 * i) for i in range(n)]
        ids = struct.unpack_from('>%dH' % n, d, g + 4 + 12 * n)
        if n == 0:   # snr_motion.BIN piece 24: one empty group
            groups.append({'floats': [], 'motions': []})
            continue
        fl_off = ents[0][1]
        cnt_start = min(e[2] for e in ents)
        nf = (cnt_start - fl_off) // 4
        floats = list(struct.unpack_from('>%df' % nf, d, g + fl_off))
        motions = []
        for e, mid in zip(ents, ids):
            ntr = 78 if mid & 0x8000 else 69
            counts = d[g + e[2]:g + e[2] + ntr]
            tracks = []
            for k in range(ntr):
                t, v = struct.unpack_from('>II', d, g + e[0] + 8 * k)
                c = counts[k]
                if c == 0:
                    if t or v:
                        raise ValueError('group %d motion %#x track %d: no keys but offsets %#x %#x' % (gi, mid, k, t, v))
                    tracks.append(None)
                    continue
                tracks.append({'times': list(struct.unpack_from('>%dH' % c, d, g + t)),
                               'words': list(struct.unpack_from('>%dH' % c, d, g + v))})
            motions.append({'id': mid, 'tracks': tracks})
        groups.append({'floats': floats, 'motions': motions})
    return {'groups': groups}


# ---- write (the layout the game's files use, checked by `roundtrip`)

def build_group(grp, base):
    """Bytes of one group placed at file offset `base` (the group is padded to a 0x20 boundary of the file)."""
    motions, floats = grp['motions'], grp['floats']
    n = len(motions)
    head = 4 + 12 * n + 2 * n
    fl_off = align(head, 4)
    cnt_off = fl_off + 4 * len(floats)
    cnt_offs, o = [], cnt_off
    for m in motions:
        cnt_offs.append(o)
        o += len(m['tracks'])
    o_cnt_end = o
    val_off = align(o, 4)   # the gap after the counts is filled with 0xFF
    # value lists in motion / track order, never shared
    val_offs, o = [], val_off
    for m in motions:
        row = []
        for tr in m['tracks']:
            if tr is None:
                row.append(0)
            else:
                row.append(o)
                o += 2 * len(tr['words'])
        val_offs.append(row)
    time_off = o
    # time lists in first-use order, shared: a list equal to the start of one already placed points into it
    tpos, tlist, o = {}, [], time_off
    for m in motions:
        for tr in m['tracks']:
            if tr is None:
                continue
            key = tuple(tr['times'])
            if key in tpos:
                continue
            start = o
            tlist.append(key)
            o += 2 * len(key)
            for k in range(1, len(key) + 1):
                tpos.setdefault(key[:k], start)
    trk_off = align(o, 4)
    trk_offs, o = [], trk_off
    for m in motions:
        trk_offs.append(o)
        o += 8 * len(m['tracks'])
    end = align(base + o, 0x20) - base
    out = bytearray(end)
    out[o_cnt_end:val_off] = bytes([0xFF]) * (val_off - o_cnt_end)
    struct.pack_into('>I', out, 0, n)
    for i in range(n):
        struct.pack_into('>3I', out, 4 + 12 * i, trk_offs[i], fl_off, cnt_offs[i])
    struct.pack_into('>%dH' % n, out, 4 + 12 * n, *[m['id'] for m in motions])
    struct.pack_into('>%df' % len(floats), out, fl_off, *floats)
    for i, m in enumerate(motions):
        out[cnt_offs[i]:cnt_offs[i] + len(m['tracks'])] = bytes(0 if t is None else len(t['words']) for t in m['tracks'])
        for k, tr in enumerate(m['tracks']):
            if tr is None:
                continue
            struct.pack_into('>%dH' % len(tr['words']), out, val_offs[i][k], *tr['words'])
            struct.pack_into('>II', out, trk_offs[i] + 8 * k, tpos[tuple(tr['times'])], val_offs[i][k])
    for key in tlist:
        struct.pack_into('>%dH' % len(key), out, tpos[key], *key)
    return bytes(out)


def build(motion):
    groups = motion['groups']
    head = align(8 + 4 * len(groups), 0x20)
    out = bytearray(head)
    struct.pack_into('>I', out, 0, len(groups))
    for i, g in enumerate(groups):
        struct.pack_into('>I', out, 4 + 4 * i, len(out))
        out += build_group(g, len(out))
    struct.pack_into('>I', out, 4 + 4 * len(groups), len(out))   # the file length follows the group offsets
    return bytes(out)


# ---- keys: decode a key word against the float pool, encode an edited key back

def decode_key(word, floats):
    kind = word >> 14
    i = word & 0x3FFF
    if kind == 1:
        return 'zero', 0.0, 0.0, 0.0
    if kind == 2:
        return 'flat', floats[i], 0.0, 0.0
    if kind == 3:
        return 'smooth', floats[i], floats[i + 1], floats[i + 1]
    return 'full', floats[i], floats[i + 1], floats[i + 2]


def key_floats(kind, v, tin, tout):
    return {'zero': [], 'flat': [v], 'smooth': [v, tin], 'full': [v, tin, tout]}[kind]


def encode_key(key, floats, pool):
    """key = {'frame', 'kind', 'value', 'in', 'out', 'word'?}: the stored word when it still decodes to the key's numbers,
    else a new word (the floats found in the pool, or appended)."""
    kind = key['kind']
    want = [f32(x) for x in key_floats(kind, key.get('value', 0.0), key.get('in', 0.0), key.get('out', 0.0))]
    w = key.get('word')
    if w is not None and KINDS[w >> 14] == kind and [f32(x) for x in key_floats(*decode_key(w, floats))] == want:
        return w
    if kind == 'zero':
        return 0x4000
    if kind == 'smooth' and f32(key.get('in', 0.0)) != f32(key.get('out', 0.0)):
        raise ValueError('a smooth key has one tangent: in and out must be equal (use kind "full")')
    t = tuple(want)
    if t not in pool:
        # overlap the pool's tail when it already ends with the start of these floats (the game's files pack this way)
        k = next((k for k in range(len(want) - 1, 0, -1) if [f32(x) for x in floats[-k:]] == want[:k]), 0)
        idx = len(floats) - k
        if idx + len(want) > 0x4000:
            raise ValueError('the group float pool is full (14-bit index)')
        floats.extend(want[k:])
        for start in range(max(0, idx - 2), len(floats)):   # every new run of 1-3 floats can be reused
            for w in (1, 2, 3):
                if start + w <= len(floats):
                    pool.setdefault(tuple(f32(x) for x in floats[start:start + w]), start)
    return (KIND_BITS[kind] << 14) | pool[t]


def to_json(motion, dol=None):
    out = {'format': 'custom-robo-motion/1', 'groups': []}
    for g in motion['groups']:
        fl = g['floats']
        jg = {'floats': fl, 'motions': []}
        for m in g['motions']:
            roles = track_roles(len(m['tracks']))
            jm = {'id': m['id'], 'name': dol.name(m['id']) if dol else '', 'frames': dol.frames(m['id']) if dol else None,
                  'tracks': []}
            for k, tr in enumerate(m['tracks']):
                if tr is None:
                    jm['tracks'].append(None)
                    continue
                keys = []
                for t, w in zip(tr['times'], tr['words']):
                    kind, v, tin, tout = decode_key(w, fl)
                    key = {'frame': t, 'kind': kind}
                    if kind != 'zero':
                        key['value'] = v
                    if kind in ('smooth', 'full'):
                        key['in'] = tin
                    if kind == 'full':
                        key['out'] = tout
                    key['word'] = w
                    keys.append(key)
                jm['tracks'].append({'role': roles[k], 'keys': keys})
            jg['motions'].append(jm)
        out['groups'].append(jg)
    return out


def from_json(j):
    groups = []
    for jg in j['groups']:
        floats = [f32(x) for x in jg['floats']]
        pool = {}
        for w in (2, 1, 3):   # the first place each run of floats occurs
            for i in range(len(floats) - w + 1):
                pool.setdefault(tuple(floats[i:i + w]), i)
        motions = []
        for jm in jg['motions']:
            tracks = []
            for tr in jm['tracks']:
                if tr is None or not tr['keys']:
                    tracks.append(None)
                    continue
                keys = tr['keys']
                times = [int(k['frame']) for k in keys]
                if times[0] != 0 or any(b < a for a, b in zip(times, times[1:])) or len(keys) > 255:
                    raise ValueError('motion %#x: key frames must start at 0, never go down, at most 255 keys' % jm['id'])
                tracks.append({'times': times, 'words': [encode_key(k, floats, pool) for k in keys]})
            if len(tracks) not in (69, 78) or (len(tracks) == 78) != bool(jm['id'] & 0x8000):
                raise ValueError('motion %#x: 78 tracks with ID bit 15 set, or 69 without' % jm['id'])
            motions.append({'id': jm['id'], 'tracks': tracks})
        groups.append({'floats': floats, 'motions': motions})
    return {'groups': groups}


# ---- sampling (fn_8001EB48)

def sample_track(tr, floats, frame):
    times, words = tr['times'], tr['words']
    frame = f32(frame)
    for i, t in enumerate(times):
        if frame <= t:
            if t != frame and i != 0:
                _, p1, m_in, _ = decode_key(words[i], floats)
                _, p0, _, m_out = decode_key(words[i - 1], floats)
                t0 = times[i - 1]
                dt = f32(TIME_UNIT * (t - t0))
                s = f32(f32(TIME_UNIT * (frame - t0)) / dt)
                u = f32(1.0 - s)
                return f32(p0 * u * u * (1.0 + 2.0 * s) + p1 * s * s * (3.0 - 2.0 * s)
                           + dt * (m_out * (s * u * u) + m_in * (-(s * s) * u)))
            return decode_key(words[i], floats)[1]
    # past the last key the game reads the word after the list; the files' motions end on a key, so hold the last value
    return decode_key(words[-1], floats)[1]


def sample(motion_entry, floats, frame):
    out = {}
    roles = track_roles(len(motion_entry['tracks']))
    for k, tr in enumerate(motion_entry['tracks']):
        out[roles[k]] = 0.0 if tr is None else sample_track(tr, floats, frame)
    return out


# ---- commands

def motion_files(paths):
    for p in map(Path, paths):
        if p.is_dir():
            yield from sorted(x for x in p.rglob('*') if x.is_file() and is_motion(x.read_bytes()))
        else:
            yield p


def is_motion(d):
    try:
        ng = u32(d, 0)
        if not 0 < ng < 16 or len(d) < 0x20:
            return False
        g = u32(d, 4)
        n = u32(d, g)
        return 0 <= n < 0x1000 and g + 4 + 14 * n <= len(d)
    except struct.error:
        return False


def cmd_list(path, dol):
    m = parse(Path(path).read_bytes())
    for gi, g in enumerate(m['groups']):
        print('group %d: %d motions, %d floats' % (gi, len(g['motions']), len(g['floats'])))
        for e in g['motions']:
            used = sum(1 for t in e['tracks'] if t)
            last = max((t['times'][-1] for t in e['tracks'] if t), default=0)
            print('  %#06x %-36s frames %-5s last key %-4d tracks %d/%d' % (
                e['id'], dol.name(e['id']) if dol else '', dol.frames(e['id']) if dol else '?', last, used,
                len(e['tracks'])))


def cmd_roundtrip(paths):
    bad = 0
    n = 0
    for p in motion_files(paths):
        d = p.read_bytes()
        j = json.loads(json.dumps(to_json(parse(d))))
        b = build(from_json(j))
        n += 1
        if b != d:
            bad += 1
            first = next((i for i in range(min(len(b), len(d))) if b[i] != d[i]), min(len(b), len(d)))
            print('DIFF %s (len %d vs %d, first difference at %#x)' % (p, len(b), len(d), first))
    print('%d files, %d byte-identical, %d differ' % (n, n - bad, bad))
    return bad


def find_motion(m, mid):
    for g in m['groups']:
        for e in g['motions']:
            if e['id'] & 0x7FFF == mid & 0x7FFF:
                return g, e
    raise SystemExit('motion %#x not in the file' % mid)


def main(argv):
    dolp = None
    if '--dol' in argv:
        i = argv.index('--dol')
        dolp = argv[i + 1]
        del argv[i:i + 2]
    if len(argv) < 2:
        raise SystemExit(__doc__)
    cmd = argv[1]
    if cmd == 'list':
        cmd_list(argv[2], load_dol(dolp))
    elif cmd == 'export':
        j = to_json(parse(Path(argv[2]).read_bytes()), load_dol(dolp))
        Path(argv[3]).write_text(json.dumps(j, indent=1))
        print('wrote', argv[3])
    elif cmd == 'import':
        b = build(from_json(json.loads(Path(argv[2]).read_text())))
        Path(argv[3]).write_bytes(b)
        print('wrote %s (%d bytes)' % (argv[3], len(b)))
    elif cmd == 'roundtrip':
        sys.exit(1 if cmd_roundtrip(argv[2:]) else 0)
    elif cmd == 'sample':
        m = parse(Path(argv[2]).read_bytes())
        g, e = find_motion(m, int(argv[3], 0))
        for role, v in sample(e, g['floats'], float(argv[4])).items():
            extra = '  (%.2f deg)' % v if 'rot' in role else ''
            print('%-14s %12.5f%s' % (role, v, extra))
    else:
        raise SystemExit(__doc__)


if __name__ == '__main__':
    main(sys.argv)
