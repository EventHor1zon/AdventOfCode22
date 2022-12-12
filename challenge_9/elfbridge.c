
/** 
 *  elfbridge.c 
 *      This challenge doesn't seem too complicated
 *      Head (H) and Tail (T) start at 0,0. Head moves
 *      about and tail must follow, remaining within 1 dist 
 *      at all times. Max dist = 1 so tail doesn't move until
 *      head is at least 2 away
 ***/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "elfbridge.h"


/**
 * wanted to do uniquifying at the end but 
 *  this ends up very large (>12,000) so uniquify 
 *  each time
 **/
int position_counter = 1;
position_t tail_positions[MAX_MOVES];

position_t head = {0};
position_t tail = {0};


bool tail_in_list() {
    for(int i=0; i<position_counter;i++) {
        if(tail.x == tail_positions[i].x &&
           tail.y == tail_positions[i].y 
        ) {
            return true;
        }
    }
    return false;
}


/** 
 * Update the position of the tail
 *  using the head pos
*/

void update_tail() {

    position_t pre_tail = {
        .x = tail.x,
        .y = tail.y
    };

    if(head.x > tail.x+1) {
        tail.x++;
        /** check if we need to change columns **/
        if(tail.y < head.y) {
            tail.y++;
        }
        else if(tail.y > head.y) {
            tail.y--;
        }
    }
    else if(head.x < tail.x-1) {
        tail.x--;
        if(tail.y < head.y) {
            tail.y++;
        }
        else if(tail.y > head.y) {
            tail.y--;
        }
    }
    else if(head.y > tail.y+1) {
        tail.y++;
        if(tail.x < head.x) {
            tail.x++;
        }
        else if(tail.x > head.x) {
            tail.x--;
        }
    }
    else if(head.y < tail.y-1) {
        tail.y--;
        if(tail.x < head.x) {
            tail.x++;
        }
        else if(tail.x > head.x) {
            tail.x--;
        }
    }

#ifdef DEBUG_MODE
    printf("T {%d:%d}\n", tail.x, tail.y);
#endif


    /** if tail position changed, add to the list of prev
     *  positions
     *  **/
    if(pre_tail.x != tail.x || pre_tail.y != tail.y) {
        if(tail_in_list() == false) {
            tail_positions[position_counter].x = tail.x;
            tail_positions[position_counter].y = tail.y;
            position_counter++;
#ifdef DEBUG_MODE
            printf("New position added! {%d:%d} (%d)\n", tail.x, tail.y, position_counter);
#endif

        }
    }

    return;
}




void update_positions(uint8_t *dir) {
    
    if(*dir == ASCII_D_HEX) {
        head.y -= 1;
    }
    else if (*dir == ASCII_U_HEX) {
        head.y += 1;
    }
    else if (*dir == ASCII_L_HEX) {
        head.x -= 1;
    }
    else if (*dir == ASCII_R_HEX) {
        head.x += 1;
    }
    else {
        printf("Error parsing instruction! %u\n", *dir);
    }

#ifdef DEBUG_MODE
    printf("H {%d:%d}\t", head.x, head.y);
#endif
    update_tail();

    return;
}


int main(int argc, char *argv[]) {

    char line_dir;
    int line_steps;
    int counter = 0;

    if(argc < 2) {
        printf("[Usage] %s <input_file>\n", argv[0]);
        return 0;
    }

    FILE *ptr = fopen(argv[1], "r");    

    while(fscanf(ptr, "%c %d\n", &line_dir, &line_steps) != EOF) {
        /** update positions from line instructions **/
#ifdef DEBUG_MODE
        printf("Read line number %d %c %d\n", counter, line_dir, line_steps);
        counter++;
#endif
        for(int i=0; i < line_steps; i++) {
            /** typecast because comparison easier 
             *  with integers. char essentially uint8_t
             */
            update_positions((uint8_t *)&line_dir);
        }
    }

    fclose(ptr);

    printf("Done reading file - position counter = %u unique positions\n", position_counter);

    return 1;
}