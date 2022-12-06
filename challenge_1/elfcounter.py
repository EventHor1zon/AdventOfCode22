import sys


def main(infile: str):
    elves = []
    elf_index = 0
    elf_total = 0

    with open(infile, "r") as f:
        for line in f.readlines():
            if len(line.strip()) > 0:
                elf_total += int(line.strip())
            else:
                elves.append(elf_total)
                elf_total = 0
                elf_index += 1
    
    max_elf = elves.index(max(elves))
    print(f"Elf #{max_elf} is carrying the most calories, with {elves[max_elf]} total")




if __name__ == '__main__':
    infile = sys.argv[1]
    main(infile)


