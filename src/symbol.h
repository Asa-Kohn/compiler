#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdlib.h>

#include "tree.h"

enum symbol_kind
{
    symbol_kind_const,
    symbol_kind_var,
    symbol_kind_type,
    symbol_kind_func
};

struct symbol_const
{
    int constval;
    struct tree_type *type;
};

struct symbol_rec
{
    size_t num;
    char *name;
    struct tree_func_decl *scope;
    enum symbol_kind kind;
    union
    {
        struct tree_type *type;
        struct tree_func_decl *func;
        struct symbol_const constrec;
    };
};

struct symbol_rec *symbol_weave(struct tree_decls *root);

#endif
