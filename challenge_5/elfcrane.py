
import sys
import re

def parse_instructions(instr: str) -> list:
    instruction_list = []
    for line in instr.split("\n")[:-1]:
        digits = [int(s) for s in re.findall(r'\d+', line)]
        instruction_list.append({
            'num': digits[0],
            'source': digits[1]-1,
            'dest': digits[2]-1
        })
    return instruction_list


def parse_row(line: str):
    out = ""
    ## just keep specific columns - these
    ## are 1,5,9 etc. x % 4 = 1
    for x in range(len(line)):
        if x % 4 == 1:
            out += line[x]
    return out

def stacks_to_lists(stacks):
    ## remove square brackets and spaces
    stack_list = []

    rows = []
    for line in stacks.split("\n"):
        rows.append(parse_row(line))

    ## transpose the rows into stacks

    ## we have the list of rows top to bottom
    ## they should all be same len
    for index in range(len(rows[0])):
        stack = []
        for row in rows:
            if row[index] != " " and row[index].isalpha():
                stack.append(row[index])
        ## reverse so they're in stack order
        stack.reverse()
        stack_list.append(stack)
    
    return stack_list

def perform_instructions(stacks, dirs):

    counter = 0
    for dir in dirs:
        for i in range(dir['num']):
            item = stacks[dir['source']].pop()
            stacks[dir['dest']].append(item)
            print(f"counter {counter}")
            counter += 1
    return stacks

def split_file(file):
    with open(file) as f:
        input = f.read()
    return input.split("\n\n")


def main(infile):
    
    stacks, instructions = split_file(infile)

    stack_lists = stacks_to_lists(stacks)

    instruction_list = parse_instructions(instructions)

    final_stacks = perform_instructions(stack_lists, instruction_list)

    print(final_stacks)
    
    answer = ""
    for stack in final_stacks:
        answer += stack.pop()
    print(f"Answer: {answer}")

if __name__ == '__main__':
    main(sys.argv[1])