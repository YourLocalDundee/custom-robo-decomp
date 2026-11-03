"""Run decomp-permuter (tools/downloads, untrusted clone) from outside its folder. usage: python -I run_permuter.py DIR [args]
Needs a `cpp` on PATH (tools/downloads/cpp_shim/cpp.exe: a copy of LLVM's clang-cpp.exe)."""
import os, sys
PERM = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'downloads', 'decomp-permuter_dl', 'decomp-permuter')
sys.path.insert(0, os.path.abspath(PERM))
if __name__ == '__main__':
    from src.main import main
    sys.argv = ['permuter.py'] + sys.argv[1:]
    main()
