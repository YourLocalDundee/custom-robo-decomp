"""apply the step 83 string-literal rule to tools/draft_batch.py (run once, after batch 12 has finished)"""
import re, sys
P = r'D:\emulators\gameTesting-hsd2\tools\draft_batch.py'
s = open(P, encoding='utf-8', newline='').read()
nl = '\r\n' if '\r\n' in s else '\n'
old_doc = '''    from an extern's (three stores of 0.0f load f0, f1, f0 with the second load moved up; three of an extern float f0,
    f0, f0: fn_80079C70, fn_8001CA84; step 69). The literal is the compiler's own pool constant (the file's .sdata2
    entry at that address: the compare masks the relocation); a variant, because the extern form matches elsewhere."""
    fl = pool_floats()
    if not fl:
        return body
'''.replace('\n', nl)
new_doc = '''    from an extern's (three stores of 0.0f load f0, f1, f0 with the second load moved up; three of an extern float f0,
    f0, f0: fn_80079C70, fn_8001CA84; step 69). The literal is the compiler's own pool constant (the file's .sdata2
    entry at that address: the compare masks the relocation); a variant, because the extern form matches elsewhere.
    Step 83: the same draft also writes the .sdata2 STRING literals it reads by name (`(s32) lbl_804504F8` ->
    `"\x96\xb3\x82\xb5"`, pool_strings): a file's constant pool holds its float literals and its short string
    literals in one sequence (first use order; `-str readonly` puts a literal of 8 bytes or less in .sdata2 as its own
    object: fn_8002444C's pool is 48, 56, 96, 88, "...", 192, "...", 72, "...", 160), so a draft that owns its floats
    must own the strings between them too, or the piece cannot be placed (promote_report.txt: "two piece starts").
    The code is the same either way (`li r3, X@sda21`); only the ownership changes, so this stays inside the variant."""
    fl = dict(pool_floats())
    fl.update(pool_strings())
    if not fl:
        return body
'''.replace('\n', nl)
assert old_doc in s, 'float_literals docstring not found'
s = s.replace(old_doc, new_doc)
old_pf = '''def pool_floats():'''
new_pf = '''def pool_strings():
    """{name: C string literal} of the string objects of .sdata2 (dtk's data:string objects of symbols.txt, the bytes
    from main.dol, every byte as a hex escape; only a string whose NUL lies inside the object), read once per process
    (step 83: the strings of a file's constant pool, see float_literals)"""
    if 'strings' not in W:
        out = {}
        try:
            read = dol_reader()
        except OSError:
            read = None
        for l in open(os.path.join(D, 'config/GXCE01/symbols.txt'), encoding='utf-8') if read else ():
            m = re.match(r'^(lbl_[0-9A-F]{8}) = \.sdata2:0x([0-9A-F]+); // type:object size:0x([0-9A-F]+)\b.*\bdata:string\b', l)
            if not m:
                continue
            b = read(int(m.group(2), 16), int(m.group(3), 16))
            if b is None or 0 not in b:
                continue
            out[m.group(1)] = '"' + ''.join('\\x%02x' % c for c in b[:b.index(0)]) + '"'
        W['strings'] = out
    return W['strings']


def pool_floats():'''
assert s.count(old_pf.replace('\n', nl)) == 1
s = s.replace(old_pf.replace('\n', nl), new_pf.replace('\n', nl))
open(P, 'w', encoding='utf-8', newline='').write(s)
print('patched')
