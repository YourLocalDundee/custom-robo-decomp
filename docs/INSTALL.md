# Install the tools

What you need, per OS, in the order to run it. The short version is in the [README](../README.md#install-the-tools);
this page has the details and the checks. Nothing here is shipped as a prebuilt binary: tools come from your package
manager, or from the pinned, SHA-256-checked downloads in [tools/pins.txt](../tools/pins.txt) that the setup script
fetches (`tools/setup.ps1` on Windows, `tools/setup.sh` on Linux / macOS).

Windows runs the PowerShell scripts (`tools/*.ps1`, `tools/batches/*.ps1`); Linux and macOS run the bash scripts
(`tools/*.sh`, `tools/batches/*.sh`), which do the same steps in the same order with the same arguments and write the
same files, so [docs/expected_checksums.txt](expected_checksums.txt) holds on every OS. No PowerShell is needed or used
outside Windows.

## What is needed

| Tool | Version | Why | Who installs it |
|---|---|---|---|
| git | any | clones this repo and the pinned reference decomps | you |
| Python | 3.11+ (Windows: the command must be `python`; Linux / macOS: `python3`) | every `tools/*.py`, the preview venv | you |
| PowerShell 7 (`pwsh`) | 7+ | the setup / batch / checksum scripts. **Windows only** (Windows PowerShell 5.1 is not enough) | you (Windows) |
| bash | 4+ (Ubuntu / Fedora / Arch ship 5; macOS: `brew install bash`) | the setup / batch / checksum scripts on Linux / macOS | Linux itself; you on macOS |
| Java JDK | 21+ | runs Ghidra 12.1 (headless decompile) | you |
| .NET SDK | 8+ | builds HSDRawViewer (glTF model export). **Windows only** | you (Windows) |
| tar | any | unpacks zip / 7z on Windows (built into Windows 10/11) | Windows itself |
| curl, unzip | any | downloads (hash-checked) and unpacks Ghidra on Linux / macOS (keeps the executable bits) | you (Linux / macOS) |
| Dolphin's `DolphinTool` | any recent | extracts the disc on Windows: the pinned official build is downloaded for you. Linux / macOS: not needed, the pinned decomp-toolkit extracts the disc (byte-identical); `dolphin-tool` is only a fallback | `setup.ps1` (Windows) |
| Ghidra 12.1.4 + GameCube loader 1.3.1 | pinned | decompiler | setup script (pinned) |
| decomp-toolkit (dtk) 1.8.4 | pinned, one binary per OS | function boundaries / SDK names; disc extraction on Linux / macOS | setup script (pinned) |
| HSDLib / HSDRawViewer | pinned commit | model export, built from source | `setup.ps1` (Windows) |
| Python packages (numpy, trimesh, ...) | pinned in `tools/requirements-preview.txt` | preview renders | setup script (venv) |
| ninja, objdiff-cli, binutils, CodeWarrior compilers, wibo | pinned (see "Phase 4 build tools") | builds main.dol in `decomp/` | setup script (pinned) |
| wine | any recent | runs CodeWarrior on Linux aarch64 only (x86_64 / macOS use the pinned wibo) | you (Linux aarch64) |

The setup script (`pwsh tools/setup.ps1 -Iso ...` / `tools/setup.sh ...`) starts by printing each tool's version and stops
with one list of what is missing.

## Windows 11 / 10 (winget)

```powershell
winget install --id Git.Git -e
winget install --id Python.Python.3.12 -e
winget install --id Microsoft.PowerShell -e
winget install --id Microsoft.DotNet.SDK.8 -e
winget install --id EclipseAdoptium.Temurin.21.JDK -e
```

Close and reopen the terminal, then run everything from `pwsh` (not the blue Windows PowerShell). All five IDs were
looked up with `winget search` on 2026-10-28. Python from winget provides `python`.

## Debian / Ubuntu (apt)

```bash
sudo apt update
sudo apt install -y git curl unzip python3 python3-venv openjdk-21-jdk
```

- `openjdk-21-jdk` exists on Ubuntu 24.04+ and Debian 13; Debian 12 only has 17: use the no-root JDK below, or
  Adoptium's apt repo.
- No PowerShell, no Dolphin and no .NET are needed on Linux: the bash scripts run everything, the disc is extracted by
  the pinned decomp-toolkit, and model export is Windows-only (see "Limits and status").
- **No root?** A JDK works from your home folder: download the Temurin 21 tarball, check its SHA-256 against the
  `.sha256.txt` Adoptium publishes next to it, unpack it under `~/.local` and put it first on `PATH` for the scripts:
  ```bash
  v=jdk-21.0.13+9; f=OpenJDK21U-jdk_x64_linux_hotspot_21.0.13_9.tar.gz
  u="https://github.com/adoptium/temurin21-binaries/releases/download/${v/+/%2B}/$f"
  curl -fsSLO "$u" && curl -fsSLO "$u.sha256.txt" && sha256sum -c "$f.sha256.txt"
  mkdir -p ~/.local/jdk-21 && tar -xzf "$f" -C ~/.local/jdk-21 --strip-components=1
  export PATH="$HOME/.local/jdk-21/bin:$PATH"      # for this shell; add it to ~/.bashrc to keep it
  java -version
  ```
  (any newer 21.x from https://adoptium.net/temurin/releases/?version=21 works the same way). Everything else the
  pipeline needs is downloaded into the repo folder by `tools/setup.sh`, so nothing else needs root.

## Arch (pacman)

```bash
sudo pacman -S --needed git curl unzip python jdk21-openjdk
```

If several JDKs are installed: `sudo archlinux-java set java-21-openjdk`. Arch's `python` package provides `python3`.
Ghidra itself is downloaded at the pinned version by `setup.sh`; do not use Arch's `ghidra` package (a different version
changes the output). Nothing from the AUR is needed.

## Fedora (dnf)

```bash
sudo dnf install -y git curl unzip python3 java-21-openjdk-devel
```

## macOS (Homebrew) - best effort

```bash
brew install git python@3.12 openjdk@21 bash
# openjdk@21 is keg-only: make it visible to "java"
sudo ln -sfn "$(brew --prefix openjdk@21)/libexec/openjdk.jdk" /Library/Java/JavaVirtualMachines/openjdk-21.jdk
```

The scripts start with `#!/usr/bin/env bash`: with Homebrew's bin first on `PATH` that is bash 5 (the stock 3.2 is
untested). `curl` and `unzip` ship with macOS. The pinned decomp-toolkit extracts the disc; Dolphin is not needed.

