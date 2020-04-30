#include "symbol.h"

#include <stdio.h>
#include <string.h>

#include "utils.h"

struct symbols
{
    struct symbol_rec *rec;
    struct symbols *next;
};

struct scopetable
{
    struct symbols *table[256];
    struct scopetable *parent;
};

static unsigned char scopetable_hash(char *s)
{
    unsigned char hash = 0;
    for(char *c = s; *c; c++)
        hash = (hash << 1) + *c;
    return hash;
}

static void scopetable_add(struct symbols **table, struct symbol_rec *rec)
{
    struct symbols *new = emalloc(sizeof(struct symbols));
    new->next = NULL;
    new->rec = rec;

    unsigned char hash = scopetable_hash(rec->name);
    if(table[hash])
    {
        struct symbols *bin = table[hash];
        while(bin->next)
            bin = bin->next;
        bin->next = new;
    }
    else
        table[hash] = new;
}

static struct symbol_rec *scopetable_getleaf(struct symbols **table,
                                             char *ident)
{
    struct symbols *bin = table[scopetable_hash(ident)];
    while(bin && strcmp(bin->rec->name, ident) != 0)
        bin = bin->next;
    if(bin)
        return bin->rec;
    return NULL;
}

/* static void scopetable_mod(struct symbols **table, struct symbol_rec *rec) */
/* { */
/*     unsigned char hash = scopetable_hash(rec->name); */
/*     struct symbols *bin = table[hash]; */
/*     if(bin) */
/*     { */
/*         while(bin->next && strcmp(bin->rec->name, rec->name) != 0) */
/*             bin = bin->next; */
/*         if(strcmp(bin->rec->name, rec->name) != 0) */
/*             (bin = bin->next = emalloc(sizeof(struct symbols)))->next = NULL; */
/*     } */
/*     else */
/*         (table[hash] = bin = emalloc(sizeof(struct symbols)))->next = NULL; */
/*     bin->rec = rec; */
/* } */

static struct symbol_rec *scopetable_get(struct scopetable *table, char *ident)
{
    if(!table)
        return NULL;
    struct symbol_rec *result = scopetable_getleaf(table->table, ident);
    if(result)
        return result;
    return scopetable_get(table->parent, ident);
}

static void scopetable_free(struct scopetable table)
{
    for(int i = 0; i < 256; i++)
    {
        struct symbols *curr = table.table[i];
        while(curr)
        {
            struct symbols *temp = curr->next;
            free(curr);
            curr = temp;
        }
    }
}

static void gather_type(struct tree_type *type, struct scopetable *table,
                        int lineno)
{
    if(!type)
        return;
    if(type->kind == tree_type_kind_reference)
    {
        if(strcmp(type->reference.name, "_") == 0)
        {
            fprintf(stderr, "Error: invalid blank identifier on line %d\n",
                    lineno);
            exit(1);
        }
        if(!type->reference.symbol)
        {
            if(!(type->reference.symbol =
                 scopetable_get(table, type->reference.name)))
            {
                fprintf(stderr, "Error: symbol \"%s\" on line %d undeclared\n",
                        type->reference.name, lineno);
                exit(1);
            }
            if(type->reference.symbol->kind != symbol_kind_type)
            {
                fprintf(stderr, "Error: symbol \"%s\" on line %d is not a type\n",
                        type->reference.name, lineno);
                exit(1);
            }
        }

        struct tree_type temp = *type;
        *type = *temp.reference.symbol->type;
        free(temp.reference.name);
    }
    else if(type->kind == tree_type_kind_struct)
        for(struct tree_fields *field = type->structtype.fields; field;
            field = field->next)
        {
            gather_type(field->type, table, lineno);
            for(struct tree_fields *i = type->structtype.fields; i != field;
                i = i->next)
                if(strcmp(i->name, field->name) == 0 &&
                   strcmp(i->name, "_") != 0)
                {
                    fprintf(stderr,
                            "Error: duplicate struct field on nline %d\n",
                            lineno);
                    exit(1);
                }
        }
    else if(type->kind == tree_type_kind_array)
        gather_type(type->array.type, table, lineno);
    else if(type->kind == tree_type_kind_slice)
        gather_type(type->slice.type, table, lineno);
}

