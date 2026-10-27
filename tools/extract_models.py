"""Write every known model in the game as a standard HSD .dat, grouped by category.

Input is the output of `sfd.py unpack` (plus TITLE_MODEL.dat from the disc). Three entry kinds:
  cr-hsd   : HSD data behind the game's 0x40-byte header (chara, weapon_*, snr_chara) -> crmodel.to_dat
  std-hsd  : already a standard .dat (bg_data even entries, leg_effect, snr_obj)
  split    : u32 count, count+1 offsets (last = end) -> each piece is a standard .dat (*_effect)

Usage: python -I extract_models.py <unpacked_dir> <disc_files_dir> <out_dir>
"""
import shutil
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))  # python -I drops the script dir
import crmodel  # noqa: E402

CR_HSD = ["chara", "weapon_gun", "weapon_bom", "weapon_pod", "weapon_leg", "scenario_us/snr_chara"]
STD_HSD = ["bg_data", "leg_effect", "scenario_us/snr_obj"]
SPLIT = ["attack_effect", "bomb_effect", "gun_effect", "pod_effect"]


def is_std_hsd(d):
    if len(d) < 0x20:
        return False
    fs, ds, rc, roots = struct.unpack_from(">IIII", d)
    return 0x20 < fs <= len(d) and ds < fs and 1 <= roots < 64 and 0x20 + ds + rc * 4 <= fs


def split(d):
    n = struct.unpack_from(">I", d)[0]
    if not 0 < n < 64:
        return None
    offs = struct.unpack_from(">%dI" % (n + 1), d, 4)
    if offs[-1] > len(d) or any(a >= b for a, b in zip(offs, offs[1:])):
        return None
    return [d[a:b] for a, b in zip(offs, offs[1:])]


def main():
    unpacked, disc_files, out = (Path(a) for a in sys.argv[1:4])
    counts = {}

    def put(cat, name, data):
        (out / cat).mkdir(parents=True, exist_ok=True)
        (out / cat / f"{name}.dat").write_bytes(data)
        counts[cat] = counts.get(cat, 0) + 1

    for arch in CR_HSD:
        cat = Path(arch).name
        for p in sorted((unpacked / arch).glob("*.bin")):
            d = p.read_bytes()
            if crmodel.parse(d):
                put(cat, f"{cat}_{p.stem}", crmodel.to_dat(d))
    for arch in STD_HSD:
        cat = Path(arch).name
        for p in sorted((unpacked / arch).glob("*.bin")):
            d = p.read_bytes()
            if is_std_hsd(d):
                put(cat, f"{cat}_{p.stem}", d)
    for arch in SPLIT:
        for p in sorted((unpacked / arch).glob("*.bin")):
            for i, piece in enumerate(split(p.read_bytes()) or []):
                if is_std_hsd(piece):
                    put(arch, f"{arch}_{p.stem}_{i}", piece)
    (out / "title").mkdir(parents=True, exist_ok=True)
    shutil.copy(disc_files / "TITLE_MODEL.dat", out / "title" / "TITLE_MODEL.dat")
    counts["title"] = 1
    for k, v in counts.items():
        print(f"{k:14} {v}")
    print("total", sum(counts.values()))


if __name__ == "__main__":
    main()