## Any other system

Install: git, Python 3.11+ (`python3`), bash 4+, a JDK 21+, curl and unzip. On Windows: PowerShell 7, Python as
`python`, and the .NET SDK 8+.

## Phase 4 build tools (decomp/, batch 9)

Nothing to install by hand: setup step 8 (`tools/setup_decomp.ps1` / `tools/setup_decomp.sh`) downloads every build
tool at the version pinned in `tools/pins.txt` and checks its SHA-256. The versions are the ones the pinned
`encounter/dtk-template` asks for.

| Tool | Version | Why | Where it goes |
|---|---|---|---|
| ninja | 1.13.2 (GitHub release zip: Windows x64, Linux x86_64 / aarch64, macOS) | runs the build | `tools/ninja/` |
| decomp-toolkit | 1.8.4 (the same pin as above; the template asks for 1.8.3) | splits main.dol into objects | `tools/dtk/` |
| objdiff-cli | 3.6.1 | progress report (`report.json`), later: per-object diffs | `decomp/build/tools/` |
| gc-wii-binutils | 2.42-2 | assembler for asm objects (`--non-matching`, Milestone 2) | `decomp/build/binutils/` |
| CodeWarrior compilers | `compilers_20251118.zip` (files.decomp.dev; GC 1.0 .. 3.0a5.2, Wii, ProDG) | `mwldeppc` links main.dol now; `mwcceppc` compiles C later, and batch 10 (`docs/compiler.md`) runs all 19 GC versions | `decomp/build/compilers/` |
| sjiswrap | 1.2.2 | Shift-JIS wrapper the template puts around mwcc | `decomp/build/tools/` |
| wibo | 1.0.3 (Linux x86_64, macOS) | runs the Windows-only `mwldeppc` / `mwcceppc` on Linux / macOS | `decomp/build/tools/` |

- **Windows**: nothing else. (`winget install --id Ninja-build.Ninja -e` gives a ninja on `PATH` if you want one; the
  scripts use the pinned one.)
