"""Generate decomp/config/GXCE01/splits.txt: object (.c file) boundaries in main.dol, only where they are proven.

CODE (.init / .text), library objects (Dolphin SDK, MSL / Runtime, MetroTRK, HSD sysdolphin):
  Every pinned reference decomp with a dtk config (tools/melee, tools/refs/*/config/*/{splits,symbols}.txt) gives, per
  object, the functions it contains in its own address order. For an object O of a reference R let F = R's functions
  in O. O's .text range in OUR main.dol is proven when:
    1. R's first and last function of O both have a real name in R (not fn_/lbl_), and that exact name is on exactly
       one function in our decomp/config/GXCE01/symbols.txt (our names are the cross-checked ones, gen_dtk_symbols.py);
    2. first <= last in our address order, in the same section;
    3. every NAMED function of ours from first to last is one of R's functions of O (unnamed fn_ ones may be in
       between: they are enclosed by O's own functions, so they belong to O), or one the pinned library sources
       define in O's very file (another revision of the file has it), or one that every reference placing it puts
       in a library file of the same name (another revision's function, or a header inline's out-of-line copy);
       in HSD objects also one that no reference places in any file and no pinned source defines (it counts as the
       unnamed function it was: step 61 iteration 3, HSD_MObjGetFlags, a name only Pokemon XD's map has);
    4. both ends are proven. An end is proven by the section edge, or when the named function next to it is NOT one
       of O's functions in R but another object's (else the object would extend further), or when it is unnamed
       and our end function is R's own first / last function of O. Unnamed functions right next to the range whose
       assert passes O's file name (tools/match_game_names.py's G3 notes) are O's and extend it; an unnamed
       neighbour whose assert names ANOTHER file proves the end. A named neighbour this reference does not have, and
       that no reference or pinned source puts in O's file, counts as an unnamed one (naming it loses no proof; HSD
       objects only, step 61: elsewhere it would change two SDK / MSL ranges, critical_regions and SISamplingRate,
       which are not reviewed under this rule yet).
       When unnamed functions sit between our end function and the next named function P (in another object of R), the
       end is still proven
         - by a size run: R has exactly as many functions between the two, with our sizes one to one (the same
           code), so they are R's functions and belong to R's objects;
         - by the per-file pools: every source file has its own .sdata2 constant pool and string literals (dtk's
           data:string symbols) and those pools are laid out in link (= code) order, so an unnamed function using a
           pool entry at or below the highest of P's side, or at or above the lowest of O's own range, in the same
           section, is in that file (entries used outside this stretch of code are not counted); proven when they
           read P's side first, then O, with no unattributed function at the boundary (method: pool_edge);
    5. our function count from first to last is within max(2, 25%) of R's count for O (catches a wrong anchor). A count
       BELOW that is accepted only when BOTH ends are proven by named neighbours that are another object's (rule 4's
       "the named function next to it is NOT one of O's functions in R but another object's"; not by the section
       edge, an assert, a size run or the pools): two named foreign neighbours fix the range whatever it holds, and
       a newer revision of a file can have fewer functions (sysdolphin in Custom Robo = Kirby Air Ride's revision
       inlines the *GetAllocData / flag accessors). A count above it is still rejected. The report marks these
       objects ("fewer functions than the reference, both edges named").
  The range is [first's address, start of the next function / symbol after last) - alignment padding after an
  object's last function belongs to that object, as the linker puts it there.
  Agreement: several references can prove the same object. When their ranges differ (different SDK / MSL / TRK
  revisions, or a decomp's own guess), each section is settled
    1. BY SOURCE: our named functions inside some of the candidate ranges but not all are looked up in the pinned
       library sources (dolsdk2001, dolsdk2004, Melee's MSL / Runtime / MetroTRK / sysdolphin, Mario Party 4's SDK /
       MSL / Runtime / TRK): which file DEFINES each one (a body, not a prototype; the object's own library first,
       else any). The one range whose membership matches for every such function (inside iff defined in this file)
       wins;
    2. else BY REVISION: only the references whose function sizes for O equal ours one to one count;
  else the object is dropped (reported with every reference's sizes). Overlapping proven ranges: when only one of
  the two has a reference with our function sizes (for the object, or for the functions both ranges hold), the
  other one's section is dropped; else both are dropped (reported).
  Two files with the generic name main.c (MetroTRK's ddh/ and gdev/ drivers) are told apart by that folder.
  The wowjinxy KAR map (refs/kar_wowjinxy, a map-derived config: bare HSD file names such as jobj.c, and whole SDK
  libraries as one file, dolphin/os.c) counts only when another reference agrees: a range it alone proves is kept
  only if doldecomp/kar (refs/kar: asm/sysdolphin/<file>.s + src/sysdolphin/<file>.c, the same GKYE01 DOL) has that
  file with the same first and last function address and the same names on both anchor functions (the naming rule:
  a wowjinxy name counts only when another reference agrees). Reference file names that are addresses (Melee's
  hsd_3910.c, ..._8004a648.c) are placeholders, never used.
  The object's path is Melee's path for a file of the same name (lower-case match), else the first reference's file
  name in Melee's folder layout (dolphin/<lib>/, MSL/, Runtime/, MetroTRK/, sysdolphin/baselib/).

DATA (.ctors .dtors .rodata .data .bss .sdata .sbss .sdata2 .sbss2), per proven library object:
  main.dol is split with sections only and linked once in a scratch directory (decomp/build/splitdata/: dtk dol
  split + the project's mwld, SHA-1 must be retail; dtk relocates by address, so the relocations do not depend on
  the splits; the code rule above uses the same link) and tools/reloc_audit.py reads every relocation of that main.elf (absolute addresses)
  plus the constant-offset accesses (CROSS: CodeWarrior addresses a file's statics from one base). Each data symbol
  (dtk's sized objects) gets its USERS: the code units (split objects, and every unsplit code range as one unit)
  holding an access to it; a symbol only other data points to inherits the users of those data symbols. The linker
  lays out every section in link order (= code order), one contiguous piece per object, so O's piece of a section is
  proven when ALL of this holds:
    1. core: at least one symbol whose only user is O; from the first to the last core symbol no symbol has users
       without O (a foreign symbol inside = not contiguous, or a global of another file: dropped);
    2. link order: the nearest symbol before the piece that has users is used only by units before O, the nearest
       after it only by units after O (or the section edge);
    3. the symbols between those neighbours and the core (unreferenced, or also used by others) are the only
       uncertainty: every start / end on a symbol boundary there is a candidate, and no candidate boundary may cut a
       constant-offset span (an anchor and the address made from it on different sides);
    4. the references agree: the references that proved O's code give O's piece of that section a size (its
       splits.txt range); those whose O has the same .text size as ours (the same revision) decide, else all of them;
       a same-revision reference with NO piece in that section rejects it; exactly one candidate range must have one
       of these sizes.
  Accepted pieces must be in code order in every section (checked; dtk checks it again). .ctors / .dtors /
  .sbss2 follow the same rule. Everything not proven stays in dtk's automatic objects (listed in the report).

DATA FROM THE MATCHED C (Phase 4 Milestone 4, step 56): a C file that matches its object proves that object's data.
  The code objects are written to splits.txt and decomp/configure.py + `ninja all_source` compile every decomp/src
  file (a C compile depends on its source only). For every C object:
    1. its functions are found in main.dol: by our name inside the object's code range, else at the one function start
       of ours (no other real name, same size) where its bytes fit; a function counts when its bytes equal main.dol's
       with every relocated field masked, main.elf (the scratch link above) relocates every such site with the same
       type, and its calls to named functions / our names go where those are. Every address is taken once.
    2. only an object whose every retail function is one of these (a whole match; another revision can have data the
       C lacks) proves data. Its data symbols that a verified function uses get their address from main.elf's
       relocation at that site (the C's offset taken off); its globals also from other C files' verified uses by
       name; a pointer in a placed symbol gives the address of what it points to; a run with no use is placed where
       its bytes and pointers fit, once in the section. mwld strips unused data: a symbol is there when a verified
       function or placed data uses it (a use of a section's -str pool keeps the whole pool; .ctors / .dtors are
       kept whole); runs with nothing stripped between keep the .o distances; a global (another file may use it) next
       to the placed ones is there only with its own bytes exactly there, else it must be shown absent (its bytes are
       not next to the piece, or the neighbour is another object's piece); between two runs exactly as far apart as
       in the .o, a global main.elf uses is at its offset.
    3. every placed symbol's bytes equal main.dol's, every pointer in it is main.elf's relocation to its target; the
       piece starts aligned and on no dtk symbol's inside, and ends at the C's last byte. THE TRAILING-PADDING RULE
       (step 61 iteration 8): when only zeros follow up to another object's piece that starts at the first address
       after it aligned to that object's alignment (dtk's: the largest align: of its symbols, else 8; from C, also its
       .o section's), the piece ends there and the padding is that object's alignment (the linker makes it, dtk makes
       no automatic object of it). Else the zeros go with it up to another object's piece / the section end (as dtk
       splits), or, before an automatic object, start that object from the next word on (an automatic object must
       start 4-aligned). A dtk symbol that runs on over the C's last byte is cut where the piece ends (symbols.txt,
       trim_symbols: a split may not end inside a symbol). A C piece replaces the relocation users' piece of the same
       object; another object's piece it overlaps is dropped.
  Report: the whole matches (with the functions our names call otherwise), the pieces, the global symbols our names
  call otherwise (an object linked from C needs every name another object uses), and every C definition ("DEF"
  lines: rule R8, the // c-define lines of symbols/symbols.txt are checked against them by check_c_callsites.py).

GAME code: no reference exists. Two ways a game object is split:
  1. FROM ITS C FILE (plan.md step 58): every file decomp/src/game/<path>.c is an object game/<path>.c. Its range is
     taken from the functions it defines (a definition = a line at column 0 that ends in "{" and names the
     function before its "("): each must be one function of ours (decomp/config/GXCE01/symbols.txt) in the game code
     (before the first library object), they must be consecutive functions of ours, and the range is [first's
     address, the start of the next function / symbol after the last) - the padding after it belongs to it. These
     are tools/promote_drafts.py's files: one function each (a matching m2c draft, no data of their own), or a FILE
     OBJECT (step 63): the consecutive functions that share a constant pool (per-file pools, above) in one file with
     its own .sdata2 constants / string literals; game_file_data places that data (every relocation of the object
     into its own data gives one piece start in main.elf, the bytes are main.dol's, no relocation from outside the
     object's code goes into the piece, the zeros up to the next dtk symbol go with it). A file that does not resolve
     stops the run. Configure.py links every game object from C (cr_matching).
  2. by tools/match_game_names.py's G3 module notes (assert .c names), which say which file a function is in, but
     not where a file starts or ends: only where both ends are proven: its first and last function have a G3 note
     for it, and the functions right outside have G3 notes for OTHER files (on the current data this proves none);
     a G3 object that overlaps a C file's object is dropped.
  Everything else of the game code stays in dtk automatic objects (listed in the report).

Output is byte-identical on every run (LF, as dtk writes it; the batch checks dtk leaves it unchanged).
Report: decomp/reports/splits_report.txt (every object with its proof, every rejected candidate, unproven ranges).
Usage: python -I tools/gen_dtk_splits.py [--no-data] [--no-c]   (--no-data: code only, no scratch link; --no-c: no data from C)
"""
import bisect
import json
import os
import re
import shutil
import sys
from collections import defaultdict
from pathlib import Path

G = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(G / "tools"))
OURS = G / "decomp/config/GXCE01/symbols.txt"
BASE = G / "tools/dtk/proj/splits.txt"  # the Sections: header (dtk's)
OUT = G / "decomp/config/GXCE01/splits.txt"
REPORT = G / "decomp/reports/splits_report.txt"
GAME_REAL = G / "symbols/game_real.txt"
KAR = G / "tools/refs/kar"
SCRATCH = G / "decomp/build/splitdata"
WOW = "GKYE01"
# reference configs, in path-preference order after Melee
REFS = [
    "melee/config/GALE01",
    "refs/tww/config/GZLE01", "refs/pikmin2/config/GPVE01", "refs/mkdd/config/MarioClub_us",
    "refs/sms/config/GMSE01", "refs/tp/config/GZ2E01", "refs/kar_wowjinxy/config/GKYE01",
    "refs/xd/config/GXXE01", "refs/ac-decomp/config/GAFE01_00", "refs/marioparty4/config/GMPE01_00",
    "refs/pikmin/config/GPIE01_01", "refs/bfbb/config/GQPE78", "refs/ttyd/config/G8MJ01",
]
LIB_DIRS = ("dolphin", "sysdolphin", "MSL", "Runtime", "MetroTRK", "TRK_MINNOW_DOLPHIN", "Dolphin", "PowerPC_EABI_Support",
            "TRK", "libc", "musyx", "SDK", "dolsdk", "amcstubs", "odenotstub", "OdemuExi2", "debugger", "Runtime.PPCEABI.H",
            "MSL_C", "MSL_C.PPCEABI.bare.H", "MetroTRK", "os", "dvd", "gx", "vi", "pad", "si", "exi", "card", "ax",
            "ai", "ar", "dsp", "mtx", "gba", "thp", "hio", "db", "base", "mcc", "axfx", "OdemuExi", "libs")
