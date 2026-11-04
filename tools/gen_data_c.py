"""Write a data-only game unit: decomp/src/game/data/<first>_<last>.c, C definitions of game data objects whose bytes are
main.dol's (plan.md step 86, docs/decomp.md "Game data objects").

A game global of symbols/cr_globals.txt (our cr_g_ name, its section and address, its type from symbols/cr_types.h)
becomes a C definition: a zero-initialised object for .bss / .sbss, an initialiser generated from main.dol's bytes for
.data / .sdata / .rodata / .sdata2, element by element along the type (scalars as numbers, floats as the shortest
literal with the same bits, pointers as the name of ours at the target address that main.elf's relocation gives there,
function pointers as the function's name). Several globals that are contiguous in main.dol (or apart by the zero padding that aligns the next one) go into one file; the
compiler lays .data / .sdata / .rodata / .sdata2 out in declaration order and .bss / .sbss in REVERSE declaration order
(measured, GC/2.6 with the game flags), every section 8-aligned, so the file declares them in that order. The types
come out of the drafts' context (decomp/drafts/ctx_pp.c: the HSD / SDK headers + symbols/cr_types.h preprocessed; run
batch 12 once), the same way tools/promote_drafts.py declares what a promoted function needs.
The file is then compiled with the game flags and placed by tools/gen_dtk_splits.py's rule for data units
(data_unit_pieces: one piece start per section from the object's symbols at our addresses, bytes and relocations
main.dol's, dtk symbol starts at both ends); only a file that passes is written. Batch 9 repeats the check, links the
unit and verifies the retail SHA-1, objdiff 100% and the shift test.
A pointer into the middle of a symbol is written `&name[i]` when that symbol is a typed global of ours, an array of what
the pointer points at, and the target is the start of element i (step 90).
Not handled (the tool stops and says so): any other pointer into the middle of a symbol, a pointer slot holding a number, a
string literal (the file's own .rodata pool: the strings would have to come with the unit), unions, bit fields, a type
whose size is not dtk's object size (the remainder would be another object's), a global of 8 bytes or less in .data /
.bss (the compiler puts it in .sdata / .sbss).
Usage: python -I tools/gen_data_c.py NAME [NAME ...]            one unit of these cr_globals.txt globals (contiguous)
       python -I tools/gen_data_c.py --all                      write every unit of UNITS again (after a type / name change)
       python -I tools/gen_data_c.py --check NAME ...           generate and verify, write nothing
"""
import bisect
import copy
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path

G = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(G / "tools"))
sys.path.insert(0, str(G / "tools/m2c"))
import gen_dtk_splits as gs  # noqa: E402
import reloc_audit as ra  # noqa: E402

