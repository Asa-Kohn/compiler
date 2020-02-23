#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "weed.h"

//stmts_kind
int weed_stmts(STMTS *stmts) {
    if(stmts == NULL) return 0;
    return weed_stmts(stmts->next) || 
        weed_stmt(&(stmts->stmt));
}

//stmt_kind
int weed_stmt(STMT *stmt) {
    if(stmt == NULL) return 0;
    switch (stmt->kind) {
        case tree_stmt_kind_exp:
        case tree_stmt_kind_assign:
            return 0;
        case tree_stmt_kind_shortdecl:
            return weed_shortdecl(&(stmt->shortdecl));
        case tree_stmt_kind_inc:
        case tree_stmt_kind_dec:
        case tree_stmt_kind_var_decl:
        case tree_stmt_kind_type_spec:
        case tree_stmt_kind_print:
        case tree_stmt_kind_println:
        case tree_stmt_kind_return:
        case tree_stmt_kind_if:
        case tree_stmt_kind_switch:
        case tree_stmt_kind_for:
            return 0;
        case tree_stmt_kind_break:
            // fprintf(stderr, "Error: break statement (line%d)", stmts->lineno);
            // exit(1);
            return 1;
        case tree_stmt_kind_continue:
            // fprintf(stderr, "Error: continue statement (line%d)", stmts->lineno);
            // exit(1);
            return 1;
        case tree_stmt_kind_fallthrough:
            return 0;
    }
    return 0;
}

//decls_kind
int weed_decls(DECLS *decls) {
    if(decls == NULL) return 0;
    switch (decls->kind) {
        case tree_decls_kind_var_decl:
            return weed_var_spec(decls->var_spec);
        case tree_decls_kind_type_spec:
            return weed_type_spec(&(decls->type_spec));
        case tree_decls_kind_func_decl:
            return weed_func_decl(&(decls->func_decl));
    }
    return weed_decls(decls->next);
}

int weed_vars(VARS *vars){
    if(vars == NULL) return 0;
    return weed_type(vars->type) ||
        weed_vars(vars->next);
}

int weed_var_spec(VAR_SPEC *var_spec) {
    if(var_spec == NULL) return 0;
    return weed_type(var_spec->type) ||
        weed_exp(var_spec->val) ||
        weed_var_spec(var_spec->next);
}

//TODO
int weed_type_spec(TYPE_SPEC *type_spec);

int weed_func_decl(FUNC_DECL *func_decl){
    if(func_decl == NULL) return 0;
    return weed_vars(func_decl->params) ||
        weed_type(func_decl->type) ||
        weed_stmts(func_decl->body);
}

//type_kind
int weed_type(TYPE *type) {
    if(type == NULL) return 0;
    switch (type->kind) {
        case tree_type_kind_name:
        case tree_type_kind_array:
        case tree_type_kind_slice:
        case tree_type_kind_struct:
            return 0;
    }
    return 0;
}

//binaryexp_kind
int weed_binaryexp(BINARYEXP *binaryexp){
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
            return weed_exp(binaryexp->left) ||
                weed_exp(binaryexp->right);
    }
    return 0;
}

int weed_exps(EXPS *exps) {
    if(exps == NULL) return 0;
    return weed_exp(exps->exp) ||
        weed_exps(exps->next);
}

//exp_kind
int weed_exp(EXP *exp) {
    if(exp == NULL) return 0;
    switch (exp->kind) {
        case tree_exp_kind_ident:
            if(exp->ident == '_'){
                // fprintf(stderr, "Error: blank identifier (line%d)", exp->lineno);
                // exit(1);
                return 1;
            }
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
            return 0;
    }
    return 0;
}

//unaryexp_kind
int weed_unaryexp(UNARYEXP *unarexp) {
    if(unarexp == NULL) return 0;
    switch (unarexp->kind) {
        case tree_unaryexp_kind_plus:
        case tree_unaryexp_kind_minus:
        case tree_unaryexp_kind_not:
        case tree_unaryexp_kind_comp:
            return 0;
    }
    return 0;
}


int weed_type_array(TYPE_ARRAY *type_array) {
    if(type_array == NULL) return 0;
    return weed_type(type_array->type);
}

int weed_type_slice(TYPE_SLICE *type_slice) {
    if(type_slice == NULL) return 0;
    return weed_type(type_slice->type);
}

int weed_type_struct(TYPE_STRUCT *type_struct) {
    if(type_struct == NULL) return 0;
    return weed_vars(type_struct->fields);
}

int weed_call(CALL *call) {
    if(call == NULL) return 0;
    return weed_exp(call->func) ||
        weed_exps(call->exps);
}

int weed_index(INDEX *index) {
    if(index == NULL) return 0;
    return weed_exp(index->arr) ||
        weed_exp(index->index);
}

int weed_field(FIELD *field) {
    if(field == NULL) return 0;
    return weed_exp(field->instance);
}

int weed_append(APPEND *append){
    if(append == NULL) return 0;
    return weed_exp(append->exp1) ||
        weed_exp(append->exp2);
}

int weed_cases(CASES *cases) {
    if(cases == NULL) return 0;
    return weed_exp(cases->val) ||
        weed_stmts(cases->body) ||
        weed_cases(cases->next);
}

int weed_assign(ASSIGN *assign) {
    if(assign == NULL) return 0;
    return weed_exp(assign->exp);
}

int weed_shortdecl(SHORTDECL *shortdecl) {
    if(shortdecl == NULL) return 0;
    // fprintf(stderr, "Error: short declaration (line%d)", exp->lineno);
    // exit(1);
    return 1;
    // weed_exp(shortdecl->exp);
}

int weed_if(IF_STMT *if_stmt) {
    if(if_stmt == NULL) return 0;
    return weed_stmts(if_stmt->init) ||
        weed_exp(if_stmt->condition) ||
        weed_stmts(if_stmt->body) ||
        weed_stmts(if_stmt->elsebody);
}

int weed_switch(SWITCH_STMT *switch_stmt) {
    if(switch_stmt == NULL) return 0;
    //switch statements not supported
    return 1;
        // weed_stmts(switch_stmt->init) ||
        // weed_exp(switch_stmt->exp) ||
        // weed_cases(switch_stmt->cases);
}

int weed_for(FOR_STMT *for_stmt) {
    if(for_stmt == NULL) return 0;
    return weed_stmts(for_stmt->init) ||
        weed_exp(for_stmt->condition) ||
        weed_stmts(for_stmt->iter) ||
        weed_stmts(for_stmt->body);
}
