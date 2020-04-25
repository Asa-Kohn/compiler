#include "python.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "symbol.h"
#include "tree.h"
#include "type.h"

#include "base_py.h"

static void print_indent(int n)
{
    for(int i = 0; i < n; i++)
        printf("    ");
}

static void py_exp(struct tree_exp *exp)
{
    if(exp->kind == tree_exp_kind_ident)
        printf("_%zd", exp->ident->symbol->num);
    else if(exp->kind == tree_exp_kind_int)
        printf("%d", exp->intval);
    else if(exp->kind == tree_exp_kind_float)
        printf("%lf", exp->floatval);
    else if(exp->kind == tree_exp_kind_rune)
        printf("%d", exp->runeval);
    else if(exp->kind == tree_exp_kind_str)
    {
        printf("'");
        for(char *c =  exp->strval; *c; c++)
            printf("\\x%02x", *c);
        printf("'");
    }
    else if(exp->kind == tree_exp_kind_unary)
    {
        if(exp->unary.kind == tree_unaryexp_kind_minus)
            printf("-");
        if(exp->unary.kind == tree_unaryexp_kind_not)
            printf("not ");
        if(exp->unary.kind == tree_unaryexp_kind_comp)
            printf("~");
        printf("(");
        py_exp(exp->unary.right);
        printf(")");
    }
    else if(exp->kind == tree_exp_kind_binary)
    {
        printf("(");
        py_exp(exp->binary.left);
        printf(") ");
        if(exp->binary.kind == tree_binaryexp_kind_or)
            printf("or");
        else if(exp->binary.kind == tree_binaryexp_kind_and)
            printf("and");
        else if(exp->binary.kind == tree_binaryexp_kind_eq)
            printf("==");
        else if(exp->binary.kind == tree_binaryexp_kind_neq)
            printf("!=");
        else if(exp->binary.kind == tree_binaryexp_kind_lt)
            printf("<");
        else if(exp->binary.kind == tree_binaryexp_kind_leq)
            printf("<=");
        else if(exp->binary.kind == tree_binaryexp_kind_gt)
            printf(">");
        else if(exp->binary.kind == tree_binaryexp_kind_geq)
            printf(">=");
        else if(exp->binary.kind == tree_binaryexp_kind_plus)
            printf("+");
        else if(exp->binary.kind == tree_binaryexp_kind_minus)
            printf("-");
        else if(exp->binary.kind == tree_binaryexp_kind_bitor)
            printf("|");
        else if(exp->binary.kind == tree_binaryexp_kind_xor)
            printf("^");
        else if(exp->binary.kind == tree_binaryexp_kind_times)
            printf("*");
        else if(exp->binary.kind == tree_binaryexp_kind_div)
            printf("//");
        else if(exp->binary.kind == tree_binaryexp_kind_rem)
            printf("%%");
        else if(exp->binary.kind == tree_binaryexp_kind_lshift)
            printf("<<");
        else if(exp->binary.kind == tree_binaryexp_kind_rshift)
            printf(">>");
        else if(exp->binary.kind == tree_binaryexp_kind_bitand)
            printf("&");
        else if(exp->binary.kind == tree_binaryexp_kind_andnot)
            printf("and not");
        printf(" (");
        py_exp(exp->binary.right);
        printf(")");
    }
    else if(exp->kind == tree_exp_kind_call)
    {
        if(exp->call.func->ident->symbol->kind == symbol_kind_func)
        {
            py_exp(exp->call.func);
            printf("(");
            for(struct tree_exps *exps = exp->call.exps; exps;
                exps = exps->next)
            {
                py_exp(exps->exp);
                printf(", ");
            }
            printf(")");
        }
        else
        {
            struct tree_type *type = rt(exp->call.func->ident->symbol->type);
            if(type->kind == tree_type_kind_base &&
               type->base == tree_base_type_str)
            {
                printf("chr(");
                py_exp(exp->call.exps->exp);
                printf(")");
            }
            else
                py_exp(exp->call.exps->exp);
        }
    }
    else if(exp->kind == tree_exp_kind_index)
    {
        py_exp(exp->index.arr);
        printf("[");
        py_exp(exp->index.index);
        printf("]");
    }
    else if(exp->kind == tree_exp_kind_field)
    {
        py_exp(exp->field.instance);
        printf("['%s']", exp->field.field);
    }
    else if(exp->kind == tree_exp_kind_append)
    {
        printf("(");
        py_exp(exp->append.exp1);
        printf(").add(");
        py_exp(exp->append.exp2);
        printf(")");
    }
    else if(exp->kind == tree_exp_kind_len)
    {
        printf("len(");
        py_exp(exp->exp);
        printf(")");
    }
    else if(exp->kind == tree_exp_kind_cap)
    {
        printf("(");
        py_exp(exp->exp);
        printf(").capacity");
    }
}

