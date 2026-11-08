# Where everything is: animations, stages, effects, particles, scene files

One page for everything: plain explanations first (sections 1-9), then the byte-level reference for tool writers
(sections 10-12). Each fact is read from the game's code (function addresses given) or counted over every file on
the disc (US, GXCE01); anything not verified says so. All file names are under the disc's `files/` folder
(`Custom Robo extracted/files/` after the one-time build). The animation byte format is in
[ANIMATION_GUIDE.md](ANIMATION_GUIDE.md); deeper pages: [stage_tables.md](stage_tables.md) (every stage record),
[formats/containers.md](formats/containers.md) (the archives), [MODDING.md](MODDING.md),
[IMPORT_EXPORT_GUIDE.md](IMPORT_EXPORT_GUIDE.md) (vertices, textures, the import rules).

## 1. Robo animations (walk, jump, shoot, victory, charge attack ...)

**Stored in:** `logo_read_us.dbn`, entry 8. Not `motion.DBN` (the game never reads that file).

**What it is:** a list of motions, each with a number (its ID, 0..1094). A motion is a set of curves for an
invisible stick figure: where the hips, hands, feet and head should be, and how 16 joints turn. It does not
store the robo's bones. Every frame the game bends each robo's own arms and legs to reach those points, so
one motion works for every robo.

**How a robo picks its animation:**

1. Each body has a list of 38 "slots" in main.dol (stance, walk, run, jump, gun, bomb, pod, falls, get-up, lose,
   victory 1 and 2, charge attack, ...). Each slot holds one motion ID.
2. What the robo is doing (its action: moving, shooting, falling, winning ...) picks the slot every frame.
3. When the slot's motion is a different one from what is playing, the game switches to it and **blends from the
   old pose to the new one over 16 frames**, so moves flow into each other.
