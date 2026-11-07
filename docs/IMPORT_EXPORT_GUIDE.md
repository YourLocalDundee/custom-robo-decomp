# Import / export guide: robos, parts, stages, effects

A reading guide for anyone writing a model exporter / importer for Custom Robo (GameCube, US) or making model mods.
It goes from the disc down to the vertices, in the order you meet things, and says for each step what is
**verified** (read from the game's own code or checked over every file) and what is still **open**. The deep
references are linked at each step; you do not need them to follow this page.

Short version:

1. Every model is HAL's **sysdolphin (HSD)** format, the same as Super Smash Bros. Melee. Stages and effects are plain
   HSD `.dat` files; robos and parts are HSD data behind a 0x40-byte header (`tools/crmodel.py` converts both ways).
2. To place vertices right you need three things a quick parser often skips: **each joint's world matrix**, the
   **fixed-point scale (`frac`) of every vertex attribute**, and, for the transforming bodies, **their own joint
   animation at frame 0**.
3. A robo body draws with **one material** (one texture atlas); parts are rigid and modelled at **10x** the body's scale.
4. Stages are model + collision as two archive entries; effects are two `.dat` files per entry.
5. The working round trip (export -> Blender -> import -> playable ISO + shareable patch) exists for robos, parts,
   stages and effects: [MODDING.md](MODDING.md).

---

## 1. Where the models are on the disc

All models are in SFD archives (`.BIN`; container format: [formats/sfd.md](formats/sfd.md)). One entry = one model
file. The one-time build (`tools/from_scratch.ps1` / `.sh`, [README](../README.md#quick-start)) unpacks them into
`unpacked/` and converts every model to a standard HSD `.dat` in `models_dat/` (1,006 models) plus glTF in
`models_export/`.

| What | Archive | Entry | Format |
|---|---|---|---|
| Robo bodies | `chara.BIN` | robo (body) N = entries **5N .. 5N+4** (the game loads 5N; the other four are most likely the colour variants, each with its own textures: not verified) | model entry, "abs" variant |
| Guns / bombs / pods / legs | `weapon_gun` / `_bom` / `_pod` / `_leg.BIN` | part number | model entry, "rel" variant (several root joints) |
| Stages (arenas) | `bg_data.BIN` | stage id s: entry **2s = model**, **2s+1 = collision** | standard HSD `.dat`, root `scene_data`; collision: own format |
| Shots, blasts, attacks | `gun_effect` / `bomb_effect` / `pod_effect` / `attack_effect.BIN` | gun / bomb / pod number; attack_effect = **body** number | small wrapper around **two** standard `.dat` files, each root `scene_data` |
| Leg effects | `leg_effect.BIN` | leg number | standard `.dat`, root `scene_data` |
| Story characters / objects / maps | `scenario_us/snr_chara`, `snr_obj`, `snr_map` | | model entry / `.dat` / map (model + collision in one entry) |

Every archive table and which code reads it: [formats/containers.md](formats/containers.md). Stage ids, names and sizes:
[stage_tables.md](stage_tables.md). Body / part numbers and names: [robo_tables.md](robo_tables.md).

**Gotcha (verified):** the game keeps its own copy of most archives' entry tables inside main.dol (chara, weapon_*,
*_effect, snr_*). A mod that changes an entry's size must update that copy too or the game reports a disc read
error. `tools/make_mod.py` does it. `bg_data.BIN` has no copy (the game reads the archive header).

## 2. The HSD structures you will walk

Same structures as Melee (HSDLib's `HSDRaw/Common` layouts are right for this game). The path from a model's root to
its triangles:

```
JObj (joint)      +0x08 child, +0x0C next, +0x10 DObj, +0x14 rotation xyz, +0x20 scale xyz, +0x2C translation xyz,
                  +0x38 inverse bind matrix
 └ DObj (mesh)    +0x04 next, +0x08 MObj, +0x0C PObj
    ├ MObj        +0x04 render mode, +0x08 TObj (texture chain), +0x0C material colours
    │  └ TObj     +0x04 next, +0x0C coordinate source, +0x10 rot / +0x1C scale / +0x28 translation (texture
    │             matrix), +0x34 / +0x38 wrap S / T, +0x3C / +0x3D repeat S / T, +0x4C image
    │     └ Image +0x00 data, +0x04 width, +0x06 height, +0x08 GX format (CMPR = 14)
    └ PObj        +0x08 vertex attribute list, +0x10 display list, +0x14 envelopes (skin weights)
```

`tools/hsd_tex_scan.py` and `tools/hsd_vtx_scan.py` (both read-only, standard library only) print the texture and
vertex-format fields of any `models_dat/` file or folder: a quick way to check your parser against the data.

## 3. Vertices: three things that make or break the shape

### 3a. Joint world matrices (rigid meshes)

A PObj **without** envelopes is rigid: its vertices are in the **local space of the joint that owns its DObj**. Build
each joint's world matrix (parent first; HSD order scale, rotation, translation) and transform the vertices by it.
Every part (gun, bomb, pod, legs) PObj is rigid. Skipping this gives the classic "pieces piled near the origin"
result for the multi-joint roots while the one-joint roots look fine (verified over the part files:
[model_entry.md, "Part files"](formats/model_entry.md#part-files-root-joints-draw-groups-which-ones-the-game-uses-and-why-a-part-can-come-out-garbled-nov-5-2026)).

Skinned PObjs (envelope flag; robo bodies) carry PNMTXIDX plus up to 4 weights per vertex (0.1 steps); their
vertices are in the bind pose. Single-weight vertices of an envelope are in the bind pose when the envelope joint
has no SKELETON / SKELETON_ROOT flag, else in that joint's space ([MODDING.md, import table](MODDING.md#what-hsdrawviewer---import-keeps-and-changes)).

### 3b. Fixed-point vertex formats: read `frac` per attribute

GameCube vertex components are often integers with a fixed-point shift: **value = raw / 2^frac**, with the component
type (u8 / s8 / u16 / s16 / f32) and `frac` stored in each attribute descriptor (0x18 bytes: +0x0C component type,
+0x10 frac). **They differ between files and between PObjs of one file.** Counted over every model
(`tools/hsd_vtx_scan.py`, Nov 6, 2026):

| Models | UV (TEX0) formats seen | Positions | Normals |
|---|---|---|---|
| Robo bodies (170) | **u16 frac 15** (most), **u16 frac 14** (bodies 12 Javelin, 13 Glaive), **s16 frac 14** (body 24 Ray Legend) | s16 frac 11 / 12 / 13 | s16 frac 14 |
| Parts | f32; u16 frac 15; **u16 frac 14** (legs 2, 10); **s16 frac 14** (pods 16, 17, 25; legs 11, 13, 14) | f32 or s16 frac 10..13 | f32 or s16 frac 14 |
| Stages | f32 everywhere | f32 | f32 |
| Effects | u16 frac 14 / 15, **u8 frac 7** | s16 frac 10..14 | s16 frac 14 |

A parser that assumes one scale (say /32768 for every u16 UV) puts the frac-14 UVs at half size, and one that reads
s16 as u16 wraps negative coordinates: both look like a texture that is "offset" or misaligned on some robos only.
Read the descriptor of each attribute of each PObj.

**TEX0 and TEX1 point at the same UV buffer** on robos (the second texture stage, the specular mask, reuses the UVs).

### 3c. Transforming bodies: apply their own animation at frame 0

Four bodies carry a joint animation of their own (body 18 Defender, 19 Seeker, **20 Breaker**: 10 frames; body 28
Ruhiel: 9 frames). Their **bind pose is not what the game draws**: every frame the game sets that animation to a
frame (`fn_80039FAC` -> `fn_80095700`, the body joint at battle robo +0x860):

- **frame 0** normally;
- during a motion whose id has bit 0x8000 set (the transform moves; the flag is kept at battle robo +0x85B) the value
  at battle robo +0x550 instead: the action code steps it by 1 up to 10 and then sets it to 9.9 (or copies +0x548
  into it).

So the battle form = the body animation at **frame 0**; the in-between frames are the transformation. For Breaker the
bind pose is the other form (the community's "the bind pose is his airship form" fits this). Exporter rule: if the
model's JOBJDesc has a joint animation (record +0x04), apply it at frame 0 before baking positions. Which bodies,
from the files (verified); which motion ids carry 0x8000: not listed yet.

The other 30 bodies have no body animation: the game poses them by writing the motion system's bone matrices into
the 19 role joints each frame ([formats/motion_playback.md](formats/motion_playback.md)), so their bind pose is
their rest pose.

## 4. Textures and materials

- **Format**: robo, part and almost every stage texture is **CMPR** (GX format 14): the image is cut into 8x8 tiles,
  each made of four 4x4 DXT1-like blocks (top-left, top-right, bottom-left, bottom-right), colours RGB565
  big-endian, 2-bit indices most significant bit first. **Alpha**: when colour 0 <= colour 1 (as u16), index 3 is
  fully transparent, so CMPR carries 1-bit alpha; a decoder that drops it loses the cut-outs. (Standard GX; a few
  stage textures are RGB5A3.)
- **Robo bodies: one material per body, a game rule** (verified in the draw code `fn_800BCC60`): the game sets up
  only the material of the block B joint's first mesh and then draws every mesh of the body with it. So every body
  is one atlas; a mesh with its own material in a new body still shows the atlas through its UVs. Material: render
  mode 0x203D, two texture stages: diffuse CMPR (TEX0, blend 1.0: the image replaces the diffuse colour) and a
  quarter-size grey CMPR specular mask (TEX1, blend 0.8, same UVs). Details:
  [MODDING.md, "How the game associates textures with meshes"](MODDING.md#how-the-game-associates-textures-with-meshes).
- **Robos and parts have no texture transforms**: every one of their 13,518 TObjs has identity scale / rotation /
  translation, repeat 1 / 1 and CLAMP wrap (`tools/hsd_tex_scan.py --odd` prints none). Misplaced robo textures
  therefore come from the UV format (3b), not from a texture matrix.
- **Stages do use them**: 291 of 1,414 stage TObjs tile, mostly the second / third stage (TEX1 / TEX2: detail and
  environment layers) with REPEAT wrap and repeat 8x8, 16x2, 24x24 or 40x40 (and a few TEX0 like 40x6). Multiply the
  UVs by the repeat counts (and the TObj scale) for those layers. Some stage materials take their coordinates from
  the normals (`COORD_REFLECTION` environment maps) instead of a UV set. Stage materials also use vertex colours
  (`VERTEX` render mode, CLR0) and translucency (XLU, no Z update).
- **Colour variants** (most likely entries 5N+1..5N+4) are separate entries with their own images (no palette swap).

## 5. Robo bodies: the skeleton the game expects

- One root joint, 51..69 joints. The game finds **19 role joints** by paths stored after the HSD data (block C: upper
  body, head, two 4-joint arms, lower body, two 4-joint legs); the motion code drives those. Block B names the joint
  whose material is the body's material; block A holds 32 limb measurements. Table and tools:
  [formats/model_entry.md](formats/model_entry.md#trailer-blocks).
- Hands and feet are IK effectors: their bind pose sits at odd places (beside the body, feet on their backs); the
  game moves them. Robo 0 faces +Y in Blender (glTF -Z).
- **Size** is in the vertices: no per-robo scale (root scale is overridden at run time). A body entry must stay under
  **0x3A000 bytes** (the per-robo load buffer).
- A new skeleton is fine with a roles file saying which joint plays each role (`tools/robo_joints.py`,
  [MODDING.md](MODDING.md#a-robo-with-a-different-skeleton-more--fewer--other-bones)).

## 6. Parts: draw groups, forms, mounts

A part file has several root joints ("draw groups"). Verified from the loader and the battle code
([model_entry.md, "Part files"](formats/model_entry.md#part-files-root-joints-draw-groups-which-ones-the-game-uses-and-why-a-part-can-come-out-garbled-nov-5-2026)):

| Part | Roots | Form 0 | Form 1 | Shown in battle |
|---|---|---|---|---|
| gun | 2 | root 0 (big, animated) | root 1 | starts form 0; its state value reaches form 1 after 60 frames unless reset |
| bomb | 2 | root 0 | root 1 | root 1 |
| pod | 4 | roots 0 + 1 (left and right pod) | roots 2 + 3 | form 0 mostly |
| legs | 6 | roots 0 + 2 (+ 1, 3) | roots 4 + 5 (+ 1, 3) | roots 4 + 5 with 1 + 3 |

- The Customize screen shows every part in form 0 (the big display models); story mode uses gun 0, bomb 0, pods 0 + 1,
  legs 0..3.
- **Placement**: each part root hangs on a body role joint (gun: arm_b forearm, bomb: arm_a forearm, pods: upper body,
  legs: shins / ankles), then a per-body offset, then **scale 0.1**, then a per-body scale vector
  ([robo_tables.md, "Part mounts per body"](robo_tables.md#part-mounts-per-body-cr_g_bodydata-nov-5-2026)). Parts are
  modelled at 10x the body's scale.
- **Part animations** are poses, not clips: frame = 10 x a 0..1 state value (11 frames, 0..10).
- No mirroring anywhere: left / right pods and legs are separate roots.

## 7. Stages

- **Model** (entry 2s): `scene_data` = up to **20 models**, one camera, lights (up to 10), fog. Each model is a
  joint tree with its first joint / material animation attached at load.
- **Collision** (entry 2s+1, verified layout): u32 group count, offsets to group headers and quad arrays, optional
  section C. Group n belongs to scene model n: header = bounding sphere (x, y, z, r), quad count, model flags. Quad
  (0x44 bytes) = 4 corners, plane normal, plane d, u32 surface attribute (values seen, meaning not mapped). Section C
  looks like a waypoint graph for the computer players (not verified).
- **Limits**: model entry <= 0x100000 bytes, collision <= 0x8000, models 1..n with at most 21 meshes on their root
  joint (the game hangs otherwise).
- **Not located yet**: spawn points, the battle camera, the meaning of the surface attributes.
- Full page: [stage_tables.md](stage_tables.md). Story maps (`snr_map`) hold a collision block and a `scene_data`
  model in one entry.

## 8. Effects (projectiles, blasts)

Each `*_effect.BIN` entry = a u32 count (2), count + 1 offsets, padding to 0x20, then two standard `.dat` files
(`scene_data` roots: models, camera, lights, fog, joint and material animations), each padded to 0x20. Effect entry
= the part number (attack_effect: the body number). Size limits per entry: gun 0x16000, bomb / pod 0x40000, attack
0x14000, leg 0x1000. What the second `.dat` is for: open. [MODDING.md, "Effects"](MODDING.md#effects-projectiles).

## 9. The working round trip

Everything above is wired into one loop (Windows for the HSDRawViewer steps; Blender and the build anywhere):

```
disc ISO --from_scratch--> models_dat/*.dat --HSDRawViewer --export--> .glb --Blender (script)--> .glb
   --HSDRawViewer --import (over the original .dat)--> mods/<mod>/models/<category>/<name>.dat
   --tools/make_mod.py--> playable ISO + data-free .crpatch (players apply it to their own ISO)
```

- `pwsh tools\model_loop.ps1 -Mod <mod> -Script <blender script> -Category <chara|weapon_gun|...|bg_data|gun_effect> -Entries ...`
  runs all of it ([MODDING.md, "Blender loop"](MODDING.md#blender-loop)); the `roundtrip` script (no edit) gives back
  the clean disc byte for byte for robos, all 40 stages and the effect archives (tested).
- **Bring your own tool**: anything that writes a standard HSD `.dat` works. For robos and parts,
  `tools/crmodel.py from-dat` rebuilds the game entry from the `.dat` (keeping blocks A/B/C), and `make_mod.py`
  handles the archive, the main.dol entry-table copy and the size limits. Stage and effect `.dat` files go in as
  they are.
- The importer keeps everything you did not change byte-identical (meshes, materials, joint flags, IK settings), so
  small edits make small patches.

## 10. Who reads what (code map)

| Asset | Loaded by | Built / drawn by |
|---|---|---|
| Robo body | `cr_CharaEntry_OnLoaded` (0x800747B0: relocation) | `cr_Chara_BuildModel` (0x80074110: joints, roles, material), draw `fn_800BCC60` -> `fn_80095E6C` |
| Parts | `cr_WeaponModel_OnLoaded` (0x80073CB8: root records, animations, joint picks) | slots `fn_80007CE4`, forms `fn_80007AE4` / `BD4` / `C24` / `C74`, placement `fn_80039FAC` |
| Body / part animation frame | | `fn_80095700` (set frame on a joint tree), called from `fn_80039FAC` |
| Stage model | `fn_8011449C` (entry 2s) | `fn_80072064` (scene build: models, lights, fog) |
| Stage collision | `fn_8011449C` (entry 2s+1) | `fn_80072A58` (offsets to pointers) |
| Effects | `fn_80075950` (per robo: gun / bomb / pod / leg / attack entries) | |
| Motions | boot bundle `logo_read_us.dbn` entry 8 | [formats/motion_playback.md](formats/motion_playback.md); which motion plays when (victory, charge attack, ...): [battle_animations.md](battle_animations.md) |

Names: `cr_` = this project's names with evidence ([name_evidence.md](name_evidence.md)); `fn_` = not named yet; `HSD_`
= the sysdolphin library's own names.

## 11. Open questions (help welcome)

- Which motion ids carry the transform bit 0x8000 (so which moves show the in-between frames).
- Spawn points, the battle camera, the collision surface attributes and group flags.
- The second `.dat` of each effect entry; record[2] / record[3] of part root records.
- Memory / vertex limits beyond the entry sizes.
- The meaning of each limb measurement (block A) float.
