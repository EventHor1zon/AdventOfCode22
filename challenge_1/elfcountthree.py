import sys


def main(infile: str):
    elves = []
    max_elves = []
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
    
    for counter in range(3):
        max_elves.append(max(elves))
        elves.pop(elves.index(max(elves)))


    print(f"The three most overburdened elves are carrying {max_elves[0]},{max_elves[1]},{max_elves[2]} respectively")
    print(f"Total: {sum(max_elves)}")



if __name__ == '__main__':
    infile = sys.argv[1]
    main(infile)


