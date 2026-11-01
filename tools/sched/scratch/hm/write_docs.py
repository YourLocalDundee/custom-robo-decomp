"""step 83 docs: plan.md (step, resume bullet, super-goal items), docs/drafts.md (file objects), docs/decomp.md.
usage: write_docs.py <after.json>  (keys: objects, units, units_total, code_pct, data_pct, game_objs, game_bytes,
game_pct, promoted, fileobjs, kept, matches, compiled, bytes_eq, hist80_99, shift11)"""
import json, re, sys
G = r'D:\emulators\gameTesting-hsd2'
A = json.load(open(sys.argv[1], encoding='utf-8'))
rows = open(r'C:\Users\yourl\AppData\Local\Temp\claude\C--Windows-System32\eb280a94-14b4-4d21-99dd-7ba6a0ebb0c5\scratchpad\runs_before.md', encoding='utf-8').read().splitlines()


def trim(line, n=6):
    cells = line.split(' | ')
    if len(cells) < 7:
        return line
    bl = cells[6].rstrip(' |')
    parts = bl.split('), ')
    if len(parts) > n:
        bl = '), '.join(parts[:n]) + ') ... +%d more' % (len(parts) - n)
    cells[6] = bl + ' |'
    return ' | '.join(cells)


table = '\r\n'.join(trim(r) for r in rows)


def rw(path, fn):
    p = G + '\\' + path
    s = open(p, encoding='utf-8', newline='').read()
    assert '\r\n' in s
    t = fn(s)
    assert t != s, path
    open(p, 'w', encoding='utf-8', newline='').write(t)


