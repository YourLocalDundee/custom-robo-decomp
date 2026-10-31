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

Or script the edit in Blender and let one command do export, import and the build: [Blender loop](#blender-loop).

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
The [Blender loop](#blender-loop) writes the roles file itself when a script adds bones (`--skeleton glb --roles`):
the step 73 legionnaire (Ray 01's 55 joints + 3 bones = 58) builds and boots that way; also not yet seen in a battle.

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

## Blender loop

Edit a robo in Blender with a script, or build a new one, and get a bootable ISO and a `.crpatch` in one command
(plan.md steps 70 and 73). By default the loop keeps the robo's skeleton exactly (same bones, same order, names
`JOBJ_n`), so the game's 19 role bones, the animations and blocks A/B/C stay valid and no roles file is needed; a
script can also replace every mesh, material and texture and add bones (the importer then writes the roles file).

```powershell
pwsh tools\model_loop.ps1 -Mod ray_legionnaire -Script legionnaire_v2 -Entries 000,001,002,003,004 -Boot   # complete replacement
pwsh tools\model_loop.ps1 -Mod ray_legionnaire_tskel -Script legionnaire_v2_template -Entries 000,001,002,003,004 -Boot
pwsh tools\model_loop.ps1 -Mod rt_test -Script roundtrip -Entries 000      # no edit: tests the loop itself
```

```bash
tools/model_loop.sh ray_legionnaire legionnaire_v2 000,001,002,003,004 --boot
tools/model_loop.sh ray_legionnaire_tskel legionnaire_v2_template 000,001,002,003,004 --boot
tools/model_loop.sh rt_test roundtrip 000
```

For each entry (`chara_<entry>`; robo N = entries 5N..5N+4, the game loads 5N, see above):

1. **export** (once; `-Fresh` / `--fresh` to redo): `HSDRawViewer --export` of `models_dat/chara/chara_<entry>.dat` to
   `build/model_loop/<mod>/export/chara_<entry>/root0_joint.glb`.
2. **roles**: `tools/robo_joints.py roles unpacked/chara/<entry>.bin` (which joint is the head, each arm, each leg),
   passed to the script.
3. **Blender**, headless: `blender -b --factory-startup --python-exit-code 1 --python tools/blender/<script>.py --
   <in.glb> <out.glb> <roles.json>`. A script may write `<out>.args` (one import option per line).
4. **import**: `HSDRawViewer --import models_dat/chara/chara_<entry>.dat <out.glb> mods/<mod>/models/chara/chara_<entry>.dat
   [options from <out>.args]`; with `--roles` it also writes `chara_<entry>.roles.json` next to the .dat.

Then `tools/make_mod.py mods/<mod>` (ISO in `build/mods/<name>/`, patch in `mods/<mod>/`; a body entry over the game's
0x3A000-byte load buffer is refused) and, with `-Boot`, a 40 s `tools/boot_check.ps1` (Dolphin must still run and
show no warning dialog; it cannot see the picture, plan.md step 20).
Options: `-Iso` / `--iso` (clean ISO), `-Blender <exe>` or `BLENDER=...` (default: Blender on PATH, else the newest
`C:\Program Files\Blender Foundation\Blender *`), `-Category` (default `chara`), `-Seconds`.
HSDRawViewer is a Windows program: the bash twin runs every step under Git Bash on Windows; on Linux / macOS it skips
export and import (run them on Windows) and still runs Blender and `make_mod.py`.
A mod folder's `mod.json` and `notes.txt` are committed (the recipe); its models, patch and ISO stay local.

Preview a glb (as built, or a built .dat exported again) without the game:

```powershell
blender -b --factory-startup --python tools/blender/render.py -- <model.glb> <out.png> [<pose.json> [<chara_NNN.roles.json>]]
```

Four views (front, three-quarter, side, back; the robos face Blender +Y) in one PNG. `legionnaire_v2.py` writes its
own preview (`<out>.preview.png`) and a `<out>.pose.json` that moves the hand / foot parts of a re-export from the IK
effectors' bind positions to where the game draws them.

Tested (Oct 30, 2026): the `roundtrip` loop on entry 000 builds an ISO with the clean disc's SHA-1 (0 changed files);
the step 70 `legionnaire` (crest and shield added to Ray 01) was seen in the Customize screen by the user
(docs/images/ray_legionnaire_v1_customize.jpg); the step 73 `legionnaire_v2` (complete replacement, below) and its
template-skeleton twin build for entries 000-004, re-export to what Blender made (docs/images/legionnaire_v2_blender.png
vs legionnaire_v2_reimport.png) and boot (40 s, no warning). **Not yet seen in game**: an in-person tester checks the
legionnaire in the Customize screen and in a battle.

### Writing a script

Start from `tools/blender/legionnaire_v2.py` (a complete model) or `legionnaire.py` (small additions);
`tools/blender/common.py` loads and saves with the loop's settings. Rules:

- Bones: keep the robo's `JOBJ_n` bones (the IK chains, their lengths and the role bones are what the game's motions
  and limb code expect; block A, the limb measurements, stays the robo's). New bones (any other name) need
  `--skeleton glb` and a roles file: write `<out>.roles_by_name.json` (`{"B_material_joint": "JOBJ_1", "C": {"head":
  "JOBJ_32", ...}}`, bone names) and put `--skeleton glb --roles <that file>` in `<out>.args`. A new bone is a
  joint axis-aligned in world space at its head, under its Blender parent.
- Geometry: mesh objects parented to the armature with an **Armature modifier** and **vertex groups named after
  bones**. A triangle whose three vertices are 100% on one bone is rigid (a DObj on that joint, like the game's own
  parts); any other triangle is smooth skinned (an envelope DObj, up to 4 bones per vertex, weights in 0.1 steps).
  Build in world space, in the bind pose the glb shows. Apply every other modifier yourself.
- Replacing the robo: delete its meshes (`Joint_<j>_Object_<d>...`) and put `--replace` in `<out>.args`: template
  DObjs missing from the glb are dropped, a mesh with an exporter name but new geometry replaces that DObj. Without
  `--replace` the import is additive (step 70) and geometry edits to the robo's own meshes are ignored.
- Materials: Principled BSDF with an **Image Texture on Base Color** (PNG, power of two, 8-1024) becomes a textured
  material like the robo's own (see "How the game associates textures with meshes"); **Metallic** sets the specular
  mask strength; **Backface Culling** off (glTF doubleSided) = no culling (thin strips, cloth), on = the game's
  CULLFRONT. Without an image: one flat colour (Base Color), the step 70 material. UVs: the first UV map.
- The block B joint (`B_material_joint`, JOBJ_1 for Ray 01) must keep a mesh: the game reads its material. The
  envelope (smooth) DObjs go to the robo's ENVELOPE_MODEL joint (JOBJ_1), which is enough.
- IK end joints (hands, feet: effector chains): the game data keeps their bind pose at odd places (Ray 01's hands
  float beside the body, its feet lie on their backs, toes sideways, -Y = sole). Build such parts where they belong,
  then move them like the robo's own hand / foot meshes are moved (legionnaire_v2.py `effector`: a translation for
  the hands, a turn for the feet, measured from the robo's meshes), and weight them 100% to the hand / foot joints.
- Ray 01 faces **Blender +Y** (glTF -Z; the face and chest gem are on +Y): arm_a (-X) is its **left** arm. The
  step 70 note (faces -Y) was wrong; the v1 photo shows its shield, put on arm_b, on the gun arm.
- Size: the body entry must stay under 0x3A000 bytes (the game's per-robo load buffer). legionnaire_v2: 3,756
  triangles + 7 images (128 / 64 / 256) = 0x38560. Images cost most: a 256x256 CMP is 32 KB (+ 8 KB specular mask).

### Blender settings (`tools/blender/common.py`)

- Import: glTF importer defaults (bone heuristic "Blender", guess original bind pose). Blender re-orients bones; that
  does not matter: the importer takes every template bone's rotation from the template and only positions, names
  and parents from the glb. The importer's hidden bone-shape `Icosphere` (collection `glTF_not_exported`) is not part
  of the model.
- Export: GLB, +Y up, skins on, no animations, no shape keys, materials exported, images in their own format (PNG),
  texture coordinates and normals on, modifiers not applied, no deform-only bone filter, only visible objects.
- Run with `--factory-startup` (no user add-ons or preferences) and `--python-exit-code 1` (a script error fails the loop).
- Baking (legionnaire_v2.py): Cycles on the CPU, 16 samples, Diffuse colour pass x Ambient Occlusion, 4-pixel margin;
  Smart UV Project (55 degrees, margin 0.02), then chosen faces scaled up and the islands packed again.

### What `HSDRawViewer --import` keeps and changes

`--import <template.dat> <edited.glb> <out.dat> [--replace] [--skeleton template|glb] [--roles <by_name.json>]
[--envelope-joint JOBJ_n]` (`ModelImport.cs`, in tools/patches/hsdlib_headless_export_resave.patch) imports **over**
the template's `root0_joint` instead of building a new model. HSDRawViewer's own importer (`ModelImporter`) rebuilds
every joint, DObj and material from the glTF: Ray 01's single shared material (the one block B points at), joint
flags, IK settings and texture settings would all be lost. Settings chosen:

| Setting | Choice | Why |
|---|---|---|
| Skeleton, `--skeleton template` (default) | the template's; the glb must have exactly `JOBJ_0..n-1` with the template's parents | role bones, animations and blocks A/B/C address joints by tree position |
| Skeleton, `--skeleton glb` | the glb's tree under `JOBJ_0`: a `JOBJ_n` bone reuses template joint n (flags, IK type, robjs, rotation, scale; moved more than 0.001: new translation and inverse bind); another name is a new joint; template joints not in the glb are dropped; siblings ordered template joints first, then new bones by name; numbered in pre-order | new bones (or fewer) without losing the IK setup; an unchanged tree stays byte-identical |
| Roles, `--roles` | the bone names of a roles file turned into the output numbers, written as `<out>.roles.json` (plus a `joints` list, number -> bone name); `make_mod.py` rebuilds blocks B and C from it | the script knows its bones by name, only the importer knows the numbering |
| Vertex space | the template's bind pose exactly as `--export` writes it (inverse bind matrices where a joint has one, else joint SRT on the parent's) | parts land where they were in Blender (a re-export matches) |
| Template meshes | matched by the exporter's names `Joint_<j>_Object_<d>`; same geometry: kept byte-identical; changed / missing: ignored / kept, or with `--replace` replaced / dropped | the game's own data stays when it is not edited |
| Texture edits of kept meshes | the base colour texture (the last diffuse-lightmap TObj) re-encoded **in its own format** (CMP for robos) when over 0.1% of its pixels differ by more than 8 | CMP as the game uses; PNG noise is not an edit |
| Colour edits of kept meshes | a Base Color factor that differs from the MObj's diffuse colour becomes the diffuse colour | Ray 01's body texture blends over its diffuse colour, so recolour the texture instead |
| New rigid geometry | triangles 100% on one bone: one DObj per (joint, material) on that joint, vertices in the joint's space | rigid like the robo's own parts |
| New skinned geometry | other triangles: one envelope DObj per material on the ENVELOPE_MODEL joint (`--envelope-joint` to choose), PNMTXIDX + up to 4 weights per vertex rounded to 0.1 (the game's bodies use 0.1 steps; fewer envelopes = fewer PObjs), vertices in the bind pose; every joint an envelope uses gets an inverse bind (= its bind pose) and the SKELETON flag | the game's own bodies skin the same way (Ray 01's J1 body: 14 envelopes over 9 joints) |
| Single-weight vertices of an envelope | in the bind pose (world) when the envelope joint has no SKELETON / SKELETON_ROOT flag (Ray 01's J1), else in that joint's space | how HSD and HSDRawViewer's exporter read them (found when the first build's arms came out twisted) |
| UVs | TEXCOORD_0 with V flipped back (the glTF reader flips it), written to TEX0 and TEX1 (one buffer, as Ray 01's) | the template material reads its specular TObj through TEX1 |
| New textured materials | below ("How the game associates textures with meshes") | retail scheme |
| New flat materials | the template's first MObj's render mode without textures (constant + diffuse + specular), diffuse = Base Color, ambient = 70% of it | step 70; lit like the robo |
| Culling | glTF doubleSided: none; single-sided: CULLFRONT with the winding reversed like HSDRawViewer's importer | as the game's bodies; thin parts show from both sides |
| Joint flags | HSDLib's `UpdateFlags` result only on joints whose DObjs changed (OPA, LIGHTING, SPECULAR, ENVELOPE_MODEL; cleared when none is left), SKELETON on enveloped joints, the ROOT_* bits added; every other bit kept | a joint without OPA does not draw its DObj |
| Block B joint | must still have a DObj with a material (error otherwise) | the game reads `jobj->dobj->mobj` of that joint |

`HSDRawViewer --info <file.dat>` prints the joint tree (flags, SRT, inverse bind, robjs), every DObj, MObj and TObj
(with their raw fields, pointers shown as `pppppppp`), every PObj (attributes, envelopes) and how far each inverse
bind is from the joint's SRT world (Ray 01: 0 for the hips and torso, 0.159 for the legs and 0.212 for the head and
arms: the IK effectors' offsets, so the bind pose is the inverse binds', not the SRTs').

Checks: export -> `--import` and export -> Blender `roundtrip.py` -> `--import` of chara_000 give the template .dat
byte for byte with every option (`--replace`, `--skeleton glb`, both; also chara_005 and chara_020); the rebuilt
entries keep their joint tree and roles (`robo_joints.py roles` equal) unless a roles file says otherwise.

### How the game associates textures with meshes

Taken from Ray 01 (chara_000) with `HSDRawViewer --info`; every robo body seen follows it. The new materials of
`--import` copy it field for field (checked: the MObj, material and both TObj raw dumps of the legionnaire equal Ray
01's except the image pointers and sizes).

- **Mesh -> material**: each DObj (one per mesh part, on the joint that carries it) points at an **MObj**. Ray 01's
  18 DObjs all share **one** MObj; the importer gives each glb material one MObj, shared by its DObjs. Block B points
  at the joint whose first DObj's MObj the game keeps (JOBJ_1).
- **MObj**: render mode `CONSTANT | DIFFUSE | SPECULAR | TEX0 | TEX1 | ALPHA_MAT` (0x203D); material colours ambient
  179,179,179, diffuse 255,255,255, specular 0,0,0, alpha 1.0, shininess 20. No PE description.
- **TObj chain (2 stages)**:
  1. diffuse: flags `LIGHTMAP_DIFFUSE | COLORMAP_BLEND`, blending 1.0 (the image replaces the diffuse colour), texture
     coordinates `GX_TG_TEX0`, wrap CLAMP / CLAMP, repeat 1 / 1, scale 1, magnification linear, no LOD, no TEV,
     image **CMP** (GX format 14, `0x0e`), no palette, no mipmaps. Ray 01: one 512x512 atlas for the whole body.
  2. specular: flags `LIGHTMAP_SPECULAR | COLORMAP_BLEND`, blending 0.8, coordinates `GX_TG_TEX1`, otherwise the same;
     a grey CMP mask at a quarter of the diffuse size (Ray 01: 128x128) laid out like the diffuse atlas. The importer
     makes it at half size from the base image's luminance x (0.25 + 0.75 x Metallic).
- **PObj**: attributes POS, NRM (int16 indexed), TEX0 and TEX1 pointing at the **same** UV buffer (uint16 indexed),
  plus PNMTXIDX for envelope (skinned) PObjs; flags CULLFRONT (and ENVELOPE for skinned ones).
- **Colours (entries 5N..5N+4)** are separate model entries, each with its own textures (no palette swap; the
  community parts expansion and Project Hive do not change textures by code either: the parts-expansion research note
  in docs/community on main). A recolour = new images in each entry's TObjs.

| legionnaire_v2 material | image | specular mask | culling | DObjs |
|---|---|---|---|---|
| Iron (lorica, shoulder lames, boss plate, blade) | 128x128 CMP | 64x64 | CULLFRONT | torso, upper arms, scutum, gladius |
| Bronze (helmet, greaves, knee caps) | 128x128 CMP | 64x64 | CULLFRONT | head, shins |
| Gold (brow band, crest holder, buckles, studs, rims, boss, guard, pommel) | 64x64 CMP | 32x32 | CULLFRONT | 7 joints |
| Red (crest, scarf, tunic, skirt, sleeves) | 128x128 CMP | 64x64 | none | crest, envelope, hips, upper body |
| Leather (belt, apron, pteruges, bracers, sandals, grip) | 128x128 CMP | 64x64 | none | envelope, hips, forearms, feet, shins |
| Skin (head, neck, arms, legs, fists, toes; face 3x texels) | 256x256 CMP | 128x128 | CULLFRONT | envelope, head, hands, toes |
| Scutum (shield field, wings, lightning, back) | 128x128 CMP | 64x64 | CULLFRONT | Scutum bone |

### Known limits (what is left)

- **Not seen in game yet** (Customize screen, battle): the custom skeleton (58 joints, 3 new bones), the smooth
  skinning and the hand / foot placement on the IK effectors are built as the game data describes them, but only a
  40 s boot is automatic. If the custom skeleton misbehaves, `ray_legionnaire_tskel` is the same model on Ray 01's
  exact skeleton.
- Joint rotations of `JOBJ_n` bones always come from the template (Blender re-orients bones); moving a bone moves
  only its translation. New bones are axis-aligned. Changing IK chain lengths is possible but block A (limb
  measurements) is not recomputed (`A_from` copies another robo's).
- No vertex colours, no new texture formats (the template's CMP), no mipmaps, no animations, no shape keys.
- A new texture is always the template MObj's two-stage scheme; other TEV setups need code.
- Size: body entries must stay under 0x3A000 bytes; memory / vertex limits beyond that are unknown (the legionnaire:
  3,756 triangles, 58 joints, 44 DObjs, 54 PObjs, boots).
- New bones need the roles file; `make_mod.py` refuses a changed skeleton without one.
- A grown archive moves to unused disc space (`iso_patch.py build`; Custom Robo's files sit at the end of the disc),
  so a size change no longer needs room after the last file.

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
writes each changed file over the clean ISO (in place if it fits, else moved to unused disc space, file table updated) →
`tools/iso_patch.py diff` stores the difference: blocks found in the clean ISO (also when moved) become
references, the rest the changed bytes only.
Tested: robo 0 recolour → 43 KB of new bytes in the patch; apply on a clean ISO gives the same SHA-1;
the ISO boots in Dolphin.
