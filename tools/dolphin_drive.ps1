#Requires -Version 7
# Drive a running Dolphin step by step (Windows): start a game, press GameCube buttons through Dolphin's default
# keyboard mapping, take a screen capture of its game window, stop it. For checks that need the menus (a mod's model
# in Customize or a battle); pair with tools/dolphin_mem.py to watch RAM.
#   pwsh tools/dolphin_drive.ps1 start <game.iso> [-Mem1Mb 64]
#   pwsh tools/dolphin_drive.ps1 press A A START DOWN ... [-HoldMs 120] [-GapMs 700]
#   pwsh tools/dolphin_drive.ps1 shot out.png
#   pwsh tools/dolphin_drive.ps1 stop
# Buttons (Dolphin's default GameCube keyboard profile): A=X, B=Z, X=C, Y=S, Z=D, START=Enter, L=Q, R=W,
# stick UP/DOWN/LEFT/RIGHT = arrow keys, D-pad DUP/DDOWN/DLEFT/DRIGHT = T/G/F/H. Player 2 (port 2, mapped in the
# pinned Dolphin's GCPadNew.ini): P2A=N, P2B=M, P2START=P, P2UP/P2DOWN/P2LEFT/P2RIGHT = numpad 8/2/4/6.
param([Parameter(Mandatory, Position = 0)][string]$Cmd, [Parameter(Position = 1, ValueFromRemainingArguments)][string[]]$Rest,
      [int]$Mem1Mb = 0, [int]$HoldMs = 120, [int]$GapMs = 700, [string]$DolphinDir)
if (-not $DolphinDir) { $DolphinDir = $env:CR_DOLPHIN_DIR }
if (-not $DolphinDir) { $DolphinDir = Join-Path (Split-Path $PSScriptRoot -Parent) 'tools/dolphin/Dolphin-x64' }
Add-Type -AssemblyName System.Drawing
Add-Type @'
using System; using System.Text; using System.Runtime.InteropServices;
public static class DrvWin {
  delegate bool EnumProc(IntPtr h, IntPtr l);
  [DllImport("user32.dll")] static extern bool EnumWindows(EnumProc f, IntPtr l);
  [DllImport("user32.dll")] static extern uint GetWindowThreadProcessId(IntPtr h, out uint pid);
  [DllImport("user32.dll", CharSet = CharSet.Unicode)] static extern int GetWindowText(IntPtr h, StringBuilder s, int n);
  [DllImport("user32.dll")] static extern bool IsWindowVisible(IntPtr h);
  [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool SetProcessDPIAware();
  public struct RECT { public int L, T, R, B; }
  // the biggest visible window of the process (the game render window, not a small dialog)
  public static IntPtr Main(uint pid) {
    IntPtr best = IntPtr.Zero; long area = 0;
    EnumWindows((h, l) => { uint p; GetWindowThreadProcessId(h, out p);
      if (p == pid && IsWindowVisible(h)) { RECT r; GetWindowRect(h, out r); long a = (long)(r.R - r.L) * (r.B - r.T);
        if (a > area) { area = a; best = h; } }
      return true; }, IntPtr.Zero);
    return best;
  }
  [DllImport("user32.dll")] static extern void keybd_event(byte vk, byte sc, uint flags, UIntPtr extra);
  [DllImport("user32.dll")] static extern uint MapVirtualKey(uint code, uint type);
  public static void Press(byte vk, bool ext, int holdMs) {
    byte sc = (byte)MapVirtualKey(vk, 0); uint e = ext ? 1u : 0u;
    keybd_event(vk, sc, 8 | e, UIntPtr.Zero); System.Threading.Thread.Sleep(holdMs); keybd_event(vk, sc, 10 | e, UIntPtr.Zero);
  }
}
'@
[DrvWin]::SetProcessDPIAware() | Out-Null
$keys = @{ A = @(0x58, $false); B = @(0x5A, $false); X = @(0x43, $false); Y = @(0x53, $false); Z = @(0x44, $false)
           START = @(0x0D, $false); L = @(0x51, $false); R = @(0x57, $false)
           UP = @(0x26, $true); DOWN = @(0x28, $true); LEFT = @(0x25, $true); RIGHT = @(0x27, $true)
           DUP = @(0x54, $false); DDOWN = @(0x47, $false); DLEFT = @(0x46, $false); DRIGHT = @(0x48, $false)
           P2A = @(0x4E, $false); P2B = @(0x4D, $false); P2START = @(0x50, $false)
           P2UP = @(0x68, $false); P2DOWN = @(0x62, $false); P2LEFT = @(0x64, $false); P2RIGHT = @(0x66, $false) }
function DolphinProc { Get-Process Dolphin -ErrorAction SilentlyContinue | Select-Object -First 1 }
switch ($Cmd) {
    'start' {
        if (DolphinProc) { throw 'Dolphin is already running (stop it first)' }
        $iso = (Resolve-Path -LiteralPath $Rest[0]).Path
        # port 2 on (a standard controller, keyboard N / M / P / numpad in this Dolphin's GCPadNew.ini) for Vs. mode
        $a = @('-b', '-C', 'Dolphin.Analytics.PermissionAsked=True', '-C', 'Dolphin.Analytics.Enabled=False',
               '-C', 'Dolphin.Core.SIDevice1=6')
        if ($Mem1Mb) { $a += '-C', 'Dolphin.Core.RAMOverrideEnable=True', '-C', "Dolphin.Core.MEM1Size=$($Mem1Mb * 1048576)" }
        $a += '-e', $iso
        $p = Start-Process -FilePath (Join-Path $DolphinDir 'Dolphin.exe') -ArgumentList $a -WorkingDirectory $DolphinDir -PassThru
        "started Dolphin $($p.Id)"
    }
    'press' {
        $p = DolphinProc; if (-not $p) { throw 'Dolphin is not running' }
        $h = [DrvWin]::Main([uint32]$p.Id); [DrvWin]::SetForegroundWindow($h) | Out-Null; Start-Sleep -Milliseconds 150
        foreach ($k in $Rest) {
            $u = $k.ToUpper(); if (-not $keys.ContainsKey($u)) { throw "unknown button $k" }
            [DrvWin]::Press([byte]$keys[$u][0], [bool]$keys[$u][1], $HoldMs); Start-Sleep -Milliseconds $GapMs
        }
        "pressed $($Rest -join ' ')"
    }
    'shot' {
        $p = DolphinProc; if (-not $p) { throw 'Dolphin is not running' }
        $h = [DrvWin]::Main([uint32]$p.Id); [DrvWin]::SetForegroundWindow($h) | Out-Null; Start-Sleep -Milliseconds 300
        $r = New-Object DrvWin+RECT; [DrvWin]::GetWindowRect($h, [ref]$r) | Out-Null
        $w = $r.R - $r.L; $ht = $r.B - $r.T
        $bmp = New-Object System.Drawing.Bitmap $w, $ht; $g = [System.Drawing.Graphics]::FromImage($bmp)
        $g.CopyFromScreen($r.L, $r.T, 0, 0, $bmp.Size); $bmp.Save($Rest[0], [System.Drawing.Imaging.ImageFormat]::Png)
        $g.Dispose(); $bmp.Dispose(); "saved $($Rest[0]) ($w x $ht)"
    }
    'stop' { $p = DolphinProc; if ($p) { Stop-Process -Id $p.Id -Force; 'stopped' } else { 'not running' } }
    default { throw "unknown command $Cmd" }
}
