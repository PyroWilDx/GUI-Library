#!/usr/bin/env python3

import os
import sys

def main():
    """."""

    exe = "./cmake/minesweeper"
    nom = "-1"

    if len(sys.argv) > 1:
        exe = sys.argv[1]
        if len(sys.argv) > 2:
            nom = sys.argv[2]

    line_1 = f"gprof {exe} gmon.out > Profile.txt"
    line_2 = f"gprof2dot Profile.txt -o Profile.dot"
    line_3 = f"dot -Tpng Profile.dot -o Profile{nom}.png"
    os.system(line_1)
    os.system(line_2)
    os.system(line_3)
    os.system(f"eog Profile{nom}.png")



main()