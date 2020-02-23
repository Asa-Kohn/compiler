#ifndef WEED_H
#define WEED_H
#include "tree.h"

int weed_func_decl(FUNC_DECL *func_decl);
int weed_decls(DECLS *decls);
int weed_type_array(TYPE_ARRAY *type_array);
int weed_type_slice(TYPE_SLICE *type_slice);
int weed_type_struct(TYPE_STRUCT *type_struct);
int weed_type(TYPE *type);
int weed_vars(VARS *vars);
int weed_unaryexp(UNARYEXP *unarexp);
int weed_binaryexp(BINARYEXP *binaryexp);
int weed_call(CALL *call);
int weed_index(INDEX *index);
int weed_field(FIELD *field);
int weed_append(APPEND *append);

int weed_cases(CASES *cases);
int weed_stmts(STMTS *stmts);
int weed_stmt(STMT *stmt);
int weed_assign(ASSIGN *assign);
int weed_shortdecl(SHORTDECL *shortdecl);
// int weed_var_decl(VAR_DECL *var_decl);
int weed_var_spec(VAR_SPEC *var_spec);
int weed_type_spec(TYPE_SPEC *type_spec);
int weed_if(IF_STMT *if_stmt);
int weed_switch(SWITCH_STMT *switch_stmt);
int weed_for(FOR_STMT *for_stmt);

int weed_exps(EXPS *exps);
int weed_exp(EXP *exp); 

#endif