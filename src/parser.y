%{
#include <stdio.h>
#include <stdlib.h>

#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "stmt.h"
#include "type.h"
#include "symbol.h"

extern char *yytext;
extern int yylex();
extern int yyerror( char *str );
struct decl * parser_result = 0;
%}

%token TOKEN_EOF   

/* Expression Symbols */  
%token TOKEN_INT_LIT    
%token TOKEN_CHAR_LIT          
%token TOKEN_FLOAT_LIT         
%token TOKEN_STR_LIT           
%token TOKEN_IDENT                

/* Keywords */
%token TOKEN_ARRAY             
%token TOKEN_AUTO              
%token TOKEN_BOOLEAN           
%token TOKEN_CHAR              
%token TOKEN_ELSE              
%token TOKEN_FALSE             
%token TOKEN_FOR  
%token TOKEN_FLOAT              
%token TOKEN_FUNCTION         
%token TOKEN_IF                
%token TOKEN_INT               
%token TOKEN_PRINT             
%token TOKEN_RETURN            
%token TOKEN_STRING            
%token TOKEN_TRUE              
%token TOKEN_VOID              
%token TOKEN_WHILE                       

/* Expression Symbols */
%token TOKEN_LPAREN            
%token TOKEN_RPAREN            
%token TOKEN_LBRACKET          
%token TOKEN_RBRACKET          
%token TOKEN_LBRACE            
%token TOKEN_RBRACE            
%token TOKEN_INCREMENT         
%token TOKEN_DECREMENT         
%token TOKEN_EXP               
%token TOKEN_MULT              
%token TOKEN_DIV               
%token TOKEN_MOD               
%token TOKEN_PLUS              
%token TOKEN_MINUS             
%token TOKEN_GTE               
%token TOKEN_LTE               
%token TOKEN_EQ                
%token TOKEN_NOT_EQ            
%token TOKEN_LT                
%token TOKEN_GT                
%token TOKEN_NOT               
%token TOKEN_ASSIGN            
%token TOKEN_COMMA             
%token TOKEN_COLON             
%token TOKEN_SEMICOL           
%token TOKEN_OR                
%token TOKEN_AND               
%token TOKEN_ERROR  

%union {    
    struct stmt *stmt;
    struct decl *decl;
    struct expr *expr;
    struct param_list *param_list;
    struct type *type;    
};

%type <expr> atomic literal id expr expr_0 expr_1 expr_2 expr_3 expr_4 expr_5 expr_6 expr_7 expr_8 expr_opt expr_opt_list expr_list expr_br
%type <decl> program decl decl_s
%type <stmt> stmt stmt_s stmt_br stmt_1 stmt_2 stmt_t
%type <type> type
%type <param_list> param_opt_list param_list

%start program

/* for debugging purpose */
/* %error-verbose */

%%    
program : decl_s                                { parser_result = $1;}
        | /*e*/                                 { $$ = 0; }
        ;

/* EXPRESSION */
expr    : expr_8                                { $$ = $1; }
        ;

expr_opt  : expr                                { $$ = $1; }
          | /*e*/                               { $$ = 0; }
          ;

expr_list : expr TOKEN_COMMA expr_list          { $$ = expr_create(EXPR_ARG, $1, $3,8); }
          | expr                                { $$ = expr_create(EXPR_ARG, $1, 0, 8); }
          ;

expr_opt_list   : expr_list                     { $$ = $1; }
                | /*e*/                         { $$ = 0; }          
                ;

expr_br : TOKEN_LBRACE expr_br TOKEN_RBRACE                               { $$ = expr_create(EXPR_BR, $2, 0, 8); }
        | TOKEN_LBRACE expr_br TOKEN_RBRACE TOKEN_COMMA expr_br           { $$ = expr_create(EXPR_BR, $2, $5,8); }
        | expr_list                                                       { $$ = $1;}
        ;

expr_8  : expr_7 TOKEN_ASSIGN expr_8            { $$ = expr_create(EXPR_ASSIGN, $1, $3,0); }
        | expr_7                                { $$ = $1; }
        ;

expr_7  : expr_7 TOKEN_OR expr_6                { $$ = expr_create(EXPR_OR, $1, $3,1); }
        | expr_6                                { $$ = $1; }
        ;

