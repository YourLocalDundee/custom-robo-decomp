# Custom Robo scenes: stages, projectile / blast effects, particles

Everything known about the game's `scene_data` files (stages, the effects of guns, bombs, pods, charge attacks and
legs, the common effects, story objects) and the stage collision, in one page for modders and tool writers. Each
fact is read from the game's code (function addresses given) or counted over every file on the disc (US, GXCE01);
anything not verified says so. Deeper pages: [stage_tables.md](stage_tables.md) (every stage, its record, the
collision table), [formats/containers.md](formats/containers.md) (the archives), [MODDING.md](MODDING.md) ("Stages",
"Effects"), [IMPORT_EXPORT_GUIDE.md](IMPORT_EXPORT_GUIDE.md) (vertices, textures, the import rules).

## 1. Where everything is

| What | File (archive number) | Entry | Loaded into | Size limit |
|---|---|---|---|---|
| Stage model | `bg_data.BIN` (12) | 2s for stage id s | the stage build `fn_80072064` | 0x100000 |
| Stage collision | `bg_data.BIN` (12) | 2s + 1 | `fn_80072A58` | 0x8000 |
| Gun shots | `gun_effect.BIN` (15) | the gun number | per robo, 0x16000 buffer (`fn_80075950`) | 0x16000 |
| Bomb blasts | `bomb_effect.BIN` (16) | the bomb number | per robo, 0x40000 | 0x40000 |
| Pod effects | `pod_effect.BIN` (17) | the pod number | per robo, 0x40000 | 0x40000 |
| Charge attack effects | `attack_effect.BIN` (18) | the **body** number | per robo, 0x14000 | 0x14000 |
| Leg effects (jets) | `leg_effect.BIN` (22) | the leg number (34 entries, 29 legs) | per robo, 0x1000 | 0x1000 |
| Common effects | `efc_common.dbn` (24) | 4 scenes | `fn_80078478` / `fn_80077F38` | - |
| **Particle banks** | `logo_read_us.dbn` (0, the boot bundle) | **entry 0** | at boot, `fn_80075D14` -> `psInitDataBankLoad` | the bundle, 2,842,624 bytes |
| Story maps | `scenario_us/snr_map.BIN` (4) | per map | collision block + scene_data + bounds ([stage_tables.md](stage_tables.md)) | - |
| Story objects | `scenario_us/snr_obj.BIN` (27) | 89 | one model each | - |
| Title model | `TITLE_MODEL.dat` (29) | - | the title screen | - |

The archive numbers are main.dol's file descriptor table `cr_g_FileDescs` (0x801F1200, 43 entries; the full list
is in section 9). The effect archives' entry sizes are also copied in main.dol's entry tables; `make_mod.py` keeps
both in step.

## 2. The HSD .dat file (verified: HSDLib and our reader agree on every file)

All effect, stage and object models are standard sysdolphin `.dat` files, as in Super Smash Bros. Melee (HSDLib /
HSDRawViewer read and write them). Big-endian.

| Offset | What |
|---|---|
| 0x00 | u32 file size |
| 0x04 | u32 data block size D |
| 0x08 | u32 relocation count |
| 0x0C | u32 root count |
| 0x10 | u32 reference (extern) count |
| 0x20 | the data block (D bytes); every pointer in it is an offset from 0x20 |
| 0x20 + D | relocation table: one u32 per pointer word (its data offset) |
| then | roots: (u32 data offset, u32 name offset) each |
| then | references: the same, for symbols resolved from another file |
| then | the name strings (offsets from here) |

Every scene file of this game has one root, **`scene_data`** (the game asks for it by name,
`HSD_ArchiveGetPublicAddress(archive, "scene_data")`).

## 3. `scene_data`: the scene object (HSDLib HSD_SOBJ; verified in the game's loaders)

