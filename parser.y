%{
#include <stdio.h>
#include <stdlib.h>

extern char *yytext;
extern int yylex();
extern int yyerror( char *str );
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

%type <expr> atomic literal id expr expr_0 expr_1 expr_2 expr_3 expr_4 expr_5 expr_6 expr_7 expr_8 expr_opt_list expr_list
%type <decl> program decl decl_s
%type <stmt> stmt stmt_s stmt_br stmt_1 stmt_2
%type <type> type
%type <param_list> param_opt_list param_list

%start program

/* for debugging purpose */
/* %error-verbose */

%%    
program : decl_s                                {}
        ;

/* EXPRESSION */
expr    : expr_8                                {}
        ;

expr_opt  : expr                                {}
          | /*e*/                               {}
          ;

expr_list : expr TOKEN_COMMA expr_list          {}
          | expr                                {}
          ;

expr_opt_list   : expr_list                     {}
                | /*e*/                         {}          
                ;

expr_br : TOKEN_LBRACE expr_br TOKEN_RBRACE                               {}
        | TOKEN_LBRACE expr_br TOKEN_RBRACE TOKEN_COMMA expr_br           {}
        | expr_list
        ;

expr_8  : expr_7 TOKEN_ASSIGN expr_8            {}
        | expr_7                                {}
        ;

expr_7  : expr_7 TOKEN_OR expr_6                {}
        | expr_6                                {}
        ;

expr_6  : expr_6 TOKEN_AND expr_5               {}
        | expr_5                                {}
        ;

expr_5  : expr_5 TOKEN_LT expr_4                {}
        | expr_5 TOKEN_LTE expr_4               {}
        | expr_5 TOKEN_GT expr_4                {}
        | expr_5 TOKEN_GTE expr_4               {}
        | expr_5 TOKEN_EQ expr_4                {}
        | expr_5 TOKEN_NOT_EQ expr_4            {}
        | expr_4                                {}
        ;

expr_4  : expr_4 TOKEN_PLUS expr_3              {}
        | expr_4 TOKEN_MINUS expr_3             {}
        | expr_3                                {}
        ;

expr_3  : expr_3 TOKEN_MULT expr_2              {}
        | expr_3 TOKEN_DIV expr_2               {}
        | expr_3 TOKEN_MOD expr_2               {}
        | expr_2                                {}
        ;

expr_2  : expr_2 TOKEN_EXP expr_1               {}
        | expr_1                                {}
        ;

expr_1  : TOKEN_NOT expr_0                      {}
        | TOKEN_PLUS expr_0                     {}
        | TOKEN_MINUS expr_0                    {}
        | expr_0                                {}
        ;

expr_0  : expr_0 TOKEN_INCREMENT                        {}
        | expr_0 TOKEN_DECREMENT                        {}
        | TOKEN_LPAREN expr TOKEN_RPAREN                {}                     // (a)
        | expr_0 TOKEN_LBRACKET expr TOKEN_RBRACKET     {}                     // a[b]
        | id TOKEN_LPAREN expr_opt_list TOKEN_RPAREN    {}                     // f() | f(a,b)
        | atomic                                        {}
        ;       

atomic  : id                                    {}
        | literal                               {}
        ;

literal : TOKEN_INT_LIT                         {}
        | TOKEN_STR_LIT                         {}
        | TOKEN_CHAR_LIT                        {}
        | TOKEN_FLOAT_LIT                       {}
        | TOKEN_TRUE                            {}
        | TOKEN_FALSE                           {}
        ;

id      : TOKEN_IDENT                           {}
        ;

/* DECLARATION */
decl_s  : decl decl_s                           {}
        | /*e*/                                 {return 0;}             // empty file
        ;

decl    : id TOKEN_COLON type TOKEN_ASSIGN expr_br TOKEN_SEMICOL        {}
        | id TOKEN_COLON type TOKEN_ASSIGN stmt_br                      {}
        | id TOKEN_COLON type TOKEN_SEMICOL                             {}
        ;

/* STATEMENT */
stmt_s  : stmt stmt_s                           {}
        | /*e*/                                 {}
        ;

stmt    : stmt_1                                {}
        | stmt_2                                {} 
        ;

stmt_1  : TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt                                                          {}                                   
        | TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt_2 TOKEN_ELSE stmt_1                                      {}
        | TOKEN_FOR TOKEN_LPAREN expr_opt TOKEN_SEMICOL expr_opt TOKEN_SEMICOL expr_opt TOKEN_RPAREN stmt_1     {}
        ;

stmt_2  : TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt_2 TOKEN_ELSE stmt_2                                      {}
        | TOKEN_FOR TOKEN_LPAREN expr_opt TOKEN_SEMICOL expr_opt TOKEN_SEMICOL expr_opt TOKEN_RPAREN stmt_2     {}
        | stmt_t                                                                                                {}
        ;

stmt_t  : decl                                          {}
        | stmt_br                                       {}
        | expr TOKEN_SEMICOL                            {}          
        | TOKEN_RETURN expr_opt TOKEN_SEMICOL           {}  
        | TOKEN_PRINT expr_opt_list TOKEN_SEMICOL       {}                                    
        ;

stmt_br : TOKEN_LBRACE stmt_s TOKEN_RBRACE              {}
        ;  


/* TYPE */
type    : TOKEN_INT                                                     {}
        | TOKEN_FLOAT                                                   {}
        | TOKEN_CHAR                                                    {}
        | TOKEN_STRING                                                  {} 
        | TOKEN_VOID                                                    {}
        | TOKEN_BOOLEAN                                                 {}
        | TOKEN_AUTO                                                    {}
        | TOKEN_ARRAY TOKEN_LBRACKET expr_opt_list TOKEN_RBRACKET type  {}
        | TOKEN_FUNCTION type TOKEN_LPAREN param_opt_list TOKEN_RPAREN  {}
        ;

/* PARAMETER */
param_opt_list  : param_list                                    {}
                |                                               {}
                ;       

param_list      : id TOKEN_COLON type  TOKEN_COMMA param_list   {}
                | id TOKEN_COLON type                           {}
                ;

%%
int yyerror( char *str ){
        printf("parse error: %s\n", str);
        return 0;
}


