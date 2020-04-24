#include "symbolprint.h"

#include <stdio.h>

#include "symbol.h"

static void indent(int n)
{
    for(int i = 0; i < n; i++)
        printf("    ");
}

void sp_type(struct tree_type *type)
{
    if(!type)
    {
        printf("[infer]");
        return;
    }
    if(type->kind == tree_type_kind_base)
    {
        switch(type->base)
        {
            case tree_base_type_int:
                printf("int");
                break;
            case tree_base_type_float64:
                printf("float64");
                break;
            case tree_base_type_rune:
                printf("rune");
                break;
            case tree_base_type_bool:
                printf("bool");
                break;
            case tree_base_type_str:
                printf("string");
                break;
        }
    }
    else if(type->kind == tree_type_kind_defined)
        printf("[defined type]");
    else if(type->kind == tree_type_kind_array)
    {
        printf("[%d]", type->array.len);
        sp_type(type->array.type);
    }
    else if(type->kind == tree_type_kind_slice)
    {
        printf("[]");
        sp_type(type->slice.type);
    }
    else if(type->kind == tree_type_kind_struct)
    {
        printf("struct{");
        for(struct tree_fields *field = type->structtype.fields; field;
            field = field->next)
        {
            printf("%s ", field->name);
            sp_type(field->type);
            if(field->next)
                printf(", ");
        }
        printf("}");
    }
}

void sp_symbol(struct symbol_rec *symbol)
{
    if(symbol->kind == symbol_kind_var)
    {
        printf("%s [var] ", symbol->name);
        sp_type(symbol->type);
        printf("\n");
    }
    else if(symbol->kind == symbol_kind_type)
    {
        printf("%s [type] ", symbol->name);
        sp_type(symbol->type);
        printf("\n");
    }
}

void sp_stmts(struct tree_stmts *stmts, int depth);

void sp_stmt(struct tree_stmt *stmt, int depth)
{
    if(!stmt)
        return;
    if(stmt->kind == tree_stmt_kind_block)
    {
        indent(depth);
        printf("{\n");
        sp_stmts(stmt->block, depth + 1);
        indent(depth);
        printf("}\n");
    }
    if(stmt->kind == tree_stmt_kind_shortdecl)
        for(struct tree_idents *ident = stmt->shortdecl.idents; ident;
            ident = ident->next)
        {
            indent(depth);
            sp_symbol(ident->ident->symbol);
        }
    if(stmt->kind == tree_stmt_kind_var_decl)
        for(struct tree_var_spec *var_spec = stmt->var_spec; var_spec;
            var_spec = var_spec->next)
        {
            indent(depth);
            sp_symbol(var_spec->ident->symbol);
        }
    if(stmt->kind == tree_stmt_kind_type_spec)
    {
        indent(depth);
        sp_symbol(stmt->type_spec.ident->symbol);
    }
    if(stmt->kind == tree_stmt_kind_if)
    {
        indent(depth);
        printf("{\n");
        sp_stmt(stmt->ifstmt.init, depth + 1);
        indent(depth + 1);
        printf("{\n");
        sp_stmts(stmt->ifstmt.body, depth + 2);
        indent(depth + 1);
        printf("}\n");
        indent(depth + 1);
        printf("{\n");
        sp_stmts(stmt->ifstmt.elsebody, depth + 2);
        indent(depth + 1);
        printf("}\n");
        indent(depth);
        printf("}\n");
    }
    if(stmt->kind == tree_stmt_kind_switch)
    {
        indent(depth);
        printf("{\n");
        sp_stmts(stmt->block, depth + 1);
        indent(depth + 1);
        printf("{\n");
        for(struct tree_cases *cases = stmt->switchstmt.cases; cases;
            cases = cases->next)
            sp_stmts(cases->body, depth + 2);
        indent(depth + 1);
        printf("}\n");
        indent(depth);
        printf("}\n");
    }
    if(stmt->kind == tree_stmt_kind_for)
    {
        indent(depth);
        printf("{\n");
        sp_stmt(stmt->forstmt.init, depth + 1);
        indent(depth + 1);
        printf("{\n");
        sp_stmts(stmt->forstmt.body, depth + 2);
        indent(depth + 1);
        printf("}\n");
        indent(depth);
        printf("}\n");
    }
}

void sp_stmts(struct tree_stmts *stmts, int depth)
{
    if(!stmts)
        return;
    sp_stmt(&stmts->stmt, depth);
    sp_stmts(stmts->next, depth);
}

void symbolprint(struct tree_decls *root)
{
    if(!root)
        return;
    if(root->kind == tree_decls_kind_var_decl)
        for(struct tree_var_spec *var_spec = root->var_spec; var_spec;
            var_spec = var_spec->next)
            sp_symbol(var_spec->ident->symbol);
    else if(root->kind == tree_decls_kind_type_spec)
        sp_symbol(root->type_spec.ident->symbol);
    else if(root->kind == tree_decls_kind_func_decl)
    {
        printf("%s [func] (", root->func_decl.ident->name);
        for(struct tree_params *param = root->func_decl.params; param;
            param = param->next)
        {
            printf("%s", param->ident->name);
            if(param->next)
                printf(", ");
            indent(1);
        }
        printf(") -> ");
        sp_type(root->func_decl.type);
        printf("\n{\n");
        sp_stmts(root->func_decl.body, 1);
        printf("}\n");
    }
    symbolprint(root->next);
}
