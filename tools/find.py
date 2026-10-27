"""Query the decompile index (decompiled/index.db, built by tools/build_index.py).

usage: python -I tools/find.py <command> <arg> [--limit N]
  text <query>        full-text search over function names and C (FTS5 syntax: chara_bin, "HSD_JObj*", a AND b)
  func <addr|name>    print a function's C (--brief hides local declarations)
  callers <addr|name> functions that call it
  callees <addr|name> functions it calls
  refs <addr>         every instruction that references an address (data or code), with the function
  str <text>          strings containing <text>, and the functions that use each one
  hint <hint>         game functions with a subsystem hint (model, anim, file-io, ...; see docs/game_map.md)
"""
import os, re, sqlite3, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
con = sqlite3.connect(os.path.join(G, 'decompiled', 'index.db'))
LIMIT = int(sys.argv[sys.argv.index('--limit') + 1]) if '--limit' in sys.argv else 50
LOCAL = re.compile(r'^\s+[A-Za-z_][\w ]*\**\s*\**\w+(\s*\[\w+\])?;$')


def resolve(x):
    try: return int(x, 16)
    except ValueError: pass
    r = con.execute('SELECT addr FROM funcs WHERE name=?', (x,)).fetchone()
    if not r: sys.exit(f'no function {x}')
    return r[0]


def name(a):
    r = con.execute('SELECT name FROM funcs WHERE addr=?', (a,)).fetchone()
    return r[0] if r else '?'


def main():
    if len(sys.argv) < 3: sys.exit(__doc__)
    cmd, arg = sys.argv[1], sys.argv[2]
    if cmd == 'text':
        for a, n, snip in con.execute(
                "SELECT rowid, name, snippet(funcs_fts, 1, '[', ']', ' ... ', 12) FROM funcs_fts "
                'WHERE funcs_fts MATCH ? ORDER BY rowid LIMIT ?', (arg, LIMIT)):
            print(f'{a:08X} {n:32} {" ".join(snip.split())}')
    elif cmd == 'func':
        a = resolve(arg)
        body = con.execute('SELECT body FROM funcs WHERE addr=?', (a,)).fetchone()[0]
        if '--brief' in sys.argv:  # hide local variable declarations and blank lines
            body = '\n'.join(l for l in body.split('\n') if l.strip() and not LOCAL.match(l))
        print(f'// {a:08X}  {name(a)}\n' + body)
    elif cmd == 'callers':
        # DATA refs to a function entry = the function is taken as a pointer (callback / function table)
        for f, n, k in con.execute("SELECT DISTINCT r.func, f.name, r.kind FROM refs r JOIN funcs f ON f.addr=r.func "
                                   "WHERE r.target=? AND (r.kind LIKE '%CALL%' OR r.kind='DATA') ORDER BY r.func LIMIT ?",
                                   (resolve(arg), LIMIT)):
            print(f'{f:08X} {n}' + ('  (takes its address: pointer / callback)' if k == 'DATA' else ''))
    elif cmd == 'callees':
        for t, n in con.execute("SELECT DISTINCT r.target, f.name FROM refs r JOIN funcs f ON f.addr=r.target "
                                "WHERE r.func=? AND r.kind LIKE '%CALL%' ORDER BY r.target LIMIT ?", (resolve(arg), LIMIT)):
            print(f'{t:08X} {n}')
    elif cmd == 'refs':
        for f, at, k, s in con.execute('SELECT func, at, kind, sym FROM refs WHERE target=? ORDER BY at LIMIT ?',
                                       (int(arg, 16), LIMIT)):
            print(f'{at:08X} in {f:08X} {name(f):32} {k} {s}')
    elif cmd == 'str':
        for a, lab, v in con.execute('SELECT addr, label, value FROM strings WHERE value LIKE ? ORDER BY addr LIMIT ?',
                                     (f'%{arg}%', LIMIT)):
            users = [f'{f:08X} {name(f)}' for (f,) in con.execute('SELECT DISTINCT func FROM refs WHERE target=?', (a,))]
            print(f'{a:08X} {v!r}  used by: {", ".join(users) or "(no direct reference)"}')
    elif cmd == 'hint':
        for a, n, h in con.execute('SELECT addr, name, hints FROM funcs WHERE hints LIKE ? ORDER BY addr LIMIT ?',
                                   (f'%{arg}%', LIMIT)):
            print(f'{a:08X} {n:32} {h}')
    else:
        sys.exit(__doc__)


if __name__ == '__main__':
    main()
