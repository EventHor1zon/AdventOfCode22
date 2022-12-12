
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


position_t knots[ELF2_NUM_KNOTS] = {0};

position_t *head = &knots[0];
position_t *tail = &knots[ELF2_NUM_KNOTS-1];


bool tail_in_list() {
    for(int i=0; i<position_counter;i++) {
        if(tail->x == tail_positions[i].x &&
           tail->y == tail_positions[i].y 
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

void update_next_knot(position_t *current, position_t *parent) {

    if(parent->x > current->x+1) {
        current->x++;
        /** check if we need to change columns **/
        if(current->y < parent->y) {
            current->y++;
        }
        else if(current->y > parent->y) {
            current->y--;
        }
    }
    else if(parent->x < current->x-1) {
        current->x--;
        if(current->y < parent->y) {
            current->y++;
        }
        else if(current->y > parent->y) {
            current->y--;
        }
    }
    else if(parent->y > current->y+1) {
        current->y++;
        if(current->x < parent->x) {
            current->x++;
        }
        else if(current->x > parent->x) {
            current->x--;
        }
    }
    else if(parent->y < current->y-1) {
        current->y--;
        if(current->x < parent->x) {
            current->x++;
        }
        else if(current->x > parent->x) {
            current->x--;
        }
    }

    return;
}




void update_head_position(uint8_t *dir) {
    
    if(*dir == ASCII_D_HEX) {
        head->y -= 1;
    }
    else if (*dir == ASCII_U_HEX) {
        head->y += 1;
    }
    else if (*dir == ASCII_L_HEX) {
        head->x -= 1;
    }
    else if (*dir == ASCII_R_HEX) {
        head->x += 1;
    }
    else {
        printf("Error parsing instruction! %u\n", *dir);
    }

// #ifdef DEBUG_MODE
//     printf("H {%d:%d}\t", head->x, head->y);
// #endif
    
    for(int i=1; i<ELF2_NUM_KNOTS; i++) {
        update_next_knot(&knots[i], &knots[i-1]);
    }

    if(tail_in_list() == false) {
        tail_positions[position_counter].x = tail->x;
        tail_positions[position_counter].y = tail->y;
        position_counter++;
    }

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
            update_head_position((uint8_t *)&line_dir);
        }
    }

    fclose(ptr);

    printf("Done reading file - position counter = %u unique positions\n", position_counter);

    return 1;
}