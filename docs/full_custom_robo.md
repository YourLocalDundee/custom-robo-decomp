# A fully custom robo: every system it touches, traced (Nov 9, 2026)

Research for the mecha samurai (owner's request: "custom everything: custom guns, every single animation, not just
the model and the skeleton"). Each row names the code that proves it. The implementation follows this page
(`mods/mecha_samurai_full`).

## What makes up a robo on screen

| Piece | File / table | Indexed by | Loaded by / read by | Limit |
|---|---|---|---|---|
| Body model | `chara.BIN` entries 5b..5b+4 | body | part load requests `lbl_8023BA84`, loader `cr_ModelEntry` 0x800748A8 | 0x3A000 (1 MB with `code/big_body.txt`) |
| Gun / bomb / pod / legs models | `weapon_gun` / `_bom` / `_pod` / `_leg.BIN` | part number | same requests, loader `cr_WeaponModel_OnLoaded` 0x80073CB8 | 0x12000 / 0x12000 / 0x14000 / 0xC000 |
| Shots and hits | `gun_effect.BIN` | gun | `fn_80075950` -> `fn_8007571C` / `fn_80078000` | 0x16000 |
| Bomb / pod flight and blast | `bomb_effect.BIN`, `pod_effect.BIN` | bomb / pod | same | 0x40000 each |
| Leg jets | `leg_effect.BIN` | leg | same | 0x1000 |
| Charge attack model | `attack_effect.BIN` | **body** | same; drawn by `fn_800389C8` | 0x14000 |
| Body stats, charge attack timing | `lbl_801C6C44` 0x134 x 34 | body | `cr_BattleRobo_InitFromLoadout`, `fn_80046A68` | - |
| Gun / bomb / pod / leg stats | 0x801CA560 (0x28C x 64), 0x801D4860 (0x17C x 52), 0x801D9590 (0x174 x 57), 0x801C952C (0x8F x 29) | part | `cr_BattleRobo_InitFromLoadout` | - |
| Part mounts | `cr_g_BodyData` 0x801C5C10 (0x78 x 34) | body | `fn_80039FAC` | - |
| Battle motions | `cr_g_BattleMotionSets` 0x801C51B8 (38 u16 per set) -> motions in `logo_read_us.dbn` entry 8 | body | `cr_BattleRobo_UpdateAction` / `cr_Motion_Play` | float pool 16,384 per group |
| Motion lengths | `cr_g_MotionInfo` 0x80205C94 (header u16 & 0xFFF) | motion ID | `cr_Motion_LoadInfo` | - |
| Names / descriptions | `bmg/info_usa.bin` message file 9 / file 0 | running part index | `cr_Text_GetBodyName` .. `GetLegName`, `cr_Text_GetPartDescription` | - |
| Pictures | `sprite/parts_illustrated_book/*_tpl`, `sprite/ending/us/robo_tpl` | part / body | sprite request lists | - |

## Findings of this pass

1. **More than one texture per body or part: a one-word patch per draw call.** The robo draw loops set up one
   material and then walk the joints with `fn_80095E6C(jobj, mtx, mask, 0x04000000)`. `fn_80095E6C` is
   `HSD_JObjDispAll` with a render-mode argument passed to `HSD_JObjDisp`; 0x04000000 tells HSD's DObj display to skip
   each mesh's own material setup. With 0 there, every mesh sets up its own material (and textures), exactly as
   stages and effects are drawn. Call sites (`lis r6, 0x400` before each `bl fn_80095E6C`):
   `fn_80038CF8` (battle and Customize robo) 0x80039078 body (+0x860), 0x800390E0 gun (+0x944), 0x80039148 bomb
   (+0x948), 0x800391AC / 0x800391C8 pods (+0x950 / +0x94C), 0x8003922C / 0x80039248 / 0x80039264 / 0x80039280 legs;
   `fn_800A6068` 0x800A625C / 0x800A637C (the story-mode robo set); `fn_800BCC60` 0x800BCD34 (the chara loop);
   `fn_8003FB00` 0x8003FE64, `fn_800426D4` 0x80042940, `fn_800764D0` 0x80076AF4. Retail bodies and parts share one
   MObj in every DObj, so they draw the same either way. Side effect to watch: `fn_800943D8` changes the shared
   material before the draw (most likely the hit flash / fade); with several materials only the block B one gets it.
2. **Gun bytes (community field list, CRBR-Parts-Expansion `PartData/CRBR/Gun/*.asm`, matches the record split
   +0xA0 that our `cr_BattleRobo_InitFromLoadout` reading confirmed):** +0xA0 illegal flag, +0xA1 slide-shot
   duration, +0xA2 homing when not facing, **+0xA7 trail (0 none, 1 trail effect)**, +0xA8 / +0xAC / +0xB0 / +0xB4
   trail RGBA (left, middle, right, unknown), +0xB8 trail duration (u32, Basic Gun 60), +0xE0 trail width (float),
   then the ground shot and air shot blocks: startup, shots, rounds per shot, spread, delay, end lag, randomisation,
   angles, and four phases of {duration, speed, horizontal / vertical homing, shot size, linger, damage, down,
   knockback, hitstun, gravity}. This answers the community's trail question (hidden_content / WHERE_IS_EVERYTHING
   section 4): the trail is switched by +0xA7.
