"""Check the documentation: links, doc paths named in code, the docs index, line ends, per-OS command blocks.

Reads only; writes nothing, no network, no batch needed (works on a fresh clone without the game).
Checks, over the files git tracks (or a walk of the repo when git is not there):
  1. every relative Markdown link [text](path#anchor) and reference link [id]: path resolves to an existing file,
     and its #anchor to a heading of that file (GitHub's anchor rules); http(s) / mailto links are not fetched
  2. every docs/... path named anywhere in a tracked text file (.md .py .ps1 .sh .java .txt .patch .yml) exists
     (so a script that prints or mentions a doc never points at a moved one)
  3. docs/README.md (the index) links every file in docs/
  4. hand-written .md files use CRLF line ends (the generated ones are written by their tools and only link-checked)
  5. command blocks are for one OS: a ```bash block never runs pwsh or a .ps1, a ```powershell block never runs a .sh
Exit code 1 on any problem, each listed as  file:line: problem.
usage: python -I tools/check_docs.py          (Linux / macOS: python3 -I tools/check_docs.py)
"""
import os, re, subprocess, sys

G = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# written by tools, never by hand (docs/README.md lists them under "Generated"); line ends are the tool's business
GENERATED = {'docs/game_map.md', 'docs/name_evidence.md', 'docs/names_crosscheck.md', 'docs/struct_cr_Chara.md',
             'docs/expected_checksums.txt', 'docs/name_tables.tsv', 'docs/community/codes_gxce01.tsv'}
TEXT_EXT = ('.md', '.py', '.ps1', '.sh', '.java', '.txt', '.patch', '.yml', '.h', '.c')
LINK = re.compile(r'(?<!!)\[(?:[^\[\]]|\[[^\]]*\])*\]\(\s*<?([^)\s>]+)>?(?:\s+"[^"]*")?\s*\)')
IMAGE = re.compile(r'!\[[^\]]*\]\(\s*<?([^)\s>]+)>?\s*\)')
REFDEF = re.compile(r'^\s{0,3}\[[^\]]+\]:\s*<?(\S+?)>?(?:\s+.*)?$')
DOCPATH = re.compile(r'(?<![\w./\\-])docs[/\\][\w./\\-]*?\.(?:md|txt|tsv)\b')
FENCE = re.compile(r'^\s*(```+|~~~+)\s*([\w+-]*)')


def tracked():
    try:
        out = subprocess.run(['git', '-C', G, 'ls-files', '-z'], capture_output=True, check=True).stdout
        files = [f for f in out.decode('utf-8').split('\0') if f]
        if files: return sorted(f for f in files if os.path.isfile(os.path.join(G, f)))
    except (OSError, subprocess.CalledProcessError):
        pass
    skip = {'.git', 'Custom Robo extracted', 'unpacked', 'models_dat', 'models_export', 'previews', 'test_build',
            'decompiled', 'build', 'ghidra_project', 'mods', 'User', 'downloads', 'refs', 'melee', 'm2c', 'HSDLib',
            'pyenv', 'dolphin', 'ninja', 'out', 'orig', 'drafts', '__pycache__'}
    res = []
    for root, dirs, names in os.walk(G):
        dirs[:] = [d for d in dirs if d not in skip and not d.startswith('ghidra_')]
        res += [os.path.relpath(os.path.join(root, n), G).replace(os.sep, '/') for n in names]
    return sorted(res)


def slug(heading):
    """GitHub's anchor for a heading: markup and punctuation dropped, lower case, spaces to '-'."""
    h = re.sub(r'!?\[([^\]]*)\]\([^)]*\)', r'\1', heading)   # links -> their text
    h = re.sub(r'<[^>]+>', '', h).replace('`', '').replace('*', '').strip().lower()
    h = re.sub(r'[^\w\- ]', '', h, flags=re.UNICODE)
    return h.replace(' ', '-')


_anchors = {}


def anchors(path):
    if path not in _anchors:
        seen, out, fence = {}, set(), False
        for line in read(path).splitlines():
            if FENCE.match(line): fence = not fence; continue
            m = None if fence else re.match(r'^\s{0,3}#{1,6}\s+(.*?)\s*#*\s*$', line)
            if not m: continue
            s = slug(m.group(1))
            n = seen.get(s, 0); seen[s] = n + 1
            out.add(s if n == 0 else f'{s}-{n}')
        for a in re.findall(r'<a\s+(?:name|id)="([^"]+)"', read(path)): out.add(a)
        _anchors[path] = out
    return _anchors[path]