static void gather_exp(struct symbol_rec *symbols, struct tree_exp *node,
                       struct scopetable *table, size_t *index)
{
    if(!node)
        return;

    switch(node->kind)
    {
        case tree_exp_kind_ident:
            if(strcmp(node->ident->name, "_") == 0)
            {
                fprintf(stderr, "Error: invalid blank identifier on line %d\n",
                        node->lineno);
                exit(1);
            }
            if(!(node->ident->symbol =
                 scopetable_get(table, node->ident->name)))
            {
                fprintf(stderr, "Error: symbol \"%s\" on line %d undeclared\n",
                        node->ident->name, node->lineno);
                exit(1);
            }
            if(node->ident->symbol->kind == symbol_kind_type)
                gather_type(node->ident->symbol->type, table, node->lineno);
            break;
        case tree_exp_kind_unary:
            gather_exp(symbols, node->unary.right, table, index);
            break;
        case tree_exp_kind_binary:
            gather_exp(symbols, node->binary.left, table, index);
            gather_exp(symbols, node->binary.right, table, index);
            break;
        case tree_exp_kind_call:
            gather_exp(symbols, node->call.func, table, index);
            for(struct tree_exps *exp = node->call.exps; exp; exp = exp->next)
                gather_exp(symbols, exp->exp, table, index);
            break;
        case tree_exp_kind_index:
            gather_exp(symbols, node->index.arr, table, index);
            gather_exp(symbols, node->index.index, table, index);
            break;
        case tree_exp_kind_field:
            if(strcmp(node->field.field, "_") == 0)
            {
                fprintf(stderr, "Error: invalid blank identifier on line %d\n",
                        node->lineno);
                exit(1);
            }
            gather_exp(symbols, node->field.instance, table, index);
            break;
        case tree_exp_kind_append:
            gather_exp(symbols, node->append.exp1, table, index);
            gather_exp(symbols, node->append.exp2, table, index);
            break;
        case tree_exp_kind_len:
        case tree_exp_kind_cap:
            gather_exp(symbols, node->exp, table, index);
            break;
        case tree_exp_kind_int:
        case tree_exp_kind_float:
        case tree_exp_kind_rune:
        case tree_exp_kind_str:
        default:
            break;
    }
}

static void gather_stmts(struct symbol_rec *symbols, struct tree_stmts *node,
                         struct scopetable *table, size_t *index);

static void gather_stmt(struct symbol_rec *symbols, struct tree_stmt *node,
                        struct scopetable *table, size_t *index)
{
    if(!node)
        return;

