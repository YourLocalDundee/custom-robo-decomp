"""scene.py: stages and effects (projectiles, blasts, charge attacks) by name: list, inspect, export, import.

A front end over HSDRawViewer (tools/HSDLib: --export / --import of the standard HSD .dat files, Windows), tools/
scene_data.py (the scene_data structures) and tools/stage_collision.py (the collision entries).
docs/SCENE_GUIDE.md explains every format and what the game does with it.

  python -I tools/scene.py list stages            ids, names, entries, scene models, collision groups
  python -I tools/scene.py list effects [--kind gun]   per effect entry: its two pieces (models / emitters)
  python -I tools/scene.py info stage 0           the scene (models, animations, camera, lights, fog) + collision
  python -I tools/scene.py info effect gun 12     both pieces of gun_effect entry 12 (gun 12's shots)
  python -I tools/scene.py export stage 0 [--out DIR]
      DIR/stage_000/: scene_data_<n>.glb per scene model (HSDRawViewer; Blender: File > Import > glTF) and
      stage_000_collision.obj (+ .mtl, .json)
  python -I tools/scene.py export effect gun 12 [--piece 0|1]
  python -I tools/scene.py import stage 0 --model 3 EDITED.glb --mod MYMOD
      HSDRawViewer --import of that scene model into mods/MYMOD/models/bg_data/bg_data_000.dat (the mod's file
      is the template when it exists, so several models chain); then python -I tools/make_mod.py mods/MYMOD
  python -I tools/scene.py import effect gun 12 --piece 0 --model 1 EDITED.glb --mod MYMOD
  python -I tools/scene.py import collision 0 EDITED.obj --mod MYMOD     (tools/stage_collision.py import)
"""
import argparse
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import scene_data       # noqa: E402
import stage_collision  # noqa: E402

R = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EXE = os.path.join(R, 'tools', 'HSDLib', 'HSDRawViewer', 'bin', 'Release', 'net8.0-windows7.0', 'HSDRawViewer.exe')
KINDS = {'gun': 'gun_effect', 'bomb': 'bomb_effect', 'pod': 'pod_effect', 'attack': 'attack_effect', 'charge': 'attack_effect'}


def stages():
    """the stage table of docs/stage_tables.md (id, menu slot, US name)"""
    out = []
    for line in open(os.path.join(R, 'docs', 'stage_tables.md'), encoding='utf-8'):
        p = [x.strip() for x in line.split('|')]
        if len(p) > 6 and p[1].isdigit() and re.fullmatch(r'0x[0-9A-Fa-f]{2}', p[2] or ''):
            out.append((int(p[1]), p[3], p[4]))
    return out


def stage_dat(s, mod=None):
    if mod:
        p = os.path.join(R, 'mods', mod, 'models', 'bg_data', 'bg_data_%03d.dat' % (2 * s))
        if os.path.exists(p):
            return p
    return os.path.join(R, 'models_dat', 'bg_data', 'bg_data_%03d.dat' % (2 * s))


def effect_dat(cat, n, piece, mod=None):
    name = '%s_%03d_%d.dat' % (cat, n, piece)
    if mod:
        p = os.path.join(R, 'mods', mod, 'models', cat, name)
        if os.path.exists(p):
            return p
    return os.path.join(R, 'models_dat', cat, name)


def need(p):
    if not os.path.exists(p):
        raise SystemExit('%s not found: run the one-time build (tools/from_scratch) first' % p)
    return p


def hsd(*args):
    if not os.path.exists(EXE):
        raise SystemExit('HSDRawViewer is not built (tools/setup.ps1) or this is not Windows; '
                         'models_export/ has the clean exports already')
    r = subprocess.run([EXE] + list(args), capture_output=True, text=True)
    sys.stdout.write(r.stdout)
    if r.returncode != 0:
        sys.stderr.write(r.stderr)
        raise SystemExit('HSDRawViewer %s failed' % args[0])


def cmd_list(a):
    if a.what == 'stages':
        print('id  menu  name                       model  collision  scene models  collision groups')
        for sid, menu, name in stages():
            m = stage_dat(sid)
            nm = ng = '-'
            if os.path.exists(m):
                d = scene_data.Dat(m)
                so = [o for n, o in d.roots if n == 'scene_data']
                if so:
                    nm = len(scene_data.scene(d, so[0])['models'])
            c = stage_collision.entry_path(sid)
            if os.path.exists(c):
                b = open(c, 'rb').read()
                if stage_collision.is_collision(b):
                    ng = len(stage_collision.parse(b)['groups'])
            print('%-3d %-5s %-26s %03d    %03d        %-13s %s' % (sid, menu, name, 2 * sid, 2 * sid + 1, nm, ng))
    else:
        cats = [KINDS[a.kind]] if a.kind else ['gun_effect', 'bomb_effect', 'pod_effect', 'attack_effect']
        for cat in cats:
            folder = os.path.join(R, 'models_dat', cat)
            n = len({f[len(cat) + 1:len(cat) + 4] for f in os.listdir(folder) if f.endswith('.dat')}) if os.path.isdir(folder) else 0
            print('== %s: %d entries (entry = the %s number)' % (cat, n, 'body' if cat == 'attack_effect' else cat.split('_')[0]))
            for i in range(n):
                row = []
                for piece in (0, 1):
                    p = effect_dat(cat, i, piece)
                    if not os.path.exists(p):
                        row.append('-')
                        continue
                    d = scene_data.Dat(p)
                    so = [o for nn, o in d.roots if nn == 'scene_data']
                    sc = scene_data.scene(d, so[0]) if so else None
                    if sc:
                        ms = sc['models']
                        ptcl = sum(1 for m in ms for x in m['joint_anim'] if 'type40' in x[2])
                        row.append('%d models / %d meshes / %d particle tracks' % (len(ms), sum(m['dobjs'] for m in ms), ptcl))
                print('  %03d  piece 0: %-40s piece 1: %s' % (i, row[0], row[1] if len(row) > 1 else '-'))


