# Batch 5: check the types the rebuild applied (ParseHeaders.java -> symbols\types_report.txt).
# Stops if any header size / field offset is wrong, any prototype or global failed, or any HSD struct size
# differs from the size Custom Robo registers for that class (tools\check_class_sizes.py).
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$report = "$g\symbols\types_report.txt"
$summary = (Get-Content -LiteralPath $report -TotalCount 1)
"types: $summary"
if ($summary -notmatch 'ASSERT_SIZE ok \d+, wrong 0, not parsed 0; field offsets ok \d+, wrong 0 .*failed 0; globals typed \d+, failed 0') {
    throw "types check failed, see $report"
}
python -I "$g\tools\check_class_sizes.py" $report "$g\decompiled\main_dol.c" | Select-Object -Last 1
if ($LASTEXITCODE) { throw 'HSD struct sizes differ from the game (run tools\check_class_sizes.py for details)' }