3. **The samurai's own 38 motions without changing any other robo.** 52 battle motions are never in a slot
   (`docs/data_unused_battle_motions.txt`), with the right lengths: 37 in group 0 (51-frame runs, 91-frame walks,
   141-frame victories) and 15 in group 1 (141-frame loses, 61-frame charge attacks, 11-frame change poses). Body 0's
   set uses 32 motions shared with other bodies (walks, runs, jumps, bomb / gun / pod poses, falls, change, get-up)
   and 4 of its own (lose, two victories, charge). Repointing body 0's set at unused IDs (with `cr_g_MotionInfo`
   lengths patched to the stock motion's) and writing the samurai's motions there leaves every other robo as it is.
4. **Parts: where the samurai's gear goes.** The menu lists (`cr_g_PartOrderLists` 0x8023B8BC -> body 0x8023B73A,
   gun 0x8023B780, bomb 0x8023B7EA, pod 0x8023B83E, legs 0x8023B898) are packed back to back, so adding the 49
   "Dummy" slots to a menu means moving the lists and hooking about 240 direct users (what CRBR-Parts-Expansion
   does). The samurai's gear goes into the parts Ray 01 starts with (Basic Gun 0, Standard Bomb 0, Standard Pod 0,
   Standard Legs 0) and their effect entries, stats and names: picking him shows all of it.
5. **Part models** (docs/formats/model_entry.md): several root joints per file, all PObjs rigid on their joint, an
   11-frame open / change animation set by frame = 10 x state. In battle: gun root 0 (root 1 at state 1.0), bomb root
   1, pod roots 0 + 1, legs roots 4 + 5 (shins) and 1 + 3 (ankles). The importer took one root only; `--root N`
   (this pass) imports over any root. The stock legs draw boots over the body's shins and the stock gun / bomb /
   pods sit on the arms and back: in the first big-body screenshots the blue boots, the yellow gun and the white pods
   were Ray 01's parts on the samurai.
