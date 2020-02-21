#ifndef TREE_H
#define TREE_H
#include <stdbool.h>
#include <stdlib.h>

typedef struct tree_decls PROGRAM;

enum tree_decls_kind
{
    tree_decls_kind_var_spec,
    tree_decls_kind_type_spec,
    tree_decls_kind_func_decl
};

enum tree_type_kind
{
    tree_type_kind_name,
    tree_type_kind_array,
    tree_type_kind_slice,
    tree_type_kind_struct
};

enum tree_stmts_kind
{
    tree_stmts_kind_exp,
    tree_stmts_kind_assign,
    tree_stmts_kind_shortdecl,
    tree_stmts_kind_inc,
    tree_stmts_kind_dec,
    tree_stmts_kind_var_spec,
    tree_stmts_kind_type_spec,
    tree_stmts_kind_print,
    tree_stmts_kind_println,
    tree_stmts_kind_return,
    tree_stmts_kind_if,
    tree_stmts_kind_switch,
    tree_stmts_kind_for,
    tree_stmts_kind_break,
    tree_stmts_kind_continue,
    tree_stmts_kind_fallthrough
};

enum tree_exp_kind
{
    tree_exp_kind_ident,
    tree_exp_kind_int,
    tree_exp_kind_float,
    tree_exp_kind_rune,
    tree_exp_kind_str,
    tree_exp_kind_unary,
    tree_exp_kind_binary,
    tree_exp_kind_call,
    tree_exp_kind_index,
    tree_exp_kind_field,
    tree_exp_kind_append,
    tree_exp_kind_len,
    tree_exp_kind_cap
};

enum tree_unaryexp_kind
{
    tree_unaryexp_kind_plus,
    tree_unaryexp_kind_minus,
    tree_unaryexp_kind_not,
    tree_unaryexp_kind_comp
};

enum tree_binaryexp_kind
{
    tree_binaryexp_kind_or,
    tree_binaryexp_kind_and,
    tree_binaryexp_kind_eq,
    tree_binaryexp_kind_neq,
    tree_binaryexp_kind_lt,
    tree_binaryexp_kind_leq,
    tree_binaryexp_kind_gt,
    tree_binaryexp_kind_geq,
    tree_binaryexp_kind_plus,
    tree_binaryexp_kind_minus,
    tree_binaryexp_kind_bitor,
    tree_binaryexp_kind_xor,
    tree_binaryexp_kind_times,
    tree_binaryexp_kind_div,
    tree_binaryexp_kind_rem,
    tree_binaryexp_kind_lshift,
    tree_binaryexp_kind_rshift,
    tree_binaryexp_kind_bitand,
    tree_binaryexp_kind_andnot
};

// construct types
struct tree_var_spec
{
    char *name;
    struct tree_type *type;
    struct tree_exp *val;
};

struct tree_type_spec
{
    char *name;
    struct tree_type *type;
};

struct tree_func_decl
{
    char *name;
    struct tree_vars *params;
    struct tree_type *type;
    struct tree_stmts *body;
};

struct tree_decls
{
    enum tree_decls_kind kind;

    union
    {
        struct tree_var_spec var_spec;
        struct tree_type_spec type_spec;
        struct tree_func_decl func_decl;
    };

    struct tree_decls *next;
};

struct tree_type_array
{
    struct tree_type *type;
    int len;
};

struct tree_type_slice
{
    struct tree_type *type;
};

struct tree_type_struct
{
    struct tree_vars *fields;
};

struct tree_type
{
    enum tree_type_kind kind;

    union
    {
        struct tree_type_array array;
        struct tree_type_slice slice;
        struct tree_type_struct structtype;
    };
};

struct tree_vars
{
    struct tree_type *type;
    char *name;

    struct tree_vars *next;
};

struct tree_unaryexp
{
    enum tree_unaryexp_kind kind;
    struct tree_exp *right;
};

struct tree_binaryexp
{
    enum tree_binaryexp_kind kind;
    struct tree_exp *left;
    struct tree_exp *right;
};

struct tree_call
{
    struct tree_exp *func;
    struct tree_exps *args;
};

struct tree_index
{
    struct tree_exp *arr;
    struct tree_exp *index;
};

struct tree_field
{
    struct tree_exp *left;
    struct tree_exps *right;
};

struct tree_append
{
    struct tree_exp *arg1;
    struct tree_exps *arg2;
};

struct tree_exp
{
    enum tree_exp_kind kind;

    union
    {
        char *ident;
        int intval;
        double floatval;
        char runeval;
        char *strval;
        struct tree_unaryexp unary;
        struct tree_binaryexp binary;
        struct tree_call call;
        struct tree_index index;
        struct tree_field field;
        struct tree_append append;
        struct tree_exp *arg;
    };
};

struct tree_assign
{
    char *var;
    struct tree_exp *exp;
};

struct tree_shortdecl
{
    char *name;
    struct tree_exp *exp;
};

struct tree_if
{
    struct tree_stmt *init;
    struct tree_exp *condition;
    struct tree_stmts *body;
    struct tree_stmts *elsebody;
};

struct tree_switch
{
    struct tree_stmt *init;
    struct tree_exp *exp;
    struct tree_cases *cases;
};

struct tree_for
{
    struct tree_stmt *init;
    struct tree_exp *condition;
    struct tree_stmt *iter;
    struct tree_stmts *body;
};

struct tree_stmts
{
    enum tree_stmts_kind kind;

    union
    {
        struct tree_exp expstmt;
        struct tree_assign assign;
        struct tree_shortdecl shortdecl;
        char *var;
        struct tree_var_spec var_spec;
        struct tree_type_spec type_spec;
        struct tree_exps *exps;
        struct tree_exp *exp;
        struct tree_if ifstmt;
        struct tree_switch switchstmt;
        struct tree_for forstmt;
    };

    struct tree_stmts *next;
};

struct tree_cases
{
    struct tree_exp *val;
    struct tree_stmts *body;

    struct tree_cases *next;
};

struct tree_exps
{
    struct tree_exp *exp;
    struct tree_exps *next;
};

// signatures
PROGRAM *tree_make_program();

#endif
