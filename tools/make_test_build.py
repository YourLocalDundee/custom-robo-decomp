"""Build a Dolphin-bootable test copy of the game with a visibly modified robo, through the full import path.

For each chara entry given: entry -> .dat (crmodel.to_dat) -> swap red/blue in every CMPR texture ->
shift the whole data block by SHIFT bytes (every pointer, reloc entry and root moves, so the trailer
blocks and header offsets must follow) -> entry (crmodel.from_dat, original entry as template) ->
chara.BIN repacked (sfd.build). Struct offsets used here are HSDLib's (HSD_JOBJ/DOBJ/MOBJ/TOBJ/Image).

Usage: python -I make_test_build.py <extracted_disc_dir> <unpacked_dir> <out_build_dir> <entry ids...>
Boot: Dolphin.exe -e <out_build_dir>\\sys\\main.dol
"""
import shutil
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))  # python -I drops the script dir
import crmodel  # noqa: E402
import sfd  # noqa: E402

SHIFT = 0x100
GX_CMP = 14


def u32(b, o):
    return struct.unpack_from(">I", b, o)[0]


def textures(data, reloc, roots):
    """Yield (image_data_offset, width, height, format) for every TObj reachable from the root joints."""
    rel, seen, out = set(reloc), set(), []

    def ptr(o):
        return u32(data, o) if o in rel else None

    def jobj(o):
        while o is not None and o not in seen:
            seen.add(o)
            flags = u32(data, o + 4)
            if not flags & ((1 << 14) | (1 << 5)):  # HSDLib: Dobj is not valid for SPLINE / PTCL joints
                d = ptr(o + 0x10)
                while d is not None:
                    m = ptr(d + 8)
                    t = ptr(m + 8) if m is not None else None
                    while t is not None:
                        img = ptr(t + 0x4C)
                        if img is not None and ptr(img) is not None:
                            out.append((ptr(img), *struct.unpack_from(">HHI", data, img + 4)))
                        t = ptr(t + 4)
                    d = ptr(d + 4)
            jobj(ptr(o + 8))
            o = ptr(o + 0xC)

    for r in roots:
        jobj(r)
    return sorted(set(out))


def swap_rb_cmpr(buf, off, w, h):
    """Swap red/blue in each CMPR sub-block, keeping each block's 4-colour / 3-colour+alpha mode."""
    for b in range(off, off + w * h // 2, 8):
        c0, c1, idx = struct.unpack_from(">HHI", buf, b)
        sw = lambda c: ((c & 0x1F) << 11) | (c & 0x07E0) | (c >> 11)  # noqa: E731
        n0, n1 = sw(c0), sw(c1)
        if (c0 > c1) != (n0 > n1) and n0 != n1:
            # mode would flip: swap endpoints and remap indices 0<->1 (and 2<->3 in 4-colour mode)
            n0, n1 = n1, n0
            m = idx
            idx = 0
            for i in range(16):
                v = (m >> (30 - 2 * i)) & 3
                v = {0: 1, 1: 0, 2: 3 if c0 > c1 else 2, 3: 2 if c0 > c1 else 3}[v]
                idx |= v << (30 - 2 * i)
        struct.pack_into(">HHI", buf, b, n0, n1, idx)


def shift_dat(dat, shift):
    data, reloc, roots = crmodel.read_dat(dat)
    new = bytearray(shift) + bytearray(data)
    for p in reloc:
        struct.pack_into(">I", new, p + shift, u32(new, p + shift) + shift)
    reloc = [p + shift for p in reloc]
    roots = {k: v + shift for k, v in roots.items()}
    rt, st = b"", b""
    for k, v in roots.items():
        rt += struct.pack(">II", v, len(st))
        st += k.encode() + b"\0"
    body = bytes(new) + b"".join(struct.pack(">I", p) for p in reloc) + rt + st
    return struct.pack(">5I", 0x20 + len(body), len(new), len(reloc), len(roots), 0) + b"\0" * 12 + body


def main():
    disc, unpacked, out = (Path(a) for a in sys.argv[1:4])
    ids = [int(x) for x in sys.argv[4:]]
    if not out.exists():
        shutil.copytree(disc, out)
    entries = [f.read_bytes() for f in sfd.entry_files(unpacked / "chara")]
    for i in ids:
        dat = bytearray(crmodel.to_dat(entries[i]))
        data, reloc, roots = crmodel.read_dat(bytes(dat))
        joints = [v for k, v in roots.items() if k.endswith("_joint")]
        texs = [t for t in textures(data, reloc, joints) if t[3] == GX_CMP]
        for off, w, h, _ in texs:
            swap_rb_cmpr(dat, 0x20 + off, w, h)
        shifted = shift_dat(bytes(dat), SHIFT)
        new_entry = crmodel.from_dat(shifted, entries[i])
        check = crmodel.parse(new_entry)
        assert check and crmodel.root_joints(check) == [j + SHIFT for j in joints], "rebuilt entry does not parse"
        entries[i] = new_entry
        print(f"chara {i:03d}: {len(texs)} CMPR textures recoloured, data shifted by {SHIFT:#x}, "
              f"entry {len(new_entry):#x} bytes")
    (out / "files" / "chara.BIN").write_bytes(sfd.build(entries))
    print(f"wrote {out / 'files' / 'chara.BIN'}")


if __name__ == "__main__":
    main()
