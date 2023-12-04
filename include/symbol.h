
#ifndef SYMBOL_H
#define SYMBOL_H

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	struct type *type;
	char *name;
	int which;	// the ordinal position of local variables and parameters
	int proto;	// whether it is a prototype
	int modified; 	// if the kind has been modified
	int total; 	// count how many 

};

#include "type.h"
#include "scratch.h"

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name );
const char *  symbol_codegen(struct symbol *e);

#endif