4. Most motions run at 30 frames a second. Victory, lose, charge attack and get-up do not: the game sets their
   frame from its own timers (victory waits, then plays over 140 game frames; the charge attack plays its 60
   frames as three 20-frame parts, each stretched to that phase's duration).

**Find it:**

```
python -I tools/anim.py slots --body 20                  # Breaker's 38 slots: motion name, ID, length
python -I tools/anim.py export --body 20 --slot victory2 # -> build/anim/<name>.glb, open in Blender
python -I tools/battle_motions.py --tsv                   # every body x slot
```

## 2. Other animations and where each lives

| Animation | Stored in | How it plays |
|---|---|---|
| Story-mode character moves | `scenario_us/snr_motion.BIN` | same format as the robo motions |
| Transforming bodies' own change (Defender, Seeker, Breaker, Ruhiel) | inside the body's model in `chara.BIN` | standard HSD animation; held at frame 0, run to frame 10 during the moves that change form |
| Part animations (gun, bomb, pod, leg opening / changing) | inside the part's model in `weapon_gun.BIN`, `weapon_bom.BIN`, `weapon_pod.BIN`, `weapon_leg.BIN` | standard HSD animation, frames 0..10: the game sets the frame from the part's state (frame = 10 x state) |
| Stage animations (moving platforms, spinning things, flashing lights) | inside the stage model, `bg_data.BIN` entry 2s | standard HSD joint / material / texture animation attached to a scene model; **loops** (every stage animation has the loop flag), e.g. Basic Arena models 2-4 run 720 frames |
| Stage break animations | the same stage models | a breakable model plays its animation once when its hit points run out |
| Shot, blast, charge attack, leg jet effects | `gun_effect.BIN`, `bomb_effect.BIN`, `pod_effect.BIN`, `attack_effect.BIN`, `leg_effect.BIN` | standard HSD animation, **played once** per spawned effect (no loop flag); keyframes of type 40 fire particles |
| Particles (sparks, smoke, flashes) | `logo_read_us.dbn` entry 0: 8 particle banks | started by an effect's particle keys (bank + generator number) |
| Common effects | `efc_common.dbn` (4 scenes) | standard HSD animation |
| Story objects / maps | `scenario_us/snr_obj.BIN`, `scenario_us/snr_map.BIN` | standard HSD |
| Title screen | `TITLE_MODEL.dat` | standard HSD |

"Standard HSD animation" is the same system as Super Smash Bros. Melee: HSDRawViewer opens it, and the exports in
`models_export/` already include it.

## 3. Stages

### How a stage gets on screen

1. The stage select shows the stages in the order of a list in main.dol (0x8023CF00, 40 bytes of stage ids). A
   stage whose record says "hidden" (+0x80 = 0) is skipped: Basic Arena (Test Use) and RPG Test.
2. You pick one: that gives a **stage id** s (0..39).
3. The game reads two entries of `bg_data.BIN`: **2s** (the look) and **2s + 1** (the collision).
4. It builds the look: every model, its animation, the lights and the fog. It sets up the collision.
5. Every frame of the battle: the stage animations play (they loop), breakable parts take damage and break,
   moving parts carry their collision with them.

### The two files of a stage

| Entry | What it holds | Size limit |
|---|---|---|
| **2s** (the look) | up to 20 models; each model can carry one joint animation (moves / turns parts), one material animation (colours, fading) and one texture animation (scrolling, swapping pictures); up to 10 lights; fog | 0x100000 bytes |
| **2s + 1** (the collision) | one group of flat faces per model (group n = model n), then a waypoint list | 0x8000 bytes |

**In the look:**

- A **model** is a normal 3D model (joints, meshes, materials, textures), the same format as Melee's.
- Its **animation loops** forever (every stage animation has the loop flag). Example: Basic Arena's models 2, 3
  and 4 run a 720-frame loop.
- **Lights**: usually two (one ambient, one directional). The first directional one also casts the robo shadows.
- **Fog**: its colour also becomes the background colour behind the stage.
- The file also has a camera, but the game **never uses it** (the battle camera is the game's own).
- Do not put more than 21 meshes directly on the top joint of model 1 or later: the game freezes on load.

**In the collision:**

- Each **face** is a flat four-cornered shape (a triangle is stored with one corner twice) with the direction it
  faces. Faces point toward where a robo stands: an arena's walls face inward, a box in the middle faces outward
  (four sides and a top, no bottom).
- Each **group** has a sphere around its faces (for fast tests) and a **flag word** for its model:

| Flag | Meaning |
|---|---|
| 0 | a normal, solid model |
| 1..7 (bits 0-2) | **breakable**: hit points 100 (1), 150 (2), 200 (3), 250 (4), 300 (5), 350 (6), 400 (7); when it breaks the game plays a sound / effect and the model's break animation; while hit, it flashes between its first two textures |
| 8, 16, 24 (bits 3-4) or 32 | **moving**: the collision follows the model's animation every frame (moving platforms, rotating walls); at most 20 moving groups, 100 faces each |

- Each face also has a **surface number** (0 for most; other values like 8, 24, 256, 512 exist: probably ice,
  slopes, magma and so on; not mapped yet).
- The **waypoint list** (0x284 bytes per point) is most likely the computer players' path map.

### The stage record in main.dol (per stage, 0x340 bytes at 0x8020F4C8)

| Field | What |
|---|---|
| +0x00 | the developer's Japanese name |
| +0x80 | shown in the stage select (0 = hidden) |
| +0x88 | how many models the game uses |
| +0xCC..+0xD4 | the opening camera's position |
| +0x120.. | a per-model kind list (model number, value; ended by 0xFF) used when the models are set up |
| +0x338..+0x33C | a stage kind number, a table index, the stage-select message, one float (not mapped) |

The US names are messages in `bmg/info_usa.bin` (message block 24, message = stage id). The full record is in
[stage_tables.md](stage_tables.md).

Two stages are **alternates** the menu never lists: holding one button (not identified yet) while choosing Basic
Arena gives Practice Stage (id 10), and No-Man's Land (27) gives the other No-Man's Land (11).

### Every stage

Entries are `bg_data.BIN` entry numbers; models / animated = scene models / models with a joint animation; collision
= groups / faces.

| id | Name | Menu slot | Entries | Models | Animated | Lights | Fog | Collision | Breakable | Moving | Waypoints |
|---|---|---|---|---|---|---|---|---|---|---|---|
| 0 | Basic Arena | 0 | 000 / 001 | 10 | 3 | 2 | no | 10 / 56 | 3 | 0 | 21 |
| 1 | High-Rise Plaza | 2 | 002 / 003 | 9 | 3 | 2 | no | 9 / 43 | 2 | 0 | 20 |
| 2 | High-Rise Cell | 5 | 004 / 005 | 9 | 2 | 2 | no | 9 / 56 | 2 | 0 | 19 |
| 3 | Nature Park | 15 | 006 / 007 | 16 | 2 | 2 | yes | 16 / 146 | 2 | 0 | 22 |
| 4 | Dead Line | 22 | 008 / 009 | 8 | 2 | 2 | no | 8 / 50 | 0 | 2 | 19 |
| 5 | Flower Garden | 12 | 010 / 011 | 13 | 1 | 2 | yes | 13 / 97 | 1 | 0 | 21 |
| 6 | Basic Arena (Test Use) | hidden | 012 / 013 | 10 | 3 | 2 | no | 10 / 56 | 3 | 0 | 21 |
| 7 | RPG Test (grey blockout of the story office, map 2 OFC = snr_map entry 2, see hidden_content.md) | hidden | 014 / 015 | 1 | 0 | 3 | no | other layout | - | - | - |
| 8 | Diamond Cell | 4 | 016 / 017 | 9 | 2 | 2 | no | 9 / 56 | 2 | 0 | 15 |
| 9 | Chinese Bowl | 13 | 018 / 019 | 8 | 2 | 2 | yes | 8 / 130 | 0 | 2 | 27 |
| 10 | Practice Stage | alt of 0 | 020 / 021 | 10 | 3 | 2 | no | 10 / 56 | 3 | 0 | 21 |
| 11 | No-Man's Land | alt of 27 | 022 / 023 | 1 | 0 | 2 | no | 1 / 5 | 0 | 0 | 16 |
| 12 | Ice and Snow | 20 | 024 / 025 | 11 | 2 | 7 | no | 11 / 101 | 2 | 0 | 27 |
| 13 | Castle Keep | 9 | 026 / 027 | 11 | 0 | 2 | yes | 11 / 107 | 0 | 0 | 19 |
| 14 | Crevice Court | 6 | 028 / 029 | 12 | 2 | 2 | no | 12 / 83 | 2 | 0 | 21 |
| 15 | Diamond Fences | 1 | 030 / 031 | 10 | 4 | 2 | no | 10 / 50 | 3 | 0 | 20 |
| 16 | Frozen Field | 21 | 032 / 033 | 10 | 3 | 2 | no | 10 / 50 | 3 | 0 | 18 |
| 17 | L Formation | 7 | 034 / 035 | 9 | 2 | 2 | yes | 9 / 57 | 2 | 0 | 21 |
| 18 | Checkmate Foundry | 8 | 036 / 037 | 11 | 2 | 2 | no | 11 / 70 | 2 | 0 | 20 |
| 19 | Little Locomotive | 16 | 038 / 039 | 17 | 8 | 2 | yes | 17 / 89 | 4 | 8 | 23 |
| 20 | Magma Hole | 18 | 040 / 041 | 12 | 4 | 2 | no | 12 / 120 | 2 | 2 | 21 |
| 21 | Castle Citadel | 10 | 042 / 043 | 11 | 0 | 2 | yes | 11 / 107 | 0 | 0 | 21 |
| 22 | Panic Cubes | 24 | 044 / 045 | 19 | 15 | 2 | yes | 19 / 87 | 0 | 14 | 20 |
| 23 | Magma Ruins | 19 | 046 / 047 | 13 | 2 | 2 | no | 13 / 117 | 0 | 1 | 17 |
| 24 | Impact Craters | 29 | 048 / 049 | 20 | 16 | 2 | no | 20 / 81 | 0 | 16 | 19 |
| 25 | Sudden Death | 30 | 050 / 051 | 4 | 0 | 2 | no | 4 / 26 | 0 | 0 | 11 |
| 26 | Gigantix Sprawl | 31 | 052 / 053 | 11 | 0 | 2 | no | 11 / 61 | 0 | 0 | 24 |
| 27 | No-Man's Land | 32 | 054 / 055 | 1 | 1 | 2 | yes | 1 / 5 | 0 | 0 | 16 |
| 28 | Robo's Room | 14 | 056 / 057 | 10 | 0 | 2 | yes | 10 / 61 | 0 | 0 | 14 |
| 29 | Practice Stage | 33 | 058 / 059 | 8 | 2 | 2 | no | 8 / 46 | 2 | 0 | 14 |
| 30 | Double Dead Line | 23 | 060 / 061 | 8 | 3 | 2 | no | 8 / 59 | 0 | 3 | 13 |
| 31 | Merry-Go-Round | 17 | 062 / 063 | 10 | 10 | 2 | no | 10 / 142 | 0 | 8 | 15 |
| 32 | Loading Dock | 27 | 064 / 065 | 11 | 3 | 2 | no | 10 / 110 | 0 | 1 | 16 |
| 33 | Battle Gear Station | 28 | 066 / 067 | 12 | 9 | 2 | no | 12 / 164 | 1 | 7 | 20 |
| 34 | Panic Walls | 25 | 068 / 069 | 18 | 18 | 2 | yes | 18 / 86 | 0 | 17 | 17 |
| 35 | Lost World | 11 | 070 / 071 | 5 | 1 | 2 | yes | 5 / 130 | 0 | 0 | 14 |
| 36 | Scramble Walls | 26 | 072 / 073 | 17 | 16 | 2 | yes | 17 / 85 | 0 | 16 | 17 |
| 37 | Dark Star | 34 | 074 / 075 | 6 | 1 | 2 | no | 12 / 102 | 0 | 0 | 30 |
| 38 | Basic Cell | 3 | 076 / 077 | 9 | 0 | 2 | no | 9 / 61 | 0 | 0 | 19 |
| 39 | Random Stage | 35 | 078 / 079 | 1 | 1 | 3 | no | 9 / 61 | 0 | 0 | 19 |

### Story-mode maps (not battle stages)

`scenario_us/snr_map.BIN` (94 entries) holds the story mode's walking maps: each entry is a stage model **and** a
collision in one file (same two formats as above), plus the map's edges (x / z min and max). The objects on the
maps are in `scenario_us/snr_obj.BIN`.

### Find it and edit it

```
python -I tools/scene.py list stages                 # the table above, from the files
python -I tools/scene.py info stage 0                # every model, its animations, lights, fog, the collision
python -I tools/scene.py export stage 0              # -> build/scene/stage_000/: a .glb per model + the collision .obj
python -I tools/stage_collision.py info 0            # collision groups, breakable / moving flags, waypoints
```

To change a stage:

1. Export it (above), or use the ready-made `models_export/bg_data/bg_data_<2s>/scene_data_<n>.glb`.
2. Edit a model in Blender, then put it back:
   `python -I tools/scene.py import stage 0 --model 3 edited.glb --mod MYMOD` (several models chain: the mod's
   file is used as the starting point when it exists).
3. If you added something robos should bump into, open `stage_000_collision.obj` (one object `group_<n>` per
   model, one face per collision face, material `attr_<surface number>`), add the new faces to the group of the
   model it stands on, facing outward (4 sides and a top), export the OBJ again with the same axes, then
   `python -I tools/stage_collision.py import 0 edited.obj --mod MYMOD`. Unchanged faces keep their bytes; new
   faces get the game's own normal rule (section 10). Checked: a 1.2 x 1.2 x 2.6 box on Basic Arena becomes 5 new
   faces in group 0, every other byte unchanged.
4. Build: `python -I tools/make_mod.py mods/MYMOD` (an ISO and a patch to share).

## 4. Shots, blasts, charge attacks, leg jets (effects)

### Where they are

| File | One entry per | Entries | Size limit per entry | Piece 0 (what flies / shows) | Piece 1 (what happens on hit) | Particle banks used |
|---|---|---|---|---|---|---|
| `gun_effect.BIN` | gun (same number as the gun) | 64 | 0x16000 | 4 models: the shot meshes (1-12 meshes) and particle emitters | 2 models: particle emitters only (the hit sparks) | 0 (shots), 6 (hits) |
| `bomb_effect.BIN` | bomb | 52 | 0x40000 | 2 models: the flying bomb | 1 model: the explosion meshes (3-6 meshes) | 1 (flying), 6 (blast) |
| `pod_effect.BIN` | pod | 57 | 0x40000 | 1 model: the pod | 1 model: the explosion meshes | 2 (pods), 6 (blast) |
| `attack_effect.BIN` | **body** (the charge attack belongs to the body) | 34 | 0x14000 | 1 model, 60 frames (the length of the charge attack) | 1 model, 2 meshes, 20 frames | 4 |
| `leg_effect.BIN` | leg (34 entries for 29 legs) | 34 | 0x1000 | one file only: 3 particle-emitter models (the jets) | - | 3 |

### How an effect works

1. When a battle loads, each robo reads its gun, bomb, pod, leg and body entries into its own buffers (that is
   why each entry has a size limit).
2. Each time something is fired, the game makes a **new copy** of the model it needs from piece 0 (a gun copies
   models 0 and 1 for every shot, model 2 once for the gun) and starts its animation at frame 0.
3. The animation **plays once** (effect animations have no loop flag). Its keyframes move and fade the meshes,
   and some keyframes are **particle keys**: at that frame they start a particle generator (sparks, smoke,
   trails) at that joint.
4. On a hit or explosion, the game copies a model from piece 1 the same way: it plays once, its particle keys fire
   the hit or blast particles.
5. The charge attack copies piece 0 model 0 and piece 1 model 0 for its whole run; a robo's leg jets are 6 copies
   of the leg models (model 0 once, model 1 four times, model 2 once).

Each effect file also has a camera, lights and fog, but the game never reads them (only the models).

**Find it:**

```
python -I tools/scene.py list effects --kind gun   # per gun: models, meshes and particle keys of both pieces
python -I tools/scene.py info effect gun 0         # both pieces: models, animations, the particles each model fires
python -I tools/scene.py export effect gun 0       # -> build/scene/gun_effect_000/: .glb files for Blender
```

Ready-made exports: `models_export/gun_effect/gun_effect_000_0/` (piece 0), `..._000_1/` (piece 1), and the same
for `bomb_effect`, `pod_effect`, `attack_effect`, `leg_effect`.

**Edit it:** edit the .glb in Blender, then
`python -I tools/scene.py import effect gun 0 --piece 0 --model 1 edited.glb --mod MYMOD` and
`python -I tools/make_mod.py mods/MYMOD`. The entry must stay under its size limit (make_mod checks, and keeps
main.dol's copy of the entry sizes in step).

## 5. Particles

**Stored in:** `logo_read_us.dbn`, entry 0 (the boot bundle, loaded once at start). It holds **8 particle banks**.
Each bank is a list of particle generators (how many particles, how they move, grow, fade, which picture they use)
and the pictures (textures) they use.

**How they are started:** only by particle keys in the effect animations above (and the common effects). A key
holds a bank number and a **generator number**; the generator number already includes the bank (bank 6's
generators are 6000, 6001, ...). Gun 0's hit, for example, fires generators 6107 and 6110.

| Bank | Generators | How many | Used by |
|---|---|---|---|
| 0 | 0..149 | 150 | gun shots (piece 0 of every gun) |
| 1 | 1000..1056 | 57 | flying bombs (piece 0) |
| 2 | 2000..2091 | 92 | pods (piece 0) |
| 3 | 3000..3146 | 147 | leg jets |
| 4 | 4000..4343 | 344 | charge attacks (both pieces) |
| 5 | 0..12 | 13 | common effects 0, 2, 3 (`efc_common.dbn`) |
| 6 | 6000..6574 | 575 | hits and explosions (piece 1 of every gun, bomb and pod) |
| 7 | 7000..7017 | 18 | common effect 1 |

**Find it:** `python -I tools/scene.py info effect gun 0` lists the generator numbers each model fires.

**Format (mapped Nov 7, `tools/ptcl.py`):** entry 0 starts with `u32 0x20`, then 8 x 4 section offsets (+ the end).
Per bank: section 0 = the generator list (HSD particle group, as in Melee: `u16 0x43`, `u16 bank`, `u32` first
generator number, `u32` count, count offsets; each generator = a 0x3C-byte header - shape, texture group, life, kind
flags, gravity, friction, velocity, radius, angle, random, size - then a byte-code track ending in FD / FE / FF);
section 1 = the pictures (texg: count, offsets; each image count, GX format, palette format, size); sections 2 and 3
small tables (not decoded). The sections hold banks 0, 1, 2, 3, 4, 6, 7, 5 in that order. In the track, `C0..CF` set
the particle colour and `D0..DF` the glow colour (a time, then the R / G / B / A bytes the low 4 bits select), `EF` /
`F0` spawn a child generator (its index in the bank). 1,395 of the 1,396 generators decode to their terminator
(bank 5's numbers 0..12 repeat bank 0's: a generator is (bank, number)).

```
python -I tools/ptcl.py list [bank]          every generator with its colour commands
python -I tools/ptcl.py recolour <in.dbn> <out.dbn> <plan.json>   hue-shift chosen generators in place (same size)
```
A mod applies a plan with `particles.json` in its folder (make_mod.py). Effect-model textures are paletted (CI8):
`tools/recolour_dat.py <in.dat> <out.dat> gold` hue-shifts their palettes in place. Most generators are shared
(gun 0's shot sparks are used by 13-64 guns); a robo's charge (bank 4) is its own (robo 0: 4000-4002, 4005, 4006).

## 6. Common effects

**Stored in:** `efc_common.dbn` (4 small scene files, loaded at battle start).

| Entry | What it has | Used by |
|---|---|---|
| 0 | 1 emitter model, 45 frames, fires common particle 0 | every robo (robo +0x1E08, at robo setup) |
| 1 | 3 models with meshes, 100 frames, fire bank 7 particles 7002 / 7006 / 7011 | one model per index (`fn_80089550`) |
| 2 | 1 emitter model, 5 frames, fires common particle 6 | the gun code (`fn_80044174`) |
| 3 | 1 emitter model, 30 frames, fires common particle 9 | every robo (robo +0x333D8, at robo setup) |

## 7. What a "scene" file is (scene_data)

Every stage, effect, common effect, story object and the title model is the same kind of file: a standard HSD
`.dat` (the format Super Smash Bros. Melee uses) with one thing inside named **`scene_data`**.

### The file around it

| Part | What |
|---|---|
| header (0x20 bytes) | file size, data size, how many pointers, how many names |
| data block | everything below (models, meshes, pictures, animations), with pointers as offsets inside the block |
| pointer list | where every pointer is (so the game can turn the offsets into addresses when it loads) |
| names | the name of the thing inside: always `scene_data` here; the game looks it up by that name |

### Inside scene_data: four lists

| List | What | Stages use it | Effects use it |
|---|---|---|---|
| **models** | each: a joint tree with meshes, plus its joint / material / texture animation | yes, up to 20 | yes |
| **cameras** | where a camera stands and looks, how wide | no (never read) | no |
| **lights** | kind (ambient, directional, point, spot), colour, position | yes, up to 10 | no |
| **fog** | start and end distance, colour (also the background colour) | yes | no |

### A model, piece by piece

```
model
 |- joint tree ............ the skeleton: every joint has a position, rotation, scale,
 |   |                      a first child and a next sibling, and maybe a mesh list
 |   '- mesh (DObj) ....... one material + one or more shapes
 |       |- material (MObj) colours (ambient, diffuse, specular), see-through amount,
 |       |    |             shininess, blending (how it mixes with what is behind)
 |       |    '- textures   pictures (TObj): the image, how it repeats, how it is
 |       |                  mapped (UVs, or reflection from the surface direction)
 |       '- shape (PObj) .. the triangles: vertex positions, normals, colours, UVs;
 |                          rigid to one joint or skinned to several
 |- joint animation ....... one entry per joint (same tree shape as the skeleton)
 |- material animation .... one entry per material: colour / fade, texture scroll, picture swaps
 '- shape animation ....... (rare) morphs
```

**Joint flags** worth knowing (a joint's flag word): 0x10 hidden, 0x80 lit by the lights, 0x200 always faces the
camera (billboard), 0x4 skinned mesh, 0x20 a particle joint, 0x40000 has solid meshes, 0x80000 has see-through
meshes, 0x10000000 / 0x20000000 the same for the whole tree below.

### An animation, piece by piece

| Part | What |
|---|---|
| **AObj** (one per animated joint or material) | the **end frame**, a **loop flag** (0x20000000: stages have it, effects do not), the list of tracks |
| **track** (FObj) | one animated value: rotation x / y / z, position x / y / z, scale x / y / z, a colour, the alpha, a texture's scroll / scale / picture, or **ptcl** (fire a particle) |
| **keys** inside a track | packed keyframes: each says how to reach the next value (jump, straight line, smooth curve with slopes) and after how many frames |

Example (Basic Arena, `bg_data_000.dat`): model 2 is a 31-joint tree with 22 meshes; its joint animation moves
20 of the joints (rotation, position, a little scale) over 720 frames and loops; its material animation fades 16
materials over the same 720 frames.

### What the game does with it

- **Stages**: loads every model, attaches its animations (the first of each list), plays them every frame; loads
  the lights (the first directional light also gets the shadow) and the fog.
- **Effects**: keeps the file in memory and makes a fresh copy of a model each time it is needed (each shot,
  each hit), starting its animation at frame 0; it never reads the effect's cameras, lights or fog.

**Find it:**

```
python -I tools/scene_data.py info models_dat/bg_data/bg_data_000.dat            # the lists, every model, its animations
python -I tools/scene_data.py info models_dat/bg_data/bg_data_000.dat --joints   # + every joint: flags, mesh, position
python -I tools/scene_data.py scan models_dat/gun_effect                         # one line per file of a folder
```

HSDRawViewer (in `tools/HSDLib`, Windows) opens the same files with a 3D view and a tree of every part, and
exports / imports .glb for Blender. The byte offsets are in section 10 and
[IMPORT_EXPORT_GUIDE.md](IMPORT_EXPORT_GUIDE.md) sections 3-4.

## 8. Other scene files

| File | What | Notes |
|---|---|---|
| `scenario_us/snr_obj.BIN` | 89 story-mode objects | one model each, with animation (up to 300 frames) |
| `scenario_us/snr_map.BIN` | 94 story-mode maps | a stage model and a collision in one entry (section 3) |
| `TITLE_MODEL.dat` | the title screen's model | one scene |
| `chara.BIN`, `weapon_*.BIN` | robos and parts | not scene files: their own model format around HSD joints ([IMPORT_EXPORT_GUIDE.md](IMPORT_EXPORT_GUIDE.md)) |

## 9. What to open for what

| I want ... | Open |
|---|---|
| a robo's victory / charge / walk animation | `python -I tools/anim.py export --body N --slot NAME` |
| which motion a robo uses for something | `python -I tools/anim.py slots --body N` |
| the list of stages | `python -I tools/scene.py list stages` |
| a stage's models and their animations | `python -I tools/scene.py export stage S` (or `models_export/bg_data/bg_data_<2S>/`) |
| a stage's collision | `python -I tools/stage_collision.py export S` (and `import S EDITED.obj --mod MYMOD`) |
| a gun's shot / hit | `python -I tools/scene.py export effect gun N` |
| a bomb's blast / a charge attack's effect | `python -I tools/scene.py export effect bomb N` / `export effect attack BODY` |
| which particles an effect fires | `python -I tools/scene.py info effect gun N` |
| a model back into the game | `python -I tools/scene.py import stage S --model M EDITED.glb --mod MYMOD` (or `import effect KIND N --piece P --model M ...`) |
| a part's open / change animation | `models_export/weapon_gun/` (and `weapon_bom`, `weapon_pod`, `weapon_leg`) |
| a transforming body's change | `models_export/chara/chara_<5N>/` |
| the particle banks | `logo_read_us.dbn` entry 0 (layout: section 10) |
| the common effects | `efc_common.dbn` (4 scenes; section 6) |
| any scene file's contents | `python -I tools/scene_data.py info FILE.dat` (`--joints` for every joint), `scan FOLDER` |
| the collision rebuild check | `python -I tools/stage_collision.py roundtrip` (all 39 entries: export, import, byte compare) |
| an ISO and a patch of my mod | `python -I tools/make_mod.py mods/MYMOD` |

## 10. Byte-level reference (for tool writers)

Big-endian throughout.

### Files, loaders and size limits

| What | File (archive number) | Entry | Loaded by | Size limit |
|---|---|---|---|---|
| Stage model | `bg_data.BIN` (12) | 2s for stage id s | the stage build `fn_80072064` | 0x100000 |
| Stage collision | `bg_data.BIN` (12) | 2s + 1 | `fn_80072A58` | 0x8000 |
| Gun shots | `gun_effect.BIN` (15) | the gun number | per robo (`fn_80075950`) | 0x16000 |
| Bomb blasts | `bomb_effect.BIN` (16) | the bomb number | per robo | 0x40000 |
| Pod effects | `pod_effect.BIN` (17) | the pod number | per robo | 0x40000 |
| Charge attack effects | `attack_effect.BIN` (18) | the body number | per robo | 0x14000 |
| Leg effects | `leg_effect.BIN` (22) | the leg number | per robo | 0x1000 |
| Common effects | `efc_common.dbn` (24) | 4 scenes | `fn_80078478` / `fn_80077F38` | - |
| Particle banks | `logo_read_us.dbn` (0, the boot bundle, 2,842,624 bytes) | entry 0 | at boot, `fn_80075D14` -> `psInitDataBankLoad` | - |
| Story maps | `scenario_us/snr_map.BIN` (4) | per map | collision block + scene_data + bounds ([stage_tables.md](stage_tables.md)) | - |
| Story objects | `scenario_us/snr_obj.BIN` (27) | 89 | one model each | - |
| Title model | `TITLE_MODEL.dat` (29) | - | the title screen | - |

The archive numbers are main.dol's file descriptor table `cr_g_FileDescs` (0x801F1200, 43 entries; first file of
each): 0 logo_read_us.dbn, 1 chara.bin, 2-7 story files (snr_t_01, snr_chara, snr_map, rpg_t_models, rpg_f_models,
snr_motion), 8-11 weapon_gun / bom / pod / leg, 12 bg_data, 13 title sprites, 15 gun_effect, 16 bomb_effect, 17
pod_effect, 18 attack_effect, 22 leg_effect, 24 efc_common.dbn, 27 snr_obj, 28 sentence test, 29 TITLE_MODEL.dat,
30 bmg/info, 31-35 ending part pictures, 36-40 parts book pictures, 41 sound banks, 42 voice streams (14, 19-21,
23, 25, 26 empty).

### The HSD .dat file (HSDLib and our reader agree on every file)

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

Every scene file has one root, `scene_data` (`HSD_ArchiveGetPublicAddress(archive, "scene_data")`).

### scene_data (HSDLib HSD_SOBJ; checked in the game's loaders)

| Offset | What | Used by the game |
|---|---|---|
| +0x00 | null-terminated array of model pointers (JOBJDesc) | always |
| +0x04 | null-terminated array of camera descs | never |
| +0x08 | null-terminated array of lights ({LightDesc, animation list}) | stages only (`HSD_LObjLoadDesc`, at most 10) |
| +0x0C | fog ({FogDesc, AObj}) | stages only (`HSD_FogLoadDesc`; its colour becomes the clear colour) |

A model (JOBJDesc, 0x10 bytes): root joint, then three null-terminated arrays: joint animations (AnimJoint trees),
material animations (MatAnimJoint trees), shape animations. The game loads the root with `HSD_JObjLoadJoint` and
attaches the first animation of each array (`HSD_JObjAddAnimAll`).

Below a joint everything is standard HSD: Joint (0x40: name, flags, child, next, DObj list or particle joint,
rotation, scale, translation, inverse bind matrix, constraints), DObj (next, MObj, PObj list), MObj (render mode,
TObj list, colours, pixel engine), TObj (image, palette, wrap, scale, filtering, texture matrix), PObj (vertex
attributes and display list; rigid, envelope or shape animation).

An AObj holds flags (0x20000000 = loop), the end frame and a list of FObjs, one per animated channel; an FObj is a
compressed keyframe stream (key opcodes CON / LIN / SPL0 / SPL / SLP / KEY, values quantised by its format byte).
Channel types seen in this game: joint rotation / translation / scale x, y, z and **40 = PTCL**; material ambient /
diffuse / specular r, g, b, alpha; texture image swap, translation u / v, scale u / v, rotation, blend.

A **PTCL key** carries a 24-bit code (bytes 1-3 of the key, little-endian): bank = bits 0-5, generator = bits
6-23, a global number (bank b's generators are numbered from b x 1000). The game registers the particle callbacks
at boot (`cr_Particle_InitCallbacks` 0x80075CA8: `HSD_JObjSetSPtclCallback`, `HSD_JObjSetDPtclCallback`,
`HSD_JObjSetPtclTargetCallback`).

### Particle banks (`logo_read_us.dbn` entry 0; `fn_80019E84`, `fn_80075D14`)

Entry 0 (0x12DD60 bytes) is a table: u32 word count (32), then 4 offsets per bank, from the entry start: generator
list, texture group, and two more blocks (passed on to `psInitDataBankLoad` as they are). A generator list starts
with u16 0x43 (the game hangs on anything else), u16 bank id, u32 first generator number, u32 count.

| Bank | First generator | Count | Generator list at | Textures at |
|---|---|---|---|---|
| 0 | 0 | 150 | 0xA0 | 0x5080 |
| 1 | 1000 | 57 | 0x27780 | 0x293A0 |
| 2 | 2000 | 92 | 0x3C600 | 0x3F000 |
| 3 | 3000 | 147 | 0x58820 | 0x5CA60 |
| 4 | 4000 | 344 | 0x7AF60 | 0x84520 |
| 6 | 6000 | 575 | 0xC0180 | 0xD1D60 |
| 7 | 7000 | 18 | 0x112C00 | 0x113480 |
| 5 | 0 | 13 | 0x122600 | 0x122C40 |

They are sysdolphin's particle system (`psInitParticle`, `psGenerateParticle`, ... in main.dol), the same as
Melee's `ptcl` / `texg` data.

### Stage build (`fn_80072064`, `fn_80072B90`, `fn_80070170`)

- Every model goes into a 0x13C-byte record (`lbl_8037569C + 0xB8 + n x 0x13C`), at most 20. Models with any
  animation get flag 0x10000000 and are set to frame 0.
- Lights: at most 10; the first with type bits 1 also gets a 600 x 440 shadow. Then the fog.
- It hangs if a root joint of models 1 and up has more than 21 DObjs. The camera desc is never read.
- The stage record's +0x88 (main.dol) is the model count the game uses after loading.
- `fn_80072B90` has hard-coded extras for stage ids 2, 3, 8, 9 (models 6 and 7) and 0xC: animations set to frame
  0 on model 0.

### Stage collision (entry 2s + 1; read by `fn_80072A58`)

Verified by a byte-identical rebuild of all 39 entries of this layout (entry 015, the RPG test stage, has another).

| Where | What |
|---|---|
| +0x00 | u32 N: groups (group n belongs to scene model n) |
| +0x04 | offset of N group-header offsets |
| +0x08 | offset of N quad-array offsets |
| +0x0C | offset of section C (the waypoint records), or 0 |
| group header (0x20) | bounding sphere x, y, z, r; u32 quad count; u32 model flags; 2 x u32 0 |
| quad (0x44) | 4 corners (12 floats), unit normal (3 floats), plane d (n . p + d = 0), u32 attribute |
| layout | 0x10 header, the two tables, zero padding to 0x10; per group its header and quads, padded to 0x10; section C |
| section C | 0x284-byte records to the end: a waypoint graph for the computer players (likely; `fn_8006D4F8` links records with a clear line of sight); Impact Craters (stage 0x18) copies 48 of them |

Rules read from the retail data (all 3,069 quads):

- normal = normalise((c1 - c0) x (c2 - c0)), d = -n . c0, and the normal points to where a robo stands.
- The sphere is the corners' box centre with the farthest corner as radius (391 of 413 groups; the other 22 are a
  little smaller than their corners).
- Attributes seen: 0 mostly; 8, 24, 25-28, 256, 512, 1024-2048, 1280, 5376, 197376.

Model flags (the group header word, copied to the model record +0xF8):

| Bits | Meaning (from the code) |
|---|---|
| 0-2 (non-zero) | breakable: hit points = `{1000, 100, 150, 200, 250, 300, 350, 400}[flags & 7]` (0x802176C8); damage adds up at record +0x104; past the hit points the game fires effect / sound 0x3000F and plays the model's break animation to its end; while hit, its first two textures' blending flashes for 4 frames |
| 3-4, or 0x20 | moving: the group is copied each frame through the model's matrix (`fn_800730F8`, through `lbl_8044EA50`); at most 20 such groups, each at most 100 quads (the copies' buffers 0x803754BC / 0x8035417C) |

### Effect files (`fn_80075950`, `fn_8007571C`, `fn_80078000`)

Each entry of gun / bomb / pod / attack_effect is a small wrapper: u32 count (2), count + 1 offsets (the last is the
end), zero padding to 0x20, then two .dat files (each padded to 0x20). `leg_effect` entries are one .dat.

| | Piece 0 (wrapper +4) | Piece 1 (wrapper +8) |
|---|---|---|
| loaded by | `fn_8007571C` | `fn_80078000` (also resolves its externs) |
| scene table (one per robo) | 0x80377EB0 attack, 0x80377EC0 bomb, 0x80377ED0 pod, 0x80377EE0 gun | 0x80379920 attack, 0x80379930 gun, 0x80379940 leg, 0x80379950 bomb, 0x80379960 pod |
| spawned by | `fn_800755E8` gun, `fn_800754B8` bomb, `fn_80075388` pod, `fn_80075258` attack | `fn_80077888` gun, `fn_80077C5C` bomb, `fn_800779A8` pod, `fn_80077768` attack, `fn_80077648` leg |

Which model goes where (the callers' constants): gun piece 0 models 0 and 1 per shot (shot record +0x310 / +0x314),
model 2 per gun (+0x2DC); gun piece 1 models 0 and 1 per shot (+0x350 / +0x354, the hit); charge attack piece 0
model 0 at robo +0x1DCC and piece 1 model 0 at +0x1DD0; leg models 0, 1 (four copies) and 2 at robo +0x333C0 ..
+0x333D4. Each instance is its own joint tree and animation (`HSD_JObjLoadJoint` + `HSD_JObjAddAnimAll` per spawn).

Common effects (`efc_common.dbn`): u32 count (4) + offsets, 4 .dat files, `scene_data` each, kept at
0x80379970[0..3]: [0] robo +0x1E08 and [3] robo +0x333D8 (`cr_BattleRobo_InitFromLoadout`), [2] the gun updater
`fn_80044174`, [1] one model per index (`fn_80089550`).

## 11. Code status (Nov 6, 2026)

Matched to C (100%): `fn_80072A58` (collision fix-up) and `fn_80075D14` (particle banks) as hand drafts that join
the build in the next full run; `fn_80075950` (part effect file reads), `fn_80073728`, `fn_80073A74`,
`fn_80074FBC` (HSD archive parse). Drafts: the stage build `fn_80072064` (17%), `fn_80072B90`, the stage update
`fn_80070170`, the waypoint linker `fn_8006D4F8` (28%), the moving-collision transform `fn_800730F8` (97%), the
effect loaders `fn_8007571C`, `fn_80078000`, `fn_80077F38` and the spawners (section 10).

## 12. Not known yet

- Where the robos start and the battle camera per stage. The stage record's floats at +0x90..+0x11F (e.g. Basic
  Arena's pairs (-2, 1), (-2, -2), (1, 2), (2, 1), (-2, 2), (1, 2) at +0xF0) have no reader through the stage
  pointer; robos start from thrown cubes, so the start spots may be aimed in battle (not verified).
- What the collision surface numbers mean; the waypoint record's fields; the RPG test stage's collision layout
  (entry 015).
- The button that picks the alternate stages (10 and 11).
- The particle generator format (bank blocks 2-4), so particles cannot be edited yet.
- What each common effect looks like in game; what gun piece 0's model 3 is for; leg_effect's 5 extra entries (34
  entries for 29 legs); a few pod particle keys that decode to bank numbers above 7 (probably not real particle
  keys).
