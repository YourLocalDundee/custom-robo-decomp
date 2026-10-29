"""Structure-only view of the Ghidra export, for the name matchers (layout_evidence / match_lib_*.py).

The matchers take OUR names only from the symbol files (symbols/symbols.txt, descriptive.txt, candidates_*.txt) and
use the export only for structure: function starts, call / data references and string values, all by address.
So nothing here reads a name or a type Ghidra applied: the name columns of decompiled/xrefs/*.tsv are dropped, and
decompiled/index.db (built later in the run, with names, C text and game_map hints) is not used at all.
That keeps the matchers' result independent of which names the previous rebuild applied (tools/batches/run_all.ps1
iterates names -> rebuild -> names to a fixpoint).

  ok()          the export exists (decompiled/xrefs/{functions,refs,strings}.tsv)
  connect()     in-memory SQLite with refs(func, at, target, kind) and strings(addr, value), same row order as the TSVs
  functions()   [(addr, size)] in file order
"""
import os, sqlite3

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
X = os.path.join(G, 'decompiled', 'xrefs')
FILES = ('functions.tsv', 'refs.tsv', 'strings.tsv')


def ok():
    return all(os.path.exists(os.path.join(X, f)) for f in FILES)


def _rows(name):
    for l in open(os.path.join(X, name), encoding='utf-8'):
        yield l.rstrip('\n').split('\t')


def functions():
    return [(int(r[0], 16), int(r[2])) for r in _rows('functions.tsv') if len(r) >= 3]


def connect():
    con = sqlite3.connect(':memory:')
    con.executescript('CREATE TABLE refs(func INTEGER, at INTEGER, target INTEGER, kind TEXT);'
                      'CREATE TABLE strings(addr INTEGER PRIMARY KEY, value TEXT);')
    con.executemany('INSERT INTO refs VALUES(?,?,?,?)',
                    ((int(r[0], 16), int(r[1], 16), int(r[2], 16), r[3]) for r in _rows('refs.tsv') if len(r) >= 4))
    # value is the 3rd column; the label (2nd) is a Ghidra name and is not used
    con.executemany('INSERT OR REPLACE INTO strings VALUES(?,?)',
                    ((int(r[0], 16), r[2]) for r in _rows('strings.tsv') if len(r) >= 3))
    con.executescript('CREATE INDEX refs_t ON refs(target); CREATE INDEX refs_f ON refs(func);')
    return con
