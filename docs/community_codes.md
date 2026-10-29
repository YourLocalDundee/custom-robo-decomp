# Community codes, texture mods and Gecko tooling (research notes)

> **Correction (Oct 29, 2026):** the code-size limit below (Dolphin cheat area, ~6 KB) is not the real limit. Project Hive loads a ~140 KB GXCE01.GCT through Riivolution and already adds new parts and robo classes by code. The Discord code channel is analysed in [community/discord_codes.md](community/discord_codes.md): 33 hooks mapped to our functions, the robo stats table at 0x801C6C44 (0x134 per robo), and the 4-robo array at 0x8024E120 + n x 0x33520.

Research date: Oct 28, 2026. Target: Custom Robo, US GXCE01 v1.0 (JP GXCJ01 noted where relevant).
Machine-readable list: `docs/community/codes_gxce01.tsv` (one row per code: raw lines, decrypted lines, Gecko form, addresses, decomp hit).

Status words: **verified** = checked against our decomp / the clean `main.dol` in this repo, or read directly from the cited page;
**unverified** = from a search snippet, a forum claim, or general knowledge that this research did not confirm against a fetched source.

## Summary

- The Discord claim ("we can add things with Action Replay and Gecko codes for mods with textures and things, just not models") is **half right**.
  Codes can change RAM values and patch code, and Dolphin can replace textures by hash without touching the disc. But **models are already
  moddable in this project by file replacement** (`tools/make_mod.py`, and earlier Zoeyism's Model Swapper). Models can't practically be done
  **through codes**, because a robo entry (186-233 KB) is about 30-40 times bigger than all the space a code list gets (about 6 KB, see "Models").
- **Public code lists are thin.** There are 16 codes for GXCE01 in total:
  - **15 Action Replay codes** (encrypted, May 2004, cheathappens.com). This research **decrypted all 15**, and every block passes the AR CRC check.
  - **1 Gecko code** (16:9 widescreen, in Dolphin's own `GameSettings/GXCE01.ini` and on the Dolphin wiki).
  - **None found** on codes.rc24.xyz (the GeckoCodes mirror returned no list), gamehacking.org (403), or for GXCJ01 (Dolphin has no `GXCJ01.ini`).
- **13 distinct target addresses, and all 13 map to code or globals in our decomp** (`decompiled/index.db`). Every data target is still an unnamed
  `DAT_`/`FLOAT_`. The codes give good **naming evidence leads**: per-player robo HP (a 4-entry array, 0x33520-byte stride), the parts-owned
  bitsets, an unlock-flags word, the camera aspect float and the 2D ortho setup.
- **Texture mods**: one public Dolphin HD texture pack exists (slightlyflightyone, Dec 2024, WIP). There are no public recolour packs or texture
  tools specific to this game. Our own route (edit the TObj inside the model `.dat`, then `make_mod.py`) already works for recolours.
- **Model mods that exist**: Zoeyism's Custom Robo (GC) Model Swapper (Feb 2023, CC0). It overwrites bytes in `rpg_t_models.BIN` / `rpg_f_models.BIN`,
  and the replacement must be smaller than the original. Its "Female Main Character" hack (Dec 2022) is the only public model hack found.
- **Gecko tooling**: we can build C2 inserts from assembly with devkitPPC `powerpc-eabi-as` (JLaferri's `gecko` bundles it), or inject
  permanently into `main.dol` like Melee Code Manager. `main.dol` uses only 2 of 7 text and 6 of 11 data section slots, so a new section is easy.
  The open question is where in RAM to put it (see the proposal).

## Codes found

| # | Name | Type | Source | Target(s) | What it does (decoded) |
|---|---|---|---|---|---|
| 1 | (M) master code | AR, encrypted | [cheathappens][ch] | `8016C860` | AR hook (`C4` master code). Not needed in Dolphin. |
| 2 | Unlock Bonuses | AR, encrypted | [cheathappens][ch] | `80321D44` | 32-bit write `FFFFFFFF` |
| 3,5,7,9 | Robo 1-4: Infinite Health | AR, encrypted | [cheathappens][ch] | `8024E194`, `802816B4`, `802B4BD4`, `802E80F4` | 32-bit write `447A0000` = 1000.0f |
| 4,6,8,10 | Robo 1-4: Dies With 1 Hit | AR, encrypted | [cheathappens][ch] | same four | if HP > 1.0f (unsigned compare of the float bits), write 1.0f |
| 11-15 | All Body / Gun / Bomb / Pod / Leg Parts | AR, encrypted | [cheathappens][ch] | `803BFB9C`, `803BFBBC`, `803BFBDC`, `803BFBFC`, `803BFC1C` | 16-bit fill `FFFF` x4 = 8 bytes of `FF` (64 bits) |
| 16 | 16:9 Widescreen | Gecko | [Dolphin GXCE01.ini][ini], [Dolphin wiki][dw] | `8044C438`, `80091780` | float write + C2 insert, see below |

Not found or not retrievable:
- [codes.rc24.xyz GXCE01.txt][rc24] returned no codes.
- gamehacking.org (403) and romhacking.net (403) could not be fetched.
- The [TCRF page][tcrf] could not be read: the fetch returned no game content. A search snippet says TCRF lists a **JP-only AR code for a hidden
  debug-mode menu option (it stops the game)** and an option-menu code that crashes (**unverified**, lines not seen).
- A [Dolphin forum thread][df1] has users saying AR codes "won't work for custom robo" in Dolphin, and one says Gecko didn't either. There are no
  code lines in it. One likely cause (**unverified**): pasting encrypted lines together with the (M) master code. Dolphin drops master codes
  ([ActionReplay.cpp][arcpp], `SUB_MASTER_CODE` is "not needed").

### Decrypting the AR codes

The AR codes are in the encrypted `XXXX-XXXX-XXXXX` form. I decrypted them with a Python port of Dolphin's [ARDecrypt.cpp][ardec] (GCNcrypt by
Parasyte), kept in the session scratchpad and not added to the repo (GPL-2.0). For every one of the 15 blocks, the parity check and the CRC check
pass, and they all give game id `0x2AB`, region 0. The first decrypted line of each block (`0556D8F4 88000000`, `0556D8F5 08000000`, ...) is the
AR verification/ID line, and Dolphin skips it. The remaining lines:

```
(M)                       C416C860 0000FF01
Unlock Bonuses            04321D44 FFFFFFFF
Robo 1: Infinite Health   0424E194 447A0000
Robo 1: Dies With 1 Hit   3424E194 3F800000
                          0424E194 3F800000
Robo 2: Infinite Health   042816B4 447A0000
Robo 2: Dies With 1 Hit   342816B4 3F800000 / 042816B4 3F800000
Robo 3: Infinite Health   042B4BD4 447A0000
Robo 3: Dies With 1 Hit   342B4BD4 3F800000 / 042B4BD4 3F800000
Robo 4: Infinite Health   042E80F4 447A0000
Robo 4: Dies With 1 Hit   342E80F4 3F800000 / 042E80F4 3F800000
All Body Types            023BFB9C 0003FFFF
All Gun Parts             023BFBBC 0003FFFF
All Bomb Parts            023BFBDC 0003FFFF
All Pod Parts             023BFBFC 0003FFFF
All Leg Parts             023BFC1C 0003FFFF
```

How the plain AR lines decode ([Dolphin ActionReplay.cpp][arcpp]): the address word holds the address in bits 0-24 (`| 0x80000000`), the size in
bits 25-26 (0 = 8-bit, 1 = 16-bit, 2 = 32-bit), the type in bits 27-29 (0 = write, 1-7 = conditionals) and the subtype in bits 30-31.
- `04xxxxxx`: type 0 / subtype 0, 32-bit write.
- `02xxxxxx YYYYZZZZ`: 16-bit write of `ZZZZ`, `YYYY`+1 times (so `0003FFFF` = 4 halfwords).
- `34xxxxxx`: type 6 (greater than, unsigned), 32-bit, subtype 0. If the test fails, the next line is skipped.
- `C4xxxxxx`: type 0 / subtype 3 = master code, which Dolphin ignores.

**Gecko forms.** Plain AR `04`/`02` writes have the same layout as Gecko `04`/`02` writes. The Dolphin forum confirms "00/01 8-bit, 02/03 16-bit,
04/05 32-bit" for Gecko ([forum][df2]), so lines 2, 3, 5, 7, 9 and 11-15 can be pasted into a `[Gecko]` section unchanged. For the "1 hit" codes,
the Gecko form uses an if-greater block: `24xxxxxx 3F800000` / `04xxxxxx 3F800000` / `E2000001 00000000`. That Gecko form is **unverified**: it
comes from the usual Gecko codetype table (20 = if equal, 22 = not equal, 24 = greater, 26 = lower, E2 = end-if), which this research could not
fetch from an authoritative page (geckocodes.org now redirects to gamehacking.org, which returned 403).

### The widescreen Gecko code, decoded

From [GXCE01.ini][ini]:
```
0444C438 3FD70A3E      04 = 32-bit write: FLOAT_8044c438 = 1.68 (clean DOL value 0x3FA147AE = 1.26; x 4/3)
C2091780 00000006      C2 = insert ASM at 80091780, 6 lines (48 bytes) follow:
3DC03F40               lis   r14, 0x3F40          ; r14 = 0.75f
91C20000               stw   r14, 0(r2)           ; park it at r2+0 = 0x804581A0
C2220000               lfs   f17, 0(r2)
C0030000               lfs   f0, 0(r3)            ; r3 = the 4x4 ortho matrix
EC110032               fmuls f0, f17, f0
D0030000               stfs  f0, 0(r3)            ; m[0][0] *= 0.75
C003000C               lfs   f0, 12(r3)
EC110032               fmuls f0, f17, f0
D003000C               stfs  f0, 12(r3)           ; m[0][3] *= 0.75
38800001               li    r4, 1                ; the replaced original instruction (verified in main.dol)
60000000 00000000      nop + the slot the code handler fills with the branch back
```
Verified against the clean `main.dol`: `80091780` holds `38800001`, and `8044C438` holds `3FA147AE` (1.26). `0x804581A0` (r2+0) lies past the end
of the DOL image (bss ends at `80452AC0`), so the code uses an unowned scratch word above the game's static data. It probably works because that
word is never used. This is a possible conflict (**unverified**). A cleaner version would keep the constant inside the code block.

## Address cross-reference with our decomp

Lookups: `python -I tools\find.py refs <addr>`, `func`, `callers`, plus a read-only containing-function query on `decompiled/index.db`.
`find.py func` only accepts function entry points.

| Address | Code(s) | What our decomp shows | Current name | Fits? |
|---|---|---|---|---|
| `8016C860` | (M) | last instruction (`blr`, verified) of `SIGetStatus` `8016C7E8` | SDK name (layout-proof) | yes; a typical AR hook point, nothing to name |
| `80321D44` | Unlock Bonuses | 33 refs / 11 funcs: `cr_SEQ_BATTLE_FrameA` (`if (DAT_803bf942 & 1 && !(x & 0xe)) {...; x |= 0xe;}`), `cr_SYS_BATTLE_RESULT` (`x |= 0xe`), `FUN_80011ac0`, `FUN_800f0dec`, `FUN_8011725c`, ... | `DAT_80321d44` | unnamed. Lead: bonus/unlock flag word |
| `8024E194` | Robo 1 HP | 43 refs / 12 funcs (`FUN_800392b0`, `FUN_8003aba0`, `FUN_8004dc78`, `FUN_8004ede4`, `FUN_8004f6d4`, ...). Indexed as `(&DAT_8024e194)[i * 0xcd48]` (floats), so the stride is 0x33520 bytes per player. Sibling fields `DAT_8024e190`, `DAT_8024e1ad`, `DAT_8024e2f8` (flags), `DAT_8024e358` use the same stride | `DAT_8024e194` | unnamed. Lead: per-player battle robo record x4, HP float at record+? (record base not pinned; `8024E194` is bss start `8024DB20` + 0x674) |
| `802816B4`, `802B4BD4`, `802E80F4` | Robo 2-4 HP | = `8024E194` + 1/2/3 x 0x33520 (verified arithmetic). One direct ref each, all in `FUN_8004ede4` (converts the four HPs to unsigned; looks like a HUD/result readout) | `DAT_` | same array. Lead: `FUN_8004ede4` reads all players' HP |
| `803BFB9C`, `803BFBBC`, `803BFBDC`, `803BFBFC`, `803BFC1C` | All Body/Gun/Bomb/Pod/Leg | `FUN_800a3c0c` clears 20 bitsets of 64 bits (`803BFB9C` ... `803BFC34`, stride 8) bit by bit, then sets bit 0 of exactly these five. So it is **5 categories x 4 bitsets x 64 bits**, and the first bitset of each category = parts owned. It also picks the save block `DAT_8044f794 = &DAT_803bf944` or `&DAT_803bfa10`. Readers: `FUN_800b0cf8`, `FUN_800bbd04`, `FUN_800bc038`, `FUN_800f21c8`, `FUN_800f8e4c` | `DAT_` | unnamed. Leads: parts-owned bitsets; `FUN_800a3c0c` = new-game/parts init (only caller `FUN_800a0e64`) |
| (helpers) | - | `FUN_80023228(base, i)`: `base[i>>5] |= 1 << (i & 31)`; `FUN_800231ec`: the same with `&= ~` | `FUN_` | leads: bitset set-bit / clear-bit |
| `8044C438` | Widescreen | float 1.26 in `.sdata`, read by 13 camera funcs: `cr_Cam_Init`, `cr_CAMMODE_STAGE_INFO_Start`, `cr_CAMMODE_STAGE_SELECT_Start`, `cr_CAMMODE_SNR_CHARA_Start`, `cr_CAMMODE_ROT_WAIT_Frame`, `FUN_8002419c`, ... | `FLOAT_8044c438` | unnamed. Lead: camera aspect ratio (the code multiplies it by 4/3 for 16:9) |
| `80091780` | Widescreen | inside `FUN_80091518` (2D setup: `C_MTXOrtho(...)`, `GXSetProjection(mtx, GX_ORTHOGRAPHIC)`, scissor/viewport 640x480). `80091780` is the `li r4,1` argument of `GXSetProjection` | `FUN_80091518` | unnamed. Lead: 2D/HUD ortho draw setup |

Result: **13 of 13 addresses match our decomp.** None of them has a game-specific name yet. Do **not** add these to `symbols/`. Under the naming
policy, they are leads, and the code plus the decomp lines above are the evidence.

## Texture modding routes

1. **Dolphin custom textures (no disc changes).**
   - Turn on Dump Textures, play, then edit the files in `Dump/Textures/GXCE01/` and put them in `Load/Textures/GXCE01/` (or the 3-letter
     `GXC/` folder for a pack that works on every region).
   - Enable "Load Custom Textures". Files are named `tex1_<W>x<H>_<hash>_<fmt>`; paletted textures get two hashes. DDS/BC7 and PNG work.
   - Sources: [recalbox guide][rb], [Dolphin forum][dft].
   - Works only in Dolphin, not on real hardware or Nintendont. Textures can't change model geometry.
   - The Dolphin wiki says this game needs D3D11 or OpenGL, or "textures fail to appear" ([wiki][dw]).
2. **Existing pack.**
   - "Custom Robo HD texture pack" by slightlyflightyone, Dolphin forums, Dec 18, 2024, update Dec 25, 2024 ([thread][hdt]). It is AI-upscaled,
     targets 1080p and is shipped as DDS.
   - Covers: the "A New Journey" story mode, parts, Holosseums, NPCs, font, menus, intro and title.
   - Not done: Grand Battle, Arcade and Vs.
   - Its tools and texture hashes are not documented in the thread.
3. **File replacement (works on hardware too).**
   - Robo/weapon textures are CMPR TObjs inside the model entries (plan.md "Model format"), and `make_mod.py` already rebuilds them
     (MODDING.md: "robo 0 recolour -> 43 KB patch, boots").
   - UI textures: the disc holds 590 `.tpl` files out of 915 files (counted on our extracted folder). Replacing them works today through
     `files/` in a mod; there is no `.tpl` tool yet (plan.md Phase 2).
4. **Through codes.** A Gecko code could only swap texture pointers or palette bytes in RAM after a load. No such code exists publicly.
   For this game it is strictly worse than routes 1 and 3. No recolour codes were found.

Community recolours or texture tools specific to Custom Robo: **none found**, beyond the HD pack.

## Models: why not codes, and what exists

**Why codes can't carry models (verified numbers, and a reasoned conclusion):**
- **Space.** Dolphin puts the code handler and the whole code list at `0x80001800`-`0x80003000`, which is 6 KB minus the handler. Codes that
  don't fit are skipped whole ([GeckoCode.h][gch], [GeckoCode.cpp][gcc]). Wiimm's toolset quotes "about 935 code lines" normally ([wiimm][wim]).
  A USB Loader GX user reports `.gct` files over ~3.31 KB being ignored ([GBAtemp][gbt], **unverified**). Our robo entries are **186-233 KB**
  (170 entries in `unpacked/chara`, median 197 KB), so one model is 30-40 times the whole budget.
- **Location.** Models are read from disc into heap memory at run time (`FUN_800747b0`, `FUN_80073cb8`, plan.md). Their RAM address isn't
  fixed, so plain write codes can't target them. A code would have to hook the loader and copy data in from somewhere, which brings us back
  to needing the data on disc.
- **Format.** Even small edits mean moving relocated HSD pointers (reloc table, joint path list). That is a file-level job.
- So the Discord view is right about **codes**. File replacement has no such limit, and this repo already does it (`make_mod.py`, `robo_swap.py`).

**What exists:**
- **Custom Robo (GC) Model Swapper** by Zoeyism (v1.0.0, Feb 2023, CC0; [GitHub][swp], [romhacking.net][rhu]; the RHDN page returned 403,
  details are from GitHub and a search snippet).
  - A Python/PySimpleGUI tool.
  - It works only on `rpg_t_models.BIN` and `rpg_f_models.BIN` (story-mode field characters). Swapping `chara.BIN` robos is in the code but
    never enabled.
  - It finds the target model's bytes anywhere in the archive, writes the new model over them and fills the rest with zeros. It **does not
    touch the SFD entry table**, and the replacement must be strictly smaller.
  - It does not handle `main.dol`'s copy of the entry table. We found in Phase 1 that the game reads that copy, and since the entry size is
    unchanged it still works.
  - Cross-check value: low. Its format knowledge is "proprietary, partially documented" per its README.
- **"Custom Robo: Female Main Character" hack** by Zoeyism (v1.0, Dec 26, 2022). It is listed on romhacking.net as hack 7416 (not fetched, 403).
  Download sites carry it as a pre-patched ISO (not downloaded). It is presumably made with the swapper; that is **unverified**.
- Older attempts: an older post (undated in the fetch) in the [Dolphin forum thread][df1] couldn't extract Ray 01 for 3D printing ("~400 parts stacked at the centre").
- No other model mods or decomps found. The only other Custom Robo decomp/toolkit in search results is this project's public repo.

## Gecko/AR tooling and how it would plug into tools/make_mod.py

**How Gecko lists are applied**
- **Dolphin:** `[Gecko]` and `[Gecko_Enabled]` sections in `User/GameSettings/GXCE01.ini`. Each code is a `$Name` line followed by
  `XXXXXXXX YYYYYYYY` lines; the format is the one in [GXCE01.ini][ini]. AR codes go in `[ActionReplay]`, and Dolphin accepts encrypted or
  plain lines ([ARDecrypt.cpp][ardec]).
- **Nintendont / USB Loader GX:** a binary `.gct` file (header `00D0C0DE 00D0C0DE`, the code lines, then `F0000000 00000000`), placed under
  `codes/GXCE01.gct` (**unverified** detail, general knowledge; no Nintendont doc fetched). The forum says button-activated codes may not work
  on Nintendont ([GBAtemp][gbt2], **unverified**).
- **Riivolution:** Wii-only (disc patching through the Wii's IOS). It does not apply to GameCube games (**unverified**, general knowledge).
- **Limits:** about 6 KB in Dolphin ([GeckoCode.h][gch]). Wiimm's `--gct-move` puts a large list into a section overlaying BSS
  ([wiimm][wim], MKWii-specific).

**Building C2 from source**
- **devkitPPC `powerpc-eabi-as` + `objcopy`.** JLaferri's [`gecko`][jlg] (Go) builds a whole code list from `codes.json` plus a folder of `.asm`
  files. Each file has a header giving the address and codetype, and the build fails on undefined symbols. It bundles the assembler and
  falls back to `$DEVKITPPC/bin`.
- **Python options:** [supSMSASM][sup] (C0/C2/fixed-location C2 from `.s` + linker script; needs devkitPro; archived) and [geckolibs][gl]
  (builds and parses Gecko code objects).
- **C2 limit** ([supSMSASM][sup]): C2 code runs from an unknown address, so `b`/`bl` to absolute targets must go through a register
  (`mtctr`/`bctrl`), or the code must be placed with `06` and branched to with a `04` write.
- CodeWrite and pyiiasmh are older GUI assemblers. Search found nothing on pyiiasmh, so it is **unverified**.

**Permanent injection into main.dol**
- **[Melee Code Manager][mcm]** writes injection mods into DOL regions marked "safe for overwriting". The regions are defined in `settings.py`,
  so other games can be added. It also handles Gecko codes, static overwrites and "standalone functions". For Melee those regions are unused
  code such as the Tournament Mode code (**unverified**: the README fetch did not name the regions).
- **[gc-c-kit][gck] / [DOL C-Kit][dck] / [Freighter][frt]:** compile C/C++/asm with devkitPPC, add **new DOL sections**, and hook functions with
  `bl` patches.
- **Wiimm `--add-section`:** puts a GCT plus code handler into a new text section at `0x80001800` ([wiimm][wim]).
- Our DOL has free slots (**verified**): text sections T0 `80003100` and T1 `800055E0`-`801B65E0` are used, T2-T6 are free; data sections
  D0-D5 (`801B65E0` ... `80452A20`) are used, D6-D10 are free. bss is `8024DB20` + `0x204FA0`.

**Proposal for `make_mod.py`** (design only, not implemented):
1. **`code/*.gecko` (or `*.txt` with a `[gecko]` header): plain Gecko lists.**
   - `make_mod.py` would export them as `build/mods/<name>/GXCE01.ini` (`[Gecko]`/`[Gecko_Enabled]`) and as `GXCE01.gct`.
   - **Static conversion where possible:** a `04`/`02`/`00` write whose target is *inside main.dol* (e.g. `0444C438`) becomes an ordinary
     patch with an automatic `[was]` check. Runtime-RAM targets (bss/heap, e.g. all 15 AR codes) can't be baked statically: they stay
     Gecko-only, or they need (3).
2. **`code/*.s`: C2-style inserts from assembly**, built with `powerpc-eabi-as` (installed via the pinned toolchain in `tools/pins.txt`), using
   the same header style as JLaferri's `gecko` (`# Address: 80091780`).
   - Output A: a C2 Gecko code (for Dolphin/Nintendont users).
   - Output B (permanent): place the assembled body in the free-space region, patch the hook with `b <body>`, and append the replaced
     instruction plus `b <hook+4>`. The `[was]` check applies to the hook instruction.
3. **Free space / new section** (`mod.json: "code_region": "auto" | "0x80001800" | "new-section"`):
   - **Option a:** a new text section at `0x80001800`-`0x80003000`. This low-memory region is unused by retail games, which is why Dolphin and
     Wiimm use it. It conflicts with running cheats in Dolphin at the same time; document that.
   - **Option b:** reuse dead code. Candidates: MetroTRK debugger functions (`TRKInterruptHandler` etc. are already named), and asserts or
     debug paths. Each must be proven unreachable with `find.py callers` first.
   - **Option c:** a new section after bss. This needs `__ArenaLo`/`OSSetArenaLo` raised, otherwise the heap overwrites it (**unverified**
     for this game; check how `OSInit` sets the arena in our decomp).
   - Start with (a) because it is the simplest and well-trodden. Use (b) only if 6 KB is not enough.
4. A **`tools/ardecrypt.py`** (our own clean-room implementation, or a GPL-compatible port with attribution) so community AR codes can be
   dropped into `code/` and converted to Gecko automatically.

## Community resources (links)

- Codes:
  - [cheathappens Custom Robo GC][ch] (15 AR codes)
  - [Dolphin GXCE01.ini][ini] (widescreen)
  - [Dolphin wiki: Custom Robo][dw] (settings, widescreen notes)
  - [codes.rc24.xyz][rc24] (empty for GXCE01)
  - [supercheats][sc] (tips; not fetched)
- Texture pack: [Custom Robo HD texture pack (Dolphin forums)][hdt].
- Model tools/hacks: [Zoeyism/custom_robo_model_swapper][swp]; [RHDN utility 1735][rhu]; RHDN hack 7416 (Female Main Character, not fetched);
  [RHDN game page 4986][rhg] (403, not fetched).
- Reference: [TCRF Custom Robo (GameCube)][tcrf] (debug/unused content; could not be read in this session).
- Community:
  - r/customrobo: a 2018 post lists the "Main Custom Robo Netplay Discord", a Battle Revolution speedrun Discord and a Google Drive folder
    ([mirror][rcr], known only from a search snippet: the mirror page returned 404; invites probably expired, **unverified**). No public mirror of the modding Discord's posts was found.
  - [speedrun.com/custom_robo][src]: 145 runs; the platform split was unclear, and its resources page returned 403. No RAM map was found.
  - [Robopedia (customrobo.fandom.com)][fan]: robo/part articles, 798 articles. The fetch returned 402, so its part-ID coverage is unknown.
  - [GameTDB GXCE01][gtdb].
- Tooling:
  - [JLaferri/gecko][jlg], [Melee Code Manager][mcm], [gc-c-kit][gck], [DOL C-Kit][dck], [Freighter][frt]
  - [Wiimm add-section][wim]
  - Dolphin [ARDecrypt.cpp][ardec] / [ActionReplay.cpp][arcpp] / [GeckoCode.h][gch]
  - [supSMSASM][sup], [geckolibs][gl]

## Open questions to ask the Discord

1. Which codes does the community actually use? Are there private Gecko lists (e.g. stat edits, part unlocks, stage select, camera, debug menu)?
   Could they share them, even as raw lines, so we can cross-reference them like the 16 above?
2. "Textures through codes": do they mean Dolphin texture packs (hash replacement), or actual Gecko codes that change textures/colours in
   RAM? If codes, which addresses?
3. Has anyone used the AR codes on real hardware (AR disc) versus Dolphin? Did the encrypted lines work in Dolphin without the (M) line?
4. Does anyone have the JP (GXCJ01) debug-mode AR code from TCRF in plain form, or know what the debug menu contains?
5. Do they know the player-record layout around `8024DB20` (stride 0x33520: HP at +0x674 from bss start), or other RAM addresses (stats,
   part IDs, story flags)?
6. Is the HD pack author (slightlyflightyone) on the Discord? Which texture hashes map to which robo? That would let us connect Dolphin
   hashes to TObjs in our `.dat` exports.
7. Is Zoeyism around? Are robo swaps in `chara.BIN` already known to them, and did they find anything about blocks A/B/D?
8. Would they test Nintendont/real-hardware builds of `make_mod.py` ISOs, and a `.gct` export?

## Proposed plan.md roadmap items

Ready to paste (Phase 2 "Ideas", plus one Phase 3 "Ship" line):

```
- **Codes (community request, Oct 28, 2026).** Public lists are 15 AR codes (cheathappens, decrypted: HP x4, 1-hit x4, parts-owned x5, unlock flags) + Dolphin's 16:9 Gecko code; all 13 addresses map into our decomp (docs/community_codes.md, docs/community/codes_gxce01.tsv). Use them as naming evidence leads, not names.
  - [ ] `tools/ardecrypt.py`: AR (encrypted or plain) -> Gecko lines, so community codes can be dropped into a mod.
  - [ ] `make_mod.py`: `code/*.gecko` -> Dolphin `GXCE01.ini` + `.gct`; writes whose target is inside main.dol become static patches with `[was]` checks; bss/heap targets stay Gecko-only.
  - [ ] `make_mod.py`: `code/*.s` -> C2 code (devkitPPC `powerpc-eabi-as`, pinned) and, permanently, a body in free space + `b` hook.
  - [ ] Free space: new DOL text section at 0x80001800-0x80003000 first (T2-T6 / D6-D10 are unused); then prove dead code (MetroTRK) with `find.py callers`; check how `OSInit` sets ArenaLo before trying a section after bss.
  - [ ] Name leads from the codes: player battle record (x4, stride 0x33520, HP float) used by `FUN_8004ede4`; parts-owned bitsets `803BFB9C..` (5 x 4 x 64 bits) set up by `FUN_800a3c0c`; bitset helpers `FUN_80023228` / `FUN_800231ec`; unlock flags `DAT_80321d44`; camera aspect `FLOAT_8044c438`; 2D ortho setup `FUN_80091518`.
- **Textures.** Dolphin hash packs already exist (HD pack, Dec 2024); our route is file replacement (model TObjs via make_mod.py, `.tpl` via `files/`).
  - [ ] Map Dolphin texture hashes to our TObjs (hash the decoded texture the way Dolphin does) so a texture pack can be turned into a disc mod and back.
  - [ ] `.tpl` read/write tool (already a Phase 2 idea) - 590 of 915 disc files are `.tpl`.
- **Models: codes can't do it** (6 KB code space vs 186-233 KB per robo; models live in heap). File replacement is the path (Phase 1). Cross-check Zoeyism's Model Swapper: it byte-overwrites `rpg_t/f_models.BIN` with a smaller model and leaves the entry table alone.
```

Phase 3 "Ship":
```
- A `GXCE01.ini` / `.gct` export next to every `.crpatch`, so code-only mods can be used without rebuilding the ISO (Dolphin cheats, Nintendont).
```

[ch]: https://www.cheathappens.com/8922-Gamecube-Custom_Robo_cheats
[ini]: https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Data/Sys/GameSettings/GXCE01.ini
[dw]: https://wiki.dolphin-emu.org/index.php?title=Custom_Robo
[rc24]: https://codes.rc24.xyz/txt/GXCE01.txt
[tcrf]: https://tcrf.net/Custom_Robo_(GameCube)
[df1]: https://forums.dolphin-emu.org/Thread-gc-custom-robo--26933
[df2]: https://forums.dolphin-emu.org/Thread-action-replay-codetypes-and-gecko?pid=319763
[ardec]: https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/ARDecrypt.cpp
[arcpp]: https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/ActionReplay.cpp
[gch]: https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/GeckoCode.h
[gcc]: https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/Core/GeckoCode.cpp
[wim]: https://szs.wiimm.de/info/add-section.html
[gbt]: https://gbatemp.net/threads/usb-loader-gx.149922/post-4820921
[gbt2]: https://gbatemp.net/threads/nintendont.349258/post-5135978
[hdt]: https://forums.dolphin-emu.org/Thread-custom-robo-hd-texture-pack
[rb]: https://wiki-next.recalbox.com/en/tutorials/games/guides/gamecube-wii/custom-hires-textures-packs
[dft]: https://forums.dolphin-emu.org/Thread-dolphin-custom-textures-info?page=8
[swp]: https://github.com/Zoeyism/custom_robo_model_swapper
[rhu]: https://www.romhacking.net/utilities/1735/
[rhg]: https://www.romhacking.net/games/4986/
[jlg]: https://github.com/JLaferri/gecko
[mcm]: https://github.com/DRGN-DRC/Melee-Code-Manager
[gck]: https://github.com/RenolY2/gc-c-kit
[dck]: https://github.com/Minty-Meeo/dol_c_kit
[frt]: https://pypi.org/project/freighter/
[sup]: https://pypi.org/project/supsmsasm
[gl]: https://pypi.org/project/geckolibs
[sc]: https://www.supercheats.com/gamecube/customrobocheats.htm
[rcr]: https://reddit.sudovanilla.org/r/customrobo
[src]: https://www.speedrun.com/custom_robo
[fan]: https://customrobo.fandom.com/wiki/Robopedia
[gtdb]: https://www.gametdb.com/Wii/GXCE01
