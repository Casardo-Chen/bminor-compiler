%option noinput
%option nounput

%{
#include "token.h"
#include <stdbool.h>

extern bool string_decode(const char *es, char *s);
%}


IDENT               [a-zA-Z_][a-zA-Z0-9_]{0,254}
INT                 [+-]?[0-9]+
FLOAT               [+-]?[0-9]*\.?[0-9]+([eE][+-]?[0-9]+)?
CHAR                \'(\\[abefnrtv\'"0-9]|\\0x[0-9a-fA-F][0-9a-fA-F]|.)\'
STRING              \"([^\"\0\n\t]|(\\.))*\"

%%
    /*  keywords    */
array                               { return TOKEN_ARRAY;         }
auto                                { return TOKEN_AUTO;          }
boolean                             { return TOKEN_BOOLEAN;       }
char                                { return TOKEN_CHAR;          }
else                                { return TOKEN_ELSE;          }
false                               { return TOKEN_FALSE;         }
for                                 { return TOKEN_FOR;           }
function                            { return TOKEN_FUNCTION;      }
if                                  { return TOKEN_IF;            }
integer                             { return TOKEN_INT;           }
print                               { return TOKEN_PRINT;         }
return                              { return TOKEN_RETURN;        }
string                              { return TOKEN_STRING;        }
true                                { return TOKEN_TRUE;          }
void                                { return TOKEN_VOID;          }
while                               { return TOKEN_WHILE;         }

    /*  Expression Symbols  */
\(                               { return TOKEN_LPAREN;        }
\)                               { return TOKEN_RPAREN;        }
\[                               { return TOKEN_LBRACKET;      }
\]                               { return TOKEN_RBRACKET;      }
\{                               { return TOKEN_LBRACE;        }
\}                               { return TOKEN_RBRACE;        }
\+\+                             { return TOKEN_INCREMENT;     }
\-\-                             { return TOKEN_DECREMENT;     }
\^                               { return TOKEN_EXP;           }
\*                               { return TOKEN_MULT;          }
\/                               { return TOKEN_DIV;           }
%                                { return TOKEN_MOD;           }
\+                               { return TOKEN_PLUS;          }
-                                { return TOKEN_MINUS;         }
>=                               { return TOKEN_GTE;           }
\<=                              { return TOKEN_LTE;           }
==                               { return TOKEN_EQ;            }
!=                               { return TOKEN_NOT_EQ;        }
\<                               { return TOKEN_LT;            }
>                                { return TOKEN_GT;            }
!                                { return TOKEN_NOT;           }
=                                { return TOKEN_ASSIGN;        }
,                                { return TOKEN_COMMA;         }
:                                { return TOKEN_COLON;         }
;                                { return TOKEN_SEMICOL;       }
\|\|                             { return TOKEN_OR;            }
&&                               { return TOKEN_AND;           }

    /*  blank & comment    */
\/\*(([^\*])|(\*+[^\/\*]))*\*+\/     {                             ;}   
\/\/[^\n]*\n                         {                             ;}   
[ \n\t\r]+                           {                             ;}
    /*  literal & identifier    */
{INT}                               { return TOKEN_INT_LIT;            }
{FLOAT}                             { return TOKEN_FLOAT_LIT;          }
{CHAR}                              { return check_char(yytext);       }
{STRING}                            { return check_string(yytext);     }
{IDENT}                             { return check_ident(yytext);      }

.                                   { return TOKEN_ERROR;}

%%

int yywrap() {return 1;}

token_t check_ident(char *  yytext){
     if (strlen(yytext) >= 256){
        return TOKEN_ERROR;
    }
    return TOKEN_IDENT;

}

token_t check_char(char *  yytext) {
    char buffer[BUFSIZ];
    // change char to string and use decoder
    char *src = yytext;
    *src = '\"';
    // check for end
    while (src){
        if (*src=='\'' && *(src+1)=='\0'){
            *src='\"';
            break;
        }
        src++;
    }
    src = yytext;

    if (string_decode(src,buffer) ==0 ){
        return TOKEN_ERROR;
    }
    if (strlen(buffer) >= 256){
        return TOKEN_ERROR;
    }
    strcpy(yytext,buffer);
    return TOKEN_CHAR_LIT; 

}

token_t check_string(char *  yytext) {
    char buffer[BUFSIZ];
    if (string_decode(yytext,buffer) ==0 ){
        return TOKEN_ERROR;
    }
    if (strlen(buffer) >= 256){
        return TOKEN_ERROR;
    }
    strcpy(yytext,buffer);
    return TOKEN_STR_LIT;
}


