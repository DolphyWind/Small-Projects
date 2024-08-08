from pathlib import Path
import os

cats = Path('./cats')
files = list(map(str, cats.iterdir()))

for i, filename in enumerate(files):
    ext = filename.split('.')[-1]
    os.rename(filename, f"./cats/temp{i}.{ext}")


files = list(map(str, cats.iterdir()))
for i, filename in enumerate(files):
    ext = filename.split('.')[-1]
    os.rename(filename, f"./cats/cat{i}.{ext}")
