# Making a mod

A mod is a folder with only the things you changed. One command turns it into an ISO you can play and a
small `.crpatch` file you can share. The patch contains **no game data**: everything that also exists on the
clean disc is stored as a reference to the player's own ISO, so only your new bytes are inside.

Before you start, run the one-time build (`pwsh tools\from_scratch.ps1 -Iso ...`, see the README), so you
have `models_dat\` (every model, editable) and `Custom Robo extracted\`.

## 1. Make the folder

```
mods\my_mod\
├─ mod.json                         {"name": "my_mod", "author": "you", "description": "what it does"}
├─ models\chara\chara_005.dat       edited models, SAME name as in models_dat\ (category folder + file name)
├─ files\font.tpl                   any disc file replaced as-is (textures, text, sound, ...)
└─ code\fixes.txt                   small code changes (see 4)
```

Everything is optional; use what your mod needs.

## 2. Edit a model

1. Find it in `models_export\<category>\<name>\` (look at the `.glb` in Blender) and its editable twin in
   `models_dat\<category>\<name>.dat`.
2. Open the `.dat` in HSDRawViewer (`tools\HSDLib\HSDRawViewer\bin\Release\net8.0-windows7.0\HSDRawViewer.exe`),
   select `root0_joint`, and edit or import over it (keep the same skeleton: same bones, same order).
3. Save it as `mods\my_mod\models\<category>\<same name>.dat`.

Supported categories: `chara` (robos), `weapon_gun`, `weapon_bom`, `weapon_pod`, `weapon_leg`, `snr_chara`
(story characters), `bg_data` (stages), `leg_effect`, `snr_obj`. Effects in `*_effect` containers are not yet.

### A robo with a different skeleton (more / fewer / other bones)

The game moves a robo's arms and legs itself, through **19 role bones** it looks up in the model (head, two
4-bone arms, two 4-bone legs, upper and lower body). Robos already differ (51-69 bones); a new skeleton is fine
as long as you say which of your bones plays each role. Next to your `.dat`, put a roles file:

```powershell
python -I tools\robo_joints.py roles unpacked\chara\000.bin > mods\my_mod\models\chara\chara_000.roles.json
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
**without** a roles file, it stops and tells you the command above. Tested: robo 5's model (59 bones) in robo 0's
slot (55 bones) builds, its patch is 32 KB with 177 new bytes, and the ISO boots. Not yet confirmed in a battle.

## 3. Build it

```powershell
python -I tools\make_mod.py mods\my_mod
```

Output:
- `build\mods\my_mod\my_mod.iso` — play it in Dolphin (Open, or drag it in)
- `mods\my_mod\my_mod.crpatch` — share this

Checks on the way: each rebuilt model must parse like the game's own loader parses it; every replaced file
must exist on the disc; the result is written from your clean ISO (SHA-1 checked).

## 4. Code changes (`code\*.txt`)

One change per line, RAM address (as in the decompiled C / Ghidra), new bytes, and the bytes you expect to
replace:

```
; make FUN_80012345 return 1 immediately
80012345: 38600001 4E800020 [was 9421FFE0 7C0802A6] ; li r3,1 / blr
```

`[was ...]` is checked against the clean `main.dol`; if it does not match, nothing is built. Always give it.
Find addresses with `python -I tools\find.py func <name>` and the Ghidra project.

## 5. Players: apply a patch

```powershell
python -I tools\iso_patch.py apply "Custom Robo (USA).iso" my_mod.crpatch "Custom Robo - my_mod.iso"
python -I tools\iso_patch.py info my_mod.crpatch        # what's in it
```

`apply` refuses any ISO that is not the clean US v1.0 disc (SHA-1 `fff6e745…0bcb16`) and checks the result's
SHA-1, so a player gets exactly the mod author's ISO. Needs only Python 3 — no other tools.

## How it works (for the curious)

`tools\make_mod.py` → models go back into their archive (`crmodel.from_dat` with the original entry as the
template, then `sfd.build`), files are copied, code lines patch a copy of `main.dol` → `tools\iso_patch.py build`
writes each changed file over the clean ISO (in place if it fits, else appended, file table updated) →
`tools\iso_patch.py diff` stores the difference: blocks found in the clean ISO (also when moved) become
references, the rest the changed bytes only.
Tested: robo 0 recolour → 43 KB of new bytes in the patch; apply on a clean ISO gives the same SHA-1;
the ISO boots in Dolphin.
