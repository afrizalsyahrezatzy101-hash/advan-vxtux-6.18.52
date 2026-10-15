# SPDX-License-Identifier: GPL-2.0
#
# drivers/vxtux/net/wireless/sprdwcn/Makefile
# Port GPL: drivers/misc/sprdwcn (fork Samsung Tab A8, T618/UMS512)
#   -> kernel 6.18 VXTux (batch B7). Blob yang digantikan: wcn_bsp.ko.
#
# Perubahan struktural vs Makefile vendor (ketiganya disengaja):
#
#  1. platform/ di-descend SEKALI. Vendor punya dua baris:
#       obj-$(CONFIG_WCN_BOOT)     += platform/
#       obj-$(CONFIG_WCN_PLATFORM) += platform/
#     dengan keduanya aktif, kbuild menerima "platform/" dua kali di obj-y.
#     Sekarang penjaganya satu: WCN_BOOT (WCN_PLATFORM tetap ada sebagai
#     simbol Kconfig untuk kode #ifdef, tapi tidak lagi mengendalikan descent).
#
#  2. pcie/ di-descend pada CONFIG_WCN_PCIE, tetapi simbol itu default n.
#     Papan T1030 memakai jalur SDIO (compatible "unisoc,marlin3lite_sdio").
#     Sebelum 2026-09-19 baris descent ini TIDAK ADA dan pcie/Kconfig juga
#     tidak pernah di-source, sehingga 8 berkas .c di pcie/ (4.733 baris)
#     mustahil dibangun dan `CONFIG_WCN_PCIE` tak dikenal kconfig.
#     Cabangnya sekarang benar-benar ada; tetap default n karena fungsi
#     penyedia root-complex-nya tidak punya definisi di pohon ini:
#     sprd_pcie_configure_device/unconfigure_device/register_event/
#     deregister_event/teardown_msi_irq/dump_rc_regs (dideklarasikan di
#     include/linux/pcie-rc-sprd.h, dipanggil pcie.c, tapi tak ada yang
#     mendefinisikannya -- juga tidak di gpl-source/).
#
#  3. Semua sub-simbol WCN di atas dibangun BUILT-IN (=y), mengikuti konvensi
#     proyek: simbol ported harus muncul di System.map supaya gate (C2/C3/C5)
#     bisa membuktikan link. Nama modul tidak relevan karena tidak ada .ko hasil
#     vendor yang tersisa.
#
#  4. PENGECUALIAN untuk BT/FM: keduanya `=m`. Blob aslinya memang modul
#     (sprdbt_tty.ko, sprd_fm.ko), jadi bentuk modul adalah yang benar — dan
#     itu memungkinkan bukti modinfo/vermagic atas .ko yang dihasilkan.
#     Jalur descent mengikuti pohon dalam-tree referensi:
#       drivers/wcn/{bluetooth,fm}/driver/  ->  bluetooth/, fm/

subdir-ccflags-y := \
	-I$(src)/include \
	-I$(src)/boot \
	-I$(src)/platform \
	-I$(src)/platform/rf

obj-$(CONFIG_SC23XX)	+= wcn_bus.o
obj-$(CONFIG_WCN_SIPC)	+= sipc/
# platform/ di-descend SATU kali, pemiliknya WCN_CORE (bukan lagi WCN_BOOT).
# WCN_CORE=m -> wcn_init_core() menjalankan semua init komposit (module_init
# hanya sah satu per modul; wcn_boot.c sebagai host init-nya).
obj-$(CONFIG_WCN_CORE)	+= platform/
obj-$(CONFIG_WCN_INTEG)	+= boot/
obj-$(CONFIG_SDIOHAL)	+= sdio/
obj-$(CONFIG_WCN_SLP)	+= sleep/
obj-$(CONFIG_WCN_BLUETOOTH)+= bluetooth/
obj-$(CONFIG_WCN_FM)	+= fm/
obj-$(CONFIG_WCN_PCIE)	+= pcie/

# WLAN SC2355 (2026-10-09): satu modul sprdwl.ko dari 27 objek sc2355/.
obj-$(CONFIG_WCN_WLAN) += wlan/
