
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

directory_t *node_list[MAX_NODES] ={0};
int num_nodes = 0;

void debug_print(char *string) {
#ifdef DEBUG_MODE
    printf("%s", string);
#endif
}

int counter = 0;

directory_t *create_dir() {
    directory_t * dir = malloc(sizeof(directory_t));
    if(dir != NULL) {
        memset(dir, 0, sizeof(directory_t));
        printf("Created new directory!\n");
        node_list[num_nodes] = dir;
        num_nodes++;
    }
    return dir;
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

    printf("%d ", counter);
    debug_print("Parsing instruction ");
    debug_print(line);
    debug_print("\n");


    // this is a console command
    if(line[0] == '$') {
        debug_print("Found console command - ");

        char cmd[4] = {0};
        sscanf(line+COMMAND_OFFSET, "%s", cmd);
        debug_print(cmd);
        debug_print(" -> ");

        if(strncmp(&cmd[0], "c", 1) == 0) {
            char arg[32];
            sscanf(line+COMMAND_ARG_OFFSET, "%s", arg);
            /** handle ".." **/
            if(strncmp(&arg[0], ".", 1) == 0 && strncmp(&arg[1], ".", 1) == 0) {
                current_dir = current_dir->parent;
                debug_print(".. \n");
            }
            /** reset to root **/
            else if(strncmp(&arg[0], "/", 1) == 0) {
                current_dir = directory_tree;
                debug_print("/\n");
            }
            else {
                debug_print(arg);
                debug_print("\n");
                current_dir = child_directory_from_name(arg);
                if(current_dir == NULL) {
                    debug_print("Directory not found in children!\n");
                    exit(1);
                }
            }
        }
        /* we don't really care about ls command */
    }
    // this is a file, starting with a size 
    else if(isdigit(line[0])) {
        sscanf(line, "%d %32s", &current_dir->files[current_dir->num_files].size,
                                &current_dir->files[current_dir->num_files].name[0]
                            );
        debug_print("Found file ");
        debug_print(&current_dir->files[current_dir->num_files].name[0]);
        debug_print("\n");

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
        new_dir->depth = new_dir->parent->depth+1;
        debug_print("Found dir: ");
        debug_print(new_dir->name);
        debug_print("\n");
        current_dir->children[current_dir->num_children] = new_dir;
        current_dir->num_children++;
    };

    counter++;

    return;
}



int find_max_depth() {
    int max=0;
    for(int i=0; i < num_nodes; i++) {
        if(node_list[i] != NULL && node_list[i]->depth > max) {
            max = node_list[i]->depth;
        }
    }
    return max;
}


void update_directory_size(directory_t *dir) {

    /** calculate file size **/
    for(int i=0; i < dir->num_files; i++) {
        dir->total_size += dir->files[i].size;
    }

    /** add on sizes of previous dirs **/
    for(int i=0; i < dir->num_children; i++) {
        dir->total_size += dir->children[i]->total_size;
    }

    return;
}

/**
 * crawl the directories and  
 * calculate their total sizes
 *  from the bottom up - start with the deepest nodes
 **/
void calculate_directory_sizes() {

    int depth = find_max_depth();
    int counter = 0;

    while(depth >= 0) {
        printf("Finding all nodes at depth %u\n", depth);
        for(int i=0; i < num_nodes; i++) {
            if(node_list[i]->depth == depth) {
                update_directory_size(node_list[i]);
                counter++;
            }
        }
        printf("Found %u nodes at depth %u\n", counter, depth);
        depth--;
        counter = 0;
    }

    return;
}


/**
 * challenge solve
 *  
 **/
uint32_t find_small_dirs() {

    uint32_t small_dir_size = 0;

    for(int i=0; i < num_nodes; i++) {
        if(node_list[i]->total_size <= CHALLENGE_THRESHOLD_SIZE) {
            small_dir_size += node_list[i]->total_size;
        }
    }

    return small_dir_size;
}

void free_nodes() {
    for(int i=0; i < num_nodes; i++) {
        if(node_list[i] != NULL) {
            free(node_list[i]);
        }
    }
}


int main(int argc, char *argv[]) {

    char linebuff[256];
    uint32_t total_size;

    if(argc < 2) {
        printf("[Usage] %s <input_file>\n", argv[0]);
        return 0;
    }

    directory_tree = create_dir();
    if(directory_tree == NULL) {
        printf("Error creating base dir\n");
        exit(1);
    }
    current_dir = directory_tree;

    FILE *ptr = fopen(argv[1], "r");    

    while(fgets(linebuff, 256, ptr) != NULL) {
        parse_instruction(linebuff);
    }

    fclose(ptr);
    /** we have parsed the instructions and can do the calculations **/
    calculate_directory_sizes();

    /** find the directories which meet criteria **/
    total_size = find_small_dirs();

    printf("Total size of nodes < 10000 is %d\n\n", total_size);

    free_nodes();

    return 1;
}