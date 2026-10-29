#Requires -Version 7
# Boot a game folder in the Project Hive Dolphin build, take a screenshot after a delay, then close Dolphin.
# Usage: pwsh boot_test.ps1 <build_dir> <out.png> [seconds]
param([string]$Build, [string]$Out, [int]$Seconds = 40)
$dolphin = 'D:\emulators\Project Hive 3.2 [Dolphin Precon 2609a]\Dolphin.exe'
Add-Type -AssemblyName System.Drawing, System.Windows.Forms
Add-Type @'
using System; using System.Runtime.InteropServices;
public static class Win { [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr hdc, uint flags);
  [StructLayout(LayoutKind.Sequential)] public struct RECT { public int L, T, R, B; } }
'@
$p = Start-Process -FilePath $dolphin -ArgumentList @('-b', '-e', "`"$Build\sys\main.dol`"") -PassThru
Start-Sleep -Seconds $Seconds
$p.Refresh()
$h = $p.MainWindowHandle
$r = New-Object Win+RECT
if ($h -ne [IntPtr]::Zero -and [Win]::GetWindowRect($h, [ref]$r)) {
    $w = $r.R - $r.L; $hh = $r.B - $r.T
} else {
    $b = [System.Windows.Forms.Screen]::PrimaryScreen.Bounds; $r.L = $b.X; $r.T = $b.Y; $w = $b.Width; $hh = $b.Height
}
$bmp = New-Object System.Drawing.Bitmap $w, $hh
$g = [System.Drawing.Graphics]::FromImage($bmp)
if ($h -ne [IntPtr]::Zero) {
    # PrintWindow with PW_RENDERFULLCONTENT (2) captures the window even when other windows cover it
    $hdc = $g.GetHdc(); [void][Win]::PrintWindow($h, $hdc, 2); $g.ReleaseHdc($hdc)
} else {
    $g.CopyFromScreen($r.L, $r.T, 0, 0, $bmp.Size)
}
$bmp.Save($Out)
"window: '$($p.MainWindowTitle)' $w x $hh -> $Out"
Stop-Process -Id $p.Id -Force
