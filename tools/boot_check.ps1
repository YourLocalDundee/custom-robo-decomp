# Boot an ISO in the Project Hive Dolphin for N seconds and report whether Dolphin raised a warning dialog
# (e.g. "Invalid read from ..."), which means the game would have crashed. Closes Dolphin afterwards.
# Usage: pwsh tools\boot_check.ps1 <game.iso> [seconds]      exit 1 if a warning appeared or Dolphin died
param([Parameter(Mandatory)][string]$Iso, [int]$Seconds = 60, [string]$Snapshot,
      [string]$DolphinDir = 'D:\emulators\Project Hive 3.2 [Dolphin Precon 2609a]')
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
Push-Location -LiteralPath $DolphinDir
$p = Start-Process -FilePath (Join-Path $DolphinDir 'Dolphin.exe') -ArgumentList @('-b', '-e', "`"$Iso`"") -PassThru
Pop-Location
$warn = $null
for ($i = 0; $i -lt $Seconds -and -not $p.HasExited; $i++) {
    Start-Sleep -Seconds 1
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
if ($alive) { Stop-Process -Id $p.Id -Force }
"boot_check: $(Split-Path $Iso -Leaf): after $i s, running=$alive, title='$title', warning=$(if ($warn) { $warn } else { 'none' })"
if ($warn -or -not $alive) { exit 1 }
