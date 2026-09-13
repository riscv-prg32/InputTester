# Build and package InputTester

InputTester uses the portable cartridge ABI from the current PRG32 `main`
branch. It requires a local PRG32 checkout and the RISC-V toolchain configured
by ESP-IDF. Set `PRG32_REPO` if that checkout is not next to InputTester.

```sh
source "$HOME/esp-idf/export.sh"
export PRG32_REPO=/path/to/PRG32

PRG32_ARCHITECTURE=esp32c6 scripts/build.sh
PRG32_ARCHITECTURE=qemu scripts/build.sh
scripts/pack-store-bundle.sh
```

The two `.prg32` files in `dist/` are portable cartridges with Store metadata
and colophons. The ZIP contains both variants and the manifest. Source builds
can be checked with `python3 -m prg32 cartridge summary` from the PRG32 root.

To upload the ESP32-C6 cartridge to a board on its setup access point:

```sh
PYTHONPATH="$PRG32_REPO" python3 -m prg32 esp32c6 upload \
  dist/inputtester_esp32c6.prg32 --url http://192.168.4.1
```

To stage the QEMU cartridge, run from the PRG32 checkout or provide the flash
image and partition table explicitly with `--flash` and `--partitions`:

```sh
PYTHONPATH="$PRG32_REPO" python3 -m prg32 qemu upload \
  dist/inputtester_qemu.prg32 \
  --flash "$PRG32_REPO/build-qemu/flash_image.bin" \
  --partitions "$PRG32_REPO/partitions_prg32.csv"
```