# ---- docs/drafts.md -------------------------------------------------------------------------------------------------
drafts_add = ('\r\n'
'### Step 83: the run table, string literals, the pool order (Oct 31, 2026, branch fileobjs)\r\n'
'\r\n'
'The file runs after step 82 (promote_report.txt "file runs" with the drafts\' sizes and match percentages; the runs\r\n'
'ranked by how many functions still block them, a run of one function with a FILE OBJECT line is linked):\r\n'
'\r\n' + table + '\r\n'
'\r\n'
'**fn_8002444C** (the one-function run that matched but could not be placed: "two piece starts") is now the fourth file\r\n'
'object. Its .sdata2 pool (0x804504E8-0x80450518) is 48, 56, 96, 88, "\\x96\\xb3\\x82\\xb5", 192, "\\x97\\x4c\\x82\\xe8", 72,\r\n'
'"\\x81\\x96", 160: the float literals (88.0f once, merged by the compiler) with the call\'s Shift-JIS string literal\r\n'
'AFTER its two floats, every string packed (5 or 3 bytes, the next float 4-aligned). Two things reproduce it byte for\r\n'
'byte: (1) **`-str reuse,readonly`** for the game (configure.py; a literal of 8 bytes or less is then an .sdata2\r\n'
'object of its own, longer ones go to .rodata, where the game\'s 2,565 strings are; with the default `-str reuse` the\r\n'
'compiler puts them in .sdata / .data, which main.dol never has); the drafts write strings as externs, so no draft\'s\r\n'
'code changes; (2) **the callee\'s parameter order**: fn_8006B144 takes `(f32 x, f32 y, const char *text)` (prototype\r\n'
'in symbols/cr_types.h with this evidence; the registers f1, f2, r3 fit either order, the pool order only the one with\r\n'
'the string last), and the draft generator\'s `float_literals` variant now also writes the **.sdata2 string literals**\r\n'
'the draft takes the address of (`pool_strings`: dtk\'s `data:string` objects and the untyped .sdata2 objects whose\r\n'
'bytes are a string followed by zeros, every byte a hex escape; only at `(s8 *) lbl_X` / `(s32) lbl_X`), so the draft\r\n'
'owns its whole pool. The same hex-escape form in the promoted file compiles to the same bytes. The 5 other matched\r\n'
'drafts that read .sdata2 strings (cr_SEQ_TITLE_FrameC, fn_8006FBE0, fn_80075120, fn_80094394, fn_80095A30) own no\r\n'
'float, so their strings stay externs and they stay one-function objects (their strings are shared by 2-6 functions\r\n'
'each: the SEQ FrameC functions, fn_80070170..fn_800730F8, fn_80095D7C..fn_80096374: one file each by the pool rule).\r\n'
'\r\n'
'**fn_8006EEF8** (0xBCC, the only blocker of the smallest run) was hand-written against the asm (scratch harness:\r\n'
'compile with the game flags against the drafts\' ctx.h, compare words with tools/draft_batch.py\'s compare, dtk elf\r\n'
'disasm side by side): 32% -> **73%**, same length (755 words), not matched. The shape that gives the 73%: the vertex\r\n'
'writes are `static inline` helpers with f32 parameters (`grid_pos(x, y, z)`, `grid_color(r, g, b, a)` on the\r\n'
'write-gather pipe); at `-opt level=0` every inlined f32 argument that is not a plain variable or constant becomes a\r\n'
'local of its own, the locals are coloured by use count into f31 downwards (the first 14: f18-f31), the rest get a stack\r\n'
'slot each (0x8C, 0x88..0x08), which is the original\'s "12 three-float groups copied to the stack"; `Mtx m; Vec p, q;`\r\n'
'in that order; `u16 n = 42` with `for (i = -n / 2; i <= n / 2; i++)` and `(f32) n * 0.5f`. Left, in the order they\r\n'
'appear: (a) the original keeps **r30 unused** between i (r31) and alpha (r29): a tiny-function test shows the\r\n'
'mechanism - a local referenced only in code the compiler removes (`if (0) { ... }`, an `if (x == 0) {}` with an empty\r\n'
'body) keeps the register its use count earned, so 198 game functions with a register gap (none matched) have such a\r\n'
'variable; no spelling of it ranked between i and alpha here (a 16-statement dead block did not); (b) `clrlwi r29,r27,24`\r\n'
'on `alpha = up` after `up -= 0x1C` but a plain `mr` for `down`: not an `s32` / `u8` type difference, not `& 0xFF`, not a\r\n'
'`(u8)` cast (all compile to `mr`); (c) the scheduler\'s register numbering in the vertex blocks (value / address pairs\r\n'
'r5/r4 vs r0/r5 and the 0x4330 conversion temporaries), which moves with (a). Also tried without effect: the SDK\'s own\r\n'
'`GXWGFifo` absolute declaration against `*(volatile f32 *) 0xCC008000`, colour parameters as u8 / u32 / int / a macro,\r\n'
'one combined position+colour helper, declaration order (register assignment is by use count, not declaration order),\r\n'
'constants held in variables (propagated away). The notes and the best C are in the step 83 log; the draft stays m2c\'s.\r\n'
'\r\n'
'**Data objects from C** (plan.md step 83, not built): eight typed .bss globals have exactly dtk\'s sizes (compile-time\r\n'
'check against ctx.h): cr_SaveData 0x1138, cr_ScriptState 0x484, cr_ScenarioState 0x364, cr_UnlockBits 0xA8,\r\n'
'cr_GameMode 0x48, cr_SeqState 0x48, cr_StageSelectorState 0x354, cr_CustomizeState 0x1B54; cr_g_Script, cr_g_SaveData\r\n'
'and cr_g_Scenario are contiguous (0x803BE358-0x803BFC78, 8-aligned), and the compiler lays a file\'s globals out in\r\n'
'reverse declaration order (.bss align 8), so one data file declaring them in reverse order compiles to that piece.\r\n'
'What is missing is the unit kind: gen_dtk_splits.game_c_objects takes only files with function definitions (.text\r\n'
'ranges), game_file_data only the data of a file with code; a data-only game unit needs its .bss / .data range from the\r\n'
'compiled object\'s defined symbols at our addresses (the next dtk symbol must start exactly at its end, or the padding\r\n'
'rule of step 65 applies), the first build without the object (the batch 9 fixpoint), shift_test\'s insertion points,\r\n'
'and batch 9\'s 100% check (trivial for .bss; .data tables need every pointer relocated by name).\r\n'
)


def drafts(s):
    anchor = '\r\n## Using it\r\n'
    assert s.count(anchor) == 1
    return s.replace(anchor, drafts_add + anchor)


rw('docs\\drafts.md', drafts)

