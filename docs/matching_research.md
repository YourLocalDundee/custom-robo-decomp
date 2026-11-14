# Matching research: what other projects and the compiler's source say (Nov 13, 2026)

Written while working the "next 100" set of [drafts.md](drafts.md). The question was: what do other CodeWarrior
GameCube decompilations know about getting the last registers and instructions right, and what tools exist beyond
the ones already here ([tools.md](tools.md))? Everything below was read on the web (reference only; nothing was
copied into the repository) and then **checked on this game's compiler** (GC/2.6, `-opt level=0,peephole,schedule,
speed`) before it was used. Where the check disagreed, the measured behaviour is what is written.

## Sources

| Source | What it is | What was used |
|---|---|---|
| https://github.com/doldecomp/sms: `AGENT_MATCHING_TIPS.md` (in its docs folder) and `AGENTS.md` | Super Mario Sunshine (GC/1.2.5, `-O4,p`): a long list of matching facts, most of them read from the compiler's decompilation | the register rules, the stack frame arithmetic, the forms of removed debug code, "replay the allocator instead of guessing" |
| https://github.com/rayanht/mwcc (already in `tools/downloads/rayanht_mwcc`, never run) | a matching decompilation of `mwcceppc.exe` GC/1.0 - 1.3 | `src/backend/CodeGen.c` (`allocate_saved_gprs`, `allocate_object_registers`, the pass order of `CodeGen_Generator`), `src/optimizer/COptimizer.c` (how a variable's usage is counted), `src/frontend/CFunc.c` (statement weights), `src/backend/TOC.c` (pooled-data base objects) |
| https://github.com/zeldaret/tww: `regalloc.md` and `decompiling.md` (in its docs folder) | The Wind Waker (GC/1.3.2, C++): a cheat sheet of register swaps | declaration order, a local copy of a base pointer, casts, `const` on inline parameters, if / else against the ternary |
| https://github.com/simonlindholm/decomp-permuter (already set up, `tools/sched/permuter_setup.py`) | random and guided source permutation | `PERM_GENERAL` / `PERM_RANDOMIZE` exist for guided runs; not needed yet |
| https://github.com/macabeus/mizuchi, https://github.com/maci0/rebrew | pipelines around m2c, objdiff, the permuter and a language model; a genetic search over flags and source | read for ideas only: both are the loop this repository already runs (draft, compile, score, keep) |
| https://hackmd.io/vPmcgdaFSlq4R2mfkq4bJg | the IDO (N64) -O2 guide | the order of work: control flow, then the set of saved registers, then small reorderings, then registers |

Not found: a guide for this exact configuration (GC/2.x at level 0 with the peephole and the scheduler on). The
level-0 rules below are this project's own measurements, with the 1.x source as the hypothesis.

## 1. Saved registers are computed, not searched

At level 0 the compiler does **not** colour the named locals: `allocate_registers_and_local_slots` takes the
`allocate_saved_gprs` / `allocate_saved_fprs` path (the hook log shows the locals already in r26 - r31 in the first
scheduling pass, before colouring; only temporaries get virtual registers). The 1.x source of that function:

```
while a saved register is free:
    best = none, bestWeight = -1
    for each parameter, in order:        if usage >= bestWeight and usage >= 2: best = it
    for each local, in list order:       if usage >= bestWeight and usage >= 2: best = it
    for each pooled-data base object:    if usage >= bestWeight and usage >= 3: best = it
    give best the next register (r31 down / f31 down)
