#ifndef WEED_H
#define WEED_H
#include "tree.h"

void weed_func_decl(FUNC_DECL *func_decl);
void weed_decls(DECLS *decls);
void weed_type_array(TYPE_ARRAY *type_array);
void weed_type_slice(TYPE_SLICE *type_slice);
void weed_type_struct(TYPE_STRUCT *type_struct);
void weed_type(TYPE *type);
void weed_vars(VARS *vars);
void weed_unaryexp(UNARYEXP *unarexp);
void weed_binaryexp(BINARYEXP *binaryexp);
void weed_call(CALL *call);
void weed_index(INDEX *index);
void weed_field(FIELD *field);
void weed_append(APPEND *append);

void weed_cases(CASES *cases);
void weed_stmt(STMT *stmt);
void weed_assign(ASSIGN *assign);
void weed_shortdecl(SHORTDECL *shortdecl);
void weed_var_decl(VAR_DECL *var_decl);
void weed_type_spec(TYPE_SPEC *type_spec);
void weed_if(IF_STMT *if_stmt);
void weed_switch(SWITCH_STMT *switch_stmt);
void weed_for(FOR_STMT *for_stmt);

void weed_exps(EXPS *exps);
void weed_exp(EXP *exp); 

#endif