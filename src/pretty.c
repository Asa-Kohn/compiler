#include <stdio.h>
#include "pretty.h"

int tab = 0; // global

void pretty_program(DECLS *ds);
void pretty_type_spec(TYPE_SPEC ts);
void pretty_func_decl(FUNC_DECL *fd);
void traverse_vars_spec(VAR_SPEC vs);
void traverse_vars(VARS * v);
void pretty_type(TYPE * t);
void pretty_slice(TYPE_SLICE tsl);
void pretty_array(TYPE_ARRAY ta);
void pretty_struct(TYPE_STRUCT tstr);
void traverse_exps(EXPS *es);
void traverse_stmts(STMTS *ss);
void pretty_exp(EXP *e);
void pretty_unaryexp(EXP *e);
void pretty_binaryexp(EXP *e);
void pretty_assignop(ASSIGNOP a);
void traverse_idents(IDENTS ids);
void pretty_shortdecl(SHORTDECL sd);
void pretty_var_spec(VAR_DECL vs);
void traverse_names(VAR_DECL vs);
void traverse_vals(VAR_DECL vs);
void pretty_if(IF_STMT is);
void pretty_switch(SWITCH_STMT ss);
void pretty_cases(CASES cs);
void pretty_for(FOR_STMT fs);
void pretty_stmt(STMT *s);
void indentation();

void pretty_program(DECLS *ds) {
    if(ds == NULL) return;

    // Go through declarations
    switch(ds->kind) {
        case tree_decls_kind_var_decl:
            pretty_var_spec(ds->var_spec);
            break;
        case tree_decls_kind_type_spec:
            pretty_type_spec(ds->type_spec);
            break;
        case tree_decls_kind_func_decl:
            pretty_func_decl(ds->func_decl);
            break;
    }

    pretty_program(ds->next);
}

void pretty_type_spec(TYPE_SPEC ts) {
    printf("type %s ", ts->name);
    pretty_type(ts->type);
}

void pretty_func_decl(FUNC_DECL *fd) {
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

void traverse_vars_spec(VAR_SPEC vs) {
    printf("%s ", vs.name);

    if(vs.type != NULL) pretty_type(vs.type);
    if(vs.next != NULL) {
        printf(", ");
        traverse_vars_spec(vs.next);
    }
}

void traverse_vars(VARS * v) {
    printf("%s ", v->name);

    if(v->next != NULL) {
        printf(", ");
        traverse_vars(v->next);
    }
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
            indentation(); printf("struct");
            indentation(); printf("\n{\n")
            tab++;
            pretty_struct(t->structtype);
            tab--;
            indentation(); printf("\n}")
            break;
    }
}

void pretty_slice(TYPE_SLICE tsl) {
    indentation();
    printf("[]");
    pretty_type(tsl.type);
}

void pretty_array(TYPE_ARRAY ta) {
    indentation();
    printf("[%d]", ta.len);
    pretty_type(ta.type);
}

void pretty_struct(TYPE_STRUCT tstr) {
    traverse_vars(tstr.fields);
    printf(" ");
    pretty_type(tstr.fields->type);
}

