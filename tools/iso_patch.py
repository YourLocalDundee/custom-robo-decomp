"""Build a modded Custom Robo ISO, and make / apply shareable patches that contain no game data.

  build <clean.iso> <mod_dir> <out.iso>
      mod_dir mirrors the disc: files/<path> and/or sys/main.dol, holding the files you changed (a full
      extracted folder works too: unchanged files are skipped). Changed files are written in place when they
      fit their original space, otherwise appended after the last file; the file table (FST) and, for
      main.dol, boot.bin are updated. Everything else stays byte-identical to the clean ISO.
  diff <clean.iso> <modded.iso> <out.crpatch>
      A patch holds only what differs: blocks found anywhere in the clean ISO are stored as references to it;
      the rest only as the bytes that differ from the clean ISO at the same place. zlib-compressed.
  apply <clean.iso> <patch.crpatch> <out.iso>
      Checks the clean ISO's SHA-1, rebuilds the modded ISO, checks the result's SHA-1.
  info <patch.crpatch>

Disc layout (GameCube GCM): boot.bin @0 (0x440; +0x420 main.dol offset, +0x424 FST offset, +0x428 FST size,
+0x42C max FST size), bi2.bin @0x440, apploader @0x2440, FST: 12-byte entries {u8 is_dir, u24 name offset,
u32 file offset | parent, u32 size | next index}, then the string table.
"""
import hashlib, json, os, shutil, struct, sys, zlib

CLEAN_SHA1 = 'fff6e74590366a5c0c5ba0d40c8ccf94770bcb16'  # Custom Robo (USA) GXCE01 v1.0
BLOCK = 0x800  # disc sector; SFD archive entries are 0x800-aligned too
MAGIC = b'CRPATCH1'


def sha1_file(path):
    h = hashlib.sha1()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(1 << 20), b''): h.update(chunk)
    return h.hexdigest()


def read_fst(iso):
    """-> (fst_offset, fst_bytes, {disc path: (entry index, offset, size)})"""
    iso.seek(0x424)
    fst_off, fst_size = struct.unpack('>II', iso.read(8))
    iso.seek(fst_off)
    fst = iso.read(fst_size)
    count = struct.unpack_from('>I', fst, 8)[0]
    names = fst[count * 12:]
    files, stack = {}, [(count, '')]  # (end index, dir path)
    for i in range(1, count):
        while stack and i >= stack[-1][0]: stack.pop()
        flags_name, a, b = struct.unpack_from('>III', fst, i * 12)
        name_off = flags_name & 0xFFFFFF
        name = names[name_off:names.index(b'\0', name_off)].decode('shift_jis')
        path = f'{stack[-1][1]}{name}' if stack else name
        if flags_name >> 24:
            stack.append((b, path + '/'))
        else:
            files[path] = (i, a, b)
    return fst_off, fst, files


def build(clean, mod_dir, out):
    if sha1_file(clean) != CLEAN_SHA1: sys.exit('clean ISO is not GXCE01 v1.0 (SHA-1 mismatch)')
    shutil.copyfile(clean, out)
    with open(out, 'r+b') as iso:
        fst_off, fst, files = read_fst(iso)
        fst = bytearray(fst)
        end = max(o + s for _, o, s in files.values())
        end = max(end, fst_off + len(fst))
        changed = []
        files_dir = os.path.join(mod_dir, 'files')
        for root, _, names in os.walk(files_dir):
            for n in names:
                path = os.path.relpath(os.path.join(root, n), files_dir).replace(os.sep, '/')
                if path not in files: sys.exit(f'{path}: not on the disc (adding new files is not supported)')
                idx, off, size = files[path]
                data = open(os.path.join(root, n), 'rb').read()
                iso.seek(off)
                if len(data) == size and iso.read(size) == data: continue
                if len(data) > size:  # does not fit: append, 0x8000-aligned like the original layout
                    off = end = (end + 0x7FFF) & ~0x7FFF
                    end += len(data)
                iso.seek(off); iso.write(data)
                struct.pack_into('>II', fst, idx * 12 + 4, off, len(data))
                changed.append(f'{path} ({size:#x} -> {len(data):#x} @ {off:#x})')
        dol = os.path.join(mod_dir, 'sys', 'main.dol')
        if os.path.exists(dol):
            data = open(dol, 'rb').read()
            iso.seek(0x420); dol_off = struct.unpack('>I', iso.read(4))[0]
            iso.seek(dol_off); old = iso.read(len(data))
            if old != data:
                if dol_off + len(data) > min(o for _, o, _ in files.values() if o > dol_off):
                    sys.exit('main.dol grew past the next file; not supported yet')
                iso.seek(dol_off); iso.write(data)
                changed.append('sys/main.dol')
        iso.seek(fst_off); iso.write(fst)
        size = os.fstat(iso.fileno()).st_size
        if end > size: sys.exit(f'mod data ends at {end:#x}, past the disc size {size:#x}')
    for c in changed: print('changed', c)
    print(f'built {out}: {len(changed)} file(s) changed, SHA-1 {sha1_file(out)}')