D = G / "decomp"
OUT = D / "src/game/data"
WORK = D / "build/gamedata/gen"
GLOBALS = G / "symbols/cr_globals.txt"
CTX = D / "drafts/ctx_pp.c"
MARK = "/* Custom Robo game data, written by tools/gen_data_c.py"
NL = "\n"
GLINE = re.compile(r"^\s*(cr_g_\w+)\s*=\s*(\.[\w.]+):0x([0-9A-Fa-f]+);\s*//\s*type:object\s+datatype:(\S+)")
# What to decide #3 (owner, Nov 2, 2026): a .bss / .sbss object no reader has typed yet may be defined as a sized byte
# array under dtk's own lbl_ name (zero bytes: no game bytes in the C), an interim form marked untyped in the unit's
# header; initialised data is never untyped. The list: symbols/bss_untyped.txt; a typed global moves to cr_globals.txt.
UNTYPED = G / "symbols/bss_untyped.txt"
ULINE = re.compile(r"^\s*(lbl_[0-9A-F]{8})\s*=\s*(\.[\w.]+):0x([0-9A-Fa-f]+);\s*//\s*type:object\s+datatype:u8\[(\d+)\]")
UNTYPED_NAMES = set()
# Types of objects that keep dtk's lbl_ names (no evidence names them; the code and the bytes prove their types):
# symbols/lbl_types.txt, written by tools/scan_lbl_types.py; `# unit` blocks as in bss_untyped.txt.
LTYPES = G / "symbols/lbl_types.txt"
LLINE = re.compile(r"^\s*([A-Za-z_]\w*)\s*=\s*(\.[\w.]+):0x([0-9A-Fa-f]+);\s*//\s*type:object\s+datatype:(\S+)")
DTYPE = re.compile(r"^([A-Za-z_]\w*)\s*(\**)\s*((?:\[\d+\])*)$")
# the units written so far (plan.md steps 86 and 90): --all writes every one again
UNITS = [
    # .bss, inside the glue span 0x803B8C00..0x803E66B4 (one file's data pool: these move only with it)
    ["cr_g_Script", "cr_g_SaveData", "cr_g_Scenario"],
    ["cr_g_AreaBoxes"],
    ["cr_g_Charas"],
    ["cr_g_ScriptReturnStack"],
    # .bss, other glue spans (the file system's, the pad's)
    ["cr_g_PackedArchives", "cr_g_FileSysReady"],
    ["cr_g_PadStatus", "cr_g_Pads"],
    # .bss outside every glue span (shift-test insertion points)
    ["cr_g_CharaEntries"],
    ["cr_g_MotionDataBit15", "cr_g_MotionData"],
    ["cr_g_MessageFiles", "cr_g_WindowStyle", "cr_g_TextRecords"],           # step 90: the window style and the text records after it
    ["cr_g_GameMode", "cr_g_SeqCurrent"],
    ["cr_g_UnlockBits"],
    # step 90 (naming round 5): the battle robos, the file system's loader + slots, the cameras, the buffers
    ["cr_g_BattleRobos"],                                                     # 4 x 0x33520
    ["cr_g_EntryTableLoad", "cr_g_FileSlots"],                                # cr_g_FileSlots' 4-aligned start comes with the object before it
    ["cr_g_Cams", "cr_g_CamSystem"],
    ["cr_g_CardWorkArea"],
    ["cr_g_ScenarioBuffer"],
    ["cr_g_ModelSlots"],
    # initialised data
    ["cr_g_BattleMotionSets"],                                                # .data, inside a glue span
    ["cr_g_FileDescs"],                                                       # .data, outside (43 pointers; dtk's 0xC0 object ends in 0x14 zero bytes)
    ["cr_g_BodyCount", "cr_g_LegCount", "cr_g_GunCount", "cr_g_BombCount"],   # .sdata, the section start
    # step 90: the robo tables (the records cr_BattleRobo_InitFromLoadout copies), the part menu lists
    ["cr_g_BattleLoadouts", "cr_g_BodyData", "cr_g_BattleParams", "cr_g_BodyStats", "cr_g_LegData"],   # cr_g_BodyData's motion_set -> &cr_g_BattleMotionSets[i]
    ["cr_g_GunData", "cr_g_BombData"],
    ["cr_g_PartOrderLeg", "cr_g_PartOrderLists"],                             # 2 bytes of alignment padding between them
    # step 90: tables dtk saw as one object with what follows (cuts: symbols/data_splits.txt)
    # the six sequence tables are one pool: cr_Game_Update reaches cr_g_SeqEnter / cr_g_SeqLeave as cr_g_SeqFrameA's base
    # + 0xF0 / + 0x12C, so they move (and are kept by the linker) only together; cr_g_SeqNames points at the SEQ_ strings
    # of .rodata by their symbols (the strings stay in their automatic object)
    ["cr_g_SeqFrameA", "cr_g_SeqNames", "cr_g_SeqFrameB", "cr_g_SeqFrameC", "cr_g_SeqEnter", "cr_g_SeqLeave"],
    ["cr_g_CamModeFuncs"],
    ["cr_g_CharaYOffset", "cr_g_CharaMotionSets"],
    # Nov 3: the motion name table (its strings stay in their automatic object, named by their symbols); 8-aligned now
    # that the motion info records before it are a unit (symbols/lbl_types.txt)
    ["cr_g_MotionNames"],
    # Nov 3: the pod table, after the bomb table (its end is a cut in symbols/data_splits.txt: batch 9 applies it first)
    ["cr_g_PodData"],
]
INTS = {"char": (1, True), "signed char": (1, True), "unsigned char": (1, False), "short": (2, True),
        "unsigned short": (2, False), "int": (4, True), "unsigned int": (4, False), "unsigned": (4, False),
        "long": (4, True), "unsigned long": (4, False), "long long": (8, True), "unsigned long long": (8, False),
        "s8": (1, True), "u8": (1, False), "s16": (2, True), "u16": (2, False), "s32": (4, True), "u32": (4, False),
        "s64": (8, True), "u64": (8, False), "BOOL": (4, True), "size_t": (4, False)}
FLOATS = {"float": 4, "double": 8, "f32": 4, "f64": 8, "vf32": 4, "vf64": 8}


class Stop(Exception):
    pass