    switch(node->kind)
    {
        case tree_stmt_kind_exp:
            gather_exp(symbols, &node->expstmt, table, index);
            break;
        case tree_stmt_kind_block:;
            struct scopetable blocktable = {.table = {NULL}, .parent = table};
            gather_stmts(symbols, node->block, &blocktable, index);
            scopetable_free(blocktable);
            break;
        case tree_stmt_kind_assign:
            for(struct tree_exps *c = node->assign.left; c; c = c->next)
                if(c->exp->kind == tree_exp_kind_ident &&
                   strcmp(c->exp->ident->name, "_") == 0)
                    c->exp->ident->symbol = NULL;
                else
                    gather_exp(symbols, c->exp, table, index);
            for(struct tree_exps *c = node->assign.right; c; c = c->next)
                gather_exp(symbols, c->exp, table, index);
            break;
        case tree_stmt_kind_assignop:
            gather_exp(symbols, node->assignop.left, table, index);
            gather_exp(symbols, node->assignop.right, table, index);
            break;
        case tree_stmt_kind_shortdecl:
            for(struct tree_exps *c = node->shortdecl.exps; c; c = c->next)
                gather_exp(symbols, c->exp, table, index);
            struct tree_idents *ident = node->shortdecl.idents;
            struct tree_exps *exp = node->shortdecl.exps;
            int newdecl = 0;
            for(; ident; ident = ident->next, exp = exp->next)
            {
                for(struct tree_idents *i = node->shortdecl.idents; i != ident;
                    i = i->next)
                    if(strcmp(ident->ident->name, i->ident->name) == 0)
                    {
                        fprintf(stderr, "Error: repeated symbol in short "
                                "declaration on line %d\n", node->lineno);
                        exit(1);
                    }
                if(strcmp(ident->ident->name, "_") == 0)
                    continue;
                if(!(ident->ident->symbol =
                     scopetable_getleaf(table->table, ident->ident->name)))
                {
                    newdecl = 1;
                    symbols[*index].num = *index;
                    symbols[*index].scope = symbol_scope_local;
                    symbols[*index].name = ident->ident->name;
                    symbols[*index].kind = symbol_kind_var;
                    symbols[*index].type = NULL;
                    scopetable_add(table->table,
                                   ident->ident->symbol = &symbols[*index]);
                    (*index)++;
                }
                if(ident->ident->symbol->kind != symbol_kind_var)
                {
                    fprintf(stderr,
                            "Error: symbol on line %d is not a variable\n",
                            node->lineno);
                    exit(1);
                }
            }
            if(!newdecl)
            {
                fprintf(stderr, "Error: no variable declared on line %d\n",
                        node->lineno);
                exit(1);
            }
            break;
        case tree_stmt_kind_inc:
        case tree_stmt_kind_dec:
        case tree_stmt_kind_return:
            gather_exp(symbols, node->exp, table, index);
            break;
        case tree_stmt_kind_var_decl:
            for(struct tree_var_spec *j = node->var_spec; j; j = j->next)
            {
                gather_type(j->type, table, node->lineno);
                gather_exp(symbols, j->val, table, index);
            }
            for(struct tree_var_spec *j = node->var_spec; j; j = j->next)
            {
                if(strcmp(j->ident->name, "_") == 0)
                    j->ident->symbol = NULL;
                else
                {
                    if(scopetable_getleaf(table->table, j->ident->name))
                    {
                        fprintf(stderr,
                                "Error: symbol \"%s\" redeclared on line %d\n",
                                j->ident->name, node->lineno);
                        exit(1);
                    }
                    symbols[*index].num = *index;
                    symbols[*index].scope = symbol_scope_local;
                    symbols[*index].name = j->ident->name;
                    symbols[*index].kind = symbol_kind_var;
                    symbols[*index].type = j->type;
                    scopetable_add(table->table,
                                   j->ident->symbol = &symbols[*index]);
                    (*index)++;
                }
            }
            break;
        case tree_stmt_kind_type_spec:
            gather_type(node->type_spec.type, table, node->lineno);
            struct tree_type *temp = emalloc(sizeof(struct tree_type));
            temp->kind = tree_type_kind_defined;
            temp->defined = node->type_spec.type;
            node->type_spec.type = temp;
            if(strcmp(node->type_spec.ident->name, "_") == 0)
                node->type_spec.ident->symbol = NULL;
            else
            {
                if(scopetable_getleaf(table->table, node->type_spec.ident->name))
                {
                    fprintf(stderr,
                            "Error: symbol \"%s\" redeclared on line %d\n",
                            node->type_spec.ident->name, node->lineno);
                    exit(1);
                }
                symbols[*index].num = *index;
                symbols[*index].scope = symbol_scope_local;
                symbols[*index].name = node->type_spec.ident->name;
                symbols[*index].kind = symbol_kind_type;
                symbols[*index].type = node->type_spec.type;
                scopetable_add(table->table,
                               node->type_spec.ident->symbol = &symbols[*index]);
                (*index)++;
            }
            break;
        case tree_stmt_kind_print:
        case tree_stmt_kind_println:
            for(struct tree_exps *exp = node->exps; exp; exp = exp->next)
                gather_exp(symbols, exp->exp, table, index);
            break;
        case tree_stmt_kind_if:;
            struct scopetable outertable = {.table = {NULL}, .parent = table};
            struct scopetable innertable =
                {.table = {NULL}, .parent = &outertable};
            gather_stmt(symbols, node->ifstmt.init, &outertable, index);
            gather_exp(symbols, node->ifstmt.condition, &outertable, index);
            gather_stmts(symbols, node->ifstmt.body, &innertable, index);
            scopetable_free(innertable);
            innertable = (struct scopetable) {.table = {NULL},
                                              .parent = &outertable};
            gather_stmts(symbols, node->ifstmt.elsebody,
                         &innertable, index);
            scopetable_free(innertable);
            scopetable_free(outertable);
            break;
        case tree_stmt_kind_switch:
            outertable = (struct scopetable){.table = {NULL}, .parent = table};
            innertable =
                (struct scopetable){.table = {NULL}, .parent = &outertable};
            gather_stmt(symbols, node->switchstmt.init, &outertable, index);
            gather_exp(symbols, node->switchstmt.exp, &outertable, index);
            for(struct tree_cases *cases = node->switchstmt.cases; cases;
                cases = cases->next)
            {
                struct scopetable casetable = {.table = {NULL},
                                               .parent = &innertable};
                for(struct tree_exps *exp = cases->val; exp; exp = exp->next)
                    gather_exp(symbols, exp->exp, &casetable, index);
                gather_stmts(symbols, cases->body, &casetable, index);
                scopetable_free(casetable);
            }
            scopetable_free(innertable);
            scopetable_free(outertable);
            break;
        case tree_stmt_kind_for:
            outertable = (struct scopetable){.table = {NULL}, .parent = table};
            innertable =
                (struct scopetable){.table = {NULL}, .parent = &outertable};
            gather_stmt(symbols, node->forstmt.init, &outertable, index);
            gather_exp(symbols, node->forstmt.condition, &outertable, index);
            gather_stmt(symbols, node->forstmt.iter, &outertable, index);
            gather_stmts(symbols, node->forstmt.body, &innertable, index);
            break;
        default:
            break;
    }
}

