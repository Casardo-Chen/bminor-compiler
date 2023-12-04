#ifndef BMINOR_HELPER
#define BMINOR_HELPER

#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "stmt.h"
#include "type.h"
#include "symbol.h"
#include "scope.h"

#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "encoder.h"

extern int yylex();
extern char *yytext;
extern FILE *yyin;
extern struct decl * parser_result;
extern int resolve_error;
extern int type_error;
FILE * outfile;

typedef enum yytokentype token_t;

int    encode();
int    scan();
int    parse();
int    print();
int    resolve();
int    typecheck();
int    codegen();
int    token_to_string( token_t token, char * yytext);

#endif