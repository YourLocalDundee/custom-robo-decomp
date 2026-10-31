"""Turn a mod folder into a playable ISO and a shareable patch (no game data inside). See docs/MODDING.md.

usage: python -I tools/make_mod.py <mod dir> [<clean.iso>]
   default ISO: "Custom Robo (USA).iso" next to this repo's tools folder's parent, else pass it.

A mod folder:
  mod.json                       {"name": "...", "author": "...", "description": "..."}   (optional)
  models/<category>/<name>.dat   edited models, named exactly as in models_dat/ (e.g. chara/chara_005.dat);
                                 robo/weapon/story-character models go back through crmodel.from_dat with the
                                 original entry as template; stage/object models replace the entry as-is
  files/<disc path>              any disc file, replaced as-is (e.g. files/font.tpl)
  code/*.txt                     main.dol patches, one per line:  80012345: 38600001 [was 38600000] ; comment
                                 (addresses are RAM addresses; "was" is checked against the clean DOL)
Output: build/mods/<name>/<name>.iso (play it in Dolphin) and <mod dir>/<name>.crpatch (share it).
"""
import json, os, re, shutil, struct, sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))  # python -I drops the script dir
import crmodel, iso_patch, robo_joints, sfd  # noqa: E402

G = Path(__file__).resolve().parent.parent
DISC = G / 'Custom Robo extracted'
# models_dat category -> (archive path under files/, kind)
ARCHIVES = {
    'chara': ('chara.BIN', 'cr'), 'weapon_gun': ('weapon_gun.BIN', 'cr'), 'weapon_bom': ('weapon_bom.BIN', 'cr'),
    'weapon_pod': ('weapon_pod.BIN', 'cr'), 'weapon_leg': ('weapon_leg.BIN', 'cr'),
    'snr_chara': ('scenario_us/snr_chara.BIN', 'cr'),
    'bg_data': ('bg_data.BIN', 'std'), 'leg_effect': ('leg_effect.BIN', 'std'), 'snr_obj': ('scenario_us/snr_obj.BIN', 'std'),
}
PATCH_LINE = re.compile(r'^\s*([0-9A-Fa-f]{8})\s*:\s*([0-9A-Fa-f\s]+?)\s*(?:\[\s*was\s+([0-9A-Fa-f\s]+?)\s*\])?\s*(?:;.*)?$')


def dol_offset(dol, addr):
    """RAM address -> file offset in main.dol (7 text + 11 data sections)."""
    offs = struct.unpack_from('>18I', dol, 0); addrs = struct.unpack_from('>18I', dol, 0x48)
    sizes = struct.unpack_from('>18I', dol, 0x90)
    for o, a, s in zip(offs, addrs, sizes):
        if s and a <= addr < a + s: return o + addr - a
    sys.exit(f'{addr:08X} is not inside main.dol')


FILE_DESCS, ARCHIVE_SLOTS = 0x801F1200, 0x2B  # cr_g_FileDescs (symbols/cr_globals.txt)


def sync_entry_table(dol, arch, old_archive, new_archive):
    """main.dol keeps its own copy of every packed archive's entry table (count, 0, then (offset, size) per
    entry), which the game reads instead of the archive's header (cr_Archive_OpenEntry / GetEntrySize).
    Rewrite that copy for the repacked archive. Returns True if it changed. Found Oct 27, 2026: a mod that
    changed an entry's size crashed the game until this table was updated."""
    u = lambda a: struct.unpack_from('>I', dol, dol_offset(dol, a))[0]
    old_n, new_n = struct.unpack_from('>I', old_archive, 4)[0], struct.unpack_from('>I', new_archive, 4)[0]
    if old_n != new_n: sys.exit(f'{arch}: entry count changed ({old_n} -> {new_n}); not supported')
    for i in range(ARCHIVE_SLOTS):
        desc = u(FILE_DESCS + 4 * i)
        if not desc or not u(desc): continue
        p = dol_offset(dol, u(desc))
        if dol[p:p + 128].split(b'\0')[0].decode('ascii', 'replace').lower() != arch.lower(): continue
        table = u(desc + 12)
        if table in (0, 1): sys.exit(f'{arch}: main.dol has no entry table for it (kind {table}); not supported yet')
        t = dol_offset(dol, table)
        if dol[t + 8:t + 8 + 8 * old_n] != old_archive[8:8 + 8 * old_n] or u(table) != old_n:
            sys.exit(f'{arch}: main.dol entry table does not match the clean archive; refusing to patch it')
        new = new_archive[8:8 + 8 * new_n]
        if dol[t + 8:t + 8 + len(new)] == new: return False
        dol[t + 8:t + 8 + len(new)] = new
        return True
    sys.exit(f'{arch}: not found among main.dol file descriptors; cannot keep its entry table in sync')


