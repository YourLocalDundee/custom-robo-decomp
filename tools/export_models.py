"""Export every .dat under models_dat/<category>/ to models_export/<category>/<name>/ via HSDRawViewer --export.

glb first; files the glTF writer rejects (NaN values) are retried as .dae.
Writes models_export/export_log.txt.

Usage: python -I export_models.py <models_dat> <models_export>
"""
import subprocess
import sys
from pathlib import Path

EXE = Path(__file__).resolve().parent / "HSDLib/HSDRawViewer/bin/Release/net8.0-windows7.0/HSDRawViewer.exe"


def run(out_dir, ext, files):
    lines = []
    for i in range(0, len(files), 100):  # keep command lines short
        r = subprocess.run([str(EXE), "--export", str(out_dir), ext] + [str(f) for f in files[i:i + 100]],
                           capture_output=True, text=True)
        lines += [l for l in r.stdout.splitlines() if l.startswith(("ok ", "fail "))]
    return lines


def main():
    src, dst = Path(sys.argv[1]), Path(sys.argv[2])
    log = []
    for cat in sorted(p for p in src.iterdir() if p.is_dir()):
        files = sorted(cat.glob("*.dat"))
        lines = run(dst / cat.name, "glb", files)
        failed = sorted({l.split()[1].rstrip(":") for l in lines if l.startswith("fail")})
        lines = [l for l in lines if l.startswith("ok")]
        if failed:
            retry = run(dst / cat.name, "dae", [cat / f for f in failed])
            lines += [l.replace("ok ", "ok(dae) ", 1) for l in retry]
        log += [f"{cat.name} {l}" for l in lines]
        bad = [l for l in lines if l.startswith("fail")]
        print(f"{cat.name:14} files={len(files):4} roots_exported={sum(l.startswith('ok') for l in lines):4} "
              f"dae_fallback={len(failed):3} still_failing={len(bad)}")
    (dst / "export_log.txt").write_text("\n".join(log) + "\n")
    if not log:  # e.g. HSDRawViewer did not start (missing .NET): fail instead of reporting an empty export
        sys.exit(f"nothing exported - check that {EXE} runs")


if __name__ == "__main__":
    main()
