# The stock robos' look, measured, and the painted look for AI models (Nov 10, 2026)

Owner's question: the AI samurai never looked like a game robo, even split into four textures; was the source simply
too high resolution? Answer: the resolution was not the problem, the kind of texture was. Measured with
`tools/robo_look_scan.py` (new) over the 34 stock bodies and the stock parts.

## What a stock robo is

| | Body (all 34) | Gun | Bomb | Pod file (4 roots) | Legs file (6 roots) |
|---|---|---|---|---|---|
| Triangles | 1,938 - 2,814 | ~450 | ~400 | ~860 - 960 | ~530 - 690 |
| Colour texture | one 512x512 CMPR, TEX0 | 256x256 | 256x256 | 256 + 256 | 128x128 per root |
| Second texture | one 128x128 CMPR on its own UV set (TEX1) | 128 | 128 | 128 | 64 |

- TObj flags: colour map `0x00030010` = LIGHTMAP_DIFFUSE + COLORMAP_BLEND; second map `0x00030020` =
  LIGHTMAP_SPECULAR + BLEND: a greyscale **specular mask** of what shines. (Gun 5 uses an I8 map with flags 0.)
- MObj render mode `0x203D` = CONSTANT | DIFFUSE | SPECULAR | TEX0 | TEX1 (+0x2000): the game lights the body itself.
  Material: ambient 0xCCCCCC (some 0xB3B3B3), diffuse white, specular black (body 7: white, shininess 10), shininess 20.
- Vertex data: POS / NRM / TEX0 / TEX1 all index16, no vertex colours.
- The textures (build/robo_look/stock_textures.png) are **painted**: flat colour areas, one smooth light ramp per
  piece, chrome as white -> grey ramps, crisp dark panel lines, shared bolt / logo pieces, no noise or wear. The
  specular masks are flat greys per material.

## Why the AI samurai looked wrong

The AI bake is a photo of worn lacquer: shading, scratches and noise baked in. Shrunk to game size it turns to mush;
at full size (four textures, 18,000 triangles, 1.8 MB) it is still a photo, not the game's style, and 7x the
triangles of any stock body.

## The painted look (`--look=paint`, `tools/blender/paint_look.py`)

Shared by `atlas_ai_body.py` (body) and `ai_part.py` (parts):

1. every bake texel sorted into one of the robo's materials by its colour in Lab (lightness weighted 0.55, so shading
   does not flip a plate), a list per robo (`SAMURAI`: red, dark red, gold, black, chrome, cream, cyan; `--paint=<json>`
   for another robo);
2. the label map cleaned into flat shapes (majority vote over 1.5 then 1 final texels; wider wiped out the gold trims);
3. each material repainted dark -> light along a ramp lit by the baked high-poly normal (front-top light, x = 0 so the
   mirrored sides match), times soft occlusion and the relief shade; glow materials (cyan) unshaded;
4. dark panel lines on material borders (~1 texel);
5. the specular mask per material (chrome 1.0, gold 0.85, red 0.45, black 0.25, cyan 0) -> the TEX1 specular map.

## The samurai, stock-sized (now in `mods/mecha_samurai_full`)

- Body: `make_body.py <body.glb> --tris=3000 --mirror=1 --atlas=512x512 --uv=smart --plates=1 --look=paint
  --detail=0.6 --ss=4 --head=<head.glb> --head_share=0.3` -> 3,000 triangles, one 512x512, 300,603 bytes (was
  1,836,239). `--uv=smart` packed better than the crease seams at this triangle count.
- Parts (`make_parts.py`, `LOOK = ['--look=paint']`): gun 450 / form, bomb 400 / form, pods 300 + 300 + 200 + 200,
  legs 280 per root (160 crumpled the calf pack) -> 900 / 800 / 1,000 / 1,120 triangles.
- The detailed Nov 9 set is kept in `build/mecha_samurai_full/models_detailed_nov9`.

Tried and dropped: `--pack=one` (every region packed together over the atlas): the inside surfaces of armour shells
took big pieces and the head lost its extra density; covered 43%, no better than the cells.

