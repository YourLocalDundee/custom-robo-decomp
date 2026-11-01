"""step 85 docs, part 2: plan.md (What's done 84 + 85, Resume bullet, super-goal items 4 / 5) and docs/decomp.md (status
line, Game file objects). Run after apply_docs85.py; takes the same values (imports its computation)."""
import re, os, sys
sys.argv = [sys.argv[0], '--check']
S = os.path.dirname(os.path.abspath(__file__))
ns = {'__file__': os.path.join(S, 'apply_docs85.py')}
exec(open(os.path.join(S, 'apply_docs85.py'), encoding='utf-8').read().split("if '--check' in sys.argv:")[0], ns)
vals, rd, wr, G = ns['vals'], ns['rd'], ns['wr'], ns['G']
V = lambda k: str(vals[k])
CR = '\r\n'

# ---- plan.md ----
p = os.path.join(G, 'plan.md'); s = rd(p)
assert 'Step 85' not in s
step84 = ("84. **Phase 4: the script-command head read to the end, the pool-symbol fix** (Oct 31, 2026, branch handlers; docs/drafts.md "
          "\"Eleventh generator\"). Register allocation is done after scheduling (the original's register numbers carry no source order); "
          "the compiler's dependence model and hoisting order from 60 micro-programs; 720 statement-order / pointer shapes and 11 compiler "
          "versions scored on the pure handler cr_CM_DUMMY_008: nothing passes 18 of 38 words, the 85% ceiling of the ~220 handlers stands and "
          "`#pragma scheduling off` with the original's order cannot give the bytes either; `script_counter_ptr` declares the pointer after the "
          "PC local (about 120 handler drafts 44% -> 50%); a C file object's own `@NN` pool symbols are named lbl_<address> in the drafts "
          "(fn_8002444C compiles and is built from its draft again). After: 480 at 100%, 1,706 compile, 521,296 bytes equal, 443 game objects "
          "(4 file objects), 646 of 922 units from C, 22.18% of main.dol's code, 5.90% of its data.")
