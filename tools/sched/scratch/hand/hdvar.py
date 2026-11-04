"""hdvar.py OUTDIR FUNC SPEC.py: SPEC defines BASE (a C text with {A} {B} ... slots) and V = {name: {slot: text}};
each variant is written and checked with hd.py (score only)"""
import runpy
import subprocess
import sys
from pathlib import Path

here = Path(__file__).parent
out, func, spec = sys.argv[1], sys.argv[2], runpy.run_path(sys.argv[3])
d = here / "hdvar_out"
d.mkdir(exist_ok=True)
for name, slots in spec["V"].items():
    text = spec["BASE"]
    for k in set(__import__("re").findall(r"\{(\w+)\}", text)):
        text = text.replace("{" + k + "}", slots.get(k, ""))
    p = d / f"{func}_{name}.c"
    p.write_text(text, encoding="utf-8")
    r = subprocess.run([sys.executable, "-I", str(here / "hd.py"), out, func, str(p)], capture_output=True, text=True)
    lines = (r.stdout + r.stderr).strip().splitlines()
    print(f"{name:24s} {lines[0] if lines else '?'}")
    if "-v" in sys.argv:
        print("\n".join(lines[1:14]))
