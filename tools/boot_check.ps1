#Requires -Version 7
# Boot an ISO (or an extracted game folder's sys\main.dol) in Dolphin (Windows; see -DolphinDir below) for N seconds and report whether
# Dolphin raised a warning dialog (e.g. "Invalid read from ..."), which means the game would have crashed. Closes
# Dolphin afterwards.
# Usage: pwsh tools\boot_check.ps1 <game.iso | folder\sys\main.dol> [seconds]   exit 1 if a warning appeared or Dolphin died
#   -Snapshot x.png    picture of the warning dialog, if one appears
#   -Screenshot x.png  picture of the game window at the end (a screen copy: the window is brought to the front)
#   -Log x.log         Dolphin's log of this run only (OS reports, files read from the disc, boot / core / CPU
#                      messages), switched on with command-line config: Dolphin's own settings are not changed
#   -Mem1Mb 64        Dolphin's emulated memory size override (MEM1 64 MB), for mods that use RAM above 24 MB
#   -PressA 16         from that second on, press the GameCube A button every 2 s (the X key, Dolphin's default keyboard
#                      mapping; the Dolphin window is brought to the front for it) until the log shows the title
#                      screen's sprite/us_title.tpl (with -Log) or the run ends. For a Dolphin whose memory card has
#                      no Custom Robo save: the game then waits at "There is no save data on the Memory Card in
#                      Slot A" for A, and reaches the title screen at about 23 s instead of 13 s (batch 11 uses it)
param([Parameter(Mandatory)][string]$Iso, [int]$Seconds = 60, [string]$Snapshot, [string]$Screenshot, [string]$Log,
      [string]$DolphinDir, [int]$PressA = 0, [int]$Mem1Mb = 0)
