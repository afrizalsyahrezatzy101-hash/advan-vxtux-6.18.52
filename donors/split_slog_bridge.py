#!/usr/bin/env python3
"""Move slog_bridge.o out of the built-in SIPC core into its own module.

slog_bridge.o calls kernel_vser_register_callback() / kernel_vser_set_pass_mode(),
which live in usb_f_vserial.ko (a MODULE). A built-in object cannot import a
module symbol, so slog_bridge has to be a module too: it imports the
built-in SIPC core (sblock_*) and the usb_f_vserial module.

    python3 split_slog_bridge.py drivers/vxtux/sipc/Makefile
"""
import sys

MAKEFILE = sys.argv[1] if len(sys.argv) > 1 else "drivers/vxtux/sipc/Makefile"

NOTE = """
# slog_bridge talks to the USB VSER gadget function (usb_f_vserial.ko, a
# MODULE) through kernel_vser_*. A built-in object cannot import a module
# symbol, so slog_bridge is a module of its own: it imports the built-in
# SIPC core (sblock_*) and the usb_f_vserial module.
obj-$(CONFIG_SPRD_SIPC_SLOG_BRIDGE) += slog_bridge.o
"""


def main():
    raw = open(MAKEFILE, "rb").read()
    nl = b"\r\n" if b"\r\n" in raw else b"\n"

    lines = raw.split(nl)
    out = []
    changed = False
    for line in lines:
        stripped = line.strip()
        if stripped.startswith(b"obj-$(CONFIG_SPRD_SIPC)") and \
                b"slog_bridge.o" in stripped:
            # this is the continuation line that lists slog_bridge.o
            out.append(line.replace(b" slog_bridge.o", b""))
            changed = True
            continue
        out.append(line)
        # after the SPRD_SIPC block ends, insert the new module rule
        if changed and stripped.startswith(b"obj-$(CONFIG_SPRD_SIPC)"):
            pass

    if not changed:
        print("pattern not found")
        return

    text = nl.join(out)
    anchor = b"obj-$(CONFIG_SPRD_SIPC_SWCNBLK)"
    if anchor not in text:
        print("anchor not found")
        return
    text = text.replace(anchor, NOTE.strip().encode().replace(b"\n", nl) + nl + nl + anchor, 1)
    open(MAKEFILE, "wb").write(text)
    print("ok")


if __name__ == "__main__":
    main()