static void gather_stmts(struct symbol_rec *symbols, struct tree_stmts *node,
                         struct scopetable *table, size_t *index)
{
    if(!node)
        return;
    gather_stmt(symbols, &node->stmt, table, index);
    gather_stmts(symbols, node->next, table, index);
}

static void gather_program(struct symbol_rec *symbols, struct tree_decls *node,
                           struct scopetable *parent, size_t *index)
{
    struct scopetable table = {NULL};
    table.parent = parent;

    for(struct tree_decls *i = node; i; i = i->next)
    {
        if(i->kind == tree_decls_kind_var_decl)
        {
            for(struct tree_var_spec *j = i->var_spec; j; j = j->next)
            {
                if(strcmp(j->ident->name, "init") == 0 ||
                   strcmp(j->ident->name, "main") == 0)
                {
                    fprintf(stderr, "Error: special symbol declared as a "
                            "variable at top level on line %d\n", i->lineno);
                    exit(1);
                }
                gather_type(j->type, &table, i->lineno);
                gather_exp(symbols, j->val, &table, index);
            }
            for(struct tree_var_spec *j = i->var_spec; j; j = j->next)
            {
                if(strcmp(j->ident->name, "_") == 0)
                    j->ident->symbol = NULL;
                else
                {
                    if(scopetable_getleaf(table.table, j->ident->name))
                    {
                        fprintf(stderr,
                                "Error: symbol \"%s\" redeclared on line %d\n",
                                j->ident->name, i->lineno);
                        exit(1);
                    }
                    symbols[*index].num = *index;
                    symbols[*index].scope = symbol_scope_global;
                    symbols[*index].name = j->ident->name;
                    symbols[*index].kind = symbol_kind_var;
                    symbols[*index].type = j->type;
                    scopetable_add(table.table,
                                   j->ident->symbol = &symbols[*index]);
                    (*index)++;
                }
            }
        }
        if(i->kind == tree_decls_kind_type_spec)
        {
            struct tree_type *temp = i->type_spec.type;
            i->type_spec.type = emalloc(sizeof(struct tree_type));
            if(strcmp(i->type_spec.ident->name, "_") == 0)
                i->type_spec.ident->symbol = NULL;
            else
            {
                if(strcmp(i->type_spec.ident->name, "init") == 0 ||
                   strcmp(i->type_spec.ident->name, "main") == 0)
                {
                    fprintf(stderr, "Error: special symbol declared as a type "
                            "at top level on line %d\n", i->lineno);
                    exit(1);
                }
                if(scopetable_getleaf(table.table, i->type_spec.ident->name))
                {
                    fprintf(stderr,
                            "Error: symbol \"%s\" redeclared on line %d\n",
                            i->type_spec.ident->name, i->lineno);
                    exit(1);
                }
                symbols[*index].num = *index;
                symbols[*index].scope = symbol_scope_global;
                symbols[*index].name = i->type_spec.ident->name;
                symbols[*index].kind = symbol_kind_type;
                symbols[*index].type = i->type_spec.type;
                scopetable_add(table.table,
                               i->type_spec.ident->symbol = &symbols[*index]);
                (*index)++;
            }
            gather_type(temp, &table, i->lineno);
            i->type_spec.type->kind = tree_type_kind_defined;
            i->type_spec.type->defined = temp;
        }
        if(i->kind == tree_decls_kind_func_decl)
        {
            if(strcmp(i->func_decl.ident->name, "_") == 0)
                i->func_decl.ident->symbol = NULL;
            else
            {
                if(scopetable_getleaf(table.table, i->func_decl.ident->name))
                {
                    fprintf(stderr,
                            "Error: symbol \"%s\" redeclared on line %d\n",
                            i->func_decl.ident->name, i->lineno);
                    exit(1);
                }
                symbols[*index].num = *index;
                symbols[*index].scope = symbol_scope_global;
                symbols[*index].name = i->func_decl.ident->name;
                symbols[*index].kind = symbol_kind_func;
                symbols[*index].func = &i->func_decl;
                if(strcmp(i->func_decl.ident->name, "init") != 0)
                    scopetable_add(table.table,
                                   i->func_decl.ident->symbol = &symbols[*index]);
                (*index)++;
            }
            gather_type(i->func_decl.type, &table, i->lineno);
            struct scopetable innertable = {.table = {NULL}, .parent = &table};
            for(struct tree_params *c = i->func_decl.params; c; c = c->next)
                gather_type(c->type, &innertable, i->lineno);
            for(struct tree_params *c = i->func_decl.params; c; c = c->next)
            {
                if(strcmp(c->ident->name, "_") == 0)
                {
                    c->ident->symbol = NULL;
                    continue;
                }
                if(scopetable_getleaf(innertable.table, c->ident->name))
                {
                    fprintf(stderr,
                            "Error: symbol \"%s\" redeclared on line %d\n",
                            c->ident->name, i->lineno);
                    exit(1);
                }
                symbols[*index].num = *index;
                symbols[*index].scope = symbol_scope_local;
                symbols[*index].name = c->ident->name;
                symbols[*index].kind = symbol_kind_var;
                symbols[*index].type = c->type;
                scopetable_add(innertable.table,
                               c->ident->symbol = &symbols[*index]);
                (*index)++;
            }
            gather_stmts(symbols, i->func_decl.body, &innertable, index);
            scopetable_free(innertable);
        }
    }
    scopetable_free(table);
}