void traverse_exps(EXPS *es) {
    // Go through an expression list
    pretty_exp(es->exp);

    if(es->next != NULL) {
        printf(", ");
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
            break;

        case tree_exp_kind_int:
            printf("%d", e->intval);
            break;

        case tree_exp_kind_float:
            printf("%lf", e->floatval);
            break;

        case tree_exp_kind_rune:
            printf("%c", e->runeval);
            break;

        case tree_exp_kind_str:
            printf("%s", e->strval);
            break;

        case tree_exp_kind_unary:
            pretty_unaryexp(e->unary);
            break;

        case tree_exp_kind_binary:
            pretty_binaryexp(e->binary);
            break;

        case tree_exp_kind_call:
            pretty_exp(e->call->func);
            printf("(");
            traverse_exps(e->call->exps);
            printf(")");
            break;

        case tree_exp_kind_index:
            pretty_exp(e->index->arr);
            printf("[");
            pretty_exp(e->call->index);
            printf("]");
            break;

        case tree_exp_kind_field:
            pretty_exp(e->field->instance);
            printf(".");
            printf("%s", e->field->field);
            break;

        case tree_exp_kind_append:
            printf("append(");
            pretty_exp(e->field->instance);
            printf(", ");
            pretty_exp(e->field->instance);
            printf(")");
            break;

        case tree_exp_kind_len:
            printf("len(");
            pretty_exp(e->exp);
            printf(")");
            break;

        case tree_exp_kind_cap:
            printf("cap(");
            pretty_exp(e->exp);
            printf(")");
            break;
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

void pretty_assignop(ASSIGNOP a) {
    pretty_exp(a.left);
    print(" ");
    switch(a.kind) {
        case tree_assignop_kind_plus:
            print("+");
            break;
        case tree_assignop_kind_minus:
            print("-");
            break;
        case tree_assignop_kind_or:
            print("|");
            break;
        case tree_assignop_kind_xor:
            print("^");
            break;
        case tree_assignop_kind_times:
            print("*");
            break;
        case tree_assignop_kind_div:
            print("/");
            break;
        case tree_assignop_kind_rem:
            print("%");
            break;
        case tree_assignop_kind_lshift:
            print("<<");
            break;
        case tree_assignop_kind_rshift:
            print(">>");
            break;
        case tree_assignop_kind_and:
            print("&");
            break;
        case tree_assignop_kind_andnot:
            print("&^");
            break;
    }
    print("= ");
    pretty_exp(a.right);
}

void traverse_idents(IDENTS ids) {
    // Go through an idents list
    pretty_name(ids.exp);

    if(ids.next != NULL) {
        printf(", ");
        traverse_exps(ids.next);
    }
}

void pretty_shortdecl(SHORTDECL sd) {
    traverse_idents(sd->idents);
    printf(" := ");
    traverse_exps(sd->exps);
}

void pretty_var_spec(VAR_DECL vs) {
    printf("var ");
    traverse_names(vs);


    if(vs.type != NULL) {
        printf(" ");
        pretty_type(vs.type);
    }

    if(vs.vals != NULL) {
        printf(" = ");
        traverse_vals(vs);
    }

}

void traverse_names(VAR_DECL vs) {
    printf("%s", vs.name);
    if(vs.next != NULL) {
        printf(", ");
        traverse_names(vs.next);
    }
}

void traverse_vals(VAR_DECL vs) {
    printf("%s", vs.val);
    if(vs.next != NULL) {
        printf(", ");
        traverse_vals(vs.next);
    }
}

void pretty_if(IF_STMT is) {
    indentation(); printf("if ");

    // initialization
    if(is.init != NULL) {
        pretty_stmt(is.init);
        printf("; ");
    }

    // condition
    pretty_exp(is.condition);

    // body
    printf(" ");
    indentation(); printf("\n{\n"); // Asa convention (Allman style)
    tab++;
    traverse_stmts(is.body);
    tab--;
    indentation(); printf("}"); // Asa convention (Allman style)

    // optional else, else if
    if(is.elsebody != NULL) {
        printf(" else ");
        indentation(); printf("\n{\n")
        tab++;
        traverse_stmts(is.elsebody);
        tab--;
        indentation(); printf("}")
    }
}

void pretty_switch(SWITCH_STMT ss) {
    printf("switch ");

    // initialization
    if(ss.init != NULL) {
        pretty_stmt(ss.init);
        printf("; ");
    }

    // expression
    pretty_exp(ss.exp);

    // cases
    printf(" ");
    indentation(); printf("\n{\n");
    tab++;
    pretty_cases(ss.cases);
    tab--;
    indentation(); printf("}");

}

void pretty_cases(CASES cs) {
    indentation(); printf("case ");
    traverse_exps(cs.val);
    printf(":\n");
    tab++;
    traverse_stmts(cs.body);
    tab--;

    if(cs.next != NULL) {
        pretty_cases(cs.next);
    }
}

void pretty_for(FOR_STMT fs) {
    indentation(); printf("for ");

    // initialization
    if(fs.init != NULL) {
        pretty_stmt(fs.init);
        printf("; ");
    }

    // condition
    if(fs.condition != NULL) {
        pretty_exp(fs.condition);
        printf("; ");
    }

    // iter statement
    if(fs.iter != NULL) {
        pretty_stmt(fs.iter);
        printf(" ");
    }

    // body
    indentation(); printf("\n{\n"); // Asa convention (Allman style)
    tab++;
    traverse_stmts(fs.body);
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
            printf("fallthrough;");
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
