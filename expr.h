#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	/* ops */
	EXPR_ADD,
	EXPR_AND,
	EXPR_ASSIGN,
	EXPR_DECREMENT,
	EXPR_DIV,
	EXPR_EQ,
	EXPR_EXP,
	EXPR_GT,
	EXPR_GTE,
	EXPR_INCREMENT,
	EXPR_LT,
	EXPR_LTE,
	EXPR_MOD,
	EXPR_MUL,
	EXPR_NEG,
	EXPR_NOT,
	EXPR_NOT_EQ,
	EXPR_OR,
	EXPR_POS,
	EXPR_PAREN,
	EXPR_SUB,
	/* literals and identifier */
	EXPR_IDENT,
	EXPR_INT_LIT,
	EXPR_CHAR_LIT,
	EXPR_STR_LIT,
	EXPR_BOOL_LIT,
	EXPR_FLOAT_LIT,

	EXPR_ARG,
	EXPR_SUBT,
	EXPR_CALL,
	EXPR_BR,
	EXPR_END
	/* many more kinds of exprs to add here */
} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	const char *name;
	int literal_value;
	const char * string_literal;
	struct symbol *symbol;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );

struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_float_literal( const char *f );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_char_literal( char c );
struct expr * expr_create_string_literal( const char *str );

void expr_print( struct expr *e );
void print_op( struct expr *e, const char *op);

#endif
