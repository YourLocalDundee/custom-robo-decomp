# Where everything is: animations, stages, effects (the short version)

The plain-language map. For the byte-level details see [ANIMATION_GUIDE.md](ANIMATION_GUIDE.md) (animations) and
[SCENE_GUIDE.md](SCENE_GUIDE.md) (stages, effects, particles). All file names are under the disc's `files/` folder
(`Custom Robo extracted/files/` after the one-time build).

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

The US names are messages in `bmg/info_usa.bin` (message block 24, message = stage id).

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
| 7 | RPG Test | hidden | 014 / 015 | 1 | 0 | 3 | no | other layout | - | - | - |
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
   `python -I tools/scene.py import stage 0 --model 3 edited.glb --mod MYMOD`.
3. If you added something robos should bump into, add its faces to the collision OBJ (in the group of the model it
   stands on), then `python -I tools/stage_collision.py import 0 edited.obj --mod MYMOD`.
4. Build: `python -I tools/make_mod.py mods/MYMOD` (an ISO and a patch to share).

### Not known yet

Where the robos start (they are thrown in their cubes, so the start spots may be aimed in battle), the battle
camera's settings per stage, what the surface numbers mean, and the waypoint record's fields.

## 4. Shots, blasts, charge attacks (effects)

Each gun, bomb and pod has its own effect entry (same number as the part); charge attacks are by **body** number.
Every entry holds two model files:

| Piece | What |
|---|---|
| **0** | what flies: the shot or blast meshes, one copy per shot |
| **1** | what happens on hit: particle emitters (guns) or the explosion meshes (bombs, pods, charge attacks) |

**Find it:**

```
python -I tools/scene.py list effects --kind gun   # per gun: models, meshes, particle tracks of both pieces
python -I tools/scene.py info effect gun 12        # both pieces of gun 12's effect
python -I tools/scene.py export effect bomb 3      # -> build/scene/bomb_effect_003/
```

Ready-made exports of everything are in `models_export/` (made by the one-time build): `models_export/chara/`,
`bg_data/`, `gun_effect/`, ... one folder per entry.

## 5. One table: what to open for what

| I want ... | Open |
|---|---|
| a robo's victory / charge / walk animation | `tools/anim.py export --body N --slot NAME` |
| which motion a robo uses for something | `tools/anim.py slots --body N` |
| a stage's models and their animations | `tools/scene.py export stage S` (or `models_export/bg_data/bg_data_<2S>/`) |
| a stage's collision | `tools/stage_collision.py export S` |
| a gun's shot | `tools/scene.py export effect gun N` |
| a part's open / change animation | `models_export/weapon_gun/` (and `weapon_bom`, `weapon_pod`, `weapon_leg`) |
| a transforming body's change | `models_export/chara/chara_<5N>/` |
| the particle banks | `logo_read_us.dbn` entry 0 (format: [SCENE_GUIDE.md](SCENE_GUIDE.md) section 4) |
