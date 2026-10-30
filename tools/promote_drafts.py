"""Promote the m2c drafts that match 100% into the decomp: decomp/src/game/<function>.c (plan.md step 58, docs/drafts.md).

Batch 12 (tools/draft_batch.py) drafts every game function and compares the compiled draft with the original with
relocation FIELDS masked. That is not enough to link it, so a draft is promoted only when all of this holds:
  1. draft_report.txt says "100% MATCH";
  2. its compiled object (decomp/drafts/obj/<name>.o) has exactly one function, of the original size, and nothing
     else that takes space: no .rodata / .data / .sdata / .sdata2 / .bss / string literals of its own (a per-function
     object cannot bring a piece of a pool that the original file shares between functions); its .text alignment
     divides the function's address;
  3. no padding: the next function of ours (decomp/config/GXCE01/symbols.txt) starts right where it ends, so the
     object is exactly the function (a C object brings no padding);
  4. every relocation is at the same offset, of the same type, and goes to the same ADDRESS as the original's (dtk's
     split object of batch 9: its symbol + addend; the C's undefined name looked up in our symbols, + addend);
  5. the promoted file (below) compiles, with the game flags of decomp/configure.py, to the same .text bytes, the same
     relocations (offset, type, symbol name, addend) and the same sections as the draft.
mwld dead-strips a C function that nothing references (dtk's asm objects keep theirs). A function that no relocation
of main.dol points to (decomp/build/splitdata/main.elf: batch 9's sections-only scratch link, every relocation dtk
found, retail SHA-1) is therefore wrapped in `#pragma force_active on` / `reset` (the .text is unchanged; the object
only marks the function as kept). Such a function may still be reached through a pointer dtk did not see.
The promoted file is the draft's own text, behind a committed header instead of the drafts' context:
    #include "game/cr_game.h"     (decomp/include/game: the basic types and the m2c forms, ours)
    + the declarations it uses from the HSD / SDK headers and symbols/cr_types.h, taken out of the drafts' context
      (decomp/drafts/ctx_pp.c, parsed with m2c's pycparser): functions / globals it calls or uses, the enums of the
      constants it names, typedefs; a struct used only through a pointer is declared incomplete (typedef struct X X;),
      a struct used by value gets its definition, recursively.
One function per object: the game's own source files are not known (no reference decomp, no file names in the
asserts for most of it). The file says so; merging functions into real files later is a new object range and nothing
else (tools/gen_dtk_splits.py takes a game object's range from the functions its file defines).

Files this tool writes start with its marker line; only those are ever rewritten or removed (a hand-edited game file
drops the marker and is then left alone). A marked file whose draft is no longer 100% is KEPT (it still matches on its
own: batch 9 links it and checks the SHA-1); a marked file whose function name is no longer ours (renamed in
symbols/descriptive.txt) is removed and, when the draft under the new name passes, written again under that name.
Writes decomp/reports/promote_report.txt (CRLF, deterministic: promoted files, files kept, every 100% draft left out
and why). Exit 1 on an internal error only. Prints "changed" when decomp/src/game changed (batch 12 then rebuilds).
Usage: python -I tools/promote_drafts.py [--check]   (--check: write nothing in decomp/src, only the report to stdout)
"""
import concurrent.futures as cf
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

G = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(G / "tools"))
sys.path.insert(0, str(G / "tools/m2c"))
import draft_batch as db  # noqa: E402
import gen_dtk_splits as gs  # noqa: E402

D = G / "decomp"
DRAFTS = D / "drafts"
SRC = D / "src/game"
WORK = D / "build/promote"
REPORT = D / "reports/promote_report.txt"
HEADER = "game/cr_game.h"
MARK = "/* Custom Robo game code, promoted by tools/promote_drafts.py"
NL = "\n"
SPACE = (".rodata", ".data", ".bss", ".sdata", ".sbss", ".sdata2", ".sbss2", ".ctors", ".dtors")


# ---- the drafts' context, parsed once ----------------------------------------------------------------------------------
def m2c_source(text):
    """the text the way m2c reads a context (its own clean-up steps), for its pycparser"""
    from m2c import c_types as ct
    for f in (ct.strip_comments, ct.process_ifdef, ct.strip_macro_defs, ct.remove_backslashes_at_eol):
        text = f(text)
    return text