static void py_stmts(struct tree_stmts *stmts, int indent,
                     struct tree_stmt *continuestmt);
static void py_varspec(struct tree_var_spec *node, int indent);

static void py_stmt(struct tree_stmt *stmt, int indent,
                    struct tree_stmt *continuestmt)
{
    if(!stmt)
        return;

    if(stmt->kind == tree_stmt_kind_exp)
    {
        print_indent(indent);
        py_exp(&stmt->expstmt);
        printf("\n");
    }
    else if(stmt->kind == tree_stmt_kind_block)
        py_stmts(stmt->block, indent, continuestmt);
    else if(stmt->kind == tree_stmt_kind_assign)
    {
        print_indent(indent);
        for(struct tree_exps *exp = stmt->assign.left; exp; exp = exp->next)
        {
            py_exp(exp->exp);
            printf(", ");
        }
        printf("= ");
        for(struct tree_exps *exp = stmt->assign.right; exp; exp = exp->next)
        {
            py_exp(exp->exp);
            printf(", ");
        }
        printf("\n");
    }
    else if(stmt->kind == tree_stmt_kind_assignop)
    {
        print_indent(indent);
        py_exp(stmt->assignop.left);
        printf(" ");
        if(stmt->assignop.kind == tree_assignop_kind_plus)
            printf("+");
        else if(stmt->assignop.kind == tree_assignop_kind_minus)
            printf("-");
        else if(stmt->assignop.kind == tree_assignop_kind_or)
            printf("|");
        else if(stmt->assignop.kind == tree_assignop_kind_xor)
            printf("^");
        else if(stmt->assignop.kind == tree_assignop_kind_times)
            printf("*");
        else if(stmt->assignop.kind == tree_assignop_kind_div)
            printf("//");
        else if(stmt->assignop.kind == tree_assignop_kind_rem)
            printf("%%");
        else if(stmt->assignop.kind == tree_assignop_kind_lshift)
            printf("<<");
        else if(stmt->assignop.kind == tree_assignop_kind_rshift)
            printf(">>");
        else if(stmt->assignop.kind == tree_assignop_kind_rshift)
            printf("&");
        else if(stmt->assignop.kind == tree_assignop_kind_rshift)
            printf("&");
        printf("= ");
        if(stmt->assignop.kind == tree_assignop_kind_andnot)
            printf("not ");
        py_exp(stmt->assignop.right);
        printf("\n");
    }
    else if(stmt->kind == tree_stmt_kind_shortdecl)
    {
        struct tree_idents *ident;
        struct tree_exps *exp;
        for(ident = stmt->shortdecl.idents, exp = stmt->shortdecl.exps; ident;
            ident = ident->next, exp = exp->next)
            if(!ident->ident->symbol)
            {
                print_indent(indent);
                py_exp(exp->exp);
                printf("\n");
            }
        print_indent(indent);
        for(ident = stmt->shortdecl.idents; ident; ident = ident->next)
            if(ident->ident->symbol)
                printf("_%zd, ", ident->ident->symbol->num);
        printf("= ");
        for(exp = stmt->shortdecl.exps, ident = stmt->shortdecl.idents; exp;
            exp = exp->next, ident = ident->next)
            if(ident->ident->symbol)
            {
                py_exp(exp->exp);
                printf(", ");
            }
        printf("\n");
    }
    else if(stmt->kind == tree_stmt_kind_inc)
    {
        print_indent(indent);
        py_exp(stmt->exp);
        printf(" += 1\n");
    }
    else if(stmt->kind == tree_stmt_kind_dec)
    {
        print_indent(indent);
        py_exp(stmt->exp);
        printf(" -= 1\n");
    }
    else if(stmt->kind == tree_stmt_kind_var_decl)
        py_varspec(stmt->var_spec, indent);
    else if(stmt->kind == tree_stmt_kind_print)
    {
        print_indent(indent);
        printf("print(");
        for(struct tree_exps *exp = stmt->exps; exp; exp = exp->next)
        {
            printf("format(");
            if(exp->exp->type->kind == tree_type_kind_base &&
               exp->exp->type->base == tree_base_type_bool)
            {
                printf("bool(");
                py_exp(exp->exp);
                printf(")");
            }
            else
                py_exp(exp->exp);
            printf(")");
            printf(", ");
        }
        printf("sep='', end='')\n");
    }
    else if(stmt->kind == tree_stmt_kind_println)
    {
        print_indent(indent);
        printf("print(");
        for(struct tree_exps *exp = stmt->exps; exp; exp = exp->next)
        {
            printf("format(");
            if(exp->exp->type->kind == tree_type_kind_base &&
               exp->exp->type->base == tree_base_type_bool)
            {
                printf("bool(");
                py_exp(exp->exp);
                printf(")");
            }
            else
                py_exp(exp->exp);
            printf(")");
            printf(", ");
        }
        printf(")\n");
    }
    else if(stmt->kind == tree_stmt_kind_return)
    {
        print_indent(indent);
        printf("return ");
        py_exp(stmt->exp);
        printf("\n");
    }
    else if(stmt->kind == tree_stmt_kind_if)
    {
        py_stmt(stmt->ifstmt.init, indent, continuestmt);
        print_indent(indent);
        printf("if ");
        py_exp(stmt->ifstmt.condition);
        printf(":\n");
        py_stmts(stmt->ifstmt.body, indent + 1, continuestmt);
        print_indent(indent);
        printf("else:\n");
        py_stmts(stmt->ifstmt.elsebody, indent + 1, continuestmt);
    }
    else if(stmt->kind == tree_stmt_kind_switch)
    {
        py_stmt(stmt->switchstmt.init, indent, continuestmt);
        print_indent(indent);
        printf("switchvals.append(");
        py_exp(stmt->switchstmt.exp);
        printf(")\n");
        struct tree_cases *cases = stmt->switchstmt.cases;
        if(cases)
        {
            struct tree_stmts *defaultbody = NULL;
            if(!cases->val)
            {
                defaultbody = cases->body;
                cases = cases->next;
            }
            print_indent(indent);
            printf("if switchvals[-1] in (");
            for(struct tree_exps *exp = cases->val; exp; exp = exp->next)
            {
                py_exp(exp->exp);
                printf(", ");
            }
            printf("):\n");
            py_stmts(cases->body, indent + 1, continuestmt);

            for(; cases; cases = cases->next)
            {
                print_indent(indent);
                printf("elif switchvals[-1] in (");
                for(struct tree_exps *exp = cases->val; exp; exp = exp->next)
                {
                    py_exp(exp->exp);
                    printf(", ");
                }
                printf("):\n");
                py_stmts(cases->body, indent + 1, continuestmt);
            }
            if(!stmt->switchstmt.cases->val && !stmt->switchstmt.cases->next)
                py_stmts(defaultbody, indent, continuestmt);
            else
            {
                print_indent(indent);
                printf("else:\n");
                py_stmts(defaultbody, indent + 1, continuestmt);
            }
        }
        print_indent(indent);
        printf("switchvals.pop()\n");
    }
    else if(stmt->kind == tree_stmt_kind_for)
    {
        py_stmt(stmt->forstmt.init, indent, continuestmt);
        print_indent(indent);
        printf("while ");
        py_exp(stmt->forstmt.condition);
        printf(":\n");
        py_stmts(stmt->forstmt.body, indent + 1, stmt->forstmt.iter);
        py_stmt(stmt->forstmt.iter, indent + 1, NULL);
    }
    else if(stmt->kind == tree_stmt_kind_break)
    {
        print_indent(indent);
        printf("break\n");
    }
    else if(stmt->kind == tree_stmt_kind_continue)
    {
        py_stmt(continuestmt, indent, NULL);
        print_indent(indent);
        printf("continue\n");
    }
}

