/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_TOKEN_H_INCLUDED
# define YY_YY_TOKEN_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_EOF = 258,
    TOKEN_INT_LIT = 259,
    TOKEN_CHAR_LIT = 260,
    TOKEN_FLOAT_LIT = 261,
    TOKEN_STR_LIT = 262,
    TOKEN_IDENT = 263,
    TOKEN_ARRAY = 264,
    TOKEN_AUTO = 265,
    TOKEN_BOOLEAN = 266,
    TOKEN_CHAR = 267,
    TOKEN_ELSE = 268,
    TOKEN_FALSE = 269,
    TOKEN_FOR = 270,
    TOKEN_FLOAT = 271,
    TOKEN_FUNCTION = 272,
    TOKEN_IF = 273,
    TOKEN_INT = 274,
    TOKEN_PRINT = 275,
    TOKEN_RETURN = 276,
    TOKEN_STRING = 277,
    TOKEN_TRUE = 278,
    TOKEN_VOID = 279,
    TOKEN_WHILE = 280,
    TOKEN_LPAREN = 281,
    TOKEN_RPAREN = 282,
    TOKEN_LBRACKET = 283,
    TOKEN_RBRACKET = 284,
    TOKEN_LBRACE = 285,
    TOKEN_RBRACE = 286,
    TOKEN_INCREMENT = 287,
    TOKEN_DECREMENT = 288,
    TOKEN_EXP = 289,
    TOKEN_MULT = 290,
    TOKEN_DIV = 291,
    TOKEN_MOD = 292,
    TOKEN_PLUS = 293,
    TOKEN_MINUS = 294,
    TOKEN_GTE = 295,
    TOKEN_LTE = 296,
    TOKEN_EQ = 297,
    TOKEN_NOT_EQ = 298,
    TOKEN_LT = 299,
    TOKEN_GT = 300,
    TOKEN_NOT = 301,
    TOKEN_ASSIGN = 302,
    TOKEN_COMMA = 303,
    TOKEN_COLON = 304,
    TOKEN_SEMICOL = 305,
    TOKEN_OR = 306,
    TOKEN_AND = 307,
    TOKEN_ERROR = 308
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 71 "parser.y" /* yacc.c:1909  */
    
    struct stmt *stmt;
    struct decl *decl;
    struct expr *expr;
    struct param_list *param_list;
    struct type *type;    

#line 116 "token.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TOKEN_H_INCLUDED  */
