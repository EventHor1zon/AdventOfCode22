
#include <stdint.h>

// #define DEBUG_MODE 1

#define MAX_ARRAY_ROWS      512
#define MAX_ARRAY_COLUMNS   512

#define TREE_VISIBLE_TOP    (1 << 0)
#define TREE_VISIBLE_RIGHT  (1 << 1)
#define TREE_VISIBLE_BOT    (1 << 2)
#define TREE_VISIBLE_LEFT   (1 << 3)