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

#ifndef YY_YY_GOLITE_TAB_H_INCLUDED
# define YY_YY_GOLITE_TAB_H_INCLUDED
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
    TOK_AND = 258,
    TOK_ANDEQ = 259,
    TOK_ANDNOT = 260,
    TOK_ANDNOTEQ = 261,
    TOK_APPEND = 262,
    TOK_ASSIGN = 263,
    TOK_BREAK = 264,
    TOK_CAP = 265,
    TOK_CASE = 266,
    TOK_CHAN = 267,
    TOK_CONST = 268,
    TOK_CONTINUE = 269,
    TOK_DEC = 270,
    TOK_DEFAULT = 271,
    TOK_DEFER = 272,
    TOK_DIVEQ = 273,
    TOK_DOTS = 274,
    TOK_ELSE = 275,
    TOK_EQ = 276,
    TOK_FALLTHROUGH = 277,
    TOK_FLOAT = 278,
    TOK_FOR = 279,
    TOK_FUNC = 280,
    TOK_GEQ = 281,
    TOK_GO = 282,
    TOK_GOTO = 283,
    TOK_GT = 284,
    TOK_IDENT = 285,
    TOK_IF = 286,
    TOK_INC = 287,
    TOK_INT = 288,
    TOK_INTERFACE = 289,
    TOK_LEN = 290,
    TOK_LEQ = 291,
    TOK_LT = 292,
    TOK_LSHIFT = 293,
    TOK_LSHIFTEQ = 294,
    TOK_MAP = 295,
    TOK_MINUSEQ = 296,
    TOK_NEQ = 297,
    TOK_OR = 298,
    TOK_OREQ = 299,
    TOK_PACKAGE = 300,
    TOK_PLUSEQ = 301,
    TOK_PRINT = 302,
    TOK_PRINTLN = 303,
    TOK_RANGE = 304,
    TOK_REMEQ = 305,
    TOK_RETURN = 306,
    TOK_RSHIFT = 307,
    TOK_RSHIFTEQ = 308,
    TOK_RUNE = 309,
    TOK_SELECT = 310,
    TOK_STR = 311,
    TOK_STRUCT = 312,
    TOK_SWITCH = 313,
    TOK_TIMESEQ = 314,
    TOK_TYPE = 315,
    TOK_VAR = 316,
    TOK_XOREQ = 317,
    UNARY = 318
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 24 "golite.y" /* yacc.c:1909  */

    int intval;
    double floatval;
    char *strval;
    char runeval;

#line 125 "golite.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_GOLITE_TAB_H_INCLUDED  */
