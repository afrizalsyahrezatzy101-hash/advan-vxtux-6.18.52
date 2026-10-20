import os
import shutil
import subprocess

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
OUT = "E:/vxtux_build_repo/build_output/vxtux_6.18.52"
os.makedirs(OUT, exist_ok=True)

# 1) the Image and vmlinux
for rel, dest in (("arch/arm64/boot/Image", "Image"),
                   ("vmlinux", "vmlinux")):
    tmp = os.path.join(os.environ.get("TMPDIR", "/tmp"), dest)
    r = subprocess.run(
        ["gh", "codespace", "cp", "-c", CS, f"remote:{ROOT}/{rel}", tmp],
        capture_output=True, text=True, timeout=900)
    if r.returncode == 0 and os.path.exists(tmp):
        shutil.copyfile(tmp, os.path.join(OUT, dest))
        print(f"OK  {dest}: {os.path.getsize(os.path.join(OUT, dest)):,} bytes")
    else:
        print(f"FAIL {dest}: {r.stderr.strip()[:150]}")

# 2) the .ko module list
r = subprocess.run(
    ["gh", "codespace", "ssh", "-c", CS, "--",
     f"cd {ROOT} && find . -name '*.ko' | sort"],
    capture_output=True, text=True, timeout=300)
mods = [l.strip() for l in r.stdout.splitlines() if l.strip().endswith(".ko")]
print(f"\nmodules found: {len(mods)}")
with open(os.path.join(OUT, "module_list.txt"), "w") as f:
    f.write("\n".join(mods))

# 3) the .ko files themselves (tar them on the remote side first)
r = subprocess.run(
    ["gh", "codespace", "ssh", "-c", CS, "--",
     f"cd {ROOT} && find . -name '*.ko' -print0 | tar --null -czf "
     f"/workspaces/vxtux-build/vxtux_modules.tar.gz -T - 2>/dev/null && "
     f"ls -la /workspaces/vxtux-build/vxtux_modules.tar.gz"],
    capture_output=True, text=True, timeout=600)
print(r.stdout.strip() or r.stderr.strip()[:200])

tmp = os.path.join(os.environ.get("TMPDIR", "/tmp"), "vxtux_modules.tar.gz")
r = subprocess.run(
    ["gh", "codespace", "cp", "-c", CS,
     "remote:/workspaces/vxtux-build/vxtux_modules.tar.gz", tmp],
    capture_output=True, text=True, timeout=900)
if r.returncode == 0 and os.path.exists(tmp):
    shutil.copyfile(tmp, os.path.join(OUT, "vxtux_modules.tar.gz"))
    print(f"OK  modules tarball: {os.path.getsize(tmp):,} bytes")
else:
    print(f"FAIL modules tarball: {r.stderr.strip()[:150]}")