expr_6  : expr_6 TOKEN_AND expr_5               { $$ = expr_create(EXPR_AND, $1, $3,2);}
        | expr_5                                { $$ = $1; }
        ;

expr_5  : expr_5 TOKEN_LT expr_4                { $$ = expr_create(EXPR_LT, $1, $3,3); }
        | expr_5 TOKEN_LTE expr_4               { $$ = expr_create(EXPR_LTE, $1, $3,3); }
        | expr_5 TOKEN_GT expr_4                { $$ = expr_create(EXPR_GT, $1, $3,3); }
        | expr_5 TOKEN_GTE expr_4               { $$ = expr_create(EXPR_GTE, $1, $3,3);}
        | expr_5 TOKEN_EQ expr_4                { $$ = expr_create(EXPR_EQ, $1, $3,3); }
        | expr_5 TOKEN_NOT_EQ expr_4            { $$ = expr_create(EXPR_NOT_EQ, $1, $3,3); }
        | expr_4                                { $$ = $1; }
        ;

expr_4  : expr_4 TOKEN_PLUS expr_3              { $$ = expr_create(EXPR_ADD,$1,$3,4); }
        | expr_4 TOKEN_MINUS expr_3             { $$ = expr_create(EXPR_SUB,$1,$3,4); }
        | expr_3                                { $$ = $1; }
        ;

expr_3  : expr_3 TOKEN_MULT expr_2              { $$ = expr_create(EXPR_MUL,$1,$3,5); }
        | expr_3 TOKEN_DIV expr_2               { $$ = expr_create(EXPR_DIV,$1,$3,5); }
        | expr_3 TOKEN_MOD expr_2               { $$ = expr_create(EXPR_MOD,$1,$3,5); }
        | expr_2                                { $$ = $1; }
        ;

expr_2  : expr_2 TOKEN_EXP expr_1               { $$ = expr_create(EXPR_EXP,$1,$3,6);}
        | expr_1                                { $$ = $1; }
        ;

expr_1  : TOKEN_NOT expr_0                      { $$ = expr_create(EXPR_NOT,$2,0,7);}
        | TOKEN_PLUS expr_0                     { $$ = expr_create(EXPR_POS,$2,0,7);}
        | TOKEN_MINUS expr_0                    { $$ = expr_create(EXPR_NEG,$2,0,7);}
        | expr_0                                { $$ = $1; }
        ;

expr_0  : expr_0 TOKEN_INCREMENT                        { $$ = expr_create(EXPR_INCREMENT,$1,0,8); }
        | expr_0 TOKEN_DECREMENT                        { $$ = expr_create(EXPR_DECREMENT,$1,0,8); }
        | TOKEN_LPAREN expr TOKEN_RPAREN                { $$ = $2;}        // (a)
        | expr_0 TOKEN_LBRACKET expr TOKEN_RBRACKET     { $$ = expr_create(EXPR_SUBT,$1,$3,8);}        // a[b]
        | id TOKEN_LPAREN expr_opt_list TOKEN_RPAREN    { $$ = expr_create(EXPR_CALL,$1,$3,8);}        // f() | f(a,b)
        | atomic                                        { $$ = $1; }
        ;       

atomic  : id                                    { $$ = $1; }
        | literal                               { $$ = $1; }
        ;

literal : TOKEN_INT_LIT                         { $$ = expr_create_integer_literal(atoi(yytext)); }
        | TOKEN_STR_LIT                         { $$ = expr_create_string_literal( yytext); }
        | TOKEN_CHAR_LIT                        { $$ = expr_create_char_literal( *yytext);}
        | TOKEN_FLOAT_LIT                       { $$ = expr_create_float_literal( yytext);}
        | TOKEN_TRUE                            { $$ = expr_create_boolean_literal( 1 ); }
        | TOKEN_FALSE                           { $$ = expr_create_boolean_literal( 0 ); }
        ;

id      : TOKEN_IDENT                           { $$ = expr_create_name( yytext );}
        ;

/* DECLARATION */
decl_s  : decl decl_s                           { $$ = $1; $1->next = $2;}
        | decl                                  { $$ = $1; }
        ;

decl    : id TOKEN_COLON type TOKEN_ASSIGN expr_br TOKEN_SEMICOL        { $$ = decl_create($1->name, $3, $5, 0, 0); }
        | id TOKEN_COLON type TOKEN_ASSIGN stmt_br                      { $$ = decl_create($1->name, $3, 0, $5, 0); }
        | id TOKEN_COLON type TOKEN_SEMICOL                             { $$ = decl_create($1->name, $3, 0, 0, 0); }
        ;