## Round 2 (Nov 10): hidden faces out, and four more methods

All options of `tools/blender/atlas_ai_body.py` (and `paint_look.py`); sources at the end.

1. **Hidden faces (`--cull`).** Ray-cast visibility (the "remove hidden / occluded geometry" step of Pixyz and
   RapidCompact, done here with Blender's BVH): a face stays when a ray from one of its sample points (centre + three
   halfway to the corners) leaves the mesh in some direction it faces (80 directions, Fibonacci sphere). Order:
   each region decimated to 2x its budget -> cull -> (mirror) -> each region decimated to its budget -> cull again.
   Decimating hides faces again, so the budgets are raised by what the last cull took and rebuilt once (up to twice)
   until the visible triangles land on the budget. Samurai A: 947 hidden faces out at the 2x stage, 137 at the end:
   2,861 triangles, all visible.
2. **Found on the way: the mirror made the right half inside-out.** `bmesh.ops.mirror` keeps the face winding, so
   every mirrored face pointed inward (checked on a cube: 5 of 10 faces inward). The game culls back faces, so every
   mirrored body built before Nov 10 showed the insides of its right side. `mirror_mesh` now reverses the new faces.
   Also: mirroring now happens before the last decimation, which is symmetric per region (a global trim after the
   mirror had taken the head from 900 triangles to 512).
3. **Edge highlights (`--edges=k`).** Hand-painted game textures light the convex plate edges and darken crevices
   (cavity / bevel lines). Cycles' Pointiness of the high model is baked (`hi_material_emit('point')`), normalised by
   its own spread, and convex edges are lit toward the material's own light colour (toward white turned the lacquer
   pink), crevices shaded; lights / visors untouched. k = 0.5.
4. **Texel importance (`--teximp`).** Older games gave the face and chest the most texels. The cull pass scores each
   face by the share of front directions (+Y) it is seen from (0.6..1.6); each UV island is scaled by its
   area-weighted score ^ 0.75 before packing, so front plates get more pixels than backs and undersides.
5. **Auto palette (`--paint=auto`).** k-means (k = 8, Lab) over the bake; each cluster becomes a material with a ramp
   (L -22 / +24, chroma 1.2x) and a specular by look (pale grey chrome 1.0, gold 0.85, dark 0.25, saturated bright
   blue / cyan = light). On the samurai's gun it found the red, gold bands, chrome and cream by itself (a darker red
   than the hand list); the samurai keeps his hand list, other robos get a start without one.
6. **Feature-protected decimation (`--feature=k`), tried and left off.** Decimate's vertex group weighted by crease
   angle and plate rims (the dihedral-angle quadric weighting of the 2024 quad-reduction paper, approximated through
   Blender's vertex-group factor). On the AI shells it stalled the first pass (rims everywhere: 164,019 faces left for
   a 4,200 target); the next pass then went from 164,000 to 900 in one step and lost the helmet's horns. Without it
   every stage reaches its target and the horns stay. Kept as an option (weights now 0.5 rims / 0.8 creases, with an
   unweighted fallback when a pass stalls).
7. **Panel lines** only where the painted colour changes (materials painted alike share a group): the red / dark-red
   border drew squiggles through the lacquer.
8. Checked: stock textures have no mipmaps (image mipmap 0, LOD 0, linear mag filter), as ours.

### The two builds for the owner's choice

| | A: stock-sized | B: split, 3,000 per region |
|---|---|---|
| Command | `--tris=3000 --atlas=512x512 --head_share=0.3` | `--region_tris=head:3000,torso:3000,arms:3000,legs:3000 --sections=1 --ss=2` |
| Shared | `--mirror=1 --uv=smart --plates=1 --look=paint --detail=0.6 --cull=1 --teximp=1 --edges=0.5 --head=<head.glb>` | same |
| Output | `build/mecha_samurai_full/stockA_nofeat` | `build/mecha_samurai_full/splitB` |

Owner's choice: **B**, then "fix the seams between the triangles when zoomed in".

