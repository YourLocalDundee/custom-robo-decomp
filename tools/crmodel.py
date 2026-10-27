"""Custom Robo (GC) model entries <-> standard HSD .dat files, with a byte-identical round trip check.

Entry layout, taken from the game's own loaders (decompiled\\main_dol.c):
  header[0] = data base (0x40). Data block = file[0x40 : reloc table]. Pointers are offsets from 0x40.
  header[2] = root object (from 0x40), relocated in every loader. abs: pointer to a JOBJDesc;
              rel: array of header[3] JOBJDesc records (0x10 each). See root_joints().
  header[8] = joint path list: bytes 02 = child, 01 = next sibling, ff = end of one path
              (FUN_800748a8 walks 0x13 paths to pick joints). Purpose of the picked joints not verified.
  header[7] = reloc entry count. Each entry is a pointer slot (from 0x40) that gets += base.
  Variant "abs" (chara, snr_chara) - loaders FUN_800747b0 / FUN_800748a8:
      reloc table at file + header[6]; header[3], [4], [8] relocated against the file start.
  Variant "rel" (weapon_*) - loader FUN_80073cb8:
      reloc table at file + 0x40 + header[6]; header[4] and header[8] (unless -1) relocated against 0x40.
  header[1] = end of trailer blocks A/B/C (file offset). header[9] (abs variant, when non-zero) = start
  of an extra block D running to end of file (inferred from data). Trailer blocks are not understood
  yet and are carried over unchanged, with their header offsets shifted.

Usage:
  python -I crmodel.py to-dat <entry.bin> <out.dat>
  python -I crmodel.py from-dat <in.dat> <template_entry.bin> <out_entry.bin>
  python -I crmodel.py roundtrip <unpacked_dir>...        entry -> .dat -> entry, compare bytes
"""
import struct
import sys
from pathlib import Path

BASE = 0x40
NONE = 0xFFFFFFFF


def _sorted_table(d, off, cnt):
    if not 0 < cnt < 0x10000 or off + cnt * 4 > len(d):
        return None
    r = struct.unpack_from(">%dI" % cnt, d, off)
    return list(r) if all(a < b for a, b in zip(r, r[1:])) else None


def parse(d):
    """-> dict(variant, header, data, reloc) or None if this is not a model entry."""
    if len(d) < BASE:
        return None
    h = list(struct.unpack_from(">16I", d, 0))
    if h[0] != BASE:
        return None
    for variant, table_off in (("abs", h[6]), ("rel", BASE + h[6])):
        reloc = _sorted_table(d, table_off, h[7])
        if reloc and table_off > BASE and h[2] < table_off - BASE:
            return {"variant": variant, "header": h, "data": d[BASE:table_off], "reloc": reloc,
                    "table_off": table_off}
    return None


def _trailer_fields(e):
    """Header indices holding file positions in the trailer, as (index, delta_to_file_offset)."""
    h = e["header"]
    if e["variant"] == "abs":
        # [9] (when non-zero) = start of an extra block D that runs to the end of the file. Seen only in
        # some snr_chara entries; inferred from the data, no loader read so far touches it.
        idx = [1, 3, 4, 8] + ([9] if h[9] else [])
        return [(i, 0) for i in idx if h[i] != NONE]
    idx = [4, 8]
    return [(i, BASE) for i in idx if h[i] != NONE] + [(1, 0)]


def _u32(data, o):
    return struct.unpack_from(">I", data, o)[0]


def root_joints(e):
    """Root JObj offsets, reached the way the game's loaders reach them.

    abs: header[2] -> pointer to one JOBJDesc {RootJoint, JointAnims, ...}   (FUN_800748a8: **header[2])
    rel: header[2] -> header[3] inline JOBJDesc records of 0x10 bytes        (FUN_80073cb8)
    """
    data, h = e["data"], e["header"]
    if e["variant"] == "abs":
        return [_u32(data, _u32(data, h[2]))]
    return [_u32(data, h[2] + 0x10 * i) for i in range(h[3])]


