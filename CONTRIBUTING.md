# Contributing

Thanks for helping! The most useful thing anyone can do is **name a function**. You don't need to be an expert.

## The two kinds of names

1. **Verified names** — `symbols/symbols.txt`. Real names, only with an independent source:
   - a decomp-toolkit signature match (SDK / runtime), or
   - an `__assert` match with the Melee decomp **and** size within 10%, or
   - layout proof: between two already-verified functions, in the same order as in a reference decomp, size within
     10%, and both neighbours also match (`tools/layout_match.py` does this automatically).

   `tools/verify_symbols.py` checks every one. Don't add guesses here.

2. **Descriptive names** — `symbols/descriptive.txt`. Our names for Custom Robo's own code (nobody outside Noise
   knows the real ones). Anyone can add these.

## Adding a descriptive name (step by step)

1. Pick a function: the "Hubs" table in [docs/game_map.md](docs/game_map.md) lists the most-used unnamed ones,
   or search: `python -I tools\find.py text <something>`.
2. Read it: `python -I tools\find.py func FUN_80012345 --brief`, see who uses it (`callers`) and what it calls (`callees`).
3. Add one line to `symbols/descriptive.txt`:
   ```
   cr_Subsystem_Action = .text:0x80012345; // type:function scope:global // why: what in the code shows this
   ```
4. Check it: `python -I tools\lint_descriptive.py`, then rebuild: `pwsh tools\batches\run_all.ps1`.
5. Open a pull request.

### Naming style

- Always start with `cr_`. Then `Subsystem_Action`, CamelCase: `cr_Archive_OpenEntry`, `cr_RoboModel_OnLoaded`.
- Reuse existing subsystem words: `FileSys`, `Archive`, `FileSlot`, `Heap`, `RoboModel`, `CharaModel`, `WeaponModel`…
- Describe what the code **does**, not what you think the game uses it for, unless the `why:` proves it.
  `cr_Archive_OpenEntry` (provable) beats `cr_LoadRoboForBattle` (a guess).
- Not sure? Name it less specifically (`cr_Model_Relocate`), or leave a note in `symbols/unverified.txt` instead.

## Rules for everything

- **Never commit game data**: no ISO, extracted files, models, textures, audio, or decompiled C.
  `.gitignore` blocks the usual paths; double-check `git status` before you commit.
- Tools should be rerunnable and keep working from a fresh `tools\setup.ps1`.
- Python scripts run with `python -I`; keep them standard-library only where possible.
