import sys
import os
import argparse

parser = argparse.ArgumentParser(description="Runs a test of the Display module.")
parser.add_argument(
    "pydDirs",
    nargs="+",
    help="Directory (or directories) containing the .pyd module to add to sys.path",
)
args = parser.parse_args()

for pydDir in args.pydDirs:
    pydDir = os.path.abspath(pydDir)
    if not os.path.isdir(pydDir):
        parser.error(f"Directory does not exist: {pydDir}")
    if pydDir not in sys.path:
        sys.path.append(pydDir)

import Display

temp = Display.Test()
print(temp.getA())
