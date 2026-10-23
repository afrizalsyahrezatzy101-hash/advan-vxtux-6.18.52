# Build environment tanpa root (codespace / mesin bersih)

Diperoleh kembali 2026-10-23 setelah codespace kehilangan seluruh toolchain
cross: `dpkg -l | grep aarch64` = 0, tidak ada `/usr/bin/aarch64*`, padahal
`.config` pohon masih menyebut "built by aarch64-linux-gnu-gcc 11.4.0".
Semua langkah di bawah **tanpa sudo** dan tidak menyentuh sistem.

## 1. Toolchain cross

```bash
mkdir -p $HOME/tc && cd $HOME/tc
curl -fSL -o tc.tar.xz \
  https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-linux-gnu.tar.xz
tar xf tc.tar.xz          # direktori: arm-gnu-toolchain-13.2.Rel1-... (R besar)
export PATH=$HOME/tc/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-linux-gnu/bin:$PATH
```

## 2. Dependensi host yang tidak ada di image

`flex`, `bc`, `m4`, `libelf-dev` — diambil sebagai `.deb` lalu diekstrak,
bukan `apt install`:

```bash
mkdir -p $HOME/apt/lists/partial $HOME/apt/cache/archives/partial $HOME/deb $HOME/sysroot
apt-get -o Dir::State::Lists=$HOME/apt/lists -o Dir::Cache=$HOME/apt/cache update
cd $HOME/deb && apt-get -o Dir::State::Lists=$HOME/apt/lists \
  -o Dir::Cache=$HOME/apt/cache download flex bc m4 libelf-dev
for f in $HOME/deb/*.deb; do dpkg -x "$f" $HOME/sysroot; done
export PATH=$HOME/sysroot/usr/bin:$PATH
```

## 3. Pohon sumber yang bersih

Pohon di `/workspaces/...` berisi artefak milik **root**, sehingga
`make O=` menolak dengan *"The source tree is not clean"* dan
`include/generated/*` tidak bisa ditulis ulang. Clone baru milik user adalah
jalan keluarnya:

```bash
git clone --depth 1 --single-branch -b test1 /workspaces/advan-vxtux-6.18.52 $HOME/src
cd $HOME/src/6.18.52-VXTux-UMS512
chmod -R +x scripts/       # bit executable hilang saat commit dari Windows
```

## 4. Build

```bash
export ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu-
cp .config .config.device
sed -i 's/\r$//' .config.device ../enhanced_config.cfg
scripts/kconfig/merge_config.sh -m -O . .config.device ../enhanced_config.cfg
make olddefconfig
make -j$(nproc) modules
```

## 5. Jebakan yang sudah kena

| Gejala | Sebab |
|---|---|
| `aarch64-linux-gnu-gcc: No such file` | toolchain dihapus dari image; pakai tarball di langkah 1 |
| `flex: not found` saat `olddefconfig` | kconfig butuh flex; parser `.c` tidak ikut di repo |
| `scripts/cc-version.sh: Permission denied` | bit executable hilang; `chmod -R +x scripts/` |
| `The source tree is not clean` (build `O=`) | artefak root di pohon `/workspaces`; pakai clone bersih |
| modpost: `"<simbol>" undefined` di build `M=` | `Module.symvers` belum ada; modul tunggal baru bisa di-modpost setelah `make modules` penuh |

## 6. Hasil verifikasi 2026-10-23

- `make modules` → **rc=0**, **228 `.ko`** (227 sebelumnya + `phy-sprd-ums512.ko`).
- `drivers/phy/sprd/phy-sprd-ums512.ko` = **433.584 B**.
- Bukti: `docs/evidence-20261023/ko_list_tick13.txt` + `config_tick13.txt`.
