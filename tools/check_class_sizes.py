"""Check the HSD struct sizes parsed from Melee's headers against Custom Robo's own binary.

Every HSD class registers itself at startup with its info-struct size and object size, e.g.
  FUN_801a74d8(&info, &parent, s_sysdolphin_base_library_..., s_hsd_jobj_..., 0x54, 0x88);
so Custom Robo states sizeof(HSD_JObjInfo) and sizeof(HSD_JObj) itself. The sizes parsed by
ParseHeaders.java (STRUCTSIZE lines of its report) must match, or the types would mislead.
usage: python -I tools/check_class_sizes.py <ParseHeaders report> [decompiled/main_dol.c]
"""
import os, re, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CALL = re.compile(r's_sysdolphin_base_library_[0-9a-f]+\s*,\s*s_(hsd_\w+?)_[0-9a-f]+\s*,\s*(0x[0-9a-f]+|\d+)\s*,\s*(0x[0-9a-f]+|\d+)\s*\)')


def struct_name(cls):
    # hsd_jobj -> HSD_JObj, hsd_fog -> HSD_Fog, hsd_fogadj -> HSD_FogAdj: try the header's spellings
    base = cls[4:]
    return [f'HSD_{base[0].upper()}{base[1:-3]}{base[-3:].capitalize()}' if base.endswith('obj') else None,
            f'HSD_{base.capitalize()}', f'HSD_{base.upper()}']


def main():
    report = sys.argv[1]
    c_file = sys.argv[2] if len(sys.argv) > 2 else os.path.join(G, 'decompiled', 'main_dol.c')
    parsed = {}
    for l in open(report, encoding='utf-8'):
        if l.startswith('STRUCTSIZE '):
            _, n, s = l.split(); parsed[n] = int(s, 16)
    found = {}
    for m in CALL.finditer(open(c_file, encoding='utf-8', errors='replace').read()):
        found[m.group(1)] = (int(m.group(2), 0), int(m.group(3), 0))
    ok = bad = unknown = known = 0
    for cls, (info, obj) in sorted(found.items()):
        names = [n for n in struct_name(cls) if n and n in parsed]
        if not names: unknown += 1; print(f'??   {cls:14} obj=0x{obj:X} info=0x{info:X} (no parsed struct by that name)'); continue
        n = names[0]
        checks = [(n, obj)] + ([(n + 'Info', info)] if n + 'Info' in parsed else [])
        for sn, want in checks:
            if parsed[sn] == want: ok += 1; print(f'OK   {sn:18} 0x{want:X}')
            elif sn.endswith('Info') and want == parsed[sn] + 4:
                # known: CR's library adds one method to each class method table (HSD_ObjInfo itself matches).
                # These *Info types are never applied (ParseHeaders.java), so this does not mislead the C.
                known += 1; print(f'KNOWN {sn:17} custom robo 0x{want:X}, parsed 0x{parsed[sn]:X} (one extra method; type not applied)')
            else: bad += 1; print(f'BAD  {sn:18} custom robo 0x{want:X}, parsed 0x{parsed[sn]:X}')
    print(f'class sizes: {ok} match, {known} known differences, {bad} differ, '
          f'{unknown} classes without a parsed struct ({len(found)} classes in the game)')
    sys.exit(1 if bad or not ok else 0)


if __name__ == '__main__':
    main()