def to_dat(d):
    """Standard .dat with roots "cr_header2" (the header[2] object, needed to rebuild the entry) and
    "root<i>_joint" for each root joint (the _joint suffix makes HSDRawViewer treat it as a JObj).
    Root names are ours; the game files carry no names."""
    e = parse(d)
    if e is None:
        raise ValueError("not a Custom Robo model entry")
    data, reloc = e["data"], e["reloc"]
    roots = [("cr_header2", e["header"][2])] + [(f"root{i}_joint", j) for i, j in enumerate(root_joints(e))]
    root_tab, strings = b"", b""
    for name, off in roots:
        root_tab += struct.pack(">II", off, len(strings))
        strings += name.encode() + b"\0"
    body = data + b"".join(struct.pack(">I", p) for p in reloc) + root_tab + strings
    header = struct.pack(">5I", 0x20 + len(body), len(data), len(reloc), len(roots), 0) + b"\0" * 12
    return header + body


def read_dat(dat):
    fs, ds, rc, nroot, nref = struct.unpack_from(">5I", dat)
    data = dat[0x20:0x20 + ds]
    reloc = list(struct.unpack_from(">%dI" % rc, dat, 0x20 + ds))
    rt = 0x20 + ds + rc * 4
    st = rt + (nroot + nref) * 8
    roots = {}
    for i in range(nroot):
        off, so = struct.unpack_from(">II", dat, rt + i * 8)
        roots[dat[st + so:dat.index(b"\0", st + so)].decode()] = off
    return data, sorted(reloc), roots


def from_dat(dat, template):
    """Rebuild a game entry from a .dat, keeping the template's header layout and trailer blocks."""
    t = parse(template)
    if t is None:
        raise ValueError("template is not a Custom Robo model entry")
    data, reloc, roots = read_dat(dat)
    if "cr_header2" not in roots:
        raise ValueError("the .dat needs the root named cr_header2 (written by to_dat)")
    h = list(t["header"])
    fields = _trailer_fields(t)
    old_reloc_end = t["table_off"] + 4 * len(t["reloc"])
    old_trailer = min(h[i] + delta for i, delta in fields)
    old_end = len(template) if t["variant"] == "abs" and h[9] else h[1]
    align = 0x20 if old_trailer != old_reloc_end else 4

    new_table_off = BASE + len(data)
    new_reloc_end = new_table_off + 4 * len(reloc)
    new_trailer = (new_reloc_end + align - 1) // align * align
    shift = new_trailer - old_trailer

    h[2] = roots["cr_header2"]
    h[6] = new_table_off if t["variant"] == "abs" else new_table_off - BASE
    h[7] = len(reloc)
    for i, _ in fields:
        h[i] = (h[i] + shift) & NONE
    out = struct.pack(">16I", *h) + data + b"".join(struct.pack(">I", p) for p in reloc)
    out += b"\0" * (new_trailer - len(out)) + template[old_trailer:old_end]
    tail_pad = len(template) - old_end  # the game pads entries; keep the same rule (multiple of 0x20)
    out += b"\0" * ((-len(out)) % 0x20 if tail_pad else 0)
    return out


def roundtrip(dirs):
    total = same = 0
    for d in dirs:
        for p in sorted(Path(d).glob("*.bin")):
            orig = p.read_bytes()
            if parse(orig) is None:
                continue
            total += 1
            rebuilt = from_dat(to_dat(orig), orig)
            if rebuilt == orig:
                same += 1
            else:
                first = next(i for i in range(min(len(rebuilt), len(orig))) if rebuilt[i] != orig[i]) \
                    if rebuilt[:len(orig)] != orig[:len(rebuilt)] else min(len(rebuilt), len(orig))
                print(f"DIFF {p} len {len(rebuilt):#x} vs {len(orig):#x}, first difference at {first:#x}")
    print(f"round trip: {same}/{total} byte-identical")
    return same == total


def main():
    cmd, *a = sys.argv[1:]
    if cmd == "to-dat":
        Path(a[1]).write_bytes(to_dat(Path(a[0]).read_bytes()))
    elif cmd == "from-dat":
        Path(a[2]).write_bytes(from_dat(Path(a[0]).read_bytes(), Path(a[1]).read_bytes()))
    elif cmd == "roundtrip":
        sys.exit(0 if roundtrip(a) else 1)
    else:
        print(__doc__)


if __name__ == "__main__":
    main()