/* STATEMENT */
stmt_s  : stmt stmt_s                           { $$ = $1; $1->next = $2;}
        | /*e*/                                 { $$ = 0; }
        ;

stmt    : stmt_1                                { $$ = $1; }
        | stmt_2                                { $$ = $1; } 
        ;

stmt_1  : TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt                                                          { $$ = stmt_create( STMT_IF_ELSE, 0, 0, $3, 0, $5, 0, 0 );}                                   
        | TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt_2 TOKEN_ELSE stmt_1                                      { $$ = stmt_create( STMT_IF_ELSE, 0, 0, $3, 0, $5, $7, 0 );}
        | TOKEN_FOR TOKEN_LPAREN expr_opt TOKEN_SEMICOL expr_opt TOKEN_SEMICOL expr_opt TOKEN_RPAREN stmt_1     { $$ = stmt_create( STMT_FOR, 0, $3, $5, $7, $9, 0, 0 );}
        ;

stmt_2  : TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt_2 TOKEN_ELSE stmt_2                                      { $$ = stmt_create( STMT_IF_ELSE, 0, 0, $3, 0, $5, $7, 0 );}
        | TOKEN_FOR TOKEN_LPAREN expr_opt TOKEN_SEMICOL expr_opt TOKEN_SEMICOL expr_opt TOKEN_RPAREN stmt_2     { $$ = stmt_create( STMT_FOR, 0, $3, $5, $7, $9, 0, 0 );}
        | stmt_t                                                                                                { $$ = $1; }
        ;

stmt_t  : decl                                          { $$ = stmt_create( STMT_DECL, $1, 0, 0, 0, 0, 0, 0 ); }
        | stmt_br                                       { $$ = $1; }
        | expr TOKEN_SEMICOL                            { $$ = stmt_create( STMT_EXPR, 0, 0, $1, 0, 0, 0, 0 ); }          
        | TOKEN_RETURN expr_opt TOKEN_SEMICOL           { $$ = stmt_create( STMT_RETURN, 0, 0, $2, 0, 0, 0, 0 ); }  
        | TOKEN_PRINT expr_opt_list TOKEN_SEMICOL       { $$ = stmt_create( STMT_PRINT, 0, 0, $2, 0, 0, 0, 0 ); }                                    
        ;

stmt_br : TOKEN_LBRACE stmt_s TOKEN_RBRACE              { $$ = stmt_create( STMT_BR, 0, 0, 0, 0, $2, 0, 0 ); }
        ;


/* TYPE */
type    : TOKEN_INT                                                     { $$ = type_create(TYPE_INTEGER, 0, 0, 0);}
        | TOKEN_FLOAT                                                   { $$ = type_create(TYPE_FLOAT, 0, 0, 0);}
        | TOKEN_CHAR                                                    { $$ = type_create(TYPE_CHARACTER, 0, 0, 0);}
        | TOKEN_STRING                                                  { $$ = type_create(TYPE_STRING, 0, 0, 0);} 
        | TOKEN_VOID                                                    { $$ = type_create(TYPE_VOID, 0, 0, 0);}
        | TOKEN_BOOLEAN                                                 { $$ = type_create(TYPE_BOOLEAN, 0, 0, 0);}
        | TOKEN_ARRAY TOKEN_LBRACKET expr_opt_list TOKEN_RBRACKET type  { $$ = type_create(TYPE_ARRAY, $5, 0, $3);}
        | TOKEN_FUNCTION type TOKEN_LPAREN param_opt_list TOKEN_RPAREN  { $$ = type_create(TYPE_FUNCTION, $2, $4, 0);}
        ;

/* PARAMETER */
param_opt_list  : param_list                                    { $$ = $1; }
                | /* e */                                       { $$ = 0; }
                ;       

param_list      : id TOKEN_COLON type TOKEN_COMMA param_list    {$$ = param_list_create($1->name, $3, $5); }
                | id TOKEN_COLON type                           {$$ = param_list_create($1->name, $3, 0); }
                ;

%%
int yyerror( char *str ){
        printf("parse error: %s\n", str);
        return 0;
}


