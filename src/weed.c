// #include <stdbool.h>
// #include <stdlib.h>
// #include <stdio.h>
#include "weed.h"

//stmts_kind
void weed_stmts(STMTS *stmts) {
    if(stmts == NULL) return 0;
    switch (stmts->kind) {
        case tree_stmts_kind_exp:
        case tree_stmts_kind_assign:
        case tree_stmts_kind_shortdecl:
        case tree_stmts_kind_inc:
        case tree_stmts_kind_dec:
        case tree_stmts_kind_var_spec:
        case tree_stmts_kind_type_spec:
        case tree_stmts_kind_print:
        case tree_stmts_kind_println:
        case tree_stmts_kind_return:
        case tree_stmts_kind_if:
        case tree_stmts_kind_switch:
        case tree_stmts_kind_for:
        case tree_stmts_kind_break:
        case tree_stmts_kind_continue:
        case tree_stmts_kind_fallthrough:
    }
}

//decls_kind
void weed_decls(DECLS *decls) {
    if(decls == NULL) return 0;
    switch (decls->kind) {
        //TODO
        case tree_decls_kind_var_spec:
            weed_var_spec(decls->var_spec);
        case tree_decls_kind_type_spec:
            weed_type_spec(decls->type_spec);
        case tree_decls_kind_func_decl:
            weed_func_decl(decls->func_decl);
    }
    weed_decls(decls->next);
}

void weed_var_spec(VAR_SPEC *var_spec) {
    if(var_spec == NULL) return 0;
    weed_type(var_spec->type);
    weed_exp(var_spec->val);
}

//TODO
void weed_type_spec(TYPE_SPEC *type_spec);

void weed_func_decl(FUNC_DECL *func_decl){
    if(func_decl == NULL) return 0;
    weed_vars(func_decl->params);
    weed_type(func_decl->type);
    weed_stmts(func_decl->body);
}

//type_kind
void weed_type(TYPE *type) {
    if(type == NULL) return 0;
    switch (type->kind) {
        case tree_type_kind_name:
        case tree_type_kind_array:
        case tree_type_kind_slice:
        case tree_type_kind_struct:
    }
}

//binaryexp_kind
void weed_binaryexp(BINARYEXP *binaryexp){
    if(binaryexp == NULL) return 0;
    switch (binaryexp->kind) {
        case tree_binaryexp_kind_or:
        case tree_binaryexp_kind_and:
        case tree_binaryexp_kind_eq:
        case tree_binaryexp_kind_neq:
        case tree_binaryexp_kind_lt:
        case tree_binaryexp_kind_leq:
        case tree_binaryexp_kind_gt:
        case tree_binaryexp_kind_geq:
        case tree_binaryexp_kind_plus:
        case tree_binaryexp_kind_minus:
        case tree_binaryexp_kind_bitor:
        case tree_binaryexp_kind_xor:
        case tree_binaryexp_kind_times:
        case tree_binaryexp_kind_div:
        case tree_binaryexp_kind_rem:
        case tree_binaryexp_kind_lshift:
        case tree_binaryexp_kind_rshift:
        case tree_binaryexp_kind_bitand:
        case tree_binaryexp_kind_andnot:
            weed_exp(binaryexp->left);
            weed_exp(binaryexp->right);
    }
}

void weed_exps(EXPS *exps) {
    if(exps == NULL) return 0;
    weed_exp(exps->exp);
    weed_exps(exps->next);
}

//exp_kind
void weed_exp(EXP *exp) {
    if(exp == NULL) return 0;
    switch (exp->kind) {
        case tree_exp_kind_ident:
        case tree_exp_kind_int:
        case tree_exp_kind_float:
        case tree_exp_kind_rune:
        case tree_exp_kind_str:
        case tree_exp_kind_unary:
        case tree_exp_kind_binary:
        case tree_exp_kind_call:
        case tree_exp_kind_index:
        case tree_exp_kind_field:
        case tree_exp_kind_append:
        case tree_exp_kind_len:
        case tree_exp_kind_cap:
    }
}

//unaryexp_kind
void weed_unaryexp(UNARYEXP *unarexp) {
    if(unarexp == NULL) return 0;
    switch (unarexp->kind) {
        case tree_unaryexp_kind_plus:
        case tree_unaryexp_kind_minus:
        case tree_unaryexp_kind_not:
        case tree_unaryexp_kind_comp:
    }
}

void weed_type_array(TYPE_ARRAY *type_array);
void weed_type_slice(TYPE_SLICE *type_slice);
void weed_type_struct(TYPE_STRUCT *type_struct);
void weed_vars(VARS *vars);
void weed_call(CALL *call);
void weed_index(INDEX *index);
void weed_field(FIELD *field);
void weed_append(APPEND *append);

void weed_cases(CASES *cases);
void weed_assign(ASSIGN *assign);
void weed_shortdecl(SHORTDECL *shortdecl);



void weed_if(IF_STMT *if_stmt);
void weed_switch(SWITCH_STMT *switch_stmt);
void weed_for(FOR_STMT *for_stmt);
