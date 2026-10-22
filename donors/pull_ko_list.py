import os
import subprocess

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
OUT = "E:/vxtux_build_repo/build_output/vxtux_6.18.52"
TMP = os.environ.get("TMPDIR", "/tmp")

# list every .ko the build produced, with its size
script = (f"cd {ROOT} && find . -name '*.ko' -printf '%P %s\\n' "
          "| sort > /workspaces/vxtux-build/ko_list86.txt && "
          "wc -l /workspaces/vxtux-build/ko_list86.txt")
r = subprocess.run(["gh", "codespace", "ssh", "-c", CS, "--", script],
                   capture_output=True, text=True, timeout=600)
print("remote:", r.stdout.strip() or r.stderr.strip()[:200])

flat = os.path.join(TMP, "ko_list86.txt")
r = subprocess.run(["gh", "codespace", "cp", "-c", CS,
                    "remote:/workspaces/vxtux-build/ko_list86.txt", flat],
                   capture_output=True, text=True, timeout=900)
if r.returncode == 0 and os.path.exists(flat):
    dst = os.path.join(OUT, "ko_list.txt")
    with open(flat, "rb") as a, open(dst, "wb") as b:
        b.write(a.read())
    lines = open(dst, encoding="utf-8", errors="replace").read().splitlines()
    print(f"OK  {len(lines)} .ko files -> {dst}")
else:
    print("FAIL:", r.stderr.strip()[:200])
