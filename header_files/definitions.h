#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

/*define a linked list*/


enum {ADD , CHANGE};

enum {INSTRUCTIONAL, COMMAND};

/*For classify the label */
enum {DATA,  EXTERN, ENTRY, CODE, MACRO};

typedef enum {OFF , ON} flag; /* on and off flag*/

/*this is a list of all errors*/

#define UNINITIALIZED -1

/*definitions in helper functions*/
#define LAST_COMMAND 16


#define MAX_LABEL_LENGTH 31
#define MAX_LINE_LENGTH 81
#define COMMAND_TABLE {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"}
#define REG_TABLE {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"}
#define MAX_MACRO_LINES 10
#define MAX_NUMBER 16383  /* 2^14 - 1 */
#define MIN_NUMBER -16384   /* -2^14 */
#define MIN_NUMBER_12BIT -2048
#define MAX_NUMBER_12BIT 2047
#define COMMAND_OPTIONS_TABLE {\
    /* mov */ {0, 0, 1}, {0, 0, 2}, {0, 0, 3}, {0, 1, 1}, {0, 1, 2}, {0, 1, 3}, {0, 2, 1}, {0, 2, 2}, {0, 2, 3}, {0, 3, 1}, {0, 3, 2}, {0, 3, 3}, \
    /* cmp */ {1, 0, 0}, {1, 0, 1}, {1, 0, 2}, {1, 0, 3}, {1, 1, 0}, {1, 1, 1}, {1, 1, 2}, {1, 1, 3}, {1, 2, 0}, {1, 2, 1}, {1, 2, 2}, {1, 2, 3}, {1, 3, 0}, {1, 3, 1}, {1, 3, 2}, {1, 3, 3}, \
    /* add */ {2, 0, 1}, {2, 0, 2}, {2, 0, 3}, {2, 1, 1}, {2, 1, 2}, {2, 1, 3}, {2, 2, 1}, {2, 2, 2}, {2, 2, 3}, {2, 3, 1}, {2, 3, 2}, {2, 3, 3}, \
    /* sub */ {3, 0, 1}, {3, 0, 2}, {3, 0, 3}, {3, 1, 1}, {3, 1, 2}, {3, 1, 3}, {3, 2, 1}, {3, 2, 2}, {3, 2, 3}, {3, 3, 1}, {3, 3, 2}, {3, 3, 3}, \
    /* lea */ {4, 1, 1}, {4, 1, 2}, {4, 1, 3}, \
    /* clr */ {5, UNINITIALIZED, 1}, {5, UNINITIALIZED, 2}, {5, UNINITIALIZED, 3}, \
    /* not */ {6, UNINITIALIZED, 1}, {6, UNINITIALIZED, 2}, {6, UNINITIALIZED, 3}, \
    /* inc */ {7, UNINITIALIZED, 1}, {7, UNINITIALIZED, 2}, {7, UNINITIALIZED, 3}, \
    /* dec */ {8, UNINITIALIZED, 1}, {8, UNINITIALIZED, 2}, {8, UNINITIALIZED, 3}, \
    /* jmp */ {9, UNINITIALIZED, 1}, {9, UNINITIALIZED, 2}, \
    /* bne */ {10, UNINITIALIZED, 1}, {10, UNINITIALIZED, 2}, \
    /* red */ {11, UNINITIALIZED, 1}, {11, UNINITIALIZED, 2}, {11, UNINITIALIZED, 3}, \
    /* prn */ {12, UNINITIALIZED, 0}, {12, UNINITIALIZED, 1}, {12, UNINITIALIZED, 2}, {12, UNINITIALIZED, 3}, \
    /* jsr */ {13, UNINITIALIZED, 1}, {13, UNINITIALIZED, 2}, \
    /* rts */ {14, UNINITIALIZED, UNINITIALIZED}, \
    /* stop */ {15, UNINITIALIZED, UNINITIALIZED} \
}


enum {IMMEDIATE_ADDRESING, DIRECT_ADDRESING, INDIRECT_REGISTER_ADDRESING, DIRECT_REGISTER_ADDRESING};

/* -------------------------------------------------------------------------------------------- */
#define SUM_COMMANDS 16
#define SUM_REGISTERS 8

#define SHIFT_UPCODE 11	
#define SHIFT_NUM 3
#define SHIFT_TARGET_LOCATION 3
#define SHIFT_SOURCE_LOCATION 7

#define TURN_ON_SRC_IMMEDIATE 7
#define TURN_ON_SRC_DIRECT 8
#define TURN_ON_SRC_INDIRECT_REG 9
#define TURN_ON_SRC_DIRECT_REG 10

#define TURN_ON_TRGT_IMMEDIATE 3
#define TURN_ON_TRGT_DIRECT 4
#define TURN_ON_TRGT_INDIRECT_REG 5
#define TURN_ON_TRGT_DIRECT_REG 6



#define TURN_ON_A 2
#define GETS_TWO_OPERANDS 4
#define NO_OPERANDS 14
#define AT_LEAST_ONE_OPERAND 13
enum{r0, r1, r2, r3, r4, r5, r6, r7};	
enum{mov, cmp, add, sub, lea, clr, nt, inc, dec, jmp, bne, red, prn, jsr, rts, stop };
enum{IMMEDIATE, LABEL, INDIR_REG, DIR_REG};