- **Linux x86_64 / macOS**: the CodeWarrior tools are Windows programs and run through the pinned **wibo**; nothing to
  install.
- **Linux aarch64** (no wibo build is pinned): install **wine**, which the template uses when there is no wibo:
  Debian / Ubuntu `sudo apt install -y wine`, Arch `sudo pacman -S --needed wine`, Fedora `sudo dnf install -y wine`.
- A ninja from your package manager also works (`sudo apt install ninja-build`, `sudo pacman -S ninja`,
  `sudo dnf install ninja-build`, `brew install ninja`) if you pass `--ninja $(command -v ninja)` to `configure.py`;
  batch 9 always uses the pinned one.
- Tested on Windows (Oct 28, 2026) and on Ubuntu 26.04 x86_64 through wibo (Oct 30, 2026, the full `from_scratch.sh`
  run below). The aarch64 / wine and macOS paths follow the template's own handling and are not yet run.

## Extracting the disc on Linux / macOS

`tools/setup.sh` extracts the ISO with the pinned decomp-toolkit (`dtk disc extract`), which writes the same
`Custom Robo extracted/` layout as Dolphin's `DolphinTool` (`sys/boot.bin`, `bi2.bin`, `apploader.img`, `fst.bin`,
`main.dol` and `files/`): checked file by file against the Windows extraction, all 920 files byte-identical. If that
fails it falls back to a `dolphin-tool` on `PATH`, `/Applications/Dolphin.app`, or the Flatpak
(`org.DolphinEmu.dolphin-emu`, run as `flatpak run --command=dolphin-tool`); you can also pass a DolphinTool file or a
Dolphin folder as the second argument of `setup.sh` to use it instead of dtk. Or extract the disc any other way into
`Custom Robo extracted/` (it must contain `sys/main.dol` and `files/`) and re-run: setup skips that step when the
folder exists.

## Clone, set up, run

Windows:

```powershell
git clone https://github.com/YourLocalDundee/custom-robo-decomp.git
cd custom-robo-decomp
pwsh tools/from_scratch.ps1 -Iso "D:\Games\Custom Robo (USA).iso"
```

Linux / macOS:

```bash
git clone https://github.com/YourLocalDundee/custom-robo-decomp.git
cd custom-robo-decomp
tools/from_scratch.sh "$HOME/Games/Custom Robo (USA).iso"
```

Or step by step: `pwsh tools/setup.ps1 -Iso ...`, `pwsh tools/batches/run_all.ps1`, `pwsh tools/checksums.ps1 -Compare`
(Windows), or `tools/setup.sh "<iso>"`, `tools/batches/run_all.sh`, `tools/checksums.sh --compare` (Linux / macOS).

## Limits and status

- **Verified end to end on Windows and on Ubuntu 26.04 (x86_64).** The Linux run (Oct 30, 2026, a 4-core machine
  without root: JDK from the tarball above, nothing else installed beyond git, python3, curl, unzip):
  `tools/from_scratch.sh` ended with PASS and `tools/checksums.sh --compare` reported every file matching
  `docs/expected_checksums.txt`, batch 11 and the glTF / preview steps skipped as described below. Arch, Fedora and
  macOS: the package lists above are from their package indexes and the scripts have no distro-specific code, but
  they have not been run there yet.
- Batch 11 (the shifted build booted in Dolphin) is Windows-only (`tools/boot_check.ps1` reads Dolphin's windows through
  the Win32 API) and is skipped on Linux / macOS; the static shift test in batch 9 still runs. On Windows it uses the
  pinned Dolphin that `setup.ps1` downloads (`tools/dolphin/Dolphin-x64`), or `-DolphinDir` / `CR_DOLPHIN_DIR`.
- Model export to glTF (`tools/export_models.py`) uses HSDRawViewer, a Windows (WinForms) program: on Linux / macOS
  `setup.sh` skips building it and `from_scratch.sh` skips the glTF and preview steps. Decompile, naming, archives,
  `.dat` models, mods and patches work wherever the scripts run.
- The outputs are byte-identical across OSes on purpose: the reports the Windows scripts wrote with CRLF line ends are
  written with CRLF by the Python tools, the Ghidra scripts and the bash batches too, and two lists the PowerShell
  batches sort in Windows culture order are sorted the same way by `tools/ps_sort.py`.
