#include "type.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "tree.h"
#include "symbol.h"

enum expkind {expkind_func, expkind_type, expkind_val};

static int typematch(struct tree_type *i, struct tree_type *j)
{
    if(!i || !j)
        return !i && !j;
    if(i->kind != j->kind)
        return 0;

    switch(i->kind)
    {
        case tree_type_kind_base:
            return i->base == j->base;
        case tree_type_kind_defined:
            return i->defined == j->defined;
        case tree_type_kind_array:
            return i->array.len == j->array.len &&
                typematch(i->array.type, j->array.type);
        case tree_type_kind_slice:
            return typematch(i->slice.type, j->slice.type);
        case tree_type_kind_struct:
            for(struct tree_fields *k = i->structtype.fields,
                    *l = j->structtype.fields;
                k && l; k = k->next, l = l->next)
            {
                if(!k || !l)
                    return 0;
                if(strcmp(k->name, l->name))
                    return 0;
                if(!typematch(k->type, l->type))
                    return 0;
            }
            return 1;
        default:
            return 0;
    }
}

static struct tree_type *rt(struct tree_type *type)
{
    if(type->kind == tree_type_kind_defined)
        return rt(type->defined);
    return type;
}

static int isnumeric(struct tree_type *type)
{
    return type->kind == tree_type_kind_base &&
        (type->base == tree_base_type_int ||
         type->base == tree_base_type_float64 ||
         type->base == tree_base_type_rune);
}

static int isinteger(struct tree_type *type)
{
    struct tree_type *resolve = rt(type);
    return resolve->kind == tree_type_kind_base &&
        (resolve->base == tree_base_type_int ||
         resolve->base == tree_base_type_rune);
}

static int isbool(struct tree_type *type)
{
    return type->kind == tree_type_kind_base &&
        type->base == tree_base_type_bool;
}

static int iscomparable(struct tree_type *type)
{
    if(type->kind == tree_type_kind_base)
        return 1;
    if(type->kind == tree_type_kind_array)
        return iscomparable(type->array.type);
    if(type->kind == tree_type_kind_struct)
    {
        for(struct tree_fields *field = type->structtype.fields; field;
            field = field->next)
            if(!iscomparable(field->type))
                return 0;
        return 1;
    }
    return 0;
}

static int isordered(struct tree_type *type)
{
    return type->kind == tree_type_kind_base &&
        type->base != tree_base_type_bool;
}

static void tc_val(struct tree_exp *exp);

