# SPDX-License-Identifier: GPL-2.0
#
# drivers/vxtux/net/wireless/sprdwcn/platform/Makefile
# Port 6.18 batch B7. Blob yang digantikan: wcn_bsp.ko (bagian platform).
#
# Kenapa Makefile vendor diubah:
#
#  Vendor memakai DUA nama komposit di objek yang sama dan dengan `:=`:
#      obj-$(CONFIG_WCN_BOOT)     := marlin.o
#      obj-$(CONFIG_WCN_PLATFORM) := wcn_platform.o
#  Kalau kedua simbol aktif, penugasan kedua MENIMPA yang pertama, jadi
#  marlin.o (beserta MARLINOFILES: wcn_boot.o, wcn_dump.o, wcn_op.o) tidak
#  pernah masuk obj-*. Artinya marlin_probe() — driver platform WCN — tidak
#  akan pernah dibangun. Itu tidak mungkin jadi konfigurasi yang dipakai
#  vendor T1030 (modulnya bernama wcn_bsp.ko, satu modul untuk seluruh
#  pohon), jadi Makefile ini ditulis ulang dengan bentuk komposit kbuild
#  yang benar: SATU nama modul (anchor) + anggota lewat `-y`:
#      obj-$(CONFIG_WCN_CORE)      += wcn_core.o
#      wcn_core-y                  += <semua objek anggota>
#  Dengan itu wcn_core.ko = SATU modul (komposit 23 objek), init fungsinya
#  dirantai dari wcn_init_core() (wcn_boot.c) karena module_init hanya sah
#  satu per modul. WCN_BOOT tetap bool sebagai pemilik tipe header saja.
#
#  daftar ini = WCN_MARLIN_COMMON_FILES + MARLINOFILES + rf/rf.o
#             + penjaga SDIOHAL + wcn_sec_verify_618.o (pengganti
#             wcn_firmware_sec_verify() milik wcn_ca_trusty.c yang tidak
#             bisa dibangun tanpa trusty-ipc).

ccflags-y += -I$(srctree)/$(src)/../include
ccflags-y += -I$(srctree)/$(src)/../boot
ccflags-y += -I$(srctree)/$(src)/rf

obj-$(CONFIG_WCN_CORE) += wcn_core.o

wcn_core-y += gnss/gnss_common_ctl.o
wcn_core-y += gnss/gnss_pmnotify_ctl.o
wcn_core-y += gnss/gnss_dbg.o
wcn_core-y += gnss/gnss_dump.o

# --- log / dump / debug ring ---
wcn_core-y += bufring.o
wcn_core-y += wcn_txrx.o
wcn_core-y += wcn_misc.o
wcn_core-y += wcn_parn_parser.o
wcn_core-y += wcn_log.o
wcn_core-y += wcn_procfs.o
wcn_core-y += loopcheck.o
wcn_core-y += reset.o
wcn_core-y += reset_test.o
wcn_core-y += rdc_debug.o
wcn_core-y += sysfs.o

# --- inti marlin (MARLINOFILES) ---
wcn_core-y += wcn_boot.o
wcn_core-y += wcn_dump.o
wcn_core-y += wcn_op.o

# --- jalur integrated (marlin3lite_sdio = chip integrated) ---
# wcn_dump_integrate DIBANGUN per 2026-10-13: wcn_procfs.c memanggil
# mdbg_snap_shoot_iram() di balik IS_ENABLED(CONFIG_WCN_INTEG) (=y di
# fragmen T1030) dan satu-satunya definisi ada di berkas ini (donor
# marohinmark_ums512 juga membangun kedua berkas dump dalam satu modul).
# Duplikat definisi vs wcn_dump.c (mdbg_dump_gnss_register /
# mdbg_dump_gnss_unregister / dump_arm_reg) diselesaikan dengan rename
# sufiks _integ di berkasnya, preseden yang sama dengan
# mdbg_dump_mem_integ. Konsumen procfs memakai milik wcn_dump.c (badan
# identik).
wcn_core-y += wcn_dump_integrate.o
wcn_core-y += rf/rf.o

# --- pengganti trusty: verifikasi citra (lihat berkasnya untuk deviasi) ---
wcn_core-y += wcn_sec_verify_618.o

# --- PM QoS WCN (revisi vendor; fork Tab A8 tidak memuat berkas ini) ---
# Menyediakan 10 ekspor blob wcn_bsp: wcn_pm_qos_init/exit/enable/disable/
# reset/get/condition_config/config_common + pm_qos_cond. Tanpa ini,
# WCN tidak pernah menaikkan batas PM QoS saat WiFi/BT throughput tinggi.
wcn_core-y += wcn_pm_qos.o

# Anggota komposit KONDISIONAL — bukan obj-y.
# Direktori ini di-descend lewat obj-m (CONFIG_WCN_CORE=m), dan obj-y di dalam
# direktori yang di-descend lewat obj-m hanya diarsipkan ke platform/built-in.a
# yang tidak ikut ke vmlinux MAUPUN ke wcn_core.ko: seluruh simbolnya hilang
# tanpa satu pun peringatan compile. Konsumennya justru anggota komposit ini:
#   mem_pd_init/exit/mgr/wifi_state/poweroff_deinit/save_bin -> wcn_boot.c,
#   wcn_procfs.c (inform_cp_wifi_download); wcn_init_debugfs -> wcn_boot.c.
# (dipindai mekanis: nol konsumen di luar komposit; gate T22 mengunci kelasnya)
wcn_core-$(CONFIG_WCN_UTILS) += wcn_debugfs.o
wcn_core-$(CONFIG_SDIOHAL)   += mem_pd_mgr.o
