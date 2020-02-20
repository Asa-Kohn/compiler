/* Code section */
%{
#include <stdlib.h>
#include <stdio.h>

// Reference to the yylex function to quiet a compiler warning
// Reference to the line number generated by the scanner
int yylex();
extern int yylineno;

void yyerror(char const *s)
{
    fprintf(stderr, "Error: syntax on line %d: %s\n", yylineno, s);
    exit(1);
}

%}

/* The union directive defines the yylval union used for associating (a) terminals (tokens)
 * returned by flex with additional scanning data (such as identifier/constant values); and
 * optionally (b) non-terminals (variables in productions) with AST information if any.
 */
%union
{
    int intval;
    double floatval;
    char *strval;
    char runeval;
}

/* Token directives define the token types to be returned by the scanner (excluding character
 * tokens). Each token definition takes [optionally, a reference to the associated field in the
 * yylval union] and an identifier. Multiple tokens can eb defined per directive by using a list
 * of identifiers separated by spaces.
 */
%token                  TOK_AND
%token                  TOK_ANDEQ
%token                  TOK_ANDNOT
%token                  TOK_ANDNOTEQ
%token                  TOK_APPEND
%token                  TOK_ASSIGN
%token                  TOK_BREAK
%token                  TOK_CAP
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
%token                  TOK_LEN
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
%token                  TOK_PRINT
%token                  TOK_PRINTLN
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

/* Precedence directives resolve grammar ambiguities by breaking ties between shift/reduce
 * operations. Tokens are grouped into precendence levels, with lower precedence coming first
 * and then higher precedence in later directives. Tokens specified in the same directive have
 * the same precedence. Ties at the same level are broken using either %left or %right, which
 * denote left-associative and right-associative respectively.
 */
%left TOK_OR 
%left TOK_AND 
%left TOK_EQ TOK_NEQ '<' TOK_LEQ '>' TOK_GEQ
%left '+' '-' '|' '^'
%left '*' '/' '%' TOK_LSHIFT TOK_RSHIFT '&' TOK_ANDNOT 
%left UNARY
%left '.'
%nonassoc '(' '['

/* Start token (by default if this is missing it takes the first production */
%start program
/* Generate the yylloc structure used for storing line numbers with tokens */
%locations
/* Generate detailed error messages */
%error-verbose

/* The second section of a bison file contains the productions. Note that rules with the
 * same LHS may be joined together and separated with a pipe.
 */
%%

program:        package_decl ';' top_level_decls
        ;

package_decl:
                TOK_PACKAGE TOK_IDENT
        ;

top_level_decls:
        |       var_decl ';' top_level_decls
        |       type_decl ';' top_level_decls
        |       func_decl ';' top_level_decls
        ;

var_decl:       TOK_VAR var_spec
        |       TOK_VAR '(' var_specs ')'
        ;

var_specs:
        |       var_spec ';' var_specs
        ;

var_spec:       idents type
        |       idents '=' exps
        |       idents type '=' exps
        ;

idents:         TOK_IDENT
        |       TOK_IDENT ',' idents
        ;

exps:           exp
        |       exp ',' exps
        ;

type_decl:      TOK_TYPE TOK_IDENT type
        |       TOK_TYPE '(' type_specs ')'
        ;

type_specs:
        |       TOK_IDENT type ';' type_specs
        ;

func_decl:      TOK_FUNC TOK_IDENT '(' params ')' type block
        |       TOK_FUNC TOK_IDENT '(' params ')' block
        ;

block:           '{' stmts '}'
        ;

params:
        |       idents type ',' params
        ;

type:           TOK_IDENT
        |       '[' ']' type
        |       '[' TOK_INT ']' type
        |       TOK_STRUCT '{' field_decls '}'
        |       '('type ')'
        ;

field_decls:
        |       idents type ';' field_decls
        ;


stmts:
        |       simplestmt ';' stmts
        |       var_decl ';' stmts
        |       type_decl ';' stmts
        |       TOK_PRINT '(' exps ')' ';' stmts
        |       TOK_PRINT '(' ')' ';' stmts
        |       TOK_PRINTLN '(' exps ')' ';' stmts
        |       TOK_PRINTLN '(' ')' ';' stmts
        |       TOK_RETURN ';' stmts
        |       TOK_RETURN exp ';' stmts
        |       ifstmt ';' stmts
        |       switchstmt ';' stmts
        |       TOK_FOR block ';' stmts
        |       TOK_FOR exp block ';' stmts
        |       TOK_FOR simplestmt ';' exp ';' simplestmt block ';' stmts
        |       TOK_BREAK ';' stmts
        |       TOK_CONTINUE ';' stmts
        |       TOK_FALLTHROUGH ';' stmts
        ;

simplestmt:
        |       exp
        |       assignment
        |       idents TOK_ASSIGN exps
        |       exp TOK_INC
        |       exp TOK_DEC
        ;

assignment:     TOK_IDENT '=' exp
        |       TOK_IDENT TOK_PLUSEQ exp
        |       TOK_IDENT TOK_MINUSEQ exp
        |       TOK_IDENT TOK_OREQ exp
        |       TOK_IDENT TOK_XOREQ exp
        |       TOK_IDENT TOK_TIMESEQ exp
        |       TOK_IDENT TOK_DIVEQ exp
        |       TOK_IDENT TOK_REMEQ exp
        |       TOK_IDENT TOK_LSHIFTEQ exp
        |       TOK_IDENT TOK_RSHIFTEQ exp
        |       TOK_IDENT TOK_ANDEQ exp
        |       TOK_IDENT TOK_ANDNOTEQ exp
        ;

ifstmt:         TOK_IF exp block
        |       TOK_IF exp block TOK_ELSE ifstmt
        |       TOK_IF exp block TOK_ELSE block
        |       TOK_IF simplestmt ';' exp block
        |       TOK_IF simplestmt ';' exp block TOK_ELSE ifstmt
        |       TOK_IF simplestmt ';' exp block TOK_ELSE block
        ;

switchstmt:     TOK_SWITCH '{' cases '}'
        |       TOK_SWITCH simplestmt ';' '{' cases '}'
        |       TOK_SWITCH exp '{' cases '}'
        |       TOK_SWITCH simplestmt ';' exp '{' cases '}'
        ;

cases:
        |       TOK_CASE exps ':' stmts cases
        |       TOK_DEFAULT ':' stmts cases
        ;

exp:            '(' exp ')'
        |       TOK_IDENT
        |       TOK_INT
        |       TOK_FLOAT
        |       TOK_RUNE
        |       TOK_STR
        |       '+' exp %prec UNARY
        |       '-' exp %prec UNARY
        |       '!' exp %prec UNARY
        |       '^' exp %prec UNARY
        |       exp TOK_OR exp
        |       exp TOK_AND exp
        |       exp TOK_EQ exp
        |       exp TOK_NEQ exp
        |       exp '<' exp
        |       exp TOK_LEQ exp
        |       exp '>' exp
        |       exp TOK_GEQ exp
        |       exp '+' exp
        |       exp '-' exp
        |       exp '|' exp
        |       exp '^' exp
        |       exp '*' exp
        |       exp '/' exp
        |       exp '%' exp
        |       exp TOK_LSHIFT exp
        |       exp TOK_RSHIFT exp
        |       exp '&' exp
        |       exp TOK_ANDNOT exp
        |       exp '(' ')'
        |       exp '(' exps ')'
        |       exp '[' exp ']'
        |       exp '.' TOK_IDENT
        |       TOK_APPEND '(' exp ',' exp ')'
        |       TOK_LEN '(' exp ')'
        |       TOK_CAP '(' exp ')'
        ;
