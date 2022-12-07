# Doing this one super simple to teach gf some python :)
#
#

import sys

counter = 0

with open(sys.argv[1]) as f:
    for line in f.readlines():
        line = line.strip()
        sets = line.split(",")
        elf1_lower = int(sets[0].split("-")[0])
        elf1_upper = int(sets[0].split("-")[1])
        elf2_lower = int(sets[1].split("-")[0])
        elf2_upper = int(sets[1].split("-")[1])

        if elf1_lower >= elf2_lower and elf1_upper <= elf2_upper:
            print(f"elf 1 contained! elf1_l {elf1_lower} elf1_u {elf1_upper} --  elf2_l {elf2_lower} elf2_u {elf2_upper}")
            counter += 1
        elif elf2_lower >= elf1_lower and elf2_upper <= elf1_upper:
            print(f"elf 2 contained! elf1_l {elf1_lower} elf1_u {elf1_upper} --  elf2_l {elf2_lower} elf2_u {elf2_upper}")
            counter += 1
        else:
            print(f"No elf containments :) elf1_l {elf1_lower} elf1_u {elf1_upper} --  elf2_l {elf2_lower} elf2_u {elf2_upper}")

print(f"Total overlaps: {counter}")