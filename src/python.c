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

static void py_exp(struct tree_exp *exp, int copy)
{
    if(!exp)
        printf("True");
    else if(exp->kind == tree_exp_kind_ident)
    {
        if(copy && rt(exp->type)->kind != tree_type_kind_base)
            printf("deepcopy(");
        if(exp->ident->symbol->scope == symbol_scope_global)
            printf("symbols._%zd", exp->ident->symbol->num);
        else
            printf("_%zd", exp->ident->symbol->num);
        if(copy && rt(exp->type)->kind != tree_type_kind_base)
            printf(")");
    }
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
        py_exp(exp->unary.right, 0);
        printf(")");
    }
    else if(exp->kind == tree_exp_kind_binary)
    {
        printf("(");
        py_exp(exp->binary.left, 0);
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
        {
            if(isinteger(rt(exp->binary.left->type)))
                printf("//");
            else
                printf("/");
        }
        else if(exp->binary.kind == tree_binaryexp_kind_rem)
            printf("%%");
        else if(exp->binary.kind == tree_binaryexp_kind_lshift)
            printf("<<");
        else if(exp->binary.kind == tree_binaryexp_kind_rshift)
            printf(">>");
        else if(exp->binary.kind == tree_binaryexp_kind_bitand)
            printf("&");
        else if(exp->binary.kind == tree_binaryexp_kind_andnot)
            printf("&~");
        printf(" (");
        py_exp(exp->binary.right, 0);
        printf(")");
    }
    else if(exp->kind == tree_exp_kind_call)
    {
        if(exp->call.func->ident->symbol->kind == symbol_kind_func)
        {
            if(copy && rt(exp->type)->kind != tree_type_kind_base)
                printf("deepcopy(");
            py_exp(exp->call.func, 0);
            printf("(");
            for(struct tree_exps *exps = exp->call.exps; exps;
                exps = exps->next)
            {
                py_exp(exps->exp, 1);
                printf(", ");
            }
            printf(")");
            if(copy && rt(exp->type)->kind != tree_type_kind_base)
                printf(")");
        }
        else
        {
            struct tree_type *type = rt(exp->call.func->ident->symbol->type);
            if(type->kind == tree_type_kind_base)
            {
                if(type->base == tree_base_type_str &&
                   isinteger(rt(exp->call.exps->exp->type)))
                {
                    printf("chr(");
                    py_exp(exp->call.exps->exp, 0);
                    printf(")");
                }
                else if(type->base == tree_base_type_float64)
                {
                    printf("float(");
                    py_exp(exp->call.exps->exp, 0);
                    printf(")");
                }
                else
                    py_exp(exp->call.exps->exp, copy);
            }
            else
                py_exp(exp->call.exps->exp, copy);
        }
    }
    else if(exp->kind == tree_exp_kind_index)
    {
        if(copy && rt(exp->type)->kind != tree_type_kind_base)
            printf("deepcopy(");
        py_exp(exp->index.arr, 0);
        printf("[");
        py_exp(exp->index.index, 0);
        printf("]");
        if(copy && rt(exp->type)->kind != tree_type_kind_base)
            printf(")");
    }
    else if(exp->kind == tree_exp_kind_field)
    {
        py_exp(exp->field.instance, copy);
        printf("['%s']", exp->field.field);
    }
    else if(exp->kind == tree_exp_kind_append)
    {
        printf("(");
        py_exp(exp->append.exp1, 1);
        printf(").add(");
        py_exp(exp->append.exp2, 1);
        printf(")");
    }
    else if(exp->kind == tree_exp_kind_len)
    {
        printf("len(");
        py_exp(exp->exp, 0);
        printf(")");
    }
    else if(exp->kind == tree_exp_kind_cap)
    {
        if(rt(exp->exp->type)->kind == tree_type_kind_array)
            printf("len");
        printf("(");
        py_exp(exp->exp, 0);
        printf(")");
        if(rt(exp->exp->type)->kind == tree_type_kind_slice)
            printf(".capacity");
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
        py_exp(&stmt->expstmt, 0);
        printf("\n");
    }
    else if(stmt->kind == tree_stmt_kind_block)
        py_stmts(stmt->block, indent, continuestmt);
    else if(stmt->kind == tree_stmt_kind_assign)
    {
        struct tree_exps *left, *right;
        int nonblank = 0;
        for(left = stmt->assign.left, right = stmt->assign.right; left;
            left = left->next, right = right->next)
            if(left->exp->kind == tree_exp_kind_ident &&
               !left->exp->ident->symbol)
            {
                print_indent(indent);
                py_exp(right->exp, 0);
                printf("\n");
            }
            else
                nonblank = 1;
        if(nonblank)
        {
            print_indent(indent);
            for(left = stmt->assign.left; left; left = left->next)
                if(!(left->exp->kind == tree_exp_kind_ident &&
                     !left->exp->ident->symbol))
                {
                    py_exp(left->exp, 0);
                    if(left->next)
                        printf(", ");
                }
            printf("= ");
            for(right = stmt->assign.right, left = stmt->assign.left; right;
                right = right->next, left = left->next)
                if(!(left->exp->kind == tree_exp_kind_ident &&
                     !left->exp->ident->symbol))
                {
                    py_exp(right->exp, 1);
                    if(left->next)
                        printf(", ");
                }
            printf("\n");
        }
    }
    else if(stmt->kind == tree_stmt_kind_assignop)
    {
        print_indent(indent);
        py_exp(stmt->assignop.left, 0);
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
        py_exp(stmt->assignop.right, 1);
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
                py_exp(exp->exp, 0);
                printf("\n");
            }
        print_indent(indent);
        for(ident = stmt->shortdecl.idents; ident; ident = ident->next)
            if(ident->ident->symbol)
            {
                if(ident->ident->symbol->scope == symbol_scope_global)
                    printf("symbols._%zd", ident->ident->symbol->num);
                else
                    printf("_%zd", ident->ident->symbol->num);
                if(ident->next)
                    printf(", ");
            }
        printf("= ");
        for(exp = stmt->shortdecl.exps, ident = stmt->shortdecl.idents; exp;
            exp = exp->next, ident = ident->next)
            if(ident->ident->symbol)
            {
                py_exp(exp->exp, 1);
                if(ident->next)
                    printf(", ");
            }
        printf("\n");
    }
    else if(stmt->kind == tree_stmt_kind_inc)
    {
        print_indent(indent);
        py_exp(stmt->exp, 0);
        printf(" += 1\n");
    }
    else if(stmt->kind == tree_stmt_kind_dec)
    {
        print_indent(indent);
        py_exp(stmt->exp, 0);
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
            if(isbool(rt(exp->exp->type)))
            {
                printf("bool(");
                py_exp(exp->exp, 0);
                printf(")");
            }
            else
                py_exp(exp->exp, 0);
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
                py_exp(exp->exp, 0);
                printf(")");
            }
            else
                py_exp(exp->exp, 0);
            printf(")");
            printf(", ");
        }
        printf(")\n");
    }
    else if(stmt->kind == tree_stmt_kind_return)
    {
        print_indent(indent);
        printf("return ");
        py_exp(stmt->exp, 1);
        printf("\n");
    }
    else if(stmt->kind == tree_stmt_kind_if)
    {
        py_stmt(stmt->ifstmt.init, indent, continuestmt);
        print_indent(indent);
        printf("if ");
        py_exp(stmt->ifstmt.condition, 0);
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
        py_exp(stmt->switchstmt.exp, 0);
        printf(")\n");
        print_indent(indent);
        printf("while True:\n");
        struct tree_cases *cases = stmt->switchstmt.cases;
        if(cases)
        {
            struct tree_stmts *defaultbody = NULL;
            if(!cases->val)
            {
                defaultbody = cases->body;
                cases = cases->next;
            }
            if(cases)
            {
                print_indent(indent + 1);
                printf("if switchvals[-1] in (");
                for(struct tree_exps *exp = cases->val; exp; exp = exp->next)
                {
                    py_exp(exp->exp, 0);
                    printf(", ");
                }
                printf("):\n");
                py_stmts(cases->body, indent + 2, continuestmt);

                for(cases = cases->next; cases; cases = cases->next)
                {
                    if(cases->val)
                    {
                        print_indent(indent + 1);
                        printf("elif switchvals[-1] in (");
                        for(struct tree_exps *exp = cases->val; exp;
                            exp = exp->next)
                        {
                            py_exp(exp->exp, 0);
                            printf(", ");
                        }
                        printf("):\n");
                        py_stmts(cases->body, indent + 2, continuestmt);
                    }
                    else
                        defaultbody = cases->body;
                }
                print_indent(indent + 1);
                printf("else:\n");
                py_stmts(defaultbody, indent + 2, continuestmt);
            }
            else
                py_stmts(defaultbody, indent + 1, continuestmt);
        }
        print_indent(indent + 1);
        printf("break\n");
        print_indent(indent);
        printf("switchvals.pop()\n");
    }
    else if(stmt->kind == tree_stmt_kind_for)
    {
        py_stmt(stmt->forstmt.init, indent, continuestmt);
        print_indent(indent);
        printf("while ");
        py_exp(stmt->forstmt.condition, 0);
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
        printf("Array(length = %d, gen = lambda: ", type->array.len);
        py_zerovalue(type->array.type);
        printf(")");
    }
    else if(type->kind == tree_type_kind_slice)
        printf("Slice()");
    else if(type->kind == tree_type_kind_struct)
    {
        printf("Struct({");
        for(struct tree_fields *c = type->structtype.fields; c; c = c->next)
        {
            printf("'%s': ", c->name);
            py_zerovalue(c->type);
            printf(", ");
        }
        printf("})");
    }
}