static enum expkind tc_exp(struct tree_exp *exp)
{
    switch(exp->kind)
    {
        case tree_exp_kind_ident:
            switch(exp->ident->symbol->kind)
            {
                case symbol_kind_const:
                    exp->type = exp->ident->symbol->constrec.type;
                    break;
                case symbol_kind_var:
                    exp->type = exp->ident->symbol->type;
                    break;
                case symbol_kind_type:
                    return expkind_type;
                case symbol_kind_func:
                    return expkind_func;
            }
            break;
        case tree_exp_kind_int:
            exp->type = emalloc(sizeof(struct tree_type));
            exp->type->kind = tree_type_kind_base;
            exp->type->base = tree_base_type_int;
            break;
        case tree_exp_kind_float:
            exp->type = emalloc(sizeof(struct tree_type));
            exp->type->kind = tree_type_kind_base;
            exp->type->base = tree_base_type_float64;
            break;
        case tree_exp_kind_rune:
            exp->type = emalloc(sizeof(struct tree_type));
            exp->type->kind = tree_type_kind_base;
            exp->type->base = tree_base_type_rune;
            break;
        case tree_exp_kind_str:
            exp->type = emalloc(sizeof(struct tree_type));
            exp->type->kind = tree_type_kind_base;
            exp->type->base = tree_base_type_str;
            break;
        case tree_exp_kind_unary:
            tc_val(exp->unary.right);
            if(((exp->unary.kind == tree_unaryexp_kind_plus ||
                 exp->unary.kind == tree_unaryexp_kind_minus) &&
                !isnumeric(rt(exp->unary.right->type))) ||
               (exp->unary.kind == tree_unaryexp_kind_not &&
                !isbool(rt(exp->unary.right->type))) ||
               (exp->unary.kind == tree_unaryexp_kind_comp &&
                !isinteger(rt(exp->unary.right->type))))
            {
                fprintf(stderr,
                        "Error: expression on line %d has bad type",
                        exp->unary.right->lineno);
                exit(1);
            }
            exp->type = exp->unary.right->type;
            break;
        case tree_exp_kind_binary:
            tc_val(exp->binary.left);
            tc_val(exp->binary.right);
            if(!typematch(exp->binary.left->type, exp->binary.right->type))
            {
                fprintf(stderr, "Error: type mismatch on line %d\n",
                        exp->lineno);
                exit(1);
            }

            exp->type = emalloc(sizeof(struct tree_type));
            exp->type->kind = tree_type_kind_base;
            struct tree_type *rtexp = rt(exp->binary.left->type);
            int valid = 0;
            switch(exp->binary.kind)
            {
                case tree_binaryexp_kind_or:
                case tree_binaryexp_kind_and:
                    valid = isbool(rtexp);
                    exp->type->base = tree_base_type_bool;
                    break;
                case tree_binaryexp_kind_eq:
                case tree_binaryexp_kind_neq:
                    valid = iscomparable(rtexp);
                    exp->type->base = tree_base_type_bool;
                    break;
                case tree_binaryexp_kind_lt:
                case tree_binaryexp_kind_leq:
                case tree_binaryexp_kind_gt:
                case tree_binaryexp_kind_geq:
                    valid = isordered(rtexp);
                    exp->type->base = tree_base_type_bool;
                    break;
                case tree_binaryexp_kind_plus:
                    valid = isnumeric(rtexp) ||
                        (rtexp->kind == tree_type_kind_base &&
                         rtexp->base == tree_base_type_str);
                    exp->type->base = rtexp->base;
                    break;
                case tree_binaryexp_kind_minus:
                case tree_binaryexp_kind_times:
                case tree_binaryexp_kind_div:
                    valid = isnumeric(rtexp);
                    exp->type->base = rtexp->base;
                    break;
                case tree_binaryexp_kind_bitor:
                case tree_binaryexp_kind_xor:
                case tree_binaryexp_kind_rem:
                case tree_binaryexp_kind_lshift:
                case tree_binaryexp_kind_rshift:
                case tree_binaryexp_kind_bitand:
                case tree_binaryexp_kind_andnot:
                    valid = isinteger(rtexp);
                    exp->type->base = rtexp->base;
                    break;
            }
            if(!valid)
            {
                fprintf(stderr, "Error: bad type for operator on line %d\n",
                        exp->lineno);
                exit(1);
            }
            break;
        case tree_exp_kind_call:
            switch(tc_exp(exp->call.func))
            {
                case expkind_func:;
                    struct tree_params *param =
                        exp->call.func->ident->symbol->func->params;
                    struct tree_exps *arg = exp->call.exps;
                    for(; param && arg; param = param->next, arg = arg->next)
                    {
                        tc_val(arg->exp);
                        if(!typematch(arg->exp->type, param->type))
                        {
                            fprintf(stderr, "Error: argument on line %d has "
                                    "wrong type\n", arg->exp->lineno);
                            exit(1);
                        }
                    }
                    if(arg && !param)
                    {
                        fprintf(stderr,
                                "Error: too many arguments on line %d\n",
                                arg->exp->lineno);
                        exit(1);
                    }
                    if(param && !arg)
                    {
                        fprintf(stderr,
                                "Error: too few arguments on line %d\n",
                                arg->exp->lineno);
                        exit(1);
                    }
                    exp->type = exp->call.func->ident->symbol->func->type;
                    break;
                case expkind_type:;
                    struct tree_type *rt1 =
                        rt(exp->call.func->ident->symbol->type);
                    if(rt1->kind != tree_type_kind_base)
                    {
                        fprintf(stderr, "Error: type on line %d does not "
                                "resolve to a base type\n",
                                exp->call.func->lineno);
                        exit(1);
                    }
                    if(!exp->call.exps || exp->call.exps->next)
                    {
                        fprintf(stderr, "Error: invalid cast on line %d\n",
                                exp->lineno);
                        exit(1);
                    }
                    tc_val(exp->call.exps->exp);
                    struct tree_type *rt2 = rt(exp->call.exps->exp->type);
                    if(!(typematch(rt1, rt2) ||
                         (isnumeric(rt1) && isnumeric(rt2)) ||
                         (rt1->base == tree_base_type_str && isinteger(rt2))))
                    {
                        fprintf(stderr, "Error: incompatible cast on line %d\n",
                                exp->lineno);
                        exit(1);
                    }
                    exp->type = exp->call.func->ident->symbol->type;
                    break;
                default:
                    fprintf(stderr, "Error: expression on line %d is neither a "
                            "function nor a type\n", exp->call.func->lineno);
                    exit(1);
            }
            break;
        case tree_exp_kind_index:
            tc_val(exp->index.arr);
            tc_val(exp->index.index);
            struct tree_type *rtarr = rt(exp->index.arr->type);
            struct tree_type *rtindex = rt(exp->index.index->type);
            if(!(rtindex->kind == tree_type_kind_base &&
                 rtindex->base == tree_base_type_int))
            {
                fprintf(stderr, "Error: bad index type on line %d\n",
                        exp->index.index->lineno);
                exit(1);
            }
            if(rtarr->kind == tree_type_kind_array)
                exp->type = rtarr->array.type;
            else if(rtarr->kind == tree_type_kind_slice)
                exp->type = rtarr->slice.type;
            else
            {
                fprintf(stderr, "Error: indexed expression on line %d is "
                        "neither an array not a slice\n",
                        exp->index.arr->lineno);
                exit(1);
            }
            break;
        case tree_exp_kind_field:
            tc_val(exp->field.instance);
            struct tree_type *rtinstance = rt(exp->field.instance->type);
            if(rtinstance->kind != tree_type_kind_struct)
            {
                fprintf(stderr, "Error: field accessed of expression on line "
                        "%d that is not a struct\n",
                        exp->field.instance->lineno);
                exit(1);
            }
            struct tree_fields *field = rtinstance->structtype.fields;
            for(; field; field = field->next)
                if(strcmp(exp->field.field, field->name) == 0)
                {
                    exp->type = field->type;
                    break;
                }
            if(!field)
            {
                fprintf(stderr, "Error: struct on line %d has no field with "
                        "that name\n", exp->field.instance->lineno);
                exit(1);
            }
            break;
        case tree_exp_kind_append:
            tc_val(exp->append.exp1);
            tc_val(exp->append.exp2);
            struct tree_type *rtslice = rt(exp->append.exp1->type);
            if(!(rtslice->kind == tree_type_kind_slice &&
                 typematch(exp->append.exp2->type, rtslice->slice.type)))
            {
                fprintf(stderr, "Error: bad types for append on line %d\n",
                        exp->lineno);
                exit(1);
            }
            exp->type = exp->append.exp1->type;
            break;
        case tree_exp_kind_len:
            tc_val(exp->exp);
            rtarr = rt(exp->exp->type);
            if(!(rtarr->kind == tree_type_kind_array ||
                 rtarr->kind == tree_type_kind_slice ||
                 (rtarr->kind == tree_type_kind_base &&
                  rtarr->base == tree_base_type_str)))
            {
                fprintf(stderr, "Error: expression on line %d is neither an "
                        "array not a slice\n", exp->index.arr->lineno);
                exit(1);
            }
            exp->type = emalloc(sizeof(struct tree_type));
            exp->type->kind = tree_type_kind_base;
            exp->type->base = tree_base_type_int;
            break;
        case tree_exp_kind_cap:
            tc_val(exp->exp);
            rtarr = rt(exp->exp->type);
            if(!(rtarr->kind == tree_type_kind_array ||
                 rtarr->kind == tree_type_kind_slice))
            {
                fprintf(stderr, "Error: expression on line %d is neither an "
                        "array not a slice\n", exp->index.arr->lineno);
                exit(1);
            }
            exp->type = emalloc(sizeof(struct tree_type));
            exp->type->kind = tree_type_kind_base;
            exp->type->base = tree_base_type_int;
            break;
    }
    return expkind_val;
}

