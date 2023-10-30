#ifndef TYPE_H
#define TYPE_H

#include "param_list.h"
#include "expr.h"
#include <stdlib.h>
#include <string.h>

typedef enum {
	TYPE_VOID,
	TYPE_BOOLEAN,
	TYPE_CHARACTER,
	TYPE_INTEGER,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_FUNCTION,
	TYPE_FLOAT
} type_t;

struct type {
	type_t kind;
	struct param_list *params;
	struct type *subtype;
	struct expr *val;
};

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params, struct expr *val );
void          type_print( struct type *t );

#endif
