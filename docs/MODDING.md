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
(story characters), `bg_data` (stages: [Stages](#stages)), `leg_effect`, `snr_obj`, and the effect containers
`gun_effect`, `bomb_effect`, `pod_effect`, `attack_effect` ([Effects](#effects-projectiles)).

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
vs legionnaire_v2_reimport.png) and boot (40 s, no warning). **Seen in game (Oct 31, 2026,
docs/images/ray_legionnaire_v2_customize.png, _battle1-3.png)**: the custom 58-joint skeleton works in Customize and in
a battle (pose, animation, attached gun and pods, the crest and scutum move), but the game draws a body with ONE
material, so the seven materials came out as one flat tan ("How the game associates textures with meshes" below). The
one-atlas build of step 74 (`pack_atlas` in `legionnaire_v2.py`: one 512x256 CMP atlas, two materials) builds and boots;
**its look in game is the open test** (plan.md "Waiting on the user").

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
- Size: the body entry must stay under 0x3A000 bytes (the game's per-robo load buffer). legionnaire_v2: 3,754
  triangles + one 512x256 atlas (64 KB + 16 KB specular mask) = 213,839 bytes (0x3434F). Images cost most: a 512x512
  CMP is 128 KB (+ 32 KB mask) and does not fit beside this many triangles.

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

- **One material per body - a game rule, not a convention** (found Oct 31, 2026 from the legionnaire v2 photos: every
  mesh drawn in one flat tan). The robo draw loop `fn_800BCC60` calls `HSD_MObjSetup` **once** with the block B
  joint's material (`cr_Chara.material` = that joint's first DObj's MObj, kept by `cr_Chara_BuildModel`) and then
  walks the joints with its own display function (`fn_80095E6C`, flag 0x04000000), which draws every DObj's PObjs
  without setting up their materials. So a body shows **one** texture, block B's, through each mesh's own UVs; a
  mesh on another material samples the wrong image. That is why every retail body has one atlas and one MObj. For
  a new body: bake everything into one atlas (legionnaire_v2.py `pack_atlas`: 512x256, the biggest that fits the
  0x3A000 entry with ~3,800 triangles; 512x512 does not) and give every part the same material, or two materials on
  the same image when some parts must be two-sided (the importer shares the encoded images; culling is per DObj).
  The importer warns when a body carries more than one distinct texture. Build numbers 2, 0xE, 0xF and 0x10 are
  drawn with `HSD_JObjDispAll` instead (their own materials).
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
     makes it at half size from the base image's luminance x (0.25 + 0.75 x metallic), metallic per pixel from the
     glTF metallicRoughness texture's blue channel when the material has one (the atlas's companion image in
     legionnaire_v2.py), else the material's Metallic factor.
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

### Stages

A stage (arena) is two `bg_data.BIN` entries: **stage id s = entry 2s, the model** (a standard HSD .dat whose root
`scene_data` holds up to 20 models, one camera, the lights and the fog) **and entry 2s+1, the collision** (one group
of quads per model). The ids, names, menu order and formats: [stage_tables.md](stage_tables.md) (plan.md step 75).
Basic Arena is id 0: `bg_data_000.dat` + entry 001.

```powershell
pwsh tools\model_loop.ps1 -Mod basic_arena_pillar -Script stage_pillar -Category bg_data -Entries 000 -Boot
pwsh tools\model_loop.ps1 -Mod rt_stage -Script roundtrip -Category bg_data -Entries 000,008,026   # no edit: tests the loop
```

```bash
tools/model_loop.sh basic_arena_pillar stage_pillar 000 --category bg_data --boot
tools/model_loop.sh rt_stage roundtrip 000,008,026 --category bg_data
```

- **Export**: `HSDRawViewer --export` writes one glb per scene model, `scene_data_<n>.glb` (bones `JOBJ_n`, meshes
  `Joint_<j>_Object_<d>`, as for robos). The loop runs the Blender script once per model (it gets
  `scene_data_<n>.glb` as `<in.glb>`; a script that edits one model saves the others unchanged) and chains the
  imports with `--model <n>`. `-Models 0,3` (`--models 0,3`) limits the loop to those models.
- **Import** (`HSDRawViewer --import <template.dat> <edited.glb> <out.dat> --model N`): the same rules as for a robo
  (kept meshes byte-identical, new rigid / skinned geometry, `--replace`), without the robo-only ones (block B, one
  atlas), plus: a new mesh on one of the stage's **own materials** (the exporter's name, e.g. `Material_6F9003D5`)
  shares that material as it is (same texture, same render mode; nothing re-encoded), and when that material colours
  from the vertices (`VERTEX`, as Basic Arena's floor) the new mesh gets vertex colours (the glb's `COLOR_0`, else
  white; export them with `common.save(path, export_vertex_color='ACTIVE')`). Joint flags of a scene only gain the
  OPA / XLU / LIGHTING / SPECULAR bits a new mesh needs.
- **File layout**: a stage .dat is written back in its **own layout**: every unchanged struct stays at its offset, new
  structs are appended after the data, the relocation table and root are rebuilt. HSDLib's normal save lays the whole
  file out again (and is not byte-identical for the game's stage files), so this matters for small patches.
- `make_mod.py` puts `models/bg_data/bg_data_NNN.dat` into `bg_data.BIN` as it is (an odd NNN, a collision entry,
  too). `bg_data.BIN` has no entry table copy in main.dol (the game reads the archive's own header at boot), so only
  the archive changes.
- Basic Arena's ISO (below): `build/mods/basic_arena_pillar/basic_arena_pillar.iso`, SHA-1 4b52d6a6...; the patch
  `mods/basic_arena_pillar/basic_arena_pillar.crpatch` (29 KB).
- `HSDRawViewer --info bg_data_000.dat` lists the scene (models, cameras, lights, fog) and every model's joints,
  materials, textures and PObjs.

Tested (Oct 31, 2026): export -> Blender `roundtrip.py` -> `--import` of every model gives the template .dat byte for
byte for all 40 stages (every scene model through Blender; the ISO of a round trip has the clean disc's SHA-1; two
stages, No-Man's Land 054 and Lost World 070, needed an exporter fix first: a zero-length normal became NaN, which
glTF refuses, so the patched exporter writes +Y for it); the `basic_arena_pillar` ISO (`tools/blender/stage_pillar.py`: a golden
1.2 x 1.2 x 2.6 pillar on Basic Arena's floor at x 4.5, z 0, with the floor's own material and texture) builds
(the model entry grows 0x96100 -> 0x96400 bytes; in the data block one pointer changes, the rest is appended; re-export: docs/images/basic_arena_pillar_reexport.png, vertex colours not shown) and boots (40 s, no warning). **Not yet seen in game** (plan.md "Waiting on the user").

**Limits** (stage_tables.md "Limits for a modded stage"):

- Model entry (2s) at most **0x100000 bytes** (its load region; `make_mod.py` refuses more); the largest retail one
  is 0xEC220. Collision entry (2s+1) at most **0x8000 bytes**.
- At most 20 models per scene; at most 21 DObjs on the root joint of models 1 and up (the game hangs otherwise).
- **Collision for new geometry**: the collision is entry 2s+1 (quads with plane equations per scene model); the
  model loop does not touch it. `python -I tools/stage_collision.py export S` writes it as an OBJ (one object per
  group), add the new faces in Blender (normals toward where a robo stands: an obstacle's faces out), then
  `stage_collision.py import S edited.obj --mod MYMOD`; unchanged quads keep their bytes (all 39 retail entries
  round-trip byte for byte). Formats, the breakable / moving model flags and the rules:
  [SCENE_GUIDE.md](SCENE_GUIDE.md#5-stages-verified-fn_80072064-fn_80072b90-fn_80070170-fn_80072a58).
- Meshes on scaled joints (Dead Line model 0: joints at scale 0.667; Battle Gear Station model 11) come back from
  Blender with another size: they are kept as they were (the import warns and ignores the difference), but do not
  edit their geometry.
- Animations, cameras, lights and fog are kept as they are; the loop does not edit them. Spawn points, the battle
  camera and the meaning of the collision attributes are not located yet.
- `snr_map` (story maps: collision + model in one entry) is not in the loop yet.

### Effects (projectiles)

`gun_effect.BIN`, `bomb_effect.BIN`, `pod_effect.BIN` and `attack_effect.BIN` hold the effect models (shots, blasts).
Every entry is a small wrapper around **two standard HSD .dat files**, each with the root `scene_data`, the same
scene format as a stage (models, a camera, lights, fog; joint and material animations): a u32 count (2), count+1
offsets (the last is the end), zero padding to 0x20, then each .dat zero-padded to 0x20. `extract_models.py` writes
them as `<container>_NNN_0.dat` and `<container>_NNN_1.dat`; `make_mod.py` puts an edited piece back into entry NNN
and rebuilds the wrapper (byte for byte for all 207 clean entries) and main.dol's entry table copy.

```powershell
pwsh tools\model_loop.ps1 -Mod rt_gun_effect -Script roundtrip -Category gun_effect -Entries 000_0   # no edit
```

The [Stages](#stages) notes on export, import and the file layout apply as they are (one glb per scene model).
Tested (Nov 6, 2026): the no-edit loop above (export -> Blender `roundtrip.py` -> `--import` of all four scene
models -> `make_mod.py`) and `make_mod.py` with unchanged pieces of gun_effect 000 / 012 and pod_effect 020 give
the clean archives byte for byte (the ISO reports 0 files changed). A test edit:
`pwsh tools\model_loop.ps1 -Mod gun0_effect_red -Script effect_tint -Category gun_effect -Entries 000_0`
(`tools/blender/effect_tint.py`: every material x (1, 0.25, 0.25), a Multiply node behind a texture; the import
changes the diffuse colour of models 0-2, the entry keeps its size) builds `build/mods/gun0_effect_red/gun0_effect_red.iso`
(SHA-1 079972ca...) and boots (40 s, no warning); not checked in game. Model 0's texture replaces its diffuse
colour (COLORMAP_BLEND, blend 1), so a tint can only show on models 1 and 2 (MODULATE).

**Which entry**: fn_80075950 loads, for each of the four battle robos, gun_effect / bomb_effect / pod_effect /
leg_effect entry = its gun / bomb / pod / leg number and attack_effect entry = its **body** number (34 bodies, 34
entries), so gun_effect_012 is the effect of gun 12 (the same number as weapon_gun_012). **Size limits** (each robo's
load buffer, `make_mod.py` refuses more): gun_effect 0x16000, bomb_effect and pod_effect 0x40000, attack_effect
0x14000, leg_effect 0x1000 bytes per entry (largest clean entries: 0x146E0, 0x218E0, 0x220E0, 0x13B80, 0x3C0).
Open: what the second piece of each entry is.

## Motions (animations)

**Quick route by body and slot** (victory, charge, get-up, ...): `python -I tools/anim.py slots --body N` lists the
38 slots; `anim.py export --body N --slot victory2` writes a glTF for Blender; `anim.py import --body N --slot victory2
my.glb --mod MYMOD` writes `mods/MYMOD/files/motion.DBN` (stretched to the game's length); then `make_mod.py`. Every
format and rule in one page: [ANIMATION_GUIDE.md](ANIMATION_GUIDE.md).

A motion lives in entry 8 of `logo_read_us.dbn` (battle and field motions, 3 groups) or a piece of `snr_motion.BIN`
(story scenes). **The loose `motion.DBN` on the disc is never read by the game** (found Nov 2, 2026; the full path:
[formats/motion.md](formats/motion.md#the-full-path-disc-to-screen-and-mod-to-iso)). A mod still gives its edited
motion file as `files/motion.DBN`; `make_mod.py` puts it into `logo_read_us.dbn` as entry 8. Read the motions to edit
from the bundle: every `crmotion.py` / `crmotion_rig.py` command that takes a motion file takes
`"Custom Robo extracted/files/logo_read_us.dbn"` too (`crmotion.py boot-extract` writes entry 8 alone). Every motion plays on every robo: the game rebuilds the body from the
motion's IK points and joint angles with that robo's own limb lengths
([formats/motion_playback.md](formats/motion_playback.md)). So one edited run changes the run of every robo that uses
that motion. Motion names come from the game (`python -I tools/crmotion.py list "Custom Robo extracted/files/logo_read_us.dbn"`):
`sf_` / `mg_` / `ab_` are the three body families; `*_normal_run*` are the runs, `*_weapon_*` the gun stance and shots,
`win_*` / `lose_*` the end poses.

### Three ways to edit, simplest first

1. **By numbers (JSON)**: `crmotion.py export motion.DBN m.json`, change key values (each track has a `role` such as
   `point3.y` = the left hand target's height or `joint6.rotz` = an arm joint in degrees), `crmotion.py import m.json
   files/motion.DBN`. Unchanged motions stay byte-identical.
2. **By script (as the Atlas mod does)**: a small Python script that samples a motion per frame
   (`crmotion.sample`), computes new channel values and rebuilds those tracks with `crmotion_rig.reduce_keys`.
   `mods/atlas_ninja_run/make_motion.py` is the example: the IK hand targets placed behind the hips and the chest
   target tipped forward give the ninja run. The gun motions turned around (root rotation y + 180), bent over 85
   degrees, with the gun hand at the butt, give the "butt shot".
3. **In Blender (glTF)**: `python -I tools/crmotion_rig.py gltf motion.DBN <ID> unpacked/chara/000.bin
   models_export/chara/chara_000/root0_joint.glb out.glb` gives the robo with the motion as a glTF animation. Edit it
   in Blender: keyframe the bones that have a role, and export glTF with the animation and the same bone names
   (JOBJ_n). Then `python -I tools/crmotion_rig.py import-gltf edited.glb <animation name or index>
   unpacked/chara/000.bin motion.DBN <ID> files/motion.DBN` solves the game's rig backwards into that motion slot
   (`--no-template` for a new animation; `--tolerance DEG`). `tools/pyenv/Scripts/python -I tools/render_motion.py
   out.glb out.png` renders frames to check. Unity reads and writes the same glTF files if you prefer it; Blender is
   what the model pipeline already uses.

Rig facts found while making the Atlas motions (Nov 2, 2026; docs/formats/motion_playback.md "Authoring notes"):
the `*_r180` motions face +Z in the points' frame (root rotation y 180), the others -Z; a leg or arm IK target is
read on the game's default skeleton, so a straight leg needs its target about 0.9 from the hip joint (0.08 below
point 0), not the robo's own leg length; the gun barrel is the gun hand's X axis, pitched by `joint14.rotz`.

What the rig can show: role joints take their rotation from the animation and their position from the robo's limb
lengths. An elbow or knee bends only one way, about its own axis, and a hand or foot is reached by two-bone IK. Moving
a bone away from its limb's length does nothing in the game.

**Size limit**: a group's float pool holds at most 16,384 floats. Group 0 of motion.DBN (the sf_ / mg_ / ab_ battle
motions) has about 2,300 free. A dense imported motion takes 1,000-2,400. A script that rebuilds tracks from smooth
values fits many motions; the Atlas mod rebuilds 29 at 14,649 floats.

### The first in-game test (Nov 2, 2026; waiting for the owner)

The 21:18 test showed stock poses: that build edited only the loose `motion.DBN`, which the game does not read. Rebuilt
the same evening with the motions in `logo_read_us.dbn`. **The later battles (Nov 2, 22:31 to 23:30): the edited
motions play.** The march with the flailing arm and the hip-thrust air dash work; the butt shot on the gun motions
breaks the battle (black 3D screen, a robo vanishing), and a build with the stock gun motions plays fine. Three builds
to tell why are waiting for a test: mods/atlas_ninja_run/notes.txt. On the way, two rules for any motion edit: every
track must end on the motion's last frame (crmotion.build enforces it), and root / joint 0 tracks are best left alone.

`build/mods/atlas_ninja_run/atlas_ninja_run.iso`. Rebuild it with `python -I mods/atlas_ninja_run/make_motion.py`
(arguments: the lean in degrees, the position tolerance) and then `python -I tools/make_mod.py mods/atlas_ninja_run`.
It is Atlas (the legionnaire body on robo 0). Since Nov 2 evening (the ninja run dropped: the game aims the gun arm
and keeps the robos facing each other): a Roman march with the off arm flailing on the 8 walk / run motions of robo
0's motion set, a groin-first hip-thrust air dash, and the butt shot on all fours on the 6 sf_weapon_* motions
(mods/atlas_ninja_run/notes.txt). In a battle, check: (1) walking / running shows the high straight-leg kicks and the
windmilling off arm; (2) the air dash (jump, then dash) flies hips first with the arms back; (3) shooting shows the
bent-over stance with the butt to the target, and where the bullets come from; (4) nothing crashes and the other
motions look as before.

**If it does not work**, in this order:

0. **Glitches, a robo vanishing, a black screen**: a track that ends before its motion does (the game reads past
   it). `crmotion.build` now refuses such a file; if you write motion bytes another way, keep every non-empty track's
   last key on the motion's last frame (docs/formats/motion_playback.md).
0. **Nothing changes in the game, first check**: the ISO's `logo_read_us.dbn` must hold the edit (`make_mod.py`
   prints `motion.DBN -> logo_read_us.dbn entry 8`). An edit only in `motion.DBN` on the disc never plays.
1. **Nothing changes in the game**: the game plays other IDs than the ones edited. Read the robo's motion set
   (cr_g_BattleMotionSets, docs/robo_tables.md) and edit the IDs it names. A quicker check: edit one motion
   drastically (all poses 90 degrees off) and see if anything moves.
2. **A crash or a frozen robo**: build the file through `crmotion.py roundtrip` first (it must be byte-identical
   unedited), then add one edited motion at a time to find the one that breaks.
3. **The pose shows but looks wrong**: author it in Blender instead (way 3 above). You get a direct view of the
   bones, and `import-gltf` keeps what the rig can show.
4. **The bullet still leaves from the chest**: the game spawns shots from the robo, not from the gun model. Then the
   spawn point is in code: find the function that creates the bullet (the users of the gun part's fire data in
   docs/robo_tables.md) and patch its offset (`code/*.txt`, section 4).

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

The patch route above changes bytes in place, so anything new must fit where the old thing was (or use Gecko / free space). The Phase 4 build (batch 9, [REPRODUCE.md](REPRODUCE.md) section 2b) rebuilds `main.dol` from split objects, and since plan.md step 51 it is **shiftable**: every pointer in code and data is a relocation (dtk's mistakes are fixed by `tools/gen_dtk_relocs.py`), so code and data can move and grow and the linker updates every reference. The static test (`tools/shift_test.py`, batch 9) adds 0x100 bytes at 30 or more points in code and data (35 at step 86: the start of every data-only game unit outside a glue span is one) (`decomp/reports/shift_report.txt`), and batch 11 (Windows) boots that build to the title screen.

What this enables: a bigger table (a new robo slot in the motion / model / name tables) or new code is an edit of the split asm or the C plus a relink, with no free-space hacks.

Still missing before it is a normal workflow (Oct 31, 2026, plan.md step 83): a battle tested by hand in the shifted build ([REPRODUCE.md](REPRODUCE.md) section 2c); the game's own data is still almost all in dtk's automatic objects (every library object and the 4 game file objects have proven data pieces; 5.90% of the data is built from C), so growing a game table is only proven safe at a file boundary (where the code stops addressing data from one base plus offsets); no mod tool drives the build yet (make_mod.py still patches bytes); and most game code is still asm: 646 of the 922 units are built from C, 22.18% of the code (every non-HSD library complete, HSD 22 of 30 objects), of the game code 443 objects, 4.25% (`decomp/reports/progress_report.txt`).

## How it works (for the curious)

`tools/make_mod.py` → models go back into their archive (`crmodel.from_dat` with the original entry as the
template, then `sfd.build`), files are copied, code lines patch a copy of `main.dol` → `tools/iso_patch.py build`
writes each changed file over the clean ISO (in place if it fits, else moved to unused disc space, file table updated) →
`tools/iso_patch.py diff` stores the difference: blocks found in the clean ISO (also when moved) become
references, the rest the changed bytes only.
Tested: robo 0 recolour → 43 KB of new bytes in the patch; apply on a clean ISO gives the same SHA-1;
the ISO boots in Dolphin.
