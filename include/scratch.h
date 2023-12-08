#ifndef SCRATCH_H
#define SCRATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int scratch_alloc(); // allocate a free register
void scratch_free( int r ); // free a given register
const char* scratch_name( int r ); // get register name
int label_create();
const char * label_name( int l );
const char * string_label_name ( int l );
int scratch_alloc_float();
const char* scratch_name_float(int r);
void scratch_free_float(int r);

#endif