step85 = ("85. **Phase 4: file runs by hand - the register rules of level 0, two generator rules** (Oct 31, 2026, branch fileruns; docs/drafts.md "
          "\"Step 85\", docs/decomp.md \"Game file objects\"). Baseline: batch 9 and the step 84 counts (480 at 100%, 1,706 compile, 521,296 bytes "
          "equal, 443 game objects, 646 of 922 units, 22.18% code, 5.90% data). Goal: close the cheapest runs of the step 83 table by hand-matching "
          "their blockers; none closed. **(1) The register rules** (micro-programs on GC/2.6 with the game flags, correcting step 84 item 1): "
          "callee-saved registers go by USE COUNT from r31 down, declaration order only breaks ties; a local that is only written, or referenced "
          "only in code the compiler removes, is appended after the live locals, so it cannot make a gap; **a gap register between two live locals "
          "is a pointer local to a stack object, assigned (not initialised) and used through members only** (`Vec *pp; pp = &p; pp->x = ...`: it "
          "ranks by its use count and takes its register, then the peephole folds every access onto r1 and drops the addi; with an initialiser the "
          "addi stays, as a call argument the pointer stays); `clrlwi rD, rS, 24` on a copy is a u8 local widened into a u32 one (two u8 locals "
          "copy with `mr`); initialisers run in declaration order, an assignment where it stands; a pool constant the compiler owns is scheduled "
          "freely (its `lfs` moves above a store through a pointer), an extern waits. **(2) fn_8006EEF8** (the smallest run's only blocker): the "
          "frame is now exact (r31 i, r30 the gap pointer with 8 uses between alpha's 11 and i's 18, r29 u32 alpha, r28 u16 n assigned `n = 42` as a "
          "statement, r27 u8 up, r26 s32 down, the clrlwi, the three initial li in the original's order, 755 words), 548 of 755 words by position "
          "(step 83: 557 with the wrong frame; 673 with the register numbers masked). Left: the scheduler's priority in the vertex blocks after "
          "each GXBegin (the original hoists the next conversion's pool loads above the write-gather stores and issues each store's `lis` late; "
          "the pointer stores of rule (1) perturb whichever group carries them; every placement of the pointer, the store forms, volatile, the "
          "SDK's GXWGFifo, a far extern pipe symbol, colour parameter types, one combined helper and all orders of the float groups were tried: "
          "503-548). Not matched; the draft stays m2c's; the run stays open. **(3) fn_8008057C..fn_800812A4's near-misses, hand-matched and "
          "generalised** (tools/draft_batch.py): `fold_temp_compound` (default): m2c's temporary for a caller-saved register holding an address "
          "used as the base of the load and the store of one field in the next statement (`temp_r3_44 = p->a->b; FIELD(temp) = (f32) ((f64) "
          "FIELD(temp) + C);`, `FIELD(temp) |= 0x2000;`) becomes a local in a callee-saved register, where the original evaluated the address once "
          "inside a compound assignment (`FIELD(p->a->b) += C;`, r3 / r4); folded when the register is r3-r12, the temporary has no other use and C "
          "is a double for the f64 form (fn_80080B80 92 -> 100%; the pattern is in 70 drafts); `store_literals` (a variant): a float constant "
          "written as its literal only where it is a whole stored value, the constants in arithmetic and compares left as externs (fn_80080A94 "
          "98 -> 100%; all of them as literals, `float_literals`, gives 91%: the compiler swaps the fadds operands). fn_80080FD8 92 -> 93%: two "
          "more gap pointers (r29, r27) of unknown placement. **After** (full batch 12 + batch 9): compile " + V('compile_after') + " of 1,751, 100% MATCH "
          + V('match_before') + " -> " + V('match_after') + " (" + V('new_matches') + "), bytes equal " + V('bytes_before') + " -> " + V('bytes_after') + ", 80-99% "
          + V('near_before') + " -> " + V('near_after') + " (" + V('ups') + " drafts up, " + V('downs') + " down, " + V('lost') + " match lost); **" + V('objs') + " game objects ("
          + V('promoted') + " promoted, " + V('fileobjs') + " file objects, " + V('kept') + " kept), " + V('units') + " of " + V('units_all') + " units from C, " + V('code_pct')
          + "% of main.dol's code, " + V('data_pct') + "% of its data** (game " + V('game_bytes') + " bytes, " + V('game_pct') + "% of the game code). **Checks**: batch 9 "
          "main.dol retail SHA-1, every C object 100%, relocations OK, static shift PASS; batch 8 0 FAIL; checksums written (the 3 decompiled\\ "
          "lines kept at the committed values); batch 11 not run (no run closed). **Left**: fn_8006EEF8's vertex-block schedule (the compiler's "
          "priority, like the handler head of step 84), fn_80080FD8's two gap pointers, the other blockers of the table (docs/drafts.md \"Step "
          "85\"); the gap-register class (~198 unmatched game functions) now has its mechanism: a pointer local to a stack object with the "
          "right use count, a hand-match recipe rather than a generator rule.")
a = CR + '83. **Phase 4: game file objects and data from C'
i = s.index(a)
j = s.index(CR, i + 4)
assert j > i
s = s[:j] + CR + step84 + CR + step85 + s[j:]
# Resume bullet
rb = ("- **Step 85 (branch fileruns): file runs by hand - the register rules of level 0, two generator rules** (docs/drafts.md \"Step 85\": "
      "callee-saved registers go by use count from r31 down, declaration order only breaks ties; a saved-and-unused register between two live "
      "locals is a pointer local to a stack object, assigned and used through members only, folded onto r1 by the peephole; `clrlwi` on a copy is a "
      "u8 widened into a u32; a pool literal the compiler owns is scheduled freely, an extern waits behind a pointer store; fn_8006EEF8's frame "
      "exact, 548 of 755 words, its vertex-block schedule still the compiler's; `fold_temp_compound` and `store_literals` in the generator: "
      "fn_80080B80 and fn_80080A94 match): " + V('match_after') + " drafts at 100% (" + V('match_before') + " -> " + V('match_after') + "), " + V('compile_after')
      + " compile, bytes equal " + V('bytes_before') + " -> " + V('bytes_after') + ", " + V('objs') + " game objects (" + V('fileobjs') + " file objects), " + V('units')
      + " of " + V('units_all') + " units from C, " + V('code_pct') + "% of main.dol's code, " + V('data_pct') + "% of its data. No run closed. Left: fn_8006EEF8's "
      "schedule (the compiler's priority, not a spelling), fn_80080FD8's two gap pointers, the 5-blocker runs, data-only game units.")