def read_globals():
    out = {}
    for ln in GLOBALS.read_text(encoding="utf-8").splitlines():
        m = GLINE.match(ln)
        if m:
            out[m.group(1)] = (m.group(2), int(m.group(3), 16), m.group(4))
    if UNTYPED.exists():
        for ln in UNTYPED.read_text(encoding="utf-8").splitlines():
            m = ULINE.match(ln)
            if not m:
                continue
            n, sec, a, size = m.group(1), m.group(2), int(m.group(3), 16), int(m.group(4))
            if sec not in (".bss", ".sbss"):
                raise SystemExit(f"{UNTYPED.name}: {n} is in {sec}: only .bss / .sbss may be untyped (What to decide #3)")
            if n in out:
                raise SystemExit(f"{UNTYPED.name}: {n} is also in cr_globals.txt: a typed global leaves the untyped list")
            out[n] = (sec, a, f"u8[{size}]")
            UNTYPED_NAMES.add(n)
    if LTYPES.exists():
        for ln in LTYPES.read_text(encoding="utf-8").splitlines():
            m = LLINE.match(ln)
            if m:
                if m.group(1) in out:
                    raise SystemExit(f"{LTYPES.name}: {m.group(1)} is listed twice (or also untyped / in cr_globals.txt)")
                out[m.group(1)] = (m.group(2), int(m.group(3), 16), m.group(4))
    return out


class Types:
    """sizes, alignments and members of the context's types (natural alignment, -align powerpc)"""

    def __init__(self, ctx):
        self.ctx, self.ca = ctx, ctx.ca

    def of_name(self, name):
        if name in INTS:
            return {"k": "int", "size": INTS[name][0], "signed": INTS[name][1], "align": INTS[name][0]}
        if name in FLOATS:
            return {"k": "float", "size": FLOATS[name], "align": FLOATS[name]}
        if name == "void":
            return {"k": "void", "size": 0, "align": 1}
        td = self.ctx.typedef.get(name)
        if td is None:
            raise Stop(f"type {name} is not declared in the drafts' context")
        return self.of(td.type)

    def of(self, t):
        ca = self.ca
        if isinstance(t, ca.TypeDecl):
            t = t.type
            if isinstance(t, ca.IdentifierType):
                names = [n for n in t.names if n not in ("const", "volatile", "signed")] or ["int"]
                return self.of_name(" ".join(names) if " ".join(names) in INTS else names[-1])
            if isinstance(t, ca.Enum):
                return {"k": "int", "size": 4, "signed": True, "align": 4}
            if isinstance(t, ca.Union):
                raise Stop(f"union {t.name}: not handled")
            if isinstance(t, ca.Struct):
                su = t
                if su.decls is None:
                    item = self.ctx.tagdef.get(("struct", su.name))
                    if item is None:
                        raise Stop(f"struct {su.name} has no definition in the context")
                    su = item.type.type if isinstance(item.type, ca.TypeDecl) else item.type
                members, off, align = [], 0, 1
                for d in su.decls:
                    if getattr(d, "bitsize", None) is not None:
                        raise Stop(f"struct {su.name}: bit field {d.name}")
                    mt = self.of(d.type)
                    off = (off + mt["align"] - 1) & -mt["align"]
                    members.append((d.name, off, mt))
                    off += mt["size"]
                    align = max(align, mt["align"])
                return {"k": "struct", "name": su.name, "members": members, "size": (off + align - 1) & -align, "align": align}
            raise Stop(f"type form {type(t).__name__} not handled")
        if isinstance(t, ca.PtrDecl):
            inner = t.type
            if isinstance(inner, ca.FuncDecl):
                return {"k": "fptr", "size": 4, "align": 4, "func": inner}
            return {"k": "ptr", "size": 4, "align": 4, "to": inner}
        if isinstance(t, ca.ArrayDecl):
            if not isinstance(t.dim, ca.Constant):
                raise Stop("an array whose size is not a constant")
            n = int(t.dim.value.rstrip("uUlL"), 0)
            et = self.of(t.type)
            return {"k": "array", "size": n * et["size"], "align": et["align"], "n": n, "elem": et}
        if isinstance(t, ca.FuncDecl):
            raise Stop("a function type as data")
        raise Stop(f"type form {type(t).__name__} not handled")

    def pointee_name(self, node):
        """the typedef / base name a pointer points at, for the extern declaration of its target"""
        ca = self.ca
        if isinstance(node, ca.TypeDecl) and isinstance(node.type, ca.IdentifierType):
            names = [n for n in node.type.names if n not in ("const", "volatile")]
            return " ".join(names)
        if isinstance(node, ca.TypeDecl) and isinstance(node.type, ca.Struct):
            return f"struct {node.type.name}"
        return None


