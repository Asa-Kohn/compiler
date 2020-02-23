#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "weed.h"
#include <string.h>

//stmts_kind
void weed_bc_stmts(STMTS *stmts) {
    if(stmts == NULL) return;
    switch (stmts->kind) {
        case tree_stmt_kind_break:
            fprintf(stderr, "Error: invalid break statement (line %d)", stmts->lineno);
            exit(1); 
        case tree_stmt_kind_continue:
            fprintf(stderr, "Error: invalid continue statement (line %d)", stmts->lineno);
            exit(1); 
    weed_stmts(stmts->next);
}

//stmt_kind
void weed_stmt_for(STMT *stmt) {
    if(stmt == NULL) return;
    switch (stmt->kind) {
        case tree_stmt_kind_shortdecl:
            weed_shortdecl(&(stmt->shortdecl));
            return;
            
        case tree_stmt_kind_break:
            // fprintf(stderr, "Error: break statement (line %d)", stmt->lineno);
            // exit(1);
            return;

        case tree_stmt_kind_continue:
            // fprintf(stderr, "Error: continue statement (line %d)", stmt->lineno);
            // exit(1);
            return;

        default:
            return;
    }
    return;
}
//stmt_kind_switch
void weed_stmt_switch(STMT *stmt) {
    if(stmt == NULL) return;
    switch (stmt->kind) {
        case tree_stmt_kind_shortdecl:
            weed_shortdecl(&(stmt->shortdecl));
            return;
            
        case tree_stmt_kind_break:
            // fprintf(stderr, "Error: break statement (line %d)", stmt->lineno);
            // exit(1);
            return;

        case tree_stmt_kind_continue:
            fprintf(stderr, "Error: continue statement (line %d)", stmt->lineno);
            exit(1);

        default:
            return;
    }
    return;
}
//stmt_kind
void weed_stmt(STMT *stmt) {
    if(stmt == NULL) return;
    switch (stmt->kind) {
        case tree_stmt_kind_shortdecl:
            weed_shortdecl(&(stmt->shortdecl));
            return;
            
        case tree_stmt_kind_break:
            fprintf(stderr, "Error: break statement (line %d)", stmt->lineno);
            exit(1);

        case tree_stmt_kind_continue:
            fprintf(stderr, "Error: continue statement (line %d)", stmt->lineno);
            exit(1);

        default:
            return;
    }
    return;
}

//decls_kind
void weed_decls(DECLS *decls) {
    if(decls == NULL) return;
    switch (decls->kind) {
        //TODO
        case tree_decls_kind_var_decl:
            weed_var_decl(decls->var_spec);
            break;
        case tree_decls_kind_type_spec:
            weed_type_spec(&(decls->type_spec));
            break;
        case tree_decls_kind_func_decl:
            weed_func_decl(&(decls->func_decl));
            break;
        default:
            break;
    }
    if(decls->next != NULL) weed_decls(decls->next);
    return;
}

void weed_vars(VARS *vars){
    if(vars == NULL) return;
    weed_type(vars->type);
    if(vars->next != NULL) weed_vars(vars->next);
}

void weed_var_spec(VAR_SPEC *var_spec) {
    if(var_spec == NULL) return;
    weed_type(var_spec->type);
    weed_exp(var_spec->val);
    if(var_spec->next != NULL) weed_var_spec(var_spec->next);
}

//TODO
void weed_type_spec(TYPE_SPEC *type_spec) {
    // to do
}

void weed_func_decl(FUNC_DECL *func_decl){
    if(func_decl == NULL) return;
    weed_vars(func_decl->params);
    weed_type(func_decl->type);
    weed_stmts(func_decl->body);
}

//type_kind
void weed_type(TYPE *type) {
    if(type == NULL) return;
    switch (type->kind) {
        case tree_type_kind_name:
            break;
        case tree_type_kind_array:
            break;
        case tree_type_kind_slice:
            break;
        case tree_type_kind_struct:
            break;
    }
    return;
}

//binaryexp_kind
void weed_binaryexp(BINARYEXP *binaryexp){
    if(binaryexp == NULL) return 0;
    switch (binaryexp->kind) {
        // case tree_binaryexp_kind_or:
        // case tree_binaryexp_kind_and:
        // case tree_binaryexp_kind_eq:
        // case tree_binaryexp_kind_neq:
        // case tree_binaryexp_kind_lt:
        // case tree_binaryexp_kind_leq:
        // case tree_binaryexp_kind_gt:
        // case tree_binaryexp_kind_geq:
        // case tree_binaryexp_kind_plus:
        // case tree_binaryexp_kind_minus:
        // case tree_binaryexp_kind_bitor:
        // case tree_binaryexp_kind_xor:
        // case tree_binaryexp_kind_times:
        // case tree_binaryexp_kind_div:
        // case tree_binaryexp_kind_rem:
        // case tree_binaryexp_kind_lshift:
        // case tree_binaryexp_kind_rshift:
        // case tree_binaryexp_kind_bitand:
        // case tree_binaryexp_kind_andnot:
        default:
            weed_exp(binaryexp->left);
            weed_exp(binaryexp->right);
            break;
    }
    return;
}

void weed_exps(EXPS *exps) {
    if(exps == NULL) return;
    weed_exp(exps->exp);
    if(exps->next != NULL) weed_exps(exps->next);
}

//exp_kind
void weed_exp(EXP *exp) {
    if(exp == NULL) return;
    switch (exp->kind) {
        case tree_exp_kind_ident:
            if(strcmp(exp->ident, "_") == 0){
                fprintf(stderr, "Error: blank identifier (line %d)", exp->lineno);
                exit(1);
            }
        case tree_exp_kind_int:
            break;

        case tree_exp_kind_float:
            break;

        case tree_exp_kind_rune:
            break;

        case tree_exp_kind_str:
            break;

        case tree_exp_kind_unary:
            weed_unaryexp(&(exp->unary));
            break;

        case tree_exp_kind_binary:
            weed_binary(&(exp->binary));
            break;

        case tree_exp_kind_call:
            weed_call(&(exp->call));
            break;

        case tree_exp_kind_index:
            weed_index(&(exp->index));
            break;

        case tree_exp_kind_field:
            weed_index(&(exp->index));
            break;

        case tree_exp_kind_append:
            weed_append(&(exp->append));
            break;

        case tree_exp_kind_len:
        case tree_exp_kind_cap:
            break;
    }
}

//unaryexp_kind
void weed_unaryexp(UNARYEXP *unaryexp) {
    if(unaryexp == NULL) return;
    switch (unaryexp->kind) {
        // case tree_unaryexp_kind_plus:
        // case tree_unaryexp_kind_minus:
        // case tree_unaryexp_kind_not:
        // case tree_unaryexp_kind_comp:
        default:
            weed_exp(unaryexp->right);
            break;
    }
}


void weed_type_array(TYPE_ARRAY *type_array) {
    if(type_array == NULL) return;
    weed_type(type_array->type);
}

void weed_type_slice(TYPE_SLICE *type_slice) {
    if(type_slice == NULL) return;
    weed_type(type_slice->type);
}

void weed_type_struct(TYPE_STRUCT *type_struct) {
    if(type_struct == NULL) return;
    weed_vars(type_struct->fields);
}

void weed_call(CALL *call) {
    if(call == NULL) return;
    weed_exp(call->func);
    weed_exps(call->exps);
}

void weed_index(INDEX *index) {
    if(index == NULL) return;
    weed_exp(index->arr);
    weed_exp(index->index);
}

void weed_field(FIELD *field) {
    if(field == NULL) return;
    weed_exp(field->instance);
}

void weed_append(APPEND *append){
    if(append == NULL) return;
    weed_exp(append->exp1);
    weed_exp(append->exp2);
}

void weed_cases(CASES *cases) {
    if(cases == NULL) return;
    weed_exp(cases->val);
    weed_stmts(cases->body);
    weed_cases(cases->next);
}

void weed_assignop(ASSIGNOP *assignop) {
    if(assignop == NULL) return;
    switch (assignop->kind) {
        case tree_assignop_kind_plus:
        case tree_assignop_kind_minus:
        case tree_assignop_kind_or:
        case tree_assignop_kind_xor:
        case tree_assignop_kind_times:
        case tree_assignop_kind_div:
        case tree_assignop_kind_rem:
        case tree_assignop_kind_lshift:
        case tree_assignop_kind_rshift:
        case tree_assignop_kind_and:
        case tree_assignop_kind_andnot:
            break;
    }
    weed_exp(assignop->left);
    weed_exp(assignop->right);
}

void weed_assign(ASSIGN *assign) {
    if(assign == NULL) return;
    weed_exps(assign->left);
    weed_exps(assign->right);
}

void weed_shortdecl(SHORTDECL *shortdecl) {
    if(shortdecl == NULL) return;
    // fprintf(stderr, "Error: short declaration (line %d)", exp->lineno);
    // exit(1);
    // weed_exp(shortdecl->exp);
    return;
}

void weed_if(IF_STMT *if_stmt) {
    if(if_stmt == NULL) return;
    weed_stmts(if_stmt->init);
    weed_exp(if_stmt->condition);
    weed_stmts(if_stmt->body);
    weed_stmts(if_stmt->elsebody);
}

void weed_switch(SWITCH_STMT *switch_stmt) {
    if(switch_stmt == NULL) return;
    weed_stmts(switch_stmt->init);
    weed_exp(switch_stmt->exp);
    weed_cases(switch_stmt->cases);
}

void weed_for(FOR_STMT *for_stmt) {
    if(for_stmt == NULL) return;
    weed_stmts(for_stmt->init);
    weed_exp(for_stmt->condition);
    weed_stmts(for_stmt->iter);
    weed_stmts(for_stmt->body);
}
