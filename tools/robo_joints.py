"""Decode the joint paths of robo model entries (blocks B and C) into joint numbers, and back.

Read from the robo builder FUN_80074110 (decompiled\\main_dol.c):
  block C (header[8]): 0x13 paths; byte 02 = go to child, 01 = go to next sibling, FF = end of a path.
                       The joints found are stored in the robo object at +0xB44 + 4*role.
  block B (header[4]): one path ending in FE; the material (jobj->dobj->mobj) of that joint is kept at +0xBC0.
  block A (header[3]): 0x80 bytes of floats used by the procedural limb code (see docs/formats/model_entry.md).
Joint numbers are depth-first pre-order (joint, its children, then its next siblings) - the order HSDRawViewer
uses for JOBJ_n in exports.

usage: python -I tools/robo_joints.py table [unpacked/chara]      roles -> joint numbers for every robo entry
       python -I tools/robo_joints.py roles <entry.bin>             a roles.json for that robo (start of a new one)
       python -I tools/robo_joints.py selftest [unpacked/chara]    re-encode every robo's paths: must be byte-identical
       python -I tools/robo_joints.py path <entry.bin> <joint#>     the path bytes that reach joint# (for new models)
"""
import struct, sys
from pathlib import Path

BASE = 0x40
ROLES = 0x13


def u32(d, o): return struct.unpack_from('>I', d, o)[0]


def joint_tree(entry):
    """-> list of (file offset of joint desc, parent index, child offset, next offset) in pre-order"""
    h = struct.unpack_from('>16I', entry)
    desc = BASE + u32(entry, BASE + h[2])        # header[2] -> pointer to JOBJDesc
    root = BASE + u32(entry, desc)               # JOBJDesc +0 -> root joint desc
    order, index = [], {}
    def walk(off, parent):
        while off != BASE:                       # null pointer (0) + base
            index[off] = len(order)
            child, nxt = u32(entry, off + 8), u32(entry, off + 0xC)
            order.append((off, parent))
            if child: walk(BASE + child, index[off])
            if not nxt: break
            off = BASE + nxt
    walk(root, -1)
    return order, index


def follow(entry, pos, end, root):
    """Walk one path from pos; returns (joint desc offset or None, position after the end byte)."""
    j = root
    while entry[pos] != end:
        b = entry[pos]
        if b == 2: j = (BASE + u32(entry, j + 8)) if j and u32(entry, j + 8) else None
        elif b == 1: j = (BASE + u32(entry, j + 0xC)) if j and u32(entry, j + 0xC) else None
        else: raise ValueError(f'unexpected path byte {b:#x} at {pos:#x}')
        pos += 1
    return j, pos + 1


def decode(entry):
    h = struct.unpack_from('>16I', entry)
    order, index = joint_tree(entry)
    root = order[0][0]
    pos, roles = h[8], []
    for _ in range(ROLES):
        if entry[pos] == 0xFF: roles.append(None); pos += 1; continue
        j, pos = follow(entry, pos, 0xFF, root)
        roles.append(index.get(j))
    b, _ = follow(entry, h[4], 0xFE, root)
    return len(order), index.get(b), roles


def path_to(entry, target):
    """Bytes that reach joint #target from the root (02 child / 01 next), ending in FF."""
    order, _ = joint_tree(entry)
    parent = {i: p for i, (_, p) in enumerate(order)}
    off_of = {i: o for i, (o, _) in enumerate(order)}
    chain = []
    i = target
    while i != 0:
        chain.append(i); i = parent[i]
    chain.reverse()
    out, cur = bytearray(), 0
    for nxt in chain:          # go to the first child, then next siblings until nxt
        c = u32(entry, off_of[cur] + 8)
        out.append(2); j = BASE + c
        while j != off_of[nxt]:
            out.append(1); j = BASE + u32(entry, j + 0xC)
        cur = nxt
    return bytes(out) + b'\xff'


