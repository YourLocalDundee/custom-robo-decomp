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
# PowerShell is not in the distro repos: add Microsoft's repo (Ubuntu: use e.g. ubuntu/24.04 instead of debian/12)
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

- **Windows is the tested platform.** Linux / macOS support was written from the docs and has not been run end to end.
- Model export to glTF (`tools/export_models.py`) uses HSDRawViewer, a Windows (WinForms) program: on Linux / macOS
  `setup.ps1` skips building it and `from_scratch.ps1` skips the glTF and preview steps. Decompile, naming, archives,
  `.dat` models, mods and patches are meant to work wherever the scripts run.
- Not checked against a live repo: the Microsoft repo URLs for your exact release, `dolphin-tool` being inside the
  Debian / Ubuntu / Arch `dolphin-emu` package, the AUR name `powershell-bin`, and the Homebrew cask names.
  `dolphin-emu-tool` on Fedora was found on packages.fedoraproject.org.