## Round 3 (Nov 10): seams

Measured (`seamscan.py` logic: open edges with another open edge within 4 mm = cracks; UV islands; UV seams on hard
edges, > 35 degrees):

| | stock Ray 01 | B | **B3 (installed)** |
|---|---|---|---|
| crack edges | 0 | 2,044 | 573 (the separate head shell meeting the neck, by design) |
| triangles per UV island | 9.5 | 3.8 | 5.5 (body 7.1 with seams, head smart) |
| UV seams on hard edges | 88% | 73% | 84% |

How the game draws it (decomp):
- `HSD_TObjSetup` (src/sysdolphin/baselib/tobj.c:1221-1286): no TObj LOD and no mipmaps -> the min filter is
  masked to plain LINEAR; mag filter LINEAR (`tobj->magFilt` 1 on every stock body). Bilinear sampling reads the
  texels just outside a UV island; CMPR stores every 4x4 block as two colours, so a block that straddles two islands
  (our 2-texel gaps) mixes their colours on both edges: a visible seam line.
- `pobj.c:1477`: PObj flags pick the cull mode. Stock Ray 01: 42 PObjs `0x8000` (CULLBACK, rigid on one joint),
  8 `0x0000` (double-sided thin parts), 2 `0xA002` (envelope); chara 50: 43 rigid + 1 envelope. **The stock bodies
  are rigid armour pieces that overlap at the joints**, not one skinned skin; ours is mostly envelope (`0xA000`).

Fixes (all in `atlas_ai_body.py` / `paint_look.py`):
1. **Regions decimated in place** (edit-mode `mesh.decimate` on the region's faces only): splitting the regions into
   objects and decimating them apart left their borders unmatched (the 2,044 cracks).
2. **4-texel gutters** (`--gap=4`) and **gutter fill after painting** (`paint_look.dilate`, from an island raster of
   the UVs: `island_raster`): no bilinear sample or CMPR block sees another island's colour.
3. **Panel lines only inside an island** (a line drawn across an island border is a seam on the model).
4. **UVs cut on hard edges** (`--uv=seams --seams=45,60,75,90`) with **the head on smart_project**
   (`--uv_regions=head:smart`): crease seams on the head left the horns joined to the bowl in one folded island,
   which baked the bowl's red onto the horns.

B3 command: `make_body.py <body.glb> --region_tris=head:3000,torso:3000,arms:3000,legs:3000 --mirror=1 --sections=1
--uv=seams --uv_regions=head:smart --seams=45,60,75,90 --gap=4 --plates=1 --look=paint --detail=0.6 --ss=2
--head=<head.glb> --cull=1 --teximp=1 --edges=0.5` -> 11,819 visible triangles (head 3,224, torso 2,241, arms 3,227,
legs 3,127), 1,719,263 bytes. Next step toward the stock construction: rigid armour pieces per joint that overlap
(the 42 rigid PObjs), instead of one envelope skin.

Sources: [Pixyz delete occluded geometries](https://docs.unity3d.com/Packages/com.unity.industry.toolkit@3.0/manual/delete-occluded-geometries.html),
[RapidCompact invisible geometry](https://rapidcompact.com/doc/cli/v05/Invisible-Geometry/index.html),
[Blender SE: interior faces by ray cast](https://backiee.wasmer.app/https_blender_stackexchange_com/questions/57540/automated-way-to-make-select-interior-faces-ignore-select-faces-that-are-visib),
[hand-painted textures: cavity / edge maps](https://medium.com/@lettier/how-to-create-hand-painted-textures-without-painting-aa7374fef815),
[Polycount: light convex edges, dark concave](https://polycount.com/discussion/comment/2523483),
[quad-dominant reduction, dihedral-angle quadrics](https://arxiv.org/html/2411.16874v1),
[Polycount: texel density weighting](https://polycount.com/discussion/231919/filling-uv-space-vs-texel-density),
[Babylon.js: planning UVs, face density](https://documentation-a5um7gq0g-babylonjs.vercel.app/preparingArtForBabylon/planningMeshUVs).