```

The local list is in reverse declaration order (each new local is prepended), and `>=` lets a later candidate
replace an equal earlier one. So:

- the variable with the highest usage gets r31;
- a tie goes to a local before a parameter, to the **first-declared** of two locals, to the **later** of two
  parameters;
- usage 1 gets no register: a parameter then stays in its stack home (`stw r3, 8(r1)`), a local goes to the stack;
- `volatile` and address-taken variables are never candidates.

**Usage on GC/2.6 at these flags** (measured by counting the dead uses that flip a pair of registers in
fn_800A44D4; the 1.x source adds the statement weight, x8 per loop level under `-opt speed`, and that is **not**
what this compiler does):

| Form | Usage added |
|---|---|
| any occurrence: read, write, argument, compare, `(void) v;`, also inside `if (0) { }` and inside a loop | 1 |
| `v += x`, `v++`, `++v`, `v = v` | 2 |
| an indirection through the variable with no cast between: `*v`, `*(v + 1)`, `v[i]`, `v->m`, a store through it | 2 |
| the same behind a cast: `*(T *) v`, `M2C_FIELD(v, T *, off)`, `((T *) v)[i]`; and `&v->m` | 1 |

Checked against the matched drafts whose locals m2c named after their registers: the rule and this table give the
register order of **200 of 207** functions (`usage_model.py`, scratch; x8 loop weights give 130).

`tools/rank_solve.py` uses it: the target register of a local is in its m2c name, of a parameter in the original's
entry block; the tool declares the locals in register order and adds the fewest `(void) v;` lines that make the
rule produce the target. One compile per function. Over the 671 open drafts: **137 up, +2,483 words, 1 match**
(cr_SYS_OVERLAY_WAIT) in about 20 minutes, where the search (`rank_sweep.py`) needed 80 minutes for 150 functions.

What a needed dead use *means* (the Sunshine guide calls a dummy that fixes allocation a pointer to the real
difference): the original used the variable once more than the draft does. Usually that is a use m2c folded away
(a second read of a parameter the draft reads through a copy, as in fn_80066F58), an indirection the draft writes
through a cast, or a compiled-out debug statement.

## 2. Pooled-data base registers are not variables

`lis r4, sym@ha; addi r31, r4, sym@l` at the top of a function, with many `N(r31)` accesses and sometimes a second
`addis r24, r31, 1`, is the compiler's own base object for the file's pooled data (the third loop above: usage at
least 3). m2c prints it as a local (`var_r31 = lbl_8021AF28;`) with byte offsets, which is why such a draft needs
dozens of dead uses of it. The original names the globals; `addi r3, r24, -27556; lwzx r0, r3, r0` is "address of
that global, then index". A draft compiled alone cannot pool, so this stays an emulation: far offsets written
through the second base (`var_r24 - 0x6BA4`), which took fn_800A3C0C from 75% to 90%. The `addi`-then-`lwzx`
shape itself is still open (every spelling folds the constant into the displacement; an inline helper that
returns the address gives the shape but takes a saved register per site).

**This was already established here on Nov 6** ([drafts.md](drafts.md), "the battle action file's statics";
`tools/file_drafts/battle_9B60/`): `pool.inc` declares the statics of that file and a scoring-only function that
references them in address order (the pool is laid out in the order of first reference), `score.py` compiles
`pool.inc` plus a draft, and `relcheck.py` maps the draft's `...bss.0` one to one onto the original's pool symbol.
fn_800456B8 and four others are at 100% there. This round re-derived it from scratch (the same recipe, 27 of 27
words for fn_800456B8) before finding the earlier work, which at least confirms it independently. What is new
is the count: **92 open drafts** have a `base_lbl_` local, in more files than the battle action one; each such
file needs its own `pool.inc`.

## 3. The stack frame

```
frame = round8( 8 + round8(locals) + 4 x saved GPRs + 8 x saved FPRs )
```

- A frame that is too small usually means missing inline expansions, not a missing local: an inlined call
  reserves a slot for its return value (when it returns one), for each argument that is an expression, and for
  each named local of its body, also when the final code never touches them.
- m2c's 1-byte or 4-byte slot for a buffer that is passed by address is the buffer's first element: the frame
  size gives the real size (`char sp8[84]` in fn_80108290, `char sp8[24]` in fn_8011ABA4).
- A local that is only written goes to the stack (usage 1) and its store can be removed, leaving the slot.
- Each int-to-float conversion site reserves 8 bytes above the named locals.

## 4. Forms the removed debug code leaves

- `if (cond) { }` keeps the compare alive until after register allocation (a dead `cmpwi` whose flags nothing
  reads); `if (cond) (void) 0;` drops it earlier. This game's originals have both shapes: `{ (void) 0; }` matched
  fn_8008891C, fn_80044174 and fn_80066F58 (a call's result compared with 0x1C000 for nothing).
- A discarded call of a function that returns a value keeps r3 busy to the end of the statement; of a void
  function not. One wrong return type in the context shifts the temporaries of every caller
  (`site_rules.py` voidcall / valuecall).

## 5. Control flow

- MWCC never swaps the two arms of an `if` and never reorders `case` labels; a loop at level 0 jumps to its test at
  the bottom (`b test` ... `blt body`).
- m2c prints a loop that leaves through a test as `loop_N:` with `goto loop_N;`. That form tests at the top and
  does not match. `tools/goto_loops.py` rewrites it as `while (C) { if (!D) { break; } ... }` (and three other
  forms, kept by score): fn_8009381C 90 -> 98%; over the 660 open drafts 66 up, +416 words.
- A branch target that only moves a value into the result register is a ternary; one that loads a constant is an
  early return.

## 6. Address sums (confirms the Nov 13 findings)

- Pointer arithmetic is normalised to `add rD, rBase, rOffset`; integer arithmetic keeps the source order.
- The constant of a sum goes into the displacement or into the index depending on where it is written:
  `((s32 *) (file + 0x10))[id]` is `add base, index; lwz 0x10(r)` (fn_80067778 and four siblings),
  `M2C_FIELD(B + (i * 2), s16 *, 0x20C)` is `lhax` with the index `i * 2 + 0x20C`.
- `text = base + off + 8`, flat and left to right, in one statement: two statements create the temporaries in the
  other order and the volatile registers swap (fn_80066D3C).

## 7. Other levers to remember

From the Sunshine and Wind Waker notes; not yet needed here or not yet checked on GC/2.6:

- a move between two named locals never coalesces (`mr r27, r28` stays); a move into a temporary does;
- `int` against `s32` for a float-to-int conversion (two `stfd` / `lwz` pairs against one);
- a redundant cast emits a dead `rlwinm` that the scheduler still sees, so it moves other instructions;
- `mr rD, rS` against `addi rD, rS, 0`: the late peephole rewrites a copy that is not first in its block;
- a local that feeds a call argument moves its computation in front of the argument moves (temporaries of an
  argument expression can never take r3 / r4);
- `&&` / `||` assigned to a bool makes flag temporaries (`li 0`, `li 1`, `clrlwi.`); nested ifs do not.

## 8. Found while using these rules (the bottom 100)

- **Not all of the game is level 0.** The sine / cosine file is optimised: `#pragma optimization_level 4` around
  cr_SinCos16, cr_Sin16, cr_Cos16. The sign in the original: locals in volatile registers without a frame, or a
  value kept in r0 / r4 / r5 across branches. `tools/opt_sweep.py` lists the drafts that score higher above level 0.
