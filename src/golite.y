%{

#include <stdlib.h>
#include <stdio.h>

int yylex();
extern int yylineno;

void yyerror(char const *s)
{
    fprintf(stderr, "Error: syntax on line %d: %s\n", yylineno, s);
    exit(1);
}

%}

%union
{
    int intval;
    double floatval;
    char *strval;
    char runeval;
}

%token                  TOK_AND
%token                  TOK_ANDEQ
%token                  TOK_ANDNOT
%token                  TOK_ANDNOTEQ
%token                  TOK_ASSIGN
%token                  TOK_BREAK
%token                  TOK_CASE
%token                  TOK_CHAN
%token                  TOK_CONST
%token                  TOK_CONTINUE
%token                  TOK_DEC
%token                  TOK_DEFAULT
%token                  TOK_DEFER
%token                  TOK_DIVEQ
%token                  TOK_DOTS
%token                  TOK_ELSE
%token                  TOK_EQ
%token                  TOK_FALLTHROUGH
%token  <floatval>      TOK_FLOAT
%token                  TOK_FOR
%token                  TOK_FUNC
%token                  TOK_GEQ
%token                  TOK_GO
%token                  TOK_GOTO
%token                  TOK_GT
%token                  TOK_IDENT
%token                  TOK_IF
%token                  TOK_INC
%token  <intval>        TOK_INT
%token                  TOK_INTERFACE
%token                  TOK_LEQ
%token                  TOK_LT
%token                  TOK_LSHIFT
%token                  TOK_LSHIFTEQ
%token                  TOK_MAP
%token                  TOK_MINUSEQ
%token                  TOK_NEQ
%token                  TOK_OR
%token                  TOK_OREQ
%token                  TOK_PACKAGE
%token                  TOK_PLUSEQ
%token                  TOK_RANGE
%token                  TOK_REMEQ
%token                  TOK_RETURN
%token                  TOK_RSHIFT
%token                  TOK_RSHIFTEQ
%token  <runeval>       TOK_RUNE
%token                  TOK_SELECT
%token  <strval>        TOK_STR
%token                  TOK_STRUCT
%token                  TOK_SWITCH
%token                  TOK_TIMESEQ
%token                  TOK_TYPE
%token                  TOK_VAR
%token                  TOK_XOREQ

%start program
%error-verbose

%%

program:
        ;
