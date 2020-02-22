#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "weed.h"

//stmts_kind
void weed_stmt(STMT *stmt) {
    if(stmt == NULL) return 0;
    switch (stmt->kind) {
        case tree_stmt_kind_exp:
        case tree_stmt_kind_assign:
            break;
        case tree_stmt_kind_shortdecl:
            weed_shortdecl(stmt->shortdecl);
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
    weed_stmts(stmt->next);
}

//decls_kind
void weed_decls(DECLS *decls) {
    if(decls == NULL) return 0;
    switch (decls->kind) {
        //TODO
        case tree_decls_kind_var_decl:
            weed_var_decl(decls->var_decl);
        case tree_decls_kind_type_spec:
            weed_type_spec(decls->type_spec);
        case tree_decls_kind_func_decl:
            weed_func_decl(decls->func_decl);
    }
    weed_decls(decls->next);
}

void weed_vars(VARS *vars){
    if(vars == NULL) return 0;
    weed_type(vars->type);
    weed_vars(vars->next);
}

void weed_var_decl(VAR_DECL *var_decl) {
    if(var_decl == NULL) return 0;
    weed_type(var_decl->type);
    weed_exp(var_decl->val);
    weed_var_decl(var_decl->next);
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


void weed_type_array(TYPE_ARRAY *type_array) {
    if(type_array == NULL) return 0;
    weed_type(type_array->type);
}

void weed_type_slice(TYPE_SLICE *type_slice) {
    if(type_slice == NULL) return 0;
    weed_type(type_slice->type);
}

void weed_type_struct(TYPE_STRUCT *type_struct) {
    if(type_struct == NULL) return 0;
    weed_vars(type_struct->fields);
}

void weed_call(CALL *call) {
    if(call == NULL) return 0;
    weed_exp(call->func);
    weed_exps(call->exps);
}

void weed_index(INDEX *index) {
    if(index == NULL) return 0;
    weed_exp(index->arr);
    weed_exp(index->index);
}

void weed_field(FIELD *field) {
    if(field == NULL) return 0;
    weed_exp(field->instance);
}

void weed_append(APPEND *append){
    if(append == NULL) return 0;
    weed_exp(append->exp1);
    weed_exp(append->exp2);
}

void weed_cases(CASES *cases) {
    if(cases == NULL) return 0;
    weed_exp(cases->val);
    weed_stmts(cases->body);
    weed_cases(cases->next);
}

void weed_assign(ASSIGN *assign) {
    if(assign == NULL) return 0;
    weed_exp(assign->exp);
}

void weed_shortdecl(SHORTDECL *shortdecl) {
    if(shortdecl == NULL) return 0;
        // fprintf(stderr, "Error: short declaration (line%d)", exp->lineno);
        // exit(1);
        return 1;
    // weed_exp(shortdecl->exp);
}

void weed_if(IF_STMT *if_stmt) {
    if(if_stmt == NULL) return 0;
    weed_stmts(if_stmt->init);
    weed_exp(if_stmt->condition);
    weed_stmts(if_stmt->body);
    weed_stmts(if_stmt->elsebody);
}

void weed_switch(SWITCH_STMT *switch_stmt) {
    if(switch_stmt == NULL) return 0;
    weed_stmts(switch_stmt->init);
    weed_exp(switch_stmt->exp);
    weed_cases(switch_stmt->cases);
}

void weed_for(FOR_STMT *for_stmt) {
    if(for_stmt == NULL) return 0;
    weed_stmts(for_stmt->init);
    weed_exp(for_stmt->condition);
    weed_stmts(for_stmt->iter);
    weed_stmts(for_stmt->body);
}
