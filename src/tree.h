#ifndef TREE_H
#define TREE_H
#include <stdbool.h>
#include <stdlib.h>

typedef struct tree_var_spec VAR_SPEC;
typedef struct tree_type_spec TYPE_SPEC;
typedef struct tree_func_decl FUNC_DECL;
typedef struct tree_decls DECLS;
typedef struct tree_type_array TYPE_ARRAY;
typedef struct tree_type_slice TYPE_SLICE;
typedef struct tree_type_struct TYPE_STRUCT;
typedef struct tree_type TYPE;
typedef struct tree_params PARAMS;
typedef struct tree_fields FIELDS;
typedef struct tree_unaryexp UNARYEXP;
typedef struct tree_binaryexp BINARYEXP;
typedef struct tree_call CALL;
typedef struct tree_index INDEX;
typedef struct tree_field FIELD;
typedef struct tree_append APPEND;
typedef struct tree_exp EXP;
typedef struct tree_assignop ASSIGNOP;
typedef struct tree_assign ASSIGN;
typedef struct tree_shortdecl SHORTDECL;
typedef struct tree_if IF_STMT;
typedef struct tree_switch SWITCH_STMT;
typedef struct tree_for FOR_STMT;
typedef struct tree_stmts STMTS;
typedef struct tree_stmt STMT;
typedef struct tree_cases CASES;
typedef struct tree_exps EXPS;
typedef struct tree_idents IDENTS;
typedef struct tree_assignop ASSIGNOP;
typedef struct tree_ident IDENT;
typedef struct symbol_rec SYMBOL;

enum tree_decls_kind
{
    tree_decls_kind_var_decl,
    tree_decls_kind_type_spec,
    tree_decls_kind_func_decl,
    tree_decls_kind_package
};

enum tree_type_kind
{
    tree_type_kind_base,
    tree_type_kind_defined,
    tree_type_kind_reference,
    tree_type_kind_array,
    tree_type_kind_slice,
    tree_type_kind_struct
};

enum tree_stmt_kind
{
    tree_stmt_kind_exp,
    tree_stmt_kind_block,
    tree_stmt_kind_assign,
    tree_stmt_kind_assignop,
    tree_stmt_kind_shortdecl,
    tree_stmt_kind_inc,
    tree_stmt_kind_dec,
    tree_stmt_kind_var_decl,
    tree_stmt_kind_type_spec,
    tree_stmt_kind_print,
    tree_stmt_kind_println,
    tree_stmt_kind_return,
    tree_stmt_kind_if,
    tree_stmt_kind_switch,
    tree_stmt_kind_for,
    tree_stmt_kind_break,
    tree_stmt_kind_continue
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

enum tree_assignop_kind
{
    tree_assignop_kind_plus,
    tree_assignop_kind_minus,
    tree_assignop_kind_or,
    tree_assignop_kind_xor,
    tree_assignop_kind_times,
    tree_assignop_kind_div,
    tree_assignop_kind_rem,
    tree_assignop_kind_lshift,
    tree_assignop_kind_rshift,
    tree_assignop_kind_and,
    tree_assignop_kind_andnot
};

enum tree_base_type
{
    tree_base_type_int,
    tree_base_type_float64,
    tree_base_type_rune,
    tree_base_type_bool,
    tree_base_type_str
};

// construct types
struct tree_var_spec
{
    struct tree_ident *ident;
    struct tree_type *type;
    struct tree_exp *val;

    struct tree_var_spec *next;
};

struct tree_type_spec
{
    struct tree_ident *ident;
    struct tree_type *type;
};

struct tree_func_decl
{
    struct tree_ident *ident;
    struct tree_params *params;
    struct tree_type *type;
    struct tree_stmts *body;
};

struct tree_decls
{
    enum tree_decls_kind kind;
    union
    {
        struct tree_var_spec *var_spec;
        struct tree_type_spec type_spec;
        struct tree_func_decl func_decl;
        char *package;
    };

    struct tree_decls *next;
    int lineno;
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
    struct tree_fields *fields;
};

struct tree_ident
{
    char *name;
    struct symbol_rec *symbol;
};

struct tree_type
{
    enum tree_type_kind kind;

    union
    {
        enum tree_base_type base;
        struct tree_type_array array;
        struct tree_type_slice slice;
        struct tree_type_struct structtype;
        struct tree_type *defined;
        struct tree_ident reference;
    };
};

struct tree_fields
{
    struct tree_type *type;
    char *name;

    struct tree_fields *next;
};

struct tree_params
{
    struct tree_type *type;
    struct tree_ident *ident;

    struct tree_params *next;
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
    struct tree_exps *exps;
};

struct tree_index
{
    struct tree_exp *arr;
    struct tree_exp *index;
};

struct tree_field
{
    struct tree_exp *instance;
    char *field;
};

struct tree_append
{
    struct tree_exp *exp1;
    struct tree_exp *exp2;
};

struct tree_exp
{
    enum tree_exp_kind kind;

    union
    {
        int intval;
        double floatval;
        char runeval;
        char *strval;
        struct tree_ident *ident;
        struct tree_unaryexp unary;
        struct tree_binaryexp binary;
        struct tree_call call;
        struct tree_index index;
        struct tree_field field;
        struct tree_append append;
        struct tree_exp *exp;
    };

    struct tree_type *type;
    int lineno;
};

struct tree_assignop
{
    enum tree_assignop_kind kind;
    struct tree_exp *left;
    struct tree_exp *right;
};

struct tree_assign
{
    struct tree_exps *left;
    struct tree_exps *right;
};

struct tree_shortdecl
{
    struct tree_idents *idents;
    struct tree_exps *exps;
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

struct tree_stmt
{
    enum tree_stmt_kind kind;

    union
    {
        struct tree_exp expstmt;
        struct tree_assign assign;
        struct tree_assignop assignop;
        struct tree_shortdecl shortdecl;
        struct tree_stmts *block;
        struct tree_var_spec *var_spec;
        struct tree_type_spec type_spec;
        struct tree_exps *exps;
        struct tree_exp *exp;
        struct tree_if ifstmt;
        struct tree_switch switchstmt;
        struct tree_for forstmt;
    };

    int lineno;
};

struct tree_stmts
{
    struct tree_stmt stmt;

    struct tree_stmts *next;
};

struct tree_cases
{
    struct tree_exps *val;
    struct tree_stmts *body;

    struct tree_cases *next;
};

struct tree_idents
{
    struct tree_ident *ident;
    struct tree_idents *next;
};

struct tree_exps
{
    struct tree_exp *exp;
    struct tree_exps *next;
};

#endif
