"""Explore a Custom Robo model entry (chara / weapon_*): header, relocation table, pointer graph.

Data block starts at 0x40; pointers are offsets from there. The reloc table lists every pointer
location. Header field 6/7 = reloc (offset, count); in chara entries header offsets are absolute,
in weapon entries they are relative to 0x40.

Usage: python -I cr_model_explore.py <entry.bin> [max_depth]
"""
import struct
import sys

BASE = 0x40


def load(path):
    d = open(path, "rb").read()
    h = struct.unpack_from(">10I", d, 0)
    for rel_base in (0, BASE):
        off, cnt = h[6] + rel_base, h[7]
        if off + cnt * 4 <= len(d):
            r = struct.unpack_from(">%dI" % cnt, d, off)
            if all(a < b for a, b in zip(r, r[1:])):
                return d, h, set(r), rel_base
    raise ValueError("reloc table not found")


def main():
    path = sys.argv[1]
    max_depth = int(sys.argv[2]) if len(sys.argv) > 2 else 6
    d, h, rel, rel_base = load(path)
    data_end = h[6] + rel_base - BASE  # data runs up to the reloc table
    targets = sorted({struct.unpack_from(">I", d, BASE + p)[0] for p in rel})
    print("header", [hex(x) for x in h], "header offsets", "absolute" if rel_base == 0 else "relative")
    print("relocs", len(rel), "distinct targets", len(targets))

    def obj_size(o):
        i = next((t for t in targets if t > o), data_end)
        return i - o

    seen = set()

    def dump(o, depth, label):
        size = obj_size(o)
        words = []
        for i in range(0, min(size, 0x60), 4):
            v = struct.unpack_from(">I", d, BASE + o + i)[0]
            words.append(("*%x" % v) if (o + i) in rel else "%08x" % v)
        print("  " * depth + f"{label} @{o:#x} size {size:#x}: " + " ".join(words))
        if o in seen or depth >= max_depth:
            return
        seen.add(o)
        for i in range(0, size, 4):
            if (o + i) in rel:
                dump(struct.unpack_from(">I", d, BASE + o + i)[0], depth + 1, f"+{i:#x}")

    roots = [p for p in sorted(rel) if p >= data_end - 0x40]
    root = roots[-1] if roots else data_end - 0x10
    dump(root - (root % 0x10), 0, "root")


if __name__ == "__main__":
    main()