CHARA_ENTRY_MAX = 0x3A000   # per-robo body load buffer (docs/robo_tables.md, part load requests)


def robo_roles(dat_path, entry, template, all_entries):
    """Robo joint roles (trailer blocks B/C, optionally A) for an edited robo; see docs/MODDING.md.
    With <name>.roles.json next to the .dat: B/C are rebuilt for the new skeleton. Without it, the skeleton
    must be the template's (same joint tree), so the original paths still point at the same bones."""
    roles_file = dat_path.with_suffix('.roles.json')
    if not roles_file.exists():
        new_tree = [p for _, p in robo_joints.joint_tree(entry)[0]]
        old_tree = [p for _, p in robo_joints.joint_tree(template)[0]]
        if new_tree != old_tree:
            sys.exit(f'{dat_path}: the skeleton differs from the original ({len(old_tree)} -> {len(new_tree)} joints).\n'
                     f'  Say which bone plays which role: start from\n'
                     f'  python -I tools/robo_joints.py roles "unpacked/chara/{dat_path.stem[-3:]}.bin" > "{roles_file}"\n'
                     f'  and change the joint numbers (JOBJ_n in HSDRawViewer / Blender) for your model.')
        return entry
    spec = json.loads(roles_file.read_text(encoding='utf-8'))
    missing = [n for n in robo_joints.ROLE_NAMES if n not in spec.get('C', {})]
    if missing or 'B_material_joint' not in spec: sys.exit(f'{roles_file}: needs B_material_joint and C.{missing}')
    n = len(robo_joints.joint_tree(entry)[0])
    joints = [spec['C'][name] for name in robo_joints.ROLE_NAMES]
    bad = [j for j in joints + [spec['B_material_joint']] if j is not None and not (0 <= j < n)]
    if bad: sys.exit(f'{roles_file}: joint numbers {bad} do not exist (the model has joints 0..{n - 1})')
    block_a = None
    if 'A_from' in spec:  # e.g. "chara_005": limb measurements of a robo with similar proportions
        src = all_entries[int(spec['A_from'][-3:])]
        h = struct.unpack_from('>16I', src); block_a = src[h[3]:h[4]]
    return robo_joints.write_paths(entry, spec['B_material_joint'], joints, block_a)