6. **The part buffers can move like the body.** The same request table holds the part offsets (gun +0x3A000, bomb
   +0x4C000, pod +0x5E000, legs +0x72000 in each robo's 0x7E000 block); moving them into the expanded RAM lifts the
   0x12000 / 0x14000 / 0xC000 limits in Dolphin.
7. **Why the first big samurai looked bad**, from the Customize and battle screenshots: (a) armour plates that
   hang free (the skirt plates, shoulder plates) were weighted by position like skin, so the thigh and arm bones
   stretched them when the legs and arms moved; (b) the bake came out dark: no light-colour lift and the AI
   texture's own shading on top of the game's; (c) stock parts on top (finding 5).

## Order of the work

1. Draw patch (finding 1) + part buffers (finding 6) + body look (finding 7: plate-rigid weights, brighter bake,
   textures per section).
2. Gun, bomb, pod, legs: AI models fitted onto each stock root, one atlas each; effects (shot meshes and colours);
   stats (gun trail on, samurai colours).
3. Motions: 38 slots on unused IDs; victories, lose, charge attack authored for him.
4. Names and descriptions (BMG writer), pictures.

## What was built (Nov 9, 2026; `mods/mecha_samurai_full`, Dolphin MEM1 64 MB)

Every piece has a script in the mod folder; run them in this order, then `python -I tools/make_mod.py mods/mecha_samurai_full`.

| Script | Makes | Notes |
|---|---|---|
| `make_code.py` | `code/samurai.txt` | 3 MB slot per robo at 0x81A00000 + r x 0x300000: body 2 MB, gun / bomb / pod / legs 256 KB each; Config48MB; the per-mesh material patch (12 draw calls) |
| `make_body.py <body.glb> --tris=18000 --mirror=1 --sections=1 --plates=1 --look=game --detail=0.6 --ss=2 --head=<head.glb> --head_share=0.38` | `models/chara/chara_000..004.dat` (1,836,239 bytes) | four textures (torso + arms 1024x1024, legs 512x1024, head 512x512, hands / feet 512x512), plate weights, the game-lit bake (`tools/blender/atlas_ai_body.py`) |
| `make_parts.py` | `models/weapon_gun|bom|pod|leg/*_000.dat` | AI parts (FLUX 3 Klein from the samurai picture, then TRELLIS.2) fitted root by root (`tools/blender/ai_part.py`: axes by length, thick end to the mount end, voxel remesh, decimate, bake); `HSDRawViewer --import --root N --replace --relayout` (new option `--root`, byte-identical round trip of both gun roots) |
| `model_loop.ps1 -Script effect_recolor -Category gun_effect|bomb_effect|pod_effect|attack_effect -Entries 000_0,000_1` | `models/*_effect/*_000_*.dat` | textures and colours remapped to dark red / cyan / gold (`tools/blender/effect_recolor.py`); sizes unchanged |
| `make_stats.py` | `code/stats.txt` | gun 0: trail on, cyan / gold trail, two rounds side by side (+-8 degrees) on the ground and air blocks (+0xDC, +0x1B4); body 0 charge: homing 25, durations 12 / 16 / 28, speeds 0 / 320 / 40 |
| `make_motion.py` | `files/motion.DBN`, `code/motion.txt` | body 0's 38 slots on its own motions: 31 shared motions copied into unused IDs (lengths set in cr_g_MotionInfo), all restyled (low wide stance, off hand on the hilt), new victories (fist raised, bow), kneeling lose, iai charge (draw, lunge with a sweep, follow-through); 283 + 40 tracks, group pools 14,929 / 10,827 floats |
| `make_text.py` | `files/bmg/info_usa.bin` | Samurai, Tanegashima, Horoku Bomb, Sashimono Pod, Kaze Legs and their descriptions; same file size |

Headless-helmet fix (Nov 9): the first build showed an empty helmet. The AI head is open shells (not watertight)
and the voxel remesh of the head's low copy dropped the helmet bowl and holed the face; `atlas_ai_body.py` now
solidifies the close-up (0.005, `--head_solid`) before the remesh, and the head gets 38% of 18,000 triangles.

Found on the way: the stock legs part is a calf thruster pack (roots 0 / 2 / 4 / 5) and an ankle clamp (roots 1 / 3),
not a shin guard; Basic Gun's developer label is "Striker Gun"; the gun record's air-shot block is at +0xB4 past the
ground block's start (+0x1B4), not at +0x18C as the community field list's sizes suggest (a fifth 0x28-byte block, or
padding, sits between).

## First in-game look and what was wrong (Nov 11, 2026)

The owner's Customize screenshot of the B3 build showed the samurai crushed into a heap with two tall red tubes over
him. The T-pose preview had hidden all of it. Three new tools, then the fixes:

- `tools/blender/robo_preview.py`: the robo as the game puts it together: the body posed by a game motion; gun root
  0, bomb root 1 (0 with `--form0`, the Customize set), pod roots 0 + 1, leg roots 4 / 1 / 5 / 3 (0 / 2 in form 0), each
  on its role joint with the body's mount offset and the 0.1 scale of `fn_80039FAC`. Checked on stock Ray 01 with
  stock parts. **Attachment check**: every part must touch its own region of the body (gun the right hand, bomb the
  left forearm, pods the chest joint, leg packs the shins) and a held part must have the hand ON its grip (30% along
  its long axis from the elbow end, within 3 cm); loose body pieces are reported; for anything floating it prints the
  shift (in the part file's frame) that fixes it.
- `tools/motion_strip.py`: that preview over the motions the mod plays (8 motions x 4 frames), flagged frames framed in
  red, a `.check.txt` beside the picture. **Nothing goes into the ISO before this passes** (owner, Nov 11).
- `tools/blender/part_frame_probe.py`: marker points sent through HSDRawViewer's import + export, which measures what
  the importer does to a part mesh: it turns it by the root joint's rotation (JOBJ_0; measured exactly, error 0, on the
  gun and the pod), nothing else.

Fixes:
1. **Body weights** (`atlas_ai_body.py`, seen in a bone-coloured posed render): the head rule had no side limit, so the
   tops of the T-posed arms followed the chest and tore into flat sheets to the elbows when the arms dropped; the leg
   rule wanted every vertex 1.5 cm off the centre line at every height, so the inner faces of legs standing close
   together followed the hips down to the ankles and tore when the legs spread; and, after the first fix, the helmet
   horns (wider than the shoulders) followed the shoulders. All three fixed: B5. `--rigid` (every face on one joint)
   and `--rigid=plates` were tried and are worse on this model (slivers, grey bake on the split arms); kept as options.
2. **Parts** (`ai_part.py`): fitted to the stock root's raw vertex data (not where the game draws it: the gun 1.4x,
   pods 1.9x, bomb 1.25x too big, the two "tubes" were the pods); now fitted to the drawn stock points, the mesh
   written with the root joint's turn undone (measured by the probe), `--long=x` stands the Sashimono poles up,
   `--shift` moves a part by what the attachment check measured (pods seated 5.5 cm into the back, leg packs onto the
   shins, gun and bomb so the hand sits on the grip).

