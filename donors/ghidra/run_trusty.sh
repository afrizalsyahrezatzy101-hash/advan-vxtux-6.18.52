#!/bin/bash
# Import + decompile the three stock Trusty blobs into Ghidra.
set -x
cd /e/vxtux_build_repo/donors

IMP=/e/vxtux_build_repo/donors/stock_vendor_modules
PROJ=/e/ghidra_proj

for f in trusty-irq trusty-log trusty-tui; do
  cmd.exe /c "E:\\tools\\ghidra_headless.cmd E:\\ghidra_proj ${f} -import E:\\vxtux_build_repo\\donors\\stock_vendor_modules\\${f}.ko -processor AARCH64:LE:64:v8A -scriptPath E:\\vxtux_build_repo\\donors\\ghidra -postScript DumpSyms.java list"
done

# decompile every function of each blob
for f in trusty-irq trusty-log trusty-tui; do
  case $f in
    trusty-irq) SYMS="trusty_irq_probe trusty_irq_remove trusty_irq_handler trusty_irq_init_normal_irq trusty_irq_init_per_cpu_irq trusty_irq_free_irqs trusty_irq_enable_irqset trusty_irq_disable_irqset trusty_irq_enable_pending_irqs trusty_irq_create_irq_mapping trusty_irq_cpu_up trusty_irq_cpu_down trusty_irq_call_notify init_module cleanup_module";;
    trusty-log) SYMS="trusty_log_probe trusty_log_remove trusty_dump_logs trusty_log_alloc_pages trusty_log_register_buf trusty_logsize_show trusty_logsize_store trusty_loglevel_show trusty_loglevel_store trusty_console_show trusty_console_store trusty_log_call_notify trusty_log_panic_notify init_module cleanup_module";;
    trusty-tui) SYMS="trusty_tui_probe trusty_tui_remove tui_open tui_read tui_write tui_poll tui_release is_in_tui notify_cancel_tui init_module cleanup_module";;
  esac
  cmd.exe /c "E:\\tools\\ghidra_headless.cmd E:\\ghidra_proj ${f} -process ${f}.ko -scriptPath E:\\vxtux_build_repo\\donors\\ghidra -postScript DumpFuncs.java $SYMS"
done
echo "ALL DONE"
