# Install the tools

What you need, per OS, in the order to run it. The short version is in the [README](../README.md#install-the-tools);
this page has the details and the checks. Nothing here is shipped as a prebuilt binary: tools come from your package
manager, or from the pinned, SHA-256-checked downloads in [tools/pins.txt](../tools/pins.txt) that `tools/setup.ps1` fetches.

## What is needed

| Tool | Version | Why | Who installs it |
|---|---|---|---|
| git | any | clones this repo and the pinned reference decomps | you |
| Python | 3.11+ (the command must be `python`) | every `tools/*.py`, the preview venv | you |
| PowerShell 7 (`pwsh`) | 7+ | the setup / batch / checksum scripts (Windows PowerShell 5.1 is not enough) | you |
| Java JDK | 21+ | runs Ghidra 12.1 (headless decompile) | you |
| .NET SDK | 8+ | builds HSDRawViewer (glTF model export). **Windows only** | you |
| tar | any | unpacks zip / 7z on Windows (built into Windows 10/11) | Windows itself |
| unzip | any | unpacks Ghidra on Linux / macOS (keeps the executable bits) | you (Linux / macOS) |
| Dolphin's `dolphin-tool` | any recent | extracts the disc. Windows: the pinned official build is downloaded for you | you on Linux / macOS |
| Ghidra 12.1.4 + GameCube loader 1.3.1 | pinned | decompiler | `setup.ps1` (pinned) |
| decomp-toolkit (dtk) 1.8.4 | pinned, one binary per OS | function boundaries / SDK names | `setup.ps1` (pinned) |
| HSDLib / HSDRawViewer | pinned commit | model export, built from source | `setup.ps1` (Windows) |
| Python packages (numpy, trimesh, ...) | pinned in `tools/requirements-preview.txt` | preview renders | `setup.ps1` (venv) |
| ninja, objdiff-cli, binutils, CodeWarrior compilers, wibo | pinned (see "Phase 4 build tools") | builds main.dol in `decomp/` | `setup.ps1` (pinned) |
| wine | any recent | runs CodeWarrior on Linux aarch64 only (x86_64 / macOS use the pinned wibo) | you (Linux aarch64) |

`pwsh tools/setup.ps1 -Iso ...` starts by printing each tool's version and stops with one list of what is missing.

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
sudo apt install -y git curl unzip python3 python3-venv python-is-python3 openjdk-21-jdk
# PowerShell is not in the distro repos: add Microsoft's repo (Ubuntu: use ubuntu/<release>, e.g. ubuntu/24.04 or ubuntu/26.04, instead of debian/12)
curl -fsSLO https://packages.microsoft.com/config/debian/12/packages-microsoft-prod.deb
sudo dpkg -i packages-microsoft-prod.deb && rm packages-microsoft-prod.deb
sudo apt update && sudo apt install -y powershell
# Dolphin, for its disc tool (see "Dolphin on Linux")
sudo apt install -y dolphin-emu
```

- `openjdk-21-jdk` exists on Ubuntu 24.04 and Debian 13; Debian 12 only has 17, so use Temurin 21 (Adoptium apt repo) there.
- Instead of the Microsoft repo: `sudo snap install powershell --classic`.
- No .NET is needed on Linux: model export is Windows-only (see "Limits and status").

## Arch (pacman, then yay for AUR)

```bash
sudo pacman -S --needed git unzip python jdk21-openjdk dolphin-emu
# PowerShell is AUR-only
yay -S --needed powershell-bin
# only if "dolphin-tool" is missing after installing dolphin-emu:
# yay -S dolphin-emu-git
```

If several JDKs are installed: `sudo archlinux-java set java-21-openjdk`. `python` is already Python 3. Ghidra itself is
downloaded at the pinned version by `setup.ps1`; do not use Arch's `ghidra` package (a different version changes the output).

## Fedora (dnf)

```bash
sudo dnf install -y git unzip python3 python-unversioned-command java-21-openjdk-devel dolphin-emu dolphin-emu-tool
# PowerShell: Microsoft's rpm repo (not in Fedora's own repos)
sudo rpm --import https://packages.microsoft.com/keys/microsoft.asc
curl -fsSL https://packages.microsoft.com/config/rhel/9/prod.repo | sudo tee /etc/yum.repos.d/microsoft.repo
sudo dnf install -y powershell
```

`dolphin-emu-tool` is the split-out `dolphin-tool` package. Fedora's `dolphin-emu` may need RPM Fusion enabled.

## macOS (Homebrew) - best effort

```bash
brew install git python@3.12 openjdk@21 unzip
brew install --cask powershell dolphin
# openjdk@21 is keg-only: make it visible, and make "python" mean python3
sudo ln -sfn "$(brew --prefix openjdk@21)/libexec/openjdk.jdk" /Library/Java/JavaVirtualMachines/openjdk-21.jdk
export PATH="$(brew --prefix python@3.12)/libexec/bin:$PATH"
```

`setup.ps1` finds `/Applications/Dolphin.app/Contents/MacOS/DolphinTool` by itself.

## Any other system

Install: git, Python 3.11+ (as `python`), PowerShell 7, a JDK 21+, unzip, and Dolphin (for `dolphin-tool`; or pass
`-Dolphin /path/to/dolphin-tool`). On Windows also the .NET SDK 8+.

## Phase 4 build tools (decomp/, batch 9)

Nothing to install by hand: `setup.ps1` step 8 (`tools/setup_decomp.ps1`) downloads every build tool at the version
pinned in `tools/pins.txt` and checks its SHA-256. The versions are the ones the pinned `encounter/dtk-template` asks for.

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
- Tested on Windows only (Oct 28, 2026). The Linux / macOS paths follow the template's own wibo / wine handling and are
  not yet run (plan: minime + Arch/yay, after the Phase 4 plan is done).

## Dolphin on Linux

`setup.ps1` looks for `dolphin-tool` on `PATH`, then the Flatpak (`org.DolphinEmu.dolphin-emu`, run as
`flatpak run --command=dolphin-tool`), then uses `-Dolphin <folder or file>` if you pass it. If your distro's Dolphin
has no `dolphin-tool`, extract the disc any other way into `Custom Robo extracted/` (it must contain `sys/main.dol`
and `files/`) and re-run: setup skips that step when the folder exists.

## Clone, set up, run

```bash
git clone https://github.com/YourLocalDundee/custom-robo-decomp.git
cd custom-robo-decomp
pwsh tools/from_scratch.ps1 -Iso "$HOME/Games/Custom Robo (USA).iso"
```

Or step by step: `pwsh tools/setup.ps1 -Iso ...`, `pwsh tools/batches/run_all.ps1`, `pwsh tools/checksums.ps1 -Compare`.

## Limits and status

- **Windows is the tested platform.** Linux / macOS support was written from the docs; the first Linux end-to-end test (Ubuntu 26.04) is in progress (Oct 29, 2026).
- Batch 11 (the shifted build booted in Dolphin) is Windows-only (`tools/boot_check.ps1` reads Dolphin's windows through the Win32 API) and is skipped on Linux / macOS; the static shift test in batch 9 still runs. On Windows it uses the pinned Dolphin that `setup.ps1` downloads (`tools/dolphin/Dolphin-x64`), or `-DolphinDir` / `CR_DOLPHIN_DIR`.
- Model export to glTF (`tools/export_models.py`) uses HSDRawViewer, a Windows (WinForms) program: on Linux / macOS
  `setup.ps1` skips building it and `from_scratch.ps1` skips the glTF and preview steps. Decompile, naming, archives,
  `.dat` models, mods and patches are meant to work wherever the scripts run.
- Not checked against a live repo: the Microsoft repo URLs for your exact release, `dolphin-tool` being inside the
  Debian / Ubuntu / Arch `dolphin-emu` package, the AUR name `powershell-bin`, and the Homebrew cask names.
  `dolphin-emu-tool` on Fedora was found on packages.fedoraproject.org.
