# Mecha samurai: where we are and what is left (handoff, Nov 12, 2026)

Read this first when picking the samurai back up. Background and findings: [full_custom_robo.md](full_custom_robo.md).

## Where things are

| What | Where |
|---|---|
| ISO to play | `build/mods/mecha_samurai_full/mecha_samurai_full.iso`, SHA-1 `0a6007cdacffb9c4f60172829abafa60c4a1859a` |
| Patch to share | `mods/mecha_samurai_full/mecha_samurai_full.crpatch` (checked: applied to the clean `Custom Robo (USA).iso` it gives that SHA-1) |
| Last commit | see `git log` on `main` (pushed) |
| Body | B18 (B17 + gold-capped hinge balls): **the plate rebuild** - rigid armour pieces on ball joints, as the game's own robos (`tools/blender/plate_body.py` on `build/reweight/pruned5.glb` -> `build/reweight/plated8.glb` -> `build/mecha_samurai_full/B18/`, installed in `mods/mecha_samurai_full/models/chara/`) |
| Stretch / clipping check | `tools/deform_sweep.py` + `tools/blender/deform_check.py` (reports and 6-angle heat renders in `build/deform/<name>/`) |
| Parts | `mods/mecha_samurai_full/make_parts.py` (gun + bomb, black lacquer and gold: `parts_palette.json`; pods and legs empty) |
| Motions | every slot his own: `mods/mecha_samurai_full/samurai_moves.py` (stand, walk/run, air, falls, actions) + `make_motion.py` (victories, lose, charge, get-up; floor lift from `floor_lift.json`) |
| Gun / charge stats | `mods/mecha_samurai_full/make_stats.py` |
| Jump sparks | `mods/mecha_samurai_full/particles.json` (`tools/ptcl.py` "tune") |
| Game captures | `build/capture/session*/` (frames + `mats_*.json`; not in git) |

Dolphin: `D:\emulators\Project Hive 3.2 [Dolphin Precon 2609a]\Dolphin.exe`. Close it before loading a new ISO.
Screenshot key in that build is **1** (not F9; Shift+F9 is a breakpoint).

## Watching edits live (owner, Nov 12: much higher recording rates, live, not after the fact)

- `tools/blender/motion_film.py <body.glb> <motion.glb> <out dir>`: the body through a motion **every frame**, 4 views
  (front, side, back, 3/4 top), triangles coloured by stretch against the rest pose (red > 1.6, yellow > 1.3, blue
  squash). Each frame's tile (`f_0000.png` ...) and `progress.txt` are written the moment it is rendered; `sheet.png`
  and `film.mp4` at the end. The motion glb must be baked against that body (any `deform_sweep.py` run on it writes
  `<slot>.motion.glb`).
- `tools/blender/watch.py`: the snapshot helper; `rebind.py --watch=<dir>` snaps the bind before, the posed mesh before
  the relax and every 20 relax iterations, heat-coloured.
- `tools/deform_sweep.py` checks every frame by default (`--step 1`).
- Dolphin: capture faster than the old 0.33 s (`tools/dolphin_capture.py <dir> N 0.1`).

## The current build