static void py_varspec(struct tree_var_spec *node, int indent)
{
    if(!node)
        return;
    int nonblank = 0;
    for(struct tree_var_spec *spec = node; spec; spec = spec->next)
        if(!spec->ident->symbol)
        {
            print_indent(indent);
            py_exp(spec->val, 0);
            printf("\n");
        }
        else
            nonblank = 1;
    if(nonblank)
    {
        print_indent(indent);
        for(struct tree_var_spec *spec = node; spec; spec = spec->next)
            if(spec->ident->symbol)
            {
                if(spec->ident->symbol->scope == symbol_scope_global)
                    printf("symbols._%zd", spec->ident->symbol->num);
                else
                    printf("_%zd", spec->ident->symbol->num);
                if(spec->next)
                    printf(", ");
            }
        printf("= ");
        for(struct tree_var_spec *spec = node; spec; spec = spec->next)
            if(spec->ident->symbol)
            {
                if(spec->val)
                    py_exp(spec->val, 1);
                else
                    py_zerovalue(rt(node->type));
                if(spec->next)
                    printf(", ");
            }
        printf("\n");
    }
}

static void py_funcdecl(struct tree_func_decl node, int indent)
{
    print_indent(indent);
    printf("def ");
    if(strcmp(node.ident->name, "init") == 0)
        printf("init");
    else
        printf("_%zd", node.ident->symbol->num);
    printf("(");
    size_t blanknum = 0;
    for(struct tree_params *param = node.params; param; param = param->next)
        if(param->ident->symbol)
            printf("_%zd, ", param->ident->symbol->num);
        else
            printf("_blankparam%zd, ", blanknum++);
    printf("):\n");
    py_stmts(node.body, indent + 1, NULL);
}

