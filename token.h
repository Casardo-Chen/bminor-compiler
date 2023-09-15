#ifndef TOKEN_H
#define TOKEN_H

typedef enum{
    TOKEN_EOF               ,
    /* Expression Symbols */
    TOKEN_INT_LIT           ,
    TOKEN_FLOAT_LIT         ,
    TOKEN_CHAR_LIT          ,
    TOKEN_STR_LIT           ,
    TOKEN_IDENT             ,

/* Keywords */
    TOKEN_ARRAY             ,
    TOKEN_AUTO              ,
    TOKEN_BOOLEAN           ,
    TOKEN_CHAR              ,
    TOKEN_ELSE              ,
    TOKEN_FALSE             ,
    TOKEN_FOR               ,
    TOKEN_FUNCTION          ,
    TOKEN_IF                ,
    TOKEN_INT               ,
    TOKEN_PRINT             ,
    TOKEN_RETURN            ,
    TOKEN_STRING            ,
    TOKEN_TRUE              ,
    TOKEN_VOID              ,
    TOKEN_WHILE             ,
/* Expression Symbols */
    TOKEN_LPAREN            ,
    TOKEN_RPAREN            ,
    TOKEN_LBRACKET          ,
    TOKEN_RBRACKET          ,
    TOKEN_LBRACE            ,
    TOKEN_RBRACE            ,
    TOKEN_INCREMENT         ,
    TOKEN_DECREMENT         ,
    TOKEN_EXP               ,
    TOKEN_MULT              ,
    TOKEN_DIV               ,
    TOKEN_MOD               ,
    TOKEN_PLUS              ,
    TOKEN_MINUS             ,
    TOKEN_GTE               ,
    TOKEN_LTE               ,
    TOKEN_EQ                ,
    TOKEN_NOT_EQ            ,
    TOKEN_LT                ,
    TOKEN_GT                ,
    TOKEN_NOT               ,
    TOKEN_ASSIGN            ,
    TOKEN_COMMA             ,
    TOKEN_COLON             ,
    TOKEN_SEMICOL           ,
    TOKEN_OR                ,
    TOKEN_AND               ,
    TOKEN_ERROR             ,
} token_t;

int yywrap();
token_t check_string(char *yytext);
token_t check_char(char *yytext);
token_t check_ident(char *yytext);


#endif