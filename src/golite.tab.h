/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Skeleton interface for Bison GLR parsers in C

   Copyright (C) 2002-2015 Free Software Foundation, Inc.

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
/* "%code requires" blocks.  */
#line 24 "golite.y" /* glr.c:197  */


#include "tree.h"


#line 50 "golite.tab.h" /* glr.c:197  */

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
    TOK_CONTINUE = 267,
    TOK_DEC = 268,
    TOK_DEFAULT = 269,
    TOK_DIVEQ = 270,
    TOK_ELSE = 271,
    TOK_EQ = 272,
    TOK_FALLTHROUGH = 273,
    TOK_FLOAT = 274,
    TOK_FOR = 275,
    TOK_FUNC = 276,
    TOK_GEQ = 277,
    TOK_IDENT = 278,
    TOK_IF = 279,
    TOK_INC = 280,
    TOK_INT = 281,
    TOK_LEN = 282,
    TOK_LEQ = 283,
    TOK_LSHIFT = 284,
    TOK_LSHIFTEQ = 285,
    TOK_MINUSEQ = 286,
    TOK_NEQ = 287,
    TOK_OR = 288,
    TOK_OREQ = 289,
    TOK_PACKAGE = 290,
    TOK_PLUSEQ = 291,
    TOK_PRINT = 292,
    TOK_PRINTLN = 293,
    TOK_REMEQ = 294,
    TOK_RETURN = 295,
    TOK_RSHIFT = 296,
    TOK_RSHIFTEQ = 297,
    TOK_RUNE = 298,
    TOK_STR = 299,
    TOK_STRUCT = 300,
    TOK_SWITCH = 301,
    TOK_TIMESEQ = 302,
    TOK_TYPE = 303,
    TOK_VAR = 304,
    TOK_XOREQ = 305,
    UNARY = 306
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 35 "golite.y" /* glr.c:197  */

    int intval;
    double floatval;
    char *strval;
    char runeval;
    struct tree_ident *ident;
    struct tree_idents *idents;
    struct tree_exps *exps;
    struct tree_decls *decls;
    struct tree_var_spec *var_decl;
    struct tree_stmts *stmts;
    struct tree_params *params;
    struct tree_fields *fields;
    struct tree_type *type;
    struct tree_stmt *stmt;
    struct tree_exp *exp;
    struct tree_if ifstmt;
    struct tree_assign assign;
    struct tree_assignop assignop;
    struct tree_switch switchstmt;
    struct tree_cases *cases;

#line 137 "golite.tab.h" /* glr.c:197  */
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