static size_t count_stmts_syms(struct tree_stmts *stmts);

static size_t count_stmt_syms(struct tree_stmt *stmt)
{
    if(!stmt)
        return 0;
    if(stmt->kind == tree_stmt_kind_block)
        return count_stmts_syms(stmt->block);
    if(stmt->kind == tree_stmt_kind_shortdecl)
    {
        size_t n = 0;
        for(struct tree_idents *c = stmt->shortdecl.idents; c; c = c->next)
            n++;
        return n;
    }
    if(stmt->kind == tree_stmt_kind_var_decl)
    {
        size_t n = 0;
        for(struct tree_var_spec *c = stmt->var_spec; c; c = c->next)
            n++;
        return n;
    }
    if(stmt->kind == tree_stmt_kind_type_spec)
        return 1;
    if(stmt->kind == tree_stmt_kind_if)
        return count_stmt_syms(stmt->ifstmt.init)
            + count_stmts_syms(stmt->ifstmt.body)
            + count_stmts_syms(stmt->ifstmt.elsebody);
    if(stmt->kind == tree_stmt_kind_switch)
    {
        size_t n = 0;
        for(struct tree_cases *c = stmt->switchstmt.cases; c;
            c = c->next) n += count_stmts_syms(c->body);
        return count_stmt_syms(stmt->switchstmt.init) + n;
    }
    if(stmt->kind == tree_stmt_kind_for)
        return count_stmts_syms(stmt->forstmt.body)
            + count_stmt_syms(stmt->forstmt.init);
    return 0;
}