- **A global with the wrong type costs registers.** `// also defines: lbl_X` renames the context's declaration away
  and the draft declares the real one (`extern f32 lbl_801E6580[1025];`): the array's own address, not a cast
  pointer, is what the original indexes.
- **Typed code is the test.** fn_8001E6B0 written with real structs has usage 6 for six of its variables and
  the tie-breaks alone give the original's nine registers. When a reconstruction needs a dead use, a type or a
  use is still wrong; when it needs none, it is probably the source.
- **Volatile registers follow creation order backwards** (the Sunshine rule, section 1 of its notes): the
  temporary created last takes the lowest free register. cr_Mtx_Pop needed only its statements reordered.
- **Saved float registers holding partial sums are named locals**, reused per column in the matrix products,
  with the statements staged across the three rows.

## 9. Locals with initialisers, shared constants, the .bss pool (Nov 13, night)

- **A run of stack words copied from a `.rodata` label is a local with an initialiser** (63 open drafts).
  The constant is anonymous, emitted in declaration order, copied word by word (bytes for char arrays, with
  the tail through word temporaries when registers run out; halfwords for u16). Up to 8 bytes it lives in
  .sdata2. `tools/local_init.py` converts, `tools/dol_vals.py` prints the values.
- **Which local is declared first decides which copy spills**: fn_8011C028 has the 4-byte colour declared
  before the 16-byte table although it lies below it on the stack. "Later declared, lower address" holds for
  most frames, not all; try both orders.
- **The same constant in several functions is an inline function** (one constant per function definition):
  cr_E_VIEWIN / cr_E_VIEWOUT are fn_800B4208 inlined, fn_800B4278 / fn_800B44D0 expand one two-table inline
  twice, cr_E_CLOCKIN / cr_E_CLOCKOUT are fn_800B44D0 inlined. A table copied in the middle of a function
  (after its prologue) is the sign. Constant arguments of the inline fold its tests away, also at level 0.
- **Constants of one file are pooled too**: a function with several initialised locals reads them through
  one base, the first constant of the file, with offsets (fn_8008C430: +0x10). The draft carries a
  scoring-only function in front that owns the earlier bytes; relcheck maps `...rodata.0` onto the label.
