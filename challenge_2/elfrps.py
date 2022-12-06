import sys


ROUND_WIN_PTS = 6
ROUND_DRAW_PTS = 3
ROUND_LOSE_PTS = 0

req = {
    'A': 1, ## rick
    'B': 2, ## paper
    'C': 3, ## morty
}

rsp = {
    'X': 1, ## rock
    'Y': 2, ## paper
    'Z': 3  ## scissors
}

def parse_plays(plays: list) -> int:
    ## handle draw condition
    if req[plays[0]] == rsp[plays[1]]:
        return ROUND_DRAW_PTS + rsp[plays[1]]
    ## handle the three win conditions
    elif (plays[0] == 'A' and plays[1] == 'Y') or \
         (plays[0] == 'B' and plays[1] == 'Z') or \
         (plays[0] == 'C' and plays[1] == 'X'):
        return ROUND_WIN_PTS + rsp[plays[1]]
    ## else lose
    else:
        return ROUND_LOSE_PTS + rsp[plays[1]]


def main(infile: str):

    score = 0

    with open(infile, "r") as f:
        for line in f.readlines():
            if len(line.strip()) > 0: 
                plays = line.split()
                score += parse_plays(plays)
    print(f"Total Score from strategy is {score}")



if __name__ == '__main__':
    infile = sys.argv[1]
    main(infile)