class Context:
    def __init__(self, path):
        from m2c.c_types import parse_c
        from m2c_pycparser import c_ast as ca
        self.ca = ca
        from m2c.c_types import add_builtin_typedefs
        ast, self.scope = parse_c(m2c_source(add_builtin_typedefs(path.read_text(encoding="utf-8"))), None)
        self.typedef, self.ident, self.tagdef, self.enumer, self.enumdef = {}, {}, {}, {}, {}
        for it in ast.ext:
            if isinstance(it, ca.Typedef):
                self.typedef.setdefault(it.name, it)
                self._defs(it, it.type)
            elif isinstance(it, ca.Decl):
                if it.name:
                    if not isinstance(self.ident.get(it.name), ca.FuncDef):
                        self.ident.setdefault(it.name, it)
                self._defs(it, it.type)
            elif isinstance(it, ca.FuncDef):
                self.ident[it.decl.name] = it

    def _defs(self, item, t):
        ca = self.ca
        if isinstance(t, ca.TypeDecl):
            t = t.type
        if isinstance(t, (ca.Struct, ca.Union)) and t.decls is not None and t.name:
            self.tagdef.setdefault((kind(t), t.name), item)
        if isinstance(t, ca.Enum) and t.values is not None:
            if t.name:
                self.enumdef.setdefault(t.name, item)
            for e in t.values.enumerators:
                self.enumer.setdefault(e.name, item)


def kind(su):
    return "union" if type(su).__name__ == "Union" else "struct"


class Unknown(Exception):
    pass


class Emitter:
    """the declarations a draft needs out of the context, dependencies first"""

    def __init__(self, ctx, provided):
        from m2c_pycparser.c_generator import CGenerator
        self.ctx, self.ca, self.gen = ctx, ctx.ca, CGenerator
        self.provided = provided
        self.out, self.done, self.tstate, self.sstate = [], set(), {}, {}
        self.local = set()

    def text(self, node):
        return self.gen().visit(node)

    # walk a node; every name it needs from the context is emitted before the caller appends the node itself
    def walk(self, n, ptr=False):
        ca = self.ca
        if n is None:
            return
        if isinstance(n, ca.PtrDecl):
            return self.walk(n.type, True)
        if isinstance(n, ca.TypeDecl):
            return self.walk(n.type, ptr)
        if isinstance(n, ca.IdentifierType):
            for name in n.names:
                if name in self.ctx.typedef or name in self.provided:
                    self.need_type(name, ptr)
            return
        if isinstance(n, (ca.Struct, ca.Union)):
            if n.decls is None:
                return self.need_tag(kind(n), n.name, ptr)
            for d in n.decls:
                self.walk(d)
            return
        if isinstance(n, ca.Enum):
            if n.values is None:
                return self.need_enum(n.name)
            return self.walk(n.values)
        if isinstance(n, ca.ArrayDecl):
            self.walk(n.type)
            return self.walk(n.dim)
        if isinstance(n, ca.FuncDecl):
            self.walk(n.args)
            return self.walk(n.type)
        if isinstance(n, ca.StructRef):
            return self.walk(n.name)
        if isinstance(n, ca.ID):
            return self.need_id(n.name)
        for _, c in n.children():
            self.walk(c)

    def emit(self, item):
        ca = self.ca
        if id(item) in self.done:
            return
        self.done.add(id(item))
        t = item.type if isinstance(item, (ca.Typedef, ca.Decl)) else None
        if isinstance(item, ca.Typedef):
            self.tstate[item.name] = "full"
        if isinstance(t, ca.TypeDecl):
            t = t.type
        if isinstance(t, (ca.Struct, ca.Union)) and t.decls is not None and t.name:
            self.sstate[(kind(t), t.name)] = "full"
        self.walk(item.decl if isinstance(item, ca.FuncDef) else item)
        if isinstance(item, ca.FuncDef):
            self.walk(item.body)
            self.out.append(self.text(item))
        else:
            self.out.append(self.text(item) + ";")

    def need_id(self, name):
        if name in self.provided or name in self.local:
            return
        c = self.ctx
        if name in c.enumer:
            return self.emit(c.enumer[name])
        if name in c.ident:
            return self.emit(c.ident[name])
        raise Unknown(name)

    def need_enum(self, name):
        if name not in self.ctx.enumdef:
            raise Unknown("enum " + name)
        self.emit(self.ctx.enumdef[name])

    def struct_def(self, su):
        key = (kind(su), su.name)
        if self.sstate.get(key) == "full":
            return
        self.sstate[key] = "full"
        for d in su.decls:
            self.walk(d)
        self.out.append(self.text(su) + ";")

    def need_type(self, name, ptr):
        if name in self.provided or self.tstate.get(name) == "full":
            return
        ca = self.ca
        td = self.ctx.typedef[name]
        u = td.type.type if isinstance(td.type, ca.TypeDecl) else None
        if isinstance(u, (ca.Struct, ca.Union)) and u.name:
            fwd = f"typedef {kind(u)} {u.name} {name};"
            if ptr:
                if name not in self.tstate:
                    self.out.append(fwd)
                    self.tstate[name] = "fwd"
                return
            if u.decls is not None:
                if name not in self.tstate:
                    return self.emit(td)
                self.tstate[name] = "full"
                return self.struct_def(u)
            if name not in self.tstate:
                self.out.append(fwd)
            self.tstate[name] = "full"
            return self.need_tag(kind(u), u.name, False)
        self.emit(td)

    def need_tag(self, k, tag, ptr):
        if tag is None or self.sstate.get((k, tag)) == "full":
            return
        if ptr:
            if (k, tag) not in self.sstate:
                self.out.append(f"{k} {tag};")
                self.sstate[(k, tag)] = "fwd"
            return
        item = self.ctx.tagdef.get((k, tag))
        if item is None:
            raise Unknown(f"{k} {tag}")
        ca = self.ca
        if isinstance(item, ca.Typedef) and self.tstate.get(item.name) is None:
            return self.emit(item)
        su = item.type.type if isinstance(item.type, ca.TypeDecl) else item.type
        self.struct_def(su)