void py_program(struct tree_decls *root, struct symbol_rec *symbols)
{
    printf("%.*s", base_py_len, base_py);

    printf("class Symbols(object):\n");
    printf("    __slots__ = (");
    for(size_t i = 0; symbols[i].name; i++)
        if(symbols[i].scope == symbol_scope_global &&
           symbols[i].kind != symbol_kind_type)
            printf("'_%zd', ", symbols[i].num);
    printf(")\n");
    printf("symbols = Symbols()\n");

    size_t mainfunc = 0;
    for(size_t i = 0; symbols[i].name; i++)
        if(symbols[i].kind == symbol_kind_const)
            printf("symbols._%zd = %d\n", symbols[i].num, symbols[i].constrec.constval);
        else if(symbols[i].kind == symbol_kind_func &&
                strcmp(symbols[i].name, "main") == 0)
            mainfunc = symbols[i].num;

    for(struct tree_decls *c = root; c; c = c->next)
        if(c->kind == tree_decls_kind_var_decl)
            py_varspec(c->var_spec, 0);
        else if(c->kind == tree_decls_kind_func_decl)
        {
            if(c->func_decl.ident->symbol)
            {
                py_funcdecl(c->func_decl, 0);
                printf("symbols._%zd = _%zd\n", c->func_decl.ident->symbol->num,
                       c->func_decl.ident->symbol->num);
            }
            else if(strcmp(c->func_decl.ident->name, "init") == 0)
            {
                py_funcdecl(c->func_decl, 0);
                printf("init()\n");
            }
        }

    if(mainfunc > 0)
        printf("\n_%zd()\n", mainfunc);
}
