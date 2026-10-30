# Making a mod

A mod is a folder with only the things you changed. One command turns it into an ISO you can play and a
small `.crpatch` file you can share. The patch contains **no game data**: everything that also exists on the
clean disc is stored as a reference to the player's own ISO, so only your new bytes are inside.

Before you start, run the one-time build (`pwsh tools\from_scratch.ps1 -Iso ...` on Windows,
`tools/from_scratch.sh ...` on Linux / macOS; see the [README](../README.md#quick-start)), so you have `models_dat/`
(every model, editable) and `Custom Robo extracted/`. Commands below come in pairs, Windows first, then Linux / macOS.
Editing models in HSDRawViewer and the glTF export are Windows-only; building and applying mods works everywhere.

## 1. Make the folder

```
mods/my_mod/
├─ mod.json                         {"name": "my_mod", "author": "you", "description": "what it does"}
├─ models/chara/chara_005.dat       edited models, SAME name as in models_dat/ (category folder + file name)
├─ files/font.tpl                   any disc file replaced as-is (textures, text, sound, ...)
└─ code/fixes.txt                   small code changes (see 4)
```

Everything is optional; use what your mod needs.

## 2. Edit a model

1. Find it in `models_export/<category>/<name>/` (look at the `.glb` in Blender) and its editable twin in
   `models_dat/<category>/<name>.dat`.
2. Open the `.dat` in HSDRawViewer (`tools\HSDLib\HSDRawViewer\bin\Release\net8.0-windows7.0\HSDRawViewer.exe`),
   select `root0_joint`, and edit or import over it (keep the same skeleton: same bones, same order).
3. Save it as `mods/my_mod/models/<category>/<same name>.dat`.

Supported categories: `chara` (robos), `weapon_gun`, `weapon_bom`, `weapon_pod`, `weapon_leg`, `snr_chara`
(story characters), `bg_data` (stages), `leg_effect`, `snr_obj`. Effects in `*_effect` containers are not yet.

### A robo with a different skeleton (more / fewer / other bones)

The game moves a robo's arms and legs itself, through **19 role bones** it looks up in the model (head, two
4-bone arms, two 4-bone legs, upper and lower body). Robos already differ (51-69 bones); a new skeleton is fine
as long as you say which of your bones plays each role. Next to your `.dat`, put a roles file:

```powershell
python -I tools\robo_joints.py roles unpacked\chara\000.bin > mods\my_mod\models\chara\chara_000.roles.json
```

```bash
python3 -I tools/robo_joints.py roles unpacked/chara/000.bin > mods/my_mod/models/chara/chara_000.roles.json
```

Then change the numbers to your model's bones (`JOBJ_n` in HSDRawViewer / Blender = joint number n):

```json
{
  "B_material_joint": 1,
  "C": { "upper_body": 28, "head": 32, "arm_a_shoulder": 35, "arm_a_upper": 37, "arm_a_fore": 39, "arm_a_hand": 41,
         "arm_b_shoulder": 45, ..., "lower_body": 4, "leg_a_thigh": 8, ..., "leg_b_foot": 25 },
  "A_from": "chara_005"
}
```

- `C`: the 19 roles (table in docs/formats/model_entry.md). `null` = this robo has no such bone.
- `B_material_joint`: a body bone whose material the game keeps (joint 1 in most robos; use the same body part).
- `A_from` (optional): copy the limb measurements (block A) from the robo whose proportions are closest to yours;
  otherwise the original robo's are kept.

`make_mod.py` writes the new bone paths and checks they lead to exactly those bones. If you change the skeleton
**without** a roles file, it stops and tells you the command above. Tested: robo 1's model (entry 005, 59 bones) in robo 0's
slot (55 bones) builds, its patch is 32 KB with 177 new bytes, and the ISO boots. Not yet confirmed in a battle.

### Robo numbers vs. entry numbers

Each robo has **5 entries** in `chara.BIN` (variants, probably colours - not verified): **robo N = entries 5N to 5N+4**. The game loads entry `robo number x 5` (`FUN_800a63c0`). So `chara_000`-`chara_004` are robo 0 (Ray 01), `chara_020`-`chara_024` are robo 4, `chara_005` is robo 1 variant 0. `tools/robo_joints.py table` lists them as robo/variant. To change a robo everywhere, replace all 5 of its entries.

**Quick test - swap two robos completely** (all 5 variants, models + skeletons + roles + limb measurements):

```powershell
python -I tools\robo_swap.py 0 7      # robo 0 (Ray 01) <-> robo 7
```

```bash
python3 -I tools/robo_swap.py 0 7     # robo 0 (Ray 01) <-> robo 7
```

It swaps the bodies **and** two per-robo values the game looks up by robo number: the **motion set**
(`cr_g_CharaMotionSets`) and **Y offset** (`cr_g_CharaYOffset`, added to the character's Y position). Both belong to the
story-mode **field characters** (their 17 motions are wait / walk / run / talk_* / battle_in..out, plan.md step 40). Tested: swapping the
motion set does **not** change battle movement. Whether these tables only matter for story-mode scenes, and what picks
battle movement, is still open (plan.md step 39). The model's own animation travels with the model anyway.

| Option | Does |
|---|---|
| `--model-only` | swap just the bodies (motion set and Y offset stay with the number) |
| `--keep-motion` | swap bodies + Y offset, keep the animation sets with the robo number |
| `--size X` | sets the root joint scale - **has no effect in game** (overridden at run time; plan.md step 29) |

**Size.** The game has no per-robo size setting: the size factor the limb code uses is 1.0 for every robo and every
model's root joint scale is 1.0, so a robo's size is in its vertices. `--size X` sets the root joint scale of the
swapped models to X. **Tested: no effect** - the root scale is overridden at run time (most likely by the animations).
Bigger robos need scaled vertices or another hook; open.

Community tests so far (Oct 27, 2026; plan.md step 29): bodies swap and fight fine (also with different skeletons);
with bodies only, a body runs with the slot's animations; changing `cr_g_CharaYOffset` does **not** change the size.

## 3. Build it

```powershell
python -I tools\make_mod.py mods\my_mod
```

```bash
python3 -I tools/make_mod.py mods/my_mod
```

Output:
- `build/mods/my_mod/my_mod.iso` — play it in Dolphin (Open, or drag it in)
- `mods/my_mod/my_mod.crpatch` — share this

The clean ISO is taken from `Custom Robo (USA).iso` in the repo root, or from a second argument.

If an archive changed (any model with a new size), `main.dol`'s own copy of that archive's entry table is updated too - the game reads that copy, not the archive header (docs/formats/model_entry.md). Checks on the way: each rebuilt model must parse like the game's own loader parses it; every replaced file
must exist on the disc; the result is written from your clean ISO (SHA-1 checked).

## 4. Code changes (`code/*.txt`)

One change per line, RAM address (as in the decompiled C / Ghidra), new bytes, and the bytes you expect to
replace:

```
; make FUN_80012345 return 1 immediately
80012345: 38600001 4E800020 [was 9421FFE0 7C0802A6] ; li r3,1 / blr
```

`[was ...]` is checked against the clean `main.dol`; if it does not match, nothing is built. Always give it.
Find addresses with `tools/find.py func <name>` ([workflows.md](workflows.md#5-read-and-search-the-code)) and the Ghidra project.

## 5. Players: apply a patch

```powershell
python -I tools\iso_patch.py apply "Custom Robo (USA).iso" my_mod.crpatch "Custom Robo - my_mod.iso"
python -I tools\iso_patch.py info my_mod.crpatch        # what's in it
```

```bash
python3 -I tools/iso_patch.py apply "Custom Robo (USA).iso" my_mod.crpatch "Custom Robo - my_mod.iso"
python3 -I tools/iso_patch.py info my_mod.crpatch       # what's in it
```

`apply` refuses any ISO that is not the clean US v1.0 disc (SHA-1 `fff6e745…0bcb16`) and checks the result's
SHA-1, so a player gets exactly the mod author's ISO. Needs only Python 3 — no other tools.

## Source-route modding (Phase 4, work in progress)

The patch route above changes bytes in place, so anything new must fit where the old thing was (or use Gecko / free space). The Phase 4 build (batch 9, [REPRODUCE.md](REPRODUCE.md) section 2b) rebuilds `main.dol` from split objects, and since plan.md step 51 it is **shiftable**: every pointer in code and data is a relocation (dtk's mistakes are fixed by `tools/gen_dtk_relocs.py`), so code and data can move and grow and the linker updates every reference. The static test (`tools/shift_test.py`, batch 9) adds 0x100 bytes at 30 points in code and data (`decomp/reports/shift_report.txt`), and batch 11 (Windows) boots that build to the title screen.

What this enables: a bigger table (a new robo slot in the motion / model / name tables) or new code is an edit of the split asm or the C plus a relink, with no free-space hacks.

Still missing before it is a normal workflow (Oct 29, 2026): a battle tested by hand in the shifted build ([REPRODUCE.md](REPRODUCE.md) section 2c); the game's own data is still in dtk's automatic objects (only library objects have proven data pieces, plan.md step 53), so growing a game table is only proven safe at a file boundary (where the code stops addressing data from one base plus offsets); no mod tool drives the build yet (make_mod.py still patches bytes); and most code is still asm: 324 of the 620 units are built from C, 5.41% of the code, of the game code 232 functions, 0.77% (`decomp/reports/progress_report.txt`).

## How it works (for the curious)

`tools/make_mod.py` → models go back into their archive (`crmodel.from_dat` with the original entry as the
template, then `sfd.build`), files are copied, code lines patch a copy of `main.dol` → `tools/iso_patch.py build`
writes each changed file over the clean ISO (in place if it fits, else appended, file table updated) →
`tools/iso_patch.py diff` stores the difference: blocks found in the clean ISO (also when moved) become
references, the rest the changed bytes only.
Tested: robo 0 recolour → 43 KB of new bytes in the patch; apply on a clean ISO gives the same SHA-1;
the ISO boots in Dolphin.
