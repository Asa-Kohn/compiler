#include "tree.h"
#include "utils.h"

// PROGRAM
PROGRAM *tree_make_program()
{
    PROGRAM *p = emalloc(sizeof(PROGRAM));
    return p;
}