# ---- tools -----------------------------------------------------------------------------------------------------------
def mwcc():
    ver, flags = db.game_flags()
    return db.mwcc_cmd(ver), flags   # the command prefix: mwcceppc.exe, through wibo / wine off Windows


def run(cmd):
    r = subprocess.run(cmd, cwd=D, capture_output=True, text=True, errors="replace")
    return r.returncode, r.stdout + r.stderr


def obj_summary(path):
    """(sections {name: (size, align, bytes)}, functions [(name, value, size)], relocations of .text
    [(offset, type, symbol name, symbol section, addend)]) of a compiled object"""
    secs, syms, rels = gs.read_obj(path)
    ti = next(i for i, s in enumerate(secs) if s[0] == ".text")
    sec = {s[0]: (s[2], s[3], s[4]) for s in secs if s[1] in (1, 8) and s[0]}
    fn = [(s[0], s[1], s[2]) for s in syms if s[3] == 2 and s[5] == ti]
    rl = sorted((o, t, syms[si][0], syms[si][5], a) for o, t, si, a in rels.get(ti, []))
    return sec, fn, rl


def header_names():
    """the typedef names decomp/include/game/cr_game.h provides"""
    return set(re.findall(r"^typedef .*?\b(\w+);", (D / "include" / HEADER).read_text(encoding="utf-8"), re.M))


def draft_body(name):
    t = (DRAFTS / "c" / f"{name}.c").read_text(encoding="utf-8")
    lines = t.split("\n")
    assert lines[0].startswith("// m2c draft of ") and '#include "ctx.h"' in lines[1:4], name
    i = 1
    while lines[i].startswith("#"):   # the draft's own context lines (#define / #include "ctx.h" / #undef)
        i += 1
    return "\n".join(lines[i:]).lstrip("\n")


def marked(path):
    try:
        return path.read_text(encoding="utf-8").startswith(MARK)
    except OSError:
        return False


def file_text(name, a, b, decls, body):
    head = [
        f"{MARK}; edit by hand only after removing this line.",
        f" * {name}: .text 0x{a:08X}-0x{b:08X} ({b - a} bytes), linked from this file (batch 9: retail SHA-1, objdiff 100%).",
        " * One function per object: the game's own source files are not known yet, so this is not the original file.",
        " * The code is the function's m2c draft (docs/drafts.md), which compiles to main.dol's bytes. */",
        f'#include "{HEADER}"',
        "",
    ]
    if decls:
        head += ["/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h, the data sizes of symbols.txt, the drafts' own prototypes */"] + decls + [""]
    return "\n".join(head) + "\n" + body.rstrip("\n") + "\n"


