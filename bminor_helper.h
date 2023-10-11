#ifndef BMINOR_HELPER
#define BMINOR_HELPER

#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "stmt.h"
#include "type.h"
#include "symbol.h"

#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "encoder.h"

extern int yylex();
extern char *yytext;
extern FILE *yyin;

typedef enum yytokentype token_t;

int    encode();
int    scan();
int    parse();
int    token_to_string( token_t token, char * yytext);

#endif