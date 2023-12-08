#include "scratch.h"

static int reg_table[] = {1,1,1,1,1,1,1};
static int NUM_REGS = 7;
static int curr_label = -1;

/* float */
static int fp_reg_table[] = {1,1,1,1,1,1,1,1};
static int NUM_FP_REGS = 8;

/* allocate a reg */ 
int scratch_alloc(){
    for (int i = 0; i < NUM_REGS; i++) {
        if (reg_table[i] == 1) {
            reg_table[i] = 0; // take up the space
            return i;
        }
    }
    printf("codegen error: no free registers left.\n"); // if there are no free registers, raise an error
    exit(1);
    return -1;
}

/* free a register */ 
void scratch_free(int r) {
    if (r < 0 || r > NUM_REGS) {
        printf("codegen error: attempted to free a register outside of known registers.\n");
        exit(1);
    }
    reg_table[r] = 1;
}

/* get the name of a register */ 
const char* scratch_name(int r) {
    if (r < 0 || r > NUM_REGS) {
        printf("codegen error: unknown register name.\n");
        exit(1);
    } else if (r == 0) {
        return "rbx";
    } else {
        char str[4];
        sprintf(str, "r1%d", r-1);
        return strdup(str);
    }
}

int label_create() {
    return ++curr_label;
}

const char* label_name(int l) {
    char str[BUFSIZ];
    sprintf(str, ".L%d", l);
    return strdup(str);
}

const char * string_label_name (int l) {
    char str[BUFSIZ];
    sprintf(str, ".SL%d", l);
    return strdup(str);
}

/* supporting float */
int scratch_alloc_float(){
    for (int i = 0; i < NUM_FP_REGS; i++) {
        if (fp_reg_table[i] == 1) {
            fp_reg_table[i] = 0;
            return i;
        }
    }
    fprintf(stderr,"codegen error: no free floating-point registers left.\n"); 
    exit(1);
    return -1;
}

const char* scratch_name_float(int r) {
    if (r < 0 || r > NUM_FP_REGS) {
        printf("codegen error: unknown floating-point register name.\n");
        exit(1);  
    } else {
        char str[BUFSIZ];
        sprintf(str, "xmm%d", r);
        return strdup(str);
    }
}

void scratch_free_float(int r) {
    if (r < 0 || r > NUM_FP_REGS) {
        printf("codegen error: attempted to free a floating-point register outside of known registers.\n");
        exit(1);  
    } else {
        fp_reg_table[r] = 1;
    } 
}
