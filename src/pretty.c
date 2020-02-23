#include <stdio.h>
#include "pretty.h"

int tab = 0; // global

void pretty_program(DECLS *ds) {
    // Go through declarations
    switch(ds->kind) {
        case tree_decls_kind_var_decl:
            pretty_var_decl(ds->var_spec);
            break;
        case tree_decls_kind_type_spec:
            pretty_type_spec(ds->type_spec);
            break;
        case tree_decls_kind_func_decl:
            pretty_func_decl(ds->func_decl);
            break;
    }

    if(ds->next != NULL) {
        pretty_program(ds->next);
    }
}

void pretty_var_decl(VAR_DECL * vd) {

}

void pretty_type_spec(TYPE_SPEC ts) {
    printf("type %s ", ts->name);
    pretty_type(ts->type);
}

void pretty_func_decl(FUNC_DECL * fd) {
    printf("func %s(", fd->name);
    traverse_vars(fd->params);
    printf(") ");
    pretty_type(fd->type);
    printf("\n{\n");
    tab++;
    traverse_stmts(fd->body);
    tab--;
    printf("}\n");
}

void traverse_vars(VARS * v) {
    printf("%s ", v->name);
    pretty_type(v->type);
}

void pretty_type(TYPE * t) {
    switch(t->kind) {
        case tree_type_kind_name:
            printf("%s", t->name);
            break;
        case tree_type_kind_array:
            pretty_array(t->array);
            break;
        case tree_type_kind_slice:
            pretty_slice(t->slice);
            break;
        case tree_type_kind_struct:
            printf("struct");
            printf("\n{\n")
            tab++;
            pretty_struct(t->structtype);
            tab--;
            printf("}")
            break;
    }
}

void pretty_slice(TYPE_SLICE tsl) {
    printf("[]");
    pretty_type(tsl->type);
}

void pretty_array(TYPE_ARRAY ta) {
    printf("[%d]", ta->len);
    pretty_type(ta->type);
}

void pretty_struct(TYPE_STRUCT tstr) {
    traverse_vars(tstr->fields);
}

void traverse_exps(EXPS *es) {
    // Go through an expression list
    pretty_exp(es->exp);

    if(es->next != NULL) {
        traverse_exps(es->next);
    }
}

void traverse_stmts(STMTS *ss) {
    // Go through an stmts
    indentation();
    pretty_stmt(ss->stmt);

    if(ss->next != NULL) {
        traverse_stmts(ss->next);
    }
}

void pretty_exp(EXP *e) {
    switch(e->kind) {
        case tree_exp_kind_ident:
            printf("%s", e->ident);

        case tree_exp_kind_int:
            printf("%d", e->intval);

        case tree_exp_kind_float:
            printf("%lf", e->floatval);

        case tree_exp_kind_rune:
            printf("%c", e->runeval);

        case tree_exp_kind_str:
            printf("%s", e->strval);

        case tree_exp_kind_unary:
            pretty_unaryexp(e->unary);

        case tree_exp_kind_binary:
            pretty_binaryexp(e->binary);

        case tree_exp_kind_call:
            pretty_exp(e->call->func);
            printf("(");
            traverse_exps(e->call->exps);
            printf(")");

        case tree_exp_kind_index:
            pretty_exp(e->index->arr);
            printf("[");
            pretty_exp(e->call->index);
            printf("]");

        case tree_exp_kind_field:
            pretty_exp(e->field->instance);
            printf(".");
            printf("%s", e->field->field);

        case tree_exp_kind_append:
            printf("append(");
            pretty_exp(e->field->instance);
            printf(", ");
            pretty_exp(e->field->instance);
            printf(")");

        case tree_exp_kind_len:
            printf("len(");
            pretty_exp(e->exp);
            printf(")");

        case tree_exp_kind_cap:
            printf("cap(");
            pretty_exp(e->exp);
            printf(")");
    }
}

void pretty_unaryexp(EXP *e) {
    switch(e->kind) {
        case tree_unaryexp_kind_plus:
            printf("+(");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_unaryexp_kind_minus:
            printf("-(");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_unaryexp_kind_not:
            printf("!(");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_unaryexp_kind_comp:
            printf("^(");
            pretty_exp(e->right);
            printf(")");
            break;
    }

}