- **Nov 12, for the owner's play session: body B18 (ISO `0a6007cd…`, not yet played).** B17 plus:
  - hinge styling: each ball is a latitude / longitude sphere with its poles on the hinge axis (side to side for
    hips / knees / ankles, front to back for shoulders / elbows / wrists) and gold discs on the poles, from the
    body's own gold texel - bolt heads, as on Ray's joints (an icosphere gave jagged stars)
  - `plate_body.py --skirt=<f>` (the skirt keeps f of its thigh weights): measured and left off (stretch +46%,
    squash x4.5, clipping unchanged - the sweep's clipping is the upper arms on the chest, not legs in the skirt)
  - still rough: dark under-suit shards around the shoulder balls (the AI mesh's armpit); plain spheres otherwise
  - build: `plate_body.py pruned5.glb roles plated8.glb <stance motion baked on pruned5> --ball_max=0.055
    --ball_pct=8 --ankle=0.042 --wrist=0.034`, then `make_body.py x --reuse=plated8.glb --weight_step=0.05 --bind=glb`

- **Nov 12, late: the plate rebuild, body B17 (ISO `3afb03a5…`, not yet played).** Owner: rebuild the armour as
  separate overlapping systems, learn from the game how it does it, whatever architecture it takes.
  - **How the game builds a robo** (Ray 01, chara_000.dat, measured): 18 meshes, 17 of them 100% rigid, one per
    joint; 96% of the vertices follow one joint. Only a 210-triangle connector is skinned (waist, neck, hip and
    shoulder sockets, weights 0.4 / 0.6). Elbows, knees, wrists and ankles have no skin: rigid shells around
    mechanical hinge discs. 2,465 triangles. On our sweep (its own motions): stance 456 stretched / 0 squashed,
    run 412 / 0, gun_l90 209 / 0, fall_back 457 / 89, victory1 965 / 57.
  - **`tools/blender/plate_body.py`** does the same to the samurai, from the clean bind (pruned5, not the rebound
    mesh: rigid pieces need no rebind, and the rebound skirt was frozen bent):
    1. 12 limb pieces (arm upper / fore / hand, leg thigh / shin / ankle per side) + the trunk, by each triangle's
       majority joint; the mesh cut along the boundaries
    2. limb pieces 100% rigid; the trunk keeps its blend among chest / hips / head only (waist, neck)
    3. funnels: every cut vertex is pulled onto its joint's ball (hips: only those near it), so each shell tapers
       into the ball and both rims ride one sphere about the pivot - no shards, no gaps
    4. 88 bridge triangles between skirt and thigh plates (no joint there) removed
    5. 12 ball joints (icospheres, the atlas's darkest texel) at shoulders, elbows, wrists, hips, knees, ankles, rigid
       on the parent joint. Pivots measured from a motion: shoulders / elbows / hips / knees are exactly the bone
       heads (drift 0.0000 m); wrists / ankles are points of the forearm / shin (the game places hands and feet by
       its own offsets, 0.4-0.9 m from Ray's bind positions)
  - **Result** (glb, 20 motions, step 2, against B14): stretch 38.6k -> 8.1k (-79%), squash 53.0k -> 4.4k (-92%),
    clipping 4.0k -> 5.7k (rigid plates overlap by design; the stock robo scores clipping too). Per motion it is at
    or under the stock robo: stance 333, run 314, gun_l90 151, fall_back 0, victory1 1,033. The game file is
    smaller (1.71 MB). Floor lift redone; strips pass; films `build/film/tex17/` and `samurai_B17_plated_reel.mp4`.
  - Tried on the way: caps over the cut ends (starbursts where a cut is not a clean ring: off), big balls sized
    from the armour rim (cannonballs: now the inner core, 3.4-5.5 cm), the rebound mesh as the base (spiky skirt).
  - Open: the balls are plain dark spheres (no hinge detail, exposed on top of the shoulders); thighs pass
    through the rigid skirt when a knee comes up high; needs a look in the game itself.

- **Nov 12, late night: pose fixes from the films (ISO `fb9ee223…`, body still B14, not yet played).** Watching
  victory1 every frame showed three red spots; the fixes that measured better are pose-level:
  - Heisman: stiff arm 0.38 -> 0.28 above the hips (still shoulder-high), the tucked gun lower and further out, the
    knee 0.24 -> 0.16: stretch -18%, squash -16%, clipping -39%.
  - Dust-off: the brushing hand a little forward and lower: clipping -19%.
  - All 20 motions (glb, step 2): stretch 40.4k -> 38.6k, squash 57.0k -> 53.0k, clipping 4.4k -> 4.0k.
  Structural ideas measured this round and NOT kept (all in the repo, switched off):
  - `span_weights.py` (no triangle across non-neighbour joints): both-sides rule +25% stretch, skipped-joint rule +2%.
  - A rest pose weighted to the stance: stance -46% but walk / run + the same amount (a straight trade).
  - Shoulder (clavicle) share, `make_motion.py` SHOULDER_SHARE + `shoulder_weights.py`: squash -38% but stretch +55%
    and clipping up; Ray's shoulder joint pivots at the neck base, so it shrugs the whole armpit into the chest.
  - Fully rigid weights: squash -82%, stretch x3 (the triangles between parts tear). Rigid only works on a model
    built as separate overlapping plates, as the stock robos are.
  - The inner-thigh red in the stance is a few big triangles under the front skirt on the wide-planted leg.

- **Nov 12, night: body B14 (ISO `c96bb583…`, not yet played).** Rebind at 100% of the motions' mean turn (was
  70%): stretch -32%, clipping -11% against B12 (glb measure below). Findings:
  - **Measure on the glb, not the game file.** The game file deforms the same within 4 mm (median 0.4 mm), but
    it splits the mesh (35k vertices for 15k) and rounds weights to 0.05, which inflates the sweep's counts 2-3x on
    tiny triangles. Use `tools/deform_sweep.py build/reweight/<body>.glb ...` for comparisons.
  - **Motion glbs must be baked against the body they are applied to** (rest pose): `rebind.py` needs motions
    baked against the un-rebound body (`build/deform/glb_pruned/`), the checks against the body under test.
  - Tried and dropped (measured): untwisting the upper arms per frame (`make_motion.py` UNTWIST, off: the twist
    went 73 -> 1 degree but the squash stayed and the gun arm's elbow moved worse, stretch +13%); slimming the arms
    toward their bones (`slim_arms.py`: the upper-arm axis runs inside the wide chest, so clipping doubled); a
    deeper kneel (more stretch).
  - Group 1 / 2 motion copies: every group of the boot motion file is registered at boot, so a 78-track copy can
    live in an unused 78-track ID of group 1 (`MOVE_SLOTS`, empty now) when group 0's float pool is full.
  - Left (glb measure, 20 motions): stretch 40k / squash 57k / clipping 4.4k vertex-frames, two thirds of it in the
    victories, lose and charge (extreme arm raises: the Heisman's off arm 124 degrees from the bind). Battle motions
    are under 1% of triangles per frame.

- **Nov 12, evening: stretch -64%, squash -70% (body B12, ISO `a842cb83…`, not yet played).** Sweep of all 20 motions
  (`build/deform/B12_final/summary.txt` vs `B10_final`): stretched triangles 265.7k -> 96.4k, squashed 226.8k -> 66.9k,
  clipping 11.7k -> 13.2k (fall_front's arms, victory2's shoulder brush). What was found and fixed:
  - **Rebind** (`tools/blender/rebind.py`): the AI body was bound arms-out with straight elbows and knees, but in the
    motions the upper arms were never within 48 degrees of that (elbows ~95, knees 70-86): every blended band was
    always deformed. The body is now bound at 70% of the motions' mean turn per joint (upper arms, elbows, hips,
    thighs, knees; ankles excluded, their turns are two-sided), with the blended bands relaxed back to their own edge
    lengths in that pose. Safe in the game: `cr_Battle_ApplyRoleMatrices` (0x800392B0) copies each computed role
    matrix into the joint (jobj+0x44, flags |= 0x03800000), so a joint's rest rotation only enters through its
    inverse bind matrix. Import: `HSDRawViewer --import ... --skeleton glb --bind glb` (new) via
    `make_body.py --bind=glb`.
  - **Weights along the skeleton** (`reweight.py --roles=`): the smoothing had spread weights across mesh edges, so
    the skirt took both legs and the collar both arms and the head (1,899 points pruned).
  - **Falls lay straight:** the chest tilt had the opposite sign of the game's own body lay (joint0.rotx), so he
    folded in half (hips 75-100 degrees off the chest, thighs 110-150); it now follows joint0. Fall stretch -68%.
  - Tools: `deform_sweep.py` / `motion_strip.py` bake each motion against the body under test (Blender keeps an
    action relative to its import skeleton's rest, so a rebound body needs its own); hand clearance 0.28 -> 0.32.
  - Tried and dropped: carving the chest again on the rebound body made clipping worse (stance 185 -> 241).

- **Nov 12 late: all motions his own, body B10, bomb fixed** (ISO `3db68e0e…`, not yet played):
  - No Ray 01 motion is played any more: every slot is generated by `samurai_moves.py` / `make_motion.py`.
  - Floor: `tools/floor_lift.py` measures where the armour goes under the floor in a deform sweep and
    `make_motion.py` lifts the whole body by that much per frame (the root's world height, `root.posy`, in the
    78-track motions). Falls, the kneel and the charge now stay within 3 cm of the floor (were up to 17 cm under).
    Rebuild: sweep without `floor_lift.json`, `floor_lift.py <sweep> floor_lift.json --slots=...`, rebuild, sweep,
    `--prev` once more.
  - Blender frames now equal game frames: `deform_check.py`, `robo_preview.py` and `carve_torso.py` set the scene to
    30 fps before importing (the default 24 fps put sweep frame x at motion frame 1.25x; old sweeps: `--scale=1.25`).
  - Charge grab and Heisman tuck moved off the chest (charge clipping 2.9k -> 1.1k vertices).
  - Bomb: the old -3 x shift had put the drum behind the upper arm, off the far side of the elbow. Now
    `--shift=-0.05,0.155,-1.03`: on the middle of the forearm in every strip frame. Captures show the bomb's joints
    sit in their rest pose under the root in most battle frames; `robo_preview.py --partpose=<capture>` (and
    `motion_strip.py --partpose`) poses the parts from a captured frame when needed.
  - Sweep of B10 (step 2, full length): `build/deform/B10_final/summary.txt`. Strips:
    all 13 motions, attachment check ok (one 3-vertex sliver, 2 cm, victory2 frame 105).

- **Freeze fixed:** the Customize freeze (a motion event spawning a battle-only effect) is gone. Tested on Nov 11:
  three trips through Customize and two battles with no stall.
- **Body B7s:** the front and back skirt flaps hang from the belt, the head / chest blend sits inside the helmet,
  the chest lean is 4 degrees. Its skin weights are smoothed where joints share them (`tools/blender/reweight.py`
  30,0.5,4) and written in 0.05 steps (`HSDRawViewer --weight-step 0.05`; the game's own bodies use 0.1).
  Measured on the game file with `tools/deform_sweep.py` (every 3rd frame of 20 motions, stretched = an edge over
  1.6x its rest length): stance 21.6k -> 7.8k stretched triangles, walk 20.9k -> 7.2k, run 20.6k -> 9.4k,
  victory 1 66k -> 39k, lose 59k -> 31k.
- **Chest sides carved for the arms** (`tools/blender/carve_torso.py`): every arm point of every motion mapped into
  the chest's rest space; where it was inside the chest, the chest's sides / armpits give way (at most 3.5 cm, never
  the front, collar or back). Game-file clipping: victory 1 8.2k -> 4.1k vertices, victory 2 11.6k -> 8.1k,
  charge 11.8k -> 8.7k, gun turn 5.4k -> 3.8k.
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
   smoothed; chest sides carved; hand clearance; victory / lose / charge hands moved off the chest and thighs.
   **Left in the numbers:** lose and the falls (his whole body folds on the ground; the stock motions, made for
   Ray 01, bend him through his own skirt) and some upper-arm / shoulder-plate overlap in victory 2 and the charge.
   Next lever if the owner still sees it: slimmer shoulder plates (sode) in the model itself.
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
   victory 2. A longer get-up would also make him stand up slower in battle (more time open to hits), so it is a
   gameplay change, not just a look: only with the owner's OK, and it needs the state code that times it (see 5).
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
then the chest carve (motion glbs from a deform_sweep.py run, e.g. build/deform/B7c_all/*.motion.glb):
blender -b --factory-startup --python tools/blender/carve_torso.py -- build/mecha_samurai_full/B8/samurai.reweighted.glb ^
  build/mecha_samurai_full/B5/roles_000.json build/reweight/carved.glb <motion glbs...> --step=4
python -I mods/mecha_samurai_full/make_body.py x --reuse=build/reweight/carved.glb --weight_step=0.05 --work=B8c --dst=...
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
