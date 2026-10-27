"""Build a searchable index of the whole decompile.

Inputs (from tools/rebuild_decomp.ps1): decompiled/main_dol.c, decompiled/xrefs/{functions,refs,strings}.tsv,
symbols/game_map.tsv (hints, optional).
Outputs:
  decompiled/index.db         SQLite: funcs, refs, strings, plus full-text table funcs_fts over the C
  decompiled/funcs/<8xxx>/    one .c file per function, grouped by the top 16 address bits
query it with tools/find.py.  usage: python -I tools/build_index.py
"""
import os, re, shutil, sqlite3

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
D = os.path.join(G, 'decompiled')
HDR = re.compile(r'^// ([0-9a-f]{8})  (\S+)$')


def tsv(name):
    p = os.path.join(D, 'xrefs', name)
    return [l.rstrip('\n').split('\t') for l in open(p, encoding='utf-8')] if os.path.exists(p) else []


def main():
    bodies, cur, buf = {}, None, []
    for line in open(os.path.join(D, 'main_dol.c'), encoding='utf-8', errors='replace'):
        m = HDR.match(line.rstrip('\n'))
        if m:
            if cur: bodies[cur] = ''.join(buf)
            cur, buf = (int(m.group(1), 16), m.group(2)), []
        elif cur:
            buf.append(line)
    if cur: bodies[cur] = ''.join(buf)

    hints = {}
    gm = os.path.join(G, 'symbols', 'game_map.tsv')
    if os.path.exists(gm):
        for row in list(l.rstrip('\n').split('\t') for l in open(gm, encoding='utf-8'))[1:]:
            hints[int(row[0], 16)] = row[5]

    db = os.path.join(D, 'index.db')
    if os.path.exists(db): os.remove(db)
    con = sqlite3.connect(db)
    con.executescript('''
        CREATE TABLE funcs(addr INTEGER PRIMARY KEY, name TEXT, size INTEGER, hints TEXT, body TEXT);
        CREATE TABLE refs(func INTEGER, at INTEGER, target INTEGER, kind TEXT, sym TEXT);
        CREATE TABLE strings(addr INTEGER PRIMARY KEY, label TEXT, value TEXT);
        CREATE VIRTUAL TABLE funcs_fts USING fts5(name, body, content='funcs', content_rowid='addr',
                                                  tokenize="unicode61 tokenchars '_'");
    ''')
    size = {int(r[0], 16): int(r[2]) for r in tsv('functions.tsv') if len(r) == 3}
    con.executemany('INSERT INTO funcs VALUES(?,?,?,?,?)',
                    [(a, n, size.get(a), hints.get(a, ''), b) for (a, n), b in bodies.items()])
    con.execute("INSERT INTO funcs_fts(funcs_fts) VALUES('rebuild')")
    con.executemany('INSERT INTO refs VALUES(?,?,?,?,?)',
                    [(int(r[0], 16), int(r[1], 16), int(r[2], 16), r[3], r[4]) for r in tsv('refs.tsv') if len(r) == 5])
    con.executemany('INSERT OR REPLACE INTO strings VALUES(?,?,?)',
                    [(int(r[0], 16), r[1], r[2]) for r in tsv('strings.tsv') if len(r) == 3])
    con.executescript('CREATE INDEX refs_t ON refs(target); CREATE INDEX refs_f ON refs(func);'
                      'CREATE INDEX funcs_n ON funcs(name);')
    con.commit()

    out = os.path.join(D, 'funcs')
    shutil.rmtree(out, ignore_errors=True)
    for (a, n), b in bodies.items():
        sub = os.path.join(out, f'{a >> 16:04X}')
        os.makedirs(sub, exist_ok=True)
        with open(os.path.join(sub, f'{a:08X}_{n[:60]}.c'), 'w', encoding='utf-8') as f:
            f.write(f'// {a:08X}  {n}\n{b}')
    counts = [con.execute(f'SELECT count(*) FROM {t}').fetchone()[0] for t in ('funcs', 'refs', 'strings')]
    print(f'index: {counts[0]} functions, {counts[1]} references, {counts[2]} strings -> decompiled/index.db; '
          f'per-function files in decompiled/funcs/')


if __name__ == '__main__':
    main()