class Gen:
    def __init__(self):
        import promote_drafts as pd
        self.pd = pd
        self.ctx = pd.Context(CTX)
        self.types = Types(self.ctx)
        self.provided = pd.header_names()
        self.emitter = pd.Emitter(self.ctx, self.provided, full="deep")
        self.ours = gs.read_syms(gs.OURS)
        self.name_addr, self.at = {}, {}
        for n, sec, a, sz, isf in self.ours:
            self.name_addr.setdefault(n, set()).add(a)
            self.at.setdefault(a, (n, sec, sz, isf))
        p = D / "build/splitdata/main.elf"
        if not p.exists():
            p = D / "build/GXCE01/main.elf"
        if not p.exists():
            raise SystemExit("no decomp/build/splitdata/main.elf or build/GXCE01/main.elf: run batch 9 first")
        self.elf = ra.Elf(str(p))
        self.dobjs = ra.Objects(self.elf)
        self.globals = read_globals()
        # a name of cr_globals.txt that batch 9 has not applied yet: the dtk symbol at its address takes it here (the
        # config symbols.txt keeps the old name until batch 9, whose sync_game_names.py renames the promoted files from it)
        for n, (sec, a, dtype) in self.globals.items():
            if n not in self.name_addr and a in self.at and not self.at[a][3]:
                old = self.at[a][0]
                self.name_addr[n] = {a}
                self.at[a] = (n,) + self.at[a][1:]
                self.ours = [(n if (x[0], x[2]) == (old, a) else x[0],) + tuple(x[1:]) for x in self.ours]
        self.externs = {}   # name -> declaration line
        self.unit_at, self.forward, self.cur = {}, {}, 0

    def bytes_at(self, a, n):
        for s in self.elf.alloc:
            if s[3] <= a and a + n <= s[3] + s[4]:
                return self.elf.d[s[5] + a - s[3]: s[5] + a - s[3] + n] if s[2] == 1 else bytes(n)
        raise Stop(f"0x{a:08X} is not in main.dol")

    # ---- values --------------------------------------------------------------------------------------------------------
    def value(self, t, a, indent):
        k = t["k"]
        if k == "int":
            if any(self.elf.relocs.get(x) for x in range(a & ~3, a + t["size"], 4)):
                raise Stop(f"0x{a:08X}: a relocated word read as a number (the type needs a pointer there)")
            v = int.from_bytes(self.bytes_at(a, t["size"]), "big", signed=t["signed"])
            return str(v) if -1000 < v < 1000 else f"0x{v & ((1 << (8 * t['size'])) - 1):X}"
        if k == "float":
            if self.elf.relocs.get(a):
                raise Stop(f"0x{a:08X}: a relocated word read as a float")
            b = self.bytes_at(a, t["size"])
            fmt = ">f" if t["size"] == 4 else ">d"
            x = struct.unpack(fmt, b)[0]
            if x != x or x in (float("inf"), float("-inf")):
                raise Stop(f"0x{a:08X}: a NaN / infinite float")
            for p in range(1, 18):
                s = f"{x:.{p}g}"
                if struct.pack(fmt, float(s)) == b:
                    if "." not in s and "e" not in s:
                        s += ".0"
                    return s + ("f" if t["size"] == 4 else "")
            raise Stop(f"0x{a:08X}: no literal gives the float bits {b.hex()}")
        if k in ("ptr", "fptr"):
            r = self.elf.relocs.get(a)
            w = struct.unpack(">I", self.bytes_at(a, 4))[0]
            if r is None:
                if w == 0:
                    return "NULL"
                if k == "ptr" and w < 0x10000 and self.types.pointee_name(t["to"]) == "void":
                    # a marker value in a void * (cr_FileDesc.packed: 1 = a single file): no relocation, the same word
                    return f"(void *) {w}"
                raise Stop(f"0x{a:08X}: a pointer slot holds 0x{w:08X} without a relocation")
            tgt = r[1]
            if tgt not in self.at or (re.match(r"lbl_[0-9A-F]{8}$", self.at[tgt][0]) and self.container(tgt)):
                # (a dtk default symbol inside a typed global of ours is one of its elements: batch 9 drops it, SYM_DROP)
                return self.element_pointer(a, tgt, t)
            n, sec, sz, isf = self.at[tgt]
            if n in self.unit_at and not isf:
                # an object of this unit: by its name; declared ahead only when it is defined further down
                ua, arr, udecl = self.unit_at[n]
                if ua > self.cur:
                    self.forward[n] = f"extern {udecl};"
                return n if arr else f"&{n}"
            if k == "fptr":
                if not isf:
                    raise Stop(f"0x{a:08X}: a function pointer to the data symbol {n}")
                self.declare_function(n, t["func"])
            else:
                if isf:
                    raise Stop(f"0x{a:08X}: a data pointer to the function {n}")
                self.declare_data(n, t["to"])
            return n
        if k == "array":
            et, n = t["elem"], t["n"]
            vals = [self.value(et, a + i * et["size"], indent + 1) for i in range(n)]
            return self.braces(vals, indent, et["k"] in ("int", "float", "ptr", "fptr"))
        if k == "struct":
            vals = []
            end = 0
            for name, off, mt in t["members"]:
                if any(self.bytes_at(a + end, off - end)):
                    raise Stop(f"0x{a:08X}+0x{end:X}: non-zero padding inside struct {t['name']}")
                vals.append(self.value(mt, a + off, indent + 1))
                end = off + mt["size"]
            if any(self.bytes_at(a + end, t["size"] - end)):
                raise Stop(f"0x{a:08X}+0x{end:X}: non-zero padding at the end of struct {t['name']}")
            return self.braces(vals, indent, all(mt["k"] in ("int", "float", "ptr", "fptr") for _, _, mt in t["members"]))
        raise Stop(f"value of a {k}")

    def container(self, tgt):
        """the typed global of ours (cr_globals.txt) whose bytes hold tgt after its start, or None"""
        if not hasattr(self, "_spans"):
            self._spans = []
            for n, (sec, base, dtype) in self.globals.items():
                if n not in self.name_addr:
                    continue
                m = DTYPE.match(dtype)
                try:
                    size = 4 if m.group(2) else self.types.of_name(m.group(1))["size"]
                except (Stop, AttributeError):
                    continue
                for d in re.findall(r"\[(\d+)\]", m.group(3)):
                    size *= int(d)
                self._spans.append((base, base + size, n))
            self._spans.sort()
            self._starts = [x[0] for x in self._spans]
        i = bisect.bisect_left(self._starts, tgt) - 1   # typed globals do not overlap: the one starting before tgt
        return self._spans[i][2] if i >= 0 and tgt < self._spans[i][1] else None

    def element_pointer(self, a, tgt, t):
        """a pointer into the middle of a symbol: `&name[i]` when the symbol is a typed global of ours (cr_globals.txt)
        whose type is an array of what the pointer points at and the target is the start of element i (step 90:
        cr_g_BodyData's records point at their cr_g_BattleMotionSets entry); anything else stops"""
        if t["k"] != "ptr":
            raise Stop(f"0x{a:08X}: a function pointer into the middle of a symbol (0x{tgt:08X})")
        n = self.container(tgt)
        if n is None:
            raise Stop(f"0x{a:08X}: points into the middle of a symbol (0x{tgt:08X}) that is no typed global of ours")
        sec, base, dtype = self.globals[n]
        m = DTYPE.match(dtype)
        pointee = self.types.pointee_name(t["to"])
        if not m or not m.group(3) or m.group(2) or pointee != m.group(1):
            raise Stop(f"0x{a:08X}: points into {n} ({dtype}), not at an element of type {pointee}")
        esz = self.types.of_name(m.group(1))["size"]
        dims = [int(d) for d in re.findall(r"\[(\d+)\]", m.group(3))]
        if len(dims) != 1 or (tgt - base) % esz or not 0 <= (tgt - base) // esz < dims[0]:
            raise Stop(f"0x{a:08X}: points at 0x{tgt:08X}, not at the start of an element of {n} ({dtype})")
        self.declare_data(n, t["to"])
        return f"&{n}[{(tgt - base) // esz}]"

    def braces(self, vals, indent, flat):
        pad = "    " * (indent + 1)
        if flat:
            lines, cur = [], ""
            for v in vals:
                if cur and len(cur) + len(v) + 2 > 100:
                    lines.append(cur.rstrip())
                    cur = ""
                cur += v + ", "
            if cur:
                lines.append(cur.rstrip())
            if len(lines) == 1 and len(lines[0]) < 60:
                return "{ " + lines[0].rstrip(",") + " }"
            return "{" + NL + NL.join(pad + ln for ln in lines).rstrip(",") + NL + "    " * indent + "}"
        return "{" + NL + NL.join(pad + v + "," for v in vals).rstrip(",") + NL + "    " * indent + "}"

    # ---- declarations ---------------------------------------------------------------------------------------------------
    def declare_function(self, name, funcdecl):
        ca = self.ctx.ca
        fd = copy.deepcopy(funcdecl)
        node = fd.type
        while not isinstance(node, ca.TypeDecl):
            node = node.type
        node.declname = name
        self.emitter.walk(fd)
        decl = ca.Decl(name, [], None, [], [], [], fd, None, None, None)
        self.externs[name] = "extern " + self.emitter.text(decl) + ";"

    def declare_data(self, name, pointee):
        ca = self.ctx.ca
        tn = self.types.pointee_name(pointee)
        if tn is None:
            raise Stop(f"a pointer to {name}: its pointee type form is not handled")
        if tn == "void":
            tn = "u8"
        else:
            self.emitter.walk(pointee)   # the pointee's full definition (an extern array needs a complete element type)
        self.externs[name] = f"extern {tn} {name}[];"

    def declaration(self, name, dtype):
        m = DTYPE.match(dtype)
        if not m:
            raise Stop(f"{name}: datatype {dtype} not understood")
        base, stars, dims = m.group(1), m.group(2), m.group(3)
        t = self.types.of_name(base)
        if base in self.ctx.typedef and base not in self.provided:
            # the base type's definition (a pointer's pointee may stay a forward declaration until a target needs it)
            self.emitter.need_type(base, bool(stars))
        for _ in stars:
            pointee = self.ctx.ca.TypeDecl(None, [], None, self.ctx.ca.IdentifierType([base]))
            t = {"k": "ptr", "size": 4, "align": 4, "to": pointee}
        for d in reversed(re.findall(r"\[(\d+)\]", dims)):
            n = int(d)
            t = {"k": "array", "size": n * t["size"], "align": t["align"], "n": n, "elem": t}
        return f"{base} {stars}{name}{dims}", t

    # ---- the unit -------------------------------------------------------------------------------------------------------
    def unit(self, names):
        items = []
        for n in names:
            if n not in self.globals:
                raise Stop(f"{n} is not a global of symbols/cr_globals.txt")
            sec, a, dtype = self.globals[n]
            if n not in self.name_addr or len(self.name_addr[n]) != 1 or a not in self.name_addr[n]:
                raise Stop(f"{n}: the config symbols.txt has no symbol of that name at 0x{a:08X}")
            decl, t = self.declaration(n, dtype)
            # (dtk's object size may differ: data_unit_pieces decides, in verify(), whether the C's size is proven)
            if t["size"] <= 8 and sec in (".data", ".bss", ".rodata"):
                raise Stop(f"{n}: {t['size']} bytes in {sec}: the compiler would put it in the small-data section")
            items.append((a, n, sec, dtype, decl, t))
        items.sort()
        for (a0, n0, s0, _, _, t0), (a1, n1, s1, _, _, t1) in zip(items, items[1:]):
            if s0 != s1:
                raise Stop(f"{n0} ({s0}) and {n1} ({s1}) are in different sections: one unit holds one section's run")
            # contiguous, or the gap is the compiler's alignment padding before the next one (zero bytes)
            e0 = a0 + t0["size"]
            if not (e0 <= a1 == (e0 + t1["align"] - 1) & -t1["align"]) or any(self.bytes_at(e0, a1 - e0)):
                raise Stop(f"{n0} ends at 0x{e0:08X}, {n1} starts at 0x{a1:08X}: not contiguous")
        sec = items[0][2]
        zero = sec in (".bss", ".sbss")
        self.unit_at = {n: (a, t["k"] == "array", decl) for a, n, s, dtype, decl, t in items}
        self.forward = {}
        defs = []
        for a, n, s, dtype, decl, t in items:
            self.cur = a
            if zero:
                defs.append((a, f"{decl};"))
            else:
                defs.append((a, f"{decl} = {self.value(t, a, 0)};"))
        order = sorted(defs, reverse=zero)
        first, last = items[0], items[-1]
        stem = first[1] if len(items) == 1 else f"{first[1]}_{last[1]}"
        head = [f"{MARK}; regenerate it, do not edit (python -I tools/gen_data_c.py --all).",
                f" * A data-only game unit (plan.md step 86, docs/decomp.md \"Game data objects\"): {sec} 0x{first[0]:08X}-0x{last[0] + last[5]['size']:08X}, "
                f"{len(items)} object{'s' if len(items) != 1 else ''}, linked from this file (batch 9: retail SHA-1, objdiff 100%, shift test).",
                " * Types: symbols/cr_types.h (through the drafts' context), names and addresses: symbols/cr_globals.txt;"
                + (" the initialisers are main.dol's bytes, element by element." if not zero else " zero-initialised, as main.dol has it."),
                " * The compiler lays " + (".bss / .sbss out in reverse declaration order" if zero else
                                           ".data / .sdata / .rodata / .sdata2 out in declaration order")
                + ", so the definitions are in that order:"]
        listed = list(reversed(items) if zero else items)
        if len(listed) > 40:
            kinds = {}
            for a, n, s, dtype, decl, t in listed:
                kinds[re.sub(r"\[\d+\]", "[]", dtype)] = kinds.get(re.sub(r"\[\d+\]", "[]", dtype), 0) + 1
            head.append(" *   " + ", ".join(f"{c} x {k}" for k, c in sorted(kinds.items()))
                        + " (each object's type: symbols/lbl_types.txt / cr_globals.txt / bss_untyped.txt)")
            listed = []
        for a, n, s, dtype, decl, t in listed:
            head.append(f" *   0x{a:08X} {n} ({dtype}, 0x{t['size']:X} bytes)"
                        + (", UNTYPED (symbols/bss_untyped.txt: size and place only, no reader typed it yet)" if n in UNTYPED_NAMES else ""))
        head.append(" * The game's own source files are not known: this unit is one contiguous run of its data, not the original file. */")
        text = NL.join(head) + NL + '#include "game/cr_game.h"' + NL + NL
        if self.emitter.out:
            text += "/* from the drafts' context (docs/drafts.md): HSD / SDK headers, symbols/cr_types.h */" + NL + NL.join(self.emitter.out) + NL + NL
        if self.externs:
            text += "/* what the pointers point at (names of decomp/config/GXCE01/symbols.txt) */" + NL + NL.join(v for k, v in sorted(self.externs.items())) + NL + NL
        if self.forward:
            text += "/* objects of this unit that a pointer above their definition names */" + NL + NL.join(v for k, v in sorted(self.forward.items())) + NL + NL
        # mwld strips a data symbol nothing names (dtk's asm objects keep theirs): a table the code reaches as another
        # table's base + offset (cr_g_SeqEnter = cr_g_SeqFrameA + 0xF0) would go, and everything after it would move
        text += ("/* the code may reach an object as another's base + offset only: keep them all (mwld strips unnamed data) */" + NL
                 + "#pragma force_active on" + NL + NL.join(d for a, d in order) + NL + "#pragma force_active reset" + NL)
        return stem, text, [(a, n, t["size"]) for a, n, s, dtype, decl, t in items]

    def verify(self, stem, text):
        """compile the text with the game flags and place it with gen_dtk_splits' rule -> (pieces, why)"""
        WORK.mkdir(parents=True, exist_ok=True)
        src = WORK / f"{stem}.c"
        src.write_text(text, encoding="utf-8", newline=NL)
        o = WORK / f"{stem}.o"
        r = subprocess.run(gs.game_compiler() + ["-c", "-o", str(o), str(src)], cwd=D, stdout=subprocess.PIPE,
                           stderr=subprocess.STDOUT, text=True)
        if r.returncode:
            return {}, "does not compile:\n" + r.stdout[-2000:]
        pieces, defs, salign, over, why = gs.data_unit_pieces(o, self.ours, self.elf, self.dobjs)
        return pieces, why