static void py_stmts(struct tree_stmts *stmts, int indent,
                     struct tree_stmt *continuestmt)
{
    if(!stmts)
    {
        print_indent(indent);
        printf("pass\n");
    }
    else
        for(struct tree_stmts *stmt = stmts; stmt; stmt = stmt->next)
            py_stmt(&stmt->stmt, indent, continuestmt);
}

static void py_zerovalue(struct tree_type *type)
{
    if(type->kind == tree_type_kind_base)
    {
        if(type->base == tree_base_type_int ||
           type->base == tree_base_type_rune)
            printf("0");
        else if(type->base == tree_base_type_float64)
            printf("0.");
        else if(type->base == tree_base_type_bool)
            printf("False");
        else if(type->base == tree_base_type_str)
            printf("''");
    }
    else if(type->kind == tree_type_kind_array)
    {
        printf("Array([");
        for(int i = 0; i < type->array.len - 1; i++)
        {
            py_zerovalue(type->array.type);
            printf(", ");
        }
        py_zerovalue(type->array.type);
        printf("])");
    }
    else if(type->kind == tree_type_kind_slice)
        printf("Slice()");
    else if(type->kind == tree_type_kind_struct)
    {
        printf("{");
        for(struct tree_fields *c = type->structtype.fields; c; c = c->next)
        {
            printf("'%s': ", c->name);
            py_zerovalue(c->type);
            printf(", ");
        }
        printf("}");
    }
}