# ---- docs/decomp.md -------------------------------------------------------------------------------------------------
def decomp(s):
    old = ('step 82: 645 of 920 objects from C, 22.17% of main.dol\'s code: Dolphin SDK 98 of 98, MSL 49 of 49, Runtime 5 of 5,\r\n'
           'MetroTRK 29 of 29, HSD 22 of 30, game 442 (0xCB18 bytes, 4.23% of the game code)')
    assert s.count(old) == 1
    new = ('step 83: {units} of {units_total} objects from C, {code_pct}% of main.dol\'s code, {data_pct}% of its data: Dolphin SDK 98 of 98, MSL 49 of 49, Runtime 5 of 5,\r\n'
           'MetroTRK 29 of 29, HSD 22 of 30, game {game_objs} ({game_bytes} bytes, {game_pct}% of the game code; {fileobjs} file objects)').format(**A)
    s = s.replace(old, new)
    old2 = 'links a group it cannot prove, so such a run simply waits until all its functions match.\r\n'
    assert s.count(old2) == 1
    new2 = old2 + ('Status after step 83 (Oct 31, 2026): {fileobjs} file objects. The run table with every blocker is in\r\n'
                   '[drafts.md](drafts.md) ("Step 83"). Two more rules came from fn_8002444C\'s pool: the game is compiled with\r\n'
                   '`-str reuse,readonly` (a string literal of 8 bytes or less is an .sdata2 object of its own, laid out between the\r\n'
                   'float literals in argument order; longer ones are in .rodata), and a draft that owns its float literals owns the\r\n'
                   '.sdata2 strings it takes the address of too (tools/draft_batch.py `pool_strings`), so the whole pool is one piece.\r\n'
                   'The smallest open run (fn_8006EEF8..fn_8006FDD0) still waits on fn_8006EEF8 (hand-written to 73%: drafts.md).\r\n').format(**A)
    return s.replace(old2, new2)


rw('docs\\decomp.md', decomp)

# ---- plan.md --------------------------------------------------------------------------------------------------------
step83 = ('83. **Phase 4: game file objects and data from C - the run table, fn_8002444C\'s pool, fn_8006EEF8 at 73%** (Oct 31, 2026, branch fileobjs; docs/drafts.md "Step 83", docs/decomp.md "Game file objects"). '
          'Baseline: batch 9 and a full batch 12 in this worktree reproduce step 82 (480 at 100%, 1,706 compile, 520,140 bytes equal; 442 game objects, 645 of 920 units, 22.17% code, 5.90% data). '
          '**(1) The run table** (every file run of a matched data-owning draft, its span, functions at 100%, and each blocker with size and match %: docs/drafts.md): ranked by blockers, fn_8006EEF8..fn_8006FDD0 (7 functions, 1 blocker: fn_8006EEF8 0xBCC 32%), fn_8008A0A8..fn_8008BD50 (11, 5: fn_8008A484 56%, fn_8008A6B0 64%, fn_8008A8B0 57%, fn_8008AAE8 56%, fn_8008ACEC compile failed), fn_800057B8..fn_8000805C (16, 5: fn_800057B8 43%, fn_80005BC8 56%, cr_BattleRobo_InitFromLoadout 36%, fn_80007CE4 65%, fn_8000805C 67%), fn_8008057C..fn_800812A4 (11, 7, two of them at 98% / 92%), fn_80076B2C..fn_80079C70 (31, 12), the fn_8007xxxx camera / robo runs (12-13), fn_8010E47C..fn_80112A0C (27, 14), cr_Bgm_GetVolume..fn_80098484 (25, 17), then runs of 17-63 blockers and the 1,739-function closure. '
          '**(2) fn_8002444C is the fourth file object** (`game/fn_8002444C_fn_8002444C.c`, .sdata2 0x804504E8-0x80450518): its draft matched but its pool gave "two piece starts" because the three Shift-JIS string literals between its float literals were externs. Findings: the game is compiled with **`-str reuse,readonly`** (configure.py game flags, tools/patches regenerated): a literal of 8 bytes or less is an .sdata2 object of its own (157 in main.dol), longer ones are the 2,565 .rodata strings; with the default -str reuse the compiler puts them in .sdata; and **fn_8006B144 takes (f32 x, f32 y, const char *text)** (symbols/cr_types.h): the pool holds each call\'s literals in argument order with the string after its floats (48, 56, 96, 88, "...", 192, "...", 72, "...", 160; 88.0f once, merged), which the registers f1, f2, r3 cannot tell. tools/draft_batch.py: `pool_strings` (dtk\'s data:string objects of .sdata2 and the untyped ones whose bytes are a string followed by zeros, as hex escapes) and the `float_literals` variant writes them where the draft takes the address (`(s8 *) lbl_X`, `(s32) lbl_X`), so a draft that owns floats owns its whole pool; promote_drafts and game_file_data needed no change (the piece is one run, bytes equal, no outside user). '
          '**(3) fn_8006EEF8** (the smallest run\'s only blocker, 1.5 h): hand-written from the asm, 32% -> 73% (755 of 755 words, 557 equal): the vertex writes are inlined f32-parameter helpers whose arguments become locals at level 0 (f31 down to f18, then one stack slot each: the original\'s 12 copied groups), `Mtx m; Vec p, q;`, `u16 n = 42`. Not matched: the original keeps r30 unused (the mechanism found in a tiny test: a local referenced only in code the compiler removes keeps the register its use count earned - 198 game functions have such a gap, none matched - but no spelling placed it between i and alpha here), `clrlwi` on `alpha = up` (no type, mask or cast gives it), and the scheduler\'s register numbering that follows from the first. The draft stays m2c\'s; the shape is in docs/drafts.md. '
          '**(4) Data from C**: eight typed .bss globals have exactly dtk\'s sizes (cr_SaveData 0x1138, cr_ScriptState 0x484, cr_ScenarioState 0x364, cr_UnlockBits 0xA8, cr_GameMode 0x48, cr_SeqState 0x48, cr_StageSelectorState 0x354, cr_CustomizeState 0x1B54), cr_g_Script / cr_g_SaveData / cr_g_Scenario are one contiguous 8-aligned piece 0x803BE358-0x803BFC78, and a data file declaring them in reverse order compiles to it; no data object was added: gen_dtk_splits has no data-only game unit (game_c_objects wants function definitions, game_file_data a .text range), and adding one (ranges from the object\'s defined symbols, the padding rule, the fixpoint, shift_test, the batch 9 check) is the next step, left for its own branch. '
          '**After**: 100% MATCH {matches}, compile {compiled} of 1,751, bytes equal {bytes_eq}, 80-99% {hist80_99}; **{game_objs} game objects ({promoted} promoted, {fileobjs} file objects, {kept} kept), {units} of {units_total} units from C, {code_pct}% of main.dol\'s code, {data_pct}% of its data** (game {game_bytes} bytes, {game_pct}% of the game code). **Checks**: batch 9 main.dol retail SHA-1, every C object 100%, relocations OK, static shift PASS; batch 8 0 FAIL; batch 11 {shift11}; checksums written (the 3 decompiled\\ lines kept at the committed values: no Ghidra export in this worktree). '
          '**Left**: the runs of the table (the cheapest after fn_8006EEF8: fn_8008A0A8..fn_8008BD50 and fn_800057B8..fn_8000805C, 5 blockers each, fn_8008057C..fn_800812A4 with fn_80080A94 at 98%), the register-gap class (a dead-code variable per function), data-only game units.').format(**A)