# Dolphin folder: -DolphinDir, else $env:CR_DOLPHIN_DIR, else the pinned Dolphin tools/setup.ps1 downloads
# (tools/dolphin/Dolphin-x64, made portable so its User/ and log stay in that folder; stock settings, no cheats).
$Iso = (Resolve-Path -LiteralPath $Iso).Path   # Dolphin starts in its own folder: a relative path would not be found
if (-not $DolphinDir) { $DolphinDir = $env:CR_DOLPHIN_DIR }
if (-not $DolphinDir) {
    $pinned = Join-Path (Split-Path $PSScriptRoot -Parent) 'tools/dolphin/Dolphin-x64'
    if (Test-Path -LiteralPath (Join-Path $pinned 'Dolphin.exe')) { $DolphinDir = $pinned }
}
if (-not $DolphinDir) { throw 'no Dolphin found: pass -DolphinDir, set CR_DOLPHIN_DIR, or run tools/setup.ps1 (it downloads the pinned Dolphin to tools/dolphin/Dolphin-x64)' }
if (-not (Test-Path -LiteralPath (Join-Path $DolphinDir 'portable.txt'))) { $null = New-Item -ItemType File -Path (Join-Path $DolphinDir 'portable.txt') }
Add-Type -AssemblyName System.Drawing
Add-Type @'
using System; using System.Text; using System.Collections.Generic; using System.Runtime.InteropServices;
public static class BootWin {
  delegate bool EnumProc(IntPtr h, IntPtr l);
  [DllImport("user32.dll")] static extern bool EnumWindows(EnumProc f, IntPtr l);
  [DllImport("user32.dll")] static extern uint GetWindowThreadProcessId(IntPtr h, out uint pid);
  [DllImport("user32.dll", CharSet = CharSet.Unicode)] static extern int GetWindowText(IntPtr h, StringBuilder s, int n);
  [DllImport("user32.dll")] static extern bool IsWindowVisible(IntPtr h);
  [DllImport("user32.dll")] static extern bool EnumChildWindows(IntPtr h, EnumProc f, IntPtr l);
  [DllImport("user32.dll", CharSet = CharSet.Unicode)] static extern IntPtr SendMessage(IntPtr h, uint m, IntPtr w, StringBuilder l);
  // title of each visible top-level window of the process, plus the text of its child controls (dialog message)
  public static List<string> Texts(uint pid) {
    var r = new List<string>();
    EnumWindows((h, l) => { uint p; GetWindowThreadProcessId(h, out p);
      if (p == pid && IsWindowVisible(h)) {
        var s = new StringBuilder(512); GetWindowText(h, s, 512); var t = s.ToString();
        EnumChildWindows(h, (c, x) => { var b = new StringBuilder(1024); SendMessage(c, 0x000D, (IntPtr)1024, b);
          if (b.Length > 0) t += " | " + b.ToString().Replace("\n", " ").Replace("\r", ""); return true; }, IntPtr.Zero);
        r.Add(t);
      }
      return true; }, IntPtr.Zero);
    return r;
  }
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr hdc, uint f);
  public struct RECT { public int L, T, R, B; }
  // handle of the process's visible window whose title starts with prefix (pictured by the script: Qt dialogs
  // have no readable text controls)
  public static IntPtr Find(uint pid, string prefix) {
    IntPtr found = IntPtr.Zero;
    EnumWindows((h, l) => { uint p; GetWindowThreadProcessId(h, out p);
      if (p == pid && IsWindowVisible(h)) { var s = new StringBuilder(512); GetWindowText(h, s, 512);
        if (s.ToString().StartsWith(prefix)) { found = h; return false; } }
      return true; }, IntPtr.Zero);
    return found;
  }
  [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
  // a key press as the keyboard would send it (scan code: Dolphin's DInput keyboard identifies keys by it)
  [DllImport("user32.dll")] static extern void keybd_event(byte vk, byte sc, uint flags, UIntPtr extra);
  [DllImport("user32.dll")] static extern uint MapVirtualKey(uint code, uint type);
  public static void Press(char key, int holdMs) {
    byte vk = (byte)key, sc = (byte)MapVirtualKey(vk, 0);
    keybd_event(vk, sc, 8, UIntPtr.Zero); System.Threading.Thread.Sleep(holdMs); keybd_event(vk, sc, 10, UIntPtr.Zero);
  }
  [DllImport("user32.dll")] public static extern bool SetProcessDPIAware();
  public static List<string> Titles(uint pid) {
    var r = new List<string>();
    EnumWindows((h, l) => { uint p; GetWindowThreadProcessId(h, out p);
      if (p == pid && IsWindowVisible(h)) { var s = new StringBuilder(512); GetWindowText(h, s, 512); r.Add(s.ToString()); }
      return true; }, IntPtr.Zero);
    return r;
  }
}
'@
# start from Dolphin's own folder so it does not create a User\ folder in the current directory
$dlog = Join-Path $DolphinDir 'User\Logs\dolphin.log'
# a Dolphin that was never started (the pinned one right after setup) asks "Allow Usage Statistics Reporting" in a modal
# dialog before the game runs: answered here (no) through command-line config, like the log settings below
$dargs = @('-b', '-C', 'Dolphin.Analytics.PermissionAsked=True', '-C', 'Dolphin.Analytics.Enabled=False')
# -Mem1Mb 64: Dolphin's emulated memory size override (Config > Advanced), for mods that use RAM above 24 MB
# (a "body_buffer" mod, docs/MODDING.md "Big bodies"); command-line config only, Dolphin's settings are not changed
if ($Mem1Mb) { $dargs += '-C', 'Dolphin.Core.RAMOverrideEnable=True', '-C', "Dolphin.Core.MEM1Size=$($Mem1Mb * 1048576)" }
if ($Log) {
    $dargs += 'Logger.Options.WriteToFile=True', 'Logger.Options.Verbosity=3', 'Logger.Logs.OSREPORT=True',
        'Logger.Logs.FileMon=True', 'Logger.Logs.BOOT=True', 'Logger.Logs.CORE=True', 'Logger.Logs.POWERPC=True',
        'Logger.Logs.MASTER=True' | ForEach-Object { '-C', $_ }
    $logStart = if (Test-Path -LiteralPath $dlog) { (Get-Item -LiteralPath $dlog).Length } else { 0 }
}
Push-Location -LiteralPath $DolphinDir
$p = Start-Process -FilePath (Join-Path $DolphinDir 'Dolphin.exe') -ArgumentList ($dargs + @('-e', "`"$Iso`"")) -PassThru
Pop-Location
$warn = $null
$pressed = @()
function LogSoFar { if ($Log -and (Test-Path -LiteralPath $dlog)) { $fs = [IO.File]::Open($dlog, 'Open', 'Read', 'ReadWrite'); [void]$fs.Seek($logStart, 'Begin'); $r = New-Object IO.StreamReader($fs); $t = $r.ReadToEnd(); $r.Close(); $t } else { '' } }
for ($i = 0; $i -lt $Seconds -and -not $p.HasExited; $i++) {
    Start-Sleep -Seconds 1
    if ($PressA -and $i -ge $PressA -and (($i - $PressA) % 2) -eq 0 -and -not ((LogSoFar) -match 'sprite/us_title\.tpl')) {
        $p.Refresh()
        if ($p.MainWindowHandle -ne [IntPtr]::Zero) { [void][BootWin]::SetForegroundWindow($p.MainWindowHandle); [BootWin]::Press('X', 150); $pressed += $i }
    }
    $w = [BootWin]::Texts([uint32]$p.Id) | Where-Object { $_ -match '^(Warning|Error|Critical|Panic|Question)' }
    if ($w) {
        $warn = $w -join ', '
        if ($Snapshot) {
            Start-Sleep -Milliseconds 500
            $h = [BootWin]::Find([uint32]$p.Id, 'Warning')
            $r = New-Object BootWin+RECT
            if ($h -ne [IntPtr]::Zero -and [BootWin]::GetWindowRect($h, [ref]$r)) {
                $bmp = New-Object System.Drawing.Bitmap ([Math]::Max(1, $r.R - $r.L)), ([Math]::Max(1, $r.B - $r.T))
                $gr = [System.Drawing.Graphics]::FromImage($bmp); $hdc = $gr.GetHdc()
                [void][BootWin]::PrintWindow($h, $hdc, 2); $gr.ReleaseHdc($hdc); $bmp.Save($Snapshot); $gr.Dispose(); $bmp.Dispose()
            }
        }
        break
    }
}
$p.Refresh()
$alive = -not $p.HasExited
$title = $p.MainWindowTitle
if ($alive -and $Screenshot -and $p.MainWindowHandle -ne [IntPtr]::Zero) {
    [void][BootWin]::SetProcessDPIAware()      # window coordinates in real pixels on a scaled display
    $h = $p.MainWindowHandle
    [void][BootWin]::SetForegroundWindow($h); Start-Sleep -Milliseconds 700
    $r = New-Object BootWin+RECT
    if ([BootWin]::GetWindowRect($h, [ref]$r)) {
        $bmp = New-Object System.Drawing.Bitmap ([Math]::Max(1, $r.R - $r.L)), ([Math]::Max(1, $r.B - $r.T))
        $gr = [System.Drawing.Graphics]::FromImage($bmp); $gr.CopyFromScreen($r.L, $r.T, 0, 0, $bmp.Size)
        $bmp.Save($Screenshot); $gr.Dispose(); $bmp.Dispose()
    }
}
if ($alive) { Stop-Process -Id $p.Id -Force }
if ($Log) {   # only this run's part of Dolphin's log
    Start-Sleep -Seconds 1
    $fs = [IO.File]::Open($dlog, 'Open', 'Read', 'ReadWrite')
    [void]$fs.Seek($logStart, 'Begin'); $buf = New-Object byte[] ($fs.Length - $logStart)
    [void]$fs.Read($buf, 0, $buf.Length); $fs.Close()
    [IO.File]::WriteAllBytes($Log, $buf)
}
"boot_check: $(Split-Path $Iso -Leaf): after $i s, running=$alive, title='$title', warning=$(if ($warn) { $warn } else { 'none' })$(if ($pressed) { ", A pressed at $($pressed -join ', ') s" })"
if ($warn -or -not $alive) { exit 1 }
