#ifndef SCRATCH_H
#define SCRATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int reg_table[] = {1,1,1,1,1,1,1};
static int NUM_REGS = 7;
static int curr_label = -1;

int scratch_alloc(); // allocate a free register
void scratch_free( int r ); // free a given register
const char* scratch_name( int r ); // get register name
int label_create();
const char * label_name( int l );
const char * string_label_name ( int l );

#endif