static int isaddressable(struct tree_exp *exp)
{
    if(exp->kind == tree_exp_kind_ident)
        return 1;
    if(exp->kind == tree_exp_kind_index)
        return exp->index.arr->type->kind == tree_type_kind_slice ||
            isaddressable(exp->index.arr);
    if(exp->kind == tree_exp_kind_field)
        return isaddressable(exp->field.instance);
    return 0;
}

static void tc_val(struct tree_exp *exp)
{
    if(tc_exp(exp) != expkind_val)
    {
        fprintf(stderr, "Error: expression on line %d is not a value\n",
                exp->lineno);
        exit(1);
    }
}

static void tc_varspecs(struct tree_var_spec *var_spec);
static void tc_stmts(struct tree_stmts *stmts, struct tree_type *rtype);

static void tc_stmt(struct tree_stmt *stmt, struct tree_type *rtype)
{
    if(!stmt)
        return;

    struct tree_idents *ident;
    struct tree_exps *exp;
    switch(stmt->kind)
    {
        case tree_stmt_kind_exp:
            tc_val(&stmt->expstmt);
            break;
        case tree_stmt_kind_block:
            tc_stmts(stmt->block, rtype);
            break;
        case tree_stmt_kind_assign:
            for(struct tree_exps *left = stmt->assign.left,
                    *right = stmt->assign.right; left;
                left = left->next, right = right->next)
            {
                tc_val(left->exp);
                tc_val(right->exp);
                if(!typematch(left->exp->type, right->exp->type))
                {
                    fprintf(stderr, "Error: type mismatch on line %d\n",
                            right->exp->lineno);
                    exit(1);
                }
                if(!isaddressable(left->exp))
                {
                    fprintf(stderr,
                            "Error: expression on line %d is not addressable\n",
                            left->exp->lineno);
                    exit(1);
                }
            }
            break;
        case tree_stmt_kind_assignop:
            tc_val(stmt->assignop.left);
            tc_val(stmt->assignop.right);
            if(!typematch(stmt->assignop.left->type,
                          stmt->assignop.right->type))
            {
                fprintf(stderr, "Error: type mismatch on line %d\n",
                        stmt->assignop.right->lineno);
                exit(1);
            }
            if(!isaddressable(stmt->assignop.left))
            {
                fprintf(stderr,
                        "Error: expression on line %d is not addressable\n",
                        stmt->assignop.left->lineno);
                exit(1);
            }

            struct tree_type *rtexp = rt(stmt->assignop.left->type);
            int valid = 0;
            switch(stmt->assignop.kind)
            {
                case tree_assignop_kind_plus:
                    valid = isnumeric(rtexp) ||
                        (rtexp->kind == tree_type_kind_base &&
                         rtexp->base == tree_base_type_str);
                    break;
                case tree_assignop_kind_minus:
                case tree_assignop_kind_times:
                case tree_assignop_kind_div:
                    valid = isnumeric(rtexp);
                    break;
                case tree_assignop_kind_or:
                case tree_assignop_kind_xor:
                case tree_assignop_kind_rem:
                case tree_assignop_kind_lshift:
                case tree_assignop_kind_rshift:
                case tree_assignop_kind_and:
                case tree_assignop_kind_andnot:
                    valid = isinteger(rtexp);
                    break;
            }
            if(!valid)
            {
                fprintf(stderr, "Error: bad type for operator on line %d\n",
                        stmt->lineno);
                exit(1);
            }
            break;
        case tree_stmt_kind_shortdecl:
            for(ident = stmt->shortdecl.idents, exp = stmt->shortdecl.exps;
                ident; ident = ident->next, exp = exp->next)
            {
                tc_val(exp->exp);
                if(!ident->ident->symbol->type &&
                   !(ident->ident->symbol->type = exp->exp->type))
                {
                    fprintf(stderr, "Error: expression on line %d with no type "
                            "used in initializer\n", exp->exp->lineno);
                    exit(1);
                }
                else if(!typematch(ident->ident->symbol->type, exp->exp->type))
                {
                    fprintf(stderr, "Error: type mismatch on line %d\n",
                            exp->exp->lineno);
                    exit(1);
                }
            }
            break;
        case tree_stmt_kind_inc:
        case tree_stmt_kind_dec:
            tc_val(stmt->exp);
            if(!isnumeric(rt(stmt->exp->type)))
            {
                fprintf(stderr, "Error: expression on line %d is not numeric",
                        stmt->exp->lineno);
                exit(1);
            }
            break;
        case tree_stmt_kind_var_decl:
            tc_varspecs(stmt->var_spec);
            break;
        case tree_stmt_kind_print:
        case tree_stmt_kind_println:
            for(exp = stmt->exps; exp; exp = exp->next)
            {
                tc_val(exp->exp);
                if(rt(exp->exp->type)->kind != tree_type_kind_base)
                {
                    fprintf(stderr,
                            "Error: expression on line %d is not printable\n",
                            exp->exp->lineno);
                    exit(1);
                }
            }
            break;
        case tree_stmt_kind_return:
            if(stmt->exp)
            {
                tc_val(stmt->exp);
                if(!typematch(stmt->exp->type, rtype))
                {
                    fprintf(stderr, "Error: return type mismatch on line %d\n",
                            stmt->exp->lineno);
                    exit(1);
                }
            }
            if(rtype && !stmt->exp)
            {
                fprintf(stderr, "Error: no value returned on line %d\n",
                        stmt->lineno);
                exit(1);
            }
            break;
        case tree_stmt_kind_if:
            tc_stmt(stmt->ifstmt.init, rtype);
            tc_val(stmt->ifstmt.condition);
            if(!isbool(rt(stmt->ifstmt.condition->type)))
            {
                fprintf(stderr,
                        "Error: condition on line %d has bad type\n",
                        stmt->ifstmt.condition->lineno);
                exit(1);
            }
            tc_stmts(stmt->ifstmt.body, rtype);
            tc_stmts(stmt->ifstmt.elsebody, rtype);
            break;
        case tree_stmt_kind_switch:
            tc_stmt(stmt->switchstmt.init, rtype);
            if(stmt->switchstmt.exp)
            {
                tc_val(stmt->switchstmt.exp);
                if(!iscomparable(rt(stmt->switchstmt.exp->type)))
                {
                    fprintf(stderr, "Error: expression on line %d in switch "
                            "statement is not comparable\n",
                            stmt->switchstmt.exp->lineno);
                    exit(1);
                }
                for(struct tree_cases *cases = stmt->switchstmt.cases; cases;
                    cases = cases->next)
                {
                    for(struct tree_exps *exps = cases->val; exps;
                        exps = exps->next)
                    {
                        tc_val(exps->exp);
                        if(!typematch(exps->exp->type,
                                      stmt->switchstmt.exp->type))
                        {
                            fprintf(stderr,
                                    "Error: case type mismatch on line %d\n",
                                    exps->exp->lineno);
                            exit(1);
                        }
                    }
                    tc_stmts(cases->body, rtype);
                }
            }
            else
            {
                for(struct tree_cases *cases = stmt->switchstmt.cases; cases;
                    cases = cases->next)
                {
                    for(struct tree_exps *exps = cases->val; exps;
                        exps = exps->next)
                    {
                        tc_val(exps->exp);
                        if(!(exps->exp->type->kind == tree_type_kind_base &&
                             exps->exp->type->base == tree_base_type_bool))
                        {
                            fprintf(stderr,
                                    "Error: case type mismatch on line %d\n",
                                    exps->exp->lineno);
                            exit(1);
                        }
                    }
                    tc_stmts(cases->body, rtype);
                }
            }
            break;
        case tree_stmt_kind_for:
            if(stmt->forstmt.condition)
            {
                if(stmt->forstmt.init)
                    tc_stmt(stmt->forstmt.init, rtype);
                tc_val(stmt->forstmt.condition);
                if(!isbool(rt(stmt->forstmt.condition->type)))
                {
                    fprintf(stderr,
                            "Error: condition on line %d has bad type\n",
                            stmt->forstmt.condition->lineno);
                    exit(1);
                }
                if(stmt->forstmt.init)
                    tc_stmt(stmt->forstmt.iter, rtype);
            }
            tc_stmts(stmt->forstmt.body, rtype);
            break;
        case tree_stmt_kind_fallthrough:
        default:
            break;
    }
}

static void tc_stmts(struct tree_stmts *stmts, struct tree_type *rtype)
{
    for(struct tree_stmts *stmt = stmts; stmt; stmt = stmt->next)
        tc_stmt(&stmt->stmt, rtype);
}

static void tc_varspecs(struct tree_var_spec *var_spec)
{
    if(!var_spec)
        return;
    if(var_spec->val)
    {
        tc_val(var_spec->val);
        if(var_spec->type)
        {
            if(!typematch(var_spec->val->type, var_spec->type))
            {
                fprintf(stderr, "Error: type mismatch on line %d\n",
                        var_spec->val->lineno);
                exit(1);
            }
        }
        else
            var_spec->ident->symbol->type = var_spec->val->type;
    }
    tc_varspecs(var_spec->next);
}

void typecheck(struct tree_decls *root)
{
    for(struct tree_decls *node = root; node; node = node->next)
    {
        if(node->kind == tree_decls_kind_var_decl)
            tc_varspecs(node->var_spec);
        else if(node->kind == tree_decls_kind_func_decl)
            tc_stmts(node->func_decl.body, node->func_decl.type);
    }
}
