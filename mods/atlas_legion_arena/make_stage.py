"""Legion Arena: Basic Arena (stage 0, bg_data entries 000 / 001) rebuilt as a Roman amphitheatre.

  python -I mods/atlas_legion_arena/make_stage.py [--skip-blender]

1. tools/blender/legion_arena.py builds the new scene models (needs Blender; the CC0 photo materials are downloaded
   from ambientCG into build/legion/cc0 when missing) -> build/legion/arena/scene_data_<n>.glb
2. HSDRawViewer --import of each model over Basic Arena, chained (models 0, 1, 5-9 --replace; 2-4 recoloured; new
   textured materials copy model 0's unlit "VERTEX, TEX0" scheme: --tex-model 0)
3. the scene itself (this script, on the .dat bytes): warm sunset lights, a fog (its colour is the background; the
   distance haze starts at 60), and a looping joint animation on model 9 (the east barricade slides 2.4 toward the
   centre and back, 8 seconds)
4. the collision (entry 001): group 9 gets model flag 8 (moving: the game moves its quads with model 9 every frame,
   fn_800730F8); everything else as Basic Arena's
Writes mods/atlas_legion_arena/models/bg_data/bg_data_000.dat and bg_data_001.dat.
"""
import json
import math
import os
import struct
import subprocess
import sys
import urllib.request
import zipfile
from pathlib import Path

G = Path(__file__).resolve().parents[2]
MOD = Path(__file__).resolve().parent
sys.path.insert(0, str(G / 'tools'))
import scene_data          # noqa: E402
import stage_collision     # noqa: E402

BUILD = G / 'build' / 'legion'
EXE = G / 'tools' / 'HSDLib' / 'HSDRawViewer' / 'bin' / 'Release' / 'net8.0-windows7.0' / 'HSDRawViewer.exe'
BLENDER = os.environ.get('BLENDER') or r'C:\Program Files\Blender Foundation\Blender 5.2\blender.exe'
CC0 = ['Ground080', 'Travertine009', 'PavingStones151', 'Marble021', 'Planks037A', 'Fabric085', 'Bricks076C']
OUT_DIR = MOD / 'models' / 'bg_data'


def run(args, **kw):
    r = subprocess.run([str(a) for a in args], capture_output=True, text=True, **kw)
    if r.returncode != 0:
        sys.stdout.write(r.stdout[-4000:])
        sys.stderr.write(r.stderr[-4000:])
        raise SystemExit('failed: %s' % args[0])
    return r.stdout


def fetch_cc0():
    d = BUILD / 'cc0'
    d.mkdir(parents=True, exist_ok=True)
    for a in CC0:
        if (d / a / f'{a}_1K-JPG_Color.jpg').exists():
            continue
        z = d / f'{a}.zip'
        print('downloading', a, '(ambientCG, CC0)')
        req = urllib.request.Request(f'https://ambientcg.com/get?file={a}_1K-JPG.zip', headers={'User-Agent': 'stage-build'})
        z.write_bytes(urllib.request.urlopen(req).read())
        with zipfile.ZipFile(z) as f:
            f.extractall(d / a)


# ---------------------------------------------------------------- a minimal HSD .dat writer (append structs)

class DatFile:
    def __init__(self, path):
        b = Path(path).read_bytes()
        _, ds, nrel, nroot, nref = struct.unpack_from('>5I', b, 0)
        self.data = bytearray(b[0x20:0x20 + ds])
        p = 0x20 + ds
        self.relocs = set(struct.unpack_from('>%dI' % nrel, b, p))
        p += 4 * nrel
        roots = [struct.unpack_from('>II', b, p + 8 * i) for i in range(nroot)]
        p += 8 * nroot
        refs = [struct.unpack_from('>II', b, p + 8 * i) for i in range(nref)]
        p += 8 * nref

        def name(o):
            e = b.index(b'\0', p + o)
            return b[p + o:e]
        self.roots = [(off, name(n)) for off, n in roots]
        self.refs = [(off, name(n)) for off, n in refs]

    def u32(self, o):
        return struct.unpack_from('>I', self.data, o)[0]

    def ptr(self, o):
        return self.u32(o) if o in self.relocs else None

    def add(self, blob, align=0x20):
        while len(self.data) % align:
            self.data.append(0)
        o = len(self.data)
        self.data += blob
        return o

    def set_ptr(self, at, target):
        struct.pack_into('>I', self.data, at, target)
        self.relocs.add(at)

    def write(self, path):
        while len(self.data) % 4:
            self.data.append(0)
        strings = b''
        rt = []
        for off, nm in self.roots:
            rt.append((off, len(strings)))
            strings += nm + b'\0'
        ft = []
        for off, nm in self.refs:
            ft.append((off, len(strings)))
            strings += nm + b'\0'
        rel = sorted(self.relocs)
        body = bytes(self.data) + struct.pack('>%dI' % len(rel), *rel)
        body += b''.join(struct.pack('>II', *x) for x in rt) + b''.join(struct.pack('>II', *x) for x in ft) + strings
        hdr = struct.pack('>5I', 0x20 + len(body), len(self.data), len(rel), len(rt), len(ft)) + bytes(12)
        Path(path).write_bytes(hdr + body)


def pack_varint(v):
    out = bytearray()
    while v > 0x7F:
        out.append((v & 0x7F) | 0x80)
        v >>= 7
    out.append(v)
    return bytes(out)


