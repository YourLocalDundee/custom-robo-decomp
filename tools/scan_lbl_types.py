"""Types for data objects that keep dtk's lbl_ names: symbols/lbl_types.txt (Nov 3, 2026).

A data object no evidence names keeps dtk's lbl_ name (the naming policy); its TYPE can still be proven by the code
and the bytes. This tool writes those types for whole record families, from the last batch 9 build's asm
(the config symbols.txt and main.elf's words and relocations), and tools/gen_data_c.py reads the list next to
symbols/cr_globals.txt: each `# unit` block is one contiguous run, one data unit of `gen_data_c.py --all`.

Families:
  motion_info   everything cr_g_MotionInfo (0x80205C94, 1095 pointers) reaches: .data 0x801FA188-0x80205C94, exactly
                contiguous, ending at cr_g_MotionInfo itself. Each pointed-at object is a cr_MotionTag list (pairs of
                tag, data; ended by tag 0 with NULL data); tag 0's data is a cr_MotionHeader, tags 6 / 7 / 10 a
                cr_MotionEvent, tag 8 a cr_MotionEventList whose values point at a u32[9]; tags 1 / 2 / 3 / 9 point
                into .sdata (other units). The tool stops on any object outside these roles or of another size.
  sqrt_table    lbl_801E90B0 (.data, 8192 f32 + 16 zero bytes, read by fn_8001B42C): sqrt(i) for every i, within 3 ulp of the
                correctly rounded value (the game's own generator); checked element by element.
  ax_data       the AX library's data with its SDK names (dtk signatures): __AXCompressorTable (u16[3360]) and
                axDspSlave (the DSP microcode, u16[3936]), contiguous from 0x80244AA0 (8-aligned) to 0x802483A0.
  file_descs    the file system's descriptor arrays: each of cr_g_FileDescs' 43 pointers -> cr_FileDesc[n], each
                relocated `packed` -> cr_PackedEntry[n]; written as the family's contiguous, 8-aligned runs.
  stage_data    everything cr_g_StageData (40 x 0x340) reaches: 4 cr_KeyPath (six tracks of keys + a float count)
                and their cr_Key7 lists (the float count equals each list's length), contiguous up to the table.
  key_paths     any other (keys, f32 count) object whose counts equal its lists' lengths: cr_KeyTrack[n], the lists
                cr_Key7[k], an object of pointers to them cr_KeyTrack*[n] (the camera modes' paths).
  records       single tables typed from their readers: lbl_8022B3B8 = cr_Record8C[304], lbl_802359F8 = cr_Record10[64],
                lbl_802362A0 = cr_CardImage[1] (the memory card image).
  window_data   the text window code's .data, typed from its readers (cr_TextRecord lists, cr_TextRecordList, cr_TextBlock,
                the code tables cr_TextCell4, cr_TextLayout14, cr_TextSound8, the function tables cr_WindowObjFunc /
                cr_TextCmdFunc): 0x8020A108-0x8020B0C0 and 0x8020DD48-0x8020EDB0, each object checked (size, words).
  script_tables lbl_8021B240 = char*[47] (file-name strings), lbl_8021B2FC = s32[47] (the BGM of each map id),
                lbl_8021B3B8 = char*[238] (file-id name strings, no reader: typed by its words), 0x8021B240-0x8021B770.
  trig_tables   the quarter sine f32[1025] (checked against sin) and the five u16 tables of fn_8001F060,
                0x801E6580-0x801E90B0.
  ui_tables     the tables of the code at 0x800F9-0x8010A, 0x80238160-0x80239AE4 and 0x80239B48-0x8023A004
                (cr_Record11C, float arrays by their strides, a text-record pool).
  camera_data   the camera code's .data 0x801F8894-0x801F8E74 (f32*[4], cr_g_CamModeNames, the f32 arrays of the table
                lbl_801F893C, the split-screen layouts cr_CamRect[n] and their tables), appended to the key-path unit.
  robo_tables   cr_g_BattleRoboActionFuncs (cr_RoboActionFunc[23]) and the five tables after it, typed from their
                readers (s32[4], u32[42][2], cr_RoboAction14[67], s32[53][2], u32[69][2]), 0x801F8EB8-0x801F9980.
Usage: python -I tools/scan_lbl_types.py        (writes symbols/lbl_types.txt; then python -I tools/gen_data_c.py --all)
"""
import collections
import re
import sys
from pathlib import Path

G = Path(__file__).resolve().parent.parent
ASM = G / "decomp/build/GXCE01/asm"
OUT = G / "symbols/lbl_types.txt"
OBJ = re.compile(r"^# \.(\w+):0x\w+ \| 0x([0-9A-F]+) \| size: 0x([0-9A-F]+)\n\.obj (\S+), \w+\n(.*?)^\.endobj", re.M | re.S)
PTR = re.compile(r"[A-Za-z_@]")   # a relocated word: the target's name (@address inside an object)


def read_objects():
    """every data object of the config symbols.txt with main.elf's words: a relocated word is the target's name (the
    symbol at that address), any other word its hex value; None for an object that is not whole words (or .bss).
    From main.elf, not dtk's asm, so an object already linked from C is still seen (batch 9 must have run)."""
    sys.path.insert(0, str(G / "tools"))
    import struct
    import gen_data_c as gdc
    g = gdc.Gen()
    objs = {}
    for n, sec, a, size, isf in g.ours:
        if isf or sec not in (".data", ".rodata", ".sdata", ".sdata2"):
            continue
        words = None
        if a % 4 == 0 and size % 4 == 0 and size:
            words = []
            for x in range(a, a + size, 4):
                r = g.elf.relocs.get(x)
                if r is not None:
                    t = g.at.get(r[1])
                    words.append(t[0] if t else f"@{r[1]:08X}")
                else:
                    words.append(f"0x{struct.unpack('>I', g.bytes_at(x, 4))[0]:08X}")
        objs[n] = (a, size, sec, words)
    return objs