def untyped_units():
    """the units of symbols/bss_untyped.txt: each block after a `# unit` line is one contiguous run"""
    units = []
    if UNTYPED.exists():
        for ln in UNTYPED.read_text(encoding="utf-8").splitlines():
            if ln.startswith("# unit"):
                units.append([])
            else:
                m = ULINE.match(ln)
                if m and units:
                    units[-1].append(m.group(1))
    return [u for u in units if u]


def typed_units():
    """the units of symbols/lbl_types.txt: each `# unit` block; `= cr_g_X` puts a global of cr_globals.txt in it"""
    units = []
    if LTYPES.exists():
        for ln in LTYPES.read_text(encoding="utf-8").splitlines():
            if ln.startswith("# unit"):
                units.append([])
            elif units and (LLINE.match(ln) or ln.startswith("= ")):
                units[-1].append(LLINE.match(ln).group(1) if LLINE.match(ln) else ln[2:].strip())
    return [u for u in units if u]


def scan_untyped():
    """--scan-untyped: write symbols/bss_untyped.txt from the .bss / .sbss automatic objects of the last batch 9 build
    (objdiff's report.json): dtk's lbl_ objects in contiguous runs, cut where a gap or a typed name of ours lies between
    two, where an object breaks the small-data rule (.bss over 8 bytes, .sbss 8 or less), and moved on to an 8-aligned
    start; the users of each object are the functions of batch 9's asm that take its address. Then --all checks them."""
    import collections
    import glob
    import json
    rep = json.loads((D / "build/GXCE01/report.json").read_text(encoding="utf-8"))
    ranges = []
    for u in rep["units"]:
        # dtk's automatic objects, and our own data units (so a second scan, after the runs are linked, finds them again:
        # the scan is a fixpoint; typed names of ours are still left out below)
        if not u["metadata"].get("auto_generated") and not u["name"].startswith("main/game/data/"):
            continue
        for s in u["sections"]:
            if s["name"] in (".bss", ".sbss") and int(s["size"]):
                a = int(s["metadata"]["virtual_address"])
                ranges.append((a, a + int(s["size"]), s["name"]))
    typed = {a for _, (sec, a, _) in ((n, v) for n, v in read_globals().items() if n not in UNTYPED_NAMES)}
    syms = []
    for ln in gs.OURS.read_text(encoding="utf-8").splitlines():
        m = re.match(r"(\S+) = (\.s?bss):0x([0-9A-F]+); // type:object size:0x([0-9A-F]+)", ln)
        if m:
            a = int(m.group(3), 16)
            if any(lo <= a < hi and s == m.group(2) for lo, hi, s in ranges):
                syms.append((a, m.group(1), m.group(2), int(m.group(4), 16)))
    syms.sort()
    runs, cur = [], []
    for a, n, sec, size in syms:
        ok = n.startswith("lbl_") and a not in typed and ((size > 8) if sec == ".bss" else (size <= 8))
        if cur and (not ok or cur[-1][0] + cur[-1][3] != a or cur[-1][2] != sec):
            runs.append(cur)
            cur = []
        if ok and (cur or a % 8 == 0):
            cur.append((a, n, sec, size))
    if cur:
        runs.append(cur)
    # the compiled section is padded to 8 bytes: a run's end must be 8-aligned or no dtk symbol may start before the
    # next 8-byte boundary (dtk stops on a split that ends within a symbol: batch 9, Nov 3, 0x8044F44E)
    starts = sorted(int(m.group(1), 16) for m in re.finditer(r"= \.s?bss:0x([0-9A-F]+);", gs.OURS.read_text(encoding="utf-8")))
    fixed = []
    for r in runs:
        while r:
            end = r[-1][0] + r[-1][3]
            nxt = starts[bisect.bisect_left(starts, end)] if bisect.bisect_left(starts, end) < len(starts) else 1 << 40
            if end % 8 == 0 or nxt >= (end + 7) & ~7:
                break
            r = r[:-1]
        if r:
            fixed.append(r)
    runs = fixed
    names = {n for r in runs for _, n, _, _ in r}
    users = collections.defaultdict(set)
    pat = re.compile(r"\b(lbl_[0-9A-F]{8})@(?:ha|l|sda21)")
    for f in glob.glob(str(D / "build/GXCE01/asm/*_text.s")):
        fn = None
        for ln in open(f, encoding="utf-8", errors="replace"):
            if ln.startswith(".fn "):
                fn = ln.split()[1].rstrip(",")
            for m in pat.finditer(ln):
                if fn and m.group(1) in names:
                    users[m.group(1)].add(fn)
    out = ["# Untyped .bss / .sbss objects defined from C as sized byte arrays under dtk's own name (What to decide #3, owner,",
           "# Nov 2, 2026): an interim form, zero bytes (no game bytes in the C), size and place only. Written by",
           "# `python -I tools/gen_data_c.py --scan-untyped` (only .bss / .sbss; initialised data is never untyped); each `# unit`",
           "# block is one contiguous run, one unit of `--all`. When a reader types an object, it moves to cr_globals.txt with a",
           "# name and a why: line and the scan leaves it out. users: the functions that take its address (batch 9's asm)."]
    for r in runs:
        out.append(f"# unit {r[0][2]} 0x{r[0][0]:08X}-0x{r[-1][0] + r[-1][3]:08X} ({len(r)} objects, 0x{sum(x[3] for x in r):X} bytes)")
        for a, n, sec, size in r:
            u = sorted(users[n])
            us = (", ".join(u[:6]) + (f" +{len(u) - 6}" if len(u) > 6 else "")) if u else "none by symbol (reached from a neighbour's base)"
            out.append(f"{n} = {sec}:0x{a:08X}; // type:object datatype:u8[{size}] // users: {us}")
    UNTYPED.write_text("\n".join(out) + "\n", encoding="utf-8", newline="\r\n")
    print(f"symbols/bss_untyped.txt: {len(runs)} runs, {sum(len(r) for r in runs)} objects, "
          f"0x{sum(x[3] for r in runs for x in r):X} bytes")