| Offset | What | Used by the game |
|---|---|---|
| +0x00 | null-terminated array of **model** pointers (JOBJDesc) | always |
| +0x04 | null-terminated array of camera descs | **never** (the battle camera is the game's own) |
| +0x08 | null-terminated array of lights ({LightDesc, animation list}) | stages only (`HSD_LObjLoadDesc`, at most 10) |
| +0x0C | fog ({FogDesc, AObj}) | stages only (`HSD_FogLoadDesc`; its colour becomes the clear colour) |

A **model** (JOBJDesc, 0x10 bytes): root joint, then three null-terminated arrays: joint animations (AnimJoint
trees), material animations (MatAnimJoint trees), shape animations. The game loads the root with `HSD_JObjLoadJoint`
and attaches the **first** animation of each array (`HSD_JObjAddAnimAll`).

The structures below a joint are standard: Joint (0x40: name, flags, child, next, DObj list or particle joint,
rotation, scale, translation, inverse bind matrix, constraints), DObj (mesh: next, MObj, PObj list), MObj
(material: render mode, TObj list, colours, pixel engine), TObj (texture: image, palette, wrap, scale, filtering, a
texture matrix), PObj (vertex attributes and display list; rigid, envelope or shape animation). Details for modellers:
[IMPORT_EXPORT_GUIDE.md](IMPORT_EXPORT_GUIDE.md) sections 3-4.

**Animations** (standard HSD): an AObj holds flags, end frame and a list of FObjs, one per animated channel; an
FObj is a compressed keyframe stream (key opcodes CON / LIN / SPL0 / SPL / SLP / KEY, values quantised by its
format byte). Channel types seen in this game:

| Kind | Channels |
|---|---|
| joint | rotation x / y / z, translation x / y / z, scale x / y / z, **40 = PTCL: fire a particle generator** |
| material | ambient / diffuse / specular r, g, b, alpha |
| texture | image swap (timg), translation u / v, scale u / v, rotation, blend |

A **PTCL key** carries a 24-bit code (bytes 1-3 of the key, little-endian): **bank = bits 0-5, generator =
bits 6-23**, a global number (bank b's generators are numbered from b x 1000). Gun 0's hit emitter fires 6107
and 6110 (bank 6, generators 107 and 110). The game registers the particle callbacks at boot
(`cr_Particle_InitCallbacks` 0x80075CA8: `HSD_JObjSetSPtclCallback`, `HSD_JObjSetDPtclCallback`,
`HSD_JObjSetPtclTargetCallback`).

## 4. Particle banks: `logo_read_us.dbn` entry 0 (verified: `fn_80019E84`, `fn_80075D14`)

Entry 0 of the boot bundle (0x12DD60 bytes) is a table: u32 word count (32), then 4 offsets per bank, from the
entry start: generator list, texture group, and two more blocks (passed on to `psInitDataBankLoad` as they are).
A generator list starts with u16 0x43 (the game hangs on anything else), u16 bank id, u32 first generator number,
u32 count:

| Bank | Generators | Count | Generator list at | Textures at |
|---|---|---|---|---|
| 0 | 0.. | 150 | 0xA0 | 0x5080 |
| 1 | 1000.. | 57 | 0x27780 | 0x293A0 |
| 2 | 2000.. | 92 | 0x3C600 | 0x3F000 |
| 3 | 3000.. | 147 | 0x58820 | 0x5CA60 |
| 4 | 4000.. | 344 | 0x7AF60 | 0x84520 |
| 6 | 6000.. | 575 | 0xC0180 | 0xD1D60 |
| 7 | 7000.. | 18 | 0x112C00 | 0x113480 |
| 5 | (0) | 13 | 0x122600 | 0x122C40 |

They are sysdolphin's particle system (`psInitParticle`, `psGenerateParticle`, ... in main.dol), the same as
Melee's `ptcl` / `texg` data; the generator byte code itself is not mapped here (open).

## 5. Stages (verified: `fn_80072064`, `fn_80072B90`, `fn_80070170`, `fn_80072A58`)

**The model entry (2s)** is one scene. The stage build loads every model into a 0x13C-byte record
(`lbl_8037569C + 0xB8 + n x 0x13C`), **at most 20**, attaches its first animations (models with any animation get
flag 0x10000000 and are set to frame 0), loads the lights (at most 10; the first with type bits 1 also gets a
600 x 440 shadow) and the fog. It **hangs if a root joint of models 1 and up has more than 21 DObjs**. The camera
desc is never read. The stage record's +0x88 (main.dol, per stage) is the model count the game uses after loading.

**The collision entry (2s + 1)** (verified by a byte-identical rebuild of all 39 entries of this format; entry 015,
the RPG test stage, has another layout):

| Where | What |
|---|---|
| +0x00 | u32 N: groups (group n belongs to scene model n) |
| +0x04 | offset of N group-header offsets |
| +0x08 | offset of N quad-array offsets |
| +0x0C | offset of section C (the waypoint records), or 0 |
| group header (0x20) | bounding sphere x, y, z, r; u32 quad count; u32 **model flags**; 2 x u32 0 |
| quad (0x44) | 4 corners (12 floats), unit normal (3 floats), plane d (n . p + d = 0), u32 attribute |
| layout | 0x10 header, the two tables, zero padding to 0x10; per group its header and quads, padded to 0x10; section C |
| section C | 0x284-byte records to the end: a waypoint graph for the computer players (likely; `fn_8006D4F8` links records with a clear line of sight); Impact Craters (stage 0x18) copies 48 of them |

Rules read from the retail data (all 3,069 quads):

- **normal = normalise((c1 - c0) x (c2 - c0))**, d = -n . c0, and the normal points to where a robo stands: arena
  walls face in, obstacles face out (an obstacle box has 4 sides and a top, no bottom).
- The sphere is the corners' box centre with the farthest corner as radius (391 of 413 groups; the other 22 are a
  little smaller than their corners).
- Attributes (surface kinds): 0 mostly; 8, 24, 25-28, 256, 512, 1024-2048, 1280, 5376, 197376 seen; not mapped.

**Model flags** (the group header word, copied to the model record +0xF8):

| Bits | Meaning (from the code) |
|---|---|
| 0-2 (non-zero) | **breakable** model: hit points = `{1000, 100, 150, 200, 250, 300, 350, 400}[flags & 7]` (0x802176C8); damage adds up at record +0x104; past the hit points the game fires effect / sound 0x3000F and plays the model's break animation to its end; while it is hit, its first two textures' blending flash for 4 frames |
| 3-4, or 0x20 | **moving** collision: the group is copied each frame through the model's matrix (`fn_800730F8`, through `lbl_8044EA50`); at most 20 such groups, each at most 100 quads (the copies' buffers 0x803754BC / 0x8035417C) |

