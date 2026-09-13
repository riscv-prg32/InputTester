#!/usr/bin/env bash
set -euo pipefail

repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
prg32_repo="${PRG32_REPO:-"$repo_dir/../PRG32"}"
architecture="${PRG32_ARCHITECTURE:-esp32c6}"

if [[ ! -f "$prg32_repo/prg32/__main__.py" ]]; then
  echo "error: set PRG32_REPO to a current PRG32 checkout" >&2
  exit 2
fi

case "$architecture" in
  esp32c6|qemu) ;;
  *) echo "error: PRG32_ARCHITECTURE must be esp32c6 or qemu" >&2; exit 2 ;;
esac

mkdir -p "$repo_dir/dist"
(cd "$prg32_repo" && python3 -m prg32 cartridge build \
  "$repo_dir/c/input_tester.c" \
  --portable \
  --entry-prefix input_tester \
  --name inputtester \
  --architecture "$architecture" \
  --out "$repo_dir/dist/inputtester-$architecture.raw.prg32")

(cd "$prg32_repo" && python3 -m prg32 store attach-metadata \
  "$repo_dir/dist/inputtester-$architecture.raw.prg32" \
  --metadata "$repo_dir/metadata/metadata.json" \
  --colophon "$repo_dir/metadata/colophon.json" \
  --icon "$repo_dir/assets/icon.png" \
  --architecture "$architecture" \
  --out "$repo_dir/dist/inputtester_$architecture.prg32")

echo "$repo_dir/dist/inputtester_$architecture.prg32"
