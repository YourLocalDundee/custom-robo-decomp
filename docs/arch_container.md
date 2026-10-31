# Arch test container

How the Arch Linux path ([INSTALL.md](INSTALL.md#arch-hyprland-or-kde-any-terminal-yay--pacman)) is tested without an Arch machine: a
Docker image built from the official `archlinux` image that does exactly what the Arch section tells a desktop user to
do, as a normal user with sudo. The recipe is [tools/arch_container/Dockerfile](../tools/arch_container/Dockerfile).
No game data is in the image: the ISO (and, to save time, a download cache) is mounted when it runs.

## What the image does

| Stage (`--target`) | Steps | Tag used here |
|---|---|---|
| `base` | `archlinux` at a fixed digest, `pacman -Syu sudo`, user `friend` (uid 1000) with sudo | |
| `yay` | yay from the AUR as its README says (`sudo pacman -S --needed git base-devel`, `git clone https://aur.archlinux.org/yay-bin.git`, `makepkg -si`); fish and zsh (only to have them to test from); `git clone` of the repo (build argument `REF`, default `main`) | |
| `yay-bash` (default) | the documented `yay -S --needed ...` line run from bash, then `bash tools/setup.sh /dev/null` (prints every tool's version, then stops at the ISO check) | `custom-robo-arch` |
| `yay-zsh`, `yay-fish` | the same two commands run from zsh / from fish | `custom-robo-arch:zsh`, `:fish` |
| `pacman` | the pacman alternative `sudo pacman -S --needed ...` from bash, the clone, the same check | `custom-robo-arch:pacman` |

Each documented install line is run unchanged, with `yes ''` piped in to give each prompt its default answer, as
pressing Enter would.

## Build

```bash
cd tools/arch_container
docker build -t custom-robo-arch .
docker build --target yay-zsh -t custom-robo-arch:zsh .
docker build --target yay-fish -t custom-robo-arch:fish .
docker build --target pacman -t custom-robo-arch:pacman .
```

`--build-arg REF=<branch>` tests a branch instead of `main`; `--no-cache` rebuilds from the pinned base with today's
packages. The tool check is in the build log (`--progress=plain`).

## Run the pipeline in it

The ISO folder read-only at `/games`, a folder for the log at `/logs` (writable by uid 1000), and optionally the
`tools/downloads/` of an existing clone read-only at `/cache/downloads`. The cache is COPIED into the clone's own,
writable `tools/downloads/`: setup reuses a file there when its SHA-256 matches `tools/pins.txt` and downloads every
file the cache lacks into the same folder (a symlink to the read-only mount fails there with `curl: (23)`). The git
references are still cloned. A cache from a Linux clone has every download; one from a Windows clone helps only with
`compilers_*.zip` (its other files are the Windows builds, and its Ghidra is unpacked, not a `ghidra.zip`):

```bash
docker run -d --name cr-arch -v "$HOME/Games:/games:ro" -v "$HOME/archtest/logs:/logs" \
  -v "$HOME/src/custom-robo-decomp/tools/downloads:/cache/downloads:ro" custom-robo-arch sleep infinity
docker exec cr-arch bash -c 'git pull -q; mkdir -p tools/downloads; [ -d /cache/downloads ] && cp -a /cache/downloads/. tools/downloads/; nohup bash tools/from_scratch.sh "/games/Custom Robo (USA).iso" > /logs/from_scratch.log 2>&1 &'
tail -f "$HOME/archtest/logs/from_scratch.log"
```

From Git Bash on Windows (Docker Desktop), put `MSYS_NO_PATHCONV=1` in front of `docker run` / `docker exec`, so the
container paths (`/games`, `/home/...`) are not turned into Windows paths; the host folders are then given as
`D:/...`.

It ends with the PASS / FAIL table of `tools/from_scratch.sh`; `docker rm -f cr-arch` removes the container.

## Results

- Oct 30, 2026, base `archlinux@sha256:e82623936fa1a69700b48e98b058df6eaab18b737c27473a7234a40fdecb9413`
  (`archlinux:latest`, created Oct 27, 2026), upgraded to that day's packages: all four stages built. yay-bin 13.1.0
  from the AUR; the yay line installed jdk21-openjdk 21.0.13.0.3.u1-1, git 2.56.0-1, unzip 6.0-23, python 3.14.8-1
  (curl 8.23.0-1 was already there; bash 5.3.20-1, glibc 2.44) with no prompt beyond "Proceed with installation?",
  from bash, zsh 5.9.2 and fish 4.9.3 alike; the pacman line the same from bash. In every stage
  `bash tools/setup.sh` printed `ok` for bash, git, python3, java, curl and unzip and then stopped at the ISO check
  (as it should with `/dev/null`).
- The same day, in the `pacman` image with an Ubuntu clone's downloaded tools mounted read-only: dtk 1.8.4, ninja
  1.13.2, objdiff-cli 3.6.1 and wibo running CodeWarrior's `mwcceppc.exe` all started; Ghidra 12.1.4's
  `analyzeHeadless` found the JDK; `python3 -m venv` plus `pip install -r tools/requirements-preview.txt` (setup step 6)
  installed every pinned package on Python 3.14.8.
- Oct 30-31, 2026, the first full pipeline run, on Docker Desktop (Docker Engine 29.9.2) on Windows 11: the default
  image built from the pinned base, a fresh clone of main at f421534. `tools/from_scratch.sh`: setup PASS (with a
  Windows clone's cache mounted: ghidra.zip, gcloader.zip and the Linux tools downloaded, compilers_*.zip reused; the
  symlinked cache of the old recipe stopped with `curl: (23)` on ghidra.zip, so the cache is copied now); run_all
  FAIL in batch 9 after 12 minutes: `CR_MATCHING names objects that are not in splits.txt: ['dolphin/dvd/dvdfs.c']`.
  Cause (plan.md step 68b): not Linux itself but the name batches reading other reference files than the Windows
  machine (tp / pikmin2 clones there still config-only from before step 68) and committed names older than the hand
  lines, plus one filesystem-order citation in match_lib_rt.py. After the fix (branch linux-fix, merged with main
  90aeae3), `tools/batches/run_all.sh` in the same container and `run_all.ps1` on Windows gave byte-identical
  name files, `decomp/config/GXCE01/{splits,symbols}.txt`, `config.yml` and batch 9 reports: retail SHA-1,
  617 objects from C, names 0 FAIL. Its batch 12 was stopped by hand: wibo compiled the drafts at 3-4 a minute
  under Docker Desktop (Windows: 1,703 in 40 minutes). On the final commit (the Windows run's outputs and checksums)
  `tools/batches/batch9_decomp_build.sh` in the container: retail SHA-1, 617 objects from C, no file changed;
  `tools/checksums.sh --compare`: all 33 files match.
- Oct 30, 2026, late: main's merge commit df9dc1c (620 objects from C) checked in the same container after
  `tools/check_refs.py` (17 clones at their pins): `batch9_decomp_build.sh` retail SHA-1, 620 objects, static shift PASS;
  then `tools/rebuild_decomp.sh` (the Ghidra export at the new names) and `tools/checksums.sh --compare`: all 33 files
  match Windows byte for byte.
