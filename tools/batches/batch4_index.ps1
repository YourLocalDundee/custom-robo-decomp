# Batch 4: searchable index of the whole decompile (C + references + strings) and one file per function.
# Needs a rebuild that ran ExportXrefs.java. Output: decompiled\index.db, decompiled\funcs\. Query: python -I tools\find.py
$ErrorActionPreference = 'Stop'
$g = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
python -I "$g\tools\build_index.py"
if ($LASTEXITCODE) { throw 'build_index.py failed' }
