"""Custom Robo (GC) SFD archive tool: unpack, pack, and byte-identical round-trip check.

Format (big-endian), verified against all 22 SFD archives on the disc:
  0x0000  "SFD\\0", u32 count, count x (u32 offset, u32 size), zero padding up to 0x8000
  entries start at 0x8000, each at the next 0x800 boundary after the previous one (zero gaps)
  file length padded with zeros to a multiple of 0x800

Usage:
  python -I sfd.py unpack <files_dir> <out_dir>     every *.BIN that is SFD, recursively; keeps sub-folders
  python -I sfd.py pack <entry_dir> <out.BIN>        entries named 000.bin, 001.bin, ... in index order
  python -I sfd.py verify <files_dir> <unpacked_dir> repack every archive in memory, compare to the original
"""
import struct
import sys
from pathlib import Path

MAGIC = b"SFD\0"
HEADER_SIZE = 0x8000
ALIGN = 0x800


def align(n):
    return (n + ALIGN - 1) // ALIGN * ALIGN


def read_entries(data):
    count = struct.unpack_from(">I", data, 4)[0]
    for i in range(count):
        off, size = struct.unpack_from(">II", data, 8 + i * 8)
        yield data[off:off + size]


def build(entries):
    table = b""
    body = b""
    pos = HEADER_SIZE
    for e in entries:
        table += struct.pack(">II", pos, len(e))
        body += e + b"\0" * (align(len(e)) - len(e))
        pos += align(len(e))
    header = MAGIC + struct.pack(">I", len(entries)) + table
    if len(header) > HEADER_SIZE:
        raise ValueError("too many entries for a 0x8000 header")
    return header + b"\0" * (HEADER_SIZE - len(header)) + body


def archives(files_dir):
    for p in sorted(Path(files_dir).rglob("*")):
        if p.suffix.lower() == ".bin" and p.is_file() and p.read_bytes()[:4] == MAGIC:
            yield p


def entry_dir_for(archive, files_dir, out_dir):
    rel = archive.relative_to(files_dir)
    return Path(out_dir) / rel.parent / rel.stem


def entry_files(entry_dir):
    return sorted(Path(entry_dir).glob("[0-9][0-9][0-9].bin"))


def unpack(files_dir, out_dir):
    for a in archives(files_dir):
        out = entry_dir_for(a, files_dir, out_dir)
        out.mkdir(parents=True, exist_ok=True)
        n = 0
        for i, e in enumerate(read_entries(a.read_bytes())):
            (out / f"{i:03d}.bin").write_bytes(e)
            n += 1
        print(f"{a.relative_to(files_dir)}: {n} entries -> {out}")


def pack(entry_dir, out_file):
    data = build([f.read_bytes() for f in entry_files(entry_dir)])
    Path(out_file).write_bytes(data)
    print(f"wrote {out_file} ({len(data):#x} bytes)")


def verify(files_dir, unpacked_dir):
    ok = True
    for a in archives(files_dir):
        orig = a.read_bytes()
        rebuilt = build([f.read_bytes() for f in entry_files(entry_dir_for(a, files_dir, unpacked_dir))])
        same = rebuilt == orig
        ok &= same
        print(f"{'OK  ' if same else 'DIFF'} {a.relative_to(files_dir)}")
    print("all archives byte-identical" if ok else "MISMATCH FOUND")
    return ok


def main():
    cmd, *args = sys.argv[1:]
    if cmd == "unpack":
        unpack(*args)
    elif cmd == "pack":
        pack(*args)
    elif cmd == "verify":
        sys.exit(0 if verify(*args) else 1)
    else:
        print(__doc__)


if __name__ == "__main__":
    main()
