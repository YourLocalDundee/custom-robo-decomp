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

A stage with id s is two entries of `bg_data.BIN`:

| Entry | What |
|---|---|
| **2s** | the look: up to 20 models (each can carry its own looping animation), lights, fog |
| **2s + 1** | the walls and floors robos collide with: one group of flat four-cornered faces per model, plus a waypoint list (most likely for computer players) |

A model's collision group also says whether that model is **breakable** (and how many hit points: 100 to 400) or
**moving** (its collision follows its animation).

**Find it:**

```
python -I tools/scene.py list stages            # id, name, entries, how many models / collision groups
python -I tools/scene.py info stage 0           # every model, its animation (length, what it moves), lights, fog, collision
python -I tools/scene.py export stage 0         # -> build/scene/stage_000/: one .glb per model + the collision .obj
python -I tools/stage_collision.py info 0       # collision groups, breakable / moving flags
```

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