- **The .bss pool has a threshold**: a function touching three or more objects of its own file's .bss
  addresses all of them from the file's first .bss object (cr_E_CLOCKIN, three objects, base lbl_803B8C00);
  with two it uses each symbol (fn_800B44D0). So the same inline gives pooled code in one caller and plain
  code in another. `tools/pool_prelude.py` writes the definitions that reproduce the pool in a draft.
- **m2c prints stores in scheduled order, not source order.** fn_800B44D0: the rate is assigned before the
  flag is set; the register of the 0x4330 conversion constant (r5, above the flag's r0) shows it was
  created first. When two volatile registers are swapped between two independent statements, swap them.
- **A flat sum keeps its constant where it is written**: `n * 30 + n * 30 + 30` gives `add` then nothing
  moved; `n * 30 + (n * 30 + 30)` has the constant pulled out to the end (fn_800FC290).
- **A compound assignment through an index keeps base and scaled index in saved registers**:
  `total[robo.team] += robo.hp` (u32 += f32) holds `&total` in r25 and `team * 4` in r24 (fn_8004FF64); m2c
  shows two extra locals.
- **Uncast indexing counts 2 again**: cr_E_CLOCKIN's script pointer read as `pc[0]` / `pc[1]` ranks above
  the count; through `M2C_FIELD` casts it ranked below (section 1).

## 10. Casts are temporaries; the pool as a method (Nov 13 - 14, the next bottom 100)

- **A cast that changes nothing is still one temporary**, and the volatile registers of a block are handed
  out over all its temporaries: one missing or one extra shifts registers far from the cast. The script
  prologue needs `(s16) pc[0]` (81 -> 100% in cr_E_CIN); fn_800B1358 needs m2c's `(u8)` / `(s16)` casts
  removed. When a straight-line block has the right instructions and only volatile registers differ, and no
  statement order fixes it, look for a cast to add or remove before anything else.
- **A dead variable counts too**: `if (0) { (*cmd_cnt)++; }` behind the prologue (the variable is declared,
  the block compiles to nothing) is needed for the same commands.
- **`(void) arg0;` for an unused parameter** keeps its register (r3) out of the temporaries (fn_80069008).
- **Member or object?** On a pool, `addi rX, base, off` followed by a displacement is a member of the object
  at `off`; a folded `disp(base)` is the object's first member (or its only one). Two adjacent ranges
  addressed with their own `addi` are two objects, whatever the symbol table says (cr_FileSys_Init).
- **A global struct array indexed by a variable** is base + (index * size + member), indexed load: the
  door / light / object tables of the script file. A 2D table `T tbl[2][2]` gives the two multiplies.
- **m2c's comma condition** `if ((p != NULL) && (a = x, b = y, cond))` is `if (p != NULL) { a = x; b = y;
  if (cond) { ... } }`.
- **Argument reads count differently**: `pc[k]` is two uses of pc in the register order, a cast read one.
  Commands differ in which they need (the pool base, the object pointer and pc are close in rank); the
  converter tries both, and a tie is settled by the order of the declarations.
- **The same function inlined is the usual reason for "the same code with one more register"**:
  fn_80098290 = fn_80098104(id, 0x7F), fn_80068E50 / fn_80068F20 = fn_80069008, the three script
  commands of section 9.
- Open: a loop counter that the original ranks below the pool base and ours above it (cr_EV_TRAIN_END,
  cr_OBJ_ARROW_CLEAR: counter 8 - 10 uses); dead references to pool objects do not raise the base.

## 11. Tools considered

| Tool | Verdict |
|---|---|
| `rank_solve.py`, `goto_loops.py` (new, this repository) | built from sections 1 and 5; in [tools.md](tools.md) |
| the compiler-state debugger the Sunshine project describes (`mwcc-izer`: dumps the AST, PCode, interference graph and frame per pass) | not public as far as the search shows; this repository's `tools/sched/hook` already logs the scheduler's input and picks for GC/2.6, and the first-pass log shows which locals are pre-assigned. Extending the hook to dump the colouring of the temporaries is the next step for the volatile-register differences |
| https://github.com/cadmic/mwcc-debugger (in `tools/downloads`, addresses for GC/2.6) | the source of the hook's addresses; its pass breakpoints are where such a dump would go |
| decomp-permuter | kept for single functions with a register-only residue; 12 minutes on fn_800E9C90 gave 85%, no match |
| language-model pipelines (mizuchi and similar) | the same draft / compile / score loop; nothing to add |