static void py_varspec(struct tree_var_spec *node, int indent)
{
    if(!node)
        return;

    if(!node->ident->symbol)
        return;
    print_indent(indent);
    printf("_%zd = ", node->ident->symbol->num);
    if(node->val)
        py_exp(node->val);
    else
        py_zerovalue(rt(node->type));
    printf("\n");
    py_varspec(node->next, indent);
}

static void py_funcdecl(struct tree_func_decl node, int indent)
{
    print_indent(indent);
    printf("def _%zd(", node.ident->symbol->num);
    for(struct tree_params *param = node.params; param; param = param->next)
        printf("_%zd, ", param->ident->symbol->num);
    printf("):\n");
    py_stmts(node.body, indent + 1, NULL);
}

void py_program(struct tree_decls *root, struct symbol_rec *symbols)
{
    printf("%.*s", base_py_len, base_py);

    size_t mainfunc = 0;
    for(size_t i = 0; symbols[i].name; i++)
        if(symbols[i].kind == symbol_kind_const)
            printf("_%zd = %d\n", symbols[i].num, symbols[i].constrec.constval);
        else if(symbols[i].kind == symbol_kind_func &&
                strcmp(symbols[i].name, "main") == 0)
            mainfunc = symbols[i].num;

    for(struct tree_decls *c = root; c; c = c->next)
        if(c->kind == tree_decls_kind_var_decl)
            py_varspec(c->var_spec, 0);
        else if(c->kind == tree_decls_kind_func_decl &&
                c->func_decl.ident->symbol)
        {
            if(strcmp(c->func_decl.ident->name, "init") == 0)
                py_stmts(c->func_decl.body, 0, NULL);
            else
                py_funcdecl(c->func_decl, 0);
        }

    printf("\n_%zd()\n", mainfunc);
}
