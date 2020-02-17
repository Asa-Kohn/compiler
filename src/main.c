#include "stdio.h"
#include <string.h>

// #include "tree.h"
// #include "pretty.h"
// #include "symbol.h"

void yyparse();

enum mode {scan, tokens, parse, pretty};
enum mode m;

PROGRAM* root;

int main(int argc, char **argv){
	yyparse();
	
	if(strcmp(argv[1], "scan") == 0){
		m = scan;
	}
	if(strcmp(argv[1], "tokens") == 0){
		m = tokens;
	}
	if(strcmp(argv[1], "parse") == 0){
		m = parse;
	}
	if(strcmp(argv[1], "pretty") == 0){
		m = pretty;
		// prettyPROGRAM(root);
	}

	// yyparse();
	printf("OK\n");
	return 0;
}
