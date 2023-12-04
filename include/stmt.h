#ifndef STMT_H
#define STMT_H

#include "decl.h"
#include "expr.h"
#include "scratch.h"
#include <stdlib.h>
#include <string.h>

typedef enum {
	STMT_DECL,
	STMT_EXPR,
	STMT_IF_ELSE,
	STMT_FOR,
	STMT_PRINT,
	STMT_RETURN,
	STMT_BR
} stmt_t;

struct stmt {
	stmt_t kind;
	struct decl *decl;
	struct expr *init_expr;
	struct expr *expr;
	struct expr *next_expr;
	struct stmt *body;
	struct stmt *else_body;
	struct stmt *next;
	int	pre_indent;
	int post_newline;
};

extern int resolve_error;
extern int type_error;
extern FILE* outfile;

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next );

struct stmt * stmt_wrap( struct stmt *s );
void stmt_print( struct stmt *s, int indent );

void stmt_resolve( struct stmt *s );
void stmt_typecheck( struct stmt *s, struct decl *d );

void stmt_codegen(struct stmt *s, const char *function_name);

#endif
