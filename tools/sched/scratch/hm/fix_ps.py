P = r'D:\emulators\gameTesting-hsd2\tools\draft_batch.py'
s = open(P, encoding='utf-8', newline='').read()
nl = '\r\n' if '\r\n' in s else '\n'
old = ("            m = re.match(r'^(lbl_[0-9A-F]{8}) = \\.sdata2:0x([0-9A-F]+); // type:object size:0x([0-9A-F]+)\x08.*\x08data:string\x08', l)" + nl +
       "            if not m:" + nl +
       "                continue" + nl +
       "            b = read(int(m.group(2), 16), int(m.group(3), 16))" + nl +
       "            if b is None or 0 not in b:" + nl +
       "                continue" + nl)
assert old in s, 'old block not found'
new = ("            # dtk types most of them `data:string`; the Shift-JIS ones of fn_8002444C it leaves untyped (`size:0x8`), so an" + nl +
       "            # untyped .sdata2 object counts too when its bytes are a non-empty string followed only by zeros" + nl +
       "            m = re.match(r'^(lbl_[0-9A-F]{8}) = \\.sdata2:0x([0-9A-F]+); // type:object size:0x([0-9A-F]+)(.*)$', l)" + nl +
       "            if not m or ('data:' in m.group(4) and 'data:string' not in m.group(4)):" + nl +
       "                continue" + nl +
       "            b = read(int(m.group(2), 16), int(m.group(3), 16))" + nl +
       "            if b is None or 0 not in b or b[0] == 0 or any(b[b.index(0):]) or any(c < 0x20 for c in b[:b.index(0)]):" + nl +
       "                continue" + nl)
s = s.replace(old, new)
# the docstring's escapes also lost their backslashes
s = s.replace('`"\x96\xb3\x82\xb5"`', '`"\\x96\\xb3\\x82\\xb5"`')
assert '\x08' not in s[s.index('def pool_strings'):s.index('def pool_floats')]
open(P, 'w', encoding='utf-8', newline='').write(s)
print('fixed')
