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


def winner_score(input):
    if input == 'A':
        return rsp['Y']
    elif input == 'B':
        return rsp['Z']
    elif input == 'C':
        return rsp['X']

def draw_score(input):
    return req[input]

def lose_score(input):
    if input == 'A':
        return rsp['Z']
    elif input == 'B':
        return rsp['X']
    elif input == 'C':
        return rsp['Y']

def parse_plays(plays: list) -> int:
    if plays[1] == 'Z':
        ## win this round
        return ROUND_WIN_PTS + winner_score(plays[0])
    elif plays[1] == 'Y':
        ## draw this round
        return ROUND_DRAW_PTS + draw_score(plays[0])
    else:
        ## lose this round
        return ROUND_LOSE_PTS + lose_score(plays[0])

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


