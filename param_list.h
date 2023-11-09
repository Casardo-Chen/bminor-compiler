
#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include "type.h"
#include "scope.h"
#include "bminor_helper.h"
#include <stdio.h>
#include <string.h>

struct expr;

struct param_list {
	char *name;
	struct type *type;
	struct symbol *symbol;
	struct param_list *next;
};

extern int resolve_error;

struct param_list * param_list_create( const char *name, struct type *type, struct param_list *next );
void param_list_print( struct param_list *p );
void param_list_resolve( struct param_list *p );
int param_list_eq(struct param_list *a, struct param_list *b);

#endif