def read(path):
    with open(os.path.join(G, path), 'rb') as f:
        return f.read().decode('utf-8', errors='replace')


def strip_code_spans(line):
    return re.sub(r'(`+)(?:(?!\1).)+\1', lambda m: ' ' * len(m.group(0)), line)


def main():
    files = tracked()
    fileset = set(files)
    dirs = {os.path.dirname(f) for f in files}
    while True:   # every parent folder too, for links to a folder (docs/formats/)
        more = {os.path.dirname(d) for d in dirs if d} - dirs
        if not more: break
        dirs |= more
    problems = []

    def exists(rel):
        rel = os.path.normpath(rel).replace(os.sep, '/')
        return rel in fileset or rel.rstrip('/') in dirs or os.path.exists(os.path.join(G, rel))

    for f in files:
        if not f.endswith(TEXT_EXT): continue
        text = read(f)
        lines = text.split('\n')
        is_md = f.endswith('.md')
        fence, lang = None, ''
        for n, line in enumerate(lines, 1):
            line = line.rstrip('\r')
            # 2. docs/... paths named in any text file (code spans included: that is where scripts name them)
            for m in DOCPATH.finditer(line):
                p = m.group(0).replace('\\', '/').rstrip('.')
                if '*' in p or '<' in p: continue
                if not exists(p): problems.append(f'{f}:{n}: names {m.group(0)}, which does not exist')
            if not is_md: continue
            fm = FENCE.match(line)
            if fm:
                if fence is None: fence, lang = fm.group(1), fm.group(2).lower()
                elif line.strip().startswith(fence[0] * len(fence)): fence, lang = None, ''
                continue
            if fence is not None:
                # 5. one OS per command block
                code = line.split('#', 1)[0] if lang in ('bash', 'sh', 'shell') else line
                if lang in ('bash', 'sh', 'shell') and re.search(r'\bpwsh\b|\.ps1\b', code):
                    problems.append(f'{f}:{n}: PowerShell command in a bash block')
                if lang in ('powershell', 'pwsh', 'ps1') and re.search(r'\btools[/\\][\w/\\]*\.sh\b', line):
                    problems.append(f'{f}:{n}: bash script in a PowerShell block')
                continue
            # 1. links (outside code)
            plain = strip_code_spans(line)
            targets = [m.group(1) for m in LINK.finditer(plain)] + [m.group(1) for m in IMAGE.finditer(plain)]
            rm = REFDEF.match(plain)
            if rm: targets.append(rm.group(1))
            for t in targets:
                if re.match(r'^[a-z][a-z0-9+.-]*:', t, re.I): continue   # http:, https:, mailto:
                path, _, frag = t.partition('#')
                if path:
                    rel = os.path.normpath(os.path.join(os.path.dirname(f), path.replace('%20', ' '))).replace(os.sep, '/')
                    if rel.startswith('..'):
                        problems.append(f'{f}:{n}: link {t} leaves the repo'); continue
                    if not exists(rel):
                        problems.append(f'{f}:{n}: link {t}: {rel} does not exist'); continue
                else:
                    rel = f
                if frag and rel.endswith('.md') and rel in fileset and frag.lower() not in anchors(rel):
                    problems.append(f'{f}:{n}: link {t}: no heading #{frag} in {rel}')
        # 4. CRLF for hand-written Markdown
        if is_md and f not in GENERATED:
            raw = open(os.path.join(G, f), 'rb').read()
            lf = raw.count(b'\n') - raw.count(b'\r\n')
            if lf: problems.append(f'{f}:1: {lf} line(s) end in LF, not CRLF')

    # 3. the index lists every doc
    index = 'docs/README.md'
    if index in fileset:
        linked = set()
        for t in LINK.findall(strip_code_spans(read(index))):
            if not re.match(r'^[a-z]+:', t):
                linked.add(os.path.normpath(os.path.join('docs', t.partition('#')[0])).replace(os.sep, '/'))
        for f in files:
            if f.startswith('docs/') and f != index and f not in linked:
                problems.append(f'{index}:1: does not list {f}')

    for p in problems: print(p)
    print(f'check_docs: {len(problems)} problem(s) in {sum(f.endswith(".md") for f in files)} Markdown files'
          f' and {sum(f.endswith(TEXT_EXT) for f in files)} text files')
    return 1 if problems else 0


if __name__ == '__main__':
    sys.exit(main())
