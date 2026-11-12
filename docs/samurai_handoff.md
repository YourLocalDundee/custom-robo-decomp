# Mecha samurai: where we are and what is left (handoff, Nov 12, 2026)

Read this first when picking the samurai back up. Background and findings: [full_custom_robo.md](full_custom_robo.md).

## Where things are

| What | Where |
|---|---|
| ISO to play | `build/mods/mecha_samurai_full/mecha_samurai_full.iso`, SHA-1 `c4531dd1bc95defa36709107e45237e191aa44d9` |
| Patch to share | `mods/mecha_samurai_full/mecha_samurai_full.crpatch` (checked: applied to the clean `Custom Robo (USA).iso` it gives that SHA-1) |
| Last commit | see `git log` on `main` (pushed) |
| Body | B7s: B7's mesh with smoothed weights (`build/mecha_samurai_full/B7s/`, installed in `mods/mecha_samurai_full/models/chara/`) |
| Stretch / clipping check | `tools/deform_sweep.py` + `tools/blender/deform_check.py` (reports and 6-angle heat renders in `build/deform/<name>/`) |
| Parts | `mods/mecha_samurai_full/make_parts.py` (gun + bomb, black lacquer and gold: `parts_palette.json`; pods and legs empty) |
| Motions | `mods/mecha_samurai_full/make_motion.py` |
| Gun / charge stats | `mods/mecha_samurai_full/make_stats.py` |
| Jump sparks | `mods/mecha_samurai_full/particles.json` (`tools/ptcl.py` "tune") |
| Game captures | `build/capture/session*/` (frames + `mats_*.json`; not in git) |

Dolphin: `D:\emulators\Project Hive 3.2 [Dolphin Precon 2609a]\Dolphin.exe`. Close it before loading a new ISO.
Screenshot key in that build is **1** (not F9; Shift+F9 is a breakpoint).

## The current build

- **Freeze fixed:** the Customize freeze (a motion event spawning a battle-only effect) is gone. Tested on Nov 11:
  three trips through Customize and two battles with no stall.
