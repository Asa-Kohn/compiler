%{

#include <stdlib.h>
#include <stdio.h>

#include "golite.tab.h"
#include "utils.h"

//print/return functions for string token & char tokens
#define OUTPUT_TOKEN(TOK) { if(print_tokens) puts( #TOK ); else return TOK; }
#define OUTPUT_TOKEN_CHAR(TOK) { if(print_tokens) printf("%c\n", TOK); else return TOK; }
#define YY_USER_ACTION { last_token = yy_act; }
extern int print_tokens;
static int last_token = 0;

%}

%option yylineno

%%

[ \n\t\r]*                                          //tab, return, etc

\/\/.*                                              //single line comment

\/\*([^*]*|\*[^\/])*\*\/                            //multi line comment

\/\*([^*]*|\*[^/])*\n([^*]*|\*[^/])*\*\/|\n {       //semi colon insertion
    if(last_token == TOK_IDENT ||
       last_token == TOK_INT ||
       last_token == TOK_FLOAT ||
       last_token == TOK_RUNE ||
       last_token == TOK_STR ||
       last_token == TOK_BREAK ||
       last_token == TOK_CONTINUE ||
       last_token == TOK_FALLTHROUGH ||
       last_token == TOK_RETURN ||
       last_token == TOK_INC ||
       last_token == TOK_DEC ||
       last_token == ')' ||
       last_token == ']' ||
       last_token == '}')
        OUTPUT_TOKEN_CHAR(';');
}

"append" OUTPUT_TOKEN(TOK_APPEND)
"break" OUTPUT_TOKEN(TOK_BREAK)
"cap" OUTPUT_TOKEN(TOK_CAP)
"case" OUTPUT_TOKEN(TOK_CASE)
"chan" OUTPUT_TOKEN(TOK_CHAN)
"const" OUTPUT_TOKEN(TOK_CONST)
"continue" OUTPUT_TOKEN(TOK_CONTINUE)
"default" OUTPUT_TOKEN(TOK_DEFAULT)
"defer" OUTPUT_TOKEN(TOK_DEFER)
"else" OUTPUT_TOKEN(TOK_ELSE)
"fallthrough" OUTPUT_TOKEN(TOK_FALLTHROUGH)
"for" OUTPUT_TOKEN(TOK_FOR)
"func" OUTPUT_TOKEN(TOK_FUNC)
"go" OUTPUT_TOKEN(TOK_GO)
"goto" OUTPUT_TOKEN(TOK_GOTO)
"if" OUTPUT_TOKEN(TOK_IF)
"import" { fprintf(stderr, "Error: import not supported\n"); exit(1); }
"interface" OUTPUT_TOKEN(TOK_INTERFACE)
"len" OUTPUT_TOKEN(TOK_LEN)
"map" OUTPUT_TOKEN(TOK_MAP)
"package" OUTPUT_TOKEN(TOK_PACKAGE)
"print" OUTPUT_TOKEN(TOK_PRINT)
"println" OUTPUT_TOKEN(TOK_PRINTLN)
"range" OUTPUT_TOKEN(TOK_RANGE)
"return" OUTPUT_TOKEN(TOK_RETURN)
"select" OUTPUT_TOKEN(TOK_SELECT)
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

'.' { yylval.runeval = yytext[1]; OUTPUT_TOKEN(TOK_RUNE) }
"'\\a'" { yylval.runeval = '\a'; OUTPUT_TOKEN(TOK_RUNE) }
"'\\b'" { yylval.runeval = '\b'; OUTPUT_TOKEN(TOK_RUNE) }
"'\\f'" { yylval.runeval = '\f'; OUTPUT_TOKEN(TOK_RUNE) }
"'\\n'" { yylval.runeval = '\n'; OUTPUT_TOKEN(TOK_RUNE) }
"'\\r'" { yylval.runeval = '\r'; OUTPUT_TOKEN(TOK_RUNE) }
"'\\t'" { yylval.runeval = '\t'; OUTPUT_TOKEN(TOK_RUNE) }
"'\\v'" { yylval.runeval = '\v'; OUTPUT_TOKEN(TOK_RUNE) }
"'\\\\'" { yylval.runeval = '\\'; OUTPUT_TOKEN(TOK_RUNE) }
"'\\''" { yylval.runeval = '\''; OUTPUT_TOKEN(TOK_RUNE) }

0x[a-fA-F]+|[0-9]+ {
    yylval.intval = strtol(yytext, NULL, 0);
    OUTPUT_TOKEN(TOK_INT)
}

\"([^\\\"]|\\a|\\b|\\f|\\n|\\r|\\t|\\v|\\\\|\\\")*\" {
    size_t i = 0;
    char *c;
    for(c = yytext; *c; c++, c += (*c == '\\'))
        i++;
    yylval.strval = emalloc(i);
    for(i = 0, c = yytext; *c; i++, c++)
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
    printf(":%s:\n", yylval.strval);
    OUTPUT_TOKEN(TOK_STR)
}

[0-9]*\.[0-9]+|[0-9]+\.[0-9]* {
    yylval.floatval = strtod(yytext, NULL);
    OUTPUT_TOKEN(TOK_FLOAT)
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.strval = estrdup(yytext);
    OUTPUT_TOKEN(TOK_IDENT)
}

. {
    fprintf(stderr, "Error: unexpected character '%c'\n", yytext[0]);
    exit(1);
}