Some stages have hard-coded extras in `fn_80072B90` (stage ids 2, 3, 8, 9 (models 6 and 7) and 0xC: animations
set to frame 0 on model 0). Spawn points and the battle camera are not in these files (not located yet).

## 6. Effects: shots, blasts, charge attacks, legs (verified: `fn_80075950`, `fn_8007571C`, `fn_80078000`)

Each entry of gun / bomb / pod / attack_effect is a small wrapper: u32 count (2), count + 1 offsets (the last is the
end), zero padding to 0x20, then **two .dat files** (each padded to 0x20). `leg_effect` entries are one .dat.

| | Piece 0 (wrapper +4) | Piece 1 (wrapper +8) |
|---|---|---|
| loaded by | `fn_8007571C` (archive, `scene_data`) | `fn_80078000` (archive, its externs located, `scene_data`) |
| scene table | 0x80377EB0 attack, 0x80377EC0 bomb, 0x80377ED0 pod, 0x80377EE0 gun (one per robo) | 0x80379920 attack, 0x80379930 gun, 0x80379940 leg, 0x80379950 bomb, 0x80379960 pod |
| spawned by | `fn_800755E8` gun, `fn_800754B8` bomb, `fn_80075388` pod, `fn_80075258` attack | `fn_80077888` gun, `fn_80077C5C` bomb, `fn_800779A8` pod, `fn_80077768` attack, `fn_80077648` leg |
| content (all files) | gun: 4 models, 1-12 meshes; bomb 2 models; pod 1; attack 1 (end frame 60, the charge attack's length) | gun: 2 models, **emitters only** (no meshes, PTCL tracks); bomb / pod: 1 model with 3-6 meshes; attack: 1 model, 2 meshes (end frame 20); leg: 3 emitter models |

Which model goes where (the callers' constants): gun piece 0 models 0 and 1 per shot (shot record +0x310 / +0x314),
model 2 per gun (+0x2DC); gun piece 1 models 0 and 1 per shot (+0x350 / +0x354, the hit); charge attack piece 0
model 0 at robo +0x1DCC and piece 1 model 0 at +0x1DD0; leg models 0, 1 (four copies) and 2 at robo +0x333C0 ..
+0x333D4. Each instance is a separate joint tree with its own animation (`HSD_JObjLoadJoint` + `HSD_JObjAddAnimAll`
per spawn). The effect files' cameras, lights and fog are never read.

**Common effects** (`efc_common.dbn`): a u32 count (4) + offsets; 4 .dat files, `scene_data` each, at
0x80379970[0..3]: [0] robo +0x1E08 and [3] robo +0x333D8 (`cr_BattleRobo_InitFromLoadout`), [2] the gun updater
(`fn_80044174`), [1] one model per index (`fn_80089550`).

## 7. Tools (this repository)

| Command | What |
|---|---|
| `python -I tools/scene.py list stages` / `list effects --kind gun` | ids, names, entries, models, collision groups / per effect entry its two pieces |
| `python -I tools/scene.py info stage 0` / `info effect gun 12` | every model (joints, meshes, materials, textures, animations with end frames and channels), lights, fog, the collision |
| `python -I tools/scene.py export stage 0` | `build/scene/stage_000/`: `scene_data_<n>.glb` per model (HSDRawViewer, Windows) + `stage_000_collision.obj` |
| `python -I tools/scene.py import stage 0 --model 3 EDITED.glb --mod MYMOD` | the model back into `mods/MYMOD/models/bg_data/bg_data_000.dat` (chains) |
| `python -I tools/scene.py import effect gun 12 --piece 0 --model 1 EDITED.glb --mod MYMOD` | the same for an effect piece |
| `python -I tools/stage_collision.py export 0` / `import 0 EDITED.obj --mod MYMOD` | the collision as an OBJ (one object `group_<n>` per group, one face per quad, material `attr_<value>`); the import keeps every unchanged quad's bytes, computes new ones by the rules above, rebuilds the spheres of changed groups |
| `python -I tools/stage_collision.py roundtrip` | all 39 entries: export, import, byte compare |
| `python -I tools/scene_data.py info FILE.dat` / `scan FOLDER` | the reader the others use (standard library only) |
| `python -I tools/make_mod.py mods/MYMOD` | builds the ISO and a shareable patch |

**Adding collision to new geometry** (for example a pillar added with the model loop): export the collision, add
the obstacle's faces to `group_0` (or the group of the model it sits on) in Blender with outward-facing normals
(4 sides and a top), export the OBJ again (keep the axes) and import it. Checked: Basic Arena with a 1.2 x 1.2 x 2.6
box gets 5 new quads in group 0 with the expected normals, every other byte unchanged.

## 8. Code map and decompilation status (Nov 6, 2026)

Matched to C (100%; `fn_80072A58` and `fn_80075D14` as hand drafts that join the build in the next full run): `fn_80072A58` (collision fix-up), `fn_80075950` (part effect file reads), `fn_80075D14`
(particle banks), `fn_80073728`, `fn_80073A74`, `fn_80074FBC` (HSD archive parse). Drafts: the stage build
`fn_80072064` (17%), `fn_80072B90`, the stage update `fn_80070170`, the waypoint linker `fn_8006D4F8` (28%), the
moving-collision transform `fn_800730F8` (97%), the effect loaders `fn_8007571C`, `fn_80078000`, `fn_80077F38` and
spawners (section 6).

## 9. Open questions

- The particle generator format (bank blocks 2-4) and which generator numbers the guns use for what.
- The collision attribute values; section C's record fields; the RPG test stage's collision layout (entry 015).
- Spawn points and the battle camera per stage (the stage record's floats at +0x90..+0x11F, e.g. Basic Arena's pairs
  (-2, 1), (-2, -2), (1, 2), (2, 1), (-2, 2), (1, 2) at +0xF0, have no reader through the stage pointer; robos start
  from thrown cubes, so the start spots may be aimed in battle: not verified).
- What the effect model indices 3 (gun piece 0) mean; leg_effect's 5 extra entries (34 entries, 29 legs).

File descriptors (`cr_g_FileDescs`, first file name of each): 0 logo_read_us.dbn, 1 chara.bin, 2-7 story files
(snr_t_01, snr_chara, snr_map, rpg_t_models, rpg_f_models, snr_motion), 8-11 weapon_gun / bom / pod / leg, 12
bg_data, 13 title sprites, 15 gun_effect, 16 bomb_effect, 17 pod_effect, 18 attack_effect, 22 leg_effect, 24
efc_common.dbn, 27 snr_obj, 28 sentence test, 29 TITLE_MODEL.dat, 30 bmg/info, 31-35 ending part pictures, 36-40
parts book pictures, 41 sound banks, 42 voice streams (14, 19-21, 23, 25, 26 empty).