ROLE_NAMES = [  # from robo 0's bind pose (plan.md step 25); "a"/"b" = the -X / +X side
    'upper_body', 'head',
    'arm_a_shoulder', 'arm_a_upper', 'arm_a_fore', 'arm_a_hand',
    'arm_b_shoulder', 'arm_b_upper', 'arm_b_fore', 'arm_b_hand',
    'lower_body',
    'leg_a_thigh', 'leg_a_shin', 'leg_a_ankle', 'leg_a_foot',
    'leg_b_thigh', 'leg_b_shin', 'leg_b_ankle', 'leg_b_foot',
]


def roles_json(entry):
    """A roles file describing an existing entry: start from this for a new skeleton."""
    _, b, roles = decode(entry)
    return {'B_material_joint': b, 'C': {name: j for name, j in zip(ROLE_NAMES, roles)}}


def write_paths(entry, b_joint, role_joints, block_a=None):
    """Rebuild trailer blocks B and C (and optionally A) of a robo entry for its own joint tree.
    role_joints: 19 joint numbers (None = no joint). Returns the new entry."""
    h = list(struct.unpack_from('>16I', entry))
    if h[9]: raise ValueError('entries with a block D are not supported here')
    if not (h[3] < h[4] < h[8] <= h[1]): raise ValueError('unexpected trailer layout (expected A, B, C)')
    a = block_a if block_a is not None else entry[h[3]:h[4]]
    if len(a) != h[4] - h[3]: raise ValueError(f'block A must be {h[4] - h[3]:#x} bytes')
    b = path_to(entry, b_joint)[:-1] + b'\xfe'
    c = b''.join(b'\xff' if j is None else path_to(entry, j) for j in role_joints)
    pad = lambda x: x + b'\0' * (-len(x) % 0x20)
    trailer = a + pad(b) + pad(c)
    h[4] = h[3] + len(a); h[8] = h[4] + len(pad(b)); h[1] = h[8] + len(pad(c))
    out = struct.pack('>16I', *h) + entry[0x40:h[3]] + trailer
    n, b2, roles2 = decode(out)  # must read back exactly as the game will walk it
    if b2 != b_joint or roles2 != list(role_joints): raise AssertionError('paths do not read back')
    return out


def main():
    if len(sys.argv) >= 2 and sys.argv[1] == 'selftest':
        # rebuilding blocks B/C from the decoded roles must give every robo entry back byte for byte
        d = Path(sys.argv[2] if len(sys.argv) > 2 else 'unpacked/chara')
        bad = []
        files = sorted(d.glob('[0-9][0-9][0-9].bin'))
        for p in files:
            e = p.read_bytes(); _, b, roles = decode(e)
            out = write_paths(e, b, roles)
            if out != e[:len(out)] or any(e[len(out):]): bad.append(p.name)
        print(f'joint paths: {len(files) - len(bad)}/{len(files)} robo entries rebuild byte-identical' + (f'; differ: {bad[:5]}' if bad else ''))
        sys.exit(1 if bad or not files else 0)
    if len(sys.argv) == 3 and sys.argv[1] == 'roles':
        import json
        print(json.dumps(roles_json(Path(sys.argv[2]).read_bytes()), indent=2))
        return
    if len(sys.argv) >= 2 and sys.argv[1] == 'table':
        d = Path(sys.argv[2] if len(sys.argv) > 2 else 'unpacked/chara')
        print('entry joints B  ' + ' '.join(f'C{r:02d}' for r in range(ROLES)))
        for p in sorted(d.glob('[0-9][0-9][0-9].bin')):
            n, b, roles = decode(p.read_bytes())
            print(f'{p.stem}   {n:4d}  {b!s:3} ' + ' '.join(f'{"-" if r is None else r:>3}' for r in roles))
    elif len(sys.argv) == 4 and sys.argv[1] == 'path':
        print(path_to(Path(sys.argv[2]).read_bytes(), int(sys.argv[3])).hex(' '))
    else:
        print(__doc__)


if __name__ == '__main__':
    main()