Result (B5 + these parts, installed Nov 11): 31 of 32 strip frames pass; the one flag is the last frame of victory 1
(fist raised, the wrist turned: the gun 2.5 cm off the hand, the stock guns ride the forearm the same way). Strip and
check: `build/mecha_samurai_full/B5/strip_B5final.png` / `.check.txt`.

### The Customize freeze (Nov 11, 2026)

Dolphin froze on the Customize screen as soon as the robo appeared. Read live from the emulator: PC in `PPCHalt` via
`HSD_Panic` <- `__assert` ("jobj.h" line 899, "jobj") <- `fn_80079214` <- `fn_8005C7F4`, which is motion tag 6 of the
frame-event table `lbl_801F9DDC` (spawns a battle effect from robo+0x333C0, which is never loaded in Customize).
Cause: `make_motion.py` copied the shared motions into unused IDs and patched only each copy's length, so the copies
kept the unused motions' own tag lists. Slot 0 (idle) became ID 2 `sf_back_run_guard`, which has tag 6 at frame 25.
Fix: each copy's `cr_g_MotionInfo` pointer now points at the stock motion's tag list (length + events), and no
tag-list header is patched any more (headers are shared between IDs, so the old patches changed other bodies' lengths too).

### Parts placed from the game's own joints (Nov 12, 2026)

The previews passed builds whose gun floated in the game. `tools/dolphin_capture.py` now saves the Dolphin window's
frames together with both robos' bone matrices (robo +0x1434) and every part joint's world matrix (the JObj trees at
robo +0x944 gun, +0x948 bomb, +0x94C / +0x950 pods, +0x954..+0x960 legs; JObj +0x44), and
`robo_preview.py --gamemats=<json> --gameparts=<json>` poses body and parts from exactly those. What it showed:
- `fn_80039FAC` builds each part root as role matrix x a per-part turn (gun: robo +0x180C, bomb +0x1824, pods
  +0x183C / +0x1848, shins +0x1860) x 0.1 x the mount offset, and `fn_80095700` sets each part's own animation frame
  from a state value; the old preview ignored both.
- The game swaps roots by state: battle gun = root 1 (6 joints), Customize gun = root 0 (21); pods = roots 2 / 3
  (4 joints), roots 0 / 1 (13) while firing; bomb = root 1 and legs = roots 4 / 1 / 5 / 3 in battle and Customize.
- In the gun mesh joint's space the forearm runs along +x (elbow 0.2, wrist 1.9, y 0.1, z 0; `part_space.py`);
  the fitted matchlock's grip sat 2.4 below it. The bomb sat past the elbow (the left forearm runs along -x); the
  leg packs bulged 1-3.5 behind the shin. Moved in `make_parts.py`; pods emptied (owner: no back poles).

Body (atlas_ai_body.py `--skirt_centre`, `--neck_up`): the front / back centre skirt flaps hang from the hips (thick
thighs made the plate test treat them as thigh, so they lifted into a sheet between spread knees), the chest / head
blend is raised into the helmet. Motions: victory 1 = a Heisman pose, victory 2 = look around and dust off both
shoulders (stock win motion held at its first frame underneath), air dash = forward lean with the arms swept back,
get-up = a quick look (its 21 frames are the game's), gun hand held out from the skirt, chest lean 4 degrees.
Jump sparks (`particles.json` "tune"): leg 0's jet generators bigger, longer-lived, wider, a gold glow.