def cmd_info(a):
    if a.what == 'stage':
        scene_data.info(need(stage_dat(a.n, a.mod)))
        if os.path.exists(stage_collision.entry_path(a.n)):
            stage_collision.cmd_info(a.n)
    else:
        cat = KINDS[a.kind]
        for piece in (0, 1):
            scene_data.info(need(effect_dat(cat, a.n, piece, a.mod)))


def cmd_export(a):
    out = a.out or os.path.join(R, 'build', 'scene')
    if a.what == 'stage':
        dst = os.path.join(out, 'stage_%03d' % a.n)
        os.makedirs(dst, exist_ok=True)
        hsd('--export', dst, 'glb', need(stage_dat(a.n, a.mod)))
        print(stage_collision.export(a.n, dst))
    else:
        cat = KINDS[a.kind]
        dst = os.path.join(out, '%s_%03d' % (cat, a.n))
        os.makedirs(dst, exist_ok=True)
        for piece in ([a.piece] if a.piece is not None else [0, 1]):
            hsd('--export', dst, 'glb', need(effect_dat(cat, a.n, piece, a.mod)))
    print('exported to', dst)


def cmd_import(a):
    if not a.mod:
        raise SystemExit('import needs --mod NAME')
    if a.what == 'collision':
        out = os.path.join(R, 'mods', a.mod, 'models', 'bg_data', 'bg_data_%03d.dat' % (2 * a.n + 1))
        os.makedirs(os.path.dirname(out), exist_ok=True)
        tmpl = out if os.path.exists(out) else None
        stage_collision.import_obj(a.n, a.file, out, tmpl)
    else:
        if a.model is None:
            raise SystemExit('give --model N (the scene_data_<N>.glb the edit came from)')
        if a.what == 'stage':
            src = need(stage_dat(a.n, a.mod))
            out = os.path.join(R, 'mods', a.mod, 'models', 'bg_data', 'bg_data_%03d.dat' % (2 * a.n))
        else:
            cat = KINDS[a.kind]
            piece = a.piece if a.piece is not None else 0
            src = need(effect_dat(cat, a.n, piece, a.mod))
            out = os.path.join(R, 'mods', a.mod, 'models', cat, '%s_%03d_%d.dat' % (cat, a.n, piece))
        os.makedirs(os.path.dirname(out), exist_ok=True)
        tmp = out + '.tmp'
        hsd('--import', src, a.file, tmp, '--model', str(a.model))
        os.replace(tmp, out)
        print('wrote', out)
    mj = os.path.join(R, 'mods', a.mod, 'mod.json')
    if not os.path.exists(mj):
        open(mj, 'w', newline='\n').write('{"name": "%s", "author": "", "description": "scene edit"}\n' % a.mod)
    print('next: python -I tools/make_mod.py mods/%s' % a.mod)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('list')
    p.add_argument('what', choices=['stages', 'effects'])
    p.add_argument('--kind', choices=sorted(KINDS))
    for name in ('info', 'export', 'import'):
        p = sub.add_parser(name)
        p.add_argument('what', choices=['stage', 'effect'] + (['collision'] if name == 'import' else []))
        p.add_argument('rest', nargs='+', help='stage / collision: N [FILE]; effect: KIND N [FILE]')
        p.add_argument('--mod')
        p.add_argument('--piece', type=int, choices=[0, 1])
        if name == 'export':
            p.add_argument('--out')
        if name == 'import':
            p.add_argument('--model', type=int)
    a = ap.parse_args()
    sys.stdout.reconfigure(errors='replace')
    if a.cmd != 'list':
        r = list(a.rest)
        a.kind = None
        if a.what == 'effect':
            if r[0] not in KINDS:
                raise SystemExit('effect kind: %s' % ', '.join(sorted(KINDS)))
            a.kind = r.pop(0)
        a.n = int(r.pop(0))
        a.file = r.pop(0) if r else None
        if a.cmd == 'import' and not a.file:
            raise SystemExit('import needs the edited file')
    {'list': cmd_list, 'info': cmd_info, 'export': cmd_export, 'import': cmd_import}[a.cmd](a)


if __name__ == '__main__':
    main()