def fobj_linear(values_every, step):
    """a FObj key stream: LIN keys of float values (little-endian, as HSD stores them), one every `step` frames"""
    n = len(values_every)
    out = bytearray(pack_varint(((n - 1) << 4) | 2))     # opcode LIN (2), n keys
    for i, v in enumerate(values_every):
        out += struct.pack('<f', v)
        out += pack_varint(step if i < n - 1 else 0)
    return bytes(out)


def scene_edits(dat_path, out_path):
    d = DatFile(dat_path)
    so = next(off for off, nm in d.roots if nm == b'scene_data')
    # lights: warm sunset ambient and sun (the stage's own two light descs: ambient, then the infinite light)
    larr = d.ptr(so + 8)
    k = 0
    while larr is not None and d.ptr(larr + 4 * k) is not None:
        entry = d.ptr(larr + 4 * k)
        lo = d.ptr(entry)
        if lo is not None:
            kind = struct.unpack_from('>H', d.data, lo + 8)[0] & 3
            col = {0: (120, 100, 92), 1: (255, 226, 186)}.get(kind)
            if col:
                d.data[lo + 0xC:lo + 0xF] = bytes(col)
        k += 1
    # fog: FogDesc (type 2 = linear, no adj, start, end, colour) + the {FogDesc, AObj} pair at scene +0xC
    fog = d.add(struct.pack('>IIff4BI', 2, 0, 60.0, 420.0, 232, 168, 120, 255, 0))
    pair = d.add(struct.pack('>II', 0, 0))
    d.set_ptr(pair, fog)
    d.set_ptr(so + 0xC, pair)
    # model 9: a looping joint animation on its root joint (translation x: 0 -> -2.4 -> 0 over 240 frames)
    jds = []
    a = d.ptr(so)
    i = 0
    while d.ptr(a + 4 * i) is not None:
        jds.append(d.ptr(a + 4 * i))
        i += 1
    jd = jds[9]
    if d.ptr(jd + 4) is not None:
        raise SystemExit('model 9 already has a joint animation')
    END, STEP = 240, 10
    vals = [-1.2 + 1.2 * math.cos(2 * math.pi * f / END) for f in range(0, END + 1, STEP)]
    keys = d.add(fobj_linear(vals, STEP), 4)
    klen = len(fobj_linear(vals, STEP))
    fo = d.add(struct.pack('>IIfBBBBI', 0, klen, 0.0, 5, 0, 0, 0, 0))    # next, length, start, type 5 = trans_x
    d.set_ptr(fo + 0x10, keys)
    ao = d.add(struct.pack('>IfII', 0x20000000, float(END), 0, 0))        # loop
    d.set_ptr(ao + 8, fo)
    aj = d.add(struct.pack('>5I', 0, 0, 0, 0, 0))
    d.set_ptr(aj + 8, ao)
    arr = d.add(struct.pack('>II', 0, 0))
    d.set_ptr(arr, aj)
    d.set_ptr(jd + 4, arr)
    d.write(out_path)
    return Path(out_path).stat().st_size


def main():
    BUILD.mkdir(parents=True, exist_ok=True)
    arena = BUILD / 'arena'
    coll_src = stage_collision.entry_path(0)
    col = stage_collision.parse(open(coll_src, 'rb').read())
    cj = BUILD / 'collision_000.json'
    cj.write_text(json.dumps({'groups': [{'flags': g['flags'], 'quads': [{'corners': q['corners'], 'normal': q['normal']} for q in g['quads']]}
                                         for g in col['groups']]}))
    if '--skip-blender' not in sys.argv:
        fetch_cc0()
        print(run([BLENDER, '-b', '--factory-startup', '--python-exit-code', '1', '--python', G / 'tools' / 'blender' / 'legion_arena.py', '--',
                   G / 'models_export' / 'bg_data' / 'bg_data_000', arena, BUILD / 'cc0', cj]).count('\n'), 'lines of Blender output')
    cur = G / 'models_dat' / 'bg_data' / 'bg_data_000.dat'
    for m in range(10):
        nxt = BUILD / f'chain_{m}.dat'
        args = [EXE, '--import', cur, arena / f'scene_data_{m}.glb', nxt, '--model', m]
        if m not in (2, 3, 4):
            args.append('--replace')
        if m != 0:
            args += ['--tex-model', '0']
        args.append('--relayout')   # a fresh layout: the bytes of replaced data go
        out = run(args)
        print('model %d: %s' % (m, out.strip().splitlines()[-1][:220]))
        cur = nxt
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    size = scene_edits(cur, OUT_DIR / 'bg_data_000.dat')
    # the load region is 0x100000 (docs/stage_tables.md "Limits for a modded stage"); 10 retail stages are past the
    # 0xA0000 caveat there, the largest is 0xEC220
    print('bg_data_000.dat: %d bytes (0x%X; the load region is 0x100000)' % (size, size))
    if size > 0xF0000:
        raise SystemExit('too big for the stage load region')
    # collision: model 9 moves
    col['groups'][9]['flags'] = 8
    for g in col['groups']:
        g['keep_header'] = False
        for q in g['quads']:
            q['keep'] = True
    (OUT_DIR / 'bg_data_001.dat').write_bytes(stage_collision.build(col['groups'], col['waypoints'], col['has_c']))
    chk = scene_data.Dat(str(OUT_DIR / 'bg_data_000.dat'))
    sc = scene_data.scene(chk, [o for n, o in chk.roots if n == 'scene_data'][0])
    print('check: %d models, model 9 joint anims %d, fog %s, lights %s' % (len(sc['models']), sc['models'][9]['joint_anims'], sc['fog'], [l['color'] for l in sc['lights']]))


if __name__ == '__main__':
    main()