SYM = re.compile(r"^\s*([^\s=]+)\s*=\s*(\.[\w.]+):0x([0-9A-Fa-f]+);\s*//(.*)$")
DEFAULT = re.compile(r"^(fn|lbl|func|FUN|zz|jumptable|gap|switch)_[0-9A-Fa-f_]+|^@|^\.L|^\$")
CODE = (".init", ".text")
DATA = (".ctors", ".dtors", ".rodata", ".data", ".bss", ".sdata", ".sbss", ".sdata2", ".sbss2")
ADDRNAME = re.compile(r"_[0-9A-Fa-f]{8}$|^hsd_[0-9A-Fa-f]{8}")  # a reference's address-based placeholder names
PLACEHOLDER_FILE = re.compile(r"^hsd_[0-9A-Fa-f]{3,8}$|_[0-9A-Fa-f]{8}$|^text_|^data$|^auto_", re.I)


def family(path: str) -> str:
    p = path.lower()
    if p.startswith("sysdolphin/"):
        return "hsd"
    if "trk" in p.split("/")[0] or "metrotrk" in p or "trk_minnow" in p:
        return "trk"
    if "msl" in p or "runtime" in p or "powerpc_eabi_support" in p or p.startswith("libc/"):
        return "msl"
    return "sdk"


def trk_driver(path: str):
    """MetroTRK's two main.c files: the ddh / gdev driver folder they are in"""
    p = path.lower()
    return "ddh" if "ddh" in p else "gdev" if "gdev" in p else None


def objkey(path: str):
    # the same file in different references: same library family and file name (.c / .cpp / .s alike)
    stem = Path(path).stem.lower()
    if stem == "main" and family(path) == "trk":
        return ("trk", "main", trk_driver(path))
    return (family(path), stem)


SDK_LIBS = {"ai", "amcstubs", "ar", "ax", "axfx", "base", "card", "db", "dsp", "dvd", "exi", "gx", "hio", "mcc", "mtx",
            "odenotstub", "odemuexi2", "os", "pad", "si", "thp", "vi", "gba"}


def canonical(path: str) -> str:
    # a file Melee does not have: Melee's folder layout (dolphin/<lib>/, MSL/, Runtime/, MetroTRK/,
    # sysdolphin/baselib/), the file name as the reference writes it
    parts = path.split("/")
    f, fam = parts[-1], family(path)
    if fam == "hsd":
        return "sysdolphin/baselib/" + f
    if fam == "trk":
        d = trk_driver(path) if Path(f).stem.lower() == "main" else None
        return f"MetroTRK/{d}/{f}" if d else "MetroTRK/" + f
    if fam == "msl":
        return ("Runtime/" if "runtime" in path.lower() else "MSL/") + f
    lib = next((x for x in reversed(parts[:-1]) if x.lower() in SDK_LIBS), None)
    return f"dolphin/{lib}/{f}" if lib else f"dolphin/{f}"


def read_syms(path: Path):
    out = []
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        m = SYM.match(line)
        if not m:
            continue
        c = m.group(4)
        sz = re.search(r"size:0x([0-9A-Fa-f]+)", c)
        out.append((m.group(1), m.group(2), int(m.group(3), 16), int(sz.group(1), 16) if sz else 0,
                    "type:function" in c))
    return out


def read_splits(path: Path):
    objs = []  # (name, [(section, start, end)])
    cur = None
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        if not line.strip() or line.startswith("Sections:") or line.lstrip().startswith("//"):
            if line.startswith("Sections:"):
                cur = None
            continue
        if not line[0].isspace():
            cur = (line.split(":")[0].strip(), [])
            objs.append(cur)
            continue
        if cur is None:
            continue
        m = re.match(r"\s+(\S+)\s+start:0x([0-9A-Fa-f]+)\s+end:0x([0-9A-Fa-f]+)", line)
        if m:
            cur[1].append((m.group(1), int(m.group(2), 16), int(m.group(3), 16)))
    return objs


def hsd_stems():
    """HSD (sysdolphin) file names known from a reference that names its folder: Melee's splits + doldecomp/kar"""
    st = set()
    for name, _ in read_splits(G / "tools/melee/config/GALE01/splits.txt"):
        if name.startswith("sysdolphin/"):
            st.add(Path(name).stem.lower())
    for sub in ("asm/sysdolphin", "src/sysdolphin"):
        d = KAR / sub
        if d.exists():
            st.update(p.stem.lower() for p in d.iterdir() if p.suffix in (".s", ".c"))
    return {s for s in st if not PLACEHOLDER_FILE.search(s)}


def lib_path(ref: str, name: str, hsd: set):
    """the reference's object name if it is a library file we can use, else None"""
    stem = Path(name).stem
    if PLACEHOLDER_FILE.search(stem) or not re.search(r"\.(c|cpp|cp|s)$", name):
        return None
    if "/" not in name:
        # the wowjinxy KAR map writes HSD files without a folder (jobj.c); only names another reference has
        return "sysdolphin/baselib/" + name if ref == WOW and stem.lower() in hsd else None
    if name.split("/")[0] not in LIB_DIRS:
        return None
    if ref == WOW and name.startswith("dolphin/") and name.count("/") == 1 and stem.lower() in SDK_LIBS:
        return None  # wowjinxy's dolphin/os.c etc.: a whole SDK library as one file, not a source file
    return name


def kar_objects(wow_syms):
    """doldecomp/kar HSD files -> {stem: (first function address, end of last function, {address: name})}.
    asm/sysdolphin/<f>.s: function labels and instruction addresses; src/sysdolphin/<f>.c: the function names its C
    defines, placed by their func_<address> name or by the same name in the wowjinxy symbols (the same GKYE01 DOL)."""
    wf = defaultdict(list)
    for s in wow_syms:
        if s[4]:
            wf[s[0]].append(s)
    out = {}
    pieces = defaultdict(list)  # stem -> [(start, end, {addr: name})]
    d = KAR / "asm/sysdolphin"
    if d.exists():
        for p in sorted(d.glob("*.s")):
            names, first, last, sec, pend = {}, None, None, None, []
            for line in p.read_text(encoding="utf-8", errors="replace").splitlines():
                if line.startswith(".section"):
                    sec = line.split()[1].rstrip(",")
                    continue
                if sec != ".text":
                    continue
                m = re.match(r"^([A-Za-z_][\w$@.]*):\s*$", line)
                if m and not m.group(1).startswith(("lbl_", "@", ".L")):
                    pend.append(m.group(1))
                    continue
                m = re.match(r"^/\* ([0-9A-F]{8}) [0-9A-F]{8}\s", line)
                if m:
                    a = int(m.group(1), 16)
                    for n in pend:
                        names[a] = n
                    pend = []
                    first = a if first is None else first
                    last = a + 4
            if first is not None:
                pieces[p.stem.lower()].append((first, last, names))
    d = KAR / "src/sysdolphin"
    if d.exists():
        for p in sorted(d.glob("*.c")):
            names = {}
            for line in p.read_text(encoding="utf-8", errors="replace").splitlines():
                m = re.match(r"^(?!static\s+inline|inline|typedef|extern|#|//)[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\([^;]*\)\s*\{?\s*$", line)
                if not m or m.group(1) in ("if", "while", "for", "switch", "return", "sizeof"):
                    continue
                n = m.group(1)
                a = re.match(r"^func_([0-9A-Fa-f]{8})$", n)
                if a:
                    hit = [s for s in wf.get(n, [])] or [("", ".text", int(a.group(1), 16), 0, True)]
                else:
                    hit = wf.get(n, [])
                if len(hit) == 1:
                    names[hit[0][2]] = n
            if names:
                ends = []
                for a in names:
                    sz = next((s[3] for s in wf.get(names[a], []) if s[2] == a), 0)
                    if not sz:
                        sz = next((s[3] for lst in wf.values() for s in lst if s[2] == a), 0)
                    ends.append(a + sz)
                pieces[p.stem.lower()].append((min(names), max(ends), names))
    allp = sorted((a, b, st) for st, lst in pieces.items() for a, b, _ in lst)
    for st, lst in pieces.items():
        a, b = min(x[0] for x in lst), max(x[1] for x in lst)
        if any(x[2] != st and x[0] < b and x[1] > a for x in allp):
            continue  # another file's piece inside: not one contiguous file
        nm = {}
        for x in lst:
            nm.update(x[2])
        out[st] = (a, b, nm)
    return out


