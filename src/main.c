#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"
#include "pretty.h"
#include "golite.tab.h"

int yylex();
int print_tokens = 0;
PROGRAM* root;

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
        printf("OK\n");
    }
    else if(strcmp(argv[1], "pretty") == 0)
    {
        yyparse();
        pretty_program(root);
    }

    return 0;
}
