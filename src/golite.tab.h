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
    TOK_ASSIGN = 262,
    TOK_BREAK = 263,
    TOK_CASE = 264,
    TOK_CHAN = 265,
    TOK_CONST = 266,
    TOK_CONTINUE = 267,
    TOK_DEC = 268,
    TOK_DEFAULT = 269,
    TOK_DEFER = 270,
    TOK_DIVEQ = 271,
    TOK_DOTS = 272,
    TOK_ELSE = 273,
    TOK_EQ = 274,
    TOK_FALLTHROUGH = 275,
    TOK_FLOAT = 276,
    TOK_FOR = 277,
    TOK_FUNC = 278,
    TOK_GEQ = 279,
    TOK_GO = 280,
    TOK_GOTO = 281,
    TOK_GT = 282,
    TOK_IDENT = 283,
    TOK_IF = 284,
    TOK_INC = 285,
    TOK_INT = 286,
    TOK_INTERFACE = 287,
    TOK_LEQ = 288,
    TOK_LT = 289,
    TOK_LSHIFT = 290,
    TOK_LSHIFTEQ = 291,
    TOK_MAP = 292,
    TOK_MINUSEQ = 293,
    TOK_NEQ = 294,
    TOK_OR = 295,
    TOK_OREQ = 296,
    TOK_PACKAGE = 297,
    TOK_PLUSEQ = 298,
    TOK_RANGE = 299,
    TOK_REMEQ = 300,
    TOK_RETURN = 301,
    TOK_RSHIFT = 302,
    TOK_RSHIFTEQ = 303,
    TOK_RUNE = 304,
    TOK_SELECT = 305,
    TOK_STR = 306,
    TOK_STRUCT = 307,
    TOK_SWITCH = 308,
    TOK_TIMESEQ = 309,
    TOK_TYPE = 310,
    TOK_VAR = 311,
    TOK_XOREQ = 312
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 18 "golite.y" /* yacc.c:1909  */

    int intval;
    double floatval;
    char *strval;
    char runeval;

#line 119 "golite.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GOLITE_TAB_H_INCLUDED  */