def main():
    if len(sys.argv) < 2: sys.exit(__doc__)
    mod = Path(sys.argv[1])
    iso = Path(sys.argv[2]) if len(sys.argv) > 2 else G / 'Custom Robo (USA).iso'
    if not iso.exists(): sys.exit(f'clean ISO not found: {iso} (pass it as the 2nd argument)')
    meta = json.loads((mod / 'mod.json').read_text(encoding='utf-8')) if (mod / 'mod.json').exists() else {}
    name = re.sub(r'[^A-Za-z0-9_.-]+', '_', meta.get('name', mod.name))
    stage = G / 'build' / 'mods' / name / 'disc'
    shutil.rmtree(stage, ignore_errors=True)
    stage.mkdir(parents=True)
    changed = []
    clean_dol = (DISC / 'sys' / 'main.dol').read_bytes()
    dol = bytearray(clean_dol)  # written to the disc only if something changed it

    # 1. models -> archive entries -> repacked archives
    groups = {}
    for p in sorted((mod / 'models').glob('*/*.dat')) if (mod / 'models').exists() else []:
        cat = p.parent.name
        if cat not in ARCHIVES: sys.exit(f'{p}: category {cat} not supported yet (supported: {", ".join(ARCHIVES)})')
        m = re.fullmatch(re.escape(cat) + r'_(\d{3})', p.stem)
        if not m: sys.exit(f'{p}: name must be {cat}_NNN.dat like in models_dat/{cat}/')
        groups.setdefault(cat, []).append((int(m.group(1)), p))
    for cat, items in groups.items():
        arch, kind = ARCHIVES[cat]
        entries = list(sfd.read_entries((DISC / 'files' / arch).read_bytes()))
        for idx, p in items:
            if idx >= len(entries): sys.exit(f'{p}: {arch} has only {len(entries)} entries')
            dat = p.read_bytes()
            if kind == 'cr':
                template = entries[idx]
                entries[idx] = crmodel.from_dat(dat, template)
                if not crmodel.parse(entries[idx]): sys.exit(f'{p}: rebuilt entry does not parse')
                if cat == 'chara':
                    entries[idx] = robo_roles(p, entries[idx], template, entries)
                    if len(entries[idx]) > CHARA_ENTRY_MAX:
                        sys.exit(f"{p}: entry is {len(entries[idx]):#x} bytes; a body entry must fit the game's "
                                 f'{CHARA_ENTRY_MAX:#x}-byte load buffer (docs/robo_tables.md)')
            else:
                entries[idx] = dat
            changed.append(f'{arch} entry {idx:03d} <- {p.relative_to(mod)}')
        out = stage / 'files' / arch
        out.parent.mkdir(parents=True, exist_ok=True)
        new_archive = sfd.build(entries)
        out.write_bytes(new_archive)
        if sync_entry_table(dol, arch, (DISC / 'files' / arch).read_bytes(), new_archive):
            changed.append(f'main.dol: entry table of {arch} updated (entry sizes changed)')

    # 2. disc files as-is
    if (mod / 'files').exists():
        for p in sorted((mod / 'files').rglob('*')):
            if p.is_file():
                rel = p.relative_to(mod / 'files')
                if (stage / 'files' / rel).exists(): sys.exit(f'{rel}: replaced both as a file and through models/')
                if not (DISC / 'files' / rel).exists(): sys.exit(f'{rel}: no such file on the disc')
                (stage / 'files' / rel).parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(p, stage / 'files' / rel)
                changed.append(f'{rel} (file)')

    # 3. code patches
    patches = sorted((mod / 'code').glob('*.txt')) if (mod / 'code').exists() else []
    clean = clean_dol
    if patches:
        for pf in patches:
            for ln, line in enumerate(pf.read_text(encoding='utf-8').splitlines(), 1):
                if not line.strip() or line.strip().startswith(';'): continue
                m = PATCH_LINE.match(line)
                if not m: sys.exit(f'{pf.name}:{ln}: expected "ADDRESS: BYTES [was BYTES] ; comment"')
                addr, new = int(m.group(1), 16), bytes.fromhex(m.group(2))
                off = dol_offset(clean, addr)
                if m.group(3) is not None and clean[off:off + len(new)] != bytes.fromhex(m.group(3)):
                    sys.exit(f'{pf.name}:{ln}: {addr:08X} holds {clean[off:off + len(new)].hex()}, not {m.group(3)}')
                dol[off:off + len(new)] = new
                changed.append(f'main.dol {addr:08X} ({len(new)} bytes, {pf.name}:{ln})')
    if dol != clean_dol:
        (stage / 'sys').mkdir(parents=True, exist_ok=True)
        (stage / 'sys' / 'main.dol').write_bytes(dol)

    if not changed: sys.exit('the mod changes nothing (see docs/MODDING.md for the folder layout)')
    for c in changed: print('  ' + c)
    out_iso = G / 'build' / 'mods' / name / f'{name}.iso'
    iso_patch.build(str(iso), str(stage), str(out_iso))
    patch = mod / f'{name}.crpatch'
    iso_patch.diff(str(iso), str(out_iso), str(patch))
    print(f'\nplay:  Dolphin > Open > {out_iso}\nshare: {patch}  (players: python -I tools/iso_patch.py apply <their clean ISO> {patch.name} <out.iso>)')


if __name__ == '__main__':
    main()