# ---------------------------------------------------------------------------------------------------- code
def code_splits(ours, rejected, notes, audit=None):
    defs = source_definers()   # which pinned library source file defines each name
    funcs = sorted([s for s in ours if s[4] and s[1] in CODE], key=lambda s: s[2])
    cby_f, cby_c = pool_refs(audit, funcs) if audit else ({}, {})
    pos = {}  # name -> index, only for names on exactly one function
    seen = defaultdict(int)
    for s in funcs:
        seen[s[0]] += 1
    for i, s in enumerate(funcs):
        if seen[s[0]] == 1 and not DEFAULT.match(s[0]):
            pos[s[0]] = i
    starts = defaultdict(list)
    for s in ours:
        starts[s[1]].append(s[2])
    for k in starts:
        starts[k] = sorted(set(starts[k]))

    def next_start(sec, addr):
        lst = starts[sec]
        j = bisect.bisect_right(lst, addr)
        return lst[j] if j < len(lst) else None

    hsd = hsd_stems()
    placed = placed_in(hsd)  # function name -> the library files (objkeys) the references place it in
    g3 = {a: Path(f).stem.lower() for a, f in read_g3().items()}  # function -> the .c file its assert names
    melee_paths = {}
    refinfo = {}  # ref tag -> (symbols, {object name: [(sec, start, end)]})
    cands = defaultdict(list)
    wow_syms = []
    for ref in REFS:
        d = G / "tools" / ref
        tagr = ref.split('/')[-1]
        if not (d / "splits.txt").exists() or not (d / "symbols.txt").exists():
            rejected.append(f"reference {ref}: no config (not fetched?)")
            continue
        rsyms = read_syms(d / "symbols.txt")
        if tagr == WOW:
            wow_syms = rsyms
        rf = sorted([s for s in rsyms if s[4]], key=lambda s: s[2])
        robjs = read_splits(d / "splits.txt")
        refinfo[tagr] = (rsyms, {name: secs for name, secs in robjs})
        where = defaultdict(set)
        members = {}
        for name, secs in robjs:
            for sec, a, b in secs:
                if sec in CODE:
                    F = [s for s in rf if a <= s[2] < b and s[1] == sec]
                    members[(name, sec)] = F
                    for s in F:
                        where[s[0]].add(name)
        for name, secs in robjs:
            path = lib_path(tagr, name, hsd)
            if path is None:
                continue
            if ref.startswith("melee/"):
                melee_paths[objkey(name)] = name
            for sec, a, b in secs:
                if sec not in CODE:
                    continue
                F = members[(name, sec)]
                if not F:
                    continue
                tag = f"{tagr}:{name} {sec}"
                names = {s[0] for s in F if not DEFAULT.match(s[0]) and not ADDRNAME.search(s[0]) and len(where[s[0]]) == 1}
                hit = sorted(pos[n] for n in names if n in pos and funcs[pos[n]][1] == sec)
                if not hit:
                    rejected.append(f"{tag}: none of its functions is (uniquely) named in ours")
                    continue
                i, j = hit[0], hit[-1]
                # (a named function R's file lacks is still allowed when the pinned library sources define it in this
                # very file: another revision of the file has it; or when every reference that places it puts it in
                # a library file of this name: another revision's function, or a header inline's out-of-line copy)
                bad = [funcs[k][0] for k in range(i, j + 1) if not DEFAULT.match(funcs[k][0]) and funcs[k][0] not in names
                       and verdict(defs, funcs[k][0], family(path)) != Path(name).stem.lower()
                       and placed.get(funcs[k][0]) != {objkey(path)}
                       and not (family(path) == "hsd" and not placed.get(funcs[k][0]) and not defs.get(funcs[k][0]))]
                # (HSD only, step 61 iteration 3: a name between two of R's functions of O that no reference places in
                # any file and no pinned source defines carries no file of its own: it counts as the unnamed function
                # it was, enclosed by O's functions - as rule 4 does for such a neighbour)
                if bad:
                    rejected.append(f"{tag}: our range {funcs[i][0]} .. {funcs[j][0]} contains other functions: {', '.join(bad[:4])}")
                    continue

                # unnamed functions next to the range that assert with this file's name (G3 notes) are this file's
                stem = Path(name).stem.lower()
                ext = 0
                while i > 0 and funcs[i - 1][1] == sec and DEFAULT.match(funcs[i - 1][0]) and g3.get(funcs[i - 1][2]) == stem:
                    i -= 1
                    ext += 1
                while j + 1 < len(funcs) and funcs[j + 1][1] == sec and DEFAULT.match(funcs[j + 1][0]) and g3.get(funcs[j + 1][2]) == stem:
                    j += 1
                    ext += 1
                rf_sec = [s for s in rf if s[1] == sec]
                rcount = defaultdict(int)
                for s in rf_sec:
                    rcount[s[0]] += 1
                ridx = {s[0]: t for t, s in enumerate(rf_sec) if rcount[s[0]] == 1 and not DEFAULT.match(s[0])}
                Fset = {s[2] for s in F}

                def size_run(i, j, back):
                    """an edge on our unnamed functions: between our end function A and the next named function P
                    the reference has exactly as many functions, with our sizes one to one (the same code), and P is
                    in another of its objects -> those functions are the reference's, in its objects"""
                    a = i if back else j
                    k, run = a, []
                    while True:
                        k += -1 if back else 1
                        if not (0 <= k < len(funcs)) or funcs[k][1] != sec or not DEFAULT.match(funcs[k][0]):
                            break
                        run.append(k)
                    if not run or not (0 <= k < len(funcs)) or funcs[k][1] != sec:
                        return None
                    P = funcs[k][0]
                    if not where.get(P) or name in where[P] or P not in ridx or funcs[a][0] not in ridx:
                        return None
                    ip, ia = ridx[P], ridx[funcs[a][0]]
                    seq = rf_sec[ip + 1:ia] if back else rf_sec[ia + 1:ip]
                    run.sort()
                    if len(seq) != len(run) or any(seq[t][3] != funcs[run[t]][3] for t in range(len(seq))):
                        return None
                    mem = [t for t in range(len(seq)) if seq[t][2] in Fset]
                    if back:
                        if mem and mem != list(range(len(seq) - len(mem), len(seq))):
                            return None
                        new = run[mem[0]] if mem else a
                    else:
                        if mem and mem != list(range(len(mem))):
                            return None
                        new = run[mem[-1]] if mem else a
                    return new, (f"size run to {P}: our {len(run)} unnamed functions have the reference's sizes one to one, "
                                 f"{len(mem)} of them in this object")

                def pool_edge(i, j, back):
                    """an edge on our unnamed functions by the per-file pools (.sdata2 constants, string literals): each
                    source file has its own pool in every such section and the pools are laid out in link (= code)
                    order. Between our end function and the next named function P (in another object X of the
                    reference) every unnamed function u is attributed: a pool entry of u at or below the highest entry
                    of X's side (P and X's other named functions next to it, with what they enclose), or at or above
                    the lowest of this object's (its range from the first to the last anchor), in the same section,
                    puts u in that file (u lies between the two in code order, so its pool lies between theirs). Only
                    entries used by functions inside this stretch of code count (an entry used elsewhere would be a
                    global). The edge is proven when, read from P towards this object, the attributed functions are X
                    first and this object after, with no unattributed function between the last X and the first one
                    of this object."""
                    a = i if back else j
                    k, run = a, []
                    while True:
                        k += -1 if back else 1
                        if not (0 <= k < len(funcs)) or funcs[k][1] != sec or not DEFAULT.match(funcs[k][0]):
                            break
                        run.append(k)
                    if not run or not (0 <= k < len(funcs)) or funcs[k][1] != sec:
                        return None
                    P = funcs[k][0]
                    if not where.get(P) or name in where[P] or len(where[P]) != 1:
                        return None
                    X = next(iter(where[P]))
                    # X's block in ours: from P away from this object while the named functions are X's
                    xk, t = k, k
                    while True:
                        t += -1 if back else 1
                        if not (0 <= t < len(funcs)) or funcs[t][1] != sec:
                            break
                        if DEFAULT.match(funcs[t][0]):
                            continue
                        if where.get(funcs[t][0]) != {X}:
                            break
                        xk = t
                    xs = range(xk, k + 1) if back else range(k, xk + 1)
                    lo_k, hi_k = (xk, j) if back else (i, xk)

                    def local(f):
                        return {c for c in cby_f.get(f, ()) if all(lo_k <= g <= hi_k for g in cby_c[c])}
                    mine, theirs = defaultdict(list), defaultdict(list)
                    for f in range(i, j + 1):
                        for sc, c in local(f):
                            mine[sc].append(c)
                    for f in xs:
                        for sc, c in local(f):
                            theirs[sc].append(c)
                    for sc in set(mine) & set(theirs):
                        if (back and max(theirs[sc]) >= min(mine[sc])) or (not back and max(mine[sc]) >= min(theirs[sc])):
                            return None
                    run.sort()
                    lab = []
                    for f in run:
                        o = x = False
                        for sc, c in local(f):
                            if back:
                                x |= bool(theirs[sc]) and c <= max(theirs[sc])
                                o |= bool(mine[sc]) and c >= min(mine[sc])
                            else:
                                o |= bool(mine[sc]) and c <= max(mine[sc])
                                x |= bool(theirs[sc]) and c >= min(theirs[sc])
                        if x and o:
                            return None
                        lab.append("O" if o else "X" if x else "?")
                    seq = "".join(lab) if back else "".join(reversed(lab))   # read from P towards this object
                    m = re.fullmatch(r"([X?]*X|)(O[O?]*|)", seq)
                    if not m or ("X" not in seq and "O" not in seq):
                        return None
                    nx, no = len(m.group(1)), len(m.group(2))
                    if not no and seq and seq[-1] != "X" or no and nx == 0 and seq[0] != "O":
                        return None   # an unattributed function next to this object / next to P: the edge is open
                    if back:
                        new = run[len(run) - no] if no else a
                    else:
                        new = run[no - 1] if no else a
                    return new, (f"pools to {P}: of our {len(run)} unnamed functions {no} use this object's pools, "
                                 f"{nx} {X}'s side's")

                def edge(k, ref_end):
                    if k < 0 or k >= len(funcs) or funcs[k][1] != sec:
                        return "section edge"
                    nb = funcs[k][0]
                    if DEFAULT.match(nb) and g3.get(funcs[k][2]) not in (None, stem):
                        return f"{nb} asserts with {g3[funcs[k][2]]}.c"
                    if DEFAULT.match(nb):
                        return f"reference's own {'first' if ref_end is F[0] else 'last'} function" if funcs[i if ref_end is F[0] else j][0] == ref_end[0] else None
                    if where.get(nb) and name not in where[nb]:
                        return f"{nb} is in {sorted(where[nb])[0]}"
                    if family(path) == "hsd" and not where.get(nb) and objkey(path) not in placed.get(nb, ()) \
                            and verdict(defs, nb, family(path)) != Path(name).stem.lower():
                        # a name this reference does not have (a newer revision's function), that no reference and
                        # no pinned source puts in this file: it counts as an unnamed neighbour (as before it was named)
                        own = funcs[i if ref_end is F[0] else j][0] == ref_end[0]
                        return (f"reference's own {'first' if ref_end is F[0] else 'last'} function ({nb} is not in "
                                f"this reference)") if own else None
                    return None
                e1, e2 = edge(i - 1, F[0]), edge(j + 1, F[-1])
                if not e1:
                    r = size_run(i, j, True) or pool_edge(i, j, True)
                    if r:
                        i, e1 = r
                if not e2:
                    r = size_run(i, j, False) or pool_edge(i, j, False)
                    if r:
                        j, e2 = r
                if not e1 or not e2:
                    rejected.append(f"{tag}: {'start' if not e1 else 'end'} not proven ({funcs[i][0]} .. {funcs[j][0]}; "
                                    f"neighbour {funcs[i - 1][0] if not e1 else funcs[j + 1][0]})")
                    continue
                n = j - i + 1
                tol = max(2, len(F) // 4)
                # rule 5: a count below the reference's is accepted when both edges are named neighbours of other
                # objects (rule 4's "named function next to it is another object's"), not pools / size runs / asserts
                named = all(e and " is in " in e and not e.startswith(("size run", "pools to")) for e in (e1, e2))
                fewer = n < len(F) - tol
                if n > len(F) + tol or (fewer and not named):
                    rejected.append(f"{tag}: {n} functions in ours vs {len(F)} in the reference"
                                    + (f" (fewer, and an edge is not a named neighbour of another object: {e1} | {e2})" if fewer else ""))
                    continue
                start = funcs[i][2]
                end = next_start(funcs[j][1], funcs[j][2]) or (funcs[j][2] + funcs[j][3])
                exact = [s[3] for s in F] == [funcs[k][3] for k in range(i, j + 1)]
                cands[objkey(path)].append(dict(ref=tagr, name=name, path=path, sec=sec, start=start, end=end, n=n,
                                                nref=len(F), first=funcs[i][0], last=funcs[j][0], e1=e1, e2=e2,
                                                exact=exact, fewer=fewer, rsizes=[s[3] for s in F],
                                                osizes=[funcs[k][3] for k in range(i, j + 1)], F=F))

    kar = kar_objects(wow_syms)
    chosen = {}  # key -> (path, {sec: (start, end)}, [candidates])
    for key, lst in sorted(cands.items(), key=lambda kv: str(kv[0])):
        path = melee_paths.get(key) or canonical(lst[0]["path"])
        # the wowjinxy map counts only where another reference gives the same range
        others = {(c["sec"], c["start"], c["end"]) for c in lst if c["ref"] != WOW}
        if others:
            wl = [c for c in lst if c["ref"] == WOW and (c["sec"], c["start"], c["end"]) not in others]
            for c in wl:
                rejected.append(f"{path}: the wowjinxy KAR map's {c['name']} {c['sec']} 0x{c['start']:08X}-0x{c['end']:08X} "
                                f"is not given by any other reference: not used")
            lst = [c for c in lst if c not in wl]
        use = settle(path, key, lst, funcs, defs, rejected, notes)
        if use is None:
            continue
        refs_used = {c["ref"] for c in use}
        if refs_used == {WOW}:
            ok, why = wow_agrees(use, kar)
            if not ok:
                rejected.append(f"{path}: only the wowjinxy KAR map proves it, and doldecomp/kar does not agree ({why})")
                continue
            for c in use:
                c["kar"] = why
        secs = {}
        for c in use:
            secs[c["sec"]] = (c["start"], c["end"])
        chosen[key] = (path, secs, use)
    same = defaultdict(list)
    for k, (_, secs, lst) in chosen.items():
        same[tuple(sorted(secs.items()))].append(k)
    for r, ks in same.items():
        if len(ks) > 1:
            ks.sort(key=lambda k: (-len({c["ref"] for c in chosen[k][2]}), k not in melee_paths, chosen[k][0]))
            rejected.append(f"same range under other names, kept {chosen[ks[0]][0]}: " + ", ".join(chosen[k][0] for k in ks[1:]))
            for k in ks[1:]:
                chosen.pop(k)
    ranges = sorted(((s, a, b, k) for k, (_, secs, _) in chosen.items() for s, (a, b) in secs.items()), key=lambda r: r[1])
    drop, drop_sec = set(), set()

    def weight(k):
        return (len({c["ref"] for c in chosen[k][2]}), k in melee_paths)
    for x, y in zip(ranges, ranges[1:]):
        if x[0] == y[0] and y[1] < x[2]:
            if (x[1], x[2]) == (y[1], y[2]) and weight(x[3]) != weight(y[3]):
                lo, hi = sorted((x[3], y[3]), key=weight)
                drop.add(lo)
                rejected.append(f"same {x[0]} range 0x{x[1]:08X}-0x{x[2]:08X} under another name, kept {chosen[hi][0]} "
                                f"(more references): {chosen[lo][0]}")
                continue
            ex = [any(c["exact"] for c in chosen[r[3]][2] if c["sec"] == r[0]) for r in (x, y)]
            if ex[0] == ex[1]:
                # else: the functions in both ranges - which side's references give them our sizes?
                both = [f for f in funcs if f[1] == x[0] and max(x[1], y[1]) <= f[2] < min(x[2], y[2])]

                def fits(r):
                    return any(all(any(g[0] == f[0] and g[3] == f[3] for g in c["F"]) for f in both)
                               for c in chosen[r[3]][2] if c["sec"] == r[0])
                ex = [fits(x), fits(y)]
            if ex[0] != ex[1]:
                # one of them has a reference with our exact function sizes, the other none: the other one loses
                lose, win = (x, y) if ex[1] else (y, x)
                drop_sec.add((lose[3], lose[0]))
                rejected.append(f"overlap: {chosen[lose[3]][0]} {lose[0]} 0x{lose[1]:08X}-0x{lose[2]:08X} and {chosen[win[3]][0]} "
                                f"0x{win[1]:08X}-0x{win[2]:08X}: only the second one's references have our function sizes "
                                f"(for the object, or for the functions both ranges hold): the first one's section dropped")
                continue
            drop.update((x[3], y[3]))
            rejected.append(f"overlap: {chosen[x[3]][0]} 0x{x[1]:08X}-0x{x[2]:08X} and {chosen[y[3]][0]} 0x{y[1]:08X}-0x{y[2]:08X}")
    for k, sec in sorted(drop_sec, key=str):
        if k in chosen and k not in drop:
            path, secs, lst = chosen[k]
            secs.pop(sec, None)
            chosen[k] = (path, secs, [c for c in lst if c["sec"] != sec])
            if not secs:
                drop.add(k)
    for k in drop:
        chosen.pop(k, None)
    paths = defaultdict(list)
    for k, v in chosen.items():
        paths[v[0]].append(k)
    for p, ks in paths.items():
        if len(ks) > 1:
            rejected.append(f"path clash {p}: {ks}")
            for k in ks:
                chosen.pop(k)
    return funcs, chosen, refinfo


LIB_SRC = [  # reference C sources of the libraries (pinned): which file DEFINES a function
    "refs/dolsdk2001/src", "refs/dolsdk2004/src", "melee/src/MSL", "melee/src/Runtime", "melee/src/MetroTRK",
    "melee/src/sysdolphin", "refs/marioparty4/src/dolphin", "refs/marioparty4/src/MSL_C.PPCEABI.bare.H",
    "refs/marioparty4/src/Runtime.PPCEABI.H", "refs/marioparty4/src/TRK_MINNOW_DOLPHIN",
    "refs/marioparty4/src/OdemuExi2", "refs/marioparty4/src/amcstubs", "refs/marioparty4/src/odenotstub",
]
DEFN = re.compile(r"^(?!#|typedef\b|extern\b|return\b|else\b)[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\(([^;]*)$")


def source_definers():
    """{function name: {(library family, file stem)}} for every function DEFINED (with a body, not a prototype) in
    the pinned library sources"""
    out = defaultdict(set)
    for root in LIB_SRC:
        r = G / "tools" / root
        if not r.exists():
            continue
        top = Path(root).name
        for p in sorted(r.rglob("*")):
            if p.suffix not in (".c", ".cpp", ".cp"):
                continue
            rel = p.relative_to(r).as_posix()
            fam = family(rel) if "dolsdk" in root else family(top + "/" + rel)
            lines = p.read_text(encoding="utf-8", errors="replace").splitlines()
            for k, line in enumerate(lines):
                m = DEFN.match(line)
                if not m or m.group(1) in ("if", "while", "for", "switch", "sizeof", "defined"):
                    continue
                text = " ".join(lines[k:k + 12])[m.end(1):]
                i1, i2 = text.find("{"), text.find(";")
                if i1 >= 0 and (i2 < 0 or i1 < i2):
                    out[m.group(1)].add((fam, p.stem.lower()))
    return out


def verdict(defs, name, fam):
    """the one file stem that defines name in the library family's sources (else in any library's), or None"""
    d = defs.get(name, set())
    st = {s for f, s in d if f == fam} or {s for f, s in d}
    return next(iter(st)) if len(st) == 1 else None


def settle(path, key, lst, funcs, defs, rejected, notes):
    """candidates of one object -> the ones to use, or None (rejected). Agreeing ranges are kept as they are; when the
    references disagree (SDK / MSL / TRK revisions, or a decomp's guess), each section is settled
      1. by source: our named functions inside some candidate range but not all are looked up in the pinned library
         sources (which file defines each); the one range whose membership matches the defining file for every such
         function (in the range iff defined in this file) wins;
      2. else by revision: only the references whose function sizes for the object equal ours one to one count."""
    per_sec = defaultdict(set)
    for c in lst:
        per_sec[c["sec"]].add((c["start"], c["end"]))
    if all(len(v) == 1 for v in per_sec.values()):
        return lst
    desc = "; ".join(f"{c['ref']} {c['sec']} 0x{c['start']:08X}-0x{c['end']:08X} "
                     f"{'sizes = ours' if c['exact'] else 'sizes ' + fmt_sizes(c['rsizes']) + ' vs ours ' + fmt_sizes(c['osizes'])}"
                     for c in lst)
    stem = key[1]
    keep, how = [], []
    for sec, rs in sorted(per_sec.items()):
        cs = [c for c in lst if c["sec"] == sec]
        if len(rs) == 1:
            keep += cs
            continue
        lo, hi = min(a for a, b in rs), max(b for a, b in rs)
        diff = [f for f in funcs if f[1] == sec and lo <= f[2] < hi and not DEFAULT.match(f[0])
                and not all(a <= f[2] < b for a, b in rs)]
        vd = {f[0]: verdict(defs, f[0], key[0]) for f in diff}
        known = {n: v for n, v in vd.items() if v}
        ok = []
        if known:
            for a, b in sorted(rs):
                if all((a <= f[2] < b) == (known[f[0]] == stem) for f in diff if f[0] in known):
                    ok.append((a, b))
        if len(ok) == 1:
            keep += [c for c in cs if (c["start"], c["end"]) == ok[0]]
            how.append(f"{sec} 0x{ok[0][0]:08X}-0x{ok[0][1]:08X} by source ("
                       + ", ".join(f"{n} defined in {v}.c" for n, v in sorted(known.items())) + ")")
            continue
        ex = {(c["start"], c["end"]) for c in cs if c["exact"]}
        if len(ex) == 1:
            r = next(iter(ex))
            keep += [c for c in cs if c["exact"]]
            how.append(f"{sec} 0x{r[0]:08X}-0x{r[1]:08X} by revision (function sizes equal ours: "
                       + ", ".join(sorted({c['ref'] for c in cs if c['exact']})) + ")")
            continue
        src = ", ".join(f"{n}: {', '.join(sorted(s for _, s in defs.get(n, ()))) or 'no source'}" for n in sorted(vd))
        rejected.append(f"{path}: references disagree on the {sec} range; neither the sources ({src or 'no named function differs'}) "
                        f"nor one revision's function sizes settle it: {desc}")
        return None
    notes.append(f"{path}: settled: " + "; ".join(how) + f"; kept {', '.join(sorted({c['ref'] for c in keep}))}. "
                 f"All candidates: {desc}")
    return keep


def fmt_sizes(lst):
    s = ",".join(f"{x:X}" for x in lst[:12])
    return f"[{s}{',..' if len(lst) > 12 else ''}]"


def placed_in(hsd):
    """{function name: {objkey of every library file a reference places it in}} over all references (code sections);
    a name another reference puts in the same library file as the object under test is that file's"""
    out = defaultdict(set)
    for ref in REFS:
        d = G / "tools" / ref
        tagr = ref.split('/')[-1]
        if not (d / "splits.txt").exists() or not (d / "symbols.txt").exists():
            continue
        rf = sorted([s for s in read_syms(d / "symbols.txt") if s[4]], key=lambda s: s[2])
        for name, secs in read_splits(d / "splits.txt"):
            path = lib_path(tagr, name, hsd)
            if path is None:
                continue
            for sec, a, b in secs:
                if sec in CODE:
                    for s in rf:
                        if a <= s[2] < b and s[1] == sec and not DEFAULT.match(s[0]):
                            out[s[0]].add(objkey(path))
    return out


def wow_agrees(use, kar):
    """a wowjinxy-only HSD object: doldecomp/kar must have the file with the same first / last function and names"""
    for c in use:
        if not c["path"].startswith("sysdolphin/"):
            return False, "not an HSD file"
        st = Path(c["path"]).stem.lower()
        k = kar.get(st)
        if not k:
            return False, f"no {st}.s / {st}.c in doldecomp/kar (or not contiguous)"
        F = c["F"]
        fa, fl = F[0][2], F[-1][2]
        la = fl + F[-1][3]
        if (k[0], k[1]) != (fa, la):
            return False, f"doldecomp/kar {st}: 0x{k[0]:08X}-0x{k[1]:08X}, wowjinxy 0x{fa:08X}-0x{la:08X}"
        for nm in (c["first"], c["last"]):
            a = next((s[2] for s in F if s[0] == nm), None)
            if a is None or k[2].get(a) != nm:
                return False, f"doldecomp/kar does not name 0x{a or 0:08X} {nm}"
    return True, f"doldecomp/kar {st} agrees (0x{k[0]:08X}-0x{k[1]:08X}, anchors named alike)"


# ---------------------------------------------------------------------------------------------------- data
def scratch_link(objs):
    """split + link objs (here: none, sections only) in decomp/build/splitdata/ -> main.elf path (SHA-1 must be retail)"""
    import gen_dtk_relocs as gr
    if SCRATCH.exists():
        shutil.rmtree(SCRATCH)
    SCRATCH.mkdir(parents=True)
    rel = os.path.relpath(SCRATCH, gr.D).replace("\\", "/")
    shutil.copyfile(OURS, SCRATCH / "symbols.txt")
    (SCRATCH / "splits.txt").write_bytes(splits_text(objs).encode("utf-8"))
    y = gr.strip_block(open(os.path.join(gr.CFG, "config.yml"), "rb").read().decode())
    y = re.sub(r"(?m)^symbols: .*$", f"symbols: {rel}/symbols.txt", y)
    y = re.sub(r"(?m)^splits: .*$", f"splits: {rel}/splits.txt", y)
    (SCRATCH / "config.yml").write_bytes(y.encode())
    gr.run([gr.DTK, "dol", "split", f"{rel}/config.yml", f"{rel}/out"])
    units = json.load(open(SCRATCH / "out/config.json"))["units"]
    sha = gr.link([u["object"] for u in units], f"{rel}/out/ldscript.lcf", f"{rel}/main.elf", f"{rel}/main.dol")
    if sha != gr.RETAIL:
        raise SystemExit(f"code-only scratch link SHA-1 {sha} is not retail")
    return str(SCRATCH / "main.elf")


def link_facts():
    """the sections-only scratch link, audited: relocations do not depend on the splits (dtk relocates by address)"""
    import reloc_audit as ra
    return ra.audit(scratch_link([]))


def pool_refs(audit, funcs):
    """{our function index: {(section, symbol start)}} and {(section, symbol start): {function index}} for the
    per-file pools: the compiler's .sdata2 constants (float / double literals) and the string literals (dtk's
    data:string symbols in .rodata / .data / .sdata / .sdata2). Each source file has its own pools."""
    import reloc_audit as ra
    elf, dobjs = audit[0], audit[1]
    strings = set()
    for line in OURS.read_text(encoding="utf-8").splitlines():
        if "data:string" in line:
            m = SYM.match(line)
            if m:
                strings.add(int(m.group(3), 16))
    secs = {x[1]: (x[3], x[3] + x[4]) for x in elf.alloc if x[1] in (".sdata2", ".rodata", ".data", ".sdata")}
    fst = [f[2] for f in funcs]
    by_f, by_c = defaultdict(set), defaultdict(set)
    for site, r in elf.relocs.items():
        if r[0] in (ra.R_REL24, ra.R_REL14):
            continue
        sec = next((n for n, (lo, hi) in secs.items() if lo <= r[1] < hi), None)
        if sec is None:
            continue
        k = bisect.bisect_right(fst, site) - 1
        if k < 0 or site >= funcs[k][2] + max(funcs[k][3], 4):
            continue
        c = dobjs.containing(r[1])
        if c and (sec == ".sdata2" or c[0] in strings):
            by_f[k].add((sec, c[0]))
            by_c[(sec, c[0])].add(k)
    return by_f, by_c


def data_splits(objs, chosen_by_path, refinfo, rejected, notes, audit):
    """objs: [(path, {sec: (a, b)})] code objects -> {path: {sec: (a, b)}} proven data pieces, report lines"""
    import reloc_audit as ra
    elf, dobjs, rng, events, raw, rels = audit
    secr = {s[1]: (s[3], s[3] + s[4]) for s in elf.alloc}

    def sec_of(a):
        for n, (lo, hi) in secr.items():
            if lo <= a < hi:
                return n
        return None
    # code units: split objects and the unsplit ranges, in address (= link) order
    units = []
    for sec in CODE:
        if sec not in secr:
            continue
        lo, hi = secr[sec]
        mine = sorted((r[0], r[1], p) for p, secs in objs for s, r in secs.items() if s == sec)
        cur = lo
        for a, b, p in mine:
            if a > cur:
                units.append((cur, a, None))
            units.append((a, b, p))
            cur = b
        if cur < hi:
            units.append((cur, hi, None))
    ustart = [u[0] for u in units]

    def unit(a):
        i = bisect.bisect_right(ustart, a) - 1
        return i if i >= 0 and a < units[i][1] else None

    def uname(i):
        u = units[i]
        return u[2] or f"unsplit code 0x{u[0]:08X}-0x{u[1]:08X}"
    ds = {}  # section -> sorted data symbol starts
    for sec in DATA:
        if sec in secr:
            lo, hi = secr[sec]
            ds[sec] = [v for v in dobjs.starts if lo <= v < hi]

    def dsym(a):
        c = dobjs.containing(a)
        return c[0] if c else None
    users = defaultdict(set)
    d2d = defaultdict(set)
    acc = [(a, r[1]) for a, r in elf.relocs.items() if r[0] not in (ra.R_REL24, ra.R_REL14)]
    acc += [(a, v) for k, a, v, d in events if k == "CROSS"]
    for site, tg in acc:
        ts = sec_of(tg)
        if ts not in ds:
            continue
        t = dsym(tg)
        if t is None:
            continue
        ss = sec_of(site)
        if ss in CODE:
            u = unit(site)
            if u is not None:
                users[t].add(u)
        elif ss in ds:
            s = dsym(site)
            if s is not None and s != t:
                d2d[t].add(s)
    for _ in range(8):  # data only other data points to: the users of those data (a string in a pointer table)
        ch = False
        for t, srcs in sorted(d2d.items()):
            if users.get(t):
                continue
            u = set()
            for s in srcs:
                u |= users.get(s, set())
            if u:
                users[t] = u
                ch = True
        if not ch:
            break
    # constant-offset spans: a boundary strictly inside one cuts an anchor from the address made from it
    spans = sorted({(min(d["anchor"], v), max(d["anchor"], v)) for k, a, v, d in events if k == "CROSS"})

    def cuts(x):
        return any(g0 < x <= g1 for g0, g1 in spans)

    out = defaultdict(dict)
    lines = []
    uidx = {u[2]: i for i, u in enumerate(units) if u[2]}
    for path, secs in objs:
        if path.startswith("game/") or path not in uidx:
            continue
        ui = uidx[path]
        cands = chosen_by_path[path][2]
        tsize = sum(b - a for a, b in secs.values())
        # the references that proved the code, with their own object's pieces
        rpieces = []
        seen = set()
        for c in cands:
            if (c["ref"], c["name"]) in seen or c["ref"] == WOW:
                continue  # (the wowjinxy map's data ranges have no second source: not used as data evidence)
            seen.add((c["ref"], c["name"]))
            rs, ro = refinfo[c["ref"]]
            rsecs = {s: (a, b) for s, a, b in ro.get(c["name"], [])}
            rt = sum(b - a for s, (a, b) in rsecs.items() if s in CODE)
            rpieces.append((c["ref"], rsecs, rt == tsize))
        for sec in DATA:
            if sec not in ds:
                continue
            lst = ds[sec]
            hi_sec = secr[sec][1]
            core = [i for i, v in enumerate(lst) if users.get(v) == {ui}]
            rsz = [(r, rsecs[sec][1] - rsecs[sec][0], same) for r, rsecs, same in rpieces if sec in rsecs]
            tag = f"{path} {sec}"
            if not core:
                if rsz:
                    rejected.append(f"data {tag}: no symbol used only by this object (references: "
                                    + ", ".join(f"{r} 0x{z:X}" for r, z, s in rsz) + ")")
                continue
            i, j = core[0], core[-1]
            inside = [lst[k] for k in range(i, j + 1) if users.get(lst[k]) and ui not in users[lst[k]]]
            if inside:
                rejected.append(f"data {tag}: 0x{lst[i]:08X}..0x{lst[j]:08X} has {len(inside)} symbol(s) of other units "
                                f"inside (first 0x{inside[0]:08X}, used by {uname(min(users[inside[0]]))})")
                continue
            k0 = i - 1
            while k0 >= 0 and (not users.get(lst[k0]) or ui in users[lst[k0]]):
                k0 -= 1
            k1 = j + 1
            while k1 < len(lst) and (not users.get(lst[k1]) or ui in users[lst[k1]]):
                k1 += 1
            if k0 >= 0 and max(users[lst[k0]]) >= ui:
                rejected.append(f"data {tag}: the symbol before, 0x{lst[k0]:08X}, is used by {uname(max(users[lst[k0]]))} "
                                f"(not before this object in link order)")
                continue
            if k1 < len(lst) and min(users[lst[k1]]) <= ui:
                rejected.append(f"data {tag}: the symbol after, 0x{lst[k1]:08X}, is used by {uname(min(users[lst[k1]]))} "
                                f"(not after this object in link order)")
                continue
            starts_c = [lst[k] for k in range(k0 + 1, i + 1) if not cuts(lst[k])]
            ends_c = [(lst[k + 1] if k + 1 < len(lst) else hi_sec) for k in range(j, k1)]
            ends_c = [e for e in ends_c if e == hi_sec or not cuts(e)]
            same = [z for r, z, s in rsz if s]
            no_piece_same = [r for r, rsecs, s in rpieces if s and sec not in rsecs]
            if no_piece_same:
                rejected.append(f"data {tag}: a reference of the same code size has no {sec} piece ({', '.join(no_piece_same)})")
                continue
            Z = set(same) if same else {z for r, z, s in rsz}
            if not Z:
                rejected.append(f"data {tag}: 0x{lst[i]:08X}.. is used only by this object, but no proving reference has a {sec} piece")
                continue
            m = [(a, b) for a in starts_c for b in ends_c if b - a in Z]
            amb = f"{i - k0 - 1}/{k1 - j - 1}"
            if len(m) != 1:
                cs = sorted({b - a for a in starts_c for b in ends_c})
                rejected.append(f"data {tag}: {'no' if not m else len(m)} candidate range(s) with a reference size "
                                f"(candidate sizes {fmt_sizes(cs)}, references {', '.join(f'{r} 0x{z:X}' + ('=' if s else '') for r, z, s in rsz)})")
                continue
            a, b = m[0]
            nsym = sum(1 for v in lst if a <= v < b)
            out[path][sec] = (a, b)
            lines.append(f"{path:<48} {sec:<7} 0x{a:08X}-0x{b:08X} {nsym:>3} sym  core {j - i + 1}, edge zone {amb}, size 0x{b - a:X} = "
                         + ", ".join(f"{r}{'(same code)' if s else ''}" for r, z, s in rsz if z == b - a))
    # code order in every section
    order = {p: k for k, (p, _) in enumerate(sorted(objs, key=lambda o: min(a for a, _ in o[1].values())))}
    for sec in DATA:
        got = sorted((v[sec][0], v[sec][1], p) for p, v in out.items() if sec in v)
        bad = set()
        for x, y in zip(got, got[1:]):
            if order[x[2]] > order[y[2]] or y[0] < x[1]:
                bad.update((x[2], y[2]))
        for p in sorted(bad):
            rejected.append(f"data {p} {sec}: not in code order with its neighbours, dropped")
            out[p].pop(sec)
            lines = [l for l in lines if not (l.startswith(p + " ") and f" {sec} " in l)]
    # unproven data: the parts of every data section outside a proven piece
    gaps = []
    for sec in DATA:
        if sec not in secr:
            continue
        lo, hi = secr[sec]
        cur = lo
        for a, b in sorted(v[sec] for v in out.values() if sec in v):
            if a > cur:
                gaps.append((sec, cur, a))
            cur = b
        if cur < hi:
            gaps.append((sec, cur, hi))
    return out, lines, gaps


# ---------------------------------------------------------------------------------------------------- data from C
CSRC = G / "decomp/src"
COBJ = G / "decomp/build/GXCE01/src"
# instruction bits a relocation leaves alone (the rest is the field the linker writes)
R_KEEP = {10: 0xFC000003, 11: 0xFFFF0003, 4: 0xFFFF0000, 5: 0xFFFF0000, 6: 0xFFFF0000, 109: 0xFFE00000}
R_ADDR32 = 1
C_PAD = 0x20  # at most this much alignment padding between two pieces of one object section / after its last symbol


def compile_c(objs, data):
    """the code objects (and the pieces proven so far) to splits.txt, then decomp/configure.py and ninja all_source:
    every decomp/src file compiled (a C compile depends on its source only, not on the split)"""
    import subprocess
    OUT.write_bytes(splits_text(objs, data).encode("utf-8"))
    ninja = G / "tools/ninja" / ("ninja.exe" if os.name == "nt" else "ninja")
    for cmd in ([sys.executable, "configure.py"], [str(ninja), "all_source"]):
        r = subprocess.run(cmd, cwd=G / "decomp", stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        if r.returncode:
            raise SystemExit(f"{' '.join(cmd)} failed in decomp/:\n{r.stdout[-3000:]}")


def read_obj(path):
    """a compiled .o -> (sections [(name, type, size, align, bytes or None)], symbols [(name, value, size, type, bind,
    shndx)], relocations {section index: [(offset, type, symbol index, addend)]})"""
    import struct
    d = path.read_bytes()
    shoff, = struct.unpack_from(">I", d, 0x20)
    shentsize, shnum, shstr = struct.unpack_from(">HHH", d, 0x2E)
    sh = [struct.unpack_from(">10I", d, shoff + i * shentsize) for i in range(shnum)]

    def nm(t, o):
        o += sh[t][4]
        return d[o:d.index(b"\0", o)].decode("latin-1")
    secs = [(nm(shstr, s[0]), s[1], s[5], s[8], d[s[4]:s[4] + s[5]] if s[1] == 1 else None) for s in sh]
    symsec = next(s for s in sh if s[1] == 2)
    syms = []
    for o in range(symsec[4], symsec[4] + symsec[5], 16):
        n, v, sz, info, _, shn = struct.unpack_from(">IIIBBH", d, o)
        syms.append((nm(symsec[6], n), v, sz, info & 15, info >> 4, shn))
    rels = defaultdict(list)
    for s in sh:
        if s[1] == 4:
            for o in range(s[4], s[4] + s[5], 12):
                off, info, add = struct.unpack_from(">IIi", d, o)
                rels[s[7]].append((off, info & 255, info >> 8, add))
    return secs, syms, rels


C_DEFS = []   # (address, name, C file, section, size, binding): what a C file that matches its whole object defines where
C_OVERHANG = {}   # (C file, section) -> (name, address, size) of the dtk symbol that runs on over the zeros after the C's last byte
C_SALIGN = {}     # (C file, section) -> the section's alignment in the compiled .o
SYM_TRIM = {}     # (section, address) -> new size: a dtk symbol cut at the end of a C piece (written to symbols.txt by main)


def c_data(elf, ours, objs, data, rejected):
    """DATA FROM THE MATCHED C (method: the docstring, "a matched C file proves its own data") ->
    ({path: {sec: (a, b)}} pieces, report lines, {address: C name} of the global symbols other units use)"""
    import struct
    import reloc_audit as ra
    dobjs = ra.Objects(elf)
    rtargets = {r[1] for r in elf.relocs.values()}
    starts = [a for a in dobjs.starts if not dobjs.info[a][0].startswith("gap_")]   # dtk's gap_ = padding
    secr = {s[1]: (s[3], s[3] + s[4]) for s in elf.alloc}
    name_addr = defaultdict(set)
    for n, sec, a, sz, isf in ours:
        name_addr[n].add(a)
    code_rng = {p: secs for p, secs in objs}

    def rbytes(a, n):
        for s in elf.alloc:
            if s[2] == 1 and s[3] <= a and a + n <= s[3] + s[4]:
                return elf.d[s[5] + a - s[3]: s[5] + a - s[3] + n]
        return None

    def rel_at(site, typ):
        r = elf.relocs.get(site)
        if r is None and typ == 109:
            r = elf.relocs.get(site & ~3)   # mwld puts a linked SDA21 relocation on the instruction, not its field
        return r if r and r[0] == typ else None

    def fits(addr, sec_bytes, fo, sz, rl):
        """the function's bytes at addr, every relocated field masked, and main.elf relocates every such site alike"""
        b = rbytes(addr, sz)
        if b is None:
            return False
        for i in range(0, sz, 4):
            keep = 0xFFFFFFFF
            for off, typ, si, add in rl:
                if fo + i <= off < fo + i + 4:
                    keep &= R_KEEP.get(typ, 0)
            if (struct.unpack_from(">I", b, i)[0] ^ struct.unpack_from(">I", sec_bytes, fo + i)[0]) & keep:
                return False
        return all(rel_at(addr + off - fo, typ) for off, typ, si, add in rl)

    # 1. every function of every C object: its retail address (our name inside the object's code range, else the one
    #    place in that range where its bytes fit), verified byte for byte with main.elf's relocations at every site
    objinfo = {}
    fstart = {}
    for n, sec, a, sz, isf in ours:
        if isf and sec in CODE:
            fstart.setdefault(a, (n, sz))
    for path in sorted(code_rng):
        o = COBJ / (path[:-2] + ".o")
        if path.startswith("game/") or not (CSRC / path).exists() or not o.exists():
            continue
        secs, syms, rels = read_obj(o)
        fmap, dead = {}, []
        cf = [(si, s) for si, s in enumerate(syms)
              if s[3] == 2 and s[2] and s[5] and s[5] < 0xFF00 and secs[s[5]][0] in CODE]

        def callee_ok(si, v, sz, shn, addr):
            """the function's calls / uses of names agree with ours: an undefined name ours has once is main.elf's
            target there; a call to a function of this file goes where that function is mapped"""
            for off, typ, ti, add in rels.get(shn, []):
                if not v <= off < v + sz:
                    continue
                r = rel_at(addr + off - v, typ)
                tn, tv, tsz, tt, tb, tshn = syms[ti]
                want = None
                if tshn == 0 and len(name_addr.get(tn, ())) == 1:
                    want = next(iter(name_addr[tn])) + add
                elif tshn and tt == 2 and ti in fmap:
                    want = fmap[ti] + add
                if want is not None and (not r or r[1] != want & 0xFFFFFFFF):
                    return False
            return True
        # by name first (our name inside the object's range), then the remaining ones by their bytes, only at a
        # function start of ours with no other real name and the same size; every address taken once
        for by_name in (True, False):
            for si, (n, v, sz, t, b, shn) in cf:
                if si in fmap:
                    continue
                rng = code_rng[path].get(secs[shn][0])
                if not rng:
                    continue
                rl = [r for r in rels.get(shn, []) if v <= r[0] < v + sz]
                taken = set(fmap.values())
                if by_name:
                    hits = [a for a in name_addr.get(n, ()) if rng[0] <= a < rng[1]]
                else:
                    hits = [a for a, (fn, fsz) in fstart.items() if rng[0] <= a < rng[1] and a not in taken
                            and fsz == sz and (DEFAULT.match(fn) or fn == n)]
                hits = [a for a in hits if fits(a, secs[shn][4], v, sz, rl)]
                if len(hits) == 1 and hits[0] not in taken:
                    fmap[si] = hits[0]
        for si, (n, v, sz, t, b, shn) in cf:   # a call that goes elsewhere: not this function after all
            if si in fmap and not callee_ok(si, v, sz, shn, fmap[si]):
                del fmap[si]
        dead = [s[0] for si, s in cf if si not in fmap]
        objinfo[path] = (secs, syms, rels, fmap, dead)

    def func_of(info, shn, off):
        secs, syms, rels, fmap, dead = info
        for fi, a in fmap.items():
            f = syms[fi]
            if f[5] == shn and f[1] <= off < f[1] + f[2]:
                return fi, a + off - f[1]
        return None, None

    # 2. the names the verified functions use from OTHER files (undefined symbols): name -> retail addresses
    ext = defaultdict(set)
    for path, info in objinfo.items():
        secs, syms, rels = info[:3]
        for shn, lst in rels.items():
            if secs[shn][0] not in CODE:
                continue
            for off, typ, si, add in lst:
                if syms[si][5] == 0 and syms[si][0]:
                    fi, site = func_of(info, shn, off)
                    r = rel_at(site, typ) if fi is not None else None
                    if r:
                        ext[syms[si][0]].add((r[1] - add) & 0xFFFFFFFF)

    out, lines, need, whole = defaultdict(dict), [], {}, []
    for path, info in sorted(objinfo.items()):
        secs, syms, rels, fmap, dead = info
        dsecs = [i for i, s in enumerate(secs) if s[0] in DATA and s[1] in (1, 8) and s[2]]
        dsyms = {i: sorted((v, sz, si) for si, (n, v, sz, t, b, shn) in enumerate(syms)
                           if shn == i and t in (0, 1) and n and sz) for i in dsecs}

        def container(shn, off):
            for v, sz, si in dsyms.get(shn, []):
                if v <= off < v + sz:
                    return si
            return None
        known = defaultdict(set)    # data symbol index -> retail addresses (with their evidence kind)
        how = defaultdict(set)
        live, pooled = set(), set()
        # own code: a verified function's relocation to this file's data, main.elf's target at that site
        for shn, lst in rels.items():
            if secs[shn][0] not in CODE:
                continue
            for off, typ, si, add in lst:
                ts = syms[si][5]
                if ts not in dsyms:
                    continue
                t = (syms[si][1] if syms[si][3] != 3 else 0) + add
                c = container(ts, t)
                fi, site = func_of(info, shn, off)
                if c is None or fi is None:
                    continue
                r = rel_at(site, typ)
                live.add(c)
                if syms[si][3] == 3 or syms[si][0].startswith("..."):
                    pooled.add(ts)   # a use of the section's pool symbol (...data.0: the -str pool) keeps it whole
                if r:
                    known[c].add((r[1] - (t - syms[c][1])) & 0xFFFFFFFF)
                    how[c].add("code")
        # other C files: a verified function of another object uses this file's global by name
        for c, (n, v, sz, t, b, shn) in enumerate(syms):
            if shn in dsyms and b == 1 and n in ext and sz:
                live.add(c)
                known[c] |= ext[n]
                how[c].add("C call site")
        # data -> data (a pointer table keeps the strings it points to); .ctors / .dtors are never stripped
        for i in dsecs:
            if secs[i][0] in (".ctors", ".dtors") or i in pooled:
                live.update(si for v, sz, si in dsyms[i])
        for _ in range(8):
            for shn in dsecs:
                for off, typ, si, add in rels.get(shn, []):
                    src = container(shn, off)
                    ts = syms[si][5]
                    if src in live and ts in dsyms:
                        if syms[si][3] == 3 or syms[si][0].startswith("..."):
                            live.update(x for v, sz, x in dsyms[ts])
                        c = container(ts, (syms[si][1] if syms[si][3] != 3 else 0) + add)
                        if c is not None:
                            live.add(c)

        # only a C file that matches the whole object proves its data: every retail function of the object is one of
        # the verified C functions (another revision can have data the C does not, used by code the C does not have)
        retail_f = {a for n, sec, a, sz, isf in ours if isf and any(lo <= a < hi for lo, hi in code_rng[path].values())}
        if not retail_f <= set(fmap.values()):
            if dsecs:
                rejected.append(f"data from C {path}: {len(retail_f - set(fmap.values()))} of {len(retail_f)} functions "
                                "of the object are not verified C functions (another revision): its data is not proven")
            continue
        at_ = defaultdict(list)
        for n, sec, a, sz, isf in ours:
            if isf:
                at_[a].append(n)
        other = [f"{'/'.join(at_[a])} = {syms[fi][0]}" for fi, a in sorted(fmap.items(), key=lambda x: x[1])
                 if syms[fi][0] not in at_[a]]
        for fi, a in fmap.items():
            C_DEFS.append((a, syms[fi][0], path, secs[syms[fi][5]][0], syms[fi][2], syms[fi][4]))
        whole.append(f"{path:<48} {len(retail_f):>3} functions" + (" (named otherwise in ours: " + ", ".join(other) + ")"
                                                                    if other else ""))
        # MAYBE there (not live, not proven stripped): a global (another file can use it)
        maybe = {si for si, (n, v, sz, t, b, shn) in enumerate(syms) if shn in dsyms and b == 1 and sz} - live

        def owner(a):
            """the other object whose proven piece holds address a (None: unproven); also another C file's piece placed
            before this one (in path order), with the zeros after its last byte up to the next 8-byte boundary (its
            alignment padding: no symbol of a third object can be there)"""
            return next((p for p, s in data.items() if p != path and any(x <= a < y for x, y in s.values())), None) or \
                next((p for p, s in out.items() if p != path and any(x <= a < (y + 7) & ~7 for x, y in s.values())), None)

        def expect(si, add):
            """the retail address a data relocation (symbol si + add) of this file must point at, None = unknown"""
            n, v, sz, t, b, shn = syms[si]
            if shn == 0:
                a = ext.get(n) or name_addr.get(n)
                return next(iter(a)) if a and len(a) == 1 else None
            if secs[shn][0] in CODE:
                fi, a = func_of(info, shn, (v if t != 3 else 0) + add)
                return a
            return None
        pos = {}    # placed data symbol index -> retail address (every section of this file)

        def toff(ti, add):
            return (syms[ti][1] if syms[ti][3] != 3 else 0) + add

        def target_of(ti, add):
            """the retail address a relocation (symbol ti + add) of this file must point at, None = unknown"""
            ts = syms[ti][5]
            if ts in dsyms:
                c = container(ts, toff(ti, add))
                return pos[c] + toff(ti, add) - syms[c][1] if c in pos else None
            return expect(ti, add)

        def sym_ok(shn, si, a):
            """symbol si at a: its bytes are main.dol's, every pointer in it is main.elf's relocation to its target"""
            sbytes = secs[shn][4]
            v, sz = syms[si][1], syms[si][2]
            mask = set()
            for off, typ, ti, add in rels.get(shn, []):
                if v <= off < v + sz:
                    mask.update(range(off - v, off - v + 4))
                    r = elf.relocs.get(a + off - v)
                    e = target_of(ti, add)
                    if typ != R_ADDR32 or not r or r[0] != R_ADDR32 or (e is not None and r[1] != e & 0xFFFFFFFF):
                        return (f"{syms[si][0]} 0x{a:08X}+0x{off - v:X}: main.elf has "
                                + ("no such relocation" if not r else f"type {r[0]} to 0x{r[1]:08X}")
                                + (f", the C points at 0x{e:08X}" if e is not None else ""))
            if sbytes is None:
                return None
            rb = rbytes(a, sz)
            if rb is None or any(rb[k] != sbytes[v + k] for k in range(sz) if k not in mask):
                return f"{syms[si][0]} at 0x{a:08X}: its bytes are not main.dol's"
            return None

        def place_section(shn, final):
            """-> None (placed), 'wait' (an address may still come from another section of the file), or why not"""
            sname, styp, ssz, salign, sbytes = secs[shn]
            C_SALIGN[(path, sname)] = salign or 1
            lo_s, hi_s = secr.get(sname, (0, 0))
            # runs of live symbols with no stripped symbol between: fixed distances inside a run
            runs, cur = [], []
            for x in dsyms[shn]:
                if x[2] in live:
                    cur.append(x)
                elif cur:
                    runs.append(cur)
                    cur = []
            if cur:
                runs.append(cur)
            place = []
            for run in runs:
                bases = {a - (v - run[0][0]) for v, sz, si in run for a in known.get(si, ())}
                if not bases and sbytes is not None and final:
                    # no address used by code: the one place in the section where the run's bytes are, every pointer
                    # in it going to its (known) target; at least one such pointer
                    ptrs = [(off - run[0][0], ti, add) for off, typ, ti, add in rels.get(shn, [])
                            if run[0][0] <= off < run[-1][0] + run[-1][1]]
                    if ptrs and all(target_of(ti, add) is not None for o_, ti, add in ptrs):
                        bases = {p for p in range(lo_s, hi_s, 4)
                                 if all((elf.relocs.get(p + o_) or (0, None))[1] == target_of(ti, add) & 0xFFFFFFFF
                                        for o_, ti, add in ptrs)
                                 and all(sym_ok(shn, si, p + v - run[0][0]) is None for v, sz, si in run)}
                        for v, sz, si in run:
                            how[si].add("pointer content")
                if len(bases) != 1:
                    if not bases and not final:
                        return "wait"
                    return (f"{len(bases)} addresses for the symbols at +0x{run[0][0]:X}..+0x{run[-1][0] + run[-1][1]:X}"
                            + (" (" + ", ".join(f"0x{b:08X}" for b in sorted(bases)[:3]) + ")" if bases else
                               " (no verified use)"))
                place.append((next(iter(bases)), run))
            # the edges: a symbol of the C that MAYBE is in main.dol, next to the first / after the last placed one.
            # Right next to it (no other symbol of the C between) with its own bytes exactly there (not only zeros):
            # it is there, the run grows. Else it must be shown absent: its bytes are nowhere next to the piece, or
            # the neighbour is another object's proven piece, or the section edge.
            order_ = dsyms[shn]
            idx = {x[2]: k for k, x in enumerate(order_)}

            def sym_at(p, v, sz):
                rb = rbytes(p, sz)
                rl = {k for off, typ, tsi, add in rels.get(shn, []) if v <= off < v + sz for k in range(off - v, off - v + 4)}
                return rb is not None and all(rb[k] == sbytes[v + k] for k in range(sz) if k not in rl)
            for side in ("before", "after"):
                step = -1 if side == "before" else 1
                k = idx[place[0][1][0][2]] - 1 if side == "before" else idx[place[-1][1][-1][2]] + 1
                adjacent = True
                while 0 <= k < len(order_):
                    v, sz, si = order_[k]
                    k += step
                    if si not in maybe:
                        adjacent = False
                        continue
                    a0, run = place[0] if side == "before" else place[-1]
                    p = a0 + v - run[0][0]
                    if adjacent and sbytes is not None and any(sbytes[v:v + sz]) and lo_s <= p and p + sz <= hi_s \
                            and sym_at(p, v, sz):
                        if side == "before":
                            place[0] = (p, [(v, sz, si)] + run)
                        else:
                            place[-1] = (a0, run + [(v, sz, si)])
                        continue
                    adjacent = False
                    if side == "before":
                        nb, ps = place[0][0] - 1, range(place[0][0] - sz - C_PAD, place[0][0] - sz + 1)
                    else:
                        e = place[-1][0] + place[-1][1][-1][0] + place[-1][1][-1][1] - place[-1][1][0][0]
                        nb, ps = e, range(e, e + C_PAD)
                    if not (lo_s <= nb < hi_s) or owner(nb):
                        continue
                    if sbytes is None:
                        return f"{syms[si][0]} (+0x{v:X}) may be {side} the piece (no bytes to tell, the neighbour is unproven)"
                    if not any(sbytes[v:v + sz]):
                        return f"{syms[si][0]} (+0x{v:X}) may be {side} the piece (zeros only: its bytes cannot tell)"
                    p = next((p for p in ps if sym_at(p, v, sz)), None)
                    if p is not None:
                        return f"{syms[si][0]} (+0x{v:X}) may be {side} the piece (its bytes are at 0x{p:08X})"
            # in .o order, each run after the last, separated by alignment padding only (zero bytes)
            for (a0, r0), (a1, r1) in zip(place, place[1:]):
                e0 = a0 + r0[-1][0] + r0[-1][1] - r0[0][0]
                if not (e0 <= a1 < e0 + C_PAD) or (sbytes is not None and rbytes(e0, a1 - e0) != bytes(a1 - e0)):
                    return f"the symbols at +0x{r1[0][0]:X} (0x{a1:08X}) do not follow +0x{r0[-1][0]:X} (ends 0x{e0:08X})"
            start = place[0][0]
            if start % min(salign or 1, 8):
                return f"starts at 0x{start:08X}, not {salign}-aligned"
            mine = {}
            for a0, run in place:
                for v, sz, si in run:
                    mine[si] = a0 + v - run[0][0]
            # a global between two runs that are exactly as far apart as in the .o (nothing stripped between) is at
            # its own offset; it is placed when main.dol uses that address
            for (a0, r0), (a1, r1) in zip(place, place[1:]):
                if a1 - a0 == r1[0][0] - r0[0][0]:
                    for v, sz, si in order_:
                        if r0[-1][0] < v < r1[0][0] and si in maybe and a0 + v - r0[0][0] in rtargets:
                            mine[si] = a0 + v - r0[0][0]
                            how[si].add("used at its offset")
            pos.update(mine)
            for si, a in mine.items():
                why = sym_ok(shn, si, a)
                if why:
                    for x in mine:
                        pos.pop(x)
                    return why
            last = place[-1]
            end = last[0] + last[1][-1][0] + last[1][-1][1] - last[1][0][0]
            # the piece ends at the C's last byte; the zeros after it (alignment padding) are placed by merge_c_data:
            # proven as the next object's alignment, or given to the next object / to this one (C_OVERHANG: a dtk
            # symbol that runs on over them is cut where the piece ends)
            for n_, s_, a_, z_, f_ in ours:
                if s_ == sname and a_ < start < a_ + z_:
                    for x in mine:
                        pos.pop(x)
                    return f"starts at 0x{start:08X}, inside dtk's symbol {n_} (0x{a_:08X}+0x{z_:X})"
                if s_ == sname and a_ < end < a_ + z_:
                    if a_ + z_ - end < C_PAD and (sbytes is None or rbytes(end, a_ + z_ - end) == bytes(a_ + z_ - end)):
                        # dtk's symbol runs on over the padding: the piece still ends at the C's last byte here;
                        # merge_c_data decides where the padding goes (C_OVERHANG) and cuts the symbol there
                        C_OVERHANG[(path, sname)] = (n_, a_, z_)
                    else:
                        for x in mine:
                            pos.pop(x)
                        return f"ends at 0x{end:08X}, inside dtk's symbol {n_} (0x{a_:08X}+0x{z_:X})"
            out[path][sname] = (start, end)
            # a pointer in it gives the address of what it points to in another section of the file
            for si, a in mine.items():
                v, sz = syms[si][1], syms[si][2]
                for off, typ, ti, add in rels.get(shn, []):
                    if v <= off < v + sz and syms[ti][5] in dsyms and syms[ti][5] != shn:
                        c = container(syms[ti][5], toff(ti, add))
                        r = elf.relocs.get(a + off - v)
                        if c is not None and r:
                            known[c].add((r[1] - (toff(ti, add) - syms[c][1])) & 0xFFFFFFFF)
                            how[c].add("pointer")
            nk = sum(1 for si in mine if known.get(si))
            lines.append(f"{path:<48} {sname:<7} 0x{start:08X}-0x{end:08X} {len(mine):>3} sym  of {len(dsyms[shn])} in the C"
                         f" ({len(dsyms[shn]) - len(mine)} stripped), {nk} at a verified use ("
                         + ", ".join(sorted({h for si in mine for h in how.get(si, ())})) + ")"
                         + (", bytes = main.dol" if sbytes is not None else ""))
            for si, a in mine.items():
                if syms[si][4] == 1:
                    need[a] = syms[si][0]
                C_DEFS.append((a, syms[si][0], path, sname, syms[si][2], syms[si][4]))
            return None
        pending = [shn for shn in dsecs if any(x[2] in live for x in dsyms[shn])]
        for final in (False, True):
            while pending:
                left = []
                for shn in pending:
                    why = place_section(shn, final)
                    if why == "wait":
                        left.append(shn)
                    elif why:
                        rejected.append(f"data from C {path} {secs[shn][0]}: {why}")
                if len(left) == len(pending):
                    break
                pending = left
    return out, lines, need, whole


def merge_c_data(objs, data, cdata, dlines, clines, rejected, elf):
    """the C's pieces into data (they replace a piece from the relocation users of the same object; another object's
    piece overlapping one is dropped, as the C proves its bytes); code order checked again -> unproven data ranges"""
    for path, secs in sorted(cdata.items()):
        for sec, (a, b) in sorted(secs.items()):
            old = data.get(path, {}).get(sec)
            if old and (old[0] != a or b > old[1] or old[1] - b >= C_PAD):   # (not: only the padding after it)
                clines.append(f"{path:<48} {sec:<7} (the relocation users gave 0x{old[0]:08X}-0x{old[1]:08X}: replaced)")
            for p2, s2 in sorted(data.items()):
                if p2 != path and sec in s2 and s2[sec][0] < b and a < s2[sec][1]:
                    if sec in cdata.get(p2, {}):
                        raise SystemExit(f"data from C: {path} and {p2} both prove {sec} 0x{a:08X}..0x{b:08X}")
                    rejected.append(f"data {p2} {sec}: 0x{s2[sec][0]:08X}-0x{s2[sec][1]:08X} overlaps {path}'s piece "
                                    f"proven by its C (0x{a:08X}-0x{b:08X}), dropped")
                    dlines[:] = [ln for ln in dlines if not (ln.startswith(p2 + " ") and f" {sec} " in ln)]
                    s2.pop(sec)
            data.setdefault(path, {})[sec] = (a, b)
    # the zeros after a C piece (alignment padding): up to another object's piece or the section end when that is
    # near, as dtk splits (it makes them one gap_ symbol, which a split may not cut). When an automatic object
    # follows, the padding starts it (linked from C, this object brings no padding; the automatic object must, as
    # the next one's alignment need not give it), from the next word on (dtk splits at 4-byte boundaries only).
    # .ctors / .dtors: zeros there are the end marker, not padding.
    import reloc_audit as ra
    dobjs = ra.Objects(elf)
    starts = [x for x in dobjs.starts if not dobjs.info[x][0].startswith("gap_")]
    secr = {s[1]: (s[3], s[3] + s[4]) for s in elf.alloc}

    def zeros(a, n):
        for s in elf.alloc:
            if s[3] <= a and a + n <= s[3] + s[4]:
                return s[2] != 1 or elf.d[s[5] + a - s[3]: s[5] + a - s[3] + n] == bytes(n)
        return False
    # THE TRAILING-PADDING RULE (step 61 iteration 8): a C piece ends at the C's last byte when only zeros follow
    # up to another object's piece that starts at the first address after it aligned to that object's alignment
    # (dtk's split alignment: the largest align: of its symbols, else 8; from C, also its .o section's): the linker
    # puts the padding there (from asm, its .balign; from C, its section alignment), and dtk makes no automatic
    # object of zeros shorter than the next split's alignment. Else as before: the padding goes to the next piece's
    # start / to the section end (this object keeps it), or, before an automatic object, starts that object from
    # the next word on (an automatic object must start 4-aligned). A dtk symbol that runs on over the C's last byte
    # is cut where the piece ends (SYM_TRIM -> symbols.txt), as a split may not end inside a symbol.
    align = {}
    for ln in OURS.read_text(encoding="utf-8", errors="replace").splitlines():
        m = SYM.match(ln)
        al = re.search(r"align:(\d+)", ln)
        if m and al:
            align[(m.group(2), int(m.group(3), 16))] = int(al.group(1))
    owner = {(sec, v[sec][0]): p for p, v in data.items() for sec in v}
    for path, secs in sorted(cdata.items()):
        for sec, (a, b) in sorted(secs.items()):
            if sec in (".ctors", ".dtors"):
                continue
            hi = secr[sec][1]
            ov = C_OVERHANG.get((path, sec))
            pieces = [s[sec][0] for p, s in data.items() if p != path and sec in s and s[sec][0] >= b]
            k = bisect.bisect_left(starts, b)
            nxt = min([hi] + pieces + ([starts[k]] if k < len(starts) else []))
            e = b
            if nxt in pieces:
                p2 = owner[(sec, nxt)]
                n_end = data[p2][sec][1]
                al = max([v for (s_, x), v in align.items() if s_ == sec and nxt <= x < n_end] or [8])
                if p2 in cdata and sec in cdata[p2]:
                    al = min(al, C_SALIGN.get((p2, sec), 1))
                if not (b < nxt and (b + al - 1) & -al == nxt and zeros(b, nxt - b)):
                    e = max(b, ov[1] + ov[2]) if ov else b   # not proven: the padding stays with this object
                    if e < nxt and nxt - e < C_PAD and zeros(e, nxt - e):
                        e = nxt
            elif nxt == hi:
                e = max(b, ov[1] + ov[2]) if ov else b
                if e < nxt and nxt - e < C_PAD and zeros(e, nxt - e):
                    e = nxt
            else:
                w = (b + 3) & ~3
                if b < w and zeros(b, w - b):
                    e = w
            if ov and ov[1] + ov[2] > e:
                SYM_TRIM[(sec, ov[1])] = e - ov[1]
            if e != b:
                data[path][sec] = cdata[path][sec] = (a, e)
    # link order = .text order (an object with .init code only, __start.c, is linked where its data says: dtk checks)
    order = {p: secs[".text"][0] for p, secs in objs if ".text" in secs}
    for sec in DATA:
        got = sorted((v[sec][0], v[sec][1], p) for p, v in data.items() if sec in v)
        for x, y in zip(got, got[1:]):
            if y[0] < x[1] or order.get(x[2], 0) > order.get(y[2], 0xFFFFFFFF):
                raise SystemExit(f"data {sec}: {x[2]} 0x{x[0]:08X} and {y[2]} 0x{y[0]:08X} are not in code order")
    secr = {s[1]: (s[3], s[3] + s[4]) for s in elf.alloc}
    gaps = []
    for sec in DATA:
        if sec not in secr:
            continue
        lo, hi = secr[sec]
        cur = lo
        for a, b in sorted(v[sec] for v in data.values() if sec in v):
            if a > cur:
                gaps.append((sec, cur, a))
            cur = b
        if cur < hi:
            gaps.append((sec, cur, hi))
    return gaps


# ---------------------------------------------------------------------------------------------------- game
def read_g3():
    """tools/match_game_names.py's G3 module notes: {function address: the .c file name its assert / panic passes}"""
    g3 = {}
    if GAME_REAL.exists():
        for line in GAME_REAL.read_text(encoding="utf-8").splitlines():
            m = re.match(r"// G3 0x([0-9A-Fa-f]{8}) (\S+\.c) ", line)
            if m:
                g3[int(m.group(1), 16)] = m.group(2)
    return g3


GAME_SRC = G / "decomp/src/game"
CDEF = re.compile(r"^[A-Za-z_][^;(]*?\b(\w+)\s*\([^;]*\)\s*\{\s*$")


def game_c_objects(funcs, game_end):
    """game objects from the files in decomp/src/game (method 1 of the docstring): [(path, {".text": (a, b)})]"""
    out = []
    if not GAME_SRC.exists():
        return out
    idx = defaultdict(list)
    for i, f in enumerate(funcs):
        idx[f[0]].append(i)
    for p in sorted(GAME_SRC.rglob("*.c")):
        rel = "game/" + p.relative_to(GAME_SRC).as_posix()
        names = [m.group(1) for m in map(CDEF.match, p.read_text(encoding="utf-8").splitlines()) if m]
        if not names:
            raise SystemExit(f"decomp/src/{rel}: no function definition found")
        ii = []
        for n in names:
            if len(idx.get(n, [])) != 1:
                raise SystemExit(f"decomp/src/{rel} defines {n}, which is not exactly one function of ours "
                                 "(renamed? rerun tools/promote_drafts.py)")
            ii.append(idx[n][0])
        ii.sort()
        if ii != list(range(ii[0], ii[-1] + 1)) or funcs[ii[0]][1] != ".text":
            raise SystemExit(f"decomp/src/{rel}: its functions are not consecutive functions of ours in .text")
        a = funcs[ii[0]][2]
        b = funcs[ii[-1] + 1][2] if ii[-1] + 1 < len(funcs) else None
        if b is None or b > game_end or a < 0x80005000:
            raise SystemExit(f"decomp/src/{rel}: not inside the game code")
        out.append((rel, {".text": (a, b)}))
    return out


GAME_DATA = (".rodata", ".data", ".sdata", ".sdata2")


def game_file_data(elf, dobjs, opath, text_start, text_end, addr_of):
    """FILE OBJECTS (plan.md step 63): a game C file's own data (its .sdata2 constant pool, string literals) placed
    where main.dol has it -> ({section: (start, end)}, "") or ({}, the reason it is not proven).
    Every relocation of the compiled object's .text into one of its own data sections goes to that section's piece
    start + the symbol's offset in the object: main.elf (the sections-only scratch link) relocates that site, with
    the same type, to that address, so all of them must give the same piece start. The piece is then proven when its
    bytes equal the object's section, it starts on a dtk data symbol, every relocation of main.elf into it comes from
    the file's own code range (the per-file pool rule: no other function shares an entry, so no other file needs it),
    and the bytes after it up to the next dtk symbol are zeros (the alignment padding, which goes with it).
    Data sections with relocations of their own (pointer tables) are not handled: reported."""
    secs, syms, rels = read_obj(opath)
    ti = next(i for i, s in enumerate(secs) if s[0] == ".text")
    own = {i: s for i, s in enumerate(secs) if s[0] in GAME_DATA and s[2]}
    if not own:
        return {}, ""
    for i in own:
        if rels.get(i):
            return {}, f"its {secs[i][0]} has relocations (a pointer table): not handled"
    start = {}
    for off, typ, si, add in rels.get(ti, []):
        n, v, sz, t, b, shn = syms[si]
        site = text_start + off
        r = elf.relocs.get(site)
        if r is None and typ == 109:
            r = elf.relocs.get(site & ~3)
        if r is None or r[0] != typ:
            return {}, f"main.elf has no relocation of type {typ} at 0x{site:08X}"
        if shn in own:
            s = r[1] - (v + add)
            if start.setdefault(shn, s) != s:
                return {}, f"{secs[shn][0]}: its relocations give two piece starts (0x{start[shn]:08X}, 0x{s:08X})"
        elif shn == ti:
            if r[1] != text_start + v + add:
                return {}, f"the call / reference at 0x{site:08X} goes elsewhere in main.elf"
        elif shn == 0:
            if addr_of(n) is None or r[1] != addr_of(n) + add:
                return {}, f"{n} at 0x{site:08X}: main.elf's target is 0x{r[1]:08X}"
    out = {}
    for shn, s in sorted(start.items()):
        name, typ, size, align, data = own[shn]
        if name not in [x[1] for x in elf.alloc] or elf.section_of(s) != name or s % max(1, align):
            return {}, f"{name} piece 0x{s:08X} is not in main.dol's {name} or not aligned"
        sec = next(x for x in elf.alloc if x[1] == name)
        got = elf.d[sec[5] + s - sec[3]: sec[5] + s - sec[3] + size]
        if got != data:
            return {}, f"{name} piece 0x{s:08X}: main.dol's bytes differ from the object's"
        if s not in dobjs.info:
            return {}, f"{name} piece 0x{s:08X} does not start on a dtk symbol"
        i = bisect.bisect_left(dobjs.starts, s + size)
        e = dobjs.starts[i] if i < len(dobjs.starts) and dobjs.starts[i] < sec[3] + sec[4] else sec[3] + sec[4]
        if any(elf.d[sec[5] + s + size - sec[3]: sec[5] + e - sec[3]]):
            return {}, f"{name} piece 0x{s:08X}: non-zero bytes between its end 0x{s + size:08X} and the next symbol"
        for site, r in elf.relocs.items():
            if s <= r[1] < e and not text_start <= site < text_end:
                return {}, f"{name} 0x{r[1]:08X} is also used from 0x{site:08X} (another file's code or data)"
        out[name] = (s, e)
    return out, ""


def game_data(audit, objs, data, ours, rejected):
    """the data pieces of the game C files that have data of their own (game_file_data) -> report lines"""
    elf, dobjs = audit[0], audit[1]
    addr = defaultdict(set)
    for n, sec, a, sz, isf in ours:
        addr[n].add(a)

    def addr_of(n):
        s = addr.get(n, ())
        return next(iter(s)) if len(s) == 1 else None
    taken = [r for v in data.values() for r in v.values()]
    lines = []
    for path, secs in objs:
        o = COBJ / (path[:-2] + ".o")
        if not path.startswith("game/") or ".text" not in secs or not o.exists():
            continue
        pieces, why = game_file_data(elf, dobjs, o, secs[".text"][0], secs[".text"][1], addr_of)
        if why:
            rejected.append(f"data {path}: {why}")
            continue
        for s, (a, b) in sorted(pieces.items()):
            if any(a < y and x < b for x, y in taken):
                rejected.append(f"data {path} {s}: 0x{a:08X}-0x{b:08X} overlaps another object's piece")
                continue
            data.setdefault(path, {})[s] = (a, b)
            taken.append((a, b))
            lines.append(f"{path:<48} {s:<7} 0x{a:08X}-0x{b:08X} the file's own {s} (bytes = the object's, used only "
                         f"by its code: file objects, plan.md step 63)")
    return lines

def game_splits(funcs, chosen, rejected):
    g3 = read_g3()
    idx_of = {s[2]: i for i, s in enumerate(funcs)}
    game_objs = []
    by_file = defaultdict(list)
    for a, f in g3.items():
        if a in idx_of:
            by_file[f].append(idx_of[a])
    lib_ranges = [r for _, secs, _ in chosen.values() for r in secs.values()]
    # the game code: .text up to the first library object (the libraries are linked after the game)
    game_end = min(a for _, secs, _ in chosen.values() for s, (a, b) in secs.items() if s == ".text")
    for f, idx in sorted(by_file.items()):
        i, j = min(idx), max(idx)
        if any(a <= funcs[i][2] < b for a, b in lib_ranges) or funcs[i][2] >= game_end:
            continue  # a library file (axdriver.c, dvd.c, ...): handled above
        others = [k for k in range(i, j + 1) if funcs[k][2] in g3 and g3[funcs[k][2]] != f]
        before = funcs[i - 1][2] if i > 0 else None
        after = funcs[j + 1][2] if j + 1 < len(funcs) else None
        if not others and before in g3 and g3[before] != f and after in g3 and g3[after] != f:
            game_objs.append((f"game/{f}", {funcs[i][1]: (funcs[i][2], funcs[j + 1][2])}))
        else:
            why = "another file's note inside" if others else "ends not next to another file's function"
            rejected.append(f"game {f}: G3 functions 0x{funcs[i][2]:08X}..0x{funcs[j][2]:08X} ({len(idx)} of {j - i + 1} "
                            f"functions in between), {why}: the functions before 0x{funcs[i][2]:08X} and after "
                            f"0x{funcs[j][2]:08X} carry no file note")
    cobjs = game_c_objects(funcs, game_end)
    taken = [r for _, secs in cobjs for r in secs.values()]
    for o in game_objs[:]:
        a, b = o[1][".text"]
        if any(a < y and x < b for x, y in taken):
            rejected.append(f"game {o[0]}: G3 range 0x{a:08X}-0x{b:08X} overlaps a C file's object (decomp/src/game)")
            game_objs.remove(o)
    return game_objs + cobjs


SEC_ORDER = {s: i for i, s in enumerate(CODE + DATA)}


def splits_text(objs, data=None):
    lines = BASE.read_text(encoding="utf-8").replace("\r\n", "\n").rstrip("\n").split("\n")
    for path, secs in objs:
        allsec = dict(secs)
        allsec.update((data or {}).get(path, {}))
        lines.append("")
        lines.append(f"{path}:")
        for s in sorted(allsec, key=lambda s: SEC_ORDER[s]):
            a, b = allsec[s]
            lines.append(f"\t{s:<11} start:0x{a:08X} end:0x{b:08X}")
    return "\n".join(lines) + "\n"


def trim_symbols():
    """cut the dtk symbols that run on over the end of a C piece (SYM_TRIM, merge_c_data) in symbols.txt: dtk
    rejects a split that ends inside a symbol; the bytes cut off are the zeros after the C's last byte"""
    if not SYM_TRIM:
        return
    out = []
    for ln in OURS.read_text(encoding="utf-8").split("\n"):
        m = SYM.match(ln)
        if m and (m.group(2), int(m.group(3), 16)) in SYM_TRIM:
            ln = re.sub(r"size:0x[0-9A-Fa-f]+", f"size:0x{SYM_TRIM[(m.group(2), int(m.group(3), 16))]:X}", ln)
        out.append(ln)
    OURS.write_bytes("\n".join(out).encode("utf-8"))


def main(argv) -> int:
    ours = read_syms(OURS)
    rejected, notes = [], []
    audit = None if "--no-data" in argv else link_facts()
    funcs, chosen, refinfo = code_splits(ours, rejected, notes, audit)
    game_objs = game_splits(funcs, chosen, rejected)
    objs = [(v[0], v[1]) for v in chosen.values()] + game_objs
    objs.sort(key=lambda o: min(a for a, _ in o[1].values()))
    by_path = {v[0]: v for v in chosen.values()}
    data, dlines, dgaps = ({}, [], [])
    clines, cnames, cwhole = [], [], []
    if "--no-data" not in argv:
        data, dlines, dgaps = data_splits(objs, by_path, refinfo, rejected, notes, audit)
        if "--no-c" not in argv:
            compile_c(objs, data)
            cdata, clines, need, cwhole = c_data(audit[0], ours, objs, data, rejected)
            clines += game_data(audit, objs, data, ours, rejected)
            dgaps = merge_c_data(objs, data, cdata, dlines, clines, rejected, audit[0])
            at = defaultdict(list)
            for n, sec, a, sz, isf in ours:
                at[a].append(n)
            cnames = [f"0x{a:08X} {n} (ours: {', '.join(at.get(a, [])) or 'no symbol'})"
                      for a, n in sorted(need.items()) if n not in at.get(a, [])]
    OUT.write_bytes(splits_text(objs, data).encode("utf-8"))
    trim_symbols()

    # unproven code ranges: the parts of .init / .text not inside an object
    covered = sorted((a, b) for _, secs in objs for (a, b) in secs.values())
    gaps = []
    for sec in CODE:
        fs = [s for s in funcs if s[1] == sec]
        if not fs:
            continue
        lo, hi = fs[0][2], fs[-1][2] + fs[-1][3]
        cur = lo
        for a, b in covered:
            if b <= lo or a >= hi:
                continue
            if a > cur:
                gaps.append((sec, cur, a))
            cur = max(cur, b)
        if cur < hi:
            gaps.append((sec, cur, hi))

    def nf(a, b):
        return sum(1 for s in funcs if a <= s[2] < b)

    lib = [o for o in objs if not o[0].startswith("game/")]
    fam = defaultdict(int)
    for p, _ in lib:
        fam[p.split("/")[0]] += 1
    dfam = defaultdict(int)
    dsec = defaultdict(int)
    for p, v in data.items():
        if v:
            dfam[p.split("/")[0]] += 1
        for s in v:
            dsec[s] += 1
    head = [
        "decomp/config/GXCE01/splits.txt: generated by tools/gen_dtk_splits.py (method: its docstring).",
        f"objects split: {len(objs)} ({len(lib)} library, {len(objs) - len(lib)} game); code: "
        + ", ".join(f"{k} {v}" for k, v in sorted(fam.items())),
        f"functions inside split objects: {sum(nf(a, b) for _, secs in objs for (a, b) in secs.values())} of {len(funcs)}",
        f"unproven code ranges (dtk automatic objects): {len(gaps)}, {sum(nf(a, b) for _, a, b in gaps)} functions",
        f"data pieces proven: {sum(dsec.values())} in {sum(1 for v in data.values() if v)} objects ("
        + ", ".join(f"{k} {v}" for k, v in sorted(dfam.items())) + "); per section: "
        + ", ".join(f"{s} {dsec[s]}" for s in DATA if dsec[s]),
        "accepted with fewer functions than the reference (rule 5: both edges are named neighbours of other objects): "
        + (", ".join(sorted({path for path, _ in objs if path in by_path and any(c.get("fewer") for c in by_path[path][2])}))
           or "none"),
        "",
        "== objects (path, range, functions, proving references: ref first..last ours/ref count)",
    ]
    for path, secs in objs:
        for s, (a, b) in secs.items():
            proofs = [c for c in by_path[path][2] if c["sec"] == s] if path in by_path else []
            pr = "; ".join(f"{c['ref']} {c['first']}..{c['last']} {c['n']}/{c['nref']} [{c['e1']} | {c['e2']}]"
                           + (f" [{c['kar']}]" if c.get("kar") else "")
                           + (" [fewer functions than the reference, both edges named: rule 5]" if c.get("fewer") else "")
                           for c in proofs) or \
                ("its C file's functions (decomp/src/" + path + ")" if (GAME_SRC.parent / path).exists() else "G3 neighbours")
            head.append(f"{path:<48} {s:<6} 0x{a:08X}-0x{b:08X} {nf(a, b):>3} fn  {pr}")
    head += ["", "== settled disagreements (references gave different ranges: settled by the defining source file, else by "
                 "the revision whose function sizes equal ours)"] + notes
    head += ["", "== data pieces (path, section, range, data symbols, evidence: core symbols used only by the object, "
                 "unattributed symbols before/after it, the references whose piece has this size)"] + dlines
    head += ["", "== objects whose every function is a function of their C file (bytes and relocations = main.dol's; the C proves their data)"] + cwhole
    head += ["", "== C definitions (the C file matches its whole object: each of its functions and placed data symbols is at this "
                 "address; tools/check_c_callsites.py checks the // c-define lines of symbols/symbols.txt against them)"]
    head += [f"DEF 0x{a:08X} {n} {p} {s} size:0x{z:X} {'global' if b == 1 else 'local'}" for a, n, p, s, z, b in sorted(set(C_DEFS))]
    head += ["", "== data pieces proven by the matched C (path, section, range, the C's symbols there, how their address "
                 "is known)"] + clines
    head += ["", "== global symbols of those pieces that our symbols name otherwise (an object linked from C needs every "
                 "name another object uses)"] + cnames
    head += ["", "== unproven code ranges (one dtk automatic object each)"]
    for s, a, b in gaps:
        head.append(f"{s:<6} 0x{a:08X}-0x{b:08X} {nf(a, b):>4} functions")
    head += ["", "== unproven data ranges (dtk automatic objects)"]
    for s, a, b in dgaps:
        head.append(f"{s:<7} 0x{a:08X}-0x{b:08X} size 0x{b - a:X}")
    head += ["", f"== rejected candidates ({len(rejected)})"] + rejected
    REPORT.parent.mkdir(parents=True, exist_ok=True)
    REPORT.write_bytes(("\r\n".join(head) + "\r\n").encode("utf-8"))
    print("\n".join(head[:5]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
