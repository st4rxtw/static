#!/usr/bin/env python3

from pathlib import Path

HEADER = """/*
 *\tCopyright 2026-2027 static - hiro
 *
 *\tUse of this source code without explicit permission from owner is strictly prohibited.
 */

"""

EXTENSIONS = {
    ".h",
    ".hpp",
    ".cpp"
}

ROOT = Path("../../Include")

if not ROOT.is_dir():
    print(f'Error: "{ROOT}" does not exist.')
    exit(1)

count = 0
skipped = 0

for file in ROOT.rglob("*"):
    if not file.is_file():
        continue

    if file.suffix.lower() not in EXTENSIONS:
        continue

    text = file.read_text(encoding="utf-8", errors="ignore")

    if "Copyright 2026-2027 static - hiro" in text:
        skipped += 1
        continue

    file.write_text(HEADER + text, encoding="utf-8")

    print(f"Added header: {file}")
    count += 1

print(f"\nDone.")
print(f"Modified: {count}")
print(f"Skipped : {skipped}")
