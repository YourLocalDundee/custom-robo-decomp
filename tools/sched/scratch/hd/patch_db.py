import re
p = r'D:\emulators\gameTesting-hsd2\tools\draft_batch.py'
s = open(p, encoding='utf-8', newline='').read()
nl = '\r\n' if '\r\n' in s else '\n'
s = s.replace('\r\n', '\n')

def rep(old, new, tag):
    global s
    assert old in s, tag
    s = s.replace(old, new)

# (1) pool symbols of a C file object: "@NN" -> lbl_<address>
rep(r"""                if blk[0] in local.values() and not all(re.match(r'^\t\.4byte \.L_', x) for x in blk[1] if x.strip()):
                    # a local object that is not a jump table (a constant, a string): not renamed, not copied
                    local = {k: v for k, v in local.items() if v != blk[0]}
                else:""",
r"""                if blk[0] in local.values() and not all(re.match(r'^\t\.4byte \.L_', x) for x in blk[1] if x.strip()):
                    # a local object that is not a jump table (a constant, a string of a file object's own pool, step 84):
                    # not copied; the functions name it lbl_<address>, as the drafts did before the object was built
                    # from C and tools/gen_c_symbols.py wrote the C object's own "@NN" into symbols.txt
                    local = {k: (v if v != blk[0] else 'lbl_' + v[len('jumptable_'):]) for k, v in local.items()}
                else:""", 'local')

rep(r"""            m = re.match(r'^(lbl_[0-9A-F]{8}) = \.sdata2:0x([0-9A-F]+); // type:object size:0x([0-9A-F]+)(.*)$', l)
            if not m or ('data:' in m.group(4) and 'data:string' not in m.group(4)):
                continue
            b = read(int(m.group(2), 16), int(m.group(3), 16))
            if b is None or 0 not in b or b[0] == 0 or any(b[b.index(0):]) or any(c < 0x20 for c in b[:b.index(0)]):
                continue
            out[m.group(1)] = '"' + ''.join('\\x%02x' % c for c in b[:b.index(0)]) + '"'""",
r"""            m = re.match(r'^(lbl_[0-9A-F]{8}|@\d+) = \.sdata2:0x([0-9A-F]+); // type:object size:0x([0-9A-F]+)(.*)$', l)
            if not m or ('data:' in m.group(4) and 'data:string' not in m.group(4)):
                continue
            b = read(int(m.group(2), 16), int(m.group(3), 16))
            if b is None or 0 not in b or b[0] == 0 or any(b[b.index(0):]) or any(c < 0x20 for c in b[:b.index(0)]):
                continue
            out[pool_name(m.group(1), m.group(2))] = '"' + ''.join('\\x%02x' % c for c in b[:b.index(0)]) + '"'""", 'strings')

rep(r"""            m = re.match(r'^(lbl_[0-9A-F]{8}) = \.sdata2:0x([0-9A-F]+); // type:object size:0x([48])\b.*\bdata:(float|double)\b', l)""",
r"""            m = re.match(r'^(lbl_[0-9A-F]{8}|@\d+) = \.sdata2:0x([0-9A-F]+); // type:object size:0x([48])\b.*\bdata:(float|double)\b', l)""", 'floats')

rep(r"""            if t:
                out[m.group(1)] = t
        W['floats'] = out
    return W['floats']""",
r'''            if t:
                out[pool_name(m.group(1), m.group(2))] = t
        W['floats'] = out
    return W['floats']


def pool_name(name, addr):
    """the draft name of a data object of symbols.txt: its own lbl_ name, or lbl_<address> for a C file object's own
    pool symbol ("@22": tools/gen_c_symbols.py writes the C object's local symbols into symbols.txt once the object is
    built from C, step 84; a draft cannot declare a name with '@')"""
    return name if not name.startswith('@') else 'lbl_%08X' % int(addr, 16)''', 'floats2')

rep(r"""        m = re.match(r'^(lbl_[0-9A-F]{8}) = \.(\w+):0x([0-9A-F]+); // type:object size:0x([0-9A-F]+)(.*)$', l)
        if not m:
            continue
        name, sec, addr, size, rest = m.group(1), m.group(2), int(m.group(3), 16), int(m.group(4), 16), m.group(5)""",
r"""        m = re.match(r'^(lbl_[0-9A-F]{8}|@\d+) = \.(\w+):0x([0-9A-F]+); // type:object size:0x([0-9A-F]+)(.*)$', l)
        if not m:
            continue
        name, sec, addr, size, rest = pool_name(m.group(1), m.group(3)), m.group(2), int(m.group(3), 16), int(m.group(4), 16), m.group(5)""", 'decls')

# (3) script_counter_ptr: the counter pointer declared after the PC local
rep(r"""    decl = '    u32 *cmd_cnt;'
    if di + 1 < len(lines) and re.match(r'^\s+[\w ]+?\**\s*\w+(?:\[\w*\])*;$', lines[di + 1]):
        lines.insert(di + 1, decl)
    else:
        lines[di + 1:di + 1] = [decl, '']
    return '\n'.join(lines)""",
r"""    decl = '    u32 *cmd_cnt;'
    # step 84: declared right after the PC local, not first: the compiler hands the callee-saved registers out in
    # declaration order (r31 down) and the original's PC local is the register above the pointer's (r31 / r30 in the
    # pure handlers); the 40 DUMMY handlers were at 44% with the pointer declared first and the two registers swapped
    pcm = re.search(r'^[ \t]+(\w+) = cr_g_ScriptPC;$', body, re.M)
    pi = None
    if pcm:
        for i in range(di + 1, len(lines)):
            if not lines[i].strip():
                break
            if re.match(r'^\s+[\w ]+?\**\s*%s;$' % re.escape(pcm.group(1)), lines[i]):
                pi = i
                break
    if pi is not None:
        lines.insert(pi + 1, decl)
    elif di + 1 < len(lines) and re.match(r'^\s+[\w ]+?\**\s*\w+(?:\[\w*\])*;$', lines[di + 1]):
        lines.insert(di + 1, decl)
    else:
        lines[di + 1:di + 1] = [decl, '']
    return '\n'.join(lines)""", 'scp')
open(p, 'w', encoding='utf-8', newline='').write(s.replace('\n', nl))
print('ok', nl == '\r\n')
