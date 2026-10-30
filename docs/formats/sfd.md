# SFD archive format (`*.BIN`)

**Verified** on all 22 SFD archives on the disc: `tools/sfd.py verify` rebuilds every one byte-identical from its unpacked entries.

## Layout (big-endian)

| Offset | Size | Field |
|---|---|---|
| 0x0000 | 4 | magic `"SFD\0"` |
| 0x0004 | 4 | entry count N |
| 0x0008 | 8 × N | N × (u32 offset, u32 size) — offsets are from the file start |
| … | | zero padding up to 0x8000 |
| 0x8000 | | entry 0 |

Rules (all verified on every archive):
- First entry always at 0x8000; the header area is zero-padded.
- Each next entry starts at the first 0x800 boundary after the previous entry ends (tight packing, zero gaps).
- File length is padded with zeros to a multiple of 0x800.
- A header holds at most (0x8000 − 8) / 8 = 4,095 entries (largest archive on disc: 170).

## Archives on the disc

| Archive | Entries | Contents (see [containers.md](containers.md)) |
|---|---|---|
| `chara.BIN` | 170 | robo models (34 robos × 5 entries) |
| `weapon_gun.BIN` / `weapon_bom.BIN` / `weapon_pod.BIN` / `weapon_leg.BIN` | 64 / 52 / 57 / 29 | weapon models |
| `attack_effect.BIN` / `bomb_effect.BIN` / `gun_effect.BIN` / `pod_effect.BIN` | 34 / 52 / 64 / 57 | effect models (2 .dat each) |
| `leg_effect.BIN` | 34 | effect models (standard .dat) |
| `bg_data.BIN` | 80 | stages: even = model .dat, odd = unknown |
| `snr_motion.BIN` | 50 | unknown (count+offset container) |
| `scenario_us\` and `scenario_jp\`: `snr_chara`, `snr_obj`, `snr_map`, `rpg_t_models`, `rpg_f_models` | 56, 89, 94, 48, 48 each | story mode |

`gcflag_joy.bin` is not an SFD archive (no magic) and is skipped by the tools.

## Tool

`tools/sfd.py` ([tools.md](../tools.md)). Windows:

```powershell
python -I tools\sfd.py unpack "Custom Robo extracted\files" unpacked     # entries -> unpacked\<path>\<name>\000.bin ...
python -I tools\sfd.py pack unpacked\chara out\chara.BIN                  # entries 000.bin, 001.bin ... in index order
python -I tools\sfd.py verify "Custom Robo extracted\files" unpacked      # repack all in memory and compare
```

Linux / macOS:

```bash
python3 -I tools/sfd.py unpack "Custom Robo extracted/files" unpacked     # entries -> unpacked/<path>/<name>/000.bin ...
python3 -I tools/sfd.py pack unpacked/chara out/chara.BIN                  # entries 000.bin, 001.bin ... in index order
python3 -I tools/sfd.py verify "Custom Robo extracted/files" unpacked      # repack all in memory and compare
```
