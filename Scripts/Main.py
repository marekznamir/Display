import sys
import os

pydDir = r"C:\Users\markz\source\repos\Display\out\build\Release-GCC-16.1.0-x86_64-w64-mingw32"

if pydDir not in sys.path:
    sys.path.append(pydDir)

import Display

temp = Display.Test()
print(temp.getA())