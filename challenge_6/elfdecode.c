/**
 * elfdecode.c
 * try one in c this time, it's been ages since I last wrote
 *  some basic c.
 * 
 *  **/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int find_elf_location(char *linebuffer) {

    // set offset as 1 (challenge counts from 1)
    int offset = 1;
    char *ptr = linebuffer;
    uint32_t length = strlen(linebuffer);
    uint8_t check_len;

    if(length < 4) {
        // return something more sensible?
        printf("Error: %u is invalid line length\n", length);
        return -1;
    }

    check_len = 4;
    uint8_t pass;

    // dont read past end of buffer
    while(offset < length-4) {
        
        pass = 1;

        for(int i=1; i<check_len; i++) {
            // if any are the same, fail check & break 
            // for loop
            if(*ptr == *(ptr+i)) {    
                pass = 0;
                break;
            }
        }

        if(pass == 0) {
            check_len = 4; // start full check on next char
        }
        else {
            check_len--;
        }
        // we have checked 4 characters and all
        // are "unique"
        if(check_len == 0) {
            printf("Check length has reached 0. We are successful! Offset: %u\n", offset);
            break;
        } else {
            offset++;
            ptr++;
        }

    }

    // challenge requires offset to end of sequence 
    return offset+4; // if we don't find a sequence, returns length
}



int main(int argc, char *argv[]) {

    char linebuffer[4096];
    int offset;

    if(argc < 2) {
        printf("[Usage] %s <input_file>\n", argv[0]);
        return 0;
    }

    FILE *ptr = fopen(argv[1], "r");    

    if(ptr == NULL) {
        printf("Error opening file\n");
        return 0;
    }

    /** assumed this would be many shorter strings but it's a single long one **/
    while(fgets(linebuffer, 4096, ptr) != NULL) {
        offset = find_elf_location(linebuffer);

        if(offset < 0 || offset > 4095) {
            printf("Error: sequence not found!\n");
        }
    }

    fclose(ptr);

    return 1;
}