anchor = CR + '- **Step 84 (branch handlers):'
assert anchor in s
s = s.replace(anchor, CR + rb + anchor, 1)
# super-goal items 4 / 5
old4 = ("(4) **game code 4.25% linked** (443 of ~1,750 functions, one object each but 4 file objects, steps 58-84; the game code is about 69% of all code): "
        "every new 100% draft is linked by batch 12 by itself; drafts with their own data (39 matched) link as file objects (step 63: the closed run of "
        "their pool users, all matched) once their runs are complete (the run table with every blocker: docs/drafts.md \"Step 83\"; the smallest, "
        "fn_8006EEF8..fn_8006FDD0, waits on fn_8006EEF8 alone, 0xBCC bytes, hand-written to 73% in step 83; the next on 5 functions each)")
assert old4 in s, 'old4'
new4 = ("(4) **game code " + V('game_pct') + "% linked** (" + V('objs') + " of ~1,750 functions, one object each but " + V('fileobjs') + " file objects, steps 58-85; the game code is about 69% of all code): "
        "every new 100% draft is linked by batch 12 by itself; drafts with their own data link as file objects (step 63: the closed run of "
        "their pool users, all matched) once their runs are complete (the run table with every blocker: docs/drafts.md \"Step 85\"; the smallest, "
        "fn_8006EEF8..fn_8006FDD0, waits on fn_8006EEF8 alone, 0xBCC bytes, hand-written to 548 of 755 words with an exact frame in step 85, the rest "
        "is the compiler's schedule; the next on 5 functions each; the hand-match recipes - use-count register order, the folded pointer behind a "
        "saved-and-unused register, u8-to-u32 clrlwi, owned literals - are in docs/drafts.md \"Step 85\")")
s = s.replace(old4, new4, 1)
old5 = "(5) the other ~1,270 game functions: 1,706 of 1,751 drafts compile (step 82), 43 do not"
assert old5 in s, 'old5'
s = s.replace(old5, "(5) the other ~1,270 game functions: " + V('compile_after') + " of 1,751 drafts compile (step 85), " + str(1751 - int(V('compile_after').replace(',', ''))) + " do not", 1)
old5b = "210 are at 80-99% (the script-command head's"
assert old5b in s, 'old5b'
s = s.replace(old5b, V('near_after') + " are at 80-99% (the script-command head's", 1)
wr(p, s)
print('plan.md done')

# ---- docs/decomp.md ----
p = os.path.join(G, 'docs/decomp.md'); s = rd(p)
old = re.search(r'step 84: 646 of 922 objects from C, 22\.18% of main\.dol\'s code, 5\.90% of its data; 480 game drafts at 100%: Dolphin SDK 98 of 98, MSL 49 of 49, Runtime 5 of 5,\r\nMetroTRK 29 of 29, HSD 22 of 30, game 443 \(0xCC44 bytes, 4\.25% of the game code; 4 file objects\)', s)
assert old, 'status'
s = s.replace(old.group(0), "step 85: " + V('units') + " of " + V('units_all') + " objects from C, " + V('code_pct') + "% of main.dol's code, " + V('data_pct') + "% of its data; " + V('match_after')
              + " game drafts at 100%: Dolphin SDK 98 of 98, MSL 49 of 49, Runtime 5 of 5,\r\nMetroTRK 29 of 29, HSD 22 of 30, game " + V('objs') + " (" + V('game_bytes') + " bytes, " + V('game_pct') + "% of the game code; " + V('fileobjs') + " file objects)", 1)
oldf = "The smallest open run (fn_8006EEF8..fn_8006FDD0) still waits on fn_8006EEF8 (hand-written to 73%: drafts.md)."
assert oldf in s, 'fileobj'
s = s.replace(oldf, oldf + "\r\nStatus after step 85 (Oct 31, 2026, branch fileruns): still " + V('fileobjs') + " file objects, no run closed. fn_8006EEF8's frame is now exact "
              "(548 of 755 words; what is left is the compiler's schedule inside its vertex blocks), and two of fn_8008057C..fn_800812A4's seven blockers "
              "match through two new generator rules (fn_80080B80, fn_80080A94). The hand-match recipes that came out of it (callee-saved registers by use "
              "count, a saved-and-unused register is a folded pointer local to a stack object, u8-to-u32 `clrlwi`, owned literals scheduled freely) and the "
              "updated run table are in [drafts.md](drafts.md) \"Step 85\".", 1)
wr(p, s)
print('decomp.md done')