def motion_info(objs):
    root = "cr_g_MotionInfo"
    if root not in objs or objs[root][3] is None:
        sys.exit(f"{root}: not found as a word object in the batch 9 asm")
    types = {}

    def want(n, t):
        if n not in objs:
            return
        if types.get(n, t) != t:
            sys.exit(f"{n}: {types[n]} and {t}")
        types[n] = t

    for n in objs[root][3]:
        if not PTR.match(n):
            continue
        a, size, sec, ws = objs[n]
        if ws is None or len(ws) % 2 or ws[-2:] != ["0x00000000", "0x00000000"]:
            sys.exit(f"{n}: not a tag list ended by (0, NULL)")
        want(n, f"cr_MotionTag[{len(ws) // 2}]")
        for i in range(0, len(ws) - 2, 2):
            tag, d = int(ws[i], 16), ws[i + 1]
            if not PTR.match(d):
                if d != "0x00000000":
                    sys.exit(f"{n}: tag {tag} holds the number {d}")
                continue
            if d not in objs:
                sys.exit(f"{n}: tag {tag} points at {d}, no object")
            if objs[d][2] != ".data":
                continue          # tags 1 / 2 / 3 / 9: .sdata objects of other units
            want(d, {0: "cr_MotionHeader", 6: "cr_MotionEvent", 7: "cr_MotionEvent", 10: "cr_MotionEvent",
                     8: "cr_MotionEventList"}.get(tag) or sys.exit(f"{n}: tag {tag} -> {d} in .data: role not known"))
    for n, t in list(types.items()):
        if t == "cr_MotionEventList":
            v = objs[n][3][1]
            if PTR.match(v):
                want(v, f"u32[{objs[v][1] // 4}]")
    size = {"cr_MotionHeader": 0x10, "cr_MotionEvent": 0xC, "cr_MotionEventList": 0xC}
    for n, t in types.items():
        m = re.match(r"(\w+)\[(\d+)\]", t)
        want_size = int(m.group(2)) * (8 if m.group(1) == "cr_MotionTag" else 4) if m else size[t]
        if objs[n][1] != want_size:
            sys.exit(f"{n}: dtk size 0x{objs[n][1]:X}, type {t} 0x{want_size:X}")
    order = sorted(types, key=lambda n: objs[n][0])
    lo, hi = objs[order[0]][0], objs[root][0]
    for x, y in zip(order, order[1:] + [root]):
        if objs[x][0] + objs[x][1] != objs[y][0]:
            sys.exit(f"{x} ends at 0x{objs[x][0] + objs[x][1]:08X}, {y} starts at 0x{objs[y][0]:08X}: not contiguous")
    inside = [n for n, (a, s, sec, w) in objs.items() if lo <= a < hi and sec == ".data"]
    if sorted(inside) != sorted(order):
        sys.exit(f"objects in 0x{lo:08X}-0x{hi:08X} that cr_g_MotionInfo does not reach: {sorted(set(inside) - set(order))[:5]}")
    count = collections.Counter(re.sub(r"\[\d+\]", "[]", t) for t in types.values())
    return (f"motion_info .data 0x{lo:08X}-0x{hi:08X} ({len(order)} objects: "
            + ", ".join(f"{c} {t}" for t, c in sorted(count.items())) + "; then cr_g_MotionInfo)",
            [(n, objs[n][2], objs[n][0], types[n]) for n in order] + [(root, None, None, None)])


def sqrt_table(objs):
    import math
    import struct
    n = "lbl_801E90B0"
    a, size, sec, ws = objs[n]
    # dtk's object is 0x8010: the 8192 values, then 16 zero bytes (the C's object ends at 0x8000, zeros follow); 0x8000 once linked
    if ws is None or size not in (0x8000, 0x8010) or any(PTR.match(w) for w in ws) or any(int(w, 16) for w in ws[8192:]):
        sys.exit(f"{n}: not 8192 plain words + 16 zero bytes")
    for i, w in enumerate(ws[:8192]):
        x = struct.unpack(">f", struct.pack(">I", int(w, 16)))[0]
        if abs(x - math.sqrt(i)) > math.sqrt(i) * 1e-6:
            sys.exit(f"{n}[{i}] = {x}, not sqrt({i})")
    return f"sqrt_table .data 0x{a:08X}-0x{a + 0x8000:08X} (f32[8192], sqrt(i) within 3 ulp)", [(n, sec, a, "f32[8192]")]


def ax_data(objs):
    out = []
    end = None
    for n in ("__AXCompressorTable", "axDspSlave"):
        a, size, sec, ws = objs[n]
        if ws is None or any(PTR.match(w) for w in ws) or (end is not None and a != end):
            sys.exit(f"{n}: not plain words, or not contiguous")
        out.append((n, sec, a, f"u16[{size // 2}]"))
        end = a + size
    if out[0][2] % 8:
        sys.exit("__AXCompressorTable: not 8-aligned")
    return f"ax_data .data 0x{out[0][2]:08X}-0x{end:08X} (the AX library's tables, SDK names)", out


