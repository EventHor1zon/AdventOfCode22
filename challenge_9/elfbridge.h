
#include <stdint.h>

#define MAX_MOVES       12000

#define ASCII_L_HEX     0x4CU
#define ASCII_R_HEX     0x52U
#define ASCII_U_HEX     0x55U
#define ASCII_D_HEX     0x44U

#define DEBUG_MODE      1

#define ELF2_NUM_KNOTS  10

typedef struct position {
    int16_t x;
    int16_t y;
} position_t;