# ---- main ------------------------------------------------------------------------------------------------------------
def main(argv):
    check = "--check" in argv
    rep = (DRAFTS / "draft_report.txt").read_text(encoding="utf-8").splitlines()
    cands = []
    status = {}
    for l in rep:
        m = re.match(r"^([0-9A-F]{8})  (\S+)\s+(\d+)  (.*?)\s*$", l)
        if m:
            status[m.group(2)] = m.group(4).split("  ")[0].strip()
            if "100% MATCH" in m.group(4):
                cands.append((int(m.group(1), 16), m.group(2), int(m.group(3))))
    ours = gs.read_syms(gs.OURS)
    addr = {}
    for n, sec, a, sz, isf in ours:
        addr.setdefault(n, set()).add(a)
    starts = sorted({a for n, sec, a, sz, isf in ours if sec == ".text"})
    funcs, _ = db.game_functions()
    where = {f[0]: (f[1], f[2], f[4]) for f in funcs}
    end = db.game_end()
    cc, flags = mwcc()
    ctx = Context(DRAFTS / "ctx_pp.c")
    provided = header_names()
    shutil.rmtree(WORK, ignore_errors=True)
    (WORK / "src").mkdir(parents=True)
    objs = {}

    def addr_of(n):
        s = addr.get(n, set())
        return next(iter(s)) if len(s) == 1 else None

    import reloc_audit as ra
    targets = {}
    for site, r in ra.Elf(str(D / "build/splitdata/main.elf")).relocs.items():
        targets.setdefault(r[1], []).append(site)

    def referenced(a, size):
        return any(not a <= x < a + size for x in targets.get(a, []))

    def check_one(c):
        a, name, size = c
        o = DRAFTS / "obj" / f"{name}.o"
        if name not in where or where[name][0] != a:
            return "not a game function of the current split at this address"
        sec, fn, rl = obj_summary(o)
        space = [f"{s} 0x{v[0]:X}" for s, v in sorted(sec.items()) if s in SPACE and v[0]]
        if space:
            return "has data of its own (" + ", ".join(space) + "): needs the data piece split too"
        if len(fn) != 1 or fn[0][0] != name or fn[0][2] != size or sec[".text"][0] != size:
            return "the object is not exactly the function (%s)" % ", ".join(f"{f[0]} 0x{f[2]:X}" for f in fn)
        if a % max(1, sec[".text"][1]):
            return f".text alignment {sec['.text'][1]} does not divide 0x{a:08X}"
        i = starts.index(a)
        nxt = starts[i + 1] if i + 1 < len(starts) else end
        if nxt != a + size:
            return f"padding after it (next symbol at 0x{nxt:08X}): the C object would not bring it"
        # 4. relocation targets against the original object's
        oa, osz, oobj = where[name]
        osec, osyms, orels = gs.read_obj(Path(oobj))
        oti = next(i for i, s in enumerate(osec) if s[0] == ".text")
        ofn = next(s for s in osyms if s[0] == name and s[3] == 2 and s[5] == oti)
        base = a - ofn[1]
        orl = {}
        for off, t, si, ad in orels.get(oti, []):
            if ofn[1] <= off < ofn[1] + size:
                sn, sv, _, _, _, sshn = osyms[si]
                tgt = (base + sv + ad) if sshn == oti else (addr_of(sn) + ad if addr_of(sn) is not None else None)
                orl[off - ofn[1]] = (t, tgt, sn)
        mine = {}
        for off, t, sn, sshn, ad in rl:
            if sshn not in (0,) and sn != name:
                return f"relocation to its own symbol {sn} at +0x{off:X}"
            tgt = a + ad if sn == name else (addr_of(sn) + ad if addr_of(sn) is not None else None)
            mine[off] = (t, tgt, sn)
        if set(mine) != set(orl):
            return "relocation sites differ from the original's"
        for off in sorted(mine):
            if mine[off][0] != orl[off][0]:
                return f"relocation type differs at +0x{off:X}"
            if mine[off][1] is None or mine[off][1] != orl[off][1]:
                return f"relocation at +0x{off:X} goes to {mine[off][2]}, the original's to {orl[off][2]}"
        # 5. the promoted file
        body = draft_body(name)
        e = Emitter(ctx, provided)
        try:
            pp = WORK / "src" / f"{name}.pp.c"
            pp.write_text(f'#include "{HEADER}"\n' + body, encoding="utf-8", newline="\n")
            rc, txt = run(cc + ["-E", "-nodefaults", "-proc", "gekko", "-nosyspath", "-i", "include",
                           str(pp.relative_to(D)).replace("\\", "/")])
            if rc:
                return "does not preprocess with the committed header: " + txt.strip().splitlines()[-1][:80]
            from m2c.c_types import parse_c
            ast, _ = parse_c(m2c_source(txt), ctx.scope)
            items = [it for it in ast.ext if not (isinstance(it, ctx.ca.Typedef) and it.name in provided)]
            for it in items:
                for n in walk_decls(ctx.ca, it):
                    e.local.add(n)
            for it in items:
                e.walk(it.decl if isinstance(it, ctx.ca.FuncDef) else it)
                if isinstance(it, ctx.ca.FuncDef):
                    e.walk(it.body)
        except Unknown as x:
            return f"uses {x} that the drafts' context does not declare"
        except Exception as x:   # a parse problem of the context reader: report, not fatal
            return f"declarations could not be generated ({type(x).__name__}: {str(x).splitlines()[0][:60]})"
        if not referenced(a, size):
            body = force_active(body, name)
        text = file_text(name, a, a + size, e.out, body)
        src = WORK / "src" / f"{name}.c"
        src.write_text(text, encoding="utf-8", newline="\n")
        obj = WORK / f"{name}.o"
        rc, txt = run(cc + ["-c"] + flags + ["-o", str(obj.relative_to(D)).replace("\\", "/"),
                                            str(src.relative_to(D)).replace("\\", "/")])
        if rc or not obj.exists():
            return "the promoted file does not compile: " + db.compile_error(txt)
        s2, f2, r2 = obj_summary(obj)
        if {k: v for k, v in s2.items() if v[0] and k != ".comment"} != \
                {k: v for k, v in sec.items() if v[0] and k != ".comment"} or f2 != fn or r2 != rl:
            return "the promoted file compiles to other bytes / relocations than the draft"
        objs[name] = text
        return None

    with cf.ThreadPoolExecutor(os.cpu_count() or 4) as ex:
        why = dict(zip([c[1] for c in cands], ex.map(check_one, cands)))
    promoted = [c for c in cands if why[c[1]] is None]
    left = [c for c in cands if why[c[1]] is not None]

    # decomp/src/game: write the promoted files; marked files of names that are no longer ours go
    changed = []
    existing = {p.stem: p for p in SRC.glob("*.c")} if SRC.exists() else {}
    kept, removed = [], []
    for stem, p in sorted(existing.items()):
        if not marked(p) or stem in objs:
            continue
        if addr_of(stem) is None:
            removed.append(stem)
            if not check:
                p.unlink()
                changed.append(stem)
        else:
            kept.append((stem, status.get(stem, "no draft")))
    for a, name, size in promoted:
        p = SRC / f"{name}.c"
        if p.exists() and not marked(p):
            continue   # hand-edited: left alone
        if not p.exists() or p.read_text(encoding="utf-8") != objs[name]:
            changed.append(name)
            if not check:
                SRC.mkdir(parents=True, exist_ok=True)
                p.write_text(objs[name], encoding="utf-8", newline="\n")
    nbytes = sum(c[2] for c in promoted)
    out = ["m2c drafts promoted to decomp/src/game (tools/promote_drafts.py; method: its docstring, plan.md step 58)",
           "one object per function (the game's own source files are not known); batch 9 links them and checks the SHA-1",
           "",
           f"drafts at 100% MATCH: {len(cands)}",
           f"promoted: {len(promoted)} ({nbytes} bytes = 0x{nbytes:X})",
           f"left out: {len(left)}",
           f"kept from earlier runs (draft no longer 100%): {len(kept)}",
           f"removed (function name no longer ours): {len(removed)}",
           "", "== promoted (address, function, bytes)"]
    out += [f"{a:08X}  {n:44} {s:5}" for a, n, s in promoted]
    out += ["", "== left out (address, function, bytes, why)"]
    out += [f"{a:08X}  {n:44} {s:5}  {why[n]}" for a, n, s in left]
    out += ["", "== kept (function, its draft's status now)"] + [f"{n:44} {s}" for n, s in kept]
    out += ["", "== removed"] + removed
    if check:
        print("\n".join(out))
    else:
        REPORT.write_bytes(("\r\n".join(out) + "\r\n").encode("utf-8"))
        print("\n".join(out[3:8]))
    print("changed" if changed else "unchanged", len(changed))
    return 0


def force_active(body, name):
    """the function's definition between #pragma force_active on / reset (nothing references it: mwld would strip it)"""
    lines = body.rstrip(NL).split(NL)
    i = next(k for k, l in enumerate(lines) if gs.CDEF.match(l) and gs.CDEF.match(l).group(1) == name)
    return NL.join(lines[:i] + ["/* nothing in main.dol references it: keep it (mwld strips unreferenced C functions) */",
                                "#pragma force_active on"] + lines[i:] + ["#pragma force_active reset"]) + NL


def walk_decls(ca, node):
    """every name a draft item declares itself (its externs, the function, parameters, locals)"""
    out = []
    if isinstance(node, ca.Decl) and node.name:
        out.append(node.name)
    if isinstance(node, ca.Typedef):
        out.append(node.name)
    if isinstance(node, ca.Enumerator):
        out.append(node.name)
    for _, c in node.children():
        out += walk_decls(ca, c)
    return out


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
