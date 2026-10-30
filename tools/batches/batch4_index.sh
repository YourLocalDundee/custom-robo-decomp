#!/usr/bin/env bash
# Batch 4: searchable index of the whole decompile (C + references + strings) and one file per function.
# Needs a rebuild that ran ExportXrefs.java. Output: decompiled/index.db, decompiled/funcs/. Query: python3 -I tools/find.py
set -euo pipefail
b=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
g=$(dirname "$(dirname "$b")")
. "$g/tools/common.sh"
"$PY" -I "$g/tools/build_index.py" || die 'build_index.py failed'
