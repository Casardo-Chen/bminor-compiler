#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "encoder.h"


extern FILE *yyin;
extern int yylex();
extern char *yytext;

int    encode();
int    scan();
int    parse();
int    token_to_string( token_t token, char * yytext);