def contiguous_runs(objs, types, label):
    """split a family's typed objects into units: contiguous (each starts where the previous ends), one section, the
    first 8-aligned (leading objects that are not are left out) -> [(head, items)]"""
    order = sorted(types, key=lambda n: objs[n][0])
    runs, cur = [], []
    for n in order:
        if cur and (objs[cur[-1]][0] + objs[cur[-1]][1] != objs[n][0] or objs[cur[-1]][2] != objs[n][2]):
            runs.append(cur)
            cur = []
        if cur or objs[n][0] % 8 == 0:
            cur.append(n)
    if cur:
        runs.append(cur)
    out = []
    for r in runs:
        a, e = objs[r[0]][0], objs[r[-1]][0] + objs[r[-1]][1]
        count = collections.Counter(re.sub(r"\[\d+\]", "[]", types[n]) for n in r)
        out.append((f"{label} {objs[r[0]][2]} 0x{a:08X}-0x{e:08X} ({len(r)} objects: "
                    + ", ".join(f"{c} {t}" for t, c in sorted(count.items())) + ")",
                    [(n, objs[n][2], objs[n][0], types[n]) for n in r]))
    return out


def file_descs(objs):
    """the file system's descriptor arrays: each of cr_g_FileDescs' 43 pointers -> cr_FileDesc[n] (0x10 each: path,
    dvd_entry, size, packed); a relocated `packed` -> cr_PackedEntry[n] (0x8 each: offset, size)"""
    root = "cr_g_FileDescs"
    types = {}
    for d in objs[root][3]:
        if not PTR.match(d) or d not in objs:
            continue
        a, size, sec, ws = objs[d]
        if ws is None or size % 0x10 or sec != ".data":
            sys.exit(f"{d}: not a word object of 0x10-byte descriptors")
        types[d] = f"cr_FileDesc[{size // 0x10}]"
        for i in range(0, len(ws), 4):
            if PTR.match(ws[i + 1]) or PTR.match(ws[i + 2]):
                sys.exit(f"{d}[{i // 4}]: dvd_entry / size relocated")
            p = ws[i + 3]
            if PTR.match(p):
                pa, psize, psec, pws = objs[p]
                if pws is None or psize % 8 or any(PTR.match(w) for w in pws) or psec != ".data":
                    sys.exit(f"{p}: not a plain table of 8-byte packed entries")
                types[p] = f"cr_PackedEntry[{psize // 8}]"
    return contiguous_runs(objs, types, "file_descs")


def stage_data(objs):
    """everything cr_g_StageData reaches: its 40 path pointers -> cr_KeyPath (6 x (keys, f32 count)), each track's
    keys -> cr_Key7[count] (the float count equals the array's length); exactly contiguous up to the table itself"""
    import struct
    root = "cr_g_StageData"
    types = {}
    for p in objs[root][3]:
        if not PTR.match(p):
            continue
        a, size, sec, ws = objs[p]
        if size != 0x30 or ws is None:
            sys.exit(f"{p}: not a 0x30 key path")
        types[p] = "cr_KeyPath"
        for i in range(0, 12, 2):
            k, c = ws[i], ws[i + 1]
            n = struct.unpack(">f", struct.pack(">I", int(c, 16)))[0]
            if not PTR.match(k) or PTR.match(c) or objs[k][1] != round(n) * 0x1C or n != round(n):
                sys.exit(f"{p} track {i // 2}: keys {k} (0x{objs[k][1]:X} bytes) and count {n} do not agree")
            if any(PTR.match(w) for w in objs[k][3]):
                sys.exit(f"{k}: keys hold a pointer")
            types[k] = f"cr_Key7[{round(n)}]"
    order = sorted(types, key=lambda n: objs[n][0])
    for x, y in zip(order, order[1:] + [root]):
        if objs[x][0] + objs[x][1] != objs[y][0]:
            sys.exit(f"{x} ends at 0x{objs[x][0] + objs[x][1]:08X}, {y} starts at 0x{objs[y][0]:08X}: not contiguous")
    if objs[order[0]][0] % 8:
        sys.exit(f"{order[0]}: not 8-aligned")
    lo, hi = objs[order[0]][0], objs[root][0]
    count = collections.Counter(re.sub(r"\[\d+\]", "[]", t) for t in types.values())
    return (f"stage_data .data 0x{lo:08X}-0x{hi:08X} ({len(order)} objects: "
            + ", ".join(f"{c} {t}" for t, c in sorted(count.items())) + "; then cr_g_StageData)",
            [(n, objs[n][2], objs[n][0], types[n]) for n in order] + [(root, None, None, None)])


def key_paths(objs, taken):
    """the key paths anywhere in .data (not yet typed): an object of (keys, f32 count) pairs where every count is a
    whole number equal to its keys object's length in 0x1C-byte cr_Key7 (no pointer inside) -> cr_KeyTrack[n]; its
    lists -> cr_Key7[k]; an object holding only pointers to such paths -> cr_KeyTrack*[n]. Self-checking: the float
    count must equal each list's length. Written as contiguous, 8-aligned runs."""
    import struct
    types = {}
    for n, (a, size, sec, ws) in objs.items():
        if sec != ".data" or ws is None or n in taken or len(ws) < 2 or len(ws) % 2:
            continue
        ok = True
        for i in range(0, len(ws), 2):
            k, c = ws[i], ws[i + 1]
            if not PTR.match(k) or PTR.match(c) or k not in objs or objs[k][3] is None:
                ok = False
                break
            x = struct.unpack(">f", struct.pack(">I", int(c, 16)))[0]
            if x != round(x) or x < 1 or objs[k][1] != round(x) * 0x1C or any(PTR.match(w) for w in objs[k][3]):
                ok = False
                break
        if ok:
            types[n] = f"cr_KeyTrack[{len(ws) // 2}]"
            for i in range(0, len(ws), 2):
                types[ws[i]] = f"cr_Key7[{objs[ws[i]][1] // 0x1C}]"
    for n, (a, size, sec, ws) in objs.items():
        if sec == ".data" and ws and n not in taken and n not in types and all(w in types and types[w].startswith("cr_KeyTrack") for w in ws):
            types[n] = f"cr_KeyTrack*[{len(ws)}]"
    if any(n in taken for n in types):
        sys.exit(f"key_paths: already typed: {[n for n in types if n in taken][:3]}")
    return contiguous_runs(objs, types, "key_paths")


