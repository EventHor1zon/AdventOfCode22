/**
 * elfforrest.c
 * 
 * Another C one - aray of integers representing tree heights.
 *                  Need to figure out which trees are visible
 *                  Try assembling a 3-d array for the tree info
 *                  Then another 3-d array for visibility for each tree?
 **/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "elfforest.h"


int tree_array[MAX_ARRAY_ROWS][MAX_ARRAY_COLUMNS];
int visibility[MAX_ARRAY_ROWS][MAX_ARRAY_COLUMNS];

int current_row = 0;
int current_column = 0;
int num_rows = 0;
int num_columns = 0;

void parse_line(char *line) {
    uint32_t len = strlen(line);
    int tree_height = 0;
    char *ptr = line;

    if(len > MAX_ARRAY_COLUMNS) {
        printf("Error: Not enough space for line length %u\n", len);
        exit(0);
    }
    else if(len < 2 || strncmp(line, "\n", 1) == 0 || &line[0] == NULL) {
        // probably a blank line / \n char
        return;
    }

    for(int i=0; i < len; i++) {
        tree_height = ((int)*ptr)-0x30;  /** ascii offset, quicker than atoi/strtol */
#ifdef DEBUG_MODE
        printf("Writing %d to [%d:%u]\n", tree_height, current_row, i);
#endif
        tree_array[current_row][i] = tree_height;
        ptr++;
    }

    /** set the max columns from the length **/
    if(len != num_columns) {
        num_columns = len;
    }
    
    current_row++;
}


void build_visibility_matrix() {

    int max = 0;
    uint8_t counter;
    int height;
    int seek_r;
    int seek_c;

    memset(visibility, 0, (sizeof(int) * MAX_ARRAY_ROWS) * MAX_ARRAY_COLUMNS);

    // we can ignore the first and last rows/columns
    // for this challenge - score will always be 0
    for(int r=1; r<num_rows-1; r++) {
        for(int c=1; c<num_columns-1; c++) {
            counter = 0;
            height = tree_array[r][c];
            seek_r = r;
            seek_c = c;
            /** look left */
            while(seek_r > -1) {
                while(seek_c < num_columns) {
                    if(height > tree_array[seek_r][seek_c]) {
                        // taller than this tree & can see over it
                        counter++;
                    }
                    else {
                        // cannot see past this tree
                        counter++;
                        break;
                    }
                    seek_c++;
                }
                seek_r--;
            }

            counter = 0;
            seek_r = r;
            seek_c = c;
            /** look up */
            while(seek_r > -1) {
                while(seek_c < num_columns) {
                    if(height > tree_array[seek_r][seek_c]) {
                        // taller than this tree & can see over it
                        counter++;
                    }
                    else {
                        // cannot see past this tree
                        counter++;
                        break;
                    }
                    seek_c++;
                }
                seek_r--;
            }

        }
    }



    printf("Done parsing visibility\n");

#ifdef DEBUG_MODE

    printf("Dumping to file\n");
    FILE *ptr = fopen("./visibilitymap.txt", "w+");
    for(int r=0; r < num_rows; r++) {
        for(int c=0; c < num_columns-1; c++) {
            fprintf(ptr, "%u", visibility[r][c]);
        }
        fprintf(ptr, "\n");
    }
    fclose(ptr);
#endif 

#ifdef DEBUG_MODE
    // printf("Dumping to file\n");
    // FILE *ptra = fopen("./treemap.txt", "w+");
    // for(int r=0; r < num_rows; r++) {
    //     for(int c=0; c < num_columns-1; c++) {
    //         fprintf(ptra, "%u", tree_array[r][c]);
    //     }
    //     fprintf(ptra, "\n");
    // }
    // fclose(ptra);
#endif

}


uint32_t find_most_visible_tree() {

    uint32_t most = 0;

    /** itterate over visibility matrix
     *  and increment total if tree is visible from any
     *  angle 
     ***/
    for(int r=0; r < num_rows; r++) {
        for(int c=0; c < num_columns; c++) {
            if(visibility[r][c] > most) {
                most = visibility[r][c];
            }
        }
    }

    return most;

}


int main(int argc, char *argv[]) {

    char linebuff[256];

    uint32_t total_visible = 0;

    if(argc < 2) {
        printf("[Usage] %s <input_file>\n", argv[0]);
        return 0;
    }

    FILE *ptr = fopen(argv[1], "r");    

    while(fgets(linebuff, 256, ptr) != NULL) {
        parse_line(linebuff);
    }

    fclose(ptr);

    printf("Done parsing %u rows of %u trees\n", current_row, num_columns);
    num_rows = current_row;

    current_row = 0;

    build_visibility_matrix();

    
    total_visible = find_most_visible_tree();

    printf("Tree with the best view: %u\n", total_visible);

    return 1;
}