static size_t count_stmts_syms(struct tree_stmts *stmts)
{
    if(!stmts)
        return 0;
    return count_stmts_syms(stmts->next) + count_stmt_syms(&stmts->stmt);
}

static size_t count_syms(struct tree_decls *decls)
{
    if(!decls)
        return 0;
    if(decls->kind == tree_decls_kind_var_decl)
    {
        size_t n = 0;
        for(struct tree_var_spec *c = decls->var_spec; c; c = c->next)
            n++;
        return count_syms(decls->next) + n;
    }
    if(decls->kind == tree_decls_kind_type_spec)
        return count_syms(decls->next) + 1;
    if(decls->kind == tree_decls_kind_func_decl)
    {
        size_t n = 1;
        for(struct tree_params *c = decls->func_decl.params; c; c = c->next)
            n++;
        return count_syms(decls->next)
            + count_stmts_syms(decls->func_decl.body) + n;
    }
    return count_syms(decls->next);
}

struct symbol_rec *symbol_weave(struct tree_decls *root)
{
    size_t nsyms = count_syms(root);
    struct symbol_rec *symbols = emalloc((nsyms + 8) * sizeof(*symbols));

    struct scopetable base = {NULL};
    base.parent = NULL;
    symbols[0] = (struct symbol_rec) {
        .num = 0,
        .name = "true",
        .kind = symbol_kind_const,
        .scope = symbol_scope_global,
        .constrec = {.constval = 1, .type = emalloc(sizeof(struct tree_type))}
    };
    symbols[1] = (struct symbol_rec) {
        .num = 1,
        .name = "false",
        .kind = symbol_kind_const,
        .scope = symbol_scope_global,
        .constrec = {.constval = 0, .type = emalloc(sizeof(struct tree_type))}
    };
    symbols[2] = (struct symbol_rec) {
        .num = 2,
        .name = "int",
        .kind = symbol_kind_type,
        .scope = symbol_scope_global,
        .type = emalloc(sizeof(struct tree_type))
    };
    symbols[3] = (struct symbol_rec) {
        .num = 3,
        .name = "float64",
        .kind = symbol_kind_type,
        .scope = symbol_scope_global,
        .type = emalloc(sizeof(struct tree_type))
    };
    symbols[4] = (struct symbol_rec) {
        .num = 4,
        .name = "rune",
        .kind = symbol_kind_type,
        .scope = symbol_scope_global,
        .type = emalloc(sizeof(struct tree_type))
    };
    symbols[5] = (struct symbol_rec) {
        .num = 5,
        .name = "bool",
        .kind = symbol_kind_type,
        .scope = symbol_scope_global,
        .type = emalloc(sizeof(struct tree_type))
    };
    symbols[6] = (struct symbol_rec) {
        .num = 6,
        .name = "string",
        .kind = symbol_kind_type,
        .scope = symbol_scope_global,
        .type = emalloc(sizeof(struct tree_type))
    };
    symbols[0].constrec.type->kind = tree_type_kind_base;
    symbols[0].constrec.type->base = tree_base_type_bool;
    symbols[1].constrec.type->kind = tree_type_kind_base;
    symbols[1].constrec.type->base = tree_base_type_bool;
    symbols[2].type->kind = tree_type_kind_base;
    symbols[2].type->base = tree_base_type_int;
    symbols[3].type->kind = tree_type_kind_base;
    symbols[3].type->base = tree_base_type_float64;
    symbols[4].type->kind = tree_type_kind_base;
    symbols[4].type->base = tree_base_type_rune;
    symbols[5].type->kind = tree_type_kind_base;
    symbols[5].type->base = tree_base_type_bool;
    symbols[6].type->kind = tree_type_kind_base;
    symbols[6].type->base = tree_base_type_str;
    for(int i = 0; i < 7; i++)
        scopetable_add(base.table, &symbols[i]);

    size_t i = 7;
    gather_program(symbols, root, &base, &i);
    scopetable_free(base);
    symbols[i].name = NULL;
    return symbols;
}
