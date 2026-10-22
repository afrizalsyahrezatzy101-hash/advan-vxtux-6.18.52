#!/bin/bash
# Set the VXTux deblob config on the codespace, in the right order.
#
# The order matters:
#   1. VXTUX_DRIVERS=y      - the umbrella; every drivers/vxtux/ symbol
#                             lives under 'if VXTUX_DRIVERS', so without it
#                             syncconfig drops them all again
#   2. the umbrella symbols that the specific ones depend on
#   3. the specific symbols
#   4. oldconfig to resolve the depends-on graph
#   5. verify that nothing was dropped
set -u
cd /workspaces/vxtux-build/6.18.52-VXTux-UMS512 || exit 1

say() { echo "--- $* ---"; }

# 1) umbrella
./scripts/config --file .config \
  --enable VXTUX_DRIVERS

# 2) the umbrellas the specific symbols hang off
./scripts/config --file .config \
  --enable SPRD_AUDIO_INFRA \
  --enable SPRD_MAILBOX \
  --enable SPRD_SIPC

# 3) the per-driver symbols
./scripts/config --file .config \
  --enable SPRD_AUDIO_MEMORY_MANAGER \
  --module SPRD_AUDIO_SIPC \
  --module SPRD_AUDIO_DSP_DUMP \
  --module SPRD_APSYS_DVFS \
  --module NVMEM_SPRD_CACHE_EFUSE \
  --module SPRD_VSP_PW_DOMAIN \
  --module TOUCHSCREEN_NOVATEK_NVT_TS_SPI \
  --disable TOUCHSCREEN_NOVATEK_NVT_TS \
  --module TRUSTY \
  --module TRUSTY_LOG \
  --module TRUSTY_TUI \
  --module TRUSTY_VIRTIO_IPC \
  --module ION_IPC_TRUSTY \
  --module MMC_SDHCI_SPRD \
  --module GPIO_PMIC_EIC_SPRD \
  --module NVMEM_SC27XX_EFUSE \
  --module RTC_DRV_SC27XX \
  --module SC27XX_ADC \
  --module CRYPTO_AES_ARM64_CE_CCM \
  --module CRYPTO_TWOFISH_COMMON \
  --module BACKLIGHT_PWM \
  --module SPRD_UMS512_CLK \
  --module SPRD_POWER_MANAGER

# 4) let the dependency graph settle
make ARCH=arm64 olddefconfig > /dev/null 2>&1

# 5) verify
say "verify"
for s in VXTUX_DRIVERS SPRD_AUDIO_INFRA SPRD_AUDIO_MEMORY_MANAGER \
         SPRD_AUDIO_SIPC SPRD_AUDIO_DSP_DUMP SPRD_APSYS_DVFS \
         NVMEM_SPRD_CACHE_EFUSE SPRD_VSP_PW_DOMAIN \
         TOUCHSCREEN_NOVATEK_NVT_TS_SPI TOUCHSCREEN_NOVATEK_NVT_TS \
         TRUSTY TRUSTY_LOG TRUSTY_TUI TRUSTY_VIRTIO_IPC \
         ION_IPC_TRUSTY MMC_SDHCI_SPRD SPRD_TSHM; do
  v=$(grep -E "^(# )?CONFIG_$s(=| is not set)" .config | head -1)
  printf "  %-42s %s\n" "$s" "${v:-ABSENT}"
done
