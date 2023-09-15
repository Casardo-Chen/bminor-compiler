#include "token.h"
#include <stdio.h>


extern FILE *yyin;
extern int yylex();
extern char *yytext;

int    scan(FILE *input);
int    token_to_string( token_t token, char * yytext);