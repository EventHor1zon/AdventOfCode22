## try and do this one in as few lines as possible


####
# List of "items" in two halves
# find duplicate items in each pack
# a-z have prio 1-26, A-Z prio 27-52
# find Sum of priorities of duplicate item types

import sys
## so stupid and unreadable ## 
with open(sys.argv[1]) as f:
    print(sum([ord(c)-96 if c.islower() else ord(c)-38 for c in \
        [x for line in \
            [l.strip() for l in f.readlines() if len(l)] \
                 for x in set(line[int(len(line)/2):]) \
                     if x in set(line[:int(len(line)/2)])]]))