def records(objs):
    """single tables typed from their readers (each with its size check): lbl_8022B3B8 = cr_Record8C[304]
    (stride 0x8C, symbols/cr_types.h)"""
    out = []
    for n, t, esz, cnt in (("lbl_8022B3B8", "cr_Record8C", 0x8C, 304), ("lbl_802359F8", "cr_Record10", 0x10, 64),
                           ("lbl_802362A0", "cr_CardImage", 0x1C40, 1)):
        a, size, sec, ws = objs[n]
        if size != esz * cnt or ws is None or any(PTR.match(w) for w in ws) or a % 8:
            sys.exit(f"{n}: not {cnt} x 0x{esz:X} plain words at an 8-aligned start")
        out.append((f"records {sec} 0x{a:08X}-0x{a + size:08X} ({t}[{cnt}])", [(n, sec, a, f"{t}[{cnt}]")]))
    return out


def window_data(objs):
    """the text window code's .data (fn_80064DE4..fn_8006B848), typed from its readers (symbols/cr_types.h: cr_TextRecord,
    cr_TextRecordList, cr_TextBlock, cr_TextCell4, cr_TextLayout14, cr_TextSound8, cr_TextPos0C, cr_Area48, the function
    tables). Two runs: 0x8020A108-0x8020B0C0 and 0x8020DD48-0x8020EDB0 (up to stage_data). Not in them: lbl_8020B0C0
    (0xB3C: its f32[5] head is read by fn_800668DC, nothing reads the rest) and the 4-aligned objects up to 0x8020DD48.
    Each object checks itself: its size, plain words or exactly the pointers its type has, list ends, function tables."""
    import math
    import struct

    def u16_at(ws, off):
        return int(ws[off // 4], 16) >> (16 if off % 4 == 0 else 0) & 0xFFFF

    def plain(n):
        return not any(PTR.match(w) for w in objs[n][3])

    def records(n, cnt):           # cr_TextRecord[cnt]: plain words, at least one record ends a list (+0x10 == 0xFFFF)
        ws = objs[n][3]
        return objs[n][1] == cnt * 0x20 and plain(n) and any(u16_at(ws, i * 0x20 + 0x10) == 0xFFFF for i in range(cnt))

    def rec_list(n, target):       # cr_TextRecordList[2]: { 0, target }, { 0xFFFE, NULL }
        return objs[n][1] == 0x10 and objs[n][3] == ["0x00000000", target, "0xFFFE0000", "0x00000000"]

    def block(n, lists):           # cr_TextBlock: plain words, +0x1C -> its lists
        ws = objs[n][3]
        return objs[n][1] == 0x20 and ws[7] == lists and not any(PTR.match(w) for w in ws[:7])

    def funcs(n, cnt, null_first):
        ws = objs[n][3]
        fs = [w for w in ws if w != "0x00000000"]
        return (objs[n][1] == cnt * 4 and len(fs) == cnt - (1 if null_first else 0)
                and all(w.startswith("fn_") for w in fs) and (ws[0] == "0x00000000") == null_first)

    def layout(n):                 # cr_TextLayout14[5]: plain, +0x10 == 0xFFFF in the last record only
        ws = objs[n][3]
        return (objs[n][1] == 5 * 0x14 and plain(n)
                and [int(ws[i * 5 + 4], 16) == 0xFFFF for i in range(5)] == [False] * 4 + [True])

    def floats(n):
        return plain(n) and all(not math.isnan(struct.unpack(">f", struct.pack(">I", int(w, 16)))[0]) for w in objs[n][3])

    runs = [
        [("lbl_8020A108", "cr_Area48", lambda n: objs[n][1] == 0x48 and floats(n)),
         ("lbl_8020A150", "cr_TextCell4[17][12]", lambda n: objs[n][1] == 17 * 12 * 4 and plain(n)),
         ("lbl_8020A480", "cr_TextCell4[96][5]", lambda n: objs[n][1] == 96 * 5 * 4 and plain(n)),
         ("lbl_8020AC00", "cr_TextRecord[1]", lambda n: records(n, 1)),
         ("lbl_8020AC20", "cr_TextRecordList[2]", lambda n: rec_list(n, "lbl_8020AC00")),
         ("lbl_8020AC30", "cr_TextBlock[1]", lambda n: block(n, "lbl_8020AC20")),
         ("lbl_8020AC50", "cr_WindowObjFunc[7]", lambda n: funcs(n, 7, True)),
         ("lbl_8020AC6C", "s32[1][16]", lambda n: objs[n][1] == 0x40 and plain(n) and "0xFFFFFFFF" in objs[n][3]),
         ("lbl_8020ACAC", "cr_TextPos0C", lambda n: objs[n][1] == 0xC and floats(n)),
         ("lbl_8020ACB8", "char[20]", lambda n: objs[n][1] == 20 and plain(n) and objs[n][3][-1] == "0x00000000"),
         ("lbl_8020ACCC", "cr_TextLayout14[5]", layout),
         ("lbl_8020AD30", "cr_TextSound8[114]", lambda n: objs[n][1] == 114 * 8 and plain(n))],
        [("lbl_8020DD48", "cr_TextCmdFunc[10]", lambda n: funcs(n, 10, False)),
         ("lbl_8020DD70", "f32[4]", lambda n: objs[n][1] == 0x10 and floats(n)),
         ("lbl_8020DD80", "cr_TextRecord[128]", lambda n: records(n, 128)),
         ("lbl_8020ED80", "cr_TextRecordList[2]", lambda n: rec_list(n, "lbl_8020DD80")),
         ("lbl_8020ED90", "cr_TextBlock", lambda n: block(n, "lbl_8020ED80"))],
    ]
    out = []
    for run in runs:
        for (n, t, ok), nxt in zip(run, run[1:] + [None]):
            if n not in objs or objs[n][3] is None:
                sys.exit(f"window_data: {n} not a word object (the cut 0x8020ED90 needs a batch 9 first)")
            if not ok(n):
                sys.exit(f"window_data: {n} does not check as {t}")
            if nxt and objs[n][0] + objs[n][1] != objs[nxt[0]][0]:
                sys.exit(f"window_data: {n} does not end where {nxt[0]} starts")
        a, e = objs[run[0][0]][0], objs[run[-1][0]][0] + objs[run[-1][0]][1]
        if a % 8:
            sys.exit(f"window_data: run 0x{a:08X}-0x{e:08X} does not start 8-aligned")
        out.append((f"window_data .data 0x{a:08X}-0x{e:08X} ({len(run)} objects)",
                    [(n, objs[n][2], objs[n][0], t) for n, t, ok in run]))
    return out


def robo_tables(objs):
    """the battle robo code's tables after cr_g_BattleRoboActionFuncs (0x801F8EB8-0x801F9980, 8-aligned at both ends):
    cr_g_BattleRoboActionFuncs (cr_globals.txt, cr_RoboActionFunc[23]: 23 function pointers), lbl_801F8F14 = s32[4]
    (fn_8004EDE4 stores 4 robo indices, cmpwi loop, lwz), lbl_801F8F24 = u32[42][2] (by the robo's u16 +0x1CC, slwi 3:
    both words to fn_80097CA8, fn_800450EC / fn_80049924), lbl_801F9074 = cr_RoboAction14[67], lbl_801F95B0 =
    s32[53][2] (fn_8003F248: +0 compared with -1 by cmpwi, then to fn_80097CA8; fn_8003FE98 +4), lbl_801F9758 =
    u32[69][2] (fn_8003D7E0 +0, fn_8003DD38 +4, to fn_80097CA8). A second run, 0x801F9DC8-0x801FA188 (up to motion_info):
    lbl_801F9DC8 = cr_MotionTagFunc[5] (fn_8005CE74), lbl_801F9DDC = cr_MotionTagFunc[11] (fn_8005C384 ... call
    table[tag]), lbl_801F9E08 = u32[56][4] (fn_8005C4D0: row lbl_8044F538 << 4, column the u16 +2 << 2, lwz to the sound
    call fn_80098104). Between them: lbl_801F9980 = u16[11] (fn_8003F048: lhz, slwi 1, until 0xFFFF) + lbl_801F9996 =
    u8[4][4] (fn_80019E84 / fn_800392B0: [i << 2] +0..+3, four lbz; 2 zero bytes pad to 0x801F99A8), and the default
    skeleton lbl_801F9B00 = cr_SkelSlot14[26] (the cut 0x801F9D08; symbols/cr_types.h has the readers). Checked: sizes,
    plain words (the skeleton: a pointer only at +0x10), list ends, padding, the functions, contiguity."""
    def plain(n):
        return not any(PTR.match(w) for w in objs[n][3])

    def funcs(n, cnt):
        return objs[n][1] == cnt * 4 and all(w.startswith("fn_") for w in objs[n][3])

    gen = []

    def gb(n):                     # an object's bytes (for the objects that are not whole words)
        if not gen:
            sys.path.insert(0, str(G / "tools"))
            import gen_data_c as gdc
            gen.append(gdc.Gen())
        return gen[0].bytes_at(objs[n][0], objs[n][1])

    runs = [[("cr_g_BattleRoboActionFuncs", None, lambda n: funcs(n, 23)),
             ("lbl_801F8F14", "s32[4]", lambda n: objs[n][1] == 0x10 and plain(n)),
             ("lbl_801F8F24", "u32[42][2]", lambda n: objs[n][1] == 42 * 8 and plain(n)),
             ("lbl_801F9074", "cr_RoboAction14[67]", lambda n: objs[n][1] == 67 * 0x14 and plain(n)),
             ("lbl_801F95B0", "s32[53][2]", lambda n: objs[n][1] == 53 * 8 and plain(n)),
             ("lbl_801F9758", "u32[69][2]", lambda n: objs[n][1] == 69 * 8 and plain(n))],
            [("lbl_801F9980", "u16[11]", lambda n: objs[n][1] == 0x16 and [
                gb(n)[i:i + 2] == b"\xff\xff" for i in range(0, 0x16, 2)] == [False] * 10 + [True]),
             # dtk's object is 0x12 (the 2 padding bytes); 0x10 once the unit is linked (its own symbol)
             ("lbl_801F9996", "u8[4][4]", lambda n: objs[n][1] == 0x10 or objs[n][1] == 0x12 and gb(n)[0x10:] == b"\0\0")],
            [("lbl_801F9B00", "cr_SkelSlot14[26]", lambda n: objs[n][1] == 26 * 0x14 and all(
                not PTR.match(w) for i, w in enumerate(objs[n][3]) if i % 5 != 4))],
            [("lbl_801F9DC8", "cr_MotionTagFunc[5]", lambda n: funcs(n, 5)),
             ("lbl_801F9DDC", "cr_MotionTagFunc[11]", lambda n: funcs(n, 11)),
             ("lbl_801F9E08", "u32[56][4]", lambda n: objs[n][1] == 56 * 16 and plain(n))]]
    out = []
    for run in runs:
        for (n, t, ok), nxt in zip(run, run[1:] + [None]):
            if n not in objs or (objs[n][3] is None and objs[n][0] % 4 == 0 and objs[n][1] % 4 == 0) or not ok(n):
                sys.exit(f"robo_tables: {n} does not check as {t or 'its cr_globals.txt type'}")
            if nxt and objs[n][0] + objs[n][1] != objs[nxt[0]][0]:
                sys.exit(f"robo_tables: {n} does not end where {nxt[0]} starts")
        lo, hi = objs[run[0][0]][0], objs[run[-1][0]][0] + objs[run[-1][0]][1]
        if lo % 8:   # a .data unit must start 8-aligned; its end need not be (the key-path unit ends at 0x801F8894)
            sys.exit(f"robo_tables: 0x{lo:08X}-0x{hi:08X} does not start 8-aligned")
        out.append((f"robo_tables .data 0x{lo:08X}-0x{hi:08X} ({len(run)} objects)",
                    [(n, None, None, None) if t is None else (n, objs[n][2], objs[n][0], t) for n, t, ok in run]))
    return out


def camera_data(objs):
    """the camera code's .data after the key paths, 0x801F8894-0x801F8E74 (all 4-aligned: it is appended to the key-path
    unit that ends at 0x801F8894; a .data unit's end need not be 8-aligned, only its start): lbl_801F8894 = f32*[4]
    (fn_80027170: lwz [i << 2], stfsx through it), cr_g_CamModeNames (cr_globals.txt), the f32 arrays the pointer table
    lbl_801F893C (f32*[8]) points at (cr_CAMMODE_DICE_WAIT_Frame / BTL_WAIT_Frame: lfsx [camera << 2]), and the
    split-screen layouts: cr_CamRect arrays (one per camera) with the two pointer tables lbl_801F8BDC (cr_CamRect*[10])
    and lbl_801F8E54 (cr_CamRect*[8]) (symbols/cr_types.h cr_CamRect has the readers). Cuts 0x801F88A4, 0x801F8BDC,
    0x801F8E74. Each object checks itself: sizes, plain words, the tables' targets are the family's own arrays."""
    start, end = 0x801F8894, 0x801F8E74
    inside = sorted((n for n, (a, s, sec, w) in objs.items() if sec == ".data" and start <= a < end), key=lambda n: objs[n][0])
    tables = {"lbl_801F893C": "f32", "lbl_801F8BDC": "cr_CamRect", "lbl_801F8E54": "cr_CamRect"}
    target = {}
    for tb, el in tables.items():
        for w in objs[tb][3]:
            if not PTR.match(w):
                sys.exit(f"camera_data: {tb} holds a number")
            target.setdefault(w, el)
            if target[w] != el:
                sys.exit(f"camera_data: {w} is reached as {target[w]} and {el}")
    items = []
    for n in inside:
        a, size, sec, ws = objs[n]
        if ws is None:
            sys.exit(f"camera_data: {n} is not whole words")
        plain = not any(PTR.match(w) for w in ws)
        if n == "lbl_801F8894":
            if size != 0x10 or not all(PTR.match(w) and w not in objs for w in ws):   # .bss targets: not in the word objects
                sys.exit("camera_data: lbl_801F8894 is not 4 pointers into .bss (the cut 0x801F88A4 needs a batch 9 first)")
            items.append((n, sec, a, "f32*[4]"))
        elif n == "cr_g_CamModeNames":
            items.append((n, None, None, None))
        elif n in tables:
            items.append((n, sec, a, f"{tables[n]}*[{size // 4}]"))
        elif target.get(n) == "f32" and plain:
            items.append((n, sec, a, f"f32[{size // 4}]"))
        elif target.get(n) == "cr_CamRect" and plain and size % 0x10 == 0 or n in ("lbl_801F895C", "lbl_801F8C04") and size == 0x20 and plain:
            items.append((n, sec, a, f"cr_CamRect[{size // 0x10}]"))
        else:
            sys.exit(f"camera_data: {n} (0x{a:08X}, 0x{size:X}) has no type in the family")
    for x, y in zip(items, items[1:]):
        if objs[x[0]][0] + objs[x[0]][1] != objs[y[0]][0]:
            sys.exit(f"camera_data: {x[0]} does not end where {y[0]} starts")
    if objs[items[-1][0]][0] + objs[items[-1][0]][1] != end:
        sys.exit("camera_data: the run does not end at 0x801F8E74 (the cut 0x801F8E74 needs a batch 9 first)")
    for w in target:
        if w not in inside and objs[w][2] == ".data":
            sys.exit(f"camera_data: a table points at {w}, outside the run")
    count = collections.Counter(re.sub(r"\[\d+\]", "[]", t) for n, s, a, t in items if t)
    return (f"camera_data .data 0x{start:08X}-0x{end:08X} ({len(items)} objects: "
            + ", ".join(f"{c} {t}" for t, c in sorted(count.items())) + ")", items)


def trig_tables(objs):
    """the trig tables 0x801E6580-0x801E90B0 (up to the sqrt table): lbl_801E6580 = f32[1025], the quarter sine read by
    cr_Sin16 / cr_Cos16 / cr_SinCos16 as [((angle & 0x3FFF) >> 4) + 1] (entry 0 is 0; entries 1..1024 are
    sin(i * pi / 2 / 1023) within 2e-5, checked one by one), then the five u16 tables fn_8001F060 reads from the same base
    (+0x1004, +0x2006, +0x2408, +0x2A08, +0x2A96: every branch ends in slwi 1, lhzx): u16[2049] / [513] / [768] / [71] /
    [77]. Cuts 0x801E7584 / 0x801E8586 / 0x801E8988 / 0x801E8F88 / 0x801E9016."""
    import math
    import struct
    sys.path.insert(0, str(G / "tools"))
    import gen_data_c as gdc
    g = gdc.Gen()
    run = [("lbl_801E6580", "f32[1025]", 1025 * 4), ("lbl_801E7584", "u16[2049]", 2049 * 2),
           ("lbl_801E8586", "u16[513]", 513 * 2), ("lbl_801E8988", "u16[768]", 768 * 2),
           ("lbl_801E8F88", "u16[71]", 71 * 2), ("lbl_801E9016", "u16[77]", 77 * 2)]
    a = 0x801E6580
    for n, t, size in run:
        if n not in objs or objs[n][0] != a or objs[n][1] != size:
            sys.exit(f"trig_tables: {n} is not 0x{size:X} bytes at 0x{a:08X} (the cuts need a batch 9 first)")
        a += size
    if a != 0x801E90B0 or any(x in g.elf.relocs for x in range(0x801E6580, a, 2)):
        sys.exit("trig_tables: the run does not end at the sqrt table, or holds a relocation")
    v = struct.unpack(">1025f", g.bytes_at(0x801E6580, 1025 * 4))
    if v[0] != 0 or any(abs(v[i + 1] - math.sin(i * math.pi / 2 / 1023)) > 2e-5 for i in range(1024)):
        sys.exit("trig_tables: lbl_801E6580 is not the quarter sine table")
    return [("trig_tables .data 0x801E6580-0x801E90B0 (f32[1025] sine, 5 u16 tables)",
             [(n, ".data", objs[n][0], t) for n, t, size in run])]


def ui_tables(objs):
    """the tables of the code at 0x800F9-0x8010A (.data 0x80238160-0x8023A004), typed from their readers (probe of the
    loads: direct lfs, lfs through base + index x stride, lfsx): lbl_80238160 = cr_Record11C[23] (cut 0x80239AE4: the
    0x64 bytes after it are not read), then from 0x80239B48: f32[4][2] (stride 8 + the four floats fn_80100484 reads),
    f32[4], cr_F32x10Pad, f32[20][6] / f32[10][6] (stride 0x18, six lfs), f32[6][2], f32[5][2], the three arrays of
    0x80239ED0 (cuts 0x80239EF8 / 0x80239F20), f32[4], f32[8], f32[6], a text-record pool (cr_TextRecord[2] ended by
    0xFFFF, cr_TextRecordList[2], cr_TextBlock pointing back; cuts 0x80239FCC / 0x80239FEC) and f32[3][2]."""
    def plain(n):
        return not any(PTR.match(w) for w in objs[n][3])

    def u16_at(ws, off):
        return int(ws[off // 4], 16) >> (16 if off % 4 == 0 else 0) & 0xFFFF

    def rec11c(n):
        ws = objs[n][3]
        return objs[n][1] == 23 * 0x11C and all(
            bool(PTR.match(w)) == ((i * 4) % 0x11C in (0, 4)) for i, w in enumerate(ws))

    def f32s(cnt):
        return lambda n: objs[n][1] == cnt * 4 and plain(n)

    runs = [[("lbl_80238160", "cr_Record11C[23]", rec11c)],
            [("lbl_80239B48", "f32[4][2]", f32s(8)),
             ("lbl_80239B68", "f32[4]", f32s(4)),
             ("lbl_80239B78", "cr_F32x10Pad", lambda n: objs[n][1] == 0x30 and plain(n) and objs[n][3][10:] == ["0x00000000"] * 2),
             ("lbl_80239BA8", "f32[20][6]", f32s(120)),
             ("lbl_80239D88", "f32[10][6]", f32s(60)),
             ("lbl_80239E78", "f32[6][2]", f32s(12)),
             ("lbl_80239EA8", "f32[5][2]", f32s(10)),
             ("lbl_80239ED0", "f32[5][2]", f32s(10)),
             ("lbl_80239EF8", "f32[5][2]", f32s(10)),
             ("lbl_80239F20", "f32[5]", f32s(5)),
             ("lbl_80239F34", "f32[4]", f32s(4)),
             ("lbl_80239F44", "f32[8]", f32s(8)),
             ("lbl_80239F64", "f32[6]", f32s(6)),
             ("lbl_80239F7C", "cr_TextRecord[2]", lambda n: objs[n][1] == 0x40 and plain(n)
              and u16_at(objs[n][3], 0x10) != 0xFFFF and u16_at(objs[n][3], 0x30) == 0xFFFF),
             ("lbl_80239FBC", "cr_TextRecordList[2]", lambda n: objs[n][3] == ["0x00000000", "lbl_80239F7C", "0xFFFE0000", "0x00000000"]),
             ("lbl_80239FCC", "cr_TextBlock", lambda n: objs[n][1] == 0x20 and objs[n][3][7] == "lbl_80239FBC"
              and not any(PTR.match(w) for w in objs[n][3][:7])),
             ("lbl_80239FEC", "f32[3][2]", f32s(6))]]
    out = []
    for run in runs:
        for (n, t, ok), nxt in zip(run, run[1:] + [None]):
            if n not in objs or objs[n][3] is None or not ok(n):
                sys.exit(f"ui_tables: {n} does not check as {t} (the cuts need a batch 9 first)")
            if nxt and objs[n][0] + objs[n][1] != objs[nxt[0]][0]:
                sys.exit(f"ui_tables: {n} does not end where {nxt[0]} starts")
        lo, hi = objs[run[0][0]][0], objs[run[-1][0]][0] + objs[run[-1][0]][1]
        if lo % 8:
            sys.exit(f"ui_tables: 0x{lo:08X} does not start 8-aligned")
        out.append((f"ui_tables .data 0x{lo:08X}-0x{hi:08X} ({len(run)} objects)",
                    [(n, objs[n][2], objs[n][0], t) for n, t, ok in run]))
    return out


def script_tables(objs):
    """three tables before lbl_8021B770 in the script code's .data (0x8021B240-0x8021B770, 8-aligned at both ends):
    lbl_8021B240 = char*[47] (fn_800A0E64: lwzx from its .data base lbl_8021AF28 + 0x318 by a signed byte x 4, passed to
    sprintf; "RPG_WRD01_BIN" ...); lbl_8021B2FC = s32[47] (a word per map id, the u16 +0x7268, slwi 2: fn_8009DAD8 /
    cr_S_BGM_DEFAULT store it as the BGM at +0xBA, cr_AreaBoxes_Update / cr_MAP_NEXT_SET compare it with that BGM by
    cmpw, a signed compare; cr_SYS_RET reads [27]); lbl_8021B3B8 = char*[238] (the cut 0x8021B3B8: no reader found, the
    type from the words: every word a relocation to a NUL-terminated .rodata string, "OBJ_BATTLE_SET_BIN" ...).
    Checked: the strings, the plain words, the sizes, contiguity."""
    sys.path.insert(0, str(G / "tools"))
    import gen_data_c as gdc
    g = gdc.Gen()
    run = [("lbl_8021B240", "char*", 47), ("lbl_8021B2FC", "s32", 47), ("lbl_8021B3B8", "char*", 238)]
    for (n, t, cnt), nxt in zip(run, run[1:] + [None]):
        if n not in objs or objs[n][3] is None or objs[n][1] != cnt * 4:
            sys.exit(f"script_tables: {n} is not {cnt} words (the cut 0x8021B3B8 needs a batch 9 first)")
        a0, size0, sec0, ws = objs[n]
        if nxt and a0 + size0 != objs[nxt[0]][0]:
            sys.exit(f"script_tables: {n} does not end where {nxt[0]} starts")
        for w in ws:
            if t == "s32":
                if PTR.match(w):
                    sys.exit(f"script_tables: {n} holds a pointer")
                continue
            a, size, sec, _ = objs.get(w, (None, 0, None, None))
            b = g.bytes_at(a, size) if a else b""
            if sec != ".rodata" or not b.rstrip(b"\0") or b"\0" in b.rstrip(b"\0") or not b.endswith(b"\0"):
                sys.exit(f"script_tables: {n} entry {w} is not a .rodata string")
    lo, hi = objs[run[0][0]][0], objs[run[-1][0]][0] + objs[run[-1][0]][1]
    if lo % 8:
        sys.exit(f"script_tables: 0x{lo:08X}-0x{hi:08X} does not start 8-aligned")
    return [(f"script_tables .data 0x{lo:08X}-0x{hi:08X} (char*[47], s32[47], char*[238])",
             [(n, ".data", objs[n][0], f"{t}[{cnt}]") for n, t, cnt in run])]


def main():
    objs = read_objects()
    out = ["# Types of data objects that keep dtk's lbl_ names (no evidence names them; their types are proven by the code and",
           "# the bytes). Written by `python -I tools/scan_lbl_types.py` from the batch 9 asm; read by tools/gen_data_c.py",
           "# next to cr_globals.txt. Each `# unit` block is one contiguous run, one unit of `gen_data_c.py --all`; a line",
           "# without a section (`= cr_g_X`) puts a global of cr_globals.txt into the unit at its place."]
    blocks = []
    for fam in (motion_info, sqrt_table, ax_data, file_descs, stage_data, records, window_data, script_tables, robo_tables, trig_tables, ui_tables):
        r = fam(objs)
        blocks += r if isinstance(r, list) else [r]
    taken = {n for h, items in blocks for n, sec, a, t in items}
    blocks += key_paths(objs, taken)
    # the camera layouts start 4-aligned right where a key-path run ends: they join that unit
    cam_head, cam_items = camera_data(objs)
    for i, (head, items) in enumerate(blocks):
        n = items[-1][0]
        if items[-1][1] and objs[n][0] + objs[n][1] == objs[cam_items[0][0]][0]:
            blocks[i] = (head + "; then " + cam_head, items + cam_items)
            break
    else:
        sys.exit(f"camera_data: no unit ends at 0x{objs[cam_items[0][0]][0]:08X}")
    for head, items in blocks:
        out.append(f"# unit {head}")
        for n, sec, a, t in items:
            out.append(f"{n} = {sec}:0x{a:08X}; // type:object datatype:{t}" if sec else f"= {n}")
        print(head)
    OUT.write_text("\n".join(out) + "\n", encoding="utf-8", newline="\r\n")
    print(f"{OUT.relative_to(G)} written")


if __name__ == "__main__":
    main()
