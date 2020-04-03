%{

#include <stdlib.h>
#include <stdio.h>

#include "golite.tab.h"
#include "utils.h"
#include "tree.h"

//print/return functions for string token & char tokens
#define OUTPUT_TOKEN(TOK)                       \
    {                                           \
        last_token = TOK;                       \
        if(print_tokens)                        \
            puts( #TOK );                       \
        else return TOK;                        \
    }

#define OUTPUT_TOK_INT                              \
    {                                               \
        last_token = TOK_INT;                       \
        if(print_tokens)                            \
            printf("TOK_INT(%d)\n", yylval.intval); \
        else return TOK_INT;                        \
    }

#define OUTPUT_TOK_FLOAT                                \
    {                                                   \
        last_token = TOK_FLOAT;                         \
        if(print_tokens)                                \
            printf("TOK_FLOAT(%f)\n", yylval.floatval); \
        else return TOK_FLOAT;                          \
    }

#define OUTPUT_TOK_RUNE                                 \
    {                                                   \
        last_token = TOK_RUNE;                          \
        if(print_tokens)                                \
            printf("TOK_RUNE(%c)\n", yylval.runeval);   \
        else return TOK_RUNE;                           \
    }

#define OUTPUT_TOK_STR                              \
    {                                               \
        last_token = TOK_STR;                       \
        if(print_tokens)                            \
            printf("TOK_STR(%s)\n", yylval.strval); \
        else return TOK_STR;                        \
    }

#define OUTPUT_TOK_IDENT                                \
    {                                                   \
        last_token = TOK_IDENT;                         \
        if(print_tokens)                                \
            printf("TOK_IDENT(%s)\n", yylval.strval);   \
        else return TOK_IDENT;                          \
    }

#define OUTPUT_TOKEN_CHAR(TOK)                                  \
    {                                                           \
        last_token = TOK;                                       \
        if(print_tokens) printf("%c\n", TOK); else return TOK;  \
    }

extern int print_tokens;
static int last_token = 0;

%}

%option yylineno

%%

<<EOF>> {
    if(last_token == TOK_IDENT ||
       last_token == TOK_INT ||
       last_token == TOK_FLOAT ||
       last_token == TOK_RUNE ||
       last_token == TOK_STR ||
       last_token == TOK_BREAK ||
       last_token == TOK_CONTINUE ||
       last_token == TOK_RETURN ||
       last_token == TOK_INC ||
       last_token == TOK_DEC ||
       last_token == ')' ||
       last_token == ']' ||
       last_token == '}')
    {
        OUTPUT_TOKEN_CHAR(';');
    }
    else
        yyterminate();
}

\/\/.*                                              //single line comment

\/\*[^\*]*\*+([^\*\/][^\*]*\*+)*\/|[ \n\t\r] {      //multi line comment
    //semicolon insertion
    if(strchr(yytext, '\n') && (last_token == TOK_IDENT ||
                                last_token == TOK_INT ||
                                last_token == TOK_FLOAT ||
                                last_token == TOK_RUNE ||
                                last_token == TOK_STR ||
                                last_token == TOK_BREAK ||
                                last_token == TOK_CONTINUE ||
                                last_token == TOK_RETURN ||
                                last_token == TOK_INC ||
                                last_token == TOK_DEC ||
                                last_token == ')' ||
                                last_token == ']' ||
                                last_token == '}'))
        OUTPUT_TOKEN_CHAR(';');
 }

"append" OUTPUT_TOKEN(TOK_APPEND)
"break" OUTPUT_TOKEN(TOK_BREAK)
"cap" OUTPUT_TOKEN(TOK_CAP)
"case" OUTPUT_TOKEN(TOK_CASE)
"chan" { fprintf(stderr, "Error: channels not supported\n"); exit(1); }
"const" { fprintf(stderr, "Error: constants not supported\n"); exit(1); }
"continue" OUTPUT_TOKEN(TOK_CONTINUE)
"default" OUTPUT_TOKEN(TOK_DEFAULT)
"defer" { fprintf(stderr, "Error: defer not supported\n"); exit(1); }
"else" OUTPUT_TOKEN(TOK_ELSE)
"fallthrough" {
    fprintf(stderr, "Error: fallthrough not supported\n"); exit(1);
}
"for" OUTPUT_TOKEN(TOK_FOR)
"func" OUTPUT_TOKEN(TOK_FUNC)
"go" { fprintf(stderr, "Error: go not supported\n"); exit(1); }
"goto" { fprintf(stderr, "Error: goto not supported\n"); exit(1); }
"if" OUTPUT_TOKEN(TOK_IF)
"import" { fprintf(stderr, "Error: import not supported\n"); exit(1); }
"interface" { fprintf(stderr, "Error: interfaces not supported\n"); exit(1); }
"len" OUTPUT_TOKEN(TOK_LEN)
"map" { fprintf(stderr, "Error: map not supported\n"); exit(1); }
"package" OUTPUT_TOKEN(TOK_PACKAGE)
"print" OUTPUT_TOKEN(TOK_PRINT)
"println" OUTPUT_TOKEN(TOK_PRINTLN)
"range" { fprintf(stderr, "Error: range not supported\n"); exit(1); }
"return" OUTPUT_TOKEN(TOK_RETURN)
"select" { fprintf(stderr, "Error: select not supported\n"); exit(1); }
"struct" OUTPUT_TOKEN(TOK_STRUCT)
"switch" OUTPUT_TOKEN(TOK_SWITCH)
"type" OUTPUT_TOKEN(TOK_TYPE)
"var" OUTPUT_TOKEN(TOK_VAR)

"+" OUTPUT_TOKEN_CHAR('+')
"-" OUTPUT_TOKEN_CHAR('-')
"*" OUTPUT_TOKEN_CHAR('*')
"/" OUTPUT_TOKEN_CHAR('/')
"%" OUTPUT_TOKEN_CHAR('%')

"&" OUTPUT_TOKEN_CHAR('&')
"|" OUTPUT_TOKEN_CHAR('|')
"^" OUTPUT_TOKEN_CHAR('^')

"<<" OUTPUT_TOKEN(TOK_LSHIFT)
">>" OUTPUT_TOKEN(TOK_RSHIFT)

"&^" OUTPUT_TOKEN(TOK_ANDNOT)

"+=" OUTPUT_TOKEN(TOK_PLUSEQ)
"-=" OUTPUT_TOKEN(TOK_MINUSEQ)
"*=" OUTPUT_TOKEN(TOK_TIMESEQ)
"/=" OUTPUT_TOKEN(TOK_DIVEQ)
"%=" OUTPUT_TOKEN(TOK_REMEQ)

"&=" OUTPUT_TOKEN(TOK_ANDEQ)
"|=" OUTPUT_TOKEN(TOK_OREQ)
"^=" OUTPUT_TOKEN(TOK_XOREQ)

"<<=" OUTPUT_TOKEN(TOK_LSHIFTEQ)
">>=" OUTPUT_TOKEN(TOK_RSHIFTEQ)

"&^=" OUTPUT_TOKEN(TOK_ANDNOTEQ)

"&&" OUTPUT_TOKEN(TOK_AND)
"||" OUTPUT_TOKEN(TOK_OR)

"<-" { fprintf(stderr, "Error: channels not supported\n"); exit(1); }

"++" OUTPUT_TOKEN(TOK_INC)
"--" OUTPUT_TOKEN(TOK_DEC)

"==" OUTPUT_TOKEN(TOK_EQ)
"<" OUTPUT_TOKEN_CHAR('<')
">" OUTPUT_TOKEN_CHAR('>')
"=" OUTPUT_TOKEN_CHAR('=')
"!" OUTPUT_TOKEN_CHAR('!')
"!=" OUTPUT_TOKEN(TOK_NEQ)
"<=" OUTPUT_TOKEN(TOK_LEQ)
">=" OUTPUT_TOKEN(TOK_GEQ)
":=" OUTPUT_TOKEN(TOK_ASSIGN)

"..." { fprintf(stderr, "Error: variadic functions not supported\n"); exit(1); }

"(" OUTPUT_TOKEN_CHAR('(')
")" OUTPUT_TOKEN_CHAR(')')
"[" OUTPUT_TOKEN_CHAR('[')
"]" OUTPUT_TOKEN_CHAR(']')
"{" OUTPUT_TOKEN_CHAR('{')
"}" OUTPUT_TOKEN_CHAR('}')

"," OUTPUT_TOKEN_CHAR(',')
"." OUTPUT_TOKEN_CHAR('.')
":" OUTPUT_TOKEN_CHAR(':')
";" OUTPUT_TOKEN_CHAR(';')

'[^\\']' { yylval.runeval = yytext[1]; OUTPUT_TOK_RUNE }
"'\\a'" { yylval.runeval = '\a'; OUTPUT_TOK_RUNE }
"'\\b'" { yylval.runeval = '\b'; OUTPUT_TOK_RUNE }
"'\\f'" { yylval.runeval = '\f'; OUTPUT_TOK_RUNE }
"'\\n'" { yylval.runeval = '\n'; OUTPUT_TOK_RUNE }
"'\\r'" { yylval.runeval = '\r'; OUTPUT_TOK_RUNE }
"'\\t'" { yylval.runeval = '\t'; OUTPUT_TOK_RUNE }
"'\\v'" { yylval.runeval = '\v'; OUTPUT_TOK_RUNE }
"'\\\\'" { yylval.runeval = '\\'; OUTPUT_TOK_RUNE }
"'\\''" { yylval.runeval = '\''; OUTPUT_TOK_RUNE }

