%{
#include "token.h"

void fixString(char*);
%}

DIGIT               [0-9]
LETTER              [a-zA-Z]
IDENT               [a-zA-Z_][a-zA-Z_0-9]{0,254}

%%

/*  comments & white spaces   */
\/\*(([^\*])|(\*+[^\/\*]))*\*+\/   {        }   // C++
\/\/[^n]*           {                       }   // C
( |\n|\t|\r)        {                       }

/*  keyworkds   */
array               { return TOKEN_ARRAY;   }
auto                { return TOKEN_AUTO;    }
boolean             { return TOKEN_BOOLEAN; }
char                { return TOKEN_CHAR;    }
else                { return TOKEN_ELSE;    }
false               { return TOKEN_FALSE;   }
for                 { return TOKEN_FOR;     }
function            { return TOKEN_FUNCTION;}
if                  { return TOKEN_IF;      }
integer             { return TOKEN_INT;     }
print               { return TOKEN_PRINT;   }
return              { return TOKEN_RETURN;  }
string              { return TOKEN_STRING;  }
true                { return TOKEN_TRUE;    }
void                { return TOKEN_VOID;    }
while               { return TOKEN_WHILE;   }

    /* Expression Symbols */
\(                  { return TOKEN_LPAREN; }
\)                  { return TOKEN_RPAREN; }
\[                  { return TOKEN_LBRACKET; }
\]                  { return TOKEN_RBRACKET; }
\{                  { return TOKEN_LBRACE; }
\}                  { return TOKEN_RBRACE; }
\+\+                { return TOKEN_INCREMENT; }
--                  { return TOKEN_DECREMENT; }
\^                  { return TOKEN_EXPONENT; }
\*                  { return TOKEN_MULT; }
\/                  { return TOKEN_DIV; }
%                   { return TOKEN_MOD; }
\+                  { return TOKEN_PLUS; }
-                   { return TOKEN_MINUS; }
>=                  { return TOKEN_GTE; }
\<=                 { return TOKEN_LTE; }
==                  { return TOKEN_EQ; }
!=                  { return TOKEN_NOT_EQ; }
>                   { return TOKEN_GT; }
\<                  { return TOKEN_LT; }
!                   { return TOKEN_NOT; }
=                   { return TOKEN_ASSIGN; }
,                   { return TOKEN_COMMA; }
:                   { return TOKEN_COLON; }
;                   { return TOKEN_SEMI; }
\|\|                { return TOKEN_OR; }
&&                  { return TOKEN_AND; }
\?                  { return TOKEN_QMARK; }



{ID}                { return TOKEN_ID; }

%%

void fixString(char * )



yywrap {return 0}
