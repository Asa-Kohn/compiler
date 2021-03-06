#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"
#include "golite.tab.h"
#include "weed.h"
#include "pretty.h"
#include "symbol.h"
#include "symbolprint.h"
#include "type.h"
#include "python.h"

int yylex();
int print_tokens = 0;
extern struct tree_decls *root;

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "Error: exactly one argument is required\n");
        exit(1);
    }

    if(strcmp(argv[1], "scan") == 0)
    {
        while(yylex());
        printf("OK\n");
    }
    else if(strcmp(argv[1], "tokens") == 0)
    {
        print_tokens = 1;
        while(yylex());
    }
    else if(strcmp(argv[1], "parse") == 0)
    {
        yyparse();
        weed(root);
        printf("OK\n");
    }
    else if(strcmp(argv[1], "pretty") == 0)
    {
        yyparse();
        weed(root);
        pretty_program(root);
    }
    else if(strcmp(argv[1], "symbol") == 0)
    {
        yyparse();
        weed(root);
        symbol_weave(root);
        symbolprint(root);
    }
    else if(strcmp(argv[1], "typecheck") == 0)
    {
        yyparse();
        weed(root);
        symbol_weave(root);
        typecheck(root);
        printf("OK\n");
    }
    else if(strcmp(argv[1], "codegen") == 0)
    {
        yyparse();
        weed(root);
        struct symbol_rec *symbols = symbol_weave(root);
        typecheck(root);
        py_program(root, symbols);
    }

    return 0;
}
