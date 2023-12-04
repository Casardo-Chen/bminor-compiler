
#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "scratch.h"
#include "param_list.h"
#include <stdio.h>
#include <string.h>

struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
};

extern int resolve_error;
extern int type_error;
extern FILE* outfile;
extern char* arg_reg[6];

struct decl * decl_create( const char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next );

void decl_print( struct decl *d, int indent );
void indent_print(int indent);

void decl_resolve( struct decl *d );
void decl_typecheck( struct decl *d );

void decl_codegen_global(struct decl *d); 
void decl_codegen_local(struct decl *d);

#endif


