#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// #define DEBUG_MODE 1

#define MAX_NODES 2056

#define MAX_CHILD_DIRS 32
#define MAX_CHILD_FILES 32
#define MAX_NAME_LEN 32

#define COMMAND_OFFSET 2
#define COMMAND_ARG_OFFSET 4

#define CHALLENGE_THRESHOLD_SIZE 100000
#define CHALLENGE_2_THRESHOLD    70000000
#define CHALLENGE_2_MINIMUM      30000000
typedef struct directory_t directory_t;

typedef enum {
    CMD_TYPE_CD,
    CMD_TYPE_LS,
} cmd_t;

typedef struct {
    char *name;
    uint32_t size;
} file_t;

struct directory_t {
    char name[MAX_NAME_LEN];
    directory_t *parent;
    int num_children;
    directory_t *children[MAX_CHILD_DIRS];
    int num_files;
    file_t files[MAX_CHILD_FILES];
    uint32_t total_size;
    uint32_t depth;
};

