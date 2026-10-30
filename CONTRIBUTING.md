# Contributing

Thanks for helping! The most useful thing anyone can do is **name a function**. You don't need to be an expert.

## The two kinds of names

1. **Verified names** — `symbols/symbols.txt`. Real names, only with an independent source:
   - a decomp-toolkit signature match (SDK / runtime), or
   - an `__assert` match with the Melee decomp **and** size within 10%, or
   - layout proof: between two already-verified functions, in the same order as in a reference decomp, size within
     10%, and both neighbours also match (`tools/layout_match.py` does this automatically).

   `tools/verify_symbols.py` checks every one. Don't add guesses here.

2. **Descriptive names** — `symbols/descriptive.txt`. Our names for the parts of Custom Robo's own code the game gives
   no name to (check "Real names first" below before adding one). Anyone can add these.

## Real names first

The game still contains many of its own names: enum names in pointer tables (`SEQ_BATTLE`, `CAMMODE_RESULT`, motion names like `m_walk_fa`), debug and format strings, assert file names. **Before inventing a `cr_` name, look for the real one:** run `tools/name_evidence.py` and read `docs/name_evidence.md` (and `docs/name_tables.tsv`). Use the game's own word wherever it exists, and quote it as the evidence. Names the game proves mechanically (a name table next to a function table of the same length, ...) are applied by `tools/match_game_names.py` (batch 7, lines tagged `// game-real` in `symbols/descriptive.txt`: don't edit those by hand); the leads it could not prove are listed with the reason in `symbols/game_unnamed_real.txt`. Descriptive `cr_` names are only for things the game gives no name to.

## Adding a descriptive name (step by step)

Commands below name the Python tool only (`tools/find.py ...`): run it as `python -I tools\find.py ...` on Windows and
`python3 -I tools/find.py ...` on Linux / macOS ([how commands are shown](docs/README.md#commands-in-these-docs)).

1. Pick a function: the "Hubs" table in [docs/game_map.md](docs/game_map.md) lists the most-used unnamed ones,
   or search: `tools/find.py text <something>`.
2. Read it: `tools/find.py func FUN_80012345 --brief`, see who uses it (`callers`) and what it calls (`callees`).
3. Add one line to `symbols/descriptive.txt`:
   ```
   cr_Subsystem_Action = .text:0x80012345; // type:function scope:global // why: what in the code shows this
   ```
4. Check it: `tools/lint_descriptive.py`, then rebuild: `pwsh tools\batches\run_all.ps1` (Windows) /
   `tools/batches/run_all.sh` (Linux / macOS).
5. Open a pull request.

### Naming style

- Always start with `cr_`. Then `Subsystem_Action`, CamelCase: `cr_Archive_OpenEntry`, `cr_CharaEntry_OnLoaded`.
- Reuse existing subsystem words: `FileSys`, `Archive`, `FileSlot`, `Heap`, `RoboModel`, `CharaModel`, `WeaponModel`…
- Describe what the code **does**, not what you think the game uses it for, unless the `why:` proves it.
  `cr_Archive_OpenEntry` (provable) beats `cr_LoadRoboForBattle` (a guess).
- Not sure? Name it less specifically (`cr_Model_Relocate`), or leave a note in `symbols/unverified.txt` instead.

## Types, struct fields, parameters and globals

What turns `*(int *)(param_1 + 0xc)` into `archive->packed`:

| What | File | Source |
|---|---|---|
| HAL library structs + prototypes (JObj, DObj, …) | Melee's headers, parsed by `tools/scripts/ParseHeaders.java` | verified: header sizes/offsets **and** the sizes Custom Robo registers for each class (`tools/check_class_sizes.py`) |
| Custom Robo's differences from Melee's library | `OVERRIDES` in `tools/clean_headers.py` | each with the code that shows it |
| Our structs + prototypes for `cr_` functions | `symbols/cr_types.h` | every field: `/* +XX */` offset + the code that shows it; `ASSERT_SIZE` comment per struct |
| Our globals | `symbols/cr_globals.txt` | `cr_g_Name = .bss:0xADDR; // type:object datatype:Type[N] // why: ...` |

To add a struct: write it in `symbols/cr_types.h` with `/* +XX */` on every field and an
`// ASSERT_SIZE(cr_Name, 0xSIZE)` comment; unknown parts are `unk_XX`. The rebuild stops if any size or
offset doesn't match. Then give a function a prototype there (parameter names become the C's names) and
type the globals that hold it in `symbols/cr_globals.txt`.

## Matching a library file (Phase 4)

1. Pick a file from `CR_NOT_MATCHING` in `decomp/configure.py` (its reason says what blocks it), or an object of
   `decomp/config/GXCE01/splits.txt` that has no C file yet.
2. Copy it from a pinned reference with `tools/import_ref_source.py` (it adds the provenance line). Never copy by hand,
   never use Ghidra's or any decompiler's output. Commands: [docs/decomp.md, "Building library objects from C"](docs/decomp.md#building-library-objects-from-c-phase-4-milestone-4-oct-29-2026).
3. Build (Windows `cd decomp; python configure.py; ..\tools\ninja\ninja.exe`, Linux / macOS
   `cd decomp && python3 configure.py && ../tools/ninja/ninja`), compare with objdiff, adapt the C until every
   function matches, and check that `tools/check_c_objects.py --link-each` says `retail` for it.
   A link that fails on a name our symbols leave as `fn_` / `lbl_`: `decomp/reports/c_callsites_report.txt` lists it
   with its address; add it to `symbols/symbols.txt` as a `// c-callsite` line (docs/decomp.md, step 3). A global
   of the file itself that other objects use: a `// c-define` line, from the `DEF` lines of
   `decomp/reports/splits_report.txt` (rule R8). The object's data pieces come from the C (gen_dtk_splits.py).
4. Move it to `CR_MATCHING`, run `tools/make_configure_patch.py`, then batch 9 (`pwsh tools\batches\batch9_decomp_build.ps1`
   / `tools/batches/batch9_decomp_build.sh`): the object must be at 100% code and data and main.dol retail. Then the
   full run (`run_all`).
5. A file that cannot reach 100% stays in `CR_NOT_MATCHING` with its reason; it is never linked.

## Rules for everything

- **Commit all project work**: scripts, batches, symbols, types, reports, docs and findings, plus what is needed to
  rebuild them from a clean ISO (pins, expected checksums).
- **Never commit game data**: no ISO, extracted files, models, textures, audio, or raw decompiler output (Ghidra's C,
  m2c drafts in decomp/drafts/). Matched C in decomp/src IS the decomp and is committed (the game files there are
  written by tools/promote_drafts.py from drafts that compile to the retail bytes; docs/drafts.md).
  `.gitignore` blocks the usual paths; double-check `git status` before you commit.
- Tools should be rerunnable and keep working from a fresh setup (`pwsh tools\setup.ps1` / `tools/setup.sh`); a
  `.ps1` script and its `.sh` twin change together and write the same files.
- Python scripts run with `python -I` (Linux / macOS `python3 -I`); keep them standard-library only where possible.
- Docs: one place per topic, linked from the [docs index](docs/README.md); commands for both systems; run
  `tools/check_docs.py` before committing a doc change.