def diff(clean, modded, out):
    if sha1_file(clean) != CLEAN_SHA1: sys.exit('clean ISO is not GXCE01 v1.0 (SHA-1 mismatch)')
    a, b = open(clean, 'rb'), open(modded, 'rb')
    if os.path.getsize(clean) != os.path.getsize(modded): sys.exit('ISOs differ in size (not supported)')
    index = {}  # block hash -> clean offset, built only when needed
    ops, literal_bytes, n = [], 0, os.path.getsize(clean)
    deltas = [0]  # recent (target - source) shifts, tried first
    for pos in range(0, n, 1 << 20):
        x, y = a.read(1 << 20), b.read(1 << 20)
        if x == y: continue
        for i in range(0, len(y), BLOCK):
            xb, yb = x[i:i + BLOCK], y[i:i + BLOCK]
            if xb == yb: continue
            if not index:
                with open(clean, 'rb') as c:
                    for off in range(0, n, BLOCK):
                        index.setdefault(hashlib.sha1(c.read(BLOCK)).digest(), off)
            src = index.get(hashlib.sha1(yb).digest())
            if src is not None:
                ops.append(['copy', pos + i, src, len(yb)]); continue
            literal_bytes += match_block(a, n, pos + i, yb, ops, deltas)
    merged = []  # back-to-back copies (e.g. a whole archive moved by a repack) become one op
    for op in ops:
        last = merged[-1] if merged else None
        if (op[0] == 'copy' and last and last[0] == 'copy'
                and last[1] + last[3] == op[1] and last[2] + last[3] == op[2]):
            last[3] += op[3]
        else:
            merged.append(op)
    ops = merged
    body = json.dumps({'base_sha1': CLEAN_SHA1, 'result_sha1': sha1_file(modded), 'ops': ops}).encode()
    with open(out, 'wb') as f: f.write(MAGIC + zlib.compress(body, 9))
    print(f'patch {out}: {len(ops)} ops, {literal_bytes} new bytes, {os.path.getsize(out)} bytes on disk')


WINDOW = 0x40000  # how far around the same place to look for moved clean data
MIN_MATCH = 32


def match_block(clean, n, dst, yb, ops, deltas):
    """Encode one changed block as copies from the clean ISO (any offset within WINDOW, e.g. data shifted
    by a repack) plus literal bytes for what is really new. Returns the number of literal bytes."""
    lo = max(0, dst - WINDOW)
    clean.seek(lo)
    win = clean.read(min(n, dst + len(yb) + WINDOW) - lo)
    lit, j, start = 0, 0, 0
    def flush(k):
        nonlocal lit
        if k > start: ops.append(['bytes', dst + start, yb[start:k].hex()]); lit += k - start
    while j < len(yb):
        best_src, best_len = None, 0
        cands = [dst + j - d for d in deltas]
        if len(yb) - j >= MIN_MATCH:
            f = win.find(yb[j:j + MIN_MATCH])
            if f >= 0: cands.append(lo + f)
        for s in cands:
            if s < 0 or s >= n: continue
            w = s - lo
            if w < 0 or w >= len(win): continue
            L = 0
            while j + L < len(yb) and w + L < len(win) and win[w + L] == yb[j + L]: L += 1
            if L > best_len: best_src, best_len = s, L
        if best_len >= MIN_MATCH or (best_len >= 4 and best_src == dst + j):
            flush(j)
            ops.append(['copy', dst + j, best_src, best_len])
            d = dst + j - best_src
            if d in deltas: deltas.remove(d)
            deltas.insert(0, d); del deltas[4:]
            j += best_len; start = j
        else:
            j += 1
    flush(len(yb))
    return lit


def load_patch(path):
    raw = open(path, 'rb').read()
    if not raw.startswith(MAGIC): sys.exit('not a .crpatch file')
    return json.loads(zlib.decompress(raw[len(MAGIC):]))


def apply(clean, patch, out):
    p = load_patch(patch)
    if sha1_file(clean) != p['base_sha1']: sys.exit('clean ISO SHA-1 does not match the patch base (need GXCE01 v1.0)')
    shutil.copyfile(clean, out)
    with open(clean, 'rb') as src, open(out, 'r+b') as dst:
        for op in p['ops']:
            if op[0] == 'copy':
                src.seek(op[2]); data = src.read(op[3])
            else:
                data = bytes.fromhex(op[2])
            dst.seek(op[1]); dst.write(data)
    got = sha1_file(out)
    if got != p['result_sha1']: sys.exit(f'result SHA-1 {got} does not match the patch ({p["result_sha1"]})')
    print(f'applied: {out} (SHA-1 {got} as expected)')


def main():
    cmd, *a = sys.argv[1:] or ['help']
    if cmd == 'build' and len(a) == 3: build(*a)
    elif cmd == 'diff' and len(a) == 3: diff(*a)
    elif cmd == 'apply' and len(a) == 3: apply(*a)
    elif cmd == 'info' and len(a) == 1:
        p = load_patch(a[0]); lit = sum(len(o[2]) // 2 for o in p['ops'] if o[0] == 'bytes')
        print(f"base {p['base_sha1']}\nresult {p['result_sha1']}\n{len(p['ops'])} ops, {lit} new bytes")
    else: print(__doc__)


if __name__ == '__main__':
    main()