def main(argv):
    if "--scan-untyped" in argv:
        scan_untyped()
        return 0
    check = "--check" in argv
    args = [a for a in argv if not a.startswith("--")]
    units = UNITS + typed_units() + untyped_units() if "--all" in argv else [args]
    if not units or not units[0]:
        sys.exit(__doc__)
    if not CTX.exists():
        sys.exit(f"{CTX} is missing: run tools/batches/batch12_drafts.ps1 (or tools/draft_batch.py) once for the drafts' context")
    bad = 0
    written = set()
    for names in units:
        g = Gen()
        try:
            stem, text, items = g.unit(names)
            pieces, why = g.verify(stem, text)
        except Stop as x:
            print(f"{' '.join(names)}: NOT written: {x}")
            bad += 1
            continue
        if why:
            print(f"{stem}: NOT written: {why}")
            bad += 1
            continue
        where = ", ".join(f"{s} 0x{a:08X}-0x{b:08X}" for s, (a, b) in sorted(pieces.items()))
        if check:
            print(f"{stem}: OK ({where}); not written (--check)")
            continue
        OUT.mkdir(parents=True, exist_ok=True)
        written.add(stem)
        p = OUT / f"{stem}.c"
        old = p.read_text(encoding="utf-8") if p.exists() else None
        p.write_text(text, encoding="utf-8", newline=NL)
        print(f"decomp/src/game/data/{stem}.c: {'written' if old is None else 'unchanged' if old == text else 'rewritten'} ({where}, "
              f"{len(items)} object{'s' if len(items) != 1 else ''})")
    if "--all" in argv and not check and not bad:
        # a generated unit no list produces any more (a type moved, a run changed): removed
        for p in sorted(OUT.glob("*.c")):
            if p.stem not in written and p.read_text(encoding="utf-8").startswith(MARK):
                p.unlink()
                print(f"decomp/src/game/data/{p.name}: removed (no unit of the lists produces it)")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