- **Body B7s:** the front and back skirt flaps hang from the belt, the head / chest blend sits inside the helmet,
  the chest lean is 4 degrees. Its skin weights are smoothed where joints share them (`tools/blender/reweight.py`
  30,0.5,4) and written in 0.05 steps (`HSDRawViewer --weight-step 0.05`; the game's own bodies use 0.1).
  Measured on the game file with `tools/deform_sweep.py` (every 3rd frame of 20 motions, stretched = an edge over
  1.6x its rest length): stance 21.6k -> 7.8k stretched triangles, walk 20.9k -> 7.2k, run 20.6k -> 9.4k,
  victory 1 66k -> 39k, lose 59k -> 31k.
- **Motions keep the hands off the body:** each hand stays 28 cm from his centre line (the stock motions were made
  for Ray 01's slim chest): walk clipping 163 -> 16 vertices, walk_weapon 550 -> 3.
- **Gun:** grip in the right fist (placed from the game's own joint positions), one barrel, black and gold.
- **Bomb:** on the left forearm, black and gold.
- **Removed:** back poles (pods) and calf packs (legs), as asked. They are still equipped and still fire.
- **Motions:**
  - victory 1: a Heisman pose
  - victory 2: looks around, dusts off both shoulders, chin up
  - air dash: forward lean with the arms swept back
  - get-up: a quick head check
  - gun hand: held out from the skirt
- **Effects:** gun trail 42 frames (was 60). Jump sparks bigger, longer-lived and wider, with a gold glow.

## What the owner needs to do next

1. Close Dolphin, open the ISO above, and play one round:
   - Customize screen
   - shoot and jump
   - air dash
   - get knocked down and get up
   - win, to see the victory pose
2. While playing, have the capture running so Claude can check frame by frame against its previews:
   `python -I tools/dolphin_capture.py build/capture/<name> 10800 0.33` (3 frames a second, owner Nov 12; Claude starts it; it waits for Dolphin).
3. Say what looks wrong. Especially:
   - do the new victories read right?
   - is the air dash better?
   - do the sparks have enough pizzazz?
   - does the right hand still clip the leg?
4. Pick 2-3 real football celebrations for future victory poses if you want more than the Heisman (poses are
   hand-keyed from reference; no animation data from other games).

## What Claude still has to do (in order)

0. **Done Nov 12 (owner away):** capture every 0.33 s; the stretch / clipping sweep over all motions; weights
   smoothed (B7s); hand clearance; victory / lose / charge hands moved off the chest and thighs. **Left:** the upper
   arms with their big shoulder plates (sode) still slide into the chest armour when the arms come in (victory 1,
   charge, lose, falls: almost all remaining "clipping" is arm_*_upper). The arms hang from Ray 01's narrow
   shoulder joints inside a bulky chest; options: slimmer chest / sode in the model, or a wider shoulder
   (needs the skeleton's shoulder offset, which the game's IK reads).

1. **Check the new build in a capture**:
   - the parts must stay attached in every motion: `robo_preview.py --gamemats=<mats> --gameparts=<mats>`
     against the captured frames
   - the Customize gun (root 0, 21 joints) has not been seen in a clean frame yet
2. **Side skirt panels**: they stretch slightly where they meet the hanging centre flap. They could hang
   straighter, but then they would clip the thighs when a knee comes up; judge in game first.
3. **New motions in game**: victory 1 / 2, air dash, get-up and the gun hand were only seen in
   `tools/motion_strip.py`. That tool places parts the old way (role + mount), so it cannot judge attachment.
4. **motion_strip.py**: teach it the game's part placement (per-part turn robo +0x180C ..., the part's own
   animation frame via `fn_80095700`) so previews of motions never played yet are exact too. Today only
   captured frames give exact parts.
5. **Decomp, animation side** (owner asked for it):
   - finish the motion player / key sampler / arm and leg IK drafts (docs/ANIMATION_GUIDE.md section 9)
   - the code that sets each part's animation state (`fn_80095700` callers: robo +0x20D4 gun, +0x28EF0 bomb,
     +0x2E470 pods, +0x333B4 legs)
6. **Get-up "dust off"**: the get-up motion is only 21 frames (the game's), so the dust-off went into
   victory 2. A longer get-up needs the state code that times it (see 5).
7. **Jump sparks**: they share generators with legs 16-33 (no free generator big enough for a private
   copy). A private copy needs a bigger particle bank (relayout of `logo_read_us.dbn` entry 0).

## How to rebuild

```
python -I mods/mecha_samurai_full/make_parts.py [gun bomb pod legs]
python -I mods/mecha_samurai_full/make_motion.py
python -I mods/mecha_samurai_full/make_stats.py
python -I mods/mecha_samurai_full/make_body.py "D:\ai\Pictures\mecha_samurai_v3\body_3d_00001_.glb" ^
  --region_tris=head:3000,torso:3000,arms:3000,legs:3000 --mirror=1 --sections=1 --uv=seams --uv_regions=head:smart ^
  --seams=45,60,75,90 --gap=4 --plates=1 --look=paint --detail=0.6 --ss=2 ^
  "--head=D:\ai\Pictures\mecha_samurai_v3\head_3d_00001_.glb" --cull=1 --teximp=1 --edges=0.5 ^
  --skirt_centre=0.12 --neck_up=0.04 --reweight=30,0.5,4 --weight_step=0.05 --work=B8 --dst=build/mecha_samurai_full/B8/dat
  (weights only, from an earlier build's glb: --reuse=build/mecha_samurai_full/B7/samurai.glb)
  (then copy build/mecha_samurai_full/B8/dat/chara_00*.dat into mods/mecha_samurai_full/models/chara/)
python -I tools/make_mod.py mods/mecha_samurai_full        -> ISO + .crpatch
```

Stretch / clipping sweep (on the game file: export it first, `HSDRawViewer --export <dir> glb chara_000.dat`):

```
python -I tools/deform_sweep.py <dir>/chara_000/root0_joint.glb build/mecha_samurai_full/B5/roles_000.json build/deform/<name> --mod mecha_samurai_full --step 3
```

Check before any ISO (memory rule): a preview against captured game frames, by eye:

```
blender -b --factory-startup --python tools/blender/robo_preview.py -- <body.glb> <any motion glb> <parts export dir> <out>
  --roles=build/mecha_samurai_full/B5/roles_000.json --gamemats=<mats json> --gameparts=<mats json>
```

Parts export dir: `HSDRawViewer.exe --export <dir> glb mods/mecha_samurai_full/models/<cat>/<cat>_000.dat`.
