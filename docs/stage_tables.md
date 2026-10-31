# Stages: which bg_data entries an arena uses, and what a stage carries (plan.md step 75)

A **stage** (arena) is numbered 0..39 by the game (the "stage id"). This page maps every stage id to its two
`bg_data.BIN` entries, says what each entry holds, and lists the main.dol tables a stage also uses, with the code that
proves it. Same style as [robo_tables.md](robo_tables.md). Modding stages with Blender: docs/MODDING.md "Stages".

How it was found (Oct 31, 2026): from the file name `bg_data.bin` in main.dol's rodata (0x801B7D21) to its file
descriptor (`cr_g_FileDescs` slot 12, 0x801F3890), then every `cr_Archive_OpenEntry(0xC, ...)` call in the m2c drafts
(decomp/drafts, docs/drafts.md), up the request tables to the stage select code; the scene data and collision layouts
were read from all 80 entries with small scripts and checked against the code that reads them; the US names are a
BMG message file of `bmg/info_usa.bin`. No symbol was renamed (all `fn_` / `lbl_` names below are dtk's).

## The map

**Stage id s uses `bg_data.BIN` entry 2s (the model: an HSD .dat whose root is `scene_data`) and entry 2s+1 (the
collision).** Proof:

| Code | Address | What it does |
|---|---|---|
| request table | 0x8023BFD0 | 7 functions; the stage loader calls entry `[state]` for its two slots (`lbl_8041A234`, 8 bytes each: slot 0 = model, slot 1 = collision) |
| `fn_8011449C` (state 0) | 0x8011449C | `cr_Archive_OpenEntry(0xC, slot + stage * 2)`, read to `*lbl_8044F2E8` (slot 0) or to `lbl_8034C100` (slot 1); stores the stage id at `lbl_8037569C + 6` |
| `fn_80114160` (state 5) | 0x80114160 | the same for slot 0 only: entry `stage * 2` -> `*lbl_8044F2E8` |
| `fn_80114214` (state 6) | 0x80114214 | slot 1: entry `stage * 2 + 1` -> `lbl_8034C100` |
| `fn_801142B4` (state 2 -> 4) | 0x801142B4 | slot 0 done: `fn_80074FBC(*lbl_8044F2E8, lbl_80354110)` (HSD archive parse in place); slot 1 done: `fn_80072A58(lbl_8037569C, lbl_8034C100)` (collision setup), then `fn_80072064` (stage build) |
| `fn_80113680`, `fn_80113934` | 0x80113680, 0x80113934 | the stage select's loaders: run the two slots with the wanted stage id `lbl_8041A244[6]` (0x8041A24A) |
| `fn_80072064` | 0x80072064 | the stage build: `HSD_ArchiveGetPublicAddress(archive, "scene_data")`, then every model, light and the fog of the scene (below) |

The stage select builds its list from the order table **0x8023CF00** (`lbl_8023CF00`, 40 bytes: stage ids in menu
order, ended by 0x28): `fn_80113934` copies every id whose stage record (below) has +0x80 != 0, except 0x0A and 0x0B,
to `lbl_8041A244 + 0x14` (cursor -> id; 6 per row). The community's stage-list codes write this same table
(docs/community/discord_codes.md, docs/community/codes_gxce01.tsv "Stagelist Toggle", 0x8023CF00). Two ids are
alternates the menu does not list: while a pad bit 0x4000 of `lbl_8031FED8 + 0x1C` is held, the selected Basic Arena
(0) becomes 0x0A (Practice Stage) and No-Man's Land 0x1B becomes 0x0B (`fn_80111040`, `fn_8010FE6C`, `fn_8011023C`);
which button that is was not identified.

US names: BMG message file 24 of `bmg/info_usa.bin` (counting its `MESGbmg1` blocks from 0), message = stage id (41
messages; found by content, the code that reads it was not traced). Developer names: the Shift-JIS string at +0x00 of
the stage record.

| id | hex | menu | US name | developer name (record +0x00) | model entry (size) | collision entry (size) | record +0x88 | scene models | models with joint / material anims | light descs | fog | collision groups |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 0 | 0x00 | 0 | Basic Arena | ベーシックステージ | 000 (0x96100) | 001 (0x45c0) | 10 | 10 | 3 / 3 | 2 | no | 10 |
| 1 | 0x01 | 2 | High-Rise Plaza | ハイライズヘブン | 002 (0x80f60) | 003 (0x3fa0) | 9 | 9 | 3 / 3 | 2 | no | 9 |
| 2 | 0x02 | 5 | High-Rise Cell | ハイライズジェイル | 004 (0xdd580) | 005 (0x4080) | 9 | 9 | 2 / 3 | 2 | no | 9 |
| 3 | 0x03 | 15 | Nature Park | アスレチックパーク | 006 (0x8e020) | 007 (0x6140) | 16 | 16 | 2 / 3 | 2 | yes | 16 |
| 4 | 0x04 | 22 | Dead Line | デッドライン | 008 (0xa4aa0) | 009 (0x3ec0) | 8 | 8 | 2 / 1 | 2 | no | 8 |
| 5 | 0x05 | 12 | Flower Garden | フラワーガーデン | 010 (0xb5e20) | 011 (0x5140) | 13 | 13 | 1 / 0 | 2 | yes | 13 |
| 6 | 0x06 | - | Basic Arena (Test Use) | ベーシックステージ（実験用） | 012 (0x96100) | 013 (0x45c0) | 10 | 10 | 3 / 3 | 2 | no | 10 |
| 7 | 0x07 | - | RPG Test | ＲＰＧテスト | 014 (0xe580) | 015 (0x1540) | 1 | 1 | 0 / 0 | 3 | no | other format |
| 8 | 0x08 | 4 | Diamond Cell | ダイヤモンドジェイル | 016 (0xe18c0) | 017 (0x3680) | 9 | 9 | 2 / 3 | 2 | no | 9 |
| 9 | 0x09 | 13 | Chinese Bowl | チャイニーズレストラン | 018 (0x98c20) | 019 (0x6820) | 8 | 8 | 2 / 0 | 2 | yes | 8 |
| 10 | 0x0A | alt of 0 | Practice Stage | 練習ステージ | 020 (0x2e9a0) | 021 (0x45c0) | 10 | 10 | 3 / 3 | 2 | no | 10 |
| 11 | 0x0B | alt of 27 | No-Man's Land | プレーンステージ | 022 (0x22d80) | 023 (0x29e0) | 1 | 1 | 0 / 0 | 2 | no | 1 |
| 12 | 0x0C | 20 | Ice and Snow | アイスバーン | 024 (0x6fa60) | 025 (0x6120) | 11 | 11 | 2 / 3 | 7 | no | 11 |
| 13 | 0x0D | 9 | Castle Keep | オリエンタルウォール | 026 (0xc6060) | 027 (0x4e60) | 11 | 11 | 0 / 0 | 2 | yes | 11 |
| 14 | 0x0E | 6 | Crevice Court | クレバスコート | 028 (0x8d900) | 029 (0x4d40) | 12 | 12 | 2 / 2 | 2 | no | 12 |
| 15 | 0x0F | 1 | Diamond Fences | ダイヤモンドフェンス | 030 (0x80ee0) | 031 (0x41c0) | 10 | 10 | 4 / 4 | 2 | no | 10 |
| 16 | 0x10 | 21 | Frozen Field | フローズンフィールド | 032 (0x80d00) | 033 (0x3cc0) | 10 | 10 | 3 / 4 | 2 | no | 10 |
| 17 | 0x11 | 7 | L Formation | Ｌフォーメーション | 034 (0x7b5a0) | 035 (0x45c0) | 9 | 9 | 2 / 3 | 2 | yes | 9 |
| 18 | 0x12 | 8 | Checkmate Foundry | チェックメイトエッジ | 036 (0x96060) | 037 (0x4700) | 11 | 11 | 2 / 3 | 2 | no | 11 |
| 19 | 0x13 | 16 | Little Locomotive | ポップンロコモーティブ | 038 (0x974e0) | 039 (0x5520) | 17 | 17 | 8 / 4 | 2 | yes | 17 |
| 20 | 0x14 | 18 | Magma Hole | マグマホール | 040 (0xb4ca0) | 041 (0x5720) | 12 | 12 | 4 / 3 | 2 | no | 12 |
| 21 | 0x15 | 10 | Castle Citadel | オリエンタルキャッスル | 042 (0xc2e20) | 043 (0x5380) | 11 | 11 | 0 / 0 | 2 | yes | 11 |
| 22 | 0x16 | 24 | Panic Cubes | パニックキューブ | 044 (0x7d500) | 045 (0x4d60) | 19 | 19 | 15 / 14 | 2 | yes | 19 |
| 23 | 0x17 | 19 | Magma Ruins | マグマカタストロフィ | 046 (0xb8720) | 047 (0x4c60) | 13 | 13 | 2 / 2 | 2 | no | 13 |
| 24 | 0x18 | 29 | Impact Craters | プレアデスインパクト | 048 (0x94860) | 049 (0x4960) | 20 | 20 | 16 / 1 | 2 | no | 20 |
| 25 | 0x19 | 30 | Sudden Death | サドンデスステージ | 050 (0x43dc0) | 051 (0x2360) | 4 | 4 | 0 / 1 | 2 | no | 4 |
| 26 | 0x1A | 31 | Gigantix Sprawl | ギガンティックステージ | 052 (0xec220) | 053 (0x4ee0) | 11 | 11 | 0 / 1 | 2 | no | 11 |
| 27 | 0x1B | 32 | No-Man's Land | プレーンステージ | 054 (0x72420) | 055 (0x29e0) | 1 | 1 | 1 / 0 | 2 | yes | 1 |
| 28 | 0x1C | 14 | Robo's Room | キープクリーン | 056 (0x66fc0) | 057 (0x3560) | 10 | 10 | 0 / 1 | 2 | yes | 10 |
| 29 | 0x1D | 33 | Practice Stage | プラクティスステージ | 058 (0x5ce80) | 059 (0x3120) | 8 | 8 | 2 / 3 | 2 | no | 8 |
| 30 | 0x1E | 23 | Double Dead Line | ダブルデッドライン | 060 (0xa5080) | 061 (0x3200) | 8 | 8 | 3 / 1 | 2 | no | 8 |
| 31 | 0x1F | 17 | Merry-Go-Round | メリーゴーランド | 062 (0x7ca00) | 063 (0x4d80) | 10 | 10 | 10 / 1 | 2 | no | 10 |
| 32 | 0x20 | 27 | Loading Dock | コンテナプラント | 064 (0x5ad80) | 065 (0x4760) | 10 | 11 | 3 / 1 | 2 | no | 10 |
| 33 | 0x21 | 28 | Battle Gear Station | バトルギアステーション | 066 (0x85000) | 067 (0x6040) | 12 | 12 | 9 / 2 | 2 | no | 12 |
| 34 | 0x22 | 25 | Panic Walls | パニックウォール | 068 (0xb0e80) | 069 (0x4560) | 18 | 18 | 18 / 1 | 2 | yes | 18 |
| 35 | 0x23 | 11 | Lost World | ロストワールド | 070 (0x7b160) | 071 (0x46c0) | 5 | 5 | 1 / 5 | 2 | yes | 5 |
| 36 | 0x24 | 26 | Scramble Walls | スクランブルウォール | 072 (0xa86e0) | 073 (0x4500) | 17 | 17 | 16 / 1 | 2 | yes | 17 |
| 37 | 0x25 | 34 | Dark Star | ダークスター | 074 (0x620a0) | 075 (0x68a0) | 6 | 6 | 1 / 0 | 2 | no | 12 |
| 38 | 0x26 | 3 | Basic Cell | ベーシックジェイル | 076 (0x8ff40) | 077 (0x41e0) | 9 | 9 | 0 / 0 | 2 | no | 9 |
| 39 | 0x27 | 35 | Random Stage | ランダムステージ | 078 (0x8a40) | 079 (0x41e0) | 1 | 1 | 1 / 0 | 3 | no | 9 |

"menu" = position in the stage select (0-based, 6 per row); "-" = not listed (record +0x80 = 0: stages 6 and 7).
Identical entries: 012 = 000 and 013 = 001 (the test copy of Basic Arena), 055 = 023 (both No-Man's Lands share the
collision), 079 = 077 (Random Stage uses Basic Cell's collision; its own model is a 0x8a40-byte one-model scene, most
likely what the select screen shows for "random"; not verified). No shape animations in any stage. Every scene has one
camera desc. Animations: the game attaches each model's first joint / material / shape animation set when it builds
the stage (`HSD_JObjAddAnimAll` in `fn_80072064`) and marks the model animated (+0x10000000 in its 0x13C-byte record);
the loop does not export or change animations (they stay in the file untouched).

## Entry 2s: the stage model (`scene_data`)

A standard HSD .dat (0x20 header, data, relocation table, one root `scene_data`, as `HSDRawViewer --info` shows;
`tools/export_models.py` and `HSDRawViewer --export` already read them). `scene_data` is an HSD scene description
(`HSD_SOBJ` in HSDLib: models, cameras, lights, fog). What `fn_80072064` does with it:

- **Models** (`JOBJDesc` array, null-terminated; root joint, joint / material / shape animation lists): each is loaded
  with `HSD_JObjLoadJoint` into a 0x13C-byte record at `lbl_8037569C + 0xB8 + n * 0x13C`; **at most 20** (the record
  array has 0x14 entries; Impact Craters uses all 20). For models 1..n-1 it copies the flags of the root joint's DObjs
  (MObj and DObj flags, up to 20 slots at +0x150 / +0x1A0) and **hangs in an endless loop if a root joint has more than
  21 DObjs** (`loop_101` in the draft); model 0 is not checked.
- Each model's record +0xF8 is the word at +0x14 of the model's collision group (below), its bits pick a per-model
  handler (`lbl_802176C8[(flags * 4) & 0x1C]`, and `*lbl_8044EA50` for bits 3-4 or 0x20); not mapped further.
- **Lights**: every light desc (null-terminated array) -> `HSD_LObjLoadDesc` (up to 10, `lbl_80354154`); the first
  LObj with type bits == 1 also gets a shadow (`HSD_ShadowAlloc`, 600 x 440).
- **Fog**: `HSD_FogLoadDesc` when present; its colour is copied to `lbl_80449EA0 + 0x48` (the clear colour, likely).
- The stage record's +0x88 (below) is the model count the game uses after loading (`lbl_8037569C + 8`); for Loading
  Dock (0x20) the last model (11th) is drawn separately with the shadow (`HSD_ShadowAddObject`); every other model gets
  joint flag 0x04000000 (`fn_800944E4`).

Materials (Basic Arena, `HSDRawViewer --info models_dat/bg_data/bg_data_000.dat`): CMP textures throughout (512x512
atlases, 32x32 and 64x64 tiles), lit materials (`CONSTANT, DIFFUSE, TEX0, TEX1`: the diffuse image plus a
`COORD_REFLECTION` environment map on TEX1, generated from the normals), unlit vertex-colour materials (`VERTEX, TEX0`
with a CLR0 colour attribute), translucent ones (`XLU`, `NO_ZUPDATE`). PObjs are POS / NRM / CLR0 / TEX0 (one UV set).

## Entry 2s+1: the collision

Read by `fn_80072A58(lbl_8037569C, buffer)`, which turns the offsets into pointers. Big-endian:

| Offset | Field |
|---|---|
| 0x00 | u32 N: number of groups |
| 0x04 | offset of N offsets to the group headers (-> `lbl_8037569C + 0xC`) |
| 0x08 | offset of N offsets to the groups' quad arrays (-> `lbl_8037569C + 0x10`) |
| 0x0C | offset of section C, or 0 (-> `lbl_8044F5D8` / `lbl_8044F5DC`) |

- **Group header** (0x20 bytes): bounding sphere `x, y, z, r` (4 floats), u32 quad count, u32 model flags (the word
  the stage build copies to the model's +0xF8: 0, 1, 3, 8, 11, 16, 72, 136 seen), 2 x u32 0. **Group n belongs to
  scene model n**: the stage build reads model n's flags from group n's header (`lbl_8037569C[3][n] + 0x14`). The
  counts match in 36 stages (Loading Dock: 11 models, 10 groups; Dark Star 6 / 12; Random Stage 1 / 9; RPG Test other).
- **Quad** (0x44 bytes, right after the header; the second offset table points at it): 4 corners (12 floats), the
  plane's unit normal (3 floats), the plane distance d (n . p + d = 0), a u32 attribute (0 mostly; 8, 24, 25-28, 256,
  512, 1024-2048, 1280, 5376, 197376 seen: surface kinds, not mapped). Checked over all 39 entries of this format:
  every normal is unit length and every group's quads end before the next group.
- Example: Basic Arena group 0 is the outer box (sphere (0, 15, 0) r 18.79; five quads: the walls x = +-8, z = +-8,
  30 high, and the floor y = 0).
- **Section C**: N x 0x284-byte records (the count is not stored; sizes are multiples of 0x284 padded to 0x20).
  Read by `fn_8006D4F8` (links records whose line of sight is clear, `fn_8005D08C`, and stores neighbour numbers at
  +0xC.. and distances at +0x4C..) and by `fn_800DBAD0` (a large battle function, reads +0 / +4 as positions): it looks
  like a **waypoint graph** (probably for the computer players); not verified. For Impact Craters (0x18) the game
  copies 0x30 records into RAM (`lbl_80344550`, 0x78C0 bytes) before using them; the others are used in place.
- Battle start (`cr_SEQ_BATTLE_FrameA`) reads group 0's sphere: x and z and (r x a constant)^2 into `lbl_8024E100`
  (the arena's centre and radius, likely the bounds check).
- **Confirmation from the community**: Project Hive's "Remove Castle Keep Wall Middle Section" (seiken;
  docs/community/codes_gxce01.tsv) writes 0x8034DD68 and 0x8034DD73 = `lbl_8034C100 + 0x1C68` / `+ 0x1C73`: in Castle
  Keep's collision (entry 027) that is group 9's header (the middle wall at x 0, z 4.2): its sphere z and the low byte
  of its quad count. Its other hook, 0x8007202C (`fn_80071974 +0x6B8`), reads the model count (`lbl_8037569C + 8`).
- RPG Test's collision (015) has another layout (1 group, offsets past the end of the entry when read this way).

## The stage record (main.dol 0x8020F4C8, 0x340 bytes x 40)

`lbl_8020F4C8 + id * 0x340` (`lbl_8044F5EC` while a stage is loaded). Fields read by the code:

| Offset | Read by | Meaning |
|---|---|---|
| +0x00 | (text) | developer name, Shift-JIS |
| +0x80 | `fn_80113934` | listed in the stage select (0 = hidden: stages 6, 7) |
| +0x84 | not traced | pointer to a parameter block (0x8020EF38 for most stages; 0x8020F0B8 for 1, 0x8020F270 for 3, 0x8020F498 for 8); floats that look like camera settings; not verified |
| +0x88 | `fn_80072064` | model count used by the game (`lbl_8037569C + 8`) |
| +0xCC, +0xD0, +0xD4 | `cr_CAMMODE_OPENING_Start`, `fn_80089950` | three floats of the opening camera |
| +0x120, +0x124 + 8n | `fn_8006EA34`, `fn_80071974` | a byte, then (model number byte, u32) pairs ended by model 0xFF: a per-model kind used when the stage's models are set up (the hook point of the wall-removal code above) |
| +0x338 | `fn_80089550`, `fn_800DF320` | a small number 0..4 (kind of stage, used in battle code); not mapped |
| +0x339 | `fn_800761D0` | index into `lbl_80217928` (0x10 bytes each) |
| +0x33A | `fn_80113934` | copied to the stage select's per-entry halfword (+0x3E); not mapped |
| +0x33C | `fn_80070170` | a float (100.0 for Basic Arena, 50 to 260 elsewhere, 0 for some) |

Floats at +0x90..+0x11F (e.g. Basic Arena: pairs like (-2, 1), (-2, -2), (1, 2), (2, 1)) may hold the start
positions, but no reader was traced: **spawn points are not located** yet.

## Limits for a modded stage

| Limit | Value | Source |
|---|---|---|
| Model entry (2s) | 0x100000 bytes: it is read whole to `*lbl_8044F2E8`, and the next region (`lbl_8044F2E4`) starts 0x100000 later (`fn_80023CBC`, the memory map at boot) | the largest retail model is 0xEC220 (Gigantix Sprawl) |
| Caveat | battle start loads something to `*lbl_8044F2E8 + 0xA0000` when `*lbl_8041EA18 != 0` (`cr_SEQ_BATTLE_FrameA`); 10 retail stages are larger than 0xA0000, so that flag is probably off in normal play; not verified | |
| Collision entry (2s+1) | 0x8000 bytes (`lbl_8034C100`, followed by `lbl_80354100`) | the largest used one is 0x68A0 (Dark Star); RPG Test's model (0xE580) is not a collision |
| Models per scene | 20 | the 0x14-record array |
| DObjs on a model's root joint | 21 for models 1..n-1 (endless loop otherwise) | `fn_80072064` |
| Light descs | 10 | `lbl_80354154` (0x28 bytes) |
| Archive | `bg_data.BIN` is a "kind 1" archive: the game reads the archive's own header (its first 0x800 bytes) at boot (`cr_FileSys_Init` registers it, `fn_80021674` reads it), so there is **no entry table in main.dol** to update (unlike chara.BIN); 80 entries need 0x288 bytes of it | `tools/make_mod.py` knows this |

`tools/make_mod.py` refuses a model entry over 0x100000 or a collision entry over 0x8000.

## snr_map (story mode maps), for comparison

`scenario_us/snr_map.BIN` (file descriptor 4, 94 entries) is opened by the map script commands (`cr_MAP_SET`,
`cr_MAP_LINK`, `cr_MAP_NEXT_SET`, `cr_AreaBoxes_Update`) as entry n or n + 0x2F (38 of the 47 pairs are identical
files); `rpg_t_models` / `rpg_f_models` (descriptors 5, 6) are opened with the same n. Its header (0x20 bytes):
`u32 0x20` (offset of a collision block in the bg_data odd-entry format above: N, 0x10, 0x14, 0), `u32` offset of an
HSD .dat with a `scene_data` root (the map model; 92 of 94 entries), four floats x min / x max / z min / z max (e.g.
-24, 24, -20, 20: the map bounds, likely), `u32` offset of a small last block (not decoded), `u32 0`. So a story map
is a stage model and a stage collision in one entry; the loop does not handle `snr_map` yet (plan.md "Resume here").

## Open questions (in-game tests are in plan.md "Waiting on the user")

- Spawn points and the battle camera: not located (stage record +0x84 / +0x90.. are candidates).
- The collision quad attribute values (surface kinds: ice, conveyor, magma?) and the group flags.
- Section C (waypoints?) and who uses it.
- The pad bit that switches Basic Arena / No-Man's Land to their alternates (0x0A / 0x0B).
- Whether the US name file 24 is what the stage select draws (found by content only).
- The `*lbl_8041EA18 != 0` load at +0xA0000 (when it happens, and whether a big stage survives it).
- Only the stage select's loader was traced: whether story-mode battles load stages the same way is not checked.
