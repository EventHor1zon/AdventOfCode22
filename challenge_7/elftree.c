
/**
 * elftree.c
 * 
 * Another C one - want to try some tree/linked list implementation
 * 
 **/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "elftree.h"

// start of directory tree
static directory_t *directory_tree;
static directory_t *current_dir;

void debug_print(char *string) {
#ifdef DEBUG_MODE
    printf("%s", string);
#endif
}


directory_t *create_dir() {
    return (directory_t *)calloc(1, sizeof(directory_t));
}

/**
 * 
 *  **/
directory_t *child_directory_from_name(char *name) {
    directory_t *child;
    for(int i=0; i< current_dir->num_children; i++) {
        child = current_dir->children[i];
        if(child == NULL) {
            printf("Error: Could not find child with name %s\n", name);
            return NULL;
        }
        if(strncmp(name, child->name, MAX_NAME_LEN) == 0) {
            /* name match, return */
            break;
        }
    }

    return child;
}

/**
 *  Parses each instruction and builds the tree 
 *  of directories from the information
 *  
 **/
void parse_instruction(char *line) {


    if(strlen(line) <= 1) {
        return;
    }

    // this is a console command
    if(line[0] == '$') {
        char cmd[3];
        sscanf(line+COMMAND_OFFSET, "%s", cmd);
        if(cmd[0] == "c") {
            char arg[32];
            sscanf(line+COMMAND_ARG_OFFSET, "%s", arg);

            /** handle ".." **/
            if(arg[0] == "." && arg[1] == ".") {
                current_dir = current_dir->parent;
            }
            /** reset to root **/
            else if(arg[0] == "/") {
                current_dir = directory_tree;
            }
            else {
                current_dir = directory_from_name(arg);
                if(current_dir == NULL) {
                    exit(1);
                }
            }
        }
        /* we don't really care about ls command */
    }
    // this is a file, starting with a size 
    else if(isdigit(line[0])) {
        sscanf(line, "%d %32s", current_dir->files[current_dir->num_files].size,
                                current_dir->files[current_dir->num_files].name
                            );
        current_dir->num_files++;
    }
    // this should be a dir
    else {
        char cmd[3];
        directory_t *new_dir = create_dir();
        if(new_dir == NULL) {
            printf("Error: New dir is NULL :(\n");
            exit(1);
        }

        sscanf(line, "%3s %32s", cmd, new_dir->name);
        new_dir->parent = current_dir;

        current_dir->children[current_dir->num_children] = new_dir;
        current_dir->num_children++;
    };

    return;
}


/**
 * crawl the directories and  
 * calculate their total sizes
 *  from the bottom up
 **/
void calculate_directory_sizes() {

}


/**
 * challenge solve
 *  
 **/
uint32_t find_small_dirs() {
    return 0;
}


int main(int argc, char *argv[]) {

    char linebuff[256];
    uint32_t total_size;

    if(argc < 2) {
        printf("[Usage] %s <input_file>\n", argv[0]);
        return 0;
    }

    FILE *ptr = fopen(argv[1], "r");    

    while(fgets(linebuff, 256, ptr) != NULL) {
        parse_instruction(linebuff);
    }


    /** we have parsed the instructions and can do the calculations **/
    calculate_directory_sizes();

    /** find the directories which meet criteria **/
    total_size = find_small_dirs();

    return 1;
}