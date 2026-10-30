# Arch Linux test image for the Linux path (docs/arch_container.md). It does what docs/INSTALL.md "Arch" tells an Arch
# desktop user to do, as a normal user with sudo: yay built from the AUR as yay's README says, then the documented
# yay command, run from bash, from zsh and from fish (one stage each, the login shell does not matter), then the
# prerequisite check of tools/setup.sh from that shell; and the pacman alternative in its own stage.
# No game data goes into the image: the ISO and the download cache are mounted at run time (docs/arch_container.md).
#   docker build -t custom-robo-arch .                          yay + bash: the image the pipeline test runs in
#   docker build --target yay-zsh  -t custom-robo-arch:zsh .    yay command and check from zsh
#   docker build --target yay-fish -t custom-robo-arch:fish .   yay command and check from fish
#   docker build --target pacman   -t custom-robo-arch:pacman . the pacman alternative (bash)
# Sources: yay-bin + "pacman -S --needed git base-devel" + makepkg -si: https://github.com/Jguer/yay ("Installation");
# package names and repos: https://archlinux.org/packages/ (git, curl, unzip, python, jdk21-openjdk; fish, zsh) and
# https://aur.archlinux.org/packages/yay-bin; --needed: https://wiki.archlinux.org/title/Pacman ("Installing packages").
ARG BASE=archlinux@sha256:e82623936fa1a69700b48e98b058df6eaab18b737c27473a7234a40fdecb9413
ARG REPO=https://github.com/YourLocalDundee/custom-robo-decomp.git
ARG REF=main

FROM ${BASE} AS base
# what a desktop install already has: an up-to-date system, sudo, and a normal user allowed to use it (uid 1000)
RUN pacman -Syu --noconfirm --needed sudo \
 && useradd -m -u 1000 -s /bin/bash friend \
 && printf 'friend ALL=(ALL) NOPASSWD: ALL\n' > /etc/sudoers.d/friend && chmod 440 /etc/sudoers.d/friend
USER friend
WORKDIR /home/friend

FROM base AS yay
# yay is in the AUR, not the repos: yay's README, binary build; fish and zsh only to have those shells to test from.
# Then the repo, as the README clones it (REF: a branch or tag to test instead of main).
ARG REPO
ARG REF
RUN yes '' | sudo pacman -S --needed git base-devel \
 && git clone https://aur.archlinux.org/yay-bin.git && cd yay-bin && makepkg -si --noconfirm \
 && cd .. && rm -rf yay-bin \
 && sudo pacman -S --needed --noconfirm fish zsh \
 && git clone -b "$REF" "$REPO" custom-robo-decomp
WORKDIR /home/friend/custom-robo-decomp

# Each stage: the documented command unchanged (yes '' answers each prompt with its default), then the tool check.
# setup.sh is given /dev/null as the ISO: it prints every tool's version, then stops at the ISO check (expected).
FROM yay AS yay-zsh
SHELL ["/usr/bin/zsh", "-c"]
RUN yes '' | yay -S --needed git curl unzip python jdk21-openjdk
RUN bash tools/setup.sh /dev/null 2>&1 | sed -n '/^checking tools/,$p'; java -version 2>&1 | head -n 1; echo "shell: $0"

FROM yay AS yay-fish
SHELL ["/usr/bin/fish", "-c"]
RUN yes '' | yay -S --needed git curl unzip python jdk21-openjdk
RUN bash tools/setup.sh /dev/null 2>&1 | sed -n '/^checking tools/,$p'; java -version 2>&1 | head -n 1; echo "shell: fish $version"

FROM base AS pacman
ARG REPO
ARG REF
RUN yes '' | sudo pacman -S --needed git curl unzip python jdk21-openjdk \
 && git clone -b "$REF" "$REPO" custom-robo-decomp
WORKDIR /home/friend/custom-robo-decomp
RUN bash tools/setup.sh /dev/null 2>&1 | sed -n '/^checking tools/,$p'; java -version 2>&1 | head -n 1

# default target, last: yay from bash, the image tools/from_scratch.sh runs in
FROM yay AS yay-bash
SHELL ["/bin/bash", "-c"]
RUN yes '' | yay -S --needed git curl unzip python jdk21-openjdk
RUN bash tools/setup.sh /dev/null 2>&1 | sed -n '/^checking tools/,$p'; java -version 2>&1 | head -n 1; echo "shell: $0 $BASH_VERSION"