void pretty_binaryexp(EXP *e) {
    switch(e->kind) {
        case tree_binaryexp_kind_or:
            printf("(");
            pretty_exp(e->left);
            printf(" || ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_and:
            printf("(");
            pretty_exp(e->left);
            printf(" && ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_eq:
            printf("(");
            pretty_exp(e->left);
            printf(" == ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_neq:
            printf("(");
            pretty_exp(e->left);
            printf(" != ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_lt:
            printf("(");
            pretty_exp(e->left);
            printf(" < ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_leq:
            printf("(");
            pretty_exp(e->left);
            printf(" <= ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_gt:
            printf("(");
            pretty_exp(e->left);
            printf(" > ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_geq:
            printf("(");
            pretty_exp(e->left);
            printf(" >= ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_plus:
            printf("(");
            pretty_exp(e->left);
            printf(" + ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_minus:
            printf("(");
            pretty_exp(e->left);
            printf(" - ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_bitor:
            printf("(");
            pretty_exp(e->left);
            printf(" | ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_xor:
            printf("(");
            pretty_exp(e->left);
            printf(" ^ ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_times:
            printf("(");
            pretty_exp(e->left);
            printf(" * ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_div:
            printf("(");
            pretty_exp(e->left);
            printf(" / ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_rem:
            printf("(");
            pretty_exp(e->left);
            printf(" \% ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_lshift:
            printf("(");
            pretty_exp(e->left);
            printf(" << ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_rshift:
            printf("(");
            pretty_exp(e->left);
            printf(" >> ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_bitand:
            printf("(");
            pretty_exp(e->left);
            printf(" & ");
            pretty_exp(e->right);
            printf(")");
            break;

        case tree_binaryexp_kind_andnot:
            printf("(");
            pretty_exp(e->left);
            printf(" &^ ");
            pretty_exp(e->right);
            printf(")");
            break;
    }

}

void pretty_assign(ASSIGN a) {
    printf("%s = ", a->var);
    pretty_exp(a->exp);
}

void pretty_shortdecl(SHORTDECL sd) {
    printf("%s := ", a->name);
    pretty_exp(a->exp);
}

void pretty_var_spec(VAR_DECL vs) {
    printf("var ");
    traverse_names(vs);

    printf(" ")
    pretty_type(vs->type);
    printf(" = ");
    traverse_vals(vs);

}

void traverse_names(VAR_DECL vs) {
    printf("%s", vs->name);
    if(vs->next != NULL) {
        printf(", ");
        traverse_names(vs->next);
    }
}

void traverse_vals(VAR_DECL vs) {
    printf("%s", vs->val);
    if(vs->next != NULL) {
        printf(", ");
        traverse_vals(vs->next);
    }
}


void pretty_if(IF_STMT is) {
    indentation(); printf("if ");

    // initialization
    if(is->init != NULL) {
        pretty_stmt(is->init);
        printf("; ");
    }

    // condition
    pretty_exp(is->condition);
    // body
    printf(" ");
    indentation(); printf("\n{\n"); // Asa convention (Allman style)
    tab++;
    traverse_stmts(is->body);
    tab--;
    indentation(); printf("}"); // Asa convention (Allman style)

    // optional else, else if
    if(is->elsebody != NULL) {
        printf(" else ");
        indentation(); printf("\n{\n")
        tab++;
        traverse_stmts(is->elsebody);
        tab--;
        printf("}")
    }
}

void pretty_switch(SWITCH_STMT ss) {
    printf("switch ");

    // initialization
    if(ss->init != NULL) {
        pretty_stmt(ss->init);
        printf("; ");
    }

    // expression
    pretty_exp(ss->exp);

    // cases
    printf(" ");
    indentation(); printf("\n{\n");
    tab++;
    pretty_cases(ss->cases);
    tab--;
    indentation(); printf("}");

}

void pretty_cases(CASES cs) {
    printf("case ");
    pretty_exp(cs->val);
    printf(":\n");
    tab++;
    traverse_stmts(cs->body);
    tab--;

    if(cs->next != NULL) {
        pretty_cases(cs->next);
    }
}

void pretty_for(FOR_STMT fs) {
    indentation(); printf("for ");

    // initialization
    if(fs->init != NULL) {
        pretty_stmt(fs->init);
        printf("; ");
    }

    // condition
    if(fs->condition != NULL) {
        pretty_exp(fs->condition);
        printf("; ");
    }

    // iter statement
    if(fs->iter != NULL) {
        pretty_stmt(fs->iter);
        printf(" ");
    }

    // body
    indentation(); printf("\n{\n"); // Asa convention (Allman style)
    tab++;
    traverse_stmts(is->body);
    tab--;
    indentation(); printf("}"); // Asa convention (Allman style)
}

void pretty_stmt(STMT *s) {
    switch(s->kind) {

        case tree_stmt_kind_exp:
            pretty_exp(s->expstmt);
            printf(";");
            break;

        case tree_stmt_kind_assign:
            pretty_assign(s->assign);
            printf(";");
            break;

        case tree_stmt_kind_shortdecl:
            pretty_shortdecl(s->shortdecl);
            break;

        case tree_stmt_kind_inc:
            pretty_exp(s->exp);
            print("++;");
            break;

        case tree_stmt_kind_dec:
            pretty_exp(s->exp);
            print("--;");
            break;

        case tree_stmt_kind_var_decl:
            pretty_var_spec(s->var_spec);
            break;

        case tree_stmt_kind_type_spec:
            pretty_type_spec(s->type_spec);
            break;

        case tree_stmt_kind_print:
            printf("print(");
            traverse_exps(s->exps);
            printf(");");
            break;

        case tree_stmt_kind_println:
            printf("println(");
            traverse_exps(s->exps);
            printf(");");
            break;

        case tree_stmt_kind_return:
            printf("return ");
            pretty_exp(s->exp);
            printf(";");
            break;

        case tree_stmt_kind_if:
            pretty_if(s->ifstmt);
            break;

        case tree_stmt_kind_switch:
            pretty_switch(s->switchstmt);
            break;

        case tree_stmt_kind_for:
            pretty_for(s->forstmt);
            break;

        case tree_stmt_kind_break:
            printf("break;");
            break;

        case tree_stmt_kind_continue:
            printf("continue;");
            break;

        case tree_stmt_kind_fallthrough:
            printf("fallthrough");
            break;
    }
    printf("\n");
}

void indentation() {
    // Indentation of statements and nested counterparts
    byte nspace = 4;
    for(int i = 0; i < tab; i++) {
        for(byte s = 0; s < nspace; s++) printf(" ");
    }
}
