# What is inside each archive

Classification by `tools/extract_models.py` logic, run over every entry of every SFD archive (`unpacked/`).

| Archive | Entry format | Models extracted | Status |
|---|---|---|---|
| `chara` | model entry, "abs" variant ([model_entry.md](model_entry.md)) | 170 | decoded; import works offline |
| `weapon_gun` / `_bom` / `_pod` / `_leg` | model entry, "rel" variant | 64 / 52 / 57 / 29 | decoded; import works offline |
| `scenario_*/snr_chara` | model entry, "abs" variant (some with block D) | 56 (US) | decoded; import works offline |
| `bg_data` even entries | standard HSD .dat, root `scene_data`: entry 2s = the model of stage id s ([stage_tables.md](../stage_tables.md)) | 40 | decoded; import works (Blender loop, `--model N`) |
| `bg_data` odd entries | entry 2s+1 = the collision of stage s: u32 group count, offsets to group headers (bounding sphere, quad count, model flags) and to their quads (4 corners, plane, attribute), optional waypoint-like records ([stage_tables.md](../stage_tables.md)) | — | layout decoded (read by `fn_80072A58`); attributes not mapped |
| `leg_effect` | standard HSD .dat, root `scene_data` | 34 | decoded |
| `scenario_*/snr_obj` | standard HSD .dat, root `scene_data` | 89 (US) | decoded |
| `attack_effect` / `bomb_effect` / `gun_effect` / `pod_effect` | small container: u32 N, then N+1 offsets (last = end); each piece is a standard HSD .dat (`scene_data`) | 68 / 104 / 128 / 114 | decoded |
| `snr_motion` | same count+offset container; pieces are **not** standard .dat | — | motion files, one per piece: [motion.md](motion.md) |
| `scenario_*/snr_map` | header 0x20 bytes: offset of a collision block (bg_data odd-entry format), offset of an HSD .dat (`scene_data`), 4 floats x / z min / max (bounds, likely), offset of a small unknown block ([stage_tables.md](../stage_tables.md#snr_map-story-mode-maps-for-comparison)) | — | layout read; not in the loop yet |
| `scenario_*/rpg_t_models` / `rpg_f_models` | entry 0: u32 count, then (id, offset) pairs; other entries differ; one entry per archive parses as a model entry without joints | — | **unknown** |
| `TITLE_MODEL.dat` (loose file) | standard HSD .dat, root `scene_data` | 1 | decoded |

Only the US `scenario_us` archives are extracted to `models_dat/`; `scenario_jp` has the same archives.

## Other file types on the disc (not yet worked on)

| Files | Format | Source |
|---|---|---|
| `sprite/*.tpl` (587), `*.tpl` | GameCube TPL textures (magic `00 20 AF 30`) | header bytes |
| `*.bmg` | Nintendo BMG text (`MESGbmg1`) | header bytes |
| `message/*.bmc` | magic `MGCLbmc1` (not BMG; likely a colour table — not verified) | header bytes |
| `thpdemo/*.thp` | THP video | extension |
| `sound/*.hps`, `*.ssm`, `*.sem` | audio | extension |
| `*.dbn` (`logo_read_*`, `efc_common`, `motion.DBN`) | u32 count + offsets. `logo_read_us.dbn` is the boot bundle: 9 entries, entry 8 the motion file the game plays ([motion.md](motion.md#the-boot-bundle-logo_read_usdbn)); `motion.DBN` is a loose motion file the game never reads; `efc_common` not parsed | header bytes; fn_80019E84 |
| `*.bnr` | GameCube banner (`BNR1`) | header bytes |
| `viewer.ini`, `IS USBServer.ini` | text config left on the disc | text |
| `gcflag_joy.bin` | unknown | — |

## Model categories in `models_dat/`

chara 170, weapon_gun 64, weapon_bom 52, weapon_pod 57, weapon_leg 29, snr_chara 56, bg_data 40, leg_effect 34, snr_obj 89, attack_effect 68, bomb_effect 104, gun_effect 128, pod_effect 114, title 1 — **1,006 total**.