0x[0-9a-fA-F]+|[1-9][0-9]*|0[0-7]* {
    yylval.intval = strtol(yytext, NULL, 0);
    OUTPUT_TOK_INT
}

\"([^\\\"\n]|\\a|\\b|\\f|\\n|\\r|\\t|\\v|\\\\|\\\")*\" {
    size_t i = 0;
    char *c;
    for(c = &yytext[1]; *c; c++, c += (*c == '\\'))
        i++;
    yylval.strval = emalloc(i);
    for(i = 0, c = &yytext[1]; *c != '"'; i++, c++)
    {
        if(*c == '\\')
            switch(*++c)
            {
                case 'a':
                    yylval.strval[i] = '\a';
                    break;
                case 'b':
                    yylval.strval[i] = '\b';
                    break;
                case 'f':
                    yylval.strval[i] = '\f';
                    break;
                case 'n':
                    yylval.strval[i] = '\n';
                    break;
                case 'r':
                    yylval.strval[i] = '\r';
                    break;
                case 't':
                    yylval.strval[i] = '\t';
                    break;
                case 'v':
                    yylval.strval[i] = '\v';
                    break;
                case '\\':
                    yylval.strval[i] = '\\';
                    break;
                case '"':
                    yylval.strval[i] = '"';
                    break;
            }
        else yylval.strval[i] = *c;
    }
    yylval.strval[i] = '\0';
    OUTPUT_TOK_STR
}

`[^`]*` {
    size_t len = strlen(yytext - 1);
    yylval.strval = emalloc(len - 1);
    strncpy(yylval.strval, &yytext[1], len - 2);
    yylval.strval[len - 2] = '\0';
    OUTPUT_TOK_STR
}

[0-9]*\.[0-9]+|[0-9]+\.[0-9]* {
    yylval.floatval = strtod(yytext, NULL);
    OUTPUT_TOK_FLOAT
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.strval = estrdup(yytext);
    OUTPUT_TOK_IDENT
}

. {
    fprintf(stderr, "Error: unexpected character '%c'\n", yytext[0]);
    exit(1);
}