resume = ('- **Step 83 (branch fileobjs): game file objects and data from C** (the run table in docs/drafts.md; `-str reuse,readonly` for the game and fn_8006B144(f32, f32, const char *) make fn_8002444C the fourth file object; draft_batch `pool_strings`; fn_8006EEF8 hand-written to 73%, the register-gap mechanism found; the eight typed .bss globals have dtk\'s sizes, a data-only unit kind is the next step): '
          '{matches} drafts at 100%, {game_objs} game objects ({fileobjs} file objects), {units} of {units_total} units from C, {code_pct}% of main.dol\'s code, {data_pct}% of its data. Left: fn_8006EEF8\'s r30 / clrlwi, the 5-blocker runs, data-only game units.\r\n').format(**A)


def plan(s):
    a1 = '## Resume here (updated Oct 31, 2026)\r\n\r\n'
    assert s.count(a1) == 1
    s = s.replace(a1, a1 + resume)
    # step 83 after step 82
    m = re.search(r'^82\. \*\*Phase 4: draft generator round 9\*\*.*?\r\n', s, re.M)
    assert m
    s = s[:m.end()] + step83 + '\r\n' + s[m.end():]
    # super-goal items (4) and (6)
    old4 = '(4) **game code 4.23% linked** (442 of ~1,750 functions, one object each but three file objects, steps 58-82;'
    assert s.count(old4) == 1
    s = s.replace(old4, '(4) **game code {game_pct}% linked** ({game_objs} of ~1,750 functions, one object each but {fileobjs} file objects, steps 58-83;'.format(**A))
    old4b = '(the smallest, fn_8006EEF8..fn_8006FDD0, waits on fn_8006EEF8 alone, 0xBCC bytes at 32%; the next on 5 functions each)'
    assert s.count(old4b) == 1
    s = s.replace(old4b, '(the run table with every blocker: docs/drafts.md "Step 83"; the smallest, fn_8006EEF8..fn_8006FDD0, waits on fn_8006EEF8 alone, 0xBCC bytes, hand-written to 73% in step 83; the next on 5 functions each)')
    old6 = '(6) data (0.38% from C) follows each matched object;'
    assert s.count(old6) == 1
    s = s.replace(old6, '(6) data ({data_pct}% from C after step 83; the typed .bss globals have dtk\'s sizes, a data-only game unit kind is the next step) follows each matched object;'.format(**A))
    return s


rw('plan.md', plan)
print('docs written')
