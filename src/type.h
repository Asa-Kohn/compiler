#ifndef TYPE_H
#define TYPE_H

#include "tree.h"

void typecheck(struct tree_decls *root);
struct tree_type *rt(struct tree_type *type);
int isbool(struct tree_type *type);
int isinteger(struct tree_type *type);

#endif
