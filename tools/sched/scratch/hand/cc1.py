"""cc1.py SRC.c: compile with the game flags (no context) and print the objdump with relocations"""
import os, subprocess, sys, tempfile
G = r"D:\emulators\gameTesting"
sys.path.insert(0, os.path.join(G, "tools"))
import draft_batch as db
ver, flags = db.game_flags()
ver = os.environ.get('VER', ver)
if os.environ.get('OPT'):
    flags = [os.environ['OPT'] if f.startswith('level=') else f for f in flags]
flags += os.environ.get('XF', '').split()
src = os.path.abspath(sys.argv[1])
o = os.path.join(tempfile.mkdtemp(), "x.o")
cmd = db.mwcc_cmd(ver) + ["-c"] + flags + ["-o", o, src.replace("\\", "/")]
rc, so, se = db.run(cmd, 120, cwd=db.D)
if rc:
    print(so + se); sys.exit(1)
OD = os.path.join(db.D, "build", "binutils", "powerpc-eabi-objdump.exe")
print(subprocess.run([OD, "-dr", o], capture_output=True, text=True).stdout)
