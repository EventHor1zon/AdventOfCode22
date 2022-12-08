
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


directory_t *create_dir() {
    directory_t * dir = malloc(sizeof(directory_t));
    if(dir != NULL) {
        memset(dir, 0, sizeof(directory_t));
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

#ifdef DEBUG_MODE
    printf("Parsing instruction %s", line);
#endif

    // this is a console command
    if(line[0] == '$') {
        char cmd[4] = {0};
        sscanf(line+COMMAND_OFFSET, "%s", cmd);


        if(strncmp(&cmd[0], "c", 1) == 0) {
            char arg[32];
            sscanf(line+COMMAND_ARG_OFFSET, "%s", arg);
            /** handle ".." **/
            if(strncmp(&arg[0], ".", 1) == 0 && strncmp(&arg[1], ".", 1) == 0) {
                current_dir = current_dir->parent;
            }
            /** reset to root **/
            else if(strncmp(&arg[0], "/", 1) == 0) {
                current_dir = directory_tree;
            }
            else {
                current_dir = child_directory_from_name(arg);
                if(current_dir == NULL) {
                    printf("Directory not found in children!\n");
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

        current_dir->children[current_dir->num_children] = new_dir;
        current_dir->num_children++;
    };

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

void print_all_dirs() {
    printf("\n\n");
    for(int i=0; i < num_nodes; i++) {
        if(node_list[i] != NULL) {
            printf("Node %s : depth %u : child dirs: %u : files: %u : total_size: %d\n",
                node_list[i]->name,
                node_list[i]->depth,
                node_list[i]->num_children,
                node_list[i]->num_files,
                node_list[i]->total_size
            );
        }
    }
    
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
        for(int i=0; i < num_nodes; i++) {
            if(node_list[i]->depth == depth) {
                update_directory_size(node_list[i]);
                counter++;
            }
        }
        depth--;
        counter = 0;
    }

    return;
}


/**
 * challenge solve
 *  
 **/
uint32_t find_smallest_valid_dir() {

    /** start with largest possible size **/
    uint32_t smallest = UINT32_MAX;
    uint32_t total_tree_size = directory_tree->total_size;

    uint32_t free_size = (CHALLENGE_2_TOTAL_DISK_SZ - total_tree_size);
    uint32_t required_free_size = CHALLENGE_2_MINIMUM_REQ_SZ - free_size;

    printf("Total disk size: 70000000, used disk space: %u | free space %u, need 30000000: must free %u\n", total_tree_size, free_size, required_free_size);

    for(int i=0; i < num_nodes; i++) {
        if(node_list[i]->total_size > required_free_size) {
            if(node_list[i]->total_size < smallest) {
                smallest = node_list[i]->total_size;
            }
        }
    }

    return smallest;
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
    total_size = find_smallest_valid_dir();

    printf("Smallest valid node is %d\n\n", total_size);

    // print_all_dirs();

    free_nodes();

    